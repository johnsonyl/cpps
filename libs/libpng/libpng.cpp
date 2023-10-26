

#include <cpps/cpps.h>
#include <iostream>
#include <string>
#include <fstream>

#ifdef WIN32
#include <libpng/pnglibconf.h>
#include <libpng/pngconf.h>
#include <libpng/png.h>
#else
#include <libpng16/pnglibconf.h>
#include <libpng16/pngconf.h>
#include <libpng16/png.h>
#endif

using namespace cpps;
using namespace std;

#define RGBASIZE 4

bool is_png(cpps::cpps_value v)
{
	static constexpr std::size_t png_sig_size = 8; // PNG header size = 8 byte.
	png_byte pngsig[png_sig_size]{};

	if (cpps::cpps_isstring(v)) {
		std::string* s = cpps_get_string(v);
		if (s->size() < png_sig_size) return false;
		memcpy(pngsig, s->c_str(), png_sig_size);
	}
	else if (cpps::cpps_isbasevar(v)) {
		if (cpps::cpps_base_type(v) == "FILE") {
			FILE* f = cpps::object_cast<FILE*>(v);
			size_t s = fread(pngsig, png_sig_size, 1, f);
			if (s == 0) return false;
		}
	}
	else {
		return false;
	}
	return png_sig_cmp(pngsig, 0, png_sig_size) == 0;
}

struct cpps_png_struct {
	png_struct* ptr;
};
struct cpps_png_info {
	png_info* ptr;
};
struct cpps_png_bytepp {
	png_bytepp ptr;
	cpps_integer tmp_h;
	cpps_integer tmp_ch;
	cpps_png_bytepp() {
		ptr = NULL;
		tmp_h = 0;
		tmp_ch = 0;
	}
	void constructor(cpps_integer w, cpps_integer h, cpps_integer ch, cpps::string *content) {
		alloc(w, h, ch);
		cpps_integer pos = 0;
		for (cpps_integer i = 0; i < h; i++) 
			for (cpps_integer j = 0; j < w ; j ++) {
				cpps_integer pos2 = 0;
				for (cpps_integer k = ch - 1; k >= 0; k--)
					ptr[i][j * ch + k] = content->real()[pos + (pos2++)];
				pos += RGBASIZE;
			}
	}
	cpps_value to_buffer(C*c,cpps_integer w, cpps_integer h, cpps_integer ch) {
		cpps::string* ret; cpps_value ret_value;
		cpps::newclass(c, &ret, &ret_value);
		ret->real().resize(w * h * RGBASIZE);
		cpps_integer posall = 0;

		for (cpps_integer i = 0; i < h; i++)
			for (cpps_integer j = 0; j < w; j++) {
				cpps_integer pos = 0;
				for (cpps_integer k = ch - 1; k >= 0; k--)
					ret->real()[posall +(pos++)] = (unsigned char)ptr[i][j * ch + k];
				posall += RGBASIZE;
			}
		return ret_value;
	}
	cpps_integer get(cpps_integer i, cpps_integer j) {
		return ptr[i][j];
	}
	void set(cpps_integer i, cpps_integer j, cpps_integer v) {
		ptr[i][j] = (png_byte)v;
	}
	void alloc(cpps_integer w,cpps_integer h,cpps_integer ch)
	{
		tmp_h = h;
		tmp_ch = ch;
		ptr = (png_bytep*)malloc(h * sizeof(png_bytep));
		for (cpps_integer i = 0; i < h; i++) 
			ptr[i] = (png_bytep)malloc((size_t)(ch * w * sizeof(unsigned char)));
	}
	void free() {
		if (ptr == NULL || tmp_h == 0) return;
		for (cpps_integer i = 0; i < tmp_h; i++)
			::free((void*)ptr[i]);
		::free((void*)ptr);
		ptr = NULL;
		tmp_h = 0;
	}
};
cpps_value cpps_png_create_read_struct(C * c,std::string ver) {
	cpps_png_struct* ret; cpps_value ret_value;
	cpps::newclass(c, &ret, &ret_value);
	ret->ptr = png_create_read_struct(ver.c_str(), NULL, NULL, NULL);
	return ret_value;
}
cpps_value cpps_png_create_write_struct(C * c,std::string ver) {
	cpps_png_struct* ret; cpps_value ret_value;
	cpps::newclass(c, &ret, &ret_value);
	ret->ptr = png_create_write_struct(ver.c_str(), NULL, NULL, NULL);
	return ret_value;
}
cpps_value cpps_png_create_info_struct(C* c, cpps_png_struct* png_ptr) {
	cpps_png_info* ret; cpps_value ret_value;
	cpps::newclass(c, &ret, &ret_value);
	ret->ptr = png_create_info_struct(png_ptr->ptr);
	if (ret->ptr == NULL) return cpps::nil;
	setjmp(png_jmpbuf(png_ptr->ptr));
	return ret_value;

}
void cpps_png_read_png(cpps_png_struct* png_ptr, cpps_png_info* info_ptr, cpps::int32 transforms/*,png_voidp params unknow params is what.*/) {
	png_read_png(png_ptr->ptr, info_ptr->ptr, transforms, 0);
}
cpps_integer cpps_png_get_channels(cpps_png_struct* png_ptr, cpps_png_info* info_ptr) {
	return png_get_channels(png_ptr->ptr, info_ptr->ptr);
}

void cpps_png_init_io(cpps_png_struct* png_ptr, FILE* fp) {
	 png_init_io(png_ptr->ptr, fp);
}

cpps_integer cpps_png_get_color_type(cpps_png_struct* png_ptr, cpps_png_info* info_ptr) {
	return png_get_color_type(png_ptr->ptr, info_ptr->ptr);
}
cpps_integer cpps_png_get_bit_depth(cpps_png_struct* png_ptr, cpps_png_info* info_ptr) {
	return png_get_bit_depth(png_ptr->ptr, info_ptr->ptr);
}
cpps_integer cpps_png_get_image_width(cpps_png_struct* png_ptr, cpps_png_info* info_ptr) {
	return png_get_image_width(png_ptr->ptr, info_ptr->ptr);
}
cpps_integer cpps_png_get_image_height(cpps_png_struct* png_ptr, cpps_png_info* info_ptr) {
	return png_get_image_height(png_ptr->ptr, info_ptr->ptr);
}
void cpps_png_set_palette_to_rgb(cpps_png_struct* png_ptr) {
	png_set_palette_to_rgb(png_ptr->ptr);
}
void cpps_png_set_expand_gray_1_2_4_to_8(cpps_png_struct* png_ptr) {
	png_set_expand_gray_1_2_4_to_8(png_ptr->ptr);
}
void cpps_png_set_strip_16(cpps_png_struct* png_ptr) {
	png_set_strip_16(png_ptr->ptr);
}
void cpps_png_set_tRNS_to_alpha(cpps_png_struct* png_ptr) {
	png_set_tRNS_to_alpha(png_ptr->ptr);
}
void cpps_png_set_gray_to_rgb(cpps_png_struct* png_ptr) {
	png_set_gray_to_rgb(png_ptr->ptr);
}
cpps::cpps_value cpps_png_get_rows(C*c,cpps_png_struct* png_ptr, cpps_png_info* info_ptr) {
	cpps_png_bytepp* ret; cpps_value ret_value;
	cpps::newclass(c, &ret, &ret_value);
	ret->ptr = png_get_rows(png_ptr->ptr, info_ptr->ptr);
	return ret_value;
}
void cpps_png_destroy_read_struct(cpps_png_struct* png_ptr, cpps_png_info* info_ptr) {
	png_destroy_read_struct(&png_ptr->ptr, &info_ptr->ptr, 0);
}
void cpps_png_destroy_write_struct(cpps_png_struct* png_ptr, cpps_png_info* info_ptr) {
	png_destroy_write_struct(&png_ptr->ptr, &info_ptr->ptr);
}
void cpps_png_set_IHDR(cpps_png_struct* png_ptr, cpps_png_info* info_ptr, cpps_integer width, cpps_integer height, cpps_integer bit_depth,
	cpps_integer color_type, cpps_integer interlace_method, cpps_integer compression_method,
	cpps_integer filter_method) {
	png_set_IHDR(png_ptr->ptr, info_ptr->ptr, (cpps::usint32)width, (cpps::usint32)height, (cpps::int32)bit_depth, (cpps::int32)color_type, (cpps::int32)interlace_method, (cpps::int32)compression_method, (cpps::int32)filter_method);
}
void cpps_png_write_info(cpps_png_struct* png_ptr, cpps_png_info* info_ptr) {
	png_write_info(png_ptr->ptr, info_ptr->ptr);
}
void cpps_png_write_image(cpps_png_struct* png_ptr, cpps_png_bytepp* bytepp_ptr) {
	png_write_image(png_ptr->ptr, bytepp_ptr->ptr);
}
void cpps_png_write_end(cpps_png_struct* png_ptr, cpps_png_info* info_ptr) {
	png_write_end(png_ptr->ptr, info_ptr->ptr);
}
cpps_export_void cpps_attach(cpps::C* c)
{
	cpps::cpps_init_cpps_class(c);
	
	cpps::_module(c, "libpng")[
		def("is_png", is_png),
		_class<cpps_png_struct>("png_struct"),
		_class<cpps_png_info>("png_info"),
		_class<cpps_png_bytepp>("png_bytepp")
			.def("constructor", &cpps_png_bytepp::constructor)
			.def_inside("to_buffer", &cpps_png_bytepp::to_buffer)
			.def("set", &cpps_png_bytepp::set)
			.def("free", &cpps_png_bytepp::free)
			.def("alloc", &cpps_png_bytepp::alloc)
			.def("get", &cpps_png_bytepp::get),
		def_inside("png_create_read_struct", cpps_png_create_read_struct),
		def_inside("png_create_write_struct", cpps_png_create_write_struct),
		def_inside("png_create_info_struct", cpps_png_create_info_struct),
		def("png_init_io", cpps_png_init_io),
		def("png_read_png", cpps_png_read_png),
		defvar(c,"RGBASIZE", RGBASIZE),
		defvar(c,"PNG_LIBPNG_VER_STRING", PNG_LIBPNG_VER_STRING),
		defvar(c,"PNG_HEADER_VERSION_STRING", PNG_HEADER_VERSION_STRING),
		defvar(c,"PNG_LIBPNG_VER_SONUM", PNG_LIBPNG_VER_SONUM),
		defvar(c,"PNG_LIBPNG_VER_DLLNUM", PNG_LIBPNG_VER_DLLNUM),
		defvar(c,"PNG_LIBPNG_VER_MAJOR", PNG_LIBPNG_VER_MAJOR),
		defvar(c,"PNG_LIBPNG_VER_MINOR", PNG_LIBPNG_VER_MINOR),
		defvar(c,"PNG_LIBPNG_VER_RELEASE", PNG_LIBPNG_VER_RELEASE),
		defvar(c,"PNG_LIBPNG_VER_BUILD", PNG_LIBPNG_VER_BUILD),
		defvar(c,"PNG_TRANSFORM_IDENTITY", PNG_TRANSFORM_IDENTITY),
		defvar(c,"PNG_TRANSFORM_STRIP_16", PNG_TRANSFORM_STRIP_16),
		defvar(c,"PNG_TRANSFORM_STRIP_ALPHA", PNG_TRANSFORM_STRIP_ALPHA),
		defvar(c,"PNG_TRANSFORM_PACKING", PNG_TRANSFORM_PACKING),
		defvar(c,"PNG_TRANSFORM_PACKSWAP", PNG_TRANSFORM_PACKSWAP),
		defvar(c,"PNG_TRANSFORM_EXPAND", PNG_TRANSFORM_EXPAND),
		defvar(c,"PNG_TRANSFORM_INVERT_MONO", PNG_TRANSFORM_INVERT_MONO),
		defvar(c,"PNG_TRANSFORM_SHIFT", PNG_TRANSFORM_SHIFT),
		defvar(c,"PNG_TRANSFORM_BGR", PNG_TRANSFORM_BGR),
		defvar(c,"PNG_TRANSFORM_SWAP_ALPHA", PNG_TRANSFORM_SWAP_ALPHA),
		defvar(c,"PNG_TRANSFORM_SWAP_ENDIAN", PNG_TRANSFORM_SWAP_ENDIAN),
		defvar(c,"PNG_TRANSFORM_INVERT_ALPHA", PNG_TRANSFORM_INVERT_ALPHA),
		defvar(c,"PNG_TRANSFORM_STRIP_FILLER", PNG_TRANSFORM_STRIP_FILLER),
		defvar(c,"PNG_TRANSFORM_STRIP_FILLER_BEFORE", PNG_TRANSFORM_STRIP_FILLER_BEFORE),
		defvar(c,"PNG_TRANSFORM_STRIP_FILLER_AFTER", PNG_TRANSFORM_STRIP_FILLER_AFTER),
		defvar(c,"PNG_TRANSFORM_GRAY_TO_RGB", PNG_TRANSFORM_GRAY_TO_RGB),
		defvar(c,"PNG_TRANSFORM_EXPAND_16", PNG_TRANSFORM_EXPAND_16),
		defvar(c,"PNG_TRANSFORM_SCALE_16", PNG_TRANSFORM_SCALE_16),
		def("png_get_channels", cpps_png_get_channels),
		def("png_get_color_type", cpps_png_get_color_type),
		def("png_get_bit_depth", cpps_png_get_bit_depth),
		def("png_get_image_width", cpps_png_get_image_width),
		def("png_get_image_height", cpps_png_get_image_height),
		def("png_set_palette_to_rgb", cpps_png_set_palette_to_rgb),
		def("png_set_expand_gray_1_2_4_to_8", cpps_png_set_expand_gray_1_2_4_to_8),
		def("png_set_strip_16", cpps_png_set_strip_16),
		def("png_set_tRNS_to_alpha", cpps_png_set_tRNS_to_alpha),
		def("png_set_gray_to_rgb", cpps_png_set_gray_to_rgb),
		def_inside("png_get_rows", cpps_png_get_rows),
		defvar(c,"PNG_COLOR_TYPE_GRAY", PNG_COLOR_TYPE_GRAY),
		defvar(c,"PNG_COLOR_TYPE_PALETTE", PNG_COLOR_TYPE_PALETTE),
		defvar(c,"PNG_COLOR_TYPE_RGB", PNG_COLOR_TYPE_RGB),
		defvar(c,"PNG_COLOR_TYPE_RGB_ALPHA", PNG_COLOR_TYPE_RGB_ALPHA),
		defvar(c,"PNG_COLOR_TYPE_GRAY_ALPHA", PNG_COLOR_TYPE_GRAY_ALPHA),
		defvar(c,"PNG_COLOR_TYPE_RGBA", PNG_COLOR_TYPE_RGBA),
		defvar(c,"PNG_COLOR_TYPE_GA", PNG_COLOR_TYPE_GA),
		defvar(c,"PNG_INTERLACE_NONE", PNG_INTERLACE_NONE),
		defvar(c,"PNG_INTERLACE_ADAM7", PNG_INTERLACE_ADAM7),
		defvar(c,"PNG_INTERLACE_LAST", PNG_INTERLACE_LAST),
		defvar(c,"PNG_COMPRESSION_TYPE_BASE", PNG_COMPRESSION_TYPE_BASE),
		defvar(c,"PNG_COMPRESSION_TYPE_DEFAULT", PNG_COMPRESSION_TYPE_DEFAULT),
		defvar(c,"PNG_FILTER_TYPE_BASE", PNG_FILTER_TYPE_BASE),
		defvar(c,"PNG_FILTER_TYPE_DEFAULT", PNG_FILTER_TYPE_DEFAULT),
		def("png_destroy_read_struct", cpps_png_destroy_read_struct),
		def("png_destroy_write_struct", cpps_png_destroy_write_struct),
		def("png_set_IHDR", cpps_png_set_IHDR),
		def("png_write_info", cpps_png_write_info),
		def("png_write_image", cpps_png_write_image),
		def("png_write_end", cpps_png_write_end)
			
	];



   
}
cpps_export_void  cpps_detach(cpps::C * c)
{
	cpps::_unmodule(c, "libpng");
}

cpps_export_finish