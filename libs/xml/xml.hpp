#pragma once
#include "cpps/cpps.h"
#include "rapidxml_adapter.hpp"
#include "rapidxml_adapter_print.hpp"
#include <iostream>

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

        void create(cpps_value name, cpps_value value)
        {
            if (cpps_isstring(name))
            {
		        std::string* str = cpps_get_string(name);
                set_name(*str);
            }
            if (cpps_isstring(value))
            {
		        std::string* str = cpps_get_string(value);
                set_value(*str);
            }
        }

        std::string name()
        {
            return attribute_->name();
        }

        std::string value()
        {
            return attribute_->value();
        }

        void set_name(std::string name)
        {
            attribute_->name(name);
        }

        void set_value(std::string value)
        {
            attribute_->value(value);
        }

    private:
        std::shared_ptr<rapidxml_adapter::xml_attribute<char>> attribute_;
    };

    class xml_node
    {
    public:
        xml_node()
        {
            node_ = std::make_shared<rapidxml_adapter::xml_node<char>>(rapidxml::node_element);
        }

        void create(cpps_value name, cpps_value value, cpps_value type_name)
        {
            if (cpps_isstring(name))
            {
		        std::string* str = cpps_get_string(name);
                set_name(*str);
            }
            if (cpps_isstring(value))
            {
		        std::string* str = cpps_get_string(value);
                set_value(*str);
            }
            if (cpps_isstring(type_name))
            {
		        std::string* str = cpps_get_string(value);
                set_type(*str);
            }
        }

        std::string to_string()
        {
            std::stringstream ss;
            ss << node_;
            return ss.str();
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

        xml_node* first_node(C* c, cpps_value name)
        {
            std::string node_name;
            if (cpps_isstring(name))
            {
                node_name = *cpps_get_string(name);
            }
		    xml_node* ret = nullptr;
            std::shared_ptr<rapidxml_adapter::xml_node<char>> temp_node = node_->first_node(node_name);
            if (temp_node)
            {
		        cpps::object::create_with_classvar<xml_node>(c, &ret);
                ret->node_ = temp_node;
            }
            return ret;
        }

        xml_node* last_node(C* c, cpps_value name)
        {
            std::string node_name;
            if (cpps_isstring(name))
            {
                node_name = *cpps_get_string(name);
            }
		    xml_node* ret = nullptr;
            std::shared_ptr<rapidxml_adapter::xml_node<char>> temp_node = node_->last_node(node_name);
            if (temp_node)
            {
		        cpps::object::create_with_classvar<xml_node>(c, &ret);
                ret->node_ = temp_node;
            }
            return ret;
        }

        xml_node* previous_node(C* c, xml_node* current, cpps_value name)
        {
            if (current == nullptr)
            {
                return last_node(c, name);
            }
            if (current->node_->parent() != node_)
            {
                cpps::error(c, "xml_node::previous_node current node must be self's child");
            }
            std::string node_name;
            if (cpps_isstring(name))
            {
                node_name = *cpps_get_string(name);
            }
            xml_node* ret = nullptr;
            std::shared_ptr<rapidxml_adapter::xml_node<char>> temp_node = node_->previous_node(current->node_, node_name);
            if (temp_node)
            {
		        cpps::object::create_with_classvar<xml_node>(c, &ret);
                ret->node_ = temp_node;
            }
            return ret;
        }

        xml_node* next_node(C* c, xml_node* current, cpps_value name)
        {
            if (current == nullptr)
            {
                return first_node(c, name);
            }
            if (current->node_->parent() != node_)
            {
                cpps::error(c, "xml_node::next_node current node must be self's child");
            }
            std::string node_name;
            if (cpps_isstring(name))
            {
                node_name = *cpps_get_string(name);
            }
            xml_node* ret = nullptr;
            std::shared_ptr<rapidxml_adapter::xml_node<char>> temp_node = node_->next_node(current->node_, node_name);
            if (temp_node)
            {
		        cpps::object::create_with_classvar<xml_node>(c, &ret);
                ret->node_ = temp_node;
            }
            return ret;
        }

        xml_attribute* first_attribute(C* c, cpps_value name)
        {
            std::string attribute_name;
            if (cpps_isstring(name))
            {
                attribute_name = *cpps_get_string(name);
            }
            xml_attribute* ret = nullptr;
            std::shared_ptr<rapidxml_adapter::xml_attribute<char>> temp_attribute = node_->first_attribute(attribute_name);
            if (temp_attribute)
            {
		        cpps::object::create_with_classvar<xml_attribute>(c, &ret);
                ret->attribute_ = temp_attribute;
            }
            return ret;
        }

        xml_attribute* last_attribute(C* c, cpps_value name)
        {
            std::string attribute_name;
            if (cpps_isstring(name))
            {
                attribute_name = *cpps_get_string(name);
            }
            xml_attribute* ret = nullptr;
            std::shared_ptr<rapidxml_adapter::xml_attribute<char>> temp_attribute = node_->last_attribute(attribute_name);
            if (temp_attribute)
            {
		        cpps::object::create_with_classvar<xml_attribute>(c, &ret);
                ret->attribute_ = temp_attribute;
            }
            return ret;
        }

        xml_attribute* previous_attribute(C* c, xml_attribute* current, cpps_value name)
        {
            if (current == nullptr)
            {
                return last_attribute(c, name);
            }
            if (current->attribute_->parent() != node_)
            {
                cpps::error(c, "xml_node::previous_attribute current attribute must be self's child");
            }
            std::string attribute_name;
            if (cpps_isstring(name))
            {
                attribute_name = *cpps_get_string(name);
            }
            xml_attribute* ret = nullptr;
            std::shared_ptr<rapidxml_adapter::xml_attribute<char>> temp_attribute = node_->previous_attribute(current->attribute_, attribute_name);
            if (temp_attribute)
            {
		        cpps::object::create_with_classvar<xml_attribute>(c, &ret);
                ret->attribute_ = temp_attribute;
            }
            return ret;
        }

        xml_attribute* next_attribute(C* c, xml_attribute* current, cpps_value name)
        {
            if (current == nullptr)
            {
                return first_attribute(c, name);
            }
            if (current->attribute_->parent() != node_)
            {
                cpps::error(c, "xml_node::next_attribute current attribute must be self's child");
            }
            std::string attribute_name;
            if (cpps_isstring(name))
            {
                attribute_name = *cpps_get_string(name);
            }
            xml_attribute* ret = nullptr;
            std::shared_ptr<rapidxml_adapter::xml_attribute<char>> temp_attribute = node_->next_attribute(current->attribute_, attribute_name);
            if (temp_attribute)
            {
		        cpps::object::create_with_classvar<xml_attribute>(c, &ret);
                ret->attribute_ = temp_attribute;
            }
            return ret;
        }

        cpps_value children(C* c, cpps_value name)
        {
            std::string node_name;
            if (cpps_isstring(name))
            {
                node_name = *cpps_get_string(name);
            }
            return xml_node::children_to_cpps_value(c, node_name);
        }

        cpps_value attributes(C* c, cpps_value name)
        {
            std::string attribute_name;
            if (cpps_isstring(name))
            {
                attribute_name = *cpps_get_string(name);
            }
            return xml_node::attributes_to_cpps_value(c, attribute_name);
        }

        void set_type(std::string type_name)
        {
            static const std::map<std::string, rapidxml::node_type> node_type_by_name = {
                {"document", rapidxml::node_document},
                {"element", rapidxml::node_element},
                {"data", rapidxml::node_data},
                {"cdata", rapidxml::node_cdata},
                {"comment", rapidxml::node_comment},
                {"declaration", rapidxml::node_declaration},
                {"doctype", rapidxml::node_doctype},
                {"pi", rapidxml::node_pi},
            };
            auto iter = node_type_by_name.find(type_name);
            if (iter != node_type_by_name.end())
            {
                node_->type(iter->second);
            }
        }

        void set_name(std::string name)
        {
            node_->name(name);
        }

        void set_value(std::string value)
        {
            node_->value(value);
        }

        void prepend_node(C* c, xml_node* node)
        {
            if (node == nullptr)
            {
                cpps::error(c, "xml_node::prepend_node new node is nil");
            }
            if (node->node_->parent())
            {
                cpps::error(c, "xml_node::prepend_node new node must not have parent");
            }
            if (node->node_->type() == rapidxml::node_document)
            {
                cpps::error(c, "xml_node::prepend_node new node must not be a document node");
            }
            node_->prepend_node(node->node_);
        }

        void append_node(C* c, xml_node* node)
        {
            if (node == nullptr)
            {
                cpps::error(c, "xml_node::append_node new node is nil");
            }
            if (node->node_->parent())
            {
                cpps::error(c, "xml_node::append_node new node must not have parent");
            }
            if (node->node_->type() == rapidxml::node_document)
            {
                cpps::error(c, "xml_node::append_node new node must not be a document node");
            }
            node_->append_node(node->node_);
        }

        void insert_node(C* c, xml_node* where, xml_node* node)
        {
            if (where == nullptr)
            {
                append_node(c, node);
            }
            if (where->node_->parent() != node_)
            {
                cpps::error(c, "xml_node::insert_node where node must be self's child");
            }
            if (node == nullptr)
            {
                cpps::error(c, "xml_node::insert_node new node is nil");
            }
            if (node->node_->parent())
            {
                cpps::error(c, "xml_node::insert_node new node must not have parent");
            }
            if (node->node_->type() == rapidxml::node_document)
            {
                cpps::error(c, "xml_node::insert_node new node must not be a document node");
            }
            node_->insert_node(where->node_, node->node_);
        }

        void remove_first_node(C* c)
        {
            if (!node_->first_node())
            {
                cpps::error(c, "xml_node::remove_first_node has no child to remove");
            }
            node_->remove_first_node();
        }

        void remove_last_node(C* c)
        {
            if (!node_->first_node())
            {
                cpps::error(c, "xml_node::remove_last_node has no child to remove");
            }
            node_->remove_last_node();
        }

        void remove_node(C* c, xml_node* node)
        {
            if (node == nullptr)
            {
                cpps::error(c, "xml_node::remove_node node is nil");
            }
            if (node->node_->parent() != node_)
            {
                cpps::error(c, "xml_node::insert_node node must be self's child");
            }
            node_->remove_node(node->node_);
        }

        void remove_all_nodes()
        {
            node_->remove_all_nodes();
        }

        void prepend_attribute(C* c, xml_attribute* attribute)
        {
            if (attribute == nullptr)
            {
                cpps::error(c, "xml_node::prepend_attribute new attribute is nil");
            }
            if (attribute->attribute_->parent())
            {
                cpps::error(c, "xml_node::prepend_attribute new attribute must not have parent");
            }
            node_->prepend_attribute(attribute->attribute_);
        }

        void append_attribute(C* c, xml_attribute* attribute)
        {
            if (attribute == nullptr)
            {
                cpps::error(c, "xml_node::append_attribute new attribute is nil");
            }
            if (attribute->attribute_->parent())
            {
                cpps::error(c, "xml_node::append_attribute new attribute must not have parent");
            }
            node_->append_attribute(attribute->attribute_);
        }

        void insert_attribute(C* c, xml_attribute* where, xml_attribute* attribute)
        {
            if (where == nullptr)
            {
                append_attribute(c, attribute);
            }
            if (where->attribute_->parent() != node_)
            {
                cpps::error(c, "xml_node::insert_attribute where attribute must be self's child");
            }
            if (attribute == nullptr)
            {
                cpps::error(c, "xml_node::insert_attribute new attribute is nil");
            }
            if (attribute->attribute_->parent())
            {
                cpps::error(c, "xml_node::insert_attribute new attribute must not have parent");
            }
            node_->insert_attribute(where->attribute_, attribute->attribute_);
        }

        void remove_first_attribute(C* c)
        {
            if (!node_->first_attribute())
            {
                cpps::error(c, "xml_node::remove_first_attribute has no attribute to remove");
            }
            node_->remove_first_attribute();
        }

        void remove_last_attribute(C* c)
        {
            if (!node_->first_attribute())
            {
                cpps::error(c, "xml_node::remove_last_attribute has no attribute to remove");
            }
            node_->remove_last_attribute();
        }

        void remove_attribute(C* c, xml_attribute* attribute)
        {
            if (attribute == nullptr)
            {
                cpps::error(c, "xml_node::remove_attribute attribute is nil");
            }
            if (attribute->attribute_->parent() != node_)
            {
                cpps::error(c, "xml_node::remove_attribute attribute must be self's child");
            }
            node_->remove_attribute(attribute->attribute_);
        }

        void remove_all_attributes()
        {
            node_->remove_all_attributes();
        }
    private:
        cpps_value children_to_cpps_value(C* c, const std::string& name = std::string())
        {
            cpps_vector* ret_vec;
            cpps_value ret;
            newclass<cpps_vector>(c, &ret_vec,&ret);
            for(auto child : node_->children(name))
            {
                xml_node* ret_node;
                cpps_value node_value;
                newclass<xml_node>(c, &ret_node,&node_value);
                ret_node->node_ = child;
                ret_vec->push_back(node_value);
            }
            return ret;
        }

        cpps_value attributes_to_cpps_value(C* c, const std::string &name = std::string())
        {
            cpps_vector* ret_vec;
            cpps_value ret;
            newclass<cpps_vector>(c, &ret_vec,&ret);
            for(auto attr : node_->attributes(name))
            {
                xml_attribute* ret_attr;
                cpps_value attr_value;
                newclass<xml_attribute>(c, &ret_attr,&attr_value);
                ret_attr->attribute_ = attr;
                ret_vec->push_back(attr_value);
            }
            return ret;
        }
    protected:
        std::shared_ptr<rapidxml_adapter::xml_node<char>> node_;
    };

    class xml_document : public xml_node
    {
    public:
        xml_document()
        {
            node_ = std::make_shared<rapidxml_adapter::xml_document<char>>();
        }

        void parse( std::string str)
        {
            std::shared_ptr<rapidxml_adapter::xml_document<char>> doc_ = std::static_pointer_cast<rapidxml_adapter::xml_document<char>>(node_);
            doc_->parse<rapidxml::parse_no_string_terminators>(str);
        }

        void clear()
        {
            std::shared_ptr<rapidxml_adapter::xml_document<char>> doc_ = std::static_pointer_cast<rapidxml_adapter::xml_document<char>>(node_);
            doc_->clear();
        }
    };

    void xml_check(C* c, bool expr, cpps_value info)
    {
        std::string info_str;
        if (cpps_isstring(info))
        {
            info_str = *cpps_get_string(info);
        }
        
        if (!expr)
        {
            cpps::error(c, "xml_check fail [%s]!!!", info_str.c_str());
        }
        
    }
}


