#ifndef cpps_cpps_htmlparser_parser_h__
#define cpps_cpps_htmlparser_parser_h__

#include <cpps/cpps.h>
#include "html_parser.hpp"

namespace cpps{
	class cpps_htmlparser_element;
	class cpps_htmlparser_parser
	{
	public:
		cpps_htmlparser_parser();
		virtual ~cpps_htmlparser_parser();
		
		cpps_value parse(C *c,std::string html);
	public:
		HtmlParser parser;
		shared_ptr<HtmlDocument> doc;
	};
	
}
#endif //cpps_cpps_htmlparser_parser_h__