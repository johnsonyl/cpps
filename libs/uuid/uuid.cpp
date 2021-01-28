

#include <cpps/cpps.h>
#include <iostream>
#include <string>
#include <fstream>

#ifndef _WIN32
#include <uuid/uuid.h>
#else
#include <objbase.h>
#endif
#define GUID_LEN 128

using namespace cpps;
using namespace std;
class cpps_uuid
{
public:
	unsigned int  Data1;
	unsigned short Data2;
	unsigned short Data3;
	unsigned char  Data4[8];
	void	init() {
#ifdef _WIN32
		GUID guid;

		if (CoCreateGuid(&guid) != NULL)
		{
			return;
		}
		memcpy(this, &guid, sizeof(guid));
#else 
		uuid_t uuid;
		uuid_generate(uuid);
		memcpy(this, uuid, sizeof(cpps_uuid));
#endif
	}
	std::string tostring() {
		char buffer[GUID_LEN] = { 0 };
		sprintf(buffer,
			"%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X",
			Data1, Data2, Data3,
			Data4[0], Data4[1], Data4[2],
			Data4[3], Data4[4], Data4[5],
			Data4[6], Data4[7]);
		return buffer;
	}
};

cpps_value cpps_uuid_create(C*c) {
	cpps_uuid* ret_uuid;
	cpps_value ret;
	newclass<cpps_uuid>(c, &ret_uuid,&ret);
	ret_uuid->init();
	return ret;
}
std::string cpps_uuid_uuid4() {
	cpps_uuid uuid;
	uuid.init();
	return uuid.tostring();
}

cpps_export_void cpps_attach(cpps::C* c)
{

	cpps::cpps_init_cpps_class(c);

	cpps::_module(c, "uuid")[
		_class<cpps_uuid>("UUID")
			.def("tostring", &cpps_uuid::tostring),
		def_inside("create", cpps_uuid_create),
		def("uuid4", cpps_uuid_uuid4)
	];



   
}
cpps_export_void  cpps_detach(cpps::C * c)
{
	cpps::_unmodule(c, "uuid");
}

cpps_export_finish