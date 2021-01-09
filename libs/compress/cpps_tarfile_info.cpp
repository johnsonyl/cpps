#include "cpps_tarfile_info.h"
namespace cpps {

	cpps_tarfile_info::cpps_tarfile_info()
	{
		file_size = 0;
		buf = NULL;
		needdelete = false;
	}

	cpps_tarfile_info::~cpps_tarfile_info()
	{
		if (needdelete && buf) delete[] buf;
		buf = NULL;
	}
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
	bool cpps_tarfile_info::isfile()
	{
		return header.typeflag == '\0' || header.typeflag == '0';
	}

	bool cpps_tarfile_info::isdir()
	{
		return header.typeflag == '5';
	}

	bool cpps_tarfile_info::issym()
	{
		return header.typeflag == '2';
	}

	bool cpps_tarfile_info::islnk()
	{
		return header.typeflag == '1';
	}

	bool cpps_tarfile_info::ischr()
	{
		return header.typeflag == '6';
	}

	bool cpps_tarfile_info::isblk()
	{
		return header.typeflag == '4';
	}

	bool cpps_tarfile_info::isfifo()
	{
		return header.typeflag == '3';
	}

	bool cpps_tarfile_info::isdev()
	{
		return ischr() || isfifo() || isblk();
	}

}