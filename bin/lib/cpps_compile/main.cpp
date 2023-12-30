#import "compress"

module cpps_compile
{
	var compile(var srcpath,var tarpath)
	{
		var out_buff = new Buffer();

		var buff = __compile(srcpath);
		var tarstr = zlib.compress(buff.tostring());
		
		out_buff.writeInt(sys.versionno);
		out_buff.writeInt(buff.length());
		out_buff.writeInt(tarstr.size());
		out_buff.writeString(tarstr);
		out_buff.writefile(tarpath);
		return true;
	}
}