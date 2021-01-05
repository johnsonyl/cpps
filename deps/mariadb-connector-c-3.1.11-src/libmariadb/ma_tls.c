/************************************************************************************
  Copyright (C) 2014 MariaDB Corporation AB

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public
  License along with this library; if not see <http://www.gnu.org/licenses>
  or write to the Free Software Foundation, Inc.,
  51 Franklin St., Fifth Floor, Boston, MA 02110, USA

 *************************************************************************************/

/*
 * this is the abstraction layer for communication via SSL.
 * The following SSL libraries/variants are currently supported:
 * - openssl
 * - gnutls
 * - schannel (windows only)
 * 
 * Different SSL variants are implemented as plugins
 * On Windows schannel is implemented as (standard)
 * built-in plugin.
 */

#ifdef HAVE_TLS

#include <ma_global.h>
#include <ma_sys.h>
#include <ma_common.h>
#include <string.h>
#include <errmsg.h>
#include <ma_pvio.h>
#include <ma_tls.h>
#include <mysql/client_plugin.h>
#include <mariadb/ma_io.h>

#ifdef HAVE_NONBLOCK
#include <mariadb_async.h>
#include <ma_context.h>
#endif

/* Errors should be handled via pvio callback function */
my_bool ma_tls_initialized= FALSE;
unsigned int mariadb_deinitialize_ssl= 1;

const char *tls_protocol_version[]=
  {"SSLv3", "TLSv1.0", "TLSv1.1", "TLSv1.2", "TLSv1.3", "Unknown"};

MARIADB_TLS *ma_pvio_tls_init(MYSQL *mysql)
{
  MARIADB_TLS *ctls= NULL;

  if (!ma_tls_initialized)
    ma_tls_start(mysql->net.last_error, MYSQL_ERRMSG_SIZE);

  if (!(ctls= (MARIADB_TLS *)calloc(1, sizeof(MARIADB_TLS))))
  {
    return NULL;
  }

  /* register error routine and methods */
  ctls->pvio= mysql->net.pvio;
  if (!(ctls->ssl= ma_tls_init(mysql)))
  {
    free(ctls);
    ctls= NULL;
  }
  return ctls;
}

my_bool ma_pvio_tls_connect(MARIADB_TLS *ctls)
{
  my_bool rc;
  
  if ((rc= ma_tls_connect(ctls)))
    ma_tls_close(ctls);
  return rc;
}

ssize_t ma_pvio_tls_read(MARIADB_TLS *ctls, const uchar* buffer, size_t length)
{
  return ma_tls_read(ctls, buffer, length);
}

ssize_t ma_pvio_tls_write(MARIADB_TLS *ctls, const uchar* buffer, size_t length)
{
  return ma_tls_write(ctls, buffer, length);
}

my_bool ma_pvio_tls_close(MARIADB_TLS *ctls)
{
  return ma_tls_close(ctls);
}

int ma_pvio_tls_verify_server_cert(MARIADB_TLS *ctls)
{
  return ma_tls_verify_server_cert(ctls);
}

const char *ma_pvio_tls_cipher(MARIADB_TLS *ctls)
{
  return ma_tls_get_cipher(ctls);
}

void ma_pvio_tls_end()
{
  ma_tls_end();
}

int ma_pvio_tls_get_protocol_version_id(MARIADB_TLS *ctls)
{
  return ma_tls_get_protocol_version(ctls);
}

const char *ma_pvio_tls_get_protocol_version(MARIADB_TLS *ctls)
{
  int version;

  version= ma_tls_get_protocol_version(ctls);
  if (version < 0 || version > PROTOCOL_MAX)
    return tls_protocol_version[PROTOCOL_UNKNOWN];
  return tls_protocol_version[version];
}

static signed char ma_hex2int(char c)
{
  if (c >= '0' && c <= '9')
    return c - '0';
  if (c >= 'A' && c <= 'F')
    return 10 + c - 'A';
  if (c >= 'a' && c <= 'f')
    return 10 + c - 'a';
  return -1;
}

static my_bool ma_pvio_tls_compare_fp(const char *cert_fp,
                                     unsigned int cert_fp_len,
                                     const char *fp, unsigned int fp_len)
{
  char *p= (char *)fp,
       *c;

  /* check length */
  if (cert_fp_len != 20)
    return 1;

  /* We support two formats:
     2 digits hex numbers, separated by colons (length=59)
     20 * 2 digits hex numbers without separators (length = 40)
  */
  if (fp_len != (strchr(fp, ':') ? 59 : 40))
    return 1;

  for(c= (char *)cert_fp; c < cert_fp + cert_fp_len; c++)
  {
    signed char d1, d2;
    if (*p == ':')
      p++;
    if (p - fp > (int)fp_len -1)
      return 1;
    if ((d1 = ma_hex2int(*p)) == - 1 ||
        (d2 = ma_hex2int(*(p+1))) == -1 ||
        (char)(d1 * 16 + d2) != *c)
      return 1;
    p+= 2;
  }
  return 0;
}

my_bool ma_pvio_tls_check_fp(MARIADB_TLS *ctls, const char *fp, const char *fp_list)
{
  unsigned int cert_fp_len= 64;
  char *cert_fp= NULL;
  my_bool rc=1;
  MYSQL *mysql= ctls->pvio->mysql;

  cert_fp= (char *)malloc(cert_fp_len);

  if ((cert_fp_len= ma_tls_get_finger_print(ctls, cert_fp, cert_fp_len)) < 1)
    goto end;
  if (fp)
    rc= ma_pvio_tls_compare_fp(cert_fp, cert_fp_len, fp, (unsigned int)strlen(fp));
  else if (fp_list)
  {
    MA_FILE *fp;
    char buff[255];

    if (!(fp = ma_open(fp_list, "r", mysql)))
      goto end;

    while (ma_gets(buff, sizeof(buff)-1, fp))
    {
      /* remove trailing new line character */
      char *pos= strchr(buff, '\r');
      if (!pos)
        pos= strchr(buff, '\n');
      if (pos)
        *pos= '\0';
        
      if (!ma_pvio_tls_compare_fp(cert_fp, cert_fp_len, buff, (unsigned int)strlen(buff)))
      {
        /* finger print is valid: close file and exit */
        ma_close(fp);
        rc= 0;
        goto end;
      }
    }

    /* No finger print matched - close file and return error */
    ma_close(fp);
  }

end:
  if (cert_fp)
    free(cert_fp);
  if (rc)
  {
    my_set_error(mysql, CR_SSL_CONNECTION_ERROR, SQLSTATE_UNKNOWN,
                         ER(CR_SSL_CONNECTION_ERROR), 
                         "Fingerprint verification of server certificate failed");
  }
  return rc;
}
#endif /* HAVE_TLS */
