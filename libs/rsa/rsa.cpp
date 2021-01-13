
#include <cpps/cpps.h>
#include <iostream>
#include <string>
#include <fstream>


#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>


using namespace cpps;
using namespace std;

std::string rsa_pub_encrypt(std::string clearText, std::string pubKey)
{
	std::string strRet;
 	BIO* keybio = BIO_new_mem_buf((unsigned char *)pubKey.c_str(), -1);
	// 此处有三种方法  
	// 1, 读取内存里生成的密钥对，再从内存生成rsa  
	// 2, 读取磁盘里生成的密钥对文本文件，在从内存生成rsa  
	// 3，直接从读取文件指针生成rsa  
	//RSA* pRSAPublicKey = RSA_new();  
	RSA* rsa = RSA_new();
	rsa = PEM_read_bio_RSAPublicKey(keybio, &rsa, NULL, NULL);
	if (!rsa)
	{
		ERR_load_crypto_strings();
		char errBuf[512];
		ERR_error_string_n(ERR_get_error(), errBuf, sizeof(errBuf));
		std::cout << "load public key failed[" << errBuf << "]" << std::endl;

		BIO_free_all(keybio);
		return std::string("");
	}

	int len = RSA_size(rsa);
	//int len = 1028;
	char* encryptedText = (char*)malloc(len + 1);
	memset(encryptedText, 0, len + 1);

	// 加密函数  
	int ret = RSA_public_encrypt((int)clearText.length(), (const unsigned char*)clearText.c_str(), (unsigned char*)encryptedText, rsa, RSA_PKCS1_PADDING);
	if (ret >= 0)
		strRet = std::string(encryptedText, ret);

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

	int len = RSA_size(rsa);
	char* decryptedText = (char*)malloc(len + 1);
	memset(decryptedText, 0, len + 1);

	// 解密函数  
	int ret = RSA_private_decrypt((int32)cipherText.length(), (const unsigned char*)cipherText.c_str(), (unsigned char*)decryptedText, rsa, RSA_PKCS1_PADDING);
	if (ret >= 0)
		strRet = std::string(decryptedText, ret);

	// 释放内存  
	free(decryptedText);
	BIO_free_all(keybio);
	RSA_free(rsa);

	return strRet;
}
cpps::mulitreturn generateRSAKey(C*c,object oKEY_LENGTH)
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
	cpps::mulitreturn ret(object::create(c, pri_key), object::create(c, pub_key));
	// 内存释放  
	RSA_free(keypair);
	BIO_free_all(pub);
	BIO_free_all(pri);
	BN_free(bne);

	free(pri_key);
	free(pub_key);

	return ret;
}
cpps_export_void  cpps_attach(cpps::C* c)
{

	cpps::cpps_init_cpps_class(c);

    cpps::_module(c, "rsa")[
      def("pri_decrypt", rsa_pri_decrypt),
      def("pub_encrypt", rsa_pub_encrypt),
      def_inside("generate_raskey", generateRSAKey)
	];



   
}
cpps_export_void  cpps_detach(cpps::C * c)
{
}

cpps_export_finish