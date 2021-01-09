#ifndef cpps_cpps_zipfile_info_h__
#define cpps_cpps_zipfile_info_h__

#include <cpps/cpps.h>
#include "minizip/zip.h"
#include "minizip/unzip.h"

#define dir_delimter '/'
#define MAX_FILENAME 512
#define READ_SIZE 8192

namespace cpps{

	class cpps_zipfile_info{
	public:
		cpps_zipfile_info();
		virtual ~cpps_zipfile_info();
	public:
		bool					is_dir();
		std::string				filename();
		std::string				data_time();
		cpps_integer			compress_type();
		std::string				comment();
		std::string				extra();
		std::string				create_system();
		cpps_integer			create_version();
		cpps_integer			extract_version();
		cpps_integer			compress_size();
		cpps_integer			CRC();
		cpps_integer			file_size();

		unz_file_info64			file_info;
		unz64_file_pos			file_pos;
		char					file_name[MAX_FILENAME];
		ZPOS64_T				filename_length;
		char					file_comment[READ_SIZE];
		ZPOS64_T				filecomment_length;
		char					file_extra[READ_SIZE];
		ZPOS64_T				fileextra_length;
		char					*buf;
	};
	
}
#endif //cpps_cpps_zipfile_info_h__