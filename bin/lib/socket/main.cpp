#import "uuid"
#import "hashlib"
class ServerOption
{
	var ip;
	var accept;
	var data;
	var close;
	var parser;
	var headersize;
}
class ClientOption
{
	var connected;
	var data;
	var close;
	var parser;
	var headersize;
}
class HttpServerOption
{
	var ip;
	var exceptionfunc;
	var notfoundfunc;
}
class HttpTemplateCache
{
	var htmlpath = "";
	var __htmltextblock = [];
	var __s = "";
	var __headers = "";
	var lastchangetime;
	var __node;
}
var 	__socket_httpserver_createuuid()
{
	var sha256 = new hashlib::sha256();
	sha256.update(uuid.uuid4());
	return sha256.hexdigest();
}
var __http__template__cache__ = {};
var render(var request,var __html,var __data)
{
	request.addheader({
		Server:"cpps Server",
		"Content-Type":"text/html; charset=UTF-8",
		Connection:"close",
		"Access-Control-Allow-Origin":"*"
	});
	
	var __htmltextblock;

	var __cache = __http__template__cache__.find(__html);
	if(__cache == null){
		__cache = new HttpTemplateCache();

		foreach(var kv:__data){
			__cache.__headers ..= "var {kv.first()} = __data['{kv.first()}'];\n";
		}
		__cache.htmlpath = "{io.getcwd()}/{__html}";
		__cache.__s = socket.prasehtml2str(request,__cache.htmlpath,__cache.__htmltextblock);
		__cache.lastchangetime = io.last_write_time(__cache.htmlpath);
		__cache.__node = parse("{__cache.__headers}{__cache.__s}");
		__http__template__cache__.insert(__html,__cache);
	}
	else {
		var lasttime = io.last_write_time(__cache.htmlpath);
		if(__cache.lastchangetime != lasttime){
			__cache.__htmltextblock.clear();
			__cache.__s = socket.prasehtml2str(request,__cache.htmlpath,__cache.__htmltextblock);
			__cache.__node.release();
			__cache.__node = parse("{__cache.__headers}{__cache.__s}");
			__cache.lastchangetime = lasttime;
		}
	}

	__htmltextblock = __cache.__htmltextblock;
	setechofunc(request.append,request);
	donode(__cache.__node);
	setechofunc(nil);
	request.send(200,"OK");
}


module socket
{
	class Echo_handler{
		var __holder = "";
		var EchoFunc(var s){ __holder.append(s);}
	}
	var render2str(var __html,var __data = {})
	{
		var __htmltextblock;
		var holder = new socket::Echo_handler();
		
		var __cache = __http__template__cache__.find(__html);
		if(__cache == null){
			__cache = new HttpTemplateCache();
			
			foreach(var kv:__data){
				__cache.__headers ..= "var {kv.first()} = __data['{kv.first()}'];\n";
			}
			__cache.htmlpath = "{io.getcwd()}/{__html}";
			__cache.__s = socket.prasehtml2str(null,__cache.htmlpath,__cache.__htmltextblock);
			__cache.lastchangetime = io.last_write_time(__cache.htmlpath);
			__cache.__node = parse("{__cache.__headers}{__cache.__s}");
			__http__template__cache__.insert(__html,__cache);	
		}
		__htmltextblock = __cache.__htmltextblock;
		setechofunc(holder.EchoFunc,holder);
		donode(__cache.__node);
		setechofunc(nil);
		return holder.__holder;
	}
	var initmimetype(var server)
	{
		server.add_type("x-world/x-3dmf","3dm");
		server.add_type("x-world/x-3dmf","3dmf");
		server.add_type("application/octet-stream","a");
		server.add_type("application/x-authorware-bin","aab");
		server.add_type("application/x-authorware-map","aam");
		server.add_type("application/x-authorware-seg","aas");
		server.add_type("text/vnd.abc","abc");
		server.add_type("text/html","acgi");
		server.add_type("video/animaflex","afl");
		server.add_type("application/postscript","ai");
		server.add_type("audio/aiff","aif");
		server.add_type("audio/x-aiff","aif");
		server.add_type("audio/aiff","aifc");
		server.add_type("audio/aiff","aiff");
		server.add_type("application/x-aim","aim");
		server.add_type("text/x-audiosoft-intra","aip");
		server.add_type("application/x-navi-animation","ani");
		server.add_type("application/x-nokia-9000-communicator-add-on-software","aos");
		server.add_type("application/mime","aps");
		server.add_type("application/octet-stream","arc");
		server.add_type("application/octet-stream","arj");
		server.add_type("image/x-jg","art");
		server.add_type("video/x-ms-asf","asf");
		server.add_type("text/x-asm","asm");
		server.add_type("text/asp","asp");
		server.add_type("application/x-mplayer2","asx");
		server.add_type("audio/x-au","au");
		server.add_type("video/avi","avi");
		server.add_type("video/avs-video","avs");
		server.add_type("application/x-bcpio","bcpio");
		server.add_type("application/octet-stream","bin");
		server.add_type("image/bmp","bm");
		server.add_type("image/bmp","bmp");
		server.add_type("application/book","boo");
		server.add_type("application/book","book");
		server.add_type("application/x-bzip2","boz");
		server.add_type("application/x-bsh","bsh");
		server.add_type("application/x-bzip","bz");
		server.add_type("application/x-bzip2","bz2");
		server.add_type("application/vnd.ms-pki.seccat","cat");
		server.add_type("application/clariscad","ccad");
		server.add_type("application/x-cocoa","cco");
		server.add_type("application/cdf","cdf");
		server.add_type("application/pkix-cert","cer");
		server.add_type("application/x-x509-ca-cert","cer");
		server.add_type("application/x-chat","cha");
		server.add_type("application/x-chat","chat");
		server.add_type("application/java","class");
		server.add_type("text/plain","com");
		server.add_type("application/x-cpio","cpio");
		server.add_type("application/mac-compactpro","cpt");
		server.add_type("application/x-compactpro","cpt");
		server.add_type("application/x-cpt","cpt");
		server.add_type("application/pkix-crl","crl");
		server.add_type("application/pkix-cert","crt");
		server.add_type("application/x-csh","csh");
		server.add_type("text/css","css");
		server.add_type("application/x-director","dcr");
		server.add_type("application/x-deepv","deepv");
		server.add_type("text/plain","def");
		server.add_type("application/x-x509-ca-cert","der");
		server.add_type("video/x-dv","dif");
		server.add_type("application/x-director","dir");
		server.add_type("video/dl","dl");
		server.add_type("video/x-dl","dl");
		server.add_type("application/msword","doc");
		server.add_type("application/msword","dot");
		server.add_type("application/commonground","dp");
		server.add_type("application/drafting","drw");
		server.add_type("application/octet-stream","dump");
		server.add_type("application/octet-stream","dmp");
		server.add_type("video/x-dv","dv");
		server.add_type("application/x-dvi","dvi");
		server.add_type("model/vnd.dwf","dwf");
		server.add_type("image/vnd.dwg","dwg");
		server.add_type("image/vnd.dwg","dxf");
		server.add_type("application/x-director","dxr");
		server.add_type("text/x-script.elisp","el");
		server.add_type("application/x-elc","elc");
		server.add_type("application/x-envoy","env");
		server.add_type("application/postscript","eps");
		server.add_type("application/x-esrehber","es");
		server.add_type("text/x-setext","etx");
		server.add_type("application/x-envoy","evy");
		server.add_type("application/octet-stream","exe");
		server.add_type("application/octet-stream","rar");
		server.add_type("application/octet-stream","xz");
		server.add_type("text/plain","f");
		server.add_type("text/x-fortran","f");
		server.add_type("text/x-fortran","f77");
		server.add_type("text/x-fortran","f90");
		server.add_type("application/vnd.fdf","fdf");
		server.add_type("image/fif","fif");
		server.add_type("video/fli","fli");
		server.add_type("image/florian","flo");
		server.add_type("text/vnd.fmi.flexstor","flx");
		server.add_type("video/x-atomic3d-feature","fmf");
		server.add_type("text/x-fortran","for");
		server.add_type("image/vnd.fpx","fpx");
		server.add_type("application/freeloader","frl");
		server.add_type("audio/make","funk");
		server.add_type("text/plain","g");
		server.add_type("image/g3fax","g3");
		server.add_type("image/gif","gif");
		server.add_type("video/gl","gl");
		server.add_type("video/x-gl","gl");
		server.add_type("audio/x-gsm","gsd");
		server.add_type("audio/x-gsm","gsm");
		server.add_type("application/x-gsp","gsp");
		server.add_type("application/x-gss","gss");
		server.add_type("application/x-gtar","gtar");
		server.add_type("application/x-compressed","gz");
		server.add_type("application/x-gzip","gz");
		server.add_type("application/x-gzip","gzip");
		server.add_type("application/x-hdf","hdf");
		server.add_type("application/x-helpfile","help");
		server.add_type("application/vnd.hp-hpgl","hgl");
		server.add_type("text/x-script","hlb");
		server.add_type("application/hlp","hlp");
		server.add_type("application/x-helpfile","hlp");
		server.add_type("application/vnd.hp-hpgl","hpg");
		server.add_type("application/vnd.hp-hpgl","hpgl");
		server.add_type("application/binhex","hqx");
		server.add_type("application/hta","hta");
		server.add_type("text/x-component","htc");
		server.add_type("text/html","htm");
		server.add_type("text/html","html");
		server.add_type("text/html","htmls");
		server.add_type("text/webviewhtml","htt");
		server.add_type("text/html","htx");
		server.add_type("x-conference/x-cooltalk","ice");
		server.add_type("image/x-icon","ico");
		server.add_type("text/plain","idc");
		server.add_type("image/ief","ief");
		server.add_type("image/ief","iefs");
		server.add_type("application/iges","igs");
		server.add_type("application/x-httpd-imap","imap");
		server.add_type("application/inf","inf");
		server.add_type("application/x-internett-signup","ins");
		server.add_type("application/x-ip2","ip");
		server.add_type("video/x-isvideo","isu");
		server.add_type("audio/it","it");
		server.add_type("application/x-inventor","iv");
		server.add_type("i-world/i-vrml","ivr");
		server.add_type("application/x-livescreen","ivy");
		server.add_type("audio/x-jam","jam");
		server.add_type("text/x-java-source","jav");
		server.add_type("text/x-java-source","java");
		server.add_type("application/x-java-commerce","jcm");
		server.add_type("image/jpeg","jfif");
		server.add_type("image/jpeg","jfif-tbnl");
		server.add_type("image/jpeg","jpe");
		server.add_type("image/jpeg","jpeg");
		server.add_type("image/jpeg","jpg");
		server.add_type("image/x-jps","jps");
		server.add_type("application/x-javascript","js");
		server.add_type("image/jutvision","jut");
		server.add_type("audio/midi","kar");
		server.add_type("music/x-karaoke","kar");
		server.add_type("text/x-script.ksh","ksh");
		server.add_type("audio/nspaudio","la");
		server.add_type("audio/x-liveaudio","lam");
		server.add_type("application/x-latex","latex");
		server.add_type("application/lha","lha");
		server.add_type("application/octet-stream","lhx");
		server.add_type("text/plain","list");
		server.add_type("audio/nspaudio","lma");
		server.add_type("audio/x-nspaudio","lma");
		server.add_type("text/plain","log");
		server.add_type("application/x-lisp","lsp");
		server.add_type("text/x-script.lisp","lsp");
		server.add_type("text/plain","lst");
		server.add_type("text/x-la-asf","lsx");
		server.add_type("application/x-latex","ltx");
		server.add_type("application/octet-stream","lzh");
		server.add_type("application/octet-stream","lzx");
		server.add_type("text/x-m","m");
		server.add_type("video/mpeg","m1v");
		server.add_type("audio/mpeg","m2a");
		server.add_type("video/mpeg","m2v");
		server.add_type("audio/x-mpequrl","m3u");
		server.add_type("application/x-troff-man","man");
		server.add_type("application/x-navimap","map");
		server.add_type("text/plain","mar");
		server.add_type("application/mbedlet","mbd");
		server.add_type("application/mcad","mcd");
		server.add_type("image/vasa","mcf");
		server.add_type("application/netmc","mcp");
		server.add_type("application/x-troff-me","me");
		server.add_type("message/rfc822","mht");
		server.add_type("message/rfc822","mhtml");
		server.add_type("audio/x-midi","midi");
		server.add_type("application/x-mif","mif");
		server.add_type("www/mime","mime");
		server.add_type("audio/x-vnd.audioexplosion.mjuicemediafile","mjf");
		server.add_type("video/x-motion-jpeg","mjpg");
		server.add_type("application/base64","mm");
		server.add_type("application/base64","mme");
		server.add_type("audio/mod","mod");
		server.add_type("video/quicktime","moov");
		server.add_type("video/quicktime","mov");
		server.add_type("video/x-sgi-movie","movie");
		server.add_type("audio/x-mpeg","mp2");
		server.add_type("audio/x-mpeg-3","mp3");
		server.add_type("video/x-mpeg-4","mp4");
		server.add_type("video/mpeg","mpa");
		server.add_type("video/mpeg","mpe");
		server.add_type("video/mpeg","mpeg");
		server.add_type("video/mpeg","mpg");
		server.add_type("audio/mpeg","mpga");
		server.add_type("application/vnd.ms-project","mpp");
		server.add_type("application/x-project","mpt");
		server.add_type("application/x-project","mpv");
		server.add_type("application/x-project","mpx");
		server.add_type("application/marc","mrc");
		server.add_type("application/x-troff-ms","ms");
		server.add_type("video/x-sgi-movie","mv");
		server.add_type("audio/make","my");
		server.add_type("application/x-vnd.audioexplosion.mzz","mzz");
		server.add_type("image/naplps","nap");
		server.add_type("image/naplps","naplps");
		server.add_type("application/x-netcdf","nc");
		server.add_type("application/vnd.nokia.configuration-message","ncm");
		server.add_type("image/x-niff","nif");
		server.add_type("image/x-niff","niff");
		server.add_type("application/x-mix-transfer","nix");
		server.add_type("application/x-conference","nsc");
		server.add_type("application/x-navidoc","nvd");
		server.add_type("application/octet-stream","o");
		server.add_type("application/oda","oda");
		server.add_type("application/x-omc","omc");
		server.add_type("application/x-omcdatamaker","omcd");
		server.add_type("application/x-omcregerator","omcr");
		server.add_type("text/x-pascal","p");
		server.add_type("application/pkcs10","p10");
		server.add_type("application/pkcs-12","p12");
		server.add_type("application/x-pkcs7-signature","p7a");
		server.add_type("application/pkcs7-mime","p7c");
		server.add_type("application/pkcs7-mime","p7m");
		server.add_type("application/x-pkcs7-certreqresp","p7r");
		server.add_type("application/pro_eng","part");
		server.add_type("text/pascal","pas");
		server.add_type("image/x-portable-bitmap","pbm");
		server.add_type("application/vnd.hp-pcl","pcl");
		server.add_type("image/x-pict","pct");
		server.add_type("image/x-pcx","pcx");
		server.add_type("chemical/x-pdb","pdb");
		server.add_type("application/pdf","pdf");
		server.add_type("audio/make","pfunk");
		server.add_type("image/x-portable-graymap","pgm");
		server.add_type("image/pict","pic");
		server.add_type("image/pict","pict");
		server.add_type("application/x-newton-compatible-pkg","pkg");
		server.add_type("application/vnd.ms-pki.pko","pko");
		server.add_type("text/x-script.perl","pl");
		server.add_type("application/x-pixclscript","plx");
		server.add_type("image/x-xpixmap","pm");
		server.add_type("application/x-pagemaker","pm4");
		server.add_type("application/x-pagemaker","pm5");
		server.add_type("image/png","png");
		server.add_type("application/x-portable-anymap","pnm");
		server.add_type("application/mspowerpoint","pot");
		server.add_type("model/x-pov","pov");
		server.add_type("application/vnd.ms-powerpoint","ppa");
		server.add_type("image/x-portable-pixmap","ppm");
		server.add_type("application/mspowerpoint","pps");
		server.add_type("application/x-mspowerpoint","ppt");
		server.add_type("application/mspowerpoint","ppz");
		server.add_type("application/x-freelance","pre");
		server.add_type("application/pro_eng","prt");
		server.add_type("application/postscript","ps");
		server.add_type("application/octet-stream","psd");
		server.add_type("paleovu/x-pv","pvu");
		server.add_type("application/vnd.ms-powerpoint","pwz");
		server.add_type("text/x-script.phyton","py");
		server.add_type("application/x-bytecode.python","pyc");
		server.add_type("audio/vnd.qcelp","qcp");
		server.add_type("x-world/x-3dmf","qd3");
		server.add_type("x-world/x-3dmf","qd3d");
		server.add_type("image/x-quicktime","qif");
		server.add_type("video/quicktime","qt");
		server.add_type("video/x-qtc","qtc");
		server.add_type("image/x-quicktime","qti");
		server.add_type("image/x-quicktime","qtif");
		server.add_type("audio/x-realaudio","ra");
		server.add_type("audio/x-pn-realaudio","ram");
		server.add_type("image/cmu-raster","ras");
		server.add_type("image/cmu-raster","rast");
		server.add_type("text/x-script.rexx","rexx");
		server.add_type("image/vnd.rn-realflash","rf");
		server.add_type("image/x-rgb","rgb");
		server.add_type("audio/x-pn-realaudio","rm");
		server.add_type("audio/mid","rmi");
		server.add_type("audio/x-pn-realaudio","rmm");
		server.add_type("audio/x-pn-realaudio","rmp");
		server.add_type("application/ringing-tones","rng");
		server.add_type("application/vnd.rn-realplayer","rnx");
		server.add_type("application/x-troff","roff");
		server.add_type("image/vnd.rn-realpix","rp");
		server.add_type("application/octet-stream","rpm");
		server.add_type("text/richtext","rt");
		server.add_type("application/x-rtf","rtf");
		server.add_type("text/richtext","rtx");
		server.add_type("video/vnd.rn-realvideo","rv");
		server.add_type("text/x-asm","s");
		server.add_type("audio/s3m","s3m");
		server.add_type("application/octet-stream","saveme");
		server.add_type("application/x-tbook","sbk");
		server.add_type("application/x-lotusscreencam","scm");
		server.add_type("text/plain","sdml");
		server.add_type("application/x-sdp","sdp");
		server.add_type("application/sounder","sdr");
		server.add_type("application/x-sea","sea");
		server.add_type("application/set","set");
		server.add_type("text/x-sgml","sgm");
		server.add_type("text/x-sgml","sgml");
		server.add_type("application/x-sh","sh");
		server.add_type("application/x-bsh","shar");
		server.add_type("text/html","shtml");
		server.add_type("text/x-server-parsed-html","shtml");
		server.add_type("audio/x-psid","sid");
		server.add_type("application/x-sit","sit");
		server.add_type("application/x-stuffit","sit");
		server.add_type("application/x-koan","skd");
		server.add_type("application/x-koan","skm");
		server.add_type("application/x-koan","skp");
		server.add_type("application/x-koan","skt");
		server.add_type("application/x-seelogo","sl");
		server.add_type("application/smil","smi");
		server.add_type("application/smil","smil");
		server.add_type("audio/basic","snd");
		server.add_type("application/solids","sol");
		server.add_type("text/x-speech","spc");
		server.add_type("application/futuresplash","spl");
		server.add_type("application/x-sprite","spr");
		server.add_type("application/x-sprite","sprite");
		server.add_type("application/x-wais-source","src");
		server.add_type("text/x-server-parsed-html","ssi");
		server.add_type("application/streamingmedia","ssm");
		server.add_type("application/vnd.ms-pki.certstore","sst");
		server.add_type("application/step","step");
		server.add_type("application/sla","stl");
		server.add_type("application/step","stp");
		server.add_type("application/x-sv4cpio","sv4cpio");
		server.add_type("application/x-sv4crc","sv4crc");
		server.add_type("image/x-dwg","svf");
		server.add_type("x-world/x-svr","svr");
		server.add_type("application/x-shockwave-flash","swf");
		server.add_type("application/x-troff","t");
		server.add_type("text/x-speech","talk");
		server.add_type("application/x-tar","tar");
		server.add_type("application/x-tbook","tbk");
		server.add_type("text/x-script.tcl","tcl");
		server.add_type("text/x-script.tcsh","tcsh");
		server.add_type("application/x-tex","tex");
		server.add_type("application/x-texinfo","texi");
		server.add_type("application/x-texinfo","texinfo");
		server.add_type("text/plain","text");
		server.add_type("application/x-compressed","tgz");
		server.add_type("image/x-tiff","tif");
		server.add_type("image/x-tiff","tiff");
		server.add_type("application/x-troff","tr");
		server.add_type("audio/tsp-audio","tsi");
		server.add_type("audio/tsplayer","tsp");
		server.add_type("text/tab-separated-values","tsv");
		server.add_type("image/florian","turbot");
		server.add_type("text/plain","txt");
		server.add_type("text/x-uil","uil");
		server.add_type("text/uri-list","unis");
		server.add_type("application/i-deas","unv");
		server.add_type("text/uri-list","uri");
		server.add_type("text/uri-list","uris");
		server.add_type("application/x-ustar","ustar");
		server.add_type("application/octet-stream","uu");
		server.add_type("application/octet-stream","woff");
		server.add_type("application/octet-stream","woff2");
		server.add_type("application/octet-stream","ttf");
		server.add_type("text/x-uuencode","uu");
		server.add_type("text/x-uuencode","uue");
		server.add_type("application/x-cdlink","vcd");
		server.add_type("text/x-vcalendar","vcs");
		server.add_type("application/vda","vda");
		server.add_type("video/vdo","vdo");
		server.add_type("application/groupwise","vew");
		server.add_type("video/vivo","viv");
		server.add_type("video/vivo","vivo");
		server.add_type("application/vocaltec-media-desc","vmd");
		server.add_type("application/vocaltec-media-file","vmf");
		server.add_type("audio/x-voc","voc");
		server.add_type("video/vosaic","vos");
		server.add_type("audio/voxware","vox");
		server.add_type("audio/x-twinvq-plugin","vqe");
		server.add_type("audio/x-twinvq","vqf");
		server.add_type("audio/x-twinvq-plugin","vql");
		server.add_type("application/x-vrml","vrml");
		server.add_type("x-world/x-vrt","vrt");
		server.add_type("application/x-visio","vsd");
		server.add_type("application/x-visio","vst");
		server.add_type("application/x-visio","vsw");
		server.add_type("application/wordperfect6.0","w60");
		server.add_type("application/wordperfect6.1","w61");
		server.add_type("application/msword","w6w");
		server.add_type("audio/x-wav","wav");
		server.add_type("application/x-qpro","wb1");
		server.add_type("image/vnd.wap.wbmp","wbmp");
		server.add_type("application/vnd.xara","web");
		server.add_type("application/msword","wiz");
		server.add_type("application/x-123","wk1");
		server.add_type("windows/metafile","wmf");
		server.add_type("text/vnd.wap.wml","wml");
		server.add_type("application/vnd.wap.wmlc","wmlc");
		server.add_type("text/vnd.wap.wmlscript","wmls");
		server.add_type("application/vnd.wap.wmlscriptc","wmlsc");
		server.add_type("application/msword","word");
		server.add_type("application/wordperfect","wp");
		server.add_type("application/wordperfect","wp5");
		server.add_type("application/wordperfect","wp6");
		server.add_type("application/wordperfect","wpd");
		server.add_type("application/x-wpwin","wpd");
		server.add_type("application/x-lotus","wq1");
		server.add_type("application/x-wri","wri");
		server.add_type("application/x-world","wrl");
		server.add_type("model/vrml","wrz");
		server.add_type("text/scriplet","wsc");
		server.add_type("application/x-wais-source","wsrc");
		server.add_type("application/x-wintalk","wtk");
		server.add_type("image/x-xbitmap","xbm");
		server.add_type("video/x-amt-demorun","xdr");
		server.add_type("xgl/drawing","xgz");
		server.add_type("image/vnd.xiff","xif");
		server.add_type("application/excel","xl");
		server.add_type("application/excel","xla");
		server.add_type("application/excel","xlb");
		server.add_type("application/excel","xlc");
		server.add_type("application/excel","xld");
		server.add_type("application/excel","xlk");
		server.add_type("application/excel","xll");
		server.add_type("application/excel","xlm");
		server.add_type("application/excel","xls");
		server.add_type("application/excel","xlt");
		server.add_type("application/excel","xlv");
		server.add_type("application/excel","xlw");
		server.add_type("audio/xm","xm");
		server.add_type("text/xml","xml");
		server.add_type("xgl/movie","xmz");
		server.add_type("application/x-vnd.ls-xpix","xpix");
		server.add_type("image/xpm","xpm");
		server.add_type("image/png","x-png");
		server.add_type("video/x-amt-showrun","xsr");
		server.add_type("image/x-xwd","xwd");
		server.add_type("chemical/x-pdb","xyz");
		server.add_type("application/x-compressed","z");
		server.add_type("application/zip","zip");
		server.add_type("application/octet-stream","zoo");
		server.add_type("text/markdown","md");

	}
}
var SESSION_ENABLED = true;									   // 是否启用SESSION
var SESSION_ENGINE = 'cache';  								   // 引擎 cache file ,db 暂时不支持.
var SESSION_FILE_PATH = 'session/';							   // file模式下 session文件保存路径.
var SESSION_COOKIE_NAME = "cpps_sessionid";                   // Session的cookie保存在浏览器上时的key，即：cpps_sessionid＝随机字符串
var SESSION_COOKIE_PATH = "/";                                // Session的cookie保存的路径
var SESSION_COOKIE_DOMAIN = "";                              // Session的cookie保存的域名
var SESSION_COOKIE_SECURE = false;                             // 是否Https传输cookie
var SESSION_COOKIE_HTTPONLY = true;                            // 是否Session的cookie只支持http传输
var SESSION_COOKIE_AGE = 1209600;                              // Session的cookie失效日期（2周）
var SESSION_EXPIRE_AT_BROWSER_CLOSE = false;                   // 是否关闭浏览器使得Session过期
var SESSION_SAVE_EVERY_REQUEST = false;                        // 是否每次请求都保存Session，默认修改之后才保存
