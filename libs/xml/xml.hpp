#pragma once
#include <memory>
#include <sstream>
#include "cpps/cpps.h"
#include "rapidxml.hpp"
#include "rapidxml_print.hpp"

namespace cpps
{
    class xml_node
    {
    public:
        std::string name() 
        {
            return node_->name(); 
        }

        cpps_value children(C* c)
        {
            cpps_vector* ret_vec;
            cpps_value ret = newclass<cpps_vector>(c, &ret_vec);
            for(auto cn = node_->first_node(); cn != nullptr; cn = cn->next_sibling())
            {
                xml_node* ret_node;
                cpps_value node_value = newclass<xml_node>(c, &ret_node);
                ret_node->node_ = cn;
                ret_vec->push_back(node_value);
            }
            return ret;
        }
    protected:
        rapidxml::xml_node<>* node_;
    };

    class xml_document : public xml_node
    {
    public:
        xml_document()
        {
            doc_ = std::make_shared<rapidxml::xml_document<>>();
            node_ = doc_.get();
        }

    public:
        std::string doc_str_;
        std::shared_ptr<rapidxml::xml_document<>> doc_;
    };

    cpps_value cpps_xml_load(C* c, std::string xml_str)
    {
        xml_document* ret_doc;
        cpps_value ret = newclass<xml_document>(c, &ret_doc);
        ret_doc->doc_str_ = xml_str;
        ret_doc->doc_->parse<0>((char *)ret_doc->doc_str_.c_str());
        return ret;
    }
}


