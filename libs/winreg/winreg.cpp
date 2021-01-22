

#include <cpps/cpps.h>
#include <iostream>
#include <string>
#include <fstream>
#include "WinReg.hpp"
#include <Tlhelp32.h>


using namespace cpps;
using namespace std;
using winreg::RegKey;
using winreg::RegException;

#define CPPS_WINREG_TRY try {

#define CPPS_WINREG_CATCH }\
catch (const RegException& e)\
{\
	throw cpps_error(__FILE__, __LINE__,0, e.what());\
}\
catch (const std::exception& e)\
{\
	throw cpps_error(__FILE__, __LINE__,0, e.what());\
}

void cpps_winreg_closekey(RegKey* hkey)
{
	CPPS_WINREG_TRY
	hkey->Close();
	CPPS_WINREG_CATCH
}
object cpps_winreg_connectregistry(C*c,std::string computer_name,HKEY key)
{
	RegKey* hkey = NULL;
	object ret = object::create_with_classvar(c, &hkey);
	CPPS_WINREG_TRY
	hkey->ConnectRegistry(computer_name,key);
	CPPS_WINREG_CATCH
	return ret;
}
object cpps_winreg_createkey(C* c, object key,std::string subkey)
{
	object ret;
	CPPS_WINREG_TRY
	if (key.isclassvar())
	{
		if (type_s(key) == "RegKey")
		{
			RegKey* hkey = object_cast<RegKey*>(key);
			hkey->Create(hkey->Get(), subkey);
			ret = key;
		}
		else if (type_s(key) == "HKEY")
		{
			HKEY hkey = object_cast<HKEY>(key);
			RegKey* regkey = NULL;
			ret = object::create_with_classvar(c, &hkey);
			regkey->Create(hkey, subkey);
		}
	}
	CPPS_WINREG_CATCH
	return ret;
}
object cpps_winreg_createkeyex(C* c, object key, std::string subkey,object accessobj)
{
	REGSAM access = KEY_WRITE;
	if (accessobj.isint()) access = (REGSAM)accessobj.toint();

	object ret;
	CPPS_WINREG_TRY
	if (key.isclassvar())
	{
		if (type_s(key) == "RegKey")
		{
			RegKey* hkey = object_cast<RegKey*>(key);
			hkey->Create(hkey->Get(), subkey, access);
			ret = key;
		}
		else if (type_s(key) == "HKEY")
		{
			HKEY hkey = object_cast<HKEY>(key);
			RegKey* regkey = NULL;
			ret = object::create_with_classvar(c, &hkey);
			regkey->Create(hkey, subkey, access);
		}
	}
	CPPS_WINREG_CATCH
	return ret;
}
void cpps_winreg_deletekey(object key,std::string subkey)
{
	CPPS_WINREG_TRY
	if (key.isclassvar())
	{
		if (type_s(key) == "RegKey")
		{
			RegKey* hkey = object_cast<RegKey*>(key);
			hkey->DeleteKey(subkey, KEY_WOW64_64KEY);
		}
		else if (type_s(key) == "HKEY")
		{
			HKEY hkey = object_cast<HKEY>(key);
			RegKey regkey(hkey);
			regkey.DeleteKey(subkey, KEY_WOW64_64KEY);
		}
	}
	CPPS_WINREG_CATCH
}
void cpps_winreg_deletevalue(object key, std::string value)
{
	CPPS_WINREG_TRY

	if (key.isclassvar())
	{
		if (type_s(key) == "RegKey")
		{
			RegKey* hkey = object_cast<RegKey*>(key);
			hkey->DeleteValue(value);
		}
		else if (type_s(key) == "HKEY")
		{
			HKEY hkey = object_cast<HKEY>(key);
			RegKey regkey(hkey);
			regkey.DeleteValue(value);
		}
	}
	CPPS_WINREG_CATCH
}
void cpps_winreg_deletekeyex(object key, std::string subkey, object accessobj)
{
	CPPS_WINREG_TRY
	REGSAM access = KEY_WOW64_64KEY;
	if (accessobj.isint()) access = (REGSAM)accessobj.toint();

	if (key.isclassvar())
	{
		if (type_s(key) == "RegKey")
		{
			RegKey* hkey = object_cast<RegKey*>(key);
			hkey->DeleteKey(subkey, access);
		}
		else if (type_s(key) == "HKEY")
		{
			HKEY hkey = object_cast<HKEY>(key);
			RegKey regkey(hkey);
			regkey.DeleteKey(subkey, access);
		}
	}
	CPPS_WINREG_CATCH
}
std::string cpps_winreg_enumkey(object key, cpps_integer index)
{
	CPPS_WINREG_TRY

	if (key.isclassvar())
	{
		if (type_s(key) == "RegKey")
		{
			RegKey* hkey = object_cast<RegKey*>(key);
			return hkey->EnumSubKeys()[(size_t)index];
		}
		else if (type_s(key) == "HKEY")
		{
			HKEY hkey = object_cast<HKEY>(key);
			RegKey regkey(hkey);
			return regkey.EnumSubKeys()[(size_t)index];
		}
	}
	CPPS_WINREG_CATCH
	return "";
}
void cpps_winreg_getvalue_swap(char* ptr, size_t size, size_t nitems) {
	size_t i;
	char tmp, * lptr, * uptr;

	/* check for quick return */

	if ((size < 2) || (nitems < 1)) return;

	/* loop over all items */

	for (i = 0; i < nitems; i++) {
		lptr = ptr; lptr = lptr + i * size;
		uptr = lptr + size - 1;
		while (lptr < uptr) {
			tmp = *lptr; *lptr = *uptr; *uptr = tmp;
			lptr++; uptr--;
		}
	}

	/* normal return */

	return;

}
object cpps_winreg_getvalue(C*c, RegKey* hkey,std::string valuename,cpps_integer type)
{
	object ret;
	CPPS_WINREG_TRY
	switch (type)
	{
	case REG_BINARY:
	{
		auto v = hkey->GetBinaryValue(valuename);
		ret = cpps::object::create(c,"");
		std::string* str = cpps_get_string(ret.value);
		str->append((char *)v.data(), v.size());
		break;
	}
	case REG_DWORD:
		//case REG_DWORD_LITTLE_ENDIAN:
	{
		auto v = hkey->GetDwordValue(valuename);
		ret = object::create(c, v);
		break;
	}
	case REG_DWORD_BIG_ENDIAN:
	{
		auto v = hkey->GetDwordValue(valuename);
		cpps_winreg_getvalue_swap((char*)&v, sizeof(v), 1);
		ret = object::create(c, v);
		break;
	}
	case REG_EXPAND_SZ:
	{
		auto v = hkey->GetExpandStringValue(valuename);
		ret = object::create(c, v);
		break;
	}
	case REG_LINK:
	{
		//not support.
		break;
	}
	case REG_MULTI_SZ:
	{
		auto v = hkey->GetMultiStringValue(valuename);
		auto vct = object::vector::create(c);
		for (auto vv : v) {
			vct.push_back(object::create(c, vv));
		}
		ret = vct.toobject();
		break;
	}
	case REG_NONE:
	{
		break;
	}
	case REG_QWORD:
		//case REG_QWORD_LITTLE_ENDIAN:
	{
		auto v = hkey->GetQwordValue(valuename);
		ret = object::create(c, v);
		break;
	}
	case REG_RESOURCE_LIST:
	{
		//not support.
		break;
	}
	case REG_FULL_RESOURCE_DESCRIPTOR:
	{
		//not support.
		break;
	}
	case REG_RESOURCE_REQUIREMENTS_LIST:
	{
		//not support.
		break;
	}
	case REG_SZ:
	{
		auto v = hkey->GetStringValue(valuename);
		ret = object::create(c, v);
		break;
	}
	default:
		break;
	}
	CPPS_WINREG_CATCH
	return ret;
}
cpps::tuple cpps_winreg_enumvalue(C*c,object key, cpps_integer index)
{
	cpps::tuple ret;
	CPPS_WINREG_TRY
	if (key.isclassvar())
	{
		if (type_s(key) == "RegKey")
		{
			RegKey* hkey = object_cast<RegKey*>(key);
			auto value = hkey->EnumValues()[(size_t)index];

			ret = cpps::tuple(object::create(c, value.first),
									cpps_winreg_getvalue(c, hkey, value.first,(cpps_integer) value.second),
									object::create(c, value.second)
									);

		
		}
		else if (type_s(key) == "HKEY")
		{
			HKEY hkey = object_cast<HKEY>(key);
			RegKey regkey(hkey);
			auto value = regkey.EnumValues()[(size_t)index];
			ret = cpps::tuple(object::create(c, value.first),
									cpps_winreg_getvalue(c, &regkey, value.first,(cpps_integer) value.second),
									object::create(c, value.second)
									);
		}
	}
	CPPS_WINREG_CATCH
	return ret;
}
void cpps_winreg_flushkey(object key)
{
	CPPS_WINREG_TRY
	if (key.isclassvar())
	{
		if (type_s(key) == "RegKey")
		{
			RegKey* hkey = object_cast<RegKey*>(key);
			hkey->FlushKey();
		}
		else if (type_s(key) == "HKEY")
		{
			HKEY hkey = object_cast<HKEY>(key);
			RegKey regkey(hkey);
			regkey.FlushKey();
		}
	}
	CPPS_WINREG_CATCH
}
void cpps_winreg_loadkey(object key,std::string subkey,std::string filename)
{
	CPPS_WINREG_TRY
	if (key.isclassvar())
	{
		if (type_s(key) == "RegKey")
		{
			RegKey* hkey = object_cast<RegKey*>(key);
			hkey->LoadKey(subkey, filename);
		}
		else if (type_s(key) == "HKEY")
		{
			HKEY hkey = object_cast<HKEY>(key);
			RegKey regkey(hkey);
			regkey.LoadKey(subkey, filename);
		}
	}
	CPPS_WINREG_CATCH
}
object cpps_winreg_openkey(C*c,object key, std::string subkey, object accessobj)
{
	object ret;
	CPPS_WINREG_TRY
	REGSAM access = KEY_ALL_ACCESS;
	if (accessobj.isint()) access = (REGSAM)accessobj.toint();
	if (key.isclassvar())
	{
		if (type_s(key) == "HKEY")
		{
			HKEY hkey = object_cast<HKEY>(key);
			RegKey* regkey = NULL;
			ret = object::create_with_classvar(c, &regkey);
			regkey->Open(hkey,subkey, access);
		}
	}
	CPPS_WINREG_CATCH
	return ret;
}
std::string cpps_winreg_expandenvironmentstrings(std::string str)
{
	CPPS_WINREG_TRY
	auto ret = RegKey::ExpandEnvironmentStrings(str);
	return ret;
	CPPS_WINREG_CATCH
	return "";
}
void  FileTimeToTime_t(FILETIME  ft, time_t* t)
{
	ULARGE_INTEGER            ui;
	ui.LowPart = ft.dwLowDateTime;
	ui.HighPart = ft.dwHighDateTime;
	*t = ((LONGLONG)(ui.QuadPart - 116444736000000000) / 10000000);
}
cpps::tuple cpps_winreg_queryvalue(C* c, object key,std::string value_name)
{
	cpps::tuple ret;
	CPPS_WINREG_TRY
	if (key.isclassvar())
	{
		if (type_s(key) == "RegKey")
		{
			RegKey* hkey = object_cast<RegKey*>(key);
			auto type = hkey->QueryValueType(value_name);
			ret = cpps::tuple(cpps_winreg_getvalue(c, hkey, value_name, (cpps_integer)type), object::create(c, type));
		}
		else if (type_s(key) == "HKEY")
		{
			HKEY hkey = object_cast<HKEY>(key);
			RegKey regkey(hkey);
			auto type = regkey.QueryValueType(value_name);
			ret = cpps::tuple(cpps_winreg_getvalue(c, &regkey, value_name, (cpps_integer)type), object::create(c, type));
		}
	}
	CPPS_WINREG_CATCH
	return ret;
}
DWORD AdjustProcessToken(HANDLE hProcess, LPCTSTR lpPrivilegeName) {
	HANDLE                hToken;        //令牌句柄
	TOKEN_PRIVILEGES    tp;
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if (!OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken)) {
		return GetLastError();
	}
	if (!LookupPrivilegeValue(NULL, lpPrivilegeName, &tp.Privileges[0].Luid)) {
		return GetLastError();
	}
	if (!AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL)) {
		return GetLastError();
	}

	return 0;
}
void cpps_winreg_savekey( object key,std::string filename)
{
	CPPS_WINREG_TRY
	if (key.isclassvar())
	{
		if (type_s(key) == "RegKey")
		{
			AdjustProcessToken(GetCurrentProcess(), SE_BACKUP_NAME); //启动特权.否则存盘会失败.
			RegKey* hkey = object_cast<RegKey*>(key);
			hkey->SaveKey(filename, NULL);
		}
		else if (type_s(key) == "HKEY")
		{
			AdjustProcessToken(GetCurrentProcess(), SE_BACKUP_NAME);
			HKEY hkey = object_cast<HKEY>(key);
			RegKey regkey(hkey);
			regkey.SaveKey(filename, NULL);
		}
	}
	CPPS_WINREG_CATCH
}

void cpps_winreg_setvalue_set(RegKey* hkey,std::string &valuename,cpps_integer type,object value)
{
	CPPS_WINREG_TRY
	switch (type)
	{
	case REG_BINARY:
	{
		std::string* str = cpps_get_string(value.value);
		hkey->SetBinaryValue(valuename, (void*)str->c_str(),(DWORD) str->size());
		break;
	}
	case REG_DWORD:
		//case REG_DWORD_LITTLE_ENDIAN:
	{
		auto v = value.toint();
		hkey->SetDwordValue(valuename, (DWORD)v);
		break;
	}
	case REG_DWORD_BIG_ENDIAN:
	{
		auto v = value.toint();
		cpps_winreg_getvalue_swap((char*)&v, sizeof(v), 1);
		hkey->SetDwordValue(valuename, (DWORD)v);
		break;
	}
	case REG_EXPAND_SZ:
	{
		std::string* str = cpps_get_string(value.value);
		hkey->SetExpandStringValue(valuename, *str);
		break;
	}
	case REG_LINK:
	{
		//not support.
		break;
	}
	case REG_MULTI_SZ:
	{
		auto vct = object::vector(value);
		std::vector<std::string> data;
		for (auto vv :vct) {
			data.push_back(cpps_to_string(vv));
		}
		hkey->SetMultiStringValue(valuename, data);
		break;
	}
	case REG_NONE:
	{
		break;
	}
	case REG_QWORD:
		//case REG_QWORD_LITTLE_ENDIAN:
	{
		auto v = value.toint();
		hkey->SetQwordValue(valuename, (ULONGLONG)v);
		break;
	}
	case REG_RESOURCE_LIST:
	{
		//not support.
		break;
	}
	case REG_FULL_RESOURCE_DESCRIPTOR:
	{
		//not support.
		break;
	}
	case REG_RESOURCE_REQUIREMENTS_LIST:
	{
		//not support.
		break;
	}
	case REG_SZ:
	{
		std::string* str = cpps_get_string(value.value);
		hkey->SetStringValue(valuename, *str);
		break;
	}
	default:
		break;
	}
	CPPS_WINREG_CATCH
}
void cpps_winreg_setvalue( object key,std::string valuename,cpps_integer type,object value)
{
	CPPS_WINREG_TRY
	if (key.isclassvar())
	{
		if (type_s(key) == "RegKey")
		{
			AdjustProcessToken(GetCurrentProcess(), SE_BACKUP_NAME); //启动特权.否则存盘会失败.
			RegKey* hkey = object_cast<RegKey*>(key);
			cpps_winreg_setvalue_set(hkey, valuename, type, value);
		}
		else if (type_s(key) == "HKEY")
		{
			AdjustProcessToken(GetCurrentProcess(), SE_BACKUP_NAME);
			HKEY hkey = object_cast<HKEY>(key);
			RegKey regkey(hkey);
			cpps_winreg_setvalue_set(&regkey, valuename, type, value);
		}
	}
	CPPS_WINREG_CATCH
}
void cpps_winreg_disablereflectionkey( object key)
{
	CPPS_WINREG_TRY
	if (key.isclassvar())
	{
		if (type_s(key) == "RegKey")
		{
			RegKey* hkey = object_cast<RegKey*>(key);
			hkey->DisableReflectionKey();
		}
		else if (type_s(key) == "HKEY")
		{
			HKEY hkey = object_cast<HKEY>(key);
			RegKey regkey(hkey);
			regkey.DisableReflectionKey();
		}
	}
	CPPS_WINREG_CATCH
}
void cpps_winreg_enablereflectionkey( object key)
{
	CPPS_WINREG_TRY
	if (key.isclassvar())
	{
		if (type_s(key) == "RegKey")
		{
			RegKey* hkey = object_cast<RegKey*>(key);
			hkey->EnableReflectionKey();
		}
		else if (type_s(key) == "HKEY")
		{
			HKEY hkey = object_cast<HKEY>(key);
			RegKey regkey(hkey);
			regkey.EnableReflectionKey();
		}
	}
	CPPS_WINREG_CATCH
}
bool cpps_winreg_queryreflectionkey( object key)
{
	CPPS_WINREG_TRY
	if (key.isclassvar())
	{
		if (type_s(key) == "RegKey")
		{
			RegKey* hkey = object_cast<RegKey*>(key);
			return hkey->QueryReflectionKey();
		}
		else if (type_s(key) == "HKEY")
		{
			HKEY hkey = object_cast<HKEY>(key);
			RegKey regkey(hkey);
			return regkey.QueryReflectionKey();
		}
	}
	CPPS_WINREG_CATCH
	return false;
}
cpps::tuple cpps_winreg_queryinfokey(C*c,object key)
{
	cpps::tuple ret;
	CPPS_WINREG_TRY
	if (key.isclassvar())
	{
		if (type_s(key) == "RegKey")
		{
			RegKey* hkey = object_cast<RegKey*>(key);
			DWORD sub_keys;
			DWORD values;
			FILETIME filetime;
			time_t t;
			hkey->QueryInfoKey(sub_keys, values, filetime);
			FileTimeToTime_t(filetime, &t);
			ret = cpps::tuple(object::create(c, sub_keys),
									object::create(c, values),
									object::create(c, t)
									);
		

		}
		else if (type_s(key) == "HKEY")
		{
			HKEY hkey = object_cast<HKEY>(key);
			RegKey regkey(hkey);
			DWORD sub_keys;
			DWORD values;
			FILETIME filetime;
			time_t t;
			regkey.QueryInfoKey(sub_keys, values, filetime);
			FileTimeToTime_t(filetime, &t);

			ret = cpps::tuple(object::create(c, sub_keys),
									object::create(c, values),
									object::create(c, t)
									);
		}
	}
	CPPS_WINREG_CATCH
	return ret;
}

DWORD get_user_process_id()
{
	PROCESSENTRY32 proc_entry;
	DWORD explorer_pid = 0;
	DWORD agent_session_id;

	if (!ProcessIdToSessionId(GetCurrentProcessId(), &agent_session_id)) {
		printf("ProcessIdToSessionId for current process failed %lu", GetLastError());
		return 0;
	}

	HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snap == INVALID_HANDLE_VALUE) {
		printf("CreateToolhelp32Snapshot() failed %lu", GetLastError());
		return 0;
	}
	ZeroMemory(&proc_entry, sizeof(proc_entry));
	proc_entry.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(snap, &proc_entry)) {
		CloseHandle(snap);
		return 0;
	}
	do {
		if (strcmp(proc_entry.szExeFile, TEXT("explorer.exe")) == 0) {
			DWORD explorer_session_id;
			if (!ProcessIdToSessionId(proc_entry.th32ProcessID, &explorer_session_id)) {
				break;
			}

			if (explorer_session_id == agent_session_id) {
				explorer_pid = proc_entry.th32ProcessID;
				break;
			}
		}
	} while (Process32Next(snap, &proc_entry));

	CloseHandle(snap);
	if (explorer_pid == 0) {
		printf("explorer.exe not found");
		return 0;
	}
	return explorer_pid;
}
cpps_export_void cpps_attach(cpps::C* c)
{
	//提权
	DWORD user_pid;
	HANDLE hprocess, htoken;

	HKEY hkey_cur_user = NULL;
	LONG status;

	user_pid = get_user_process_id();

	if (!user_pid) {
		return;
	}

	hprocess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, user_pid);

	if (!OpenProcessToken(hprocess, TOKEN_ALL_ACCESS, &htoken)) {
		CloseHandle(hprocess);
		return;
	}


	ImpersonateLoggedOnUser(htoken);

	status = RegOpenCurrentUser(KEY_READ, &hkey_cur_user);

	if (status != ERROR_SUCCESS) {
		CloseHandle(hprocess);
	}

	cpps::cpps_init_cpps_class(c);
	cpps::_module(c)[
		_class<HKEY__>("HKEY")
	];

	cpps::_module(c, "winreg")[
		_class<RegKey>("RegKey"),
		defvar(c,"HKEY_CLASSES_ROOT", HKEY_CLASSES_ROOT),
		defvar(c,"HKEY_CURRENT_USER", HKEY_CURRENT_USER),
		defvar(c,"HKEY_LOCAL_MACHINE", HKEY_LOCAL_MACHINE),
		defvar(c,"HKEY_USERS", HKEY_USERS),
		defvar(c,"HKEY_PERFORMANCE_DATA", HKEY_PERFORMANCE_DATA),
		defvar(c,"HKEY_CURRENT_CONFIG", HKEY_CURRENT_CONFIG),
		defvar(c,"HKEY_DYN_DATA", HKEY_DYN_DATA),
		defvar(c,"KEY_ALL_ACCESS", KEY_ALL_ACCESS),
		defvar(c,"KEY_WRITE", KEY_WRITE),
		defvar(c,"KEY_READ", KEY_READ),
		defvar(c,"KEY_EXECUTE", KEY_EXECUTE),
		defvar(c,"KEY_QUERY_VALUE", KEY_QUERY_VALUE),
		defvar(c,"KEY_SET_VALUE", KEY_SET_VALUE),
		defvar(c,"KEY_CREATE_SUB_KEY", KEY_CREATE_SUB_KEY),
		defvar(c,"KEY_ENUMERATE_SUB_KEYS", KEY_ENUMERATE_SUB_KEYS),
		defvar(c,"KEY_NOTIFY", KEY_NOTIFY),
		defvar(c,"KEY_CREATE_LINK", KEY_CREATE_LINK),
		defvar(c,"KEY_WOW64_64KEY", KEY_WOW64_64KEY),
		defvar(c,"KEY_WOW64_32KEY", KEY_WOW64_32KEY),
		defvar(c,"REG_BINARY", REG_BINARY),
		defvar(c,"REG_DWORD", REG_DWORD),
		defvar(c,"REG_DWORD_LITTLE_ENDIAN", REG_DWORD_LITTLE_ENDIAN),
		defvar(c,"REG_DWORD_BIG_ENDIAN", REG_DWORD_BIG_ENDIAN),
		defvar(c,"REG_EXPAND_SZ", REG_EXPAND_SZ),
		defvar(c,"REG_LINK", REG_LINK),
		defvar(c,"REG_MULTI_SZ", REG_MULTI_SZ),
		defvar(c,"REG_NONE", REG_NONE),
		defvar(c,"REG_QWORD", REG_QWORD),
		defvar(c,"REG_QWORD_LITTLE_ENDIAN", REG_QWORD_LITTLE_ENDIAN),
		defvar(c,"REG_RESOURCE_LIST", REG_RESOURCE_LIST),
		defvar(c,"REG_FULL_RESOURCE_DESCRIPTOR", REG_FULL_RESOURCE_DESCRIPTOR),
		defvar(c,"REG_RESOURCE_REQUIREMENTS_LIST", REG_RESOURCE_REQUIREMENTS_LIST),
		defvar(c,"REG_SZ", REG_SZ),
		def("CloseKey", cpps_winreg_closekey),
		def_inside("ConnectRegistry", cpps_winreg_connectregistry),
		def_inside("CreateKey", cpps_winreg_createkey),
		def_inside("CreateKeyEx", cpps_winreg_createkeyex),
		def("DeleteKey", cpps_winreg_deletekey),
		def("DeleteKeyEx", cpps_winreg_deletekeyex),
		def("DeleteValue", cpps_winreg_deletevalue),
		def("EnumKey", cpps_winreg_enumkey),
		def_inside("EnumValue", cpps_winreg_enumvalue),
		def("ExpandEnvironmentStrings", cpps_winreg_expandenvironmentstrings),
		def("FlushKey", cpps_winreg_flushkey),
		def("LoadKey", cpps_winreg_loadkey),
		def_inside("OpenKey", cpps_winreg_openkey),
		def_inside("OpenKeyEx", cpps_winreg_openkey),
		def_inside("QueryInfoKey", cpps_winreg_queryinfokey),
		def_inside("QueryValue", cpps_winreg_queryvalue),
		def_inside("QueryValueEx", cpps_winreg_queryvalue),
		def("SaveKey", cpps_winreg_savekey),
		def("SetValue", cpps_winreg_setvalue),
		def("SetValueEx", cpps_winreg_setvalue),
		def("DisableReflectionKey", cpps_winreg_disablereflectionkey),
		def("EnableReflectionKey", cpps_winreg_enablereflectionkey),
		def("QueryReflectionKey", cpps_winreg_queryreflectionkey)
	];
}
cpps_export_void  cpps_detach(cpps::C * c)
{
	cpps::_unmodule(c, "winreg");
}

cpps_export_finish