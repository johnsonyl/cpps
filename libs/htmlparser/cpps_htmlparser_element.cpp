#include "cpps_htmlparser_element.h"

namespace cpps
{

	cpps_htmlparser_element::cpps_htmlparser_element()
	{
		
	}
	
	cpps_htmlparser_element::~cpps_htmlparser_element()
	{
		
	}
	


	std::string cpps_htmlparser_element::attr(std::string k)
	{
		return element->GetAttribute(k);
	}

	cpps::cpps_value cpps_htmlparser_element::children(C *c,std::string k)
	{
		std::shared_ptr<HtmlElement> ele = element->GetElementById(k);
		if (!ele) return nil;

		cpps_htmlparser_element* ret_ele;
		cpps_value ret = newclass<cpps_htmlparser_element>(c, &ret_ele);
		ret_ele->element = ele;
		return ret;
	}

	cpps::cpps_value cpps_htmlparser_element::childrens_by_classname(C* c, std::string k)
	{
		std::vector<std::shared_ptr<HtmlElement>> eles;
		element->GetElementByClassName(k,eles);

		cpps_vector* ret_vct;
		cpps_value ret = newclass< cpps_vector>(c, &ret_vct);
		for (auto& ele : eles) {
			cpps_htmlparser_element* ret_ele;
			cpps_value ele_val = newclass<cpps_htmlparser_element>(c, &ret_ele);
			ret_ele->element = ele;
			ret_vct->push_back(ele_val);
		}
		
		return ret;
	}

	cpps::cpps_value cpps_htmlparser_element::childrens_by_tagname(C* c, std::string k)
	{
		std::vector<std::shared_ptr<HtmlElement>> eles;
		element->GetElementByTagName(k, eles);

		cpps_vector* ret_vct;
		cpps_value ret = newclass< cpps_vector>(c, &ret_vct);
		for (auto& ele : eles) {
			cpps_htmlparser_element* ret_ele;
			cpps_value ele_val = newclass<cpps_htmlparser_element>(c, &ret_ele);
			ret_ele->element = ele;
			ret_vct->push_back(ele_val);
		}

		return ret;
	}

	cpps::cpps_value cpps_htmlparser_element::childrens(C* c)
	{
		std::vector<std::shared_ptr<HtmlElement>> eles;
		element->GetAllElement(eles);
		cpps_vector* ret_vct;
		cpps_value ret = newclass< cpps_vector>(c, &ret_vct);
		for (auto& ele : eles) {
			cpps_htmlparser_element* ret_ele;
			cpps_value ele_val = newclass<cpps_htmlparser_element>(c, &ret_ele);
			ret_ele->element = ele;
			ret_vct->push_back(ele_val);
		}

		return ret;
	}

	cpps::cpps_value cpps_htmlparser_element::select(C* c, std::string rule)
	{
		std::vector<std::shared_ptr<HtmlElement>> eles;
		element->SelectElement(rule,eles);
		cpps_vector* ret_vct;
		cpps_value ret = newclass< cpps_vector>(c, &ret_vct);
		for (auto& ele : eles) {
			cpps_htmlparser_element* ret_ele;
			cpps_value ele_val = newclass<cpps_htmlparser_element>(c, &ret_ele);
			ret_ele->element = ele;
			ret_vct->push_back(ele_val);
		}

		return ret;
	}

	cpps::cpps_value cpps_htmlparser_element::parent(C* c)
	{
		std::shared_ptr<HtmlElement> ele = element->GetParent();
		if (!ele) return nil;

		cpps_htmlparser_element* ret_ele;
		cpps_value ret = newclass<cpps_htmlparser_element>(c, &ret_ele);
		ret_ele->element = ele;
		return ret;
	}

	std::string cpps_htmlparser_element::value()
	{
		return element->GetValue();
	}

	std::string cpps_htmlparser_element::name()
	{
		return element->GetName();
	}

	std::string cpps_htmlparser_element::text()
	{
		return element->text();
	}

	std::string cpps_htmlparser_element::html()
	{
		return element->html();
	}

}
