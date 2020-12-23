

#include <cpps/cpps.h>
#include <iostream>
#include <string>
#include <fstream>


using namespace cpps;
using namespace std;

std::string EncodeQuoted(std::string pSrc,cpps_value MaxLineLen)
{
	int nMaxLineLen = 73;
	if (MaxLineLen.tt == CPPS_TINTEGER) nMaxLineLen = (int) MaxLineLen.value.integer;

	std::string pDst = "";
	int nLineLen = 0;
	for (size_t i = 0; i < pSrc.size(); i++)
	{
		// ASCII 33-60, 62-126原样输出，其余的需编码
		if (((pSrc[i] >= 33 && pSrc[i] <= 60) || (pSrc[i] >= 62 && pSrc[i] <= 126)) && pSrc[i] != '.')
		{
			pDst.push_back(pSrc[i]);
			nLineLen++;
		}
		else
		{
			char tmp[32];
			sprintf(tmp, "=%02X", (unsigned char)pSrc[i]);
			pDst.append(tmp);
			nLineLen += 3;
		}
		// 输出换行？
		if (nLineLen >= nMaxLineLen - 3)
		{
			pDst.append("=\r\n");
			nLineLen = 0;
		}
	}
	return pDst;
}

std::string DecodeQuoted(std::string pSrc)
{
	std::string pDst = "";
	int nDstLen; // 输出的字符计数
	size_t i;
	i = 0;
	nDstLen = 0;
	while (i < pSrc.size())
	{
		if (strncmp(pSrc.c_str() + i, "=\r\n", 3) == 0) // 软回车，跳过
		{
			i += 3;
		}
		else
		{
			if (pSrc[i] == '=') // 是编码字节
			{
				char tmp[32];
				sscanf(pSrc.c_str() + i, "=%02X", (usint32*)tmp);
				pDst.append(tmp);
				i += 3;
			}
			else // 非编码字节
			{
				pDst.push_back(pSrc[i]);
				i++;
			}

			nDstLen++;
		}
	}
	return pDst;
}

cpps_export_void cpps_attach(cpps::C* c)
{

	cpps::cpps_init_cpps_class(c);

	cpps::_module(c, "quotedprintable")[
		def("encode", EncodeQuoted),
		def("decode", DecodeQuoted)
	];



   
}
cpps_export_void  cpps_detach(cpps::C * c)
{
}

cpps_export_finish