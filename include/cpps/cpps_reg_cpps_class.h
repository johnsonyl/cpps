#pragma once

namespace cpps
{
	template<class T>
	bool	cpps_reg_cpps_class(cpps::C* c, std::string classname, std::string node = "_G")
	{
		cpps_domain* root = c->_G;
		cpps_domain* leftdomain = NULL;
		if (node != "_G")
		{
			cpps::cpps_regvar* d = c->_G->getvar(node, leftdomain, false);
			if (!d)
				return false;
			if (!d->getval().isdomain())
				return false;

			root = d->getval().value.domain;
		}
		leftdomain = NULL;
		cpps::cpps_regvar* var = root->getvar(classname, leftdomain, false);
		cpps_cppsclass* cls = (cpps_cppsclass*)var->getval().value.domain;
		cpps_class_singleton<T*>::instance()->setsls(cls);
		return true;
	}
	void	cpps_init_cpps_class(cpps::C* c);
}