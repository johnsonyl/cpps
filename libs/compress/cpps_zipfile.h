#ifndef cpps_zipfile_h__
#define cpps_zipfile_h__
#include <cpps/cpps.h>
#include <zlib.h>
#include "cpps_zipfile_info.h"
#include "minizip/zip.h"
#include "minizip/unzip.h"

namespace cpps {

	class cpps_zipfile {
	public:
		cpps_zipfile();
		virtual ~cpps_zipfile();
		/*
		* file  地址 
		* pwd 为密码 没有则为空
		* mode 为 r 读取 x 创建并写入存在则失败, w 写入不存在则失败 a 追加  默认r
		* level 压缩等级 默认-1 
		* 暂时仅支持zip格式 bz2 lzma 暂时未支持
		*/
		bool			open(std::string file,cpps_value pwd, cpps_value mode, cpps_value level);
		/*
		* 返回:void
		*/
		void			close();
		/*
		* 返回:cpps::cpps_value 返回cpps_zipfile_info
		* std::string name 文件路径
		*/
		cpps_zipfile_info*		getinfo(std::string name);
		/*
		* 返回:cpps::cpps_value
		* C * c
		*/
		cpps_value		infolist(C* c);
		/*
		* 返回:cpps::cpps_value 返回为列表
		*/
		cpps_value		namelist(C* c);
		/*
		* 返回:bool
		* cpps_value member 可以是字符串 也可以是 cpps_zipfile_info
		* cpps_value path 解压路径 默认当前目录
		*/
		bool			extract(cpps_value member, cpps_value path);
		/*
		* 返回:bool
		* cpps_zipfile_info * member
		* std::string path
		*/
		bool			real_extract(unzFile zipfile, cpps_zipfile_info* member, std::string path);
		/*
		* 返回:bool
		* cpps_value path 解压路径 默认当前目录
		* cpps_value members cpps_zipfile_info列表
		*/
		bool			extractall(cpps_value path, cpps_value members);
		/*
		* 返回:void
		* std::string pwd
		*/
		void			setpassword(std::string pwd);
		/*
		* 返回:cpps::cpps_value
		* cpps_value name
		* 取出某一个文件的Buffer
		*/
		cpps_value		read(C* c, cpps_value name);
		void			real_read(cpps_zipfile_info* info, char* buf);
		/*
		* 返回:bool
		* 读取归档中的所有文件并检查它们的 CRC 和文件头。
		*/
		bool			testzip();
		/*
		* 返回:bool
		* std::string filepath 读取文件路径 
		* cpps_value arcname 相对zip的路径 空则写到root目录
		*/
		bool			write(std::string filepath, cpps_value arcname);

	public:
		std::string smode;
		std::string password;
		std::string file_path;
		char file_comment[READ_SIZE];
		ZPOS64_T filecomment_length;
		unz_global_info64 global_info;
		phmap::flat_hash_map<std::string, cpps_zipfile_info*>	file_list;
		zipFile zipfile;
		cpps_integer nlevel;
	};
}

#endif // cpps_zipfile_h__
