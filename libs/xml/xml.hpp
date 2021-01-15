#pragma once
#include <memory>
#include <sstream>
#include "cpps/cpps.h"
#include "rapidxml.hpp"
#include "rapidxml_print.hpp"
#include <iostream>

namespace cpps
{
    class xml_node
    {
    public:
        std::string name() 
        { 
            std::cout << doc_.get() << std::endl;
            std::cout << doc_->first_node() << std::endl;
            std::cout << node_->name() << std::endl;
            return node_->name(); 
        }
        cpps_value document(C* c);
    private:
        rapidxml::xml_node<>* node_;
        std::shared_ptr<rapidxml::xml_document<>> doc_;

        friend class xml_document;
    };

    class xml_document
    {
    public:
        xml_document()
        {
            std::cout << doc_.get() << std::endl;
            std::cout << "xml_doc()" << std::endl;
            doc_ = std::make_shared<rapidxml::xml_document<>>();
        }
        
        cpps_value load(C* c, std::string xml_str)
        {
            doc_->parse<0>((char *)xml_str.c_str());
            std::cout << doc_.get() << std::endl;
            std::cout << doc_->first_node() << std::endl;
            xml_node* ret_node;
            cpps_value ret = newclass<xml_node>(c, &ret_node);
            ret_node->node_ = doc_->first_node();
            ret_node->doc_ = doc_;
            std::cout << ret_node->node_->name() << std::endl;
            return ret;
        }
        std::string save()
        {
            std::stringstream ss;
            ss << *doc_;
            return ss.str();
        }
    private:
        std::shared_ptr<rapidxml::xml_document<>> doc_;
        
        friend class xml_node;
    };

    cpps_value xml_node::document(C* c)
    {
        xml_document* ret_doc;
        cpps_value ret = newclass<xml_document>(c, &ret_doc);
        ret_doc->doc_ = doc_;
        return ret;
    }
}


