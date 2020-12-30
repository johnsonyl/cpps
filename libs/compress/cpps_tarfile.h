#ifndef cpps_tarfile_h__
#define cpps_tarfile_h__
#include <cpps/cpps.h>
#include <zlib.h>
#include "cpps_tarfile_info.h"
namespace cpps {
	


#define TMAGIC   "ustar"
#define TMAGLEN  6
#define TVERSION "00"   
#define TVERSLEN 2

#define REGTYPE  '0'
#define AREGTYPE '\0'
#define LNKTYPE  '1'
#define SYMTYPE  '2'
#define CHRTYPE  '3'
#define BLKTYPE  '4'
#define DIRTYPE  '5'
#define FIFOTYPE '6'
#define CONTTYPE '7'




	
	
	class cpps_tarfile
	{
	public:
		cpps_tarfile();
		virtual ~cpps_tarfile();

	public:
		/*
		* open 打开一个文件
		*/
		bool			open(std::string filepath, object vmode,cpps_value bufsize);
		/*
		* 获取一个成员 不存在则返回空
		*/
		cpps_tarfile_info* getmember(std::string name);
		/*
		* 获取成员列表
		*/
		cpps_value		getmembers(C* c);
		/*
		* 解压所有成员
		*/
		void			extractall(cpps_value path, cpps_value members, cpps_value numeric_owner);
		/*
		* 解压单个成员
		*/
		void			extract(cpps_value member, cpps_value path, cpps_value set_attrs, cpps_value numeric_owner);
		void			real_extract(cpps_tarfile_info* info, std::string spath, bool set_attrs, bool numeric_owner);
		/*
		* 获取解压文件FILE
		*/
		FILE*			extractfile(cpps_value member);
		/*
		* 关闭 在写入模式下，会向归档添加两个表示结束的零数据块。
		* 返回  压缩前原始文件大小
		*/
		cpps_integer			close();
		/*
		* 不知道干啥的.
		*/
		void			add(std::string name, cpps_value arcname, cpps_value recursive, cpps_value filter);
		/*
		* 添加一个文件
		*/
		void			addfile(cpps_tarfile_info* info, cpps_value fileobj);
		/*
		* 创建一个文件info
		*/
		cpps_tarfile_info* gettarinfo(C* c, std::string name,cpps_value arcname);
	public:
		size_t filesize;
		std::vector<cpps_tarfile_info* >	file_list;
		Byte* decompress_file_buffer;
		std::string realmode;
		std::string compresstype;
		std::string takefilepath;

	};


}

#endif // cpps_tarfile_h__
