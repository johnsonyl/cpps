#include "stdafx.h"
#include "cpps_zipfile.h"
#include "cpps_zipfile_info.h"
#include <fstream>
namespace cpps 
{

	std::string cpps_io_getfilename(std::string str);
	std::string cpps_io_getfilepath(std::string str);
	cpps_integer cpps_io_mkdirs(std::string szdir);
	bool cpps_io_file_exists(std::string path);

	cpps_zipfile::cpps_zipfile()
	{
		filecomment_length = 0;
	}

	cpps_zipfile::~cpps_zipfile()
	{

	}

	bool cpps_zipfile::open(std::string file, cpps_value pwd, cpps_value mode, cpps_value level)
	{
		nlevel = level.tt == CPPS_TINTEGER ? level.value.integer : Z_BEST_COMPRESSION;
		smode = mode.tt == CPPS_TSTRING ? cpps_to_string(mode) : "r";
		password = mode.tt == CPPS_TSTRING ? cpps_to_string(pwd) : "";
		file_path = file;
		bool b = cpps_io_file_exists(file_path);
		int t = APPEND_STATUS_CREATE;
		/*解压缩 a为追加 也要先解压缩.*/
		if (smode == "r" ) {
			if (!b) return false;
			unzFile unzipfile = unzOpen64(file.c_str());
			if (unzipfile == NULL)
			{
				return false;
			}

			if (unzGetGlobalInfo64(unzipfile, &global_info) != UNZ_OK)
			{
				unzClose(unzipfile);
				return false;
			}

			unzGetGlobalComment(unzipfile, file_comment, READ_SIZE);
			filecomment_length = strlen(file_comment);

			ZPOS64_T i = 0;
			for (;i < global_info.number_entry; ++i)
			{
				cpps_zipfile_info* zipfile_info = new cpps_zipfile_info();

				//unzGetFilePos64
				if (unzGetCurrentFileInfo64(
					unzipfile,
					&zipfile_info->file_info,
					zipfile_info->file_name,
					MAX_FILENAME,
					zipfile_info->file_extra, READ_SIZE, zipfile_info->file_comment, READ_SIZE) != UNZ_OK)
				{
					unzClose(unzipfile);
					return false;
				}

				unzGetFilePos64(unzipfile, &zipfile_info->file_pos);
				zipfile_info->filename_length = strlen(zipfile_info->file_name);
				zipfile_info->fileextra_length = strlen(zipfile_info->file_extra);
				zipfile_info->filecomment_length = strlen(zipfile_info->file_comment);

		
				file_list.insert(phmap::flat_hash_map<std::string, cpps_zipfile_info*>::value_type(zipfile_info->file_name, zipfile_info));
				

				// Go the the next entry listed in the zip file.
				if ((i + 1) < global_info.number_entry)
				{
					if (unzGoToNextFile(unzipfile) != UNZ_OK)
					{
						unzClose(unzipfile);
						return false;
					}
				}

			}

			unzClose(unzipfile);
			return true;

		}
		else if(smode == "w" ){  //写入文件
		}
		else if (smode == "x") {
			if (b) return false;
		}
		else if (smode == "a"){
			if (!b) return false;
			t = APPEND_STATUS_ADDINZIP;
		}
		/*这里都是打开文件*/
		zipfile = zipOpen64(file_path.c_str(), t);
		if (zipfile == NULL) {
			return false;
		}

		return true;
	}

	void cpps_zipfile::close()
	{
		if(zipfile) zipClose(zipfile, NULL);
		zipfile = NULL;
	}

	cpps_zipfile_info* cpps_zipfile::getinfo(std::string name)
	{
		if (smode != "r") return NULL;

		cpps_zipfile_info* ret = NULL;
		auto it = file_list.find(name);
		if (it != file_list.end()) {
			ret = it->second;
		}
		return ret;
	}

	cpps_value cpps_zipfile::infolist(C *c)
	{
		if (smode != "r") return nil;
		cpps_vector* vec = NULL;
		cpps_value ret = newclass<cpps_vector>(c, &vec);
		vec->realvector().reserve(file_list.size());
		for (auto it : file_list) {
			vec->realvector().push_back(cpps_cpp_to_cpps_converter<cpps_zipfile_info*>::apply(c, it.second));
		}
		return ret;
	}

	cpps_value cpps_zipfile::namelist(C*c)
	{
		if (smode != "r") return nil;
		cpps_vector* vec = NULL;
		cpps_value ret = newclass<cpps_vector>(c, &vec);
		vec->realvector().reserve(file_list.size());
		for (auto it : file_list) {
			vec->realvector().push_back(cpps_value(c, it.second->file_name));
		}
		return ret;
	}

	bool cpps_zipfile::real_extract(unzFile zipfile,cpps_zipfile_info* member, std::string path)
	{
		// Check if this entry is a directory or file.
		if (!member->is_dir()){

			if (unzGoToFilePos64(zipfile, &member->file_pos) != UNZ_OK){
				return false;
			}
			if (password.empty() && unzOpenCurrentFile(zipfile) != UNZ_OK) {
				return false;
			}
			else if (!password.empty() && unzOpenCurrentFilePassword(zipfile, password.c_str()) != UNZ_OK) {
				return false;
			}
			std::string realpath = path + member->file_name;
			cpps_io_mkdirs(cpps_io_getfilepath(realpath));
			// Open a file to write out the data.
			FILE* out = fopen(realpath.c_str(), "wb");
			if (out == NULL){
				unzCloseCurrentFile(zipfile);
				unzClose(zipfile);
				return false;
			}
			char read_buffer[READ_SIZE];

			int error = UNZ_OK;
			do
			{
				error = unzReadCurrentFile(zipfile, read_buffer, READ_SIZE);
				if (error < 0)
				{
					unzCloseCurrentFile(zipfile);
					unzClose(zipfile);
					return false;
				}

				// Write data to file.
				if (error > 0)
				{
					fwrite(read_buffer, error, 1, out); // You should check return of fwrite...
				}
			} while (error > 0);

			fclose(out);
			unzCloseCurrentFile(zipfile);
			return true;
		}
		else {
			std::string realpath = path + member->file_name;
			cpps_io_mkdirs(realpath);
			return true;
		}
		return false;
	}
	bool cpps_zipfile::extract(cpps_value member, cpps_value path)
	{

		if (smode != "r") return false;

		cpps_zipfile_info* info = NULL;
		if (member.tt == CPPS_TNIL) {
			return false;
		}
		else if (member.tt == CPPS_TSTRING) {
			std::string* s = cpps_get_string(member);
			info = getinfo(*s);
		}
		else if (member.isdomain() && member.value.domain->domainname == "zipfile_info") {
			info = cpps_converter< cpps_zipfile_info*>::apply(member);
		}
		if (info == NULL) return false;

		unzFile zipfile = unzOpen64(file_path.c_str());
		if (zipfile == NULL)
		{
			return false;
		}
		std::string spath = path.tt == CPPS_TSTRING ? cpps_to_string(path) : "";
		real_extract(zipfile, info, spath);
		unzClose(zipfile);
		return true;
	}

	bool cpps_zipfile::extractall(cpps_value path, cpps_value members)
	{
		if (smode != "r") return false;
		unzFile zipfile = unzOpen64(file_path.c_str());
		if (zipfile == NULL)
		{
			return false;
		}
		std::string spath = path.tt == CPPS_TSTRING ? cpps_to_string(path) : "";
		cpps_vector* vec = NULL;
		if (members.isdomain() && members.value.domain->domainname == "vector") {
			vec = cpps_to_cpps_vector(members);
		}

		if (vec) {
			for (auto it : vec->realvector()) {
				cpps_zipfile_info* info = cpps_converter<cpps_zipfile_info*>::apply(it);
				real_extract(zipfile,info, spath);
			}
		}
		else {
			for (auto it : file_list) {
				cpps_zipfile_info* info = it.second;
				real_extract(zipfile, info, spath);
			}
		}

		unzClose(zipfile);
		return true;
	}

	void cpps_zipfile::setpassword(std::string pwd)
	{
		password = pwd;
	}
	void cpps_zipfile::real_read(cpps_zipfile_info* info,char *buf)
	{
		unzFile zipfile = unzOpen64(file_path.c_str());
		if (zipfile == NULL)
		{
			return ;
		}
		if (!info->is_dir()) {
			if (unzGoToFilePos64(zipfile, &info->file_pos) != UNZ_OK) {
				return ;
			}
			if (password.empty() && unzOpenCurrentFile(zipfile) != UNZ_OK) {
				return ;
			}
			else if (!password.empty() && unzOpenCurrentFilePassword(zipfile, password.c_str()) != UNZ_OK) {
				return ;
			}

			int err = unzReadCurrentFile(zipfile, buf, (usint32)info->file_info.uncompressed_size);
			if (err != (int)info->file_info.uncompressed_size) {
				printf("info->file_info.uncompressed_size:%d err:%d", (int)info->file_info.uncompressed_size, err);
			}
			unzCloseCurrentFile(zipfile);
		}
		unzClose(zipfile);
	}
	cpps_value cpps_zipfile::read(C *c,cpps_value member)
	{
		cpps_zipfile_info* info = NULL;
		if (member.tt == CPPS_TNIL) {
			return false;
		}
		else if (member.tt == CPPS_TSTRING) {
			std::string* s = cpps_get_string(member);
			info = getinfo(*s);
		}
		else if (member.isdomain() && member.value.domain->domainname == "zipfile_info") {
			info = cpps_converter< cpps_zipfile_info*>::apply(member);
		}
		if (info == NULL) return false;

		Buffer* buffer = NULL;
		cpps_value ret = newclass<Buffer>(c, &buffer);
		buffer->realloc(info->file_info.uncompressed_size);
		real_read(info, buffer->getbuffer());
		buffer->seek(info->file_info.uncompressed_size);

		return ret;
	}

	bool cpps_zipfile::testzip()
	{
		return true;
	}
	int writeInZipFile(zipFile zFile, const std::string& file)
	{
		std::fstream f(file.c_str(), std::ios::binary | std::ios::in);
		f.seekg(0, std::ios::end);
		size_t size = f.tellg();
		f.seekg(0, std::ios::beg);
		if (size <= 0)
		{
			return zipWriteInFileInZip(zFile, NULL, 0);
		}
		char* buf = new char[size];
		f.read(buf, size);
		int ret = zipWriteInFileInZip(zFile, buf,(usint32) size);
		delete[] buf;
		return ret;
	}
	bool cpps_zipfile::write(std::string filepath, cpps_value arcname)
	{
		//只有这三种情况下会写文件.
		if (smode == "w" || smode == "x" || smode == "a") {

			std::string sarcname = arcname.tt == CPPS_TSTRING ? cpps_to_string(arcname) : "";
			zip_fileinfo zi;
			memset(&zi,0, sizeof(zip_fileinfo));
			
			
			int err = zipOpenNewFileInZip3_64(zipfile, sarcname.empty() ? cpps_io_getfilename(filepath).c_str() : sarcname.c_str(), &zi, NULL, 0, NULL, 0, NULL, nlevel != 0 ? Z_DEFLATED : 0, (int)nlevel, 0,-MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY,
				password.empty()? NULL : password.c_str(), 0, 1);
			if (err != ZIP_OK)
			{
				return false;
			}
			err = writeInZipFile(zipfile, filepath);
			if (err != ZIP_OK)
			{
				return false;
			}

			return true;
		}
		return false;
	}

}