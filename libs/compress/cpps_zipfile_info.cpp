#include "stdafx.h"
#include "cpps_zipfile_info.h"

namespace cpps{

	cpps_zipfile_info::cpps_zipfile_info()
	{
		memset(file_name, 0, MAX_FILENAME);
		memset(file_comment, 0, READ_SIZE);
		memset(file_extra, 0, READ_SIZE);
		buf = NULL;
	}
	
	cpps_zipfile_info::~cpps_zipfile_info()
	{
		if (buf) {
			delete[] buf;
			buf = NULL;
		}
	}
	
	bool cpps_zipfile_info::is_dir()
	{
		return file_name[filename_length - 1] == dir_delimter;
	}

	std::string cpps_zipfile_info::filename()
	{
		return file_name;
	}

	std::string cpps_zipfile_info::data_time()
	{
		return "";
	}

	cpps_integer cpps_zipfile_info::compress_type()
	{
		return 1;//½öÖ§³Özip
	}

	std::string cpps_zipfile_info::comment()
	{
		return file_comment;
	}

	std::string cpps_zipfile_info::extra()
	{
		return file_extra;
	}

	std::string cpps_zipfile_info::create_system()
	{
		return "cpps os";
	}

	cpps_integer cpps_zipfile_info::create_version()
	{
		return (cpps_integer)file_info.version;
	}

	cpps_integer cpps_zipfile_info::extract_version()
	{
		return (cpps_integer)file_info.version_needed;
	}

	cpps_integer cpps_zipfile_info::compress_size()
	{
		return (cpps_integer)file_info.compressed_size;
	}

	cpps_integer cpps_zipfile_info::CRC()
	{
		return  (cpps_integer)file_info.crc;
	}

	cpps_integer cpps_zipfile_info::file_size()
	{
		return (cpps_integer)file_info.uncompressed_size;
	}

}
