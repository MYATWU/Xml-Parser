#include "xml.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include "Parser.h"

bool operator==(const Xml& lhs, const Xml& rhs)
{
    return (lhs.m_name == rhs.m_name && lhs.m_text == rhs.m_text && lhs.m_attr == rhs.m_attr && lhs.m_child == rhs.m_child);
}

bool operator!=(const Xml& lhs, const Xml& rhs)
{
    return !(lhs == rhs);
}

Xml::Xml() : m_name(nullptr), m_text(nullptr), m_attr(nullptr), m_child(nullptr)
{

}

Xml::Xml(const string& str) : m_text(nullptr), m_attr(nullptr), m_child(nullptr)
{
    m_name = new string(str);
}

Xml::Xml(const char* str) : Xml(string(str)) { }

Xml::Xml(const Xml& lhs)
{
    m_name = lhs.m_name;
    m_text = lhs.m_text;
    m_attr = lhs.m_attr;
    m_child = lhs.m_child;
}

Xml& Xml::operator=(const Xml& lhs)
{
    if (lhs != *this)
    {
        clear();
        m_name = lhs.m_name;
        m_text = lhs.m_text;
        m_attr = lhs.m_attr;
        m_child = lhs.m_child;
    }
    return *this;
}

void Xml::clear()
{
    if (m_name != nullptr)
    {
        delete m_name;
        m_name = nullptr;
    }
    if (m_text != nullptr)
    {
        delete m_text;
        m_text = nullptr;
    }
    if (m_attr != nullptr)
    {
        delete m_attr;
        m_attr = nullptr;
    }
    if (m_child != nullptr)
    {
        for (auto it = m_child->begin(); it != m_child->end(); ++it)
        {
            it->clear();
        }
        delete m_child;
        m_child = nullptr;
    }
}

string Xml::name() const
{
    if (m_name == nullptr)
    {
        return "";
    }
    return *m_name;
}

void Xml::name(const string& str)
{
    if (m_name != nullptr)
    {
        delete m_name;
        m_name = nullptr;
    }
    m_name = new string(str);
}

string Xml::text() const
{
    if (m_text == nullptr)
    {
        return "";
    }
    return *m_text;
}

bool Xml::text(const string& str)
{
    if (m_text != nullptr)
    {
        delete m_text;
        m_text = nullptr;
    }
    m_text = new string(str);
    if (!m_text)
    {
        std::cerr << "error in new m_text" << std::endl;
        return false;
    }
    return true;
}

Value& Xml::attr(const string& key)
{
    if (m_attr == nullptr)
    {
        m_attr = new std::map<string, Value>();
    }
    return (*m_attr)[key];
}

void Xml::attr(const string& key, const Value& value)
{
    if (m_attr == nullptr)
    {
        m_attr = new std::map<string, Value>();
    }
    (*m_attr)[key] = value;
}

string Xml::str() const
{
    if (m_name == nullptr)
    {
        return "";
    }
    std::ostringstream oss;
    oss << "<" << *m_name;
    if (m_attr != nullptr)
    {
        for (auto beg = m_attr->begin(); beg != m_attr->end(); ++beg)
        {
            oss << " " << beg->first << "=\"" << string(beg->second) << "\"";
        }
    }
    oss << ">";
    if (m_child != nullptr)
    {
        for (auto it = m_child->begin(); it != m_child->end(); ++it)
        {
            oss << it->str();
        }
    }
    if (m_text != nullptr)
    {
        oss << *m_text;
    }
    oss << "</" << *m_name << ">";
    return oss.str();
}

void Xml::append(const Xml& child)
{
    if (m_child == nullptr)
    {
        m_child = new std::list<Xml>();
    }
    m_child->push_back(child);
}

Xml& Xml::operator[](int index)
{
    if (m_child == nullptr)
    {
        throw std::runtime_error("no child");
    }
    auto it = m_child->begin();
    int sz = m_child->size();
    if (index < 0)
    {
        throw std::runtime_error("index less than zero");
    }
    if (index >= sz)
    {
        throw std::runtime_error("out of range");
    }
    for (int i = 0; i < index; ++i)
    {
        ++it;
    }
    return *it;
}

Xml& Xml::operator[](const char* name)
{
    return (*this)[string(name)];
}

Xml& Xml::operator[](const string& name)
{
    if (m_child == nullptr)
    {
        m_child = new std::list<Xml>();
    }
    for (auto& c : *m_child)
    {
        if (*c.m_name == name)
        {
            return c;
        }
    }
    m_child->push_back(Xml(name));
    return m_child->back();
}

void Xml::remove(int index)
{
    if (m_child == nullptr)
    {
        return ;
    }
    int sz = m_child->size();
    if (index < 0)
    {
        throw std::runtime_error("index less than zero");
    }
    if (index >= sz)
    {
        throw std::runtime_error("index out of range");
    }
    auto it = m_child->begin();
    for (int i = 0; i < index; ++i)
    {
        ++it;
    }
    it->clear();
    m_child->erase(it);
}

void Xml::remove(const char* name)
{
    remove(string(name));
}

void Xml::remove(const string& name)
{
    if (m_child == nullptr)
    {
        return ;
    }
    for (auto it = m_child->begin(); it != m_child->end();)
    {
        if (it->name() == name)
        {
            it->clear();
            it = m_child->erase(it);
        }
        else
        {
            ++it;
        }
    }
}

int Xml::size() const
{
    if (m_child == nullptr)
    {
        throw std::runtime_error("no child");
    }
    return m_child->size();
}

Xml::iterator Xml::begin()
{
    return m_child->begin();
}

Xml::iterator Xml::end()
{
    return m_child->end();
}

Xml::const_iterator Xml::begin() const
{
    return m_child->cbegin();
}

Xml::const_iterator Xml::end() const
{
    return m_child->cend();
}

Xml::iterator Xml::erase(iterator it)
{
    it->clear();
    return m_child->erase(it);
}

bool Xml::load(const string& filename)
{
    Parser p;
    if (!p.load_file(filename))
    {
        std::cerr << "error when Parser load file" << std::endl;
        return false;
    }
    *this = p.parse();
    return true;
}

bool Xml::save(const string& filename)
{
    std::ofstream ofile(filename);
    if (!ofile)
    {
        std::cerr << "Can not open output file" << std::endl;
        return false;
    }
    ofile << str();
    ofile.close();
    return true;
}

bool Xml::parse(const string& str)
{
    Parser p;
    if (!p.load_str(str))
    {
        std::cerr << "error when Parser load str" << std::endl;
        return false;
    }
    *this = p.parse();
    return true;
}

std::ostream& operator<<(std::ostream& os, const Xml& xml)
{
    os << xml.str();
    return os;
}