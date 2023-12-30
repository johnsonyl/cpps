#import "compress"

var __cxo_to_buffer(var _uncompress_content)
{
	var buffer = new Buffer();
	buffer.writeString(_uncompress_content);
	buffer.seek(0);
	var _version = buffer.readInt();
	var _src_size = buffer.readInt();
	var _compress_size = buffer.readInt();
	var _compress_content = buffer.readString(_compress_size);
	var _src_content = zlib.decompress(_compress_content,_src_size);
	return _src_content;
}