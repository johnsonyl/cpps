system("chcp 65001"); //use utf8 string


#import "libpng"


class PNGHandle
{
	var _width = 0;
	var _height = 0;
	var _channels = 0;
	var _color_type = 6;
	var _bit_depth = 8;
	var _buffer;
	
	var Load(var path){
		
		var fp = io.fopen(path,"rb");

		var png_ptr  = libpng.png_create_read_struct(libpng.PNG_LIBPNG_VER_STRING); 
		var info_ptr = libpng.png_create_info_struct(png_ptr);

		libpng.png_init_io(png_ptr, fp);

		libpng.png_read_png(png_ptr, info_ptr, libpng.PNG_TRANSFORM_EXPAND); //读取文件信息

		_channels 	= libpng.png_get_channels(png_ptr, info_ptr); //通道数量
		_color_type 	= libpng.png_get_color_type(png_ptr, info_ptr);//颜色类型
		_bit_depth = libpng.png_get_bit_depth(png_ptr, info_ptr);//位深度	
		_width 	 = libpng.png_get_image_width(png_ptr, info_ptr);//宽
		_height  = libpng.png_get_image_height(png_ptr, info_ptr);//高

		var row_pointers = libpng.png_get_rows(png_ptr, info_ptr); 

		_buffer = row_pointers.to_buffer(_width,_height,_channels);

		libpng.png_destroy_read_struct(png_ptr, info_ptr);
		io.fclose(fp);

	}
	
	var Create(var width,var height,var channels,var buffer){
		_width = width;
		_height = height;
		_channels = channels;
		_buffer = buffer;
	}
	var GetSubPNG(var x,var y, var width,var height,var borderLeft = 0,var borderRight = 0,var borderTop = 0,var borderBottom = 0){
		var buffer = "";
		var realwidth = (width + borderLeft + borderRight);
		var realheight = (height+borderTop+borderBottom)
		buffer.resize(realwidth * realheight * libpng.RGBASIZE);
		
		
		
		for(var i = x; i < x+width; i++)
			for(var j = y; j < y+height; j++)
				for(var k = 0; k < libpng.RGBASIZE; k++)
					buffer[(j-y+borderTop)*realwidth*libpng.RGBASIZE + ((i-x+borderLeft) * libpng.RGBASIZE) + k ] = _buffer[(j-1)*_width*libpng.RGBASIZE + ((i-1) * libpng.RGBASIZE) + k ];
				
				
		var _handle = new PNGHandle();
		_handle.Create(realwidth,realheight,_channels,buffer);
		return _handle;
		
	}
	var RotationRight90() //顺时针旋转90度
	{
		var tempSrc = _buffer;
		var i = 0;
		var j = 0;
		var k = 0;
		var desW = 0;
		var desH = 0;
		desW = _height;
		desH = _width;
	 
		for(i = 0; i < desH; i ++)
		{
			for(j = 0; j < desW; j ++)
			{
				for(k = 0; k < libpng.RGBASIZE; k ++)
				{
					_buffer[(i * desW + j) * libpng.RGBASIZE + k] = tempSrc[((_height - 1 - j) * _width + i) * libpng.RGBASIZE + k]; //替换像素
				}
			}
		}
		_height = desH;
		_width = desW;
		
		return 0;
	}


	var Save(var path){
		

		var fp = io.fopen(path,"wb+");

		var png_ptr	= libpng.png_create_write_struct(libpng.PNG_LIBPNG_VER_STRING);
		var info_ptr = libpng.png_create_info_struct(png_ptr);
		libpng.png_init_io(png_ptr, fp);

		libpng.png_set_IHDR(png_ptr, info_ptr, _width, _height, _bit_depth,
			_color_type, libpng.PNG_INTERLACE_NONE, libpng.PNG_COMPRESSION_TYPE_BASE, libpng.PNG_FILTER_TYPE_BASE);

		libpng.png_write_info(png_ptr, info_ptr);

		var row_pointers = new libpng::png_bytepp(_width,_height,_channels,_buffer);


		libpng.png_write_image(png_ptr, row_pointers);

		libpng.png_write_end(png_ptr,info_ptr);


		libpng.png_destroy_write_struct(png_ptr, info_ptr);
		io.fclose(fp);
		row_pointers.free();

	}
}

var atlaspath = "D:/work/code/ReBleach/Assets/UI/Atlas";
var paths = io.walk(atlaspath,true);

foreach(var path:paths){
	var ext = io.getfileext(path);
	if(ext == "prefab"){
		var filename = io.getfilenamenotext(path);
		var justpath = io.getfilepath(path);
		var pngpath = "{justpath}/{filename}.png";
		if(io.file_exists(pngpath)){
			var currpath = justpath;
			currpath.replace(atlaspath,"");
			currpath.erase(0,1);
			currpath = "output/{currpath}";
			io.mkdirs(currpath);
			
			var handle = new PNGHandle();
			handle.Load(pngpath);

			var prefab = io.readfile(path);

			prefab = prefab.strcut("mSprites:","mPixelSize");

			var l = prefab.strcuts("- ","paddingBottom: 0");

			foreach(var c : l){
				
				var name = c.strcut("name: ","\n");
				var x = int(c.strcut("x: ","\n"));
				var y = int(c.strcut("y: ","\n"));
				var width = int(c.strcut("width: ","\n"));
				var height = int(c.strcut("height: ","\n"));
				var borderLeft = int(c.strcut("borderLeft: ","\n"));
				var borderRight = int(c.strcut("borderRight: ","\n"));
				var borderTop = int(c.strcut("borderTop: ","\n"));
				var borderBottom = int(c.strcut("borderBottom: ","\n"));
				
				 var handle2 = handle.GetSubPNG(x,y,width,height,borderLeft,borderRight,borderTop,borderBottom);

				handle2.Save("{currpath}/{name}.png"); 
				println("{currpath}/{name}.png")
			}
		}
	}
}


/*  */

/*  
 */
//保存PNG
println("ojbk")