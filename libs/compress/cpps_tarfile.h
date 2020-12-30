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
		* open ��һ���ļ�
		*/
		bool			open(std::string filepath, object vmode,cpps_value bufsize);
		/*
		* ��ȡһ����Ա �������򷵻ؿ�
		*/
		cpps_tarfile_info* getmember(std::string name);
		/*
		* ��ȡ��Ա�б�
		*/
		cpps_value		getmembers(C* c);
		/*
		* ��ѹ���г�Ա
		*/
		void			extractall(cpps_value path, cpps_value members, cpps_value numeric_owner);
		/*
		* ��ѹ������Ա
		*/
		void			extract(cpps_value member, cpps_value path, cpps_value set_attrs, cpps_value numeric_owner);
		void			real_extract(cpps_tarfile_info* info, std::string spath, bool set_attrs, bool numeric_owner);
		/*
		* ��ȡ��ѹ�ļ�FILE
		*/
		FILE*			extractfile(cpps_value member);
		/*
		* �ر� ��д��ģʽ�£�����鵵���������ʾ�����������ݿ顣
		* ����  ѹ��ǰԭʼ�ļ���С
		*/
		cpps_integer			close();
		/*
		* ��֪����ɶ��.
		*/
		void			add(std::string name, cpps_value arcname, cpps_value recursive, cpps_value filter);
		/*
		* ���һ���ļ�
		*/
		void			addfile(cpps_tarfile_info* info, cpps_value fileobj);
		/*
		* ����һ���ļ�info
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
