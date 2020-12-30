// libdemo.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"


#include <cpps/cpps.h>
#include <iostream>
#include <string>
#include <fstream>

using namespace cpps;
using namespace std;
#include "MD5.h"
#include "sha1.h"
#include "sha2.h"

cpps_export_void  cpps_attach(cpps::C* c)
{

	cpps::cpps_init_cpps_class(c);

	cpps::_module(c, "hashlib")[
		_class<MD5>("md5")
		.def("constructor",&MD5::cpps_update)
		.def("update",&MD5::cpps_update)
		.def("hexdigest",&MD5::hex_digest)
		.def("hexdigest16",&MD5::hex_digest16)
		.def("digest",&MD5::raw_digest)
		.def("update_file",&MD5::update_file),
		_class<CSHA1>("sha1")
		.def("constructor",&CSHA1::cpps_update)
		.def("update",&CSHA1::cpps_update)
		.def("hexdigest",&CSHA1::hex_digest)
		.def("digest",&CSHA1::digest)
		.def("update_file",&CSHA1::update_file),
		_class<SHA224>("sha224")
		.def("constructor",&SHA224::update)
		.def("update",&SHA224::update)
		.def("hexdigest",&SHA224::hex_digest)
		.def("digest",&SHA224::digest)
		.def("update_file",&SHA224::update_file),
		_class<SHA256>("sha256")
		.def("constructor",&SHA256::update)
		.def("update",&SHA256::update)
		.def("hexdigest",&SHA256::hex_digest)
		.def("digest",&SHA256::digest)
		.def("update_file",&SHA256::update_file),
		_class<SHA384>("sha384")
		.def("constructor",&SHA384::update)
		.def("update",&SHA384::update)
		.def("hexdigest",&SHA384::hex_digest)
		.def("digest",&SHA384::digest)
		.def("update_file",&SHA384::update_file),
		_class<SHA512>("sha512")
		.def("constructor",&SHA512::update)
		.def("update",&SHA512::update)
		.def("digest",&SHA512::digest)
		.def("hexdigest",&SHA512::hex_digest)
		.def("update_file",&SHA512::update_file)
	];



   
}
cpps_export_void  cpps_detach(cpps::C * c)
{
}

cpps_export_finish