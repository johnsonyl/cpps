
#include <cpps/cpps.h>
#include <iostream>
#include <string>
#include <fstream>



#include <openssl/err.h>
#include <openssl/ossl_typ.h>
#include <openssl/engine.h>
#include <openssl/evp.h>
#include <openssl/x509v3.h>
#include <openssl/ssl.h>
#include <openssl/pkcs12.h>
#include <openssl/pkcs7.h>
#include <openssl/hmac.h>
#include <openssl/rand.h>
#include <openssl/conf.h>
#include <openssl/conf_api.h>

#define ALLOW_SSL_ERROR_PRINTF (0)
#if ALLOW_SSL_ERROR_PRINTF
SSL_load_error_strings();
#endif


#if ALLOW_SSL_ERROR_PRINTF
ERR_print_errors_fp(stderr);
#endif


#if ALLOW_SSL_ERROR_PRINTF
ERR_free_strings();
#endif

using namespace cpps;
using namespace std;

// 公钥解密    
std::string rsa_pub_decrypt(std::string cipherText, std::string pubKey)
{
	std::string strRet;
	BIO* keybio = BIO_new_mem_buf((unsigned char*)pubKey.c_str(), -1);
	RSA* rsa = RSA_new();
	rsa = PEM_read_bio_RSAPublicKey(keybio, &rsa, NULL, NULL);
	if (!rsa)
	{
		ERR_load_crypto_strings();
		char errBuf[512];
		ERR_error_string_n(ERR_get_error(), errBuf, sizeof(errBuf));
		std::cout << "load key failed[" << errBuf << "]" << std::endl;
		BIO_free_all(keybio);
		return std::string("");
	}

	size_t len = (size_t)RSA_size(rsa);
	char* decryptedText = (char*)malloc(len + 1);
	memset(decryptedText, 0, len + 1);

	// 解密函数  
	int ret = RSA_public_decrypt((int32)cipherText.length(), (const unsigned char*)cipherText.c_str(), (unsigned char*)decryptedText, rsa,(size_t) RSA_PKCS1_PADDING);
	if (ret >= 0)
		strRet.assign(decryptedText, ret);
	else {
		ERR_load_crypto_strings();
		char err[512];

		ERR_error_string(ERR_get_error(), err);
		fprintf(stderr, "%d,Error decrypting message: %s\n", (int32)cipherText.size(),err);

		printf("err : %d, %s", ret, decryptedText);
	}
	// 释放内存  
	free(decryptedText);
	BIO_free_all(keybio);
	RSA_free(rsa);

	return strRet;
}

std::string rsa_pub_encrypt(std::string clearText, std::string pubKey)
{
	std::string strRet;
 	BIO* keybio = BIO_new_mem_buf((unsigned char *)pubKey.c_str(), -1);
	RSA* rsa = RSA_new();
	rsa = PEM_read_bio_RSAPublicKey(keybio, &rsa, NULL, NULL);
	if (!rsa)
	{
#ifdef _DEBUG
		ERR_load_crypto_strings();
		char errBuf[512];
		ERR_error_string_n(ERR_get_error(), errBuf, sizeof(errBuf));
		std::cout << "load key failed[" << errBuf << "]" << std::endl;
#endif
		BIO_free_all(keybio);
		return std::string("");
	}

	size_t len = (size_t)RSA_size(rsa);
	//int len = 1028;
	char* encryptedText = (char*)malloc(len + 1);
	memset(encryptedText, 0, len + 1);

	// 加密函数  
	int ret = RSA_public_encrypt((int32)clearText.length(), (const unsigned char*)clearText.c_str(), (unsigned char*)encryptedText, rsa, RSA_PKCS1_PADDING);
	if (ret >= 0)
		strRet.assign(encryptedText, ret);
	else
		printf("err : %d, %s", ret, encryptedText);
	// 释放内存  
	free(encryptedText);
	BIO_free_all(keybio);
	RSA_free(rsa);

	return strRet;
}
std::string rsa_pub_encrypt_no_padding(std::string clearText, std::string pubKey)
{
	std::string strRet;
 	BIO* keybio = BIO_new_mem_buf((unsigned char *)pubKey.c_str(), -1);
	RSA* rsa = RSA_new();
	rsa = PEM_read_bio_RSAPublicKey(keybio, &rsa, NULL, NULL);
	if (!rsa)
	{
#ifdef _DEBUG
		ERR_load_crypto_strings();
		char errBuf[512];
		ERR_error_string_n(ERR_get_error(), errBuf, sizeof(errBuf));
		std::cout << "load key failed[" << errBuf << "]" << std::endl;
#endif
		BIO_free_all(keybio);
		return std::string("");
	}

	size_t len = (size_t)RSA_size(rsa);
	//int len = 1028;
	char* encryptedText = (char*)malloc(len + 1);
	memset(encryptedText, 0, len + 1);


	char* clearText_2 = (char*)malloc(len * 2);
	memset(clearText_2, 0, len * 2);
	memcpy(clearText_2, clearText.c_str(), clearText.size());


	// 加密函数  
	int ret = RSA_public_encrypt((int32)len, (const unsigned char*)clearText_2, (unsigned char*)encryptedText, rsa, RSA_NO_PADDING);
	if (ret >= 0)
		strRet.assign(encryptedText, ret);
	else
		printf("err : %d, %s", ret, encryptedText);
	// 释放内存  
	free(encryptedText);
	BIO_free_all(keybio);
	RSA_free(rsa);

	return strRet;
}

std::string rsa_pri_encrypt(std::string clearText, std::string priKey)
{
	std::string strRet;
	RSA* rsa = RSA_new();
	BIO* keybio;
	keybio = BIO_new_mem_buf((unsigned char*)priKey.c_str(), -1);

	// 此处有三种方法  
	// 1, 读取内存里生成的密钥对，再从内存生成rsa  
	// 2, 读取磁盘里生成的密钥对文本文件，在从内存生成rsa  
	// 3，直接从读取文件指针生成rsa  
	rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL, NULL);
	if (!rsa)
	{
#ifdef _DEBUG
		ERR_load_crypto_strings();
		char errBuf[512];
		ERR_error_string_n(ERR_get_error(), errBuf, sizeof(errBuf));
		std::cout << "load key failed[" << errBuf << "]" << std::endl;
#endif
		BIO_free_all(keybio);
		return std::string("");
	}

	size_t len = (size_t)RSA_size(rsa);
	char* encryptedText = (char*)malloc(len + 1);
	memset(encryptedText, 0, len + 1);

	// 解密函数  
	int ret = RSA_private_encrypt((int32)clearText.length(), (const unsigned char*)clearText.c_str(), (unsigned char*)encryptedText, rsa, RSA_PKCS1_PADDING);
	if (ret >= 0)
		strRet.assign(encryptedText, ret);

	// 释放内存  
	free(encryptedText);
	BIO_free_all(keybio);
	RSA_free(rsa);

	return strRet;
}

// 私钥解密    
std::string rsa_pri_decrypt(std::string cipherText, std::string priKey)
{
	std::string strRet;
	RSA* rsa = RSA_new();
	BIO* keybio;
	keybio = BIO_new_mem_buf((unsigned char*)priKey.c_str(), -1);

	// 此处有三种方法  
	// 1, 读取内存里生成的密钥对，再从内存生成rsa  
	// 2, 读取磁盘里生成的密钥对文本文件，在从内存生成rsa  
	// 3，直接从读取文件指针生成rsa  
	rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL, NULL);
	if (!rsa)
	{
#ifdef _DEBUG
		ERR_load_crypto_strings();
		char errBuf[512];
		ERR_error_string_n(ERR_get_error(), errBuf, sizeof(errBuf));
		std::cout << "load key failed[" << errBuf << "]" << std::endl;
#endif
		BIO_free_all(keybio);
		return std::string("");
	}

	size_t len = (size_t)RSA_size(rsa);
	char* decryptedText = (char*)malloc(len + 1);
	memset(decryptedText, 0, len + 1);

	// 解密函数  
	int ret = RSA_private_decrypt((int32)cipherText.length(), (const unsigned char*)cipherText.c_str(), (unsigned char*)decryptedText, rsa, RSA_PKCS1_PADDING);
	if (ret >= 0)
		strRet.assign(decryptedText, ret);

	// 释放内存  
	free(decryptedText);
	BIO_free_all(keybio);
	RSA_free(rsa);

	return strRet;
}

std::string rsa_pri_to_pkcs8(std::string priKey)
{
	std::string strRet;
	RSA* rsa = RSA_new();
	BIO* keybio;
	keybio = BIO_new_mem_buf((unsigned char*)priKey.c_str(), -1);

	// 此处有三种方法  
	// 1, 读取内存里生成的密钥对，再从内存生成rsa  
	// 2, 读取磁盘里生成的密钥对文本文件，在从内存生成rsa  
	// 3，直接从读取文件指针生成rsa  
	rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL, NULL);
	if (!rsa)
	{
#ifdef _DEBUG
		ERR_load_crypto_strings();
		char errBuf[512];
		ERR_error_string_n(ERR_get_error(), errBuf, sizeof(errBuf));
		std::cout << "load key failed[" << errBuf << "]" << std::endl;
#endif
		BIO_free_all(keybio);
		return std::string("");
	}

	BIO* out;
	EVP_PKEY* evp_pkey = EVP_PKEY_new();
	EVP_PKEY_assign_RSA(evp_pkey, rsa);

	if (!(out = BIO_new(BIO_s_mem())))
	{
		return "";
	}

	PEM_write_bio_PKCS8PrivateKey(out, evp_pkey,NULL,NULL,0,NULL,NULL);
	size_t pub_len = BIO_pending(out);
	char* pub_key = (char*)malloc(pub_len + 1);
	BIO_read(out, pub_key, (int32)pub_len);

	strRet.append(pub_key, pub_len);

	BIO_free_all(out);
	free(pub_key);
	EVP_PKEY_free(evp_pkey);

	return strRet;

}
std::string rsa_pri_to_pkcs1(std::string priKey)
{

	std::string strRet;
	BIO* keybio = BIO_new_mem_buf((unsigned char*)priKey.c_str(), -1);
	EVP_PKEY* evp_pkey = NULL;
	PEM_read_bio_PrivateKey(keybio, &evp_pkey, NULL, NULL);
	if (evp_pkey == NULL) {
		return "";
	}
	RSA* rsa = (RSA*)EVP_PKEY_get0_RSA(evp_pkey);
	if (!rsa)
	{
#ifdef _DEBUG
		ERR_load_crypto_strings();
		char errBuf[512];
		ERR_error_string_n(ERR_get_error(), errBuf, sizeof(errBuf));
		std::cout << "load key failed[" << errBuf << "]" << std::endl;
#endif
		BIO_free_all(keybio);
		return std::string("");
	}


	BIO* out;
	if (!(out = BIO_new(BIO_s_mem())))
	{
		return "";
	}

	PEM_write_bio_RSAPrivateKey(out, rsa,NULL,NULL,0,NULL,NULL);
	size_t pub_len = BIO_pending(out);
	char* pub_key = (char*)malloc(pub_len + 1);
	BIO_read(out, pub_key, (int32)pub_len);

	strRet.append(pub_key, pub_len);

	BIO_free_all(out);
	free(pub_key);
	EVP_PKEY_free(evp_pkey);

	return strRet;
}
std::string rsa_pub_to_pkcs8(std::string pubKey)
{
	std::string strRet;
	BIO* keybio = BIO_new_mem_buf((unsigned char*)pubKey.c_str(), -1);
	RSA* rsa = RSA_new();
	rsa = PEM_read_bio_RSAPublicKey(keybio, &rsa, NULL, NULL);
	if (!rsa)
	{
#ifdef _DEBUG
		ERR_load_crypto_strings();
		char errBuf[512];
		ERR_error_string_n(ERR_get_error(), errBuf, sizeof(errBuf));
		std::cout << "load key failed[" << errBuf << "]" << std::endl;
#endif
		BIO_free_all(keybio);
		return std::string("");
	}


	BIO* out;
	EVP_PKEY* evp_pkey = EVP_PKEY_new();
	EVP_PKEY_assign_RSA(evp_pkey, rsa);

	if (!(out = BIO_new(BIO_s_mem())))
	{
		return "";
	}

	PEM_write_bio_PUBKEY(out, evp_pkey);
	size_t pub_len = BIO_pending(out);
	char* pub_key = (char*)malloc(pub_len + 1);
	BIO_read(out, pub_key, (int32)pub_len);

	strRet.append(pub_key, pub_len);

	BIO_free_all(out);
	free(pub_key);
	EVP_PKEY_free(evp_pkey);

	return strRet;
}
std::string rsa_pub_to_pkcs1(std::string pubKey)
{

	std::string strRet;
	BIO* keybio = BIO_new_mem_buf((unsigned char*)pubKey.c_str(), -1);
	EVP_PKEY* evp_pkey = NULL;
	PEM_read_bio_PUBKEY(keybio, &evp_pkey, NULL, NULL);
	if (evp_pkey == NULL) {
		return "";
	}
	RSA* rsa = (RSA*)EVP_PKEY_get0_RSA(evp_pkey);
	if (!rsa)
	{
#ifdef _DEBUG
		ERR_load_crypto_strings();
		char errBuf[512];
		ERR_error_string_n(ERR_get_error(), errBuf, sizeof(errBuf));
		std::cout << "load key failed[" << errBuf << "]" << std::endl;
#endif
		BIO_free_all(keybio);
		return std::string("");
	}

	
	BIO* out;
	if (!(out = BIO_new(BIO_s_mem())))
	{
		return "";
	}

	PEM_write_bio_RSAPublicKey(out, rsa);
	size_t pub_len = BIO_pending(out);
	char* pub_key = (char*)malloc(pub_len + 1);
	BIO_read(out, pub_key, (int32)pub_len);

	strRet.append(pub_key, pub_len);

	BIO_free_all(out);
	free(pub_key);
	EVP_PKEY_free(evp_pkey);

	return strRet;
}
cpps::tuple generateRSAKey(C*c,object oKEY_LENGTH)
{
	cpps_integer KEY_LENGTH = 1024;
	if (oKEY_LENGTH.isint()) KEY_LENGTH = oKEY_LENGTH.toint();

	// 公私密钥对    
	size_t pri_len;
	size_t pub_len;
	char* pri_key = NULL;
	char* pub_key = NULL;

	// 生成密钥对    
	
	RSA* keypair = RSA_new();
	BIGNUM* bne = BN_new();
	BN_set_word(bne, RSA_F4);
	RSA_generate_key_ex(keypair,(int32)KEY_LENGTH, bne, NULL);

	BIO* pri = BIO_new(BIO_s_mem());
	BIO* pub = BIO_new(BIO_s_mem());

	PEM_write_bio_RSAPrivateKey(pri, keypair, NULL, NULL, 0, NULL, NULL);
	PEM_write_bio_RSAPublicKey(pub, keypair);


	// 获取长度    
	pri_len = BIO_pending(pri);
	pub_len = BIO_pending(pub);

	// 密钥对读取到字符串
	pri_key = (char*)malloc(pri_len + 1);
	pub_key = (char*)malloc(pub_len + 1);

	BIO_read(pri, pri_key, (int32)pri_len);
	BIO_read(pub, pub_key, (int32)pub_len);

	pri_key[pri_len] = '\0';
	pub_key[pub_len] = '\0';
	cpps::tuple ret(object::create(c, pri_key), object::create(c, pub_key));
	// 内存释放  
	RSA_free(keypair);
	BIO_free_all(pub);
	BIO_free_all(pri);
	BN_free(bne);

	free(pri_key);
	free(pub_key);

	return ret;
}

std::string rsa_make_pub(std::string module, std::string exp)
{
	std::string strRet;

	BIGNUM* n = BN_bin2bn((const unsigned char* )module.c_str(),(int) module.size(), NULL);
	BIGNUM* e = BN_bin2bn((const unsigned char* )exp.c_str(),(int)exp.size(), NULL);
	if (n && e)
	{
		EVP_PKEY* pRsaKey = EVP_PKEY_new();
		RSA* rsa = RSA_new();
		RSA_set0_key(rsa, n, e, NULL);
		EVP_PKEY_assign_RSA(pRsaKey, rsa);

		BIO* out;

		if (!(out = BIO_new(BIO_s_mem())))
		{
			return "";
		}

		PEM_write_bio_PUBKEY(out, pRsaKey);
		size_t pub_len = BIO_pending(out);
		char* pub_key = (char*)malloc(pub_len + 1);
		BIO_read(out, pub_key, (int32)pub_len);

		strRet.append(pub_key, pub_len);

		BIO_free_all(out);
		free(pub_key);
		EVP_PKEY_free(pRsaKey);
	}
	else {

		if (n) BN_free(n);
		if (e) BN_free(e);
	}

	return strRet;
}

cpps_export_void  cpps_attach(cpps::C* c)
{

	cpps::cpps_init_cpps_class(c);

    cpps::_module(c, "rsa")[
      def("pri_decrypt", rsa_pri_decrypt),
      def("pri_encrypt", rsa_pri_encrypt),
      def("pub_encrypt", rsa_pub_encrypt),
      def("pub_encrypt_no_padding", rsa_pub_encrypt_no_padding),
      def("pub_decrypt", rsa_pub_decrypt),
      def("pub_to_pkcs8", rsa_pub_to_pkcs8),
      def("pri_to_pkcs8", rsa_pri_to_pkcs8),
      def("pri_to_pkcs1", rsa_pri_to_pkcs1),
      def("pub_to_pkcs1", rsa_pub_to_pkcs1),
      def("rsa_make_pub", rsa_make_pub),
      def_inside("generate_rsakey", generateRSAKey)
	];



   
}
cpps_export_void  cpps_detach(cpps::C * c)
{
	cpps::_unmodule(c, "rsa");
}

cpps_export_finish