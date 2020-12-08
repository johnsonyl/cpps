#ifndef cpps_gzip_h__
#define cpps_gzip_h__
#include <cpps.h>
#include <zlib.h>


namespace cpps {

	/*
	gzip.compress(data,level = -1)
	*/
	cpps_value cpps_compress_gzip_compress(C* c, cpps_value data, cpps_value level);
	/*
	gzip.decompress(data,bufsize = DEF_BUF_SIZE,wbits = MAX_WBITS)
	*/
	cpps_value cpps_compress_gzip_decompress(C* c, cpps_value data, cpps_value bufsize);
	
}

#endif // cpps_gzip_h__
