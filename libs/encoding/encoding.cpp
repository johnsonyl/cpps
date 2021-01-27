#include <cpps/cpps.h>
#include <iostream>
#include <string>
#include <fstream>
#include "strnormalize.h"
#include "ConvertUTF.h"
#include <string.h>

bool cpps_check_utf8(char* str, size_t length)
{
	cpps::usint32 nBytes = 0;
	cpps::usint8 chr;
	/*bool bAllAscii = true;*/
	for (size_t i = 0; i < length; ++i)
	{
		chr = *(str + i);
		/*if ((chr & 0x80) != 0)
			bAllAscii = false;*/
		if (nBytes == 0)
		{
			if (chr >= 0x80)
			{
				if (chr >= 0xFC && chr <= 0xFD)
					nBytes = 6;
				else if (chr >= 0xF8)
					nBytes = 5;
				else if (chr >= 0xF0)
					nBytes = 4;
				else if (chr >= 0xE0)
					nBytes = 3;
				else if (chr >= 0xC0)
					nBytes = 2;
				else
					return false;

				nBytes--;
			}
		}
		else
		{
			if ((chr & 0xC0) != 0x80)
				return false;

			nBytes--;
		}
	}
	if (nBytes > 0)
		return false;
	//if (bAllAscii) 
	//	return false;

	return TRUE;
}
#define CPPS_ENCODING_UTF8 0
#define CPPS_ENCODING_GBK 1

std::string  encode(cpps::cpps_value src, cpps::usint8 encoding)
{
	std::string ret;
	std::string* s = cpps_get_string(src);
	if (encoding == CPPS_ENCODING_UTF8)
	{
		bool b = cpps_check_utf8((char*)s->c_str(), s->size());
		if (b) return *s;

		size_t utf8buffer_len = s->size() * 3 + 1;
		char* utf8buffer = (char*)malloc(utf8buffer_len);
		memset(utf8buffer, 0, utf8buffer_len);

		gbk_to_utf8(s->c_str(), (unsigned int)s->size(), &utf8buffer, (unsigned int *)&utf8buffer_len);

		ret = utf8buffer;

		free(utf8buffer);
	}
	else if(encoding == CPPS_ENCODING_GBK)
	{
		bool b = cpps_check_utf8((char*)s->c_str(), s->size());
		if (!b) return *s;

		size_t gbkbuffer_len = s->size() * 2 + 1;
		char* gbkbuffer = (char*)malloc(gbkbuffer_len);
		memset(gbkbuffer, 0, gbkbuffer_len);

		utf8_to_gbk(s->c_str(), (unsigned int)s->size(), &gbkbuffer, (unsigned int*)&gbkbuffer_len);
		ret = gbkbuffer;
		free(gbkbuffer);

	}
	return ret;
}
std::string utf8_to_utf16(cpps::object src)
{
	std::string ret;
	if (src.isstring())
	{
		std::string* srcstr = cpps_get_string(src.value);


		const UTF8* Src = (const UTF8*)(srcstr->c_str());
		const UTF8* SrcEnd = Src + srcstr->size();

		ret.resize(srcstr->size() * UNI_MAX_UTF8_BYTES_PER_CODE_POINT + 1);
		UTF16 *Dst = reinterpret_cast<UTF16*>(&ret[0]);
		UTF16* DstEnd = reinterpret_cast<UTF16*>(&ret[ret.size() - 1]);

	
		ConversionResult result = ConvertUTF8toUTF16(&Src, SrcEnd, &Dst, DstEnd, ConversionFlags::strictConversion);
		if (result != conversionOK) {
			ret.clear();
			return ret;
		}
		ret.resize(reinterpret_cast<char*>(Dst) - &ret[0]);
	}
	return ret;
}
std::string utf16_to_utf8(cpps::object src)
{
	std::string ret;
	if (src.isstring())
	{
		std::string* srcstr = cpps_get_string(src.value);
		
		const UTF16* Src = reinterpret_cast<const UTF16*>(srcstr->c_str());
		const UTF16* SrcEnd = Src + (srcstr->size() / sizeof(UTF16));
		ret.resize(srcstr->size() * UNI_MAX_UTF8_BYTES_PER_CODE_POINT + 1);
		UTF8* Dst = reinterpret_cast<UTF8*>(&ret[0]);
		UTF8* DstEnd = reinterpret_cast<UTF8*>(&ret[ret.size() - 1]);

		ConversionResult result = ConvertUTF16toUTF8(&Src, SrcEnd, &Dst, DstEnd, ConversionFlags::strictConversion);
		if (result != conversionOK) {
			ret.clear();
			return ret;
		}
		ret.resize(reinterpret_cast<char*>(Dst) - &ret[0]);
		ret.push_back(0);
		ret.pop_back();
	}
	return ret;
}
std::string utf8_to_utf32(cpps::object src)
{
	std::string ret;
	if (src.isstring())
	{
		std::string* srcstr = cpps_get_string(src.value);
		const UTF8* Src = (const UTF8*)(srcstr->c_str());
		const UTF8* SrcEnd = Src + srcstr->size();

		ret.resize(srcstr->size() * UNI_MAX_UTF8_BYTES_PER_CODE_POINT + 1);
		UTF32* Dst = reinterpret_cast<UTF32*>(&ret[0]);
		UTF32* DstEnd = reinterpret_cast<UTF32*>(&ret[ret.size() - 1]);


		ConversionResult result = ConvertUTF8toUTF32(&Src, SrcEnd, &Dst, DstEnd, ConversionFlags::strictConversion);
		if (result != conversionOK) {
			ret.clear();
			return ret;
		}
		ret.resize(reinterpret_cast<char*>(Dst) - &ret[0]);
	}
	return ret;
}
std::string utf32_to_utf8(cpps::object src)
{
	std::string ret;
	if (src.isstring())
	{
		std::string* srcstr = cpps_get_string(src.value);

		const UTF32* Src = reinterpret_cast<const UTF32*>(srcstr->c_str());
		const UTF32* SrcEnd = Src + (srcstr->size() / sizeof(UTF32));
		ret.resize(srcstr->size() * UNI_MAX_UTF8_BYTES_PER_CODE_POINT + 1);
		UTF8* Dst = reinterpret_cast<UTF8*>(&ret[0]);
		UTF8* DstEnd = reinterpret_cast<UTF8*>(&ret[ret.size() - 1]);

		ConversionResult result = ConvertUTF32toUTF8(&Src, SrcEnd, &Dst, DstEnd, ConversionFlags::strictConversion);
		if (result != conversionOK) {
			ret.clear();
			return ret;
		}
		ret.resize(reinterpret_cast<char*>(Dst) - &ret[0]);
		ret.push_back(0);
		ret.pop_back();

	}
	return ret;
}
std::string utf16_to_utf32(cpps::object src)
{
	std::string ret;
	if (src.isstring())
	{
		std::string* srcstr = cpps_get_string(src.value);
		const UTF16* Src = (const UTF16*)(srcstr->c_str());
		const UTF16* SrcEnd = Src + (srcstr->size()/sizeof(UTF16));

		ret.resize(srcstr->size() * UNI_MAX_UTF8_BYTES_PER_CODE_POINT + 1);
		UTF32* Dst = reinterpret_cast<UTF32*>(&ret[0]);
		UTF32* DstEnd = reinterpret_cast<UTF32*>(&ret[ret.size() - 1]);


		ConversionResult result = ConvertUTF16toUTF32(&Src, SrcEnd, &Dst, DstEnd, ConversionFlags::strictConversion);
		if (result != conversionOK) {
			ret.clear();
			return ret;
		}
		ret.resize(reinterpret_cast<char*>(Dst) - &ret[0]);

	}
	return ret;
}
std::string utf32_to_utf16(cpps::object src)
{
	std::string ret;
	if (src.isstring())
	{
		std::string* srcstr = cpps_get_string(src.value);

		const UTF32* Src = reinterpret_cast<const UTF32*>(srcstr->c_str());
		const UTF32* SrcEnd = Src + (srcstr->size() / sizeof(UTF32));
		ret.resize(srcstr->size() * UNI_MAX_UTF8_BYTES_PER_CODE_POINT + 1);
		UTF16* Dst = reinterpret_cast<UTF16*>(&ret[0]);
		UTF16* DstEnd = reinterpret_cast<UTF16*>(&ret[ret.size() - 1]);

		ConversionResult result = ConvertUTF32toUTF16(&Src, SrcEnd, &Dst, DstEnd, ConversionFlags::strictConversion);
		if (result != conversionOK) {
			ret.clear();
			return ret;
		}
		ret.resize(reinterpret_cast<char*>(Dst) - &ret[0]);
	}
	return ret;
}
void codepoint16_to_utf8(cpps_integer codepoint16,cpps::object target)
{
	if (target.isstring()) {
		std::string* targetstr = cpps_get_string(target.value);
		targetstr->clear();
		targetstr->resize(8);
		char* resultptr = (char *)(targetstr->c_str());
		char* take = resultptr;
		bool result = llvm::ConvertCodePoint16ToUTF8((UTF16)codepoint16, resultptr);
		if (!result) {
			targetstr->clear();
			return;
		}
		targetstr->resize(resultptr - take);
	}
}
void codepoint_to_utf8(cpps_integer codepoint, cpps::object target)
{
	std::string* targetstr = cpps_get_string(target.value);
	targetstr->clear();
	targetstr->resize(8);
	char* resultptr = (char*)(targetstr->c_str());
	char* take = resultptr;
	bool result = llvm::ConvertCodePoint16ToUTF8((UTF16)codepoint, resultptr);
	if (!result) {
		targetstr->clear();
		return;
	}
	targetstr->resize(resultptr - take);
}

cpps::int32 hex2dec(char* s) {
	size_t L = strlen(s);
	char c;
	cpps::int32 re = 0;
	while ((c = ((s++)[0]))) {
		if (c >= '0' && c <= '9') {
			c -= 48;
		}
		else {
			c = c > 'Z' ? c - 32 : c;
			c -= 'A' - 10;
		}
		re += c * (cpps::int32)pow(16, --L);
	}
	return (cpps::int32)re;
}
std::string utf8_unescape(std::string text) {
	std::string rets; 
	const char* str = text.c_str();
	char* re = (char*)calloc(strlen(str) + 1, 1);
	const char* _str;
	char* _re = re;
	char code[5] = { 0 };
	UTF16 wc;
	size_t n;
	while (str) {
		_str = strchr(str, '%');
		if (!_str) break;
		if ((n = (_str - str))) {
			memcpy(_re, str, n);
			_re += n;
		}
		memset(code, 0, 5);
		if (_str[1] == 'u') {
			memcpy(code, _str + 2, 4);
			str = _str + 6;
			wc = (UTF16) hex2dec(code);
			memset(code, 0, 5);


			const UTF16* Src = reinterpret_cast<const UTF16*>(&wc);
			const UTF16* SrcEnd = Src + 1;
			
			UTF8* Dst = reinterpret_cast<UTF8*>(code);
			UTF8* DstEnd = reinterpret_cast<UTF8*>(code + 5);

			ConversionResult result = ConvertUTF16toUTF8(&Src, SrcEnd, &Dst, DstEnd, ConversionFlags::strictConversion);
			if (result != conversionOK) {
				break;
			}
			code[4] = '\0';
			memcpy(_re, code, 2);
			_re += 2;
		}
		else {
			memcpy(code, _str + 1, 2);
			str = _str + 3;
			_re[0] = (char) hex2dec(code);
			_re++;
		}
	}
	strcpy(_re, str);
	rets = re;
	free(re);

	return re;
}

using namespace cpps;
using namespace std;
cpps_export_void cpps_attach(cpps::C* c)
{

	cpps::cpps_init_cpps_class(c);

	str_normalize_init();

	cpps::_module(c, "encoding")[
		def("encode", encode),
		def("utf8_to_utf16", utf8_to_utf16),
		def("utf16_to_utf8", utf16_to_utf8),
		def("utf8_to_utf32", utf8_to_utf32),
		def("utf32_to_utf8", utf32_to_utf8),
		def("utf16_to_utf32", utf16_to_utf32),
		def("utf32_to_utf16", utf32_to_utf16),
		def("codepoint16_to_utf8", codepoint16_to_utf8),
		def("codepoint_to_utf8", codepoint_to_utf8),
		def("unescape", utf8_unescape),
		defvar(c,"UTF8", CPPS_ENCODING_UTF8),
		defvar(c,"GBK", CPPS_ENCODING_GBK)
	];



   
}
cpps_export_void  cpps_detach(cpps::C * c)
{
	cpps::_unmodule(c, "encoding");
}

cpps_export_finish