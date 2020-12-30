/*
 * FIPS 180-2 SHA-224/256/384/512 implementation
 * Last update: 02/02/2007
 * Issue date:  04/30/2005
 *
 * Copyright (C) 2005, 2007 Olivier Gay <olivier.gay@a3.epfl.ch>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the project nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE PROJECT AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE PROJECT OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef SHA2_H
#define SHA2_H

#define SHA224_DIGEST_SIZE ( 224 / 8)
#define SHA256_DIGEST_SIZE ( 256 / 8)
#define SHA384_DIGEST_SIZE ( 384 / 8)
#define SHA512_DIGEST_SIZE ( 512 / 8)

#define SHA256_BLOCK_SIZE  ( 512 / 8)
#define SHA512_BLOCK_SIZE  (1024 / 8)
#define SHA384_BLOCK_SIZE  SHA512_BLOCK_SIZE
#define SHA224_BLOCK_SIZE  SHA256_BLOCK_SIZE

#ifndef SHA2_TYPES
#define SHA2_TYPES
typedef unsigned char uint8;
typedef unsigned int  uint32;
typedef unsigned long long uint64;
#endif

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct {
        unsigned int tot_len;
        unsigned int len;
        unsigned char block[2 * SHA256_BLOCK_SIZE];
        uint32 h[8];
    } sha256_ctx;

    typedef struct {
        unsigned int tot_len;
        unsigned int len;
        unsigned char block[2 * SHA512_BLOCK_SIZE];
        uint64 h[8];
    } sha512_ctx;

    typedef sha512_ctx sha384_ctx;
    typedef sha256_ctx sha224_ctx;

    void sha224_init(sha224_ctx* ctx);
    void sha224_update(sha224_ctx* ctx, const unsigned char* message,
        unsigned int len);
    void sha224_final(sha224_ctx* ctx, unsigned char* digest);
    void sha224(const unsigned char* message, unsigned int len,
        unsigned char* digest);

    void sha256_init(sha256_ctx* ctx);
    void sha256_update(sha256_ctx* ctx, const unsigned char* message,
        unsigned int len);
    void sha256_final(sha256_ctx* ctx, unsigned char* digest);
    void sha256(const unsigned char* message, unsigned int len,
        unsigned char* digest);

    void sha384_init(sha384_ctx* ctx);
    void sha384_update(sha384_ctx* ctx, const unsigned char* message,
        unsigned int len);
    void sha384_final(sha384_ctx* ctx, unsigned char* digest);
    void sha384(const unsigned char* message, unsigned int len,
        unsigned char* digest);

    void sha512_init(sha512_ctx* ctx);
    void sha512_update(sha512_ctx* ctx, const unsigned char* message,
        unsigned int len);
    void sha512_final(sha512_ctx* ctx, unsigned char* digest);
    void sha512(const unsigned char* message, unsigned int len,
        unsigned char* digest);

#ifdef __cplusplus
}
#endif
#include <cpps/cpps.h>
#include <string>

class SHA224
{
public:
    SHA224() {
        sha224_init(&ctx);
    }
public:
    void        update(std::string input) {
		if (input.empty()) return;

        sha224_update(&ctx, (const unsigned char*)input.c_str(), (unsigned int)input.size());
    }
    std::string digest(){
        std::string ret;
		unsigned char digest[SHA224_DIGEST_SIZE];
        sha224_final(&ctx, digest);
        ret.append((char *)digest, SHA224_DIGEST_SIZE);
        return ret;
    }
    std::string hex_digest()
    {
		unsigned char digest[SHA224_DIGEST_SIZE];
		sha224_final(&ctx, digest);

		char s[SHA224_DIGEST_SIZE*2 + 1];
		::memset(s, 0, SHA224_DIGEST_SIZE * 2+1);

		for (int i = 0; i < SHA224_DIGEST_SIZE; i++)
			sprintf(s + i * 2, "%02x", digest[i]);

		s[SHA224_DIGEST_SIZE * 2] = '\0';

        return s;
    }
    void        update_file(FILE* file)
    {
        if (!file) return;

		unsigned char buffer[1024];
		size_t len;

		while ((len = fread(buffer, 1, 1024, file)) != 0)
            sha224_update(&ctx,buffer, (unsigned int)len);

		fclose(file);
    }

public:
    sha224_ctx ctx;
};

class SHA256
{
public:
	SHA256() {
		sha256_init(&ctx);
	}
public:
	void        update(std::string input) {
		if (input.empty()) return;

		sha256_update(&ctx, (const unsigned char*)input.c_str(), (unsigned int)input.size());
	}
	std::string digest() {
		std::string ret;
		unsigned char digest[SHA256_DIGEST_SIZE];
		sha256_final(&ctx, digest);
		ret.append((char*)digest, SHA256_DIGEST_SIZE);
		return ret;
	}
	std::string hex_digest()
	{
		unsigned char digest[SHA256_DIGEST_SIZE];
		sha256_final(&ctx, digest);

		char s[SHA256_DIGEST_SIZE * 2 + 1];
		::memset(s, 0, SHA256_DIGEST_SIZE * 2 + 1);

		for (int i = 0; i < SHA256_DIGEST_SIZE; i++)
			sprintf(s + i * 2, "%02x", digest[i]);

		s[SHA256_DIGEST_SIZE * 2] = '\0';

		return s;
	}
	void        update_file(FILE* file)
	{
		if (!file) return;

		unsigned char buffer[1024];
		size_t len;

		while ((len = fread(buffer, 1, 1024, file)) != 0)
			sha256_update(&ctx, buffer, (unsigned int)len);

		fclose(file);
	}

public:
	sha256_ctx ctx;
};

class SHA384
{
public:
	SHA384() {
		sha384_init(&ctx);
	}
public:
	void        update(std::string input) {
		if (input.empty()) return;

		sha384_update(&ctx, (const unsigned char*)input.c_str(), (unsigned int)input.size());
	}
	std::string digest() {
		std::string ret;
		unsigned char digest[SHA384_DIGEST_SIZE];
		sha384_final(&ctx, digest);
		ret.append((char*)digest, SHA384_DIGEST_SIZE);
		return ret;
	}
	std::string hex_digest()
	{
		unsigned char digest[SHA384_DIGEST_SIZE];
		sha384_final(&ctx, digest);

		char s[SHA384_DIGEST_SIZE * 2 + 1];
		::memset(s, 0, SHA384_DIGEST_SIZE * 2 + 1);

		for (int i = 0; i < SHA384_DIGEST_SIZE; i++)
			sprintf(s + i * 2, "%02x", digest[i]);

		s[SHA384_DIGEST_SIZE * 2] = '\0';

		return s;
	}
	void        update_file(FILE* file)
	{
		if (!file) return;

		unsigned char buffer[1024];
		size_t len;

		while ((len = fread(buffer, 1, 1024, file)) != 0)
			sha384_update(&ctx, buffer, (unsigned int)len);

		fclose(file);
	}

public:
	sha384_ctx ctx;
};

class SHA512
{
public:
	SHA512() {
		sha512_init(&ctx);
	}
public:
	void        update(std::string input) {
		if (input.empty()) return;
		sha512_update(&ctx, (const unsigned char*)input.c_str(), (unsigned int)input.size());
	}
	std::string digest() {
		std::string ret;
		unsigned char digest[SHA512_DIGEST_SIZE];
		sha512_final(&ctx, digest);
		ret.append((char*)digest, SHA512_DIGEST_SIZE);
		return ret;
	}

	std::string hex_digest()
	{
		unsigned char digest[SHA512_DIGEST_SIZE];
		sha512_final(&ctx, digest);

		char s[SHA512_DIGEST_SIZE * 2 + 1];
		::memset(s, 0, SHA512_DIGEST_SIZE * 2 + 1);

		for (int i = 0; i < SHA512_DIGEST_SIZE; i++)
			sprintf(s + i * 2, "%02x", digest[i]);

		s[SHA512_DIGEST_SIZE * 2] = '\0';

		return s;
	}
	void        update_file(FILE* file)
	{
		if (!file) return;

		unsigned char buffer[1024];
		size_t len;

		while ((len = fread(buffer, 1, 1024, file)) != 0)
			sha512_update(&ctx, buffer, (unsigned int)len);

		fclose(file);
	}

public:
	sha512_ctx ctx;
};

#endif /* !SHA2_H */

