#include "stdafx.h"
#include "cpps_tarfile.h"
namespace cpps {
	bool cpps_io_file_exists(std::string path);
	std::string cpps_io_getfilename(std::string str);
	int gzcompress(Bytef* zdata, uLong* nzdata, Bytef* data, uLong ndata, cpps_integer level);
	int gzdecompress(
		Byte* data, uLong* ndata,
		Byte* zdata, uLong nzdata);
	std::string cpps_io_getfilepath(std::string str);
	cpps_integer cpps_io_mkdirs(std::string szdir);

	cpps_tarfile::cpps_tarfile()
	{
		decompress_file_buffer = NULL;
		filesize = 0;
	}

	cpps_tarfile::~cpps_tarfile()
	{
		close();
	}

	bool cpps_tarfile::open(std::string filepath, object vmode, cpps_value bufsize)
	{
		if (decompress_file_buffer) {
			return false;
		}
		cpps_integer nbufsize = bufsize.tt == CPPS_TINTEGER ? bufsize.value.integer : 10240;
		std::string mode = "r";
		if (vmode.isstring()) mode = vmode.tostring();

		size_t pos = mode.find(':');
		if (pos != std::string::npos) {
			realmode = mode.substr(0, pos);
			compresstype = mode.substr(pos + 1);
		}
		else {
			realmode = mode;
		}

		/*realmode = r x a w 4种模式
		* r 只读 如果文件不存在则失败
		* x 创建并写入 如果以存在则失败
		* a 追加 如果文件不存在 则创建
		* w 写入 如果文件不存在则失败.
		* 根据需求修改读取规则.
		*/
		bool b = cpps_io_file_exists(filepath);
		if (realmode == "r"){
			if(!b)
				return false;
			realmode = "rb";
		}
		else if (realmode == "x" ){
			if(b)
				return false;
			realmode = "wb+";
		}
		else if(realmode == "w") {
			realmode = "wb+";
		}
		else if (realmode == "a"){
			realmode = "ab+";
		}

		FILE* file = NULL;
		file = fopen(filepath.c_str(), realmode.c_str());
		if (!file) return false;
		takefilepath = filepath;

		if (realmode == "rb" || realmode == "ab+") {
			fseek(file, 0, SEEK_END);
			filesize = ftell(file);
			fseek(file, 0, SEEK_SET);

			Byte* source_file_buffer = new Byte[filesize];
			if (fread(source_file_buffer, filesize, 1, file)) {}
			fclose(file);

			if (compresstype.empty()) {
				decompress_file_buffer = source_file_buffer;
				source_file_buffer = NULL;
			}
			else if (compresstype == "gz") {

				decompress_file_buffer = new Byte[(size_t)nbufsize];
				uLong decompress_file_buffer_size = (uLong)nbufsize;
				int err = gzdecompress(decompress_file_buffer, &decompress_file_buffer_size, source_file_buffer, (uLong)filesize);
				if (err != Z_OK) {
					delete[] decompress_file_buffer;
					delete[] source_file_buffer;
					return false;
				}
				delete[] source_file_buffer;
				source_file_buffer = NULL;
				filesize = decompress_file_buffer_size;
			}
			else {
				/*暂时不支持*/
			}

			const int block_size{ 512 };
			tar_posix_header* header = NULL;


			/*验证tar文件*/
			if (filesize % block_size != 0) {
				delete[] decompress_file_buffer;
				return false;
			}

			pos = 0;

			while (pos < filesize) {
				header = (tar_posix_header*)(decompress_file_buffer + pos);
				if (strncmp(header->magic, TMAGIC, 5)) {
					break; /*末尾*/
				}

				pos += block_size;
				uLong file_size{ 0 };
				sscanf(header->size, "%lo", &file_size);
				size_t file_block_count = (file_size + block_size - 1) / block_size;

				cpps_tarfile_info* info = new cpps_tarfile_info();
				info->file_size = (size_t)file_size;
				info->buf = decompress_file_buffer + pos;
				info->header = *header;

				file_list.push_back( info);


				pos += file_block_count * block_size;
			}
		}
		
		return true;
	}

	cpps_tarfile_info* cpps_tarfile::getmember(std::string name)
	{
		cpps_tarfile_info* ret = NULL;
		for (auto it : file_list) {
			if (name == it->header.name) {
				ret = it;
				break;
			}
		}
		
		return ret;
	}

	cpps_value cpps_tarfile::getmembers(C* c)
	{
		cpps_vector* vec = NULL;
		cpps_value ret = newclass<cpps_vector>(c, &vec);
		vec->realvector().reserve(file_list.size());
		for (auto it : file_list) {
			vec->realvector().push_back(cpps_cpp_to_cpps_converter<cpps_tarfile_info*>::apply(c,it));
		}
		return ret;
	}

	void cpps_tarfile::extractall(cpps_value path, cpps_value members, cpps_value numeric_owner)
	{
		bool bnumeric_owner = numeric_owner.tt == CPPS_TBOOLEAN ? numeric_owner.value.b : false;
		std::string spath = path.tt == CPPS_TSTRING ? cpps_to_string(path) : "";
		cpps_vector* vec = NULL;
		if (members.isdomain() && members.value.domain->domainname == "vector") {
			vec = cpps_to_cpps_vector(members);
		}
		if (vec) {
			for (auto it : vec->realvector()) {
				cpps_tarfile_info* info = cpps_converter<cpps_tarfile_info*>::apply(it);
				real_extract(info, spath, true, bnumeric_owner);
			}
		}
		else {
			for (auto it : file_list) {
				cpps_tarfile_info* info = it;
				real_extract(info, spath, true, bnumeric_owner);
			}
		}
		
	}
	void cpps_tarfile::real_extract(cpps_tarfile_info* info, std::string spath, bool set_attrs, bool numeric_owner)
	{
		std::string realpath = spath + info->header.name;
		if (info->isfile()) {
			cpps_io_mkdirs(cpps_io_getfilepath(realpath));
			FILE* file = fopen(realpath.c_str(), "wb+");
			if (file) {
				fwrite(info->buf, info->file_size, 1, file);
				fclose(file);
			}
		}
		else if (info->isdir()) {
			cpps_io_mkdirs(realpath);
		}
		else
		{
			/*没遇见过呀.*/
		}
	}
	void cpps_tarfile::extract(cpps_value member, cpps_value path, cpps_value set_attrs, cpps_value numeric_owner)
	{
		cpps_tarfile_info* info = NULL;
		if (member.tt == CPPS_TNIL) {
			return;
		}
		else if (member.tt == CPPS_TSTRING){
			std::string* s = cpps_get_string(member);
			info = getmember(*s);
		}
		else if (member.isdomain() && member.value.domain->domainname == "tarfile_info"){
			info = cpps_converter< cpps_tarfile_info*>::apply(member);
		}
		if (info == NULL) return;
		std::string spath = path.tt == CPPS_TSTRING ? cpps_to_string(path) : "";
		bool bnumeric_owner = numeric_owner.tt == CPPS_TBOOLEAN ? numeric_owner.value.b : false;
		bool bset_attrs = set_attrs.tt == CPPS_TBOOLEAN ? set_attrs.value.b : true;
		real_extract(info, spath, bset_attrs, bnumeric_owner);
	}

	FILE* cpps_tarfile::extractfile(cpps_value member)
	{
		return NULL;
	}

	cpps_integer cpps_tarfile::close()
	{
		size_t block_size = 512;
		std::string source_file_buffer;
		Byte* save_file_buffer = NULL;
		size_t save_file_buffer_size = 0;
		if (realmode == "wb+" || realmode == "wb" || realmode == "ab+") {
			/*需要重新写入文件*/
			
			for (auto it : file_list)
			{
				cpps_tarfile_info* info = it;
				size_t file_block_count = (info->file_size + block_size - 1) / block_size * block_size;
				char* tmpbuffer = new char[file_block_count];
				memset(tmpbuffer, 0, file_block_count);
				memcpy(tmpbuffer, info->buf, info->file_size);
				source_file_buffer.append((const char *)&info->header, sizeof(tar_posix_header));
				source_file_buffer.append(tmpbuffer, file_block_count);
				delete[] tmpbuffer;
				save_file_buffer_size += sizeof(tar_posix_header) + file_block_count;
			}
			/*zero*/
			tar_posix_header zeroheader;
			source_file_buffer.append((char*)&zeroheader, sizeof(tar_posix_header));
			save_file_buffer_size += sizeof(tar_posix_header);
			if (compresstype.empty()) {
				save_file_buffer = (Byte*)source_file_buffer.c_str();
				FILE* file = fopen(takefilepath.c_str(), "wb+");
				if (file) {
					fwrite(save_file_buffer, save_file_buffer_size, 1, file);
					fclose(file);
				}
			}
			else if (compresstype == "gz") {
				uLong compress_buf_size = (uLong)save_file_buffer_size + 20;
				Byte* compress_buf = new Byte[compress_buf_size];

				int err = gzcompress(compress_buf, &compress_buf_size, (Byte*)source_file_buffer.c_str(), (uLong)source_file_buffer.size(),-1);
				if (err != Z_OK)
				{
					delete[] compress_buf;
					return -1;
				}
				
				FILE* file = fopen(takefilepath.c_str(), "wb+");
				if (file) {

					fwrite(compress_buf, compress_buf_size, 1, file);
					fclose(file);
				}
				delete[] compress_buf;
			}
		}

		if (decompress_file_buffer) delete[] decompress_file_buffer;
		decompress_file_buffer = NULL;
		return save_file_buffer_size;
	}

	void cpps_tarfile::add(std::string name, cpps_value arcname, cpps_value recursive, cpps_value filter)
	{
		/*不理解是做啥的.*/
	}

	void cpps_tarfile::addfile(cpps_tarfile_info* info, cpps_value fileobj)
	{
		if (!info) return;
		cpps_tarfile_info* cpyinfo = new cpps_tarfile_info();
		cpyinfo->buf = info->buf;
		cpyinfo->file_size = info->file_size;
		cpyinfo->header = info->header;
		cpyinfo->needdelete = info->needdelete;
		file_list.push_back(cpyinfo);
		info->needdelete = false; //交换控制权
	}


	std::string cpps_tarfile_getusername()
	{
#if defined _WIN32   //linux system  
		const int MAX_LEN = 100;
		char szBuffer[MAX_LEN];
		DWORD len = MAX_LEN;
		GetUserName(szBuffer, &len);     //用户名保存在szBuffer中,len是用户名的长度  
		return szBuffer;
#elif defined LINUX  
		uid_t userid;
		struct passwd* pwd;
		userid = getuid();
		pwd = getpwuid(userid);
		return pwd->pw_name;
#elif defined __APPLE__
		return getlogin();
#endif
	}

	std::string cpps_tarfile_getgroupname() {
#if defined _WIN32
		return cpps_tarfile_getusername();
#elif defined LINUX  
		uid_t gid;
		gid = getgid();
		struct group* g = getgrgid(gid);
		if (g) {
			return g->gr_name;
		}
		return "";
#elif defined __APPLE__
		return "staff"; //
#endif
	}

	cpps_tarfile_info* cpps_tarfile::gettarinfo(C *c,std::string name, cpps_value arcname)
	{
		std::string sarcname = arcname.tt == CPPS_TSTRING ? cpps_to_string(arcname) : "";

		cpps_tarfile_info* info = NULL;
		cpps_value ret = newclass< cpps_tarfile_info>(c, &info);
#ifdef WIN32
		struct _stat64 statinfo;
		_stati64(name.c_str(),&statinfo);
#else
		struct stat statinfo;
		stat(name.c_str(), &statinfo);
#endif
		/*
		char name[100];
		char mode[8];
		char uid[8];
		char gid[8];
		char size[12];
		char mtime[12];
		char chksum[8];
		char typeflag;
		char linkname[100];
		char magic[6];
		char version[2];
		char uname[32];
		char gname[32];
		char devmajor[8];
		char devminor[8];
		char prefix[155];
		*/
		strcpy(info->header.name, sarcname.empty() ?  cpps_io_getfilename(name).c_str() : sarcname.c_str());
		sprintf(info->header.mode, "%06d ", (usint16)statinfo.st_mode); info->header.mode[7] = '\0';
		sprintf(info->header.uid, "%06d ", (usint16)statinfo.st_uid); info->header.uid[7] = '\0';
		sprintf(info->header.gid, "%06d ", (usint16)statinfo.st_gid); info->header.gid[7] = '\0';
		sprintf(info->header.size, "%011o", (usint32)statinfo.st_size); info->header.size[11] = ' ';  //size mtime chksum 为8进制
		sprintf(info->header.mtime, "%011o", (usint32)statinfo.st_mtime); info->header.mtime[11] = ' ';
		//校验和先设置空格最后计算值
		for (int i = 0; i < 8; i++)
			info->header.chksum[i] = 0x20;
		/*
		'0'intentionally dropping through
		'\0'normal file
		'1'hard link
		'2'symbolic link
		'3'device file/special file
		'4'block device
		'5'directory
		'6'named pipe
		*/
		if (S_ISDIR(statinfo.st_mode)) {
			info->header.typeflag = '5';
		}
		else if (S_ISLNK(statinfo.st_mode)) {
			info->header.typeflag = '1';
		}
		else if (S_ISCHR(statinfo.st_mode)) {
			info->header.typeflag = '2';
		}
		else if (S_ISBLK(statinfo.st_mode)) {
			info->header.typeflag = '4';
		}
		else if (S_ISFIFO(statinfo.st_mode)) {
			info->header.typeflag = '6';
		}
		else if (S_ISREG(statinfo.st_mode)) {
			info->header.typeflag = '0';
		}
		else {
			info->header.typeflag = '3';
		}
		strcpy(info->header.magic, TMAGIC);
		info->header.version[0] = '0';
		info->header.version[1] = '0';
		strcpy(info->header.uname, cpps_tarfile_getusername().c_str());
		strcpy(info->header.gname, cpps_tarfile_getgroupname().c_str());
		sprintf(info->header.devmajor, "000000 "); info->header.devmajor[7] = '\0';
		sprintf(info->header.devminor, "000000 "); info->header.devminor[7] = '\0';

		
		// 计算校验和
		unsigned int sum = 0;
		for (int i = 0; i < 512; i++) {
			sum += ((unsigned char*)&(info->header))[i];
		}
		sprintf(info->header.chksum, "%06o", (usint16)sum); info->header.chksum[6] = '\0';  info->header.chksum[7] = ' ';

		info->buf = new Byte[statinfo.st_size];
		FILE* file = fopen(name.c_str(), "rb");
		if (file) {
			if (fread(info->buf, statinfo.st_size, 1, file)) {}
			fclose(file);
			info->file_size = statinfo.st_size;
		}
		info->needdelete = true;
			
		return info;

	}
}