#include "cpps_htmlparser_parser.h"
#include "cpps_htmlparser_element.h"

namespace cpps
{

	cpps_htmlparser_parser::cpps_htmlparser_parser()
	{
		
	}
	
	cpps_htmlparser_parser::~cpps_htmlparser_parser()
	{
		
	}
	
	cpps_value cpps_htmlparser_parser::parse(C*c,std::string html)
	{
		doc = parser.Parse(html.c_str(), html.size());
		cpps_htmlparser_element* ret_ele;
		cpps_value ret = newclass<cpps_htmlparser_element>(c, &ret_ele);
		ret_ele->element = doc->GetRoot();
		return ret;
	}

}
