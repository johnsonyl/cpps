#ifndef cpps_cpps_htmlparser_element_h__
#define cpps_cpps_htmlparser_element_h__

#include <cpps/cpps.h>
#include "html_parser.hpp"

namespace cpps{

	class cpps_htmlparser_element
	{
	public:
		cpps_htmlparser_element();
		virtual ~cpps_htmlparser_element();
		
		std::string					attr(std::string k);
		cpps_value					children(C* c, std::string k);
		cpps_value					childrens_by_classname(C* c, std::string k);
		cpps_value					childrens_by_tagname(C* c, std::string k);
		cpps_value					childrens(C* c);
		cpps_value					select(C* c, std::string rule);
		cpps_value					parent(C* c);
		std::string					value();
		std::string					name();
		std::string					text();
		std::string					html();
	public:
		std::shared_ptr<HtmlElement> element;
	};
	
}
#endif //cpps_cpps_htmlparser_element_h__