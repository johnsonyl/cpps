#include "cpps/cpps.h"
#include "cpps_logging_scripthandler.h"

cpps::cpps_logging_scripthandler::cpps_logging_scripthandler()
{
	_parent_c = NULL;
	_c = NULL;
}

cpps::cpps_logging_scripthandler::cpps_logging_scripthandler(C* c)
{
	_parent_c = c;
	_c = NULL;
}

cpps::cpps_logging_scripthandler::~cpps_logging_scripthandler()
{
	
}
void cpps::cpps_logging_scripthandler::constructor(C* c,object _cls)
{
	object _class;
	_parent_c = c;
	if (_cls.isclass()) {
		_class = _cls;
	}
	else if (_cls.isstring()) {
		_class = object::globals(_parent_c)[_cls.tostring()];
	}
	else {
		cpps::error(c, "unknow script handler.");
	}
	cpps::newcppsclasvar(_parent_c, cpps_to_cpps_cppsclass(_class.realval()), &_classvar.realval());

}
void cpps::cpps_logging_scripthandler::create(std::string& _cls,cpps_map* config) {
	
	cpps_value _configv;
	cpps_map* _config_real;
	cpps::newclass(_parent_c, &_config_real, &_configv);
	_config = _configv;
	object _class = object::globals(_parent_c)[_cls];
	cpps::newcppsclasvar(_parent_c, cpps_to_cpps_cppsclass(_class.realval()), &_classvar.realval());

	_config_real->realmap().insert(config->realmap().begin(),config->realmap().end());
}
void cpps::cpps_logging_scripthandler::pop(usint8 level, std::string& msg)
{
	C* c = _c;
	if (_c == NULL) {
		_c = cpps::create(_parent_c);
		_CPPS_TRY
			if (_config.ismap()) {
				object func = _classvar["create"];
				if (func.isfunction()) {
					cpps::doclassfunction(_c, _classvar, func, _config);
				}
			}
		_CPPS_CATCH
	}
	_CPPS_TRY
		object func = _classvar["pop"];
		if (func.isfunction()) {
			cpps::doclassfunction(_c, _classvar, func, level, msg);
		}
	_CPPS_CATCH

}

void cpps::cpps_logging_scripthandler::close()
{
	if (_c != NULL) {
		cpps::close(_c, _parent_c);
		_c = NULL;
	}
}

cpps::cpps_value cpps::cpps_logging_scripthandler::get_handler()
{
	return _classvar.realval();
}
