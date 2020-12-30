#include "stdafx.h"
#include "json/json.h"
#include "jsonconvert.h"

std::string json_itos_func(cpps_integer n)
{
	std::ostringstream result;
	result << n;
	return result.str();
}

Json::Value cppstojsonvalue(cpps::object obj)
{
	Json::Value ret;
	switch (cpps::type(obj))
	{
	case CPPS_TBOOLEAN:
		ret = cpps::object_cast<bool>(obj);
		break;
	case CPPS_TSTRING:
		ret = cpps::object_cast<std::string>(obj);
		break;
	case CPPS_TNUMBER:
		ret = cpps::object_cast<cpps_number>(obj);
		break;
	case CPPS_TINTEGER:
		ret = cpps::object_cast<int>(obj);
		break;
	case CPPS_TFUNCTION:
		ret = std::string("func@") + obj.value.value.func->getfuncname();
		break;
	case CPPS_TLAMBDAFUNCTION:
		ret = std::string("func@") + obj.value.value.func->getfuncname();
		break;
	case CPPS_TCLASSVAR:
	{
		cpps::cpps_cppsclass *cls = (cpps::cpps_cppsclass *)obj.value.value.domain->parent[0];

		if (cls->getclassname() == "map" )
		{
			ret = Json::Value(Json::objectValue);
			cpps::cpps_map *m = cpps::object_cast<cpps::cpps_map*>(obj);
			for (m->begin(); m->end(); m->next())
			{
				cpps::cpps_value k = m->key();
				cpps::cpps_value v = m->it();

				Json::Value v2 = cppstojsonvalue(v);

				if (k.tt == CPPS_TSTRING)
				{
					ret[cpps_to_string(k)] = v2;
				}
				else if (k.tt == CPPS_TINTEGER)
				{
					ret[json_itos_func(k.value.integer)] = v2;
				}
			}
		}
		else if (cls->getclassname() == "vector")
		{
			ret = Json::Value(Json::arrayValue);
			cpps::cpps_vector *m = cpps::object_cast<cpps::cpps_vector*>(obj);
			for (m->begin(); m->end(); m->next())
			{
				cpps::cpps_value v = m->it();
				ret.append(cppstojsonvalue(v));
			}
		}

		break;
	}
	default:

		break;
	}
	
	return ret;
}

std::string cppstojson(cpps::object obj, cpps::object encodeType)
{
	cpps::int32 nencodeType = Json::encode_utf8;
	if (encodeType.isint()) nencodeType = (cpps::int32) encodeType.toint();
	Json::Value ret = cppstojsonvalue(obj);
	ret.encodetyle(nencodeType);
	return ret.toStyledString(nencodeType);
}

cpps::object jsonvaluetocpps(cpps::C *c, Json::Value& v)
{
	if (v.isArray())
	{
		cpps::cpps_vector *vct = NULL;
		cpps::cpps_value ret = cpps::newclass<cpps::cpps_vector>(c, &vct);
		for (int i = 0; i < static_cast<int>(v.size());i++)
		{
			vct->push_back(jsonvaluetocpps(c, v[i]).value);
		}
		return ret;
	}
	else if (v.isBool())
	{
		return cpps::object(c, v.asBool());
	}
	else if (v.isString())
	{
		if (v.asString().find("func@") == 0)
		{
			cpps::object func = _G(c)[v.asString().substr(5)];
			if (cpps::type(func) == CPPS_TFUNCTION || cpps::type(func) == CPPS_TLAMBDAFUNCTION)
			{
				return func;
			}
		}
		return cpps::object(c, v.asString());
	}
	else if (v.isInt())
	{
		return cpps::object(c,v.asInt());
	}
	else if (v.isNumeric())
	{
		return cpps::object(c, v.asDouble());
	}
	else if (v.isObject())
	{

		cpps::cpps_map *vct = NULL;
		cpps::cpps_value ret = cpps::newclass<cpps::cpps_map>(c, &vct);

		Json::Value::Members members = v.getMemberNames();
		for (Json::Value::Members::iterator it = members.begin(); it != members.end(); ++it)
		{
			vct->insert(cpps::cpps_value(c,*it), jsonvaluetocpps(c, v[*it]).value);
		}
		return ret;
	}
	return cpps::cpps_value();
}

cpps::object jsontocpps(cpps::C *c, std::string json)
{
	Json::Value root;
	JSONCPP_STRING errs;
	Json::CharReaderBuilder readerBuilder;
	std::unique_ptr<Json::CharReader> const jsonReader(readerBuilder.newCharReader());
	jsonReader->parse(json.c_str(), json.c_str() + json.length(), &root, &errs);
	
	return jsonvaluetocpps(c, root);
}
