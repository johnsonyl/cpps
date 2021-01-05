/************************************************************************************
  Copyright (C) 2012 Monty Program AB

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
#if defined(WIN32) && defined(HEAP_CHECK)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include "my_test.h"
#include <ma_pthread.h>
#ifdef HAVE_OPENSSL
#include <openssl/opensslv.h>
#include <openssl/ssl.h>
#endif

#define FNLEN 4096

static int skip_ssl= 1;
static uchar have_openssl= 1;
static unsigned char have_tls13= 0;

const char *ssluser= "ssluser";
const char *sslpw= "sslpw";
char sslhost[128];
char sslcert[FNLEN];
char sslcombined[FNLEN];
char sslkey[FNLEN];
char sslkey_enc[FNLEN];
char sslca[FNLEN];
char sslcrl[FNLEN];
char ssl_cert_finger_print[129]= {0};
char bad_cert_finger_print[]= "00:11:22:33:44:55:66:77:88:99:AA:BB:CC:DD:EE:FF:01:23:45:67";

pthread_mutex_t LOCK_test;

void read_fingerprint()
{
  FILE *f= fopen(CERT_PATH "/server-cert.sha1", "r");
  if (f)
  {
    if (!fscanf(f, "%128s", ssl_cert_finger_print))
      ssl_cert_finger_print[0]= 0;
    fclose(f);
  }
}

int check_skip_ssl()
{
  const char *ssldir= NULL;
#ifndef HAVE_TLS
  diag("client library built without OpenSSL support -> skip");
  return 1;
#endif
  if (skip_ssl)
  {
    diag("server doesn't support SSL -> skip");
    return 1;
  }
  if (!(ssldir= getenv("SECURE_LOAD_PATH")))
  {
    ssldir= CERT_PATH;
    if (!strlen(ssldir))
    {
      diag("certificate directory not found");
      return 1;
    }
  }
  snprintf(sslcert, FNLEN - 1, "%s/%s", ssldir, "client-cert.pem");
  snprintf(sslcombined, FNLEN - 1, "%s/%s", ssldir, "client-certkey.pem");
  snprintf(sslkey, FNLEN - 1, "%s/%s", ssldir, "client-key.pem");
  snprintf(sslkey_enc, FNLEN - 1, "%s/%s", ssldir, "client-key-enc.pem");
  snprintf(sslca, FNLEN - 1, "%s/%s", ssldir, "cacert.pem");
  return 0;
}

static int check_cipher(MYSQL *mysql)
{
  char *cipher= (char *)mysql_get_ssl_cipher(mysql);
  if (!cipher)
    return 1;
  diag("cipher: %s", cipher);

  return 0;
}

static int create_ssl_user(const char *ssluser, my_bool is_X509)
{
  int rc;
  char query[1024];
  MYSQL *mysql= mysql_init(NULL);

  FAIL_IF(!mysql_real_connect(mysql, hostname, username, password, schema,
                         port, socketname, 0), mysql_error(mysql));

  sprintf(query, "DROP USER '%s'@'%s'", ssluser, this_host);
  rc= mysql_query(mysql, query);

  sprintf(query, "CREATE USER '%s'@'%s' IDENTIFIED BY '%s'", ssluser, this_host, sslpw);
  rc= mysql_query(mysql, query);
  check_mysql_rc(rc,mysql);

  sprintf(query, "GRANT ALL ON %s.* TO  '%s'@'%s' REQUIRE %s", schema, ssluser, this_host, is_X509 ? "X509" : "SSL");
  rc= mysql_query(mysql, query);
  check_mysql_rc(rc,mysql);
  rc= mysql_query(mysql, "FLUSH PRIVILEGES");
  check_mysql_rc(rc,mysql);

  mysql_close(mysql);

  return rc;
}

static int test_ssl(MYSQL *mysql)
{
  int rc;
  unsigned int iversion;
  MYSQL_RES *res;
  MYSQL_ROW row;
  char *tls_library;
  MYSQL *my= mysql_init(NULL);

  mysql_ssl_set(my,0, 0, 0, 0, 0);

  create_ssl_user("ssluser", 0);

  FAIL_IF(!mysql_real_connect(my, hostname, ssluser, sslpw, schema,
                         port, socketname, 0), mysql_error(my));

  mariadb_get_infov(my, MARIADB_CONNECTION_TLS_VERSION_ID, &iversion);
  diag("iversion: %d", iversion);
  if (iversion == 4)
    have_tls13= 1;

  mysql_close(my);

  rc= mysql_query(mysql, "SELECT @@have_ssl, @@have_openssl");
  check_mysql_rc(rc, mysql);

  res= mysql_store_result(mysql);
  FAIL_IF(!res, mysql_error(mysql));

  while ((row= mysql_fetch_row(res)))
  {
    if (!strcmp(row[0], "YES"))
      skip_ssl= 0;
    if (strcmp(row[1], "YES"))
      have_openssl= 0;
    diag("SSL: %s", row[0]);
  }
  mysql_free_result(res);

  /* In MySQL we need to check tls_version */
  if (!mariadb_connection(mysql))
  {
    rc= mysql_query(mysql, "select locate('v1.2', @@tls_version) > 0");
    check_mysql_rc(rc, mysql);

    res= mysql_store_result(mysql);
    FAIL_IF(!res, mysql_error(mysql));

    if ((row= mysql_fetch_row(res)))
    {
      if (row[0] && row[0][0] == '0')
        have_openssl= 0;
    }
    mysql_free_result(res);
  }
  diag("OpenSSL: %d", have_openssl);

  mariadb_get_infov(NULL, MARIADB_TLS_LIBRARY, &tls_library);
  diag("SSL library: %s", tls_library);

  sslhost[0]= 0;

  if (!skip_ssl)
  {
    char *p;

    rc= mysql_query(mysql, "SELECT CURRENT_USER()");
    check_mysql_rc(rc, mysql);
    res= mysql_store_result(mysql);
    row= mysql_fetch_row(res);
    diag("user: %s", row[0]);
    if ((p= strchr(row[0], '@')))
      strcpy(sslhost, p+1);
    mysql_free_result(res);
  }

  return OK;
}

static int test_ssl_cipher(MYSQL *unused __attribute__((unused)))
{
  MYSQL *my;
  MYSQL_RES *res;
  MYSQL_ROW row;
  int rc;
  
  if (check_skip_ssl())
    return SKIP;

  my= mysql_init(NULL);
  FAIL_IF(!my, "mysql_init() failed");

  mysql_ssl_set(my,0, 0, sslca, 0, 0);

  FAIL_IF(!mysql_real_connect(my, hostname, ssluser, sslpw, schema,
                         port, socketname, 0), mysql_error(my));

  rc= mysql_query(my, "SHOW session status like 'Ssl_version'");
  check_mysql_rc(rc, my);
  res= mysql_store_result(my);
  row= mysql_fetch_row(res);
  diag("%s: %s", row[0], row[1]);
  diag("cipher: %s", mysql_get_ssl_cipher(my));
  mysql_free_result(res);

  FAIL_IF(check_cipher(my) != 0, "Invalid cipher");
  mysql_close(my);
  return OK;
}

static int test_conc95(MYSQL *unused __attribute__((unused)))
{
  MYSQL *mysql;

  if (check_skip_ssl())
    return SKIP;

  create_ssl_user("ssluser1", 1);

  mysql= mysql_init(NULL);
  mysql_ssl_set(mysql, 
                sslkey, 
                sslcert, 
                NULL,
                NULL, 
                NULL);

  if (!mysql_real_connect(mysql, hostname, "ssluser1", sslpw, schema,
                          port, socketname, 0))
  {
    diag("could not establish x509 connection. Error: %s", mysql_error(mysql));
    mysql_close(mysql);
    return FAIL;
  }
  mysql_close(mysql);
  return OK;
}

static int test_multi_ssl_connections(MYSQL *unused __attribute__((unused)))
{
  MYSQL *mysql[50], *my;
  int i, rc;
  int old_connections= 0, new_connections= 0;
  MYSQL_RES *res;
  MYSQL_ROW row;

  if (check_skip_ssl())
    return SKIP;

  diag("Test doesn't work with yassl");
  return SKIP;

  create_ssl_user(ssluser, 0);

  my= mysql_init(NULL);
  FAIL_IF(!my,"mysql_init() failed");
  FAIL_IF(!mysql_real_connect(my, hostname, ssluser, sslpw, schema,
           port, socketname, 0), mysql_error(my));

  rc= mysql_query(my, "SHOW STATUS LIKE 'Ssl_accepts'");
  check_mysql_rc(rc, my);

  res= mysql_store_result(my);
  if ((row= mysql_fetch_row(res)))
    old_connections= atoi(row[1]);
  mysql_free_result(res);

  for (i=0; i < 50; i++)
  {
    mysql[i]= mysql_init(NULL);
    FAIL_IF(!mysql[i],"mysql_init() failed");

    mysql_ssl_set(mysql[i], 0, 0, sslca, 0, 0);

    mysql_real_connect(mysql[i], hostname, ssluser, sslpw, schema,
                         port, socketname, 0);
    if (mysql_errno(mysql[i]))
    {
      diag("loop: %d error: %d %s", i, mysql_errno(mysql[i]), mysql_error(mysql[i]));
      return FAIL;
    }

    FAIL_IF(check_cipher(mysql[i]) != 0, "Invalid cipher");
  }
  for (i=0; i < 50; i++)
    mysql_close(mysql[i]);

  rc= mysql_query(my, "SHOW STATUS LIKE 'Ssl_accepts'");
  check_mysql_rc(rc, my);

  res= mysql_store_result(my);
  if ((row= mysql_fetch_row(res)))
    new_connections= atoi(row[1]);
  mysql_free_result(res);

  mysql_close(my);

  diag("%d SSL connections processed", new_connections - old_connections);
  FAIL_IF(new_connections - old_connections < 50, "new_connections should be at least old_connections + 50");
  return OK;
}

#ifndef WIN32
static void ssl_thread(void *unused __attribute__((unused)))
#else
DWORD WINAPI ssl_thread(void *dummy)
#endif
{
  MYSQL *mysql= NULL;

  mysql_thread_init();
  
  if (!(mysql= mysql_init(NULL)))
  {
    goto end;
  }
  mysql_ssl_set(mysql, 0, 0, sslca, 0, 0);

  if(!mysql_real_connect(mysql, hostname, ssluser, sslpw, schema,
          port, socketname, 0))
  {
    diag(">Error: %s", mysql_error(mysql));
    goto end;
  }

  pthread_mutex_lock(&LOCK_test);
  mysql_query(mysql, "UPDATE ssltest SET a=a+1");
  pthread_mutex_unlock(&LOCK_test);

end:
  if(mysql)
    mysql_close(mysql);
  mysql_thread_end();
#ifdef _WIN32
  return 0;
#endif
}

static int test_ssl_threads(MYSQL *mysql)
{
  int i, rc;
#ifndef WIN32
  pthread_t threads[50];
#else
  HANDLE hthreads[50];
  DWORD dthreads[50];
#endif
  MYSQL_RES *res;
  MYSQL_ROW row;
  
  if (check_skip_ssl())
    return SKIP;

  rc= mysql_query(mysql, "DROP TABLE IF exists ssltest");
  check_mysql_rc(rc, mysql);
  rc= mysql_query(mysql, "CREATE TABLE ssltest (a int)");
  check_mysql_rc(rc, mysql);
  rc= mysql_query(mysql, "INSERT into ssltest VALUES (0)");
  check_mysql_rc(rc, mysql);
  pthread_mutex_init(&LOCK_test, NULL);

  pthread_mutex_init(&LOCK_test, NULL);

  for (i=0; i < 50; i++)
  {
#ifndef WIN32
    pthread_create(&threads[i], NULL, (void *)ssl_thread, NULL);
#else
    hthreads[i]= CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ssl_thread, NULL, 0, &dthreads[i]);
    if (hthreads[i]==NULL)
      diag("error while starting thread");
#endif
  }
  for (i=0; i < 50; i++)
#ifndef WIN32
    pthread_join(threads[i], NULL);
#else
    WaitForSingleObject(hthreads[i], INFINITE);
#endif

  pthread_mutex_destroy(&LOCK_test);

  rc= mysql_query(mysql, "SELECT a FROM ssltest");
  check_mysql_rc(rc, mysql);
  res= mysql_store_result(mysql);
  row= mysql_fetch_row(res);
  diag("Found: %s", row[0]);
  FAIL_IF(strcmp(row[0], "50") != 0, "Expected 50");
  mysql_free_result(res);
  rc= mysql_query(mysql, "DROP TABLE IF exists ssltest");
  check_mysql_rc(rc, mysql);
  return OK;
}

static int test_phpbug51647(MYSQL *unused __attribute__((unused)))
{
  MYSQL* mysql;

  if (check_skip_ssl())
    return SKIP;

  mysql= mysql_init(NULL);
  FAIL_IF(!mysql, "Can't allocate memory");

  mysql_ssl_set(mysql, sslkey, 
                       sslcert,
                       sslca, 0, 0);

  FAIL_IF(!mysql_real_connect(mysql, hostname, ssluser, sslpw, schema,
           port, socketname, 0), mysql_error(mysql));
  diag("%s", mysql_get_ssl_cipher(mysql));
  mysql_close(mysql);

  return OK;
}

static int test_password_protected(MYSQL *unused __attribute__((unused)))
{
  MYSQL* mysql;

  if (check_skip_ssl())
    return SKIP;

  mysql= mysql_init(NULL);
  FAIL_IF(!mysql, "Can't allocate memory");

  mysql_ssl_set(mysql, sslkey_enc,
                       sslcert,
                       sslca, 0, 0);

  mysql_options(mysql, MARIADB_OPT_TLS_PASSPHRASE, "qwerty");

  FAIL_IF(!mysql_real_connect(mysql, hostname, ssluser, sslpw, schema,
           port, socketname, 0), mysql_error(mysql));
  diag("%s", mysql_get_ssl_cipher(mysql));
  mysql_close(mysql);

  return OK;
}


static int test_conc50(MYSQL *unused __attribute__((unused)))
{
  MYSQL *mysql;

  if (check_skip_ssl())
    return SKIP;

  mysql= mysql_init(NULL);
  FAIL_IF(!mysql, "Can't allocate memory");

  mysql_ssl_set(mysql, NULL, NULL, "./non_exisiting_cert.pem", NULL, NULL);

  mysql_real_connect(mysql, hostname, ssluser, sslpw, schema,
           port, socketname, 0);
  diag("Error: %d %s", mysql_errno(mysql), mysql_error(mysql));
  FAIL_IF(mysql_errno(mysql) != 2026, "Expected errno 2026");
  mysql_close(mysql);

  return OK;
}

static int test_conc50_1(MYSQL *unused __attribute__((unused)))
{
  MYSQL *mysql;

  if (check_skip_ssl())
    return SKIP;

  if (!have_openssl)
  {
    diag("Server with OpenSSL required");
    return SKIP;
  }

  create_ssl_user(ssluser, 0);

  mysql= mysql_init(NULL);
  FAIL_IF(!mysql, "Can't allocate memory");

  mysql_ssl_set(mysql, NULL, NULL, sslca, NULL, NULL);

  mysql_real_connect(mysql, hostname, ssluser, sslpw, schema,
           port, socketname, 0);
  if (mysql_errno(mysql))
    diag("Error: %d %s", mysql_errno(mysql), mysql_error(mysql));
  FAIL_IF(mysql_errno(mysql), "No error expected");

  mysql_close(mysql);

  return OK;
}

static int test_conc50_2(MYSQL *unused __attribute__((unused)))
{
  MYSQL *mysql;

  if (check_skip_ssl())
    return SKIP;

  mysql= mysql_init(NULL);
  FAIL_IF(!mysql, "Can't allocate memory");

  mysql_ssl_set(mysql, NULL, NULL, "./non_exisiting_cert.pem", NULL, NULL);

  mysql_real_connect(mysql, hostname, ssluser, sslpw, schema,
           port, socketname, 0);
  FAIL_IF(mysql_errno(mysql) != 2026, "Expected errno 2026");
  mysql_close(mysql);

  return OK;
}

static int test_conc127(MYSQL *unused __attribute__((unused)))
{
  MYSQL *mysql;

  diag("test disabled - for testing disable other tests or run this test as first test");
  return SKIP;

  if (check_skip_ssl())
    return SKIP;

  mysql= mysql_init(NULL);
  FAIL_IF(!mysql, "Can't allocate memory");

  mysql_ssl_set(mysql, NULL, NULL, "./non_exisiting.pem", NULL, NULL);

  mysql_real_connect(mysql, hostname, ssluser, sslpw, schema,
           port, socketname, 0);
  diag("Error: %s", mysql_error(mysql));
  FAIL_IF(mysql_errno(mysql) == 0, "Error expected (invalid certificate)");
  mysql_close(mysql);

  return OK;
}

static int test_conc50_3(MYSQL *unused __attribute__((unused)))
{
  MYSQL *mysql;

  if (check_skip_ssl())
    return SKIP;

  create_ssl_user(ssluser, 0);

  mysql= mysql_init(NULL);
  FAIL_IF(!mysql, "Can't allocate memory");

  mysql_real_connect(mysql, hostname, ssluser, sslpw, schema,
           port, socketname, 0);
  FAIL_IF(!mysql_errno(mysql), "Error expected, SSL connection required!");
  mysql_close(mysql);

  mysql= mysql_init(NULL);
  FAIL_IF(!mysql, "Can't allocate memory");

  mysql_ssl_set(mysql, NULL, NULL, sslca, NULL, NULL);

  mysql_real_connect(mysql, hostname, ssluser, sslpw, schema,
           port, socketname, 0);
  diag("Error: %s<", mysql_error(mysql));
  FAIL_IF(mysql_errno(mysql), "No error expected");
  mysql_close(mysql);

  return OK;
}

static int test_conc50_4(MYSQL *unused __attribute__((unused)))
{
  MYSQL *mysql;

  if (check_skip_ssl())
    return SKIP;

  mysql= mysql_init(NULL);
  FAIL_IF(!mysql, "Can't allocate memory");

  mysql_ssl_set(mysql, NULL, sslca, NULL, NULL, NULL);

  mysql_real_connect(mysql, hostname, ssluser, sslpw, schema,
           port, socketname, 0);
  FAIL_IF(!mysql_errno(mysql) , "Error expected");
  mysql_close(mysql);

  return OK;
}

static int verify_ssl_server_cert(MYSQL *unused __attribute__((unused)))
{
  MYSQL *mysql;
  uint verify= 1;

  if (check_skip_ssl())
    return SKIP;

  if (!hostname || !strcmp(hostname, "localhost"))
    return SKIP;

  SKIP_TRAVIS();

  mysql= mysql_init(NULL);
  FAIL_IF(!mysql, "Can't allocate memory");

  mysql_ssl_set(mysql, NULL, NULL, sslca, NULL, NULL);
  mysql_options(mysql, MYSQL_OPT_SSL_VERIFY_SERVER_CERT, &verify);

  mysql_real_connect(mysql, hostname, ssluser, sslpw, schema,
           port, socketname, 0);

  FAIL_IF(!mysql_errno(mysql), "Expected error");
  diag("Error (expected): %s", mysql_error(mysql));
  mysql_close(mysql);

  return OK;
}

static int test_bug62743(MYSQL *unused __attribute__((unused)))
{
  MYSQL *mysql;

  if (check_skip_ssl())
    return SKIP;

  mysql= mysql_init(NULL);
  FAIL_IF(!mysql, "Can't allocate memory");

  mysql_ssl_set(mysql, "dummykey", NULL, NULL, NULL, NULL);

  mysql_real_connect(mysql, hostname, ssluser, sslpw, schema,
           port, socketname, 0);
  diag("Error: %s", mysql_error(mysql));
  FAIL_IF(mysql_errno(mysql) != 2026, "Expected errno 2026");
  mysql_close(mysql);

  mysql= mysql_init(NULL);
  FAIL_IF(!mysql, "Can't allocate memory");

  mysql_ssl_set(mysql, sslkey, NULL, NULL, NULL, NULL);

  mysql_real_connect(mysql, hostname, ssluser, sslpw, schema,
           port, socketname, 0);
  diag("Error with key: %s", mysql_error(mysql));
  FAIL_IF(mysql_errno(mysql) != 2026, "Expected errno 2026");
  mysql_close(mysql);

  mysql= mysql_init(NULL);
  FAIL_IF(!mysql, "Can't allocate memory");

  mysql_ssl_set(mysql, sslkey,
                       sslcert, NULL, NULL, NULL);

  mysql_real_connect(mysql, hostname, ssluser, sslpw, schema,
           port, socketname, 0);
  FAIL_IF(mysql_errno(mysql) != 0, "Expected no error");
  mysql_close(mysql);

  mysql= mysql_init(NULL);
  FAIL_IF(!mysql, "Can't allocate memory");

  mysql_ssl_set(mysql, sslkey, "blablubb", NULL, NULL, NULL);

  mysql_real_connect(mysql, hostname, ssluser, sslpw, schema,
           port, socketname, 0);
  diag("Error with cert: %s", mysql_error(mysql));
  FAIL_IF(mysql_errno(mysql) == 0, "Expected error");
  mysql_close(mysql);

  return OK;
}

#ifndef WIN32
int thread_conc102(void)
#else
DWORD WINAPI thread_conc102(void)
#endif
{
  MYSQL *mysql;
  int rc;
  MYSQL_RES *res;
  mysql_thread_init();
  mysql= mysql_init(NULL);

  mysql_ssl_set(mysql, sslkey,
                       sslcert,
                       sslca,
                        NULL, NULL);
  mysql_ssl_set(mysql,0, 0, sslca, 0, 0);

  if(!mysql_real_connect(mysql, hostname, username, password, schema,
          port, socketname, 0))
  {
    diag(">Error: %s", mysql_error(mysql));
    goto end;
  }
  if (!mysql_get_ssl_cipher(mysql))
  {
    diag("Error: No ssl connection");
    goto end;
  }
  pthread_mutex_lock(&LOCK_test);
  rc= mysql_query(mysql, "UPDATE t_conc102 SET a=a+1");
  check_mysql_rc(rc, mysql);
  pthread_mutex_unlock(&LOCK_test);
  check_mysql_rc(rc, mysql);
  if ((res= mysql_store_result(mysql)))
    mysql_free_result(res);
end:
  mysql_close(mysql);
  mysql_thread_end();
  return 0;
}

static int test_conc_102(MYSQL *mysql)
{

  int rc;
  int i;
  MYSQL_ROW row;
  MYSQL_RES *res;
#ifndef WIN32
  pthread_t threads[50];
#else
  HANDLE hthreads[50];
  DWORD threads[50];
#endif

  if (check_skip_ssl())
    return SKIP;

  rc= mysql_query(mysql, "DROP TABLE IF EXISTS t_conc102");
  check_mysql_rc(rc, mysql);
  rc= mysql_query(mysql, "CREATE TABLE t_conc102 ( a int)");
  check_mysql_rc(rc, mysql);
  rc= mysql_query(mysql, "INSERT INTO t_conc102 VALUES (0)");
  check_mysql_rc(rc, mysql);
  pthread_mutex_init(&LOCK_test, NULL);

  for (i=0; i < 50; i++)
  {
#ifndef WIN32
    pthread_create(&threads[i], NULL, (void *)thread_conc102, NULL);
#else
    hthreads[i]= CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread_conc102, NULL, 0, &threads[i]);
    if (hthreads[i]==NULL)
      diag("error while starting thread");
#endif
  }
  for (i=0; i < 50; i++)
  {
#ifndef WIN32
    pthread_join(threads[i], NULL);
#else
    WaitForSingleObject(hthreads[i], INFINITE);
#endif
  } 
  pthread_mutex_destroy(&LOCK_test);
  rc= mysql_query(mysql, "SELECT a FROM t_conc102");
  check_mysql_rc(rc, mysql);
  res= mysql_store_result(mysql);
  row= mysql_fetch_row(res);
  diag("Found: %s", row[0]);
  FAIL_IF(strcmp(row[0], "50") != 0, "Expected 50");
  mysql_free_result(res);
  rc= mysql_query(mysql, "DROP TABLE IF EXISTS t_conc102");
  check_mysql_rc(rc, mysql);
  return OK;
}

static int test_ssl_fp(MYSQL *unused __attribute__((unused)))
{
  MYSQL *my;
  MYSQL_RES *res;
  MYSQL_ROW row;
  int rc;
  
  if (check_skip_ssl())
    return SKIP;

  my= mysql_init(NULL);
  FAIL_IF(!my, "mysql_init() failed");

  mysql_ssl_set(my,0, 0, sslca, 0, 0);

  mysql_options(my, MARIADB_OPT_SSL_FP, bad_cert_finger_print);

  FAIL_IF(mysql_real_connect(my, hostname, username, password, schema,
                             port, socketname, 0), mysql_error(my));

  mysql_options(my, MARIADB_OPT_SSL_FP, ssl_cert_finger_print);

  FAIL_IF(!mysql_real_connect(my, hostname, username, password, schema,
                         port, socketname, 0), mysql_error(my));
  
  FAIL_IF(check_cipher(my) != 0, "Invalid cipher");

  rc= mysql_query(my, "SET @a:=1");
  check_mysql_rc(rc, my);

  rc= mysql_query(my, "SELECT @a");
  check_mysql_rc(rc, my);

  if ((res= mysql_store_result(my)))
  {
    row= mysql_fetch_row(res);
    diag("@a:=%s", row[0]);
    mysql_free_result(res);
  }

  mysql_close(my);
  return OK;
}

static int test_ssl_fp_list(MYSQL *unused __attribute__((unused)))
{
  MYSQL *my;

  if (check_skip_ssl())
    return SKIP;

  my= mysql_init(NULL);
  FAIL_IF(!my, "mysql_init() failed");

  mysql_ssl_set(my,0, 0, sslca, 0, 0);

  mysql_options(my, MARIADB_OPT_SSL_FP_LIST, CERT_PATH "/server-cert.sha1");

  if(!mysql_real_connect(my, hostname, username, password, schema,
                         port, socketname, 0))
  {
    diag("Error: %s", mysql_error(my));
    mysql_close(my);
    return FAIL;
  }

  FAIL_IF(check_cipher(my) != 0, "Invalid cipher");
  mysql_close(my);
  return OK;
}

static int test_ssl_version(MYSQL *unused __attribute__((unused)))
{
  unsigned int iversion;
  char *version, *library;
  MYSQL *my;

  if (check_skip_ssl())
    return SKIP;

  my= mysql_init(NULL);
  FAIL_IF(!my, "mysql_init() failed");

  mysql_ssl_set(my,0, 0, sslca, 0, 0);
  FAIL_IF(!mysql_real_connect(my, hostname, ssluser, sslpw, schema,
                         port, socketname, 0), mysql_error(my));

  diag("cipher: %s", mysql_get_ssl_cipher(my));
  mariadb_get_infov(my, MARIADB_CONNECTION_TLS_VERSION_ID, &iversion);
  diag("protocol: %d", iversion);
  mariadb_get_infov(my, MARIADB_CONNECTION_TLS_VERSION, &version);
  diag("protocol: %s", version);

  mariadb_get_infov(my, MARIADB_TLS_LIBRARY, &library);
  diag("library: %s", library);

  mysql_close(my);

  return OK;
}

#ifdef HAVE_SCHANNEL
static int test_schannel_cipher(MYSQL *unused __attribute__((unused)))
{
  MYSQL *my;
  unsigned int cipher_strength= 256;

  if (check_skip_ssl())
    return SKIP;

  my= mysql_init(NULL);
  FAIL_IF(!my, "mysql_init() failed");

  mysql_ssl_set(my,0, 0, sslca, 0, 0);
  mysql_options(my, MARIADB_OPT_TLS_CIPHER_STRENGTH, &cipher_strength);
  FAIL_IF(!mysql_real_connect(my, hostname, ssluser, sslpw, schema,
                         port, socketname, 0), mysql_error(my));

  diag("cipher: %s", mysql_get_ssl_cipher(my));

  mysql_close(my);

  return OK;
}

#endif

#if defined(HAVE_GNUTLS) || defined(HAVE_OPENSSL)

static int test_cipher_mapping(MYSQL *unused __attribute__((unused)))
{
  unsigned int i=0;
  const char *ciphers[]= { "DHE-RSA-AES256-GCM-SHA384", "DHE-RSA-AES256-SHA256", "DHE-RSA-AES256-SHA",
#ifdef TEST_CAMELLIA_CIPHER
                           "DHE-RSA-CAMELLIA256-SHA", "CAMELLIA256-SHA",
                           "DHE-RSA-CAMELLIA128-SHA", "CAMELLIA128-SHA",
#endif
#ifdef TEST_DES_CIPHER
                           "EDH-RSA-DES-CBC3-SHA",
                           "DES-CBC3-SHA",
#endif
                           "AES256-GCM-SHA384", "AES256-SHA256", "AES256-SHA",
                           "DHE-RSA-AES128-GCM-SHA256", "DHE-RSA-AES128-SHA256", "DHE-RSA-AES128-SHA",
                           "AES128-GCM-SHA256", "AES128-SHA256", "AES128-SHA",
                           "DHE-RSA-AES256-SHA", "AES256-SHA",
                           NULL };

  diag("This test depends on OpenSSL version - since several ciphers might not be available");
  return SKIP;

  if (check_skip_ssl())
    return SKIP;

  if (!have_openssl)
  {
    diag("test requires Server with OpenSSL");
    return SKIP;
  }

  while (ciphers[i] != NULL)
  {
    MYSQL *mysql= mysql_init(NULL);
    MYSQL_ROW row;
    MYSQL_RES *res;
    char c[100];
    int rc;
    const char *cipher;

    mysql_options(mysql, MYSQL_OPT_TLS_VERSION, "TLSv1.0,TLSv1.1,TLSv1.2");
    mysql_ssl_set(mysql, NULL, NULL, NULL, NULL, ciphers[i]);
    diag("%s", ciphers[i]);
    
    mysql->options.use_ssl= 1;
    FAIL_IF(!mysql_real_connect(mysql, hostname, username, password, schema,
                         port, socketname, 0), mysql_error(mysql));
    if (!(cipher= mysql_get_ssl_cipher(mysql)) ||
        strcmp(ciphers[i], cipher) != 0)
    {
      diag("cipher %s differs: (%s)", ciphers[i], cipher);
      mysql_close(mysql);
      goto cont;
    }
    else
    {
      rc= mysql_query(mysql, "SHOW STATUS LIKE 'ssl_cipher'");
      check_mysql_rc(rc, mysql);
      res= mysql_store_result(mysql);
      row= mysql_fetch_row(res);
      strcpy(c, row[1]);
      mysql_free_result(res);
      mysql_close(mysql);
      if (strcmp(ciphers[i], c) != 0)
      {
        diag("expected: %s instead of %s", ciphers[i], c);
        /* depending if server supports ECC, ciphers may differ,
           so we don't return failure here */
      }
    }
cont:
    i++;
  }
  return OK;
}
#endif  

static int test_openssl_1(MYSQL *mysql)
{
  int rc;
  MYSQL *my;
  uchar val= 1;
  char query[1024];
  int i;

  if (check_skip_ssl())
    return SKIP;

  if (have_tls13)
    return SKIP;

  if (!mariadb_connection(mysql))
    return SKIP;

  for (i=1; i < 6; i++)
  {
    sprintf(query, "DROP USER 'ssluser%d'@'%s'", i, this_host);
    rc= mysql_query(mysql, query);
    sprintf(query, "CREATE USER 'ssluser%d'@'%s'", i, this_host);
    rc= mysql_query(mysql, query);
    check_mysql_rc(rc, mysql);
  }
  rc= mysql_query(mysql, "FLUSH PRIVILEGES");
  check_mysql_rc(rc, mysql);
  diag("sslusers created");

  diag("ssluser1");
  sprintf(query, "grant select on %s.* to 'ssluser1'@'%s' require ssl", schema, this_host);
  rc= mysql_query(mysql, query);
  check_mysql_rc(rc, mysql);


  my= mysql_init(NULL);
  mysql_ssl_set(my, NULL, NULL, NULL, NULL, "AES128-SHA");
  FAIL_IF(!mysql_real_connect(my, hostname, "ssluser1", NULL, schema,
                         port, socketname, 0), mysql_error(my));
  FAIL_IF(!mysql_get_ssl_cipher(my), "No TLS connection");
  mysql_close(my);

  my= mysql_init(NULL);
  mysql_options(my, MYSQL_OPT_SSL_ENFORCE, &val);
  FAIL_IF(!mysql_real_connect(my, hostname, "ssluser1", NULL, schema,
                         port, socketname, 0), mysql_error(my));
  FAIL_IF(!mysql_get_ssl_cipher(my), "No TLS connection");
  mysql_close(my);

  diag("ssluser2");
  sprintf(query, "grant select on %s.* to 'ssluser2'@'%s' require cipher 'AES256-SHA'", schema, this_host);
  rc= mysql_query(mysql, query);
  check_mysql_rc(rc, mysql);

#ifdef TEST_RANDOM_RESULT
/* ssl_user2: connect with enforce should work */
  my= mysql_init(NULL);
  mysql_options(my, MYSQL_OPT_SSL_ENFORCE, &val);
  mysql_real_connect(my, hostname, "ssluser2", NULL, schema,
                         port, socketname, 0);
  if (!mysql_error(my) &&
       strcmp(mysql_get_ssl_cipher(my), "AES256-SHA"))
  {
    diag("Expected error or correct cipher");
    return FAIL;
  }
  mysql_close(my);
#endif
  /* ssl_user2: connect with correct cipher */
  diag("ssluser2");
  if (mysql_get_server_version(mysql) >= 100100)
  {
    my= mysql_init(NULL);
    mysql_ssl_set(my, NULL, NULL, NULL, NULL, "AES256-SHA");
    FAIL_IF(!mysql_real_connect(my, hostname, "ssluser2", NULL, schema,
                           port, socketname, 0), mysql_error(my));
    FAIL_IF(strcmp("AES256-SHA", mysql_get_ssl_cipher(my)) != 0, "expected cipher AES256-SHA");
    mysql_close(my);
  }

  /* ssl_user2: connect with wrong cipher should not work */
  diag("ssluser2");
  my= mysql_init(NULL);
  mysql_ssl_set(my, NULL, NULL, NULL, NULL, "AES128-SHA");
  FAIL_IF(mysql_real_connect(my, hostname, "ssluser2", NULL, schema,
                         port, socketname, 0), "Error expected");
  mysql_close(my);


  if (!travis_test)
  {
    sprintf(query, "grant select on %s.* to 'ssluser3'@'%s' require cipher 'AES256-SHA' AND "
                 " SUBJECT '/C=FI/ST=Helsinki/L=Helsinki/O=MariaDB/CN=client'", schema, this_host);
    rc= mysql_query(mysql, query);
    check_mysql_rc(rc, mysql);

    /* ssluser3: connect with cipher only */
    my= mysql_init(NULL);
    mysql_ssl_set(my, NULL, NULL, NULL, NULL, "AES256-SHA");
    FAIL_IF(mysql_real_connect(my, hostname, "ssluser3", NULL, schema,
                               port, socketname, 0), "Error expected");
    mysql_close(my);

    /* ssluser3 connect with cipher and certs */
    my= mysql_init(NULL);
    mysql_ssl_set(my, sslkey, 
                  sslcert, 
                  sslca, 
                  NULL, 
                  "AES256-SHA");
    FAIL_IF(!mysql_real_connect(my, hostname, "ssluser3", NULL, schema,
                           port, socketname, 0), mysql_error(my));

    mysql_close(my);

    sprintf(query, "grant select on %s.* to 'ssluser4'@'%s' require cipher 'AES256-SHA' AND "
                   " ISSUER '/CN=cacert/C=FI/ST=Helsinki/L=Helsinki/O=MariaDB'", schema, this_host);
    rc= mysql_query(mysql, query);
    check_mysql_rc(rc, mysql);

    /* ssluser4: connect with cipher only */
    my= mysql_init(NULL);
    mysql_ssl_set(my, NULL, NULL, NULL, NULL, "AES256-SHA");
    FAIL_IF(mysql_real_connect(my, hostname, "ssluser4", NULL, schema,
                           port, socketname, 0), "Error expected");
    mysql_close(my);

    /* ssluser4 connect with cipher and certs */
    my= mysql_init(NULL);
    mysql_ssl_set(my, sslkey,
                  sslcert,
                  sslca,
                  NULL,
                  "AES256-SHA");
    FAIL_IF(!mysql_real_connect(my, hostname, "ssluser4", NULL, schema,
                           port, socketname, 0), mysql_error(my));
    mysql_close(my);
  }
  diag("drop users");
  for (i=1; i < 6; i++)
  {
    sprintf(query, "DROP USER 'ssluser%d'@'%s'", i, this_host);
    rc= mysql_query(mysql, query);
  }

  return OK;
}

static int test_ssl_timeout(MYSQL *unused __attribute__((unused)))
{
  MYSQL *mysql;
  my_bool enforce= 1;
  int read_timeout= 1;
  int rc;

  if (check_skip_ssl())
    return SKIP;

  mysql= mysql_init(NULL);
  mysql_options(mysql, MYSQL_OPT_SSL_ENFORCE, &enforce);
  mysql_options(mysql, MYSQL_OPT_READ_TIMEOUT, &read_timeout);
  mysql->options.use_ssl= 1;
  FAIL_IF(!mysql_real_connect(mysql, hostname, username, password, schema,
                         port, socketname, 0), mysql_error(mysql));
  diag("cipher: %s\n", mysql_get_ssl_cipher(mysql));
  rc= mysql_query(mysql, "SELECT SLEEP(600)");
  if (!rc)
  {
    diag("error expected (timeout)");
    return FAIL;
  }

  mysql_close(mysql);
  return OK;
}

static int drop_ssl_user(MYSQL *mysql)
{
  int rc;

  rc= mysql_query(mysql, "DELETE FROM mysql.user where user like 'ssl%'");
  check_mysql_rc(rc, mysql);
  rc= mysql_query(mysql, "DELETE FROM mysql.db where user like 'ssl%'");
  check_mysql_rc(rc, mysql);
  return OK;
}

static int test_conc286(MYSQL *unused __attribute__((unused)))
{
  MYSQL *my;

  if (check_skip_ssl())
    return SKIP;

  my= mysql_init(NULL);
  FAIL_IF(!my, "mysql_init() failed");

  mysql_options(my, MARIADB_OPT_SSL_FP, ssl_cert_finger_print);

  FAIL_IF(!mysql_real_connect(my, hostname, username, password, schema,
                         port, socketname, 0), mysql_error(my));
  
  FAIL_IF(check_cipher(my) != 0, "Invalid cipher");

  mysql_close(my);
  return OK;
}

static int test_mdev14027(MYSQL *mysql __attribute__((unused)))
{
  char *tls_library;
  const char *check_library=
#if defined(HAVE_OPENSSL)
#if defined(HAVE_LIBRESSL)
    "LibreSSL";
#else
    "OpenSSL";
#endif
#elif defined(HAVE_GNUTLS)
    "GnuTLS";
#elif defined(HAVE_SCHANNEL)
    "Schannel";
#else
    "Off";
#endif
  mariadb_get_infov(NULL, MARIADB_TLS_LIBRARY, &tls_library);
  diag("TLS/SSL library in use: %s\n", tls_library);
  if (!strstr(tls_library, check_library))
  {
    diag("expected %s, got %s", check_library, tls_library);
    return FAIL;
  }
  return OK;
}

static int test_mdev14101(MYSQL *my __attribute__((unused)))
{
  struct {
    bool do_yassl;
    const char *opt_tls_version;
    const char *expected;
  } combinations[]= {
    {1, "TLSv1.1", "TLSv1.1"},
    {1, "TLSv1,TLSv1.1", "TLSv1.1"},
    {0, "TLSv1.2", "TLSv1.2"},
    {0, "TLSv1.1,TLSv1.2", "TLSv1.2"},
    {1, NULL, NULL}
  };

  int i;
#ifdef HAVE_SCHANNEL
  bool skip_tlsv12= 1;
#else
  bool skip_tlsv12= !have_openssl;
#endif

#if defined(HAVE_OPENSSL) && defined(TLS1_3_VERSION)
  diag("Test fails with TLS v1.3");
  return(SKIP);
#endif

  for (i=0; combinations[i].expected; i++)
  {
    MYSQL *mysql;
    bool val=1;
    char *tls_version;

    if (!combinations[i].do_yassl && skip_tlsv12)
      break;

    diag("combination %d: %s", i, combinations[i].opt_tls_version);

    mysql= mysql_init(NULL);
    mysql_options(mysql, MYSQL_OPT_SSL_ENFORCE, &val);
    mysql_options(mysql, MARIADB_OPT_TLS_VERSION, combinations[i].opt_tls_version);
    FAIL_IF(!mysql_real_connect(mysql, hostname, username, password, schema,
                         port, socketname, 0), mysql_error(mysql));
    mariadb_get_infov(mysql, MARIADB_CONNECTION_TLS_VERSION, &tls_version);
    diag("options: %s", combinations[i].opt_tls_version);
    diag("protocol: %s expected: %s", tls_version, combinations[i].expected);
    FAIL_IF(strcmp(combinations[i].expected, tls_version), "Wrong tls_version");
    mysql_close(mysql);
  }
  return OK;
}

static int test_conc386(MYSQL *mysql)
{
  mysql= mysql_init(NULL);
  mysql_ssl_set(mysql,
                sslcombined,
                NULL,
                NULL,
                NULL,
                NULL);
  FAIL_IF(!mysql_real_connect(mysql, hostname, username, password, schema,
                         port, socketname, 0), mysql_error(mysql));
  FAIL_IF(check_cipher(mysql) != 0, "Invalid cipher");
  mysql_close(mysql);
  return OK;
}

#ifndef HAVE_SCHANNEL
static int test_ssl_verify(MYSQL *my __attribute__((unused)))
{
  MYSQL *mysql;
  my_bool verify= 1, enforce= 1;

  if (check_skip_ssl())
    return SKIP;

  /* verify, using system ca should fail with self signed certificate */
  mysql= mysql_init(NULL);
  mysql_options(mysql, MYSQL_OPT_SSL_ENFORCE, &enforce);
  mysql_options(mysql, MYSQL_OPT_SSL_VERIFY_SERVER_CERT, &verify);
  FAIL_IF(mysql_real_connect(mysql, hostname, username, password, schema,
                         port, socketname, 0), "Error expected");
  diag("error expected: %s\n", mysql_error(mysql));
  mysql_close(mysql);

  /* verify, using system ca should pass */

  /* Disable this for now, since for some unknown reason it fails on travis
  setenv("SSL_CERT_DIR", CERT_PATH, 1);
  mysql= mysql_init(NULL);
  mysql_options(mysql, MYSQL_OPT_SSL_ENFORCE, &enforce);
  mysql_options(mysql, MYSQL_OPT_SSL_VERIFY_SERVER_CERT, &verify);
  FAIL_IF(!mysql_real_connect(mysql, hostname, username, password, schema,
                         port, socketname, 0), mysql_error(mysql));
  mysql_close(mysql);
  unsetenv("SSL_CERT_DIR");
  */

  /* verify against local ca, this should pass */
  mysql= mysql_init(NULL);
  mysql_ssl_set(mysql,0, 0, sslca, 0, 0);
  mysql_options(mysql, MYSQL_OPT_SSL_VERIFY_SERVER_CERT, &verify);
  FAIL_IF(!mysql_real_connect(mysql, hostname, username, password, schema,
                         port, socketname, 0), mysql_error(mysql));
  mysql_close(mysql);

  mysql= mysql_init(NULL);
  mysql_options(mysql, MYSQL_OPT_SSL_ENFORCE, &enforce);
  FAIL_IF(!mysql_real_connect(mysql, hostname, username, password, schema,
                         port, socketname, 0), mysql_error(mysql));

  diag("cipher: %s", mysql_get_ssl_cipher(mysql));
  mysql_close(mysql);
  return OK;
}
#endif

struct my_tests_st my_tests[] = {
  {"test_ssl", test_ssl, TEST_CONNECTION_NEW, 0,  NULL,  NULL},
#ifndef HAVE_SCHANNEL
  {"test_ssl_verify", test_ssl_verify, TEST_CONNECTION_NEW, 0,  NULL,  NULL},
#endif
  {"test_mdev14101", test_mdev14101, TEST_CONNECTION_NEW, 0,  NULL,  NULL},
  {"test_mdev14027", test_mdev14027, TEST_CONNECTION_NEW, 0,  NULL,  NULL},
  {"test_conc286", test_conc286, TEST_CONNECTION_NEW, 0,  NULL,  NULL},
  {"test_ssl_timeout", test_ssl_timeout, TEST_CONNECTION_NEW, 0,  NULL,  NULL},
  {"test_openssl_1", test_openssl_1, TEST_CONNECTION_NEW, 0,  NULL,  NULL},
#ifndef HAVE_SCHANNEL
  {"test_cipher_mapping", test_cipher_mapping, TEST_CONNECTION_NONE, 0,  NULL,  NULL},
#endif
  {"test_conc127", test_conc127, TEST_CONNECTION_NEW, 0,  NULL,  NULL},
/* Both tests work with GNU tls, however we can't create fingerprints with
   gnutls-cli in CMakeLists.txt */
#ifndef HAVE_SCHANNEL
  {"test_ssl_fp", test_ssl_fp, TEST_CONNECTION_NEW, 0,  NULL,  NULL},
  {"test_ssl_fp_list", test_ssl_fp_list, TEST_CONNECTION_NEW, 0,  NULL,  NULL},
#endif
  {"test_conc50", test_conc50, TEST_CONNECTION_NEW, 0,  NULL,  NULL},
  {"test_conc50_1", test_conc50_1, TEST_CONNECTION_NEW, 0,  NULL,  NULL},
  {"test_conc50_2", test_conc50_2, TEST_CONNECTION_NEW, 0,  NULL,  NULL},
  {"test_conc50_3", test_conc50_3, TEST_CONNECTION_NEW, 0,  NULL,  NULL},
  {"test_conc50_4", test_conc50_4, TEST_CONNECTION_NEW, 0,  NULL,  NULL},
  {"test_conc95", test_conc95, TEST_CONNECTION_NEW, 0,  NULL,  NULL},
  {"verify_ssl_server_cert", verify_ssl_server_cert, TEST_CONNECTION_NEW, 0,  NULL,  NULL},
  {"test_bug62743", test_bug62743, TEST_CONNECTION_NEW, 0,  NULL,  NULL},
  {"test_phpbug51647", test_phpbug51647, TEST_CONNECTION_NONE, 0, NULL, NULL},
  {"test_ssl_cipher", test_ssl_cipher, TEST_CONNECTION_NONE, 0,  NULL,  NULL},
  {"test_multi_ssl_connections", test_multi_ssl_connections, TEST_CONNECTION_NONE, 0,  NULL,  NULL},
  {"test_conc_102", test_conc_102, TEST_CONNECTION_NEW, 0, NULL, NULL},
  {"test_ssl_version", test_ssl_version, TEST_CONNECTION_NEW, 0, NULL, NULL},
  {"test_ssl_threads", test_ssl_threads, TEST_CONNECTION_NEW, 0,  NULL,  NULL},
#ifndef HAVE_SCHANNEL
  {"test_password_protected", test_password_protected, TEST_CONNECTION_NEW, 0,  NULL,  NULL},
#else
  {"test_schannel_cipher", test_schannel_cipher, TEST_CONNECTION_NEW, 0,  NULL,  NULL},
#endif
  {"test_conc386", test_conc386, TEST_CONNECTION_NEW, 0,  NULL,  NULL},
  {"drop_ssl_user", drop_ssl_user, TEST_CONNECTION_NEW, 0,  NULL,  NULL},
  {NULL, NULL, 0, 0, NULL, NULL}
};


int main(int argc, char **argv)
{
#if defined(WIN32) && defined(HEAP_CHECK)
   _CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_FILE );
   _CrtSetReportFile( _CRT_WARN, _CRTDBG_FILE_STDOUT );
   _CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_FILE );
   _CrtSetReportFile( _CRT_ERROR, _CRTDBG_FILE_STDOUT );
   _CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_FILE );
   _CrtSetReportFile( _CRT_ASSERT, _CRTDBG_FILE_STDOUT );
#endif

  get_envvars();
  read_fingerprint();

  if (argc > 1)
    get_options(argc, argv);
  run_tests(my_tests);

  mysql_server_end();
#if defined(WIN32) && defined(HEAP_CHECK)
  _CrtDumpMemoryLeaks();
#endif
  return(exit_status());
}

