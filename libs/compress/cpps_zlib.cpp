#include "compress.h"
#include "cpps_zlib.h"
namespace cpps {
	
	cpps_value cpps_compress_zlib_compress(C *c,cpps_value data, cpps_value level)
	{
		cpps_integer nlevel = cpps_isint(level)? level.value.integer : Z_DEFAULT_COMPRESSION;
		const Bytef* buf = NULL;
		uLong len = 0;

		if (data.tt == CPPS_TSTRING) {
			std::string* str = cpps_get_string(data);
			buf = (Bytef*)str->c_str();
			len = (uLong)str->size();
		}
		else if (data.isdomain() && data.is_kindof<Buffer>())
		{
			cpps_cppsclassvar* cppsclassvar = (cpps_cppsclassvar*)data.value.domain;
			Buffer* buffer = (Buffer*)cppsclassvar->getclsptr();
			buf = (Bytef*)buffer->getbuffer();
			len = (uLong)buffer->length();
		}
		else {
			throw(cpps_error("", 0, cpps_error_normalerror, "compress data just support Buffer or string."));
		}

		uLongf destlen = (uLongf)len + 20; /*不知道这个协议头到底是多大*/
		Bytef* dest = new Bytef[destlen];

		int32 err = compress2(dest, &destlen, buf, len, (int)nlevel);
		if (err != Z_OK) {
			delete[] dest;
			dest = NULL;
			return nil;
		}

		std::string* rets;
		cpps_value ret;
		newclass<std::string>(c, &rets,&ret);
		ret.tt = CPPS_TSTRING;
		rets->append((const char*)dest, destlen);
		delete[] dest;
		dest = NULL;

		return ret;
	}

	cpps_value cpps_compress_zlib_decompress(C* c, cpps_value data, cpps_value bufsize, cpps_value wbits)
	{
		//cpps_integer nwbits = wbits.tt == CPPS_TINTEGER ? wbits.value.integer : MAX_WBITS;
		cpps_uinteger nbufsize = cpps_isint(bufsize) ? bufsize.value.uinteger : DEF_BUF_SIZE;

		const Bytef* buf = NULL;
		uLong len = 0;

		if (data.tt == CPPS_TSTRING) {
			std::string* str = cpps_get_string(data);
			buf = (Bytef*)str->c_str();
			len = (uLong)str->size();
		}
		else if (data.isdomain() && data.is_kindof<Buffer>())
		{
			cpps_cppsclassvar* cppsclassvar = (cpps_cppsclassvar*)data.value.domain;
			Buffer* buffer = (Buffer*)cppsclassvar->getclsptr();
			buf = (Bytef*)buffer->getbuffer();
			len = (uLong)buffer->length();
		}
		else {
			throw(cpps_error("", 0, cpps_error_normalerror, "decompress data just support Buffer or string."));
		}

		Bytef* dest = new Bytef[size_t(nbufsize)];
		uLong destlen = (uLong)nbufsize;

		int32 err = uncompress2(dest, &destlen, buf, &len);
		if (err != Z_OK) {
			return nil;
		}

		std::string *rets;
		cpps_value ret;
		newclass<std::string>(c, &rets,&ret);
		ret.tt = CPPS_TSTRING;
		rets->append((const char*)dest, destlen);
		delete[] dest;
		dest = NULL;

		return ret;
	}
	cpps_uinteger cpps_compress_zlib_adler32(cpps_value data, cpps_value adler)
	{
		cpps_uinteger nadler = cpps_isint(adler) ? adler.value.uinteger : 1;
		const Bytef* buf = NULL;
		uInt len = 0;

		if (data.tt == CPPS_TSTRING) {
			std::string* str = cpps_get_string(data);
			buf = (Bytef*)str->c_str();
			len = (uLong)str->size();
		}
		else if (data.isdomain() && data.is_kindof<Buffer>())
		{
			cpps_cppsclassvar* cppsclassvar = (cpps_cppsclassvar*)data.value.domain;
			Buffer* buffer = (Buffer*)cppsclassvar->getclsptr();
			buf = (Bytef*)buffer->getbuffer();
			len = (uLong)buffer->length();
		}
		else {
			throw(cpps_error("", 0, cpps_error_normalerror, "adler32 data just support Buffer or string."));
		}

		return (cpps_uinteger)adler32((uLong)nadler, buf, len);
	}
	cpps_uinteger cpps_compress_zlib_crc32(cpps_value data, cpps_value crc)
	{
		cpps_integer ncrc = cpps_isint(crc) ? crc.value.uinteger : 1;
		const Bytef* buf = NULL;
		uInt len = 0;

		if (data.tt == CPPS_TSTRING) {
			std::string* str = cpps_get_string(data);
			buf = (Bytef*)str->c_str();
			len = (uLong)str->size();
		}
		else if (data.isdomain() && data.is_kindof<Buffer>())
		{
			cpps_cppsclassvar* cppsclassvar = (cpps_cppsclassvar*)data.value.domain;
			Buffer* buffer = (Buffer*)cppsclassvar->getclsptr();
			buf = (Bytef*)buffer->getbuffer();
			len = (uLong)buffer->length();
		}
		else {
			throw(cpps_error("", 0, cpps_error_normalerror, "crc32 data just support Buffer or string."));
		}

		return (cpps_uinteger)crc32((uLong)ncrc, buf, len);
	}

}
