#pragma once
#include "rapidxml_adapter.hpp"

namespace rapidxml_adapter
{
    const int print_no_indenting = 0x1;   //!< Printer flag instructing the printer to suppress indenting of XML. See print() function.
    namespace internal
    {
        template<class OutIt, class Ch>
        inline OutIt copy_string(const string<Ch>& str, OutIt out)
        {
            for (auto ch : str)
            {
                *out++ = ch;
            }
            return out;
        }
        
        template<class OutIt, class Ch>
        inline OutIt copy_and_expand_string(const string<Ch>& str, Ch noexpand, OutIt out)
        {
            for (auto ch : str)
            {
                if (ch == noexpand)
                {
                    *out++ = ch;    // No expansion, copy character
                }
                else
                {
                    switch (ch)
                    {
                    case Ch('<'):
                        *out++ = Ch('&'); *out++ = Ch('l'); *out++ = Ch('t'); *out++ = Ch(';');
                        break;
                    case Ch('>'): 
                        *out++ = Ch('&'); *out++ = Ch('g'); *out++ = Ch('t'); *out++ = Ch(';');
                        break;
                    case Ch('\''): 
                        *out++ = Ch('&'); *out++ = Ch('a'); *out++ = Ch('p'); *out++ = Ch('o'); *out++ = Ch('s'); *out++ = Ch(';');
                        break;
                    case Ch('"'): 
                        *out++ = Ch('&'); *out++ = Ch('q'); *out++ = Ch('u'); *out++ = Ch('o'); *out++ = Ch('t'); *out++ = Ch(';');
                        break;
                    case Ch('&'): 
                        *out++ = Ch('&'); *out++ = Ch('a'); *out++ = Ch('m'); *out++ = Ch('p'); *out++ = Ch(';'); 
                        break;
                    default:
                        *out++ = ch;    // No expansion, copy character
                    }
                }
            }
            return out;
        }
        
        template<class OutIt, class Ch>
        inline OutIt fill_chars(OutIt out, int n, Ch ch)
        {
            for (int i = 0; i < n; ++i)
                *out++ = ch;
            return out;
        }

        template<class Ch, Ch ch>
        inline bool find_char(const string<Ch>& str)
        {
            return str.find(ch) != string<Ch>::npos;
        }

        template<class OutIt, class Ch>
        inline OutIt print_node(OutIt out, std::shared_ptr<xml_node<Ch>> node, int flags, int indent);
        
        // Print children of the node                               
        template<class OutIt, class Ch>
        inline OutIt print_children(OutIt out, std::shared_ptr<xml_node<Ch>> node, int flags, int indent)
        {
            for (auto child : node->children())
                out = print_node(out, child, flags, indent);
            return out;
        }

        // Print attributes of the node
        template<class OutIt, class Ch>
        inline OutIt print_attributes(OutIt out, std::shared_ptr<xml_node<Ch>> node, int flags)
        {
            for (auto attribute : node->attributes())
            {
                if (!attribute->name().empty() && !attribute->value().empty())
                {
                    // Print attribute name
                    *out = Ch(' '), ++out;
                    out = copy_string(attribute->name(), out);
                    *out = Ch('='), ++out;
                    // Print attribute value using appropriate quote type
                    string<Ch> value = attribute->value();
                    if (find_char<Ch, Ch('"')>(value))
                    {
                        *out = Ch('\''), ++out;
                        out = copy_and_expand_string(value, Ch('"'), out);
                        *out = Ch('\''), ++out;
                    }
                    else
                    {
                        *out = Ch('"'), ++out;
                        out = copy_and_expand_string(value, Ch('\''), out);
                        *out = Ch('"'), ++out;
                    }
                }
            }
            return out;
        }

        // Print data node
        template<class OutIt, class Ch>
        inline OutIt print_data_node(OutIt out, std::shared_ptr<xml_node<Ch>> node, int flags, int indent)
        {
            assert(node->type() == rapidxml::node_data);
            if (!(flags & print_no_indenting))
                out = fill_chars(out, indent, Ch('\t'));
            out = copy_and_expand_string(node->value(), Ch(0), out);
            return out;
        }

        // Print data node
        template<class OutIt, class Ch>
        inline OutIt print_cdata_node(OutIt out, std::shared_ptr<xml_node<Ch>> node, int flags, int indent)
        {
            assert(node->type() == rapidxml::node_cdata);
            if (!(flags & print_no_indenting))
                out = fill_chars(out, indent, Ch('\t'));
            *out = Ch('<'); ++out;
            *out = Ch('!'); ++out;
            *out = Ch('['); ++out;
            *out = Ch('C'); ++out;
            *out = Ch('D'); ++out;
            *out = Ch('A'); ++out;
            *out = Ch('T'); ++out;
            *out = Ch('A'); ++out;
            *out = Ch('['); ++out;
            out = copy_string(node->value(), out);
            *out = Ch(']'); ++out;
            *out = Ch(']'); ++out;
            *out = Ch('>'); ++out;
            return out;
        }

        // Print element node
        template<class OutIt, class Ch>
        inline OutIt print_element_node(OutIt out, std::shared_ptr<xml_node<Ch>> node, int flags, int indent)
        {
            assert(node->type() == rapidxml::node_element);

            // Print element name and attributes, if any
            if (!(flags & print_no_indenting))
                out = fill_chars(out, indent, Ch('\t'));
            *out = Ch('<'), ++out;
            out = copy_string(node->name(), out);
            out = print_attributes(out, node, flags);
            
            // If node is childless
            if (node->value().empty() && !node->first_node())
            {
                // Print childless node tag ending
                *out = Ch('/'), ++out;
                *out = Ch('>'), ++out;
            }
            else
            {
                // Print normal node tag ending
                *out = Ch('>'), ++out;

                // Test if node contains a single data node only (and no other nodes)
                std::shared_ptr<xml_node<Ch>> child = node->first_node();
                if (!child)
                {
                    // If node has no children, only print its value without indenting
                    out = copy_and_expand_string(node->value(), Ch(0), out);
                }
                else if (node->last_node() == node->first_node() && child->type() == rapidxml::node_data)
                {
                    // If node has a sole data child, only print its value without indenting
                    out = copy_and_expand_string(child->value(), Ch(0), out);
                }
                else
                {
                    // Print all children with full indenting
                    if (!(flags & print_no_indenting))
                        *out = Ch('\n'), ++out;
                    out = print_children(out, node, flags, indent + 1);
                    if (!(flags & print_no_indenting))
                        out = fill_chars(out, indent, Ch('\t'));
                }

                // Print node end
                *out = Ch('<'), ++out;
                *out = Ch('/'), ++out;
                out = copy_string(node->name(), out);
                *out = Ch('>'), ++out;
            }
            return out;
        }

        // Print declaration node
        template<class OutIt, class Ch>
        inline OutIt print_declaration_node(OutIt out, std::shared_ptr<xml_node<Ch>> node, int flags, int indent)
        {
            // Print declaration start
            if (!(flags & print_no_indenting))
                out = fill_chars(out, indent, Ch('\t'));
            *out = Ch('<'), ++out;
            *out = Ch('?'), ++out;
            *out = Ch('x'), ++out;
            *out = Ch('m'), ++out;
            *out = Ch('l'), ++out;

            // Print attributes
            out = print_attributes(out, node, flags);
            
            // Print declaration end
            *out = Ch('?'), ++out;
            *out = Ch('>'), ++out;
            
            return out;
        }

        // Print comment node
        template<class OutIt, class Ch>
        inline OutIt print_comment_node(OutIt out, std::shared_ptr<xml_node<Ch>> node, int flags, int indent)
        {
            assert(node->type() == rapidxml::node_comment);
            if (!(flags & print_no_indenting))
                out = fill_chars(out, indent, Ch('\t'));
            *out = Ch('<'), ++out;
            *out = Ch('!'), ++out;
            *out = Ch('-'), ++out;
            *out = Ch('-'), ++out;
            out = copy_string(node->value(), out);
            *out = Ch('-'), ++out;
            *out = Ch('-'), ++out;
            *out = Ch('>'), ++out;
            return out;
        }

        // Print doctype node
        template<class OutIt, class Ch>
        inline OutIt print_doctype_node(OutIt out, std::shared_ptr<xml_node<Ch>> node, int flags, int indent)
        {
            assert(node->type() == rapidxml::node_doctype);
            if (!(flags & print_no_indenting))
                out = fill_chars(out, indent, Ch('\t'));
            *out = Ch('<'), ++out;
            *out = Ch('!'), ++out;
            *out = Ch('D'), ++out;
            *out = Ch('O'), ++out;
            *out = Ch('C'), ++out;
            *out = Ch('T'), ++out;
            *out = Ch('Y'), ++out;
            *out = Ch('P'), ++out;
            *out = Ch('E'), ++out;
            *out = Ch(' '), ++out;
            out = copy_string(node->value(), out);
            *out = Ch('>'), ++out;
            return out;
        }

        // Print pi node
        template<class OutIt, class Ch>
        inline OutIt print_pi_node(OutIt out, std::shared_ptr<xml_node<Ch>> node, int flags, int indent)
        {
            assert(node->type() == rapidxml::node_pi);
            if (!(flags & print_no_indenting))
                out = fill_chars(out, indent, Ch('\t'));
            *out = Ch('<'), ++out;
            *out = Ch('?'), ++out;
            out = copy_string(node->name(), out);
            *out = Ch(' '), ++out;
            out = copy_string(node->value(), out);
            *out = Ch('?'), ++out;
            *out = Ch('>'), ++out;
            return out;
        }

        template<class OutIt, class Ch>
        inline OutIt print_node(OutIt out, std::shared_ptr<xml_node<Ch>> node, int flags, int indent)
        {
            // Print proper node type
            switch (node->type())
            {

            // Document
            case rapidxml::node_document:
                out = print_children(out, node, flags, indent);
                break;

            // Element
            case rapidxml::node_element:
                out = print_element_node(out, node, flags, indent);
                break;
            
            // Data
            case rapidxml::node_data:
                out = print_data_node(out, node, flags, indent);
                break;
            
            // CDATA
            case rapidxml::node_cdata:
                out = print_cdata_node(out, node, flags, indent);
                break;

            // Declaration
            case rapidxml::node_declaration:
                out = print_declaration_node(out, node, flags, indent);
                break;

            // Comment
            case rapidxml::node_comment:
                out = print_comment_node(out, node, flags, indent);
                break;
            
            // Doctype
            case rapidxml::node_doctype:
                out = print_doctype_node(out, node, flags, indent);
                break;

            // Pi
            case rapidxml::node_pi:
                out = print_pi_node(out, node, flags, indent);
                break;

                // Unknown
            default:
                assert(0);
                break;
            }
            
            // If indenting not disabled, add line break after node
            if (!(flags & print_no_indenting))
                *out = Ch('\n'), ++out;

            // Return modified iterator
            return out;
        }
    }
    

    template<class OutIt, class Ch> 
    inline OutIt print(OutIt out, std::shared_ptr<xml_node<Ch>> node, int flags = 0)
    {
        return internal::print_node(out, node, flags, 0);
    }

#ifndef RAPIDXML_NO_STREAMS
    template<class Ch> 
    inline std::basic_ostream<Ch> &print(std::basic_ostream<Ch> &out, std::shared_ptr<xml_node<Ch>> node, int flags = 0)
    {
        print(std::ostream_iterator<Ch>(out), node, flags);
        return out;
    }

    template<class Ch> 
    inline std::basic_ostream<Ch> &operator <<(std::basic_ostream<Ch> &out, std::shared_ptr<xml_node<Ch>> node)
    {
        return print(out, node);
    }

    template<class Ch> 
    inline std::basic_ostream<Ch> &operator <<(std::basic_ostream<Ch> &out, std::shared_ptr<xml_document<Ch>> doc)
    {
        return print(out, std::static_pointer_cast<xml_node<Ch>>(doc));
    }
#endif
}
