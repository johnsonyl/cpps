#pragma once
#include <memory>
#include <sstream>
#include "cpps/cpps.h"
#include "rapidxml_adapter.hpp"

namespace cpps
{
    class xml_attribute
    {
        friend class xml_node;
    public:
        xml_attribute()
        {
            attribute_ = std::make_shared<rapidxml_adapter::xml_attribute<char>>();
        }

        std::string name()
        {
            return attribute_->name();
        }

        std::string value()
        {
            return attribute_->value();
        }

    private:
        std::shared_ptr<rapidxml_adapter::xml_attribute<char>> attribute_;
    };

    class xml_node
    {
        friend class xml_document;
    public:
        xml_node()
        {
            node_ = std::make_shared<rapidxml_adapter::xml_node<char>>(rapidxml::node_comment);
        }

        std::string type()
        {
            switch (node_->type())
            {
            case rapidxml::node_document:
                return "document";
            case rapidxml::node_element:
                return "element";
            case rapidxml::node_data:
                return "data";
            case rapidxml::node_cdata:
                return "cdata";
            case rapidxml::node_comment:
                return "comment";
            case rapidxml::node_declaration:
                return "declaration";
            case rapidxml::node_doctype:
                return "doctype";
            case rapidxml::node_pi:
                return "pi";
            default:
                return "none";
            }
            return "none";
        }

        std::string name() 
        {
            return node_->name(); 
        }

        std::string value()
        {
            return node_->value();
        }

        cpps_value children(C* c)
        {
            return xml_node::children_to_cpps_value(c, node_);
        }

        cpps_value children_by_name(C* c, std::string name)
        {
            return xml_node::children_to_cpps_value(c, node_, name);
        }

        cpps_value attributes(C* c)
        {
            return xml_node::attributes_to_cpps_value(c, node_);
        }

        cpps_value attributes_by_name(C* c, std::string name)
        {
            return xml_node::attributes_to_cpps_value(c, node_, name);
        }
    private:
        static cpps_value children_to_cpps_value(C* c, std::shared_ptr<rapidxml_adapter::xml_node<char>> node, const std::string& name = std::string())
        {
            cpps_vector* ret_vec;
            cpps_value ret;
            newclass<cpps_vector>(c, &ret_vec,&ret);
            for(auto child : node->children(name))
            {
                xml_node* ret_node;
                cpps_value node_value;
                newclass<xml_node>(c, &ret_node,&node_value);
                ret_node->node_ = child;
                ret_vec->push_back(node_value);
            }
            return ret;
        }

        static cpps_value attributes_to_cpps_value(C* c, std::shared_ptr<rapidxml_adapter::xml_node<char>> node,const std::string &name = std::string())
        {
            cpps_vector* ret_vec;
            cpps_value ret;
            newclass<cpps_vector>(c, &ret_vec,&ret);
            for(auto attr : node->attributes(name))
            {
                xml_attribute* ret_attr;
                cpps_value attr_value;
                newclass<xml_attribute>(c, &ret_attr,&attr_value);
                ret_attr->attribute_ = attr;
                ret_vec->push_back(attr_value);
            }
            return ret;
        }
    private:
        std::shared_ptr<rapidxml_adapter::xml_node<char>> node_;
    };

    class xml_document
    {
    public:
        xml_document()
        {
            doc_ = std::make_shared<rapidxml_adapter::xml_document<char>>();
        }

        void parse( std::string str)
        {
            doc_->parse(str);
        }

        void clear()
        {
            doc_->clear();
        }

        cpps_value children(C* c)
        {
            return xml_node::children_to_cpps_value(c, doc_);
        }

        cpps_value children_by_name(C* c, std::string name)
        {
            return xml_node::children_to_cpps_value(c, doc_, name);
        }

    private:
        std::shared_ptr<rapidxml_adapter::xml_document<char>> doc_;
    };
}


