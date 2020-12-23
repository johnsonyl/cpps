#ifndef cpps_tarfile_info_h__
#define cpps_tarfile_info_h__
#include <cpps/cpps.h>
#include <zlib.h>
namespace cpps {

	typedef struct posix_header
	{
		posix_header() {
			memset(this, 0, sizeof(posix_header));
		}
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
		char padding[12];
	} tar_posix_header;

	class cpps_tarfile_info
	{
	public:
		cpps_tarfile_info();
		virtual ~cpps_tarfile_info();

		bool		isfile();
		bool		isdir();
		bool		issym();
		bool		islnk();
		bool		ischr();
		bool		isblk();
		bool		isfifo();
		bool		isdev();
	public:
		size_t file_size;
		posix_header header;
		Byte	*buf;
		bool	needdelete;
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
	};
}

#endif // cpps_tarfile_info_h__
