#pragma once
#include <memory>
#include <string>
#include <list>
#include "rapidxml.hpp"

namespace rapidxml_adapter
{
	template < typename Ch >
	using string = std::basic_string<Ch>;
	template < typename Ch >
	class xml_attribute;
	template < typename Ch >
	class xml_node;
	template < typename Ch >
	class xml_document;
	template < typename Ch >
	using xml_attribute_list = std::list<std::shared_ptr<xml_attribute<Ch>>>;
	template < typename Ch >
	using xml_node_list = std::list<std::shared_ptr<xml_node<Ch>>>;

	namespace internal
	{
		template < typename Ch >
		bool compare(const std::basic_string<Ch>& _str1, const std::basic_string<Ch>& _str2, bool _case_sensitive)
		{
			return rapidxml::internal::compare<Ch>(_str1.c_str(), _str1.size(), _str2.c_str(), _str2.size(), _case_sensitive);
		}
	}

	template < typename Ch = char >
	class xml_base : public std::enable_shared_from_this<xml_base<Ch>>
	{
	public:
		xml_base()
			: m_name()
			, m_value()
			, m_parent()
		{ }

		string<Ch> name() const
		{
			return m_name;
		}

		string<Ch> value() const
		{
			return m_value;
		}

		void name(const string<Ch>& _name)
		{
			m_name = _name;
		}

		void value(const string<Ch>& _value)
		{
			m_value = _value;
		}

		std::shared_ptr<xml_node<Ch>> parent() const
		{
			return m_parent.lock();
		}

	protected:
		xml_base(const rapidxml::xml_base<Ch>* _base, std::weak_ptr<xml_node<Ch>> _parent)
			: m_name(_base->name(), _base->name_size())
			, m_value(_base->value(), _base->value_size())
			, m_parent(_parent)
		{
		}

	protected:
		string<Ch>				m_name;
		string<Ch>				m_value;
		std::weak_ptr<xml_node<Ch>>	m_parent;
	};

	template < typename Ch = char >
	class xml_attribute : public xml_base<Ch>
	{
		friend class xml_node<Ch>;
	public:
		xml_attribute()
			: xml_base<Ch>()
		{ }

		xml_attribute(const rapidxml::xml_attribute<Ch>* _attribute, std::weak_ptr<xml_node<Ch>> _parent)
			: xml_base<Ch>(_attribute, _parent)
		{ }
	};

	template < typename Ch = char >
	class xml_node : public xml_base<Ch>
	{
	public:
		xml_node()
			: xml_base<Ch>()
			, m_type((rapidxml::node_type)-1)
			, m_children()
			, m_attributes()
		{ }

		xml_node(rapidxml::node_type _type)
			: xml_base<Ch>()
			, m_type(_type)
			, m_children()
			, m_attributes()
		{ }

		xml_node(const rapidxml::xml_node<Ch> * _node, std::weak_ptr<xml_node<Ch>> _parent)
			: xml_base<Ch>(_node, _parent)
			, m_type(_node->type())
			, m_children()
			, m_attributes()
		{ }

		rapidxml::node_type type() const
		{
			return m_type;
		}

		xml_node_list<Ch> children(const string<Ch>& _name = string<Ch>(), bool _case_sensitive = true) const
		{
			if (_name.empty())
			{
				return m_children;
			}
			xml_node_list<Ch> node_list;
			for (std::shared_ptr<xml_node<Ch>> node : m_children)
			{
				if (internal::compare(_name, node->name(), _case_sensitive))
				{
					node_list.push_back(node);
				}
			}
			return node_list;
		}

		xml_attribute_list<Ch> attributes(const string<Ch>& _name = string<Ch>(), bool _case_sensitive = true) const
		{
			if(_name.empty())
			{
				return m_attributes;
			}
			xml_attribute_list<Ch> attribute_list;
			for(std::shared_ptr<xml_attribute<Ch>> attribute : m_attributes)
			{
				if(internal::compare(_name, attribute->name(), _case_sensitive))
				{
					attribute_list.push_back(attribute);
				}
			}
			return attribute_list;
		}

	protected:
		void convert_from_rapidxml_node(const rapidxml::xml_node<Ch>* _node)
		{
			for(rapidxml::xml_node<Ch>* child = _node->first_node(); child != nullptr; child = child->next_sibling())
			{
				std::shared_ptr<xml_node<Ch>> node = std::make_shared<xml_node<Ch>>(child, shared_ptr_of_this());
				node->convert_from_rapidxml_node(child);
				m_children.emplace_back(node);
			}
			for(rapidxml::xml_attribute<Ch>* child = _node->first_attribute(); child != nullptr; child = child->next_attribute())
			{
				m_attributes.emplace_back(std::make_shared<xml_attribute<Ch>>(child, shared_ptr_of_this()));
			}
		}

		std::shared_ptr<xml_node<Ch>> shared_ptr_of_this()
		{
			return std::static_pointer_cast<xml_node<Ch>>(xml_base<Ch>::shared_from_this());
		}
	protected:
		rapidxml::node_type		m_type;
		xml_node_list<Ch>		m_children;
		xml_attribute_list<Ch>	m_attributes;
	};

	template < typename Ch = char >
	class xml_document : public xml_node<Ch>
	{
	public:
		xml_document()
			: xml_node<Ch>(rapidxml::node_document)
		{ }

		void parse(const string<Ch>& _str)
		{
			clear();
			rapidxml::xml_document<Ch> doc;
			doc.parse((Ch *)_str.c_str());
			xml_node<Ch>::convert_from_rapidxml_node(&doc);
		}

		void clear()
		{
			xml_node<Ch>::m_children.clear();
			xml_node<Ch>::m_attributes.clear();
		}
	};
}
