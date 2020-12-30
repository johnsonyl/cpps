// libdemo.cpp : 定义 DLL 应用程序的导出函数。
//
#include "stdafx.h"

#include <cpps/cpps.h>
#include <zlib.h>
#include "cpps_zlib.h"
#include "cpps_gzip.h"
#include "cpps_tarfile.h"
#include "cpps_zipfile.h"

using namespace cpps;
using namespace std;

cpps_value cpps_compress_tarfile_open(C*c,std::string filepath, cpps_value mode, cpps_value bufsize)
{
    cpps_tarfile* tarfile = NULL;
    cpps_value ret = newclass< cpps_tarfile>(c, &tarfile);
    bool b = tarfile->open(filepath, mode, bufsize);
    if (!b)return nil;
    return ret;
}
cpps_value cpps_compress_zipfile_open(C*c,std::string filepath, cpps_value pwd,cpps_value mode,cpps_value level)
{
    cpps_zipfile* zipfile = NULL;
    cpps_value ret = newclass< cpps_zipfile>(c, &zipfile);
    bool b = zipfile->open(filepath,pwd, mode, level);
	if (!b)return nil;
	return ret;
}

cpps_export_void  cpps_attach(cpps::C* c)
{

	cpps::cpps_init_cpps_class(c);
    cpps::_module(c, "zlib")[
        def_inside("compress", cpps_compress_zlib_compress),
        def_inside("decompress", cpps_compress_zlib_decompress),
        def("adler32", cpps_compress_zlib_adler32),
        def("crc32", cpps_compress_zlib_crc32),
        defvar(c,"Z_NO_COMPRESSION", Z_NO_COMPRESSION),
        defvar(c,"Z_BEST_SPEED", Z_BEST_SPEED),
        defvar(c,"Z_BEST_COMPRESSION", Z_BEST_COMPRESSION),
        defvar(c,"Z_DEFAULT_COMPRESSION", Z_DEFAULT_COMPRESSION),
        defvar(c,"ZLIB_VERSION",ZLIB_VERSION),
        def("ZLIB_RUNTIME_VERSION", zlibVersion)
	]; 
    cpps::_module(c, "gzip")[
		def_inside("compress", cpps_compress_gzip_compress),
		def_inside("decompress", cpps_compress_gzip_decompress),
		defvar(c, "Z_BEST_SPEED", Z_BEST_SPEED),
		defvar(c, "Z_BEST_COMPRESSION", Z_BEST_COMPRESSION),
		defvar(c, "Z_BEST_COMPRESSION", Z_BEST_COMPRESSION),
		defvar(c, "Z_DEFAULT_COMPRESSION", Z_DEFAULT_COMPRESSION)
	];

    cpps::_module(c, "tarfile")[
        _class< cpps_tarfile>("tarfile")
            .def("open",&cpps_tarfile::open)
            .def_inside("getmembers",&cpps_tarfile::getmembers)
            .def("extract",&cpps_tarfile::extract)
            .def("extractall",&cpps_tarfile::extractall)
            .def("close",&cpps_tarfile::close)
            .def_inside("gettarinfo",&cpps_tarfile::gettarinfo)
            .def("addfile",&cpps_tarfile::addfile)
            .def("getmember",&cpps_tarfile::getmember),
        _class< cpps_tarfile_info>("tarfile_info")
            .def("isblk",&cpps_tarfile_info::isblk)
            .def("ischr",&cpps_tarfile_info::ischr)
            .def("isdev",&cpps_tarfile_info::isdev)
            .def("isdir",&cpps_tarfile_info::isdir)
            .def("isfifo",&cpps_tarfile_info::isfifo)
            .def("islnk",&cpps_tarfile_info::islnk)
            .def("issym",&cpps_tarfile_info::issym)
            .def("isfile",&cpps_tarfile_info::isfile),
        def_inside("open", cpps_compress_tarfile_open)
    ];

    cpps::_module(c, "zipfile")[
		_class< cpps_zipfile>("zipfile")
			.def("open", &cpps_zipfile::open)
			.def_inside("infolist", &cpps_zipfile::infolist)
			.def_inside("namelist", &cpps_zipfile::namelist)
			.def("extract", &cpps_zipfile::extract)
			.def("extractall", &cpps_zipfile::extractall)
			.def("close", &cpps_zipfile::close)
			.def_inside("read", &cpps_zipfile::read)
			.def("write", &cpps_zipfile::write)
			.def("getinfo", &cpps_zipfile::getinfo),
			_class< cpps_zipfile_info>("zipfile_info")
			    .def("comment", &cpps_zipfile_info::comment)
			    .def("compress_size", &cpps_zipfile_info::compress_size)
			    .def("compress_type", &cpps_zipfile_info::compress_type)
			    .def("CRC", &cpps_zipfile_info::CRC)
			    .def("create_system", &cpps_zipfile_info::create_system)
			    .def("create_version", &cpps_zipfile_info::create_version)
			    .def("data_time", &cpps_zipfile_info::data_time)
			    .def("extra", &cpps_zipfile_info::extra)
			    .def("extract_version", &cpps_zipfile_info::extract_version)
			    .def("file_size", &cpps_zipfile_info::file_size)
			    .def("filename", &cpps_zipfile_info::filename)
			    .def("is_dir", &cpps_zipfile_info::is_dir),
			def_inside("open", cpps_compress_zipfile_open)
	];

    

   
}
cpps_export_void  cpps_detach(cpps::C * c)
{
}

cpps_export_finish