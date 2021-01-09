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
		* file  ��ַ 
		* pwd Ϊ���� û����Ϊ��
		* mode Ϊ r ��ȡ x ������д�������ʧ��, w д�벻������ʧ�� a ׷��  Ĭ��r
		* level ѹ���ȼ� Ĭ��-1 
		* ��ʱ��֧��zip��ʽ bz2 lzma ��ʱδ֧��
		*/
		bool			open(std::string file,cpps_value pwd, cpps_value mode, cpps_value level);
		/*
		* ����:void
		*/
		void			close();
		/*
		* ����:cpps::cpps_value ����cpps_zipfile_info
		* std::string name �ļ�·��
		*/
		cpps_zipfile_info*		getinfo(std::string name);
		/*
		* ����:cpps::cpps_value
		* C * c
		*/
		cpps_value		infolist(C* c);
		/*
		* ����:cpps::cpps_value ����Ϊ�б�
		*/
		cpps_value		namelist(C* c);
		/*
		* ����:bool
		* cpps_value member �������ַ��� Ҳ������ cpps_zipfile_info
		* cpps_value path ��ѹ·�� Ĭ�ϵ�ǰĿ¼
		*/
		bool			extract(cpps_value member, cpps_value path);
		/*
		* ����:bool
		* cpps_zipfile_info * member
		* std::string path
		*/
		bool			real_extract(unzFile zipfile, cpps_zipfile_info* member, std::string path);
		/*
		* ����:bool
		* cpps_value path ��ѹ·�� Ĭ�ϵ�ǰĿ¼
		* cpps_value members cpps_zipfile_info�б�
		*/
		bool			extractall(cpps_value path, cpps_value members);
		/*
		* ����:void
		* std::string pwd
		*/
		void			setpassword(std::string pwd);
		/*
		* ����:cpps::cpps_value
		* cpps_value name
		* ȡ��ĳһ���ļ���Buffer
		*/
		cpps_value		read(C* c, cpps_value name);
		void			real_read(cpps_zipfile_info* info, char* buf);
		/*
		* ����:bool
		* ��ȡ�鵵�е������ļ���������ǵ� CRC ���ļ�ͷ��
		*/
		bool			testzip();
		/*
		* ����:bool
		* std::string filepath ��ȡ�ļ�·�� 
		* cpps_value arcname ���zip��·�� ����д��rootĿ¼
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
