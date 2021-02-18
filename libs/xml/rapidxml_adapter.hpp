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

		std::shared_ptr<xml_node<Ch>> parent() const
		{
			return m_parent.lock();
		}

		void name(const string<Ch>& _name)
		{
			m_name = _name;
		}

		void value(const string<Ch>& _value)
		{
			m_value = _value;
		}

		void reset_parent()
		{
			m_parent.reset();
		}

	protected:
		xml_base(const rapidxml::xml_base<Ch>* _base, std::weak_ptr<xml_node<Ch>> _parent)
			: m_name(_base->name(), _base->name_size())
			, m_value(_base->value(), _base->value_size())
			, m_parent(_parent)
		{
		}

		void parent(std::weak_ptr<xml_node<Ch>> _parent)
		{
			m_parent = _parent;
		}

	private:
		string<Ch>					m_name;
		string<Ch>					m_value;
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
		xml_node(rapidxml::node_type _type = static_cast<rapidxml::node_type>(-1))
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
		{
		}

		rapidxml::node_type type() const
		{
			return m_type;
		}

		std::shared_ptr<xml_document<Ch>> document() const
		{
			std::shared_ptr<xml_node<Ch>> node = shared_ptr_of_this();
			while(node->parent())
			{
				node = node->parent();
			}
			return node->type() == rapidxml::node_document ? std::static_pointer_cast<xml_document<Ch>>(node) : std::shared_ptr<xml_document<Ch>>();
		}

		std::shared_ptr<xml_node<Ch>> first_node(const string<Ch>& _name = string<Ch>(), bool _case_sensitive = true) const
		{
			if (_name.empty())
			{
				return m_children.empty() ? std::shared_ptr<xml_node<Ch>>() : m_children.front();
			}
			else
			{
				for (auto iter = m_children.begin(); iter != m_children.end(); ++ iter)
				{
					if (internal::compare(_name, (*iter)->name(), _case_sensitive))
					{
						return *iter;
					}
				}
			}
			return std::shared_ptr<xml_node<Ch>>();
		}

		std::shared_ptr<xml_node<Ch>> last_node(const string<Ch>& _name = string<Ch>(), bool _case_sensitive = true) const
		{
			assert(m_children.size() > 0);
			if (_name.empty())
			{
				return m_children.back();
			}
			else
			{
				for (auto iter = m_children.rbegin(); iter != m_children.rend(); ++ iter)
				{
					if (internal::compare(_name, (*iter)->name(), _case_sensitive))
					{
						return *iter;
					}
				}
				
			}
			return std::shared_ptr<xml_node<Ch>>();
		}

		std::shared_ptr<xml_node<Ch>> previous_node(const std::shared_ptr<xml_node<Ch>> _current, const string<Ch>& _name = string<Ch>(), bool _case_sensitive = true) const
		{
			assert(!_current || _current->parent() == shared_ptr_of_this());
			if (!_current)
			{
				return last_node(_name, _case_sensitive);
			}
			bool found = false;
			for (auto iter = m_children.rbegin(); iter != m_children.rend(); ++ iter)
			{
				if (!found)
				{
					if (*iter == _current)
					{
						found = true;
					}
				}
				else
				{
					if (_name.empty() || internal::compare(_name, (*iter)->name(), _case_sensitive))
					{
						return *iter;
					}
				}
			}
			return std::shared_ptr<xml_node<Ch>>();
		}

		std::shared_ptr<xml_node<Ch>> next_node(const std::shared_ptr<xml_node<Ch>> _current, const string<Ch>& _name = string<Ch>(), bool _case_sensitive = true) const
		{
			assert(!_current || _current->parent() == shared_ptr_of_this());
			if (!_current)
			{
				return first_node(_name, _case_sensitive);
			}
			bool found = false;
			for (auto iter = m_children.begin(); iter != m_children.end(); ++ iter)
			{
				if (!found)
				{
					if (*iter == _current)
					{
						found = true;
					}
				}
				else
				{
					if (_name.empty() || internal::compare(_name, (*iter)->name(), _case_sensitive))
					{
						return *iter;
					}
				}
			}
			return std::shared_ptr<xml_node<Ch>>();
		}
		
		std::shared_ptr<xml_attribute<Ch>> first_attribute(const string<Ch>& _name = string<Ch>(), bool _case_sensitive = true) const
		{
			if (_name.empty())
			{
				return m_attributes.empty() ? std::shared_ptr<xml_attribute<Ch>>() : m_attributes.front();
			}
			else
			{
				for (auto iter = m_attributes.begin(); iter != m_attributes.end(); ++ iter)
				{
					if (internal::compare(_name, (*iter)->name(), _case_sensitive))
					{
						return *iter;
					}
				}
			}
			return std::shared_ptr<xml_attribute<Ch>>();
		}

		std::shared_ptr<xml_attribute<Ch>> last_attribute(const string<Ch>& _name = string<Ch>(), bool _case_sensitive = true) const
		{
			assert(m_attributes.size() > 0);
			if (_name.empty())
			{
				return m_attributes.back();
			}
			else
			{
				for (auto iter = m_attributes.rbegin(); iter != m_attributes.rend(); ++ iter)
				{
					if (internal::compare(_name, (*iter)->name(), _case_sensitive))
					{
						return *iter;
					}
				}
				
			}
			return std::shared_ptr<xml_attribute<Ch>>();
		}

		std::shared_ptr<xml_attribute<Ch>> previous_attribute(const std::shared_ptr<xml_attribute<Ch>> _current, const string<Ch>& _name = string<Ch>(), bool _case_sensitive = true) const
		{
			assert(!_current || _current->parent() == shared_ptr_of_this());
			if (!_current)
			{
				return last_attribute(_name, _case_sensitive);
			}
			bool found = false;
			for (auto iter = m_attributes.rbegin(); iter != m_attributes.rend(); ++ iter)
			{
				if (!found)
				{
					if (*iter == _current)
					{
						found = true;
					}
				}
				else
				{
					if (_name.empty() || internal::compare(_name, (*iter)->name(), _case_sensitive))
					{
						return *iter;
					}
				}
			}
			return std::shared_ptr<xml_attribute<Ch>>();
		}

		std::shared_ptr<xml_attribute<Ch>> next_attribute(const std::shared_ptr<xml_attribute<Ch>> _current, const string<Ch>& _name = string<Ch>(), bool _case_sensitive = true) const
		{
			assert(!_current || _current->parent() == shared_ptr_of_this());
			if (!_current)
			{
				return first_attribute(_name, _case_sensitive);
			}
			bool found = false;
			for (auto iter = m_attributes.begin(); iter != m_attributes.end(); ++ iter)
			{
				if (!found)
				{
					if (*iter == _current)
					{
						found = true;
					}
				}
				else
				{
					if (_name.empty() || internal::compare(_name, (*iter)->name(), _case_sensitive))
					{
						return *iter;
					}
				}
			}
			return std::shared_ptr<xml_attribute<Ch>>();
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

		void type(rapidxml::node_type _type)
		{
			m_type = _type;
		}

		void prepend_node(std::shared_ptr<xml_node<Ch>> _child)
		{
			assert(_child && !_child->parent() && _child->type() != rapidxml::node_document);
			_child->parent(shared_ptr_of_this());
			m_children.emplace_front(_child);
		}

		void append_node(std::shared_ptr<xml_node<Ch>> _child)
		{
			assert(_child && !_child->parent() && _child->type() != rapidxml::node_document);
			_child->parent(shared_ptr_of_this());
			m_children.emplace_back(_child);
		}

		void insert_node(std::shared_ptr<xml_node<Ch>> _where, std::shared_ptr<xml_node<Ch>> _child)
		{
			assert(!_where || _where->parent() == shared_ptr_of_this());
			assert(_child && !_child->parent() && _child->type() != rapidxml::node_document);
			if (!_where)
			{
				append_node(_child);
			}
			else
			{
				_child->parent(shared_ptr_of_this());
				for (auto iter = m_children.begin(); iter != m_children.end(); ++ iter)
				{
					if ((*iter) == _where)
					{
						m_children.emplace(iter, _child);
						break;
					}
				}
			}
		}

		void remove_first_node()
		{
			assert(m_children.size() > 0);
			m_children.front()->reset_parent();
			m_children.pop_front();
		}

		void remove_last_node()
		{
			assert(m_children.size() > 0);
			m_children.back()->reset_parent();
			m_children.pop_back();
		}

		void remove_node(std::shared_ptr<xml_node<Ch>> _where)
		{
			assert(_where && _where->parent() == shared_ptr_of_this());
			for (auto iter = m_children.begin(); iter != m_children.end(); ++ iter)
			{
				if ((*iter) == _where)
				{
					_where->reset_parent();
					m_children.erase(iter);
					break;
				}
			}
		}

		void remove_all_nodes()
		{
			for(auto node : m_children)
			{
				node->reset_parent();
			}
			m_children.clear();
		}

		void prepend_attribute(std::shared_ptr<xml_attribute<Ch>> _attribute)
		{
			assert(_attribute && !_attribute->parent());
			_attribute->parent(shared_ptr_of_this());
			m_attributes.emplace_front(_attribute);
		}

		void append_attribute(std::shared_ptr<xml_attribute<Ch>> _attribute)
		{
			assert(_attribute && !_attribute->parent());
			_attribute->parent(shared_ptr_of_this());
			m_attributes.emplace_back(_attribute);
		}

		void insert_attribute(std::shared_ptr<xml_attribute<Ch>> _where, std::shared_ptr<xml_attribute<Ch>> _attribute)
		{
			assert(!_where || _where->parent() == shared_ptr_of_this());
			assert(_attribute && !_attribute->parent());
			if (!_where)
			{
				append_attribute(_attribute);
			}
			else
			{
				_attribute->parent(shared_ptr_of_this());
				for(auto iter = m_attributes.begin(); iter != m_attributes.end(); ++ iter)
				{
					if ((*iter) == _where)
					{
						m_attributes.insert(iter, _attribute);
						break;
					}
				}
			}
		}

		void remove_first_attribute()
		{
			assert(m_attributes.size() > 0);
			m_attributes.front()->reset_parent();
			m_attributes.pop_front();
		}

		void remove_last_attribute()
		{
			assert(m_attributes.size() > 0);
			m_attributes.back()->reset_parent();
			m_attributes.pop_back();
		}

		void remove_attribute(std::shared_ptr<xml_attribute<Ch>> _where)
		{
			assert(_where && _where->parent() == shared_ptr_of_this());
			for(auto iter = m_attributes.begin(); iter != m_attributes.end(); ++iter)
			{
				if((*iter) == _where)
				{
					_where->reset_parent();
					m_attributes.erase(iter);
					break;
				}
			}
		}

		void remove_all_attributes()
		{
			for (auto attribute : m_attributes)
			{
				attribute->reset_parent();
			}
			m_attributes.clear();
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

		std::shared_ptr<xml_node<Ch>> shared_ptr_of_this() const
		{
			return std::static_pointer_cast<xml_node<Ch>>(std::const_pointer_cast<xml_base<Ch>>(xml_base<Ch>::shared_from_this()));
		}
	private:
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

		template < int Flags >
		void parse(const string<Ch>& _str)
		{
			clear();
			rapidxml::xml_document<Ch> doc;
			doc.template parse<Flags>((Ch *)_str.c_str());
			xml_node<Ch>::convert_from_rapidxml_node(&doc);
		}

		void clear()
		{
			xml_node<Ch>::remove_all_nodes();
			xml_node<Ch>::remove_all_attributes();
		}
	};
}
