#ifndef cpps_zlib_h__
#define cpps_zlib_h__
#include <cpps.h>
#include <zlib.h>

namespace cpps {
	/*
	zlib.compress(data,level = -1)
	*/
	cpps_value cpps_compress_zlib_compress(C* c, cpps_value data,cpps_value level);
	/*
	zlib.decompress(data,bufsize = DEF_BUF_SIZE,wbits = MAX_WBITS)
	*/
	cpps_value cpps_compress_zlib_decompress(C* c, cpps_value data, cpps_value bufsize, cpps_value wbits);
	/*
	zlib.adler32(data,adler = -1)
	*/
	cpps_integer cpps_compress_zlib_adler32(cpps_value data, cpps_value adler);
	/*
	zlib.adler32(data,adler = -1)
	*/
	cpps_integer cpps_compress_zlib_crc32(cpps_value data, cpps_value crc);

}

#endif // cpps_zlib_h__
