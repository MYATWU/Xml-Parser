#include "Parser.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cctype>

string blanks(" \t\n\r\f\v");
string name_symbols("_-.:");

Parser::~Parser()
{
    if (m_str)
    {
        delete m_str;
        m_str = nullptr;
    }
}

bool Parser::load_file(const std::string& filename)
{
    std::ifstream ifile(filename);
    if (!ifile)
    {
        std::cerr << "Can not open target file" << std::endl;
        return false;        
    }
    if (m_str == nullptr)
    {
        m_str = new std::string();
    }
    std::ostringstream oss;
    oss << ifile.rdbuf();
    *m_str = oss.str();
    m_idx = 0;
    return true;
}

bool Parser::load_str(const std::string& xmlstr)
{
    if (m_str == nullptr)
    {
        m_str = new std::string();
    }
    *m_str = xmlstr;
    m_idx = 0;
    return true;
}

Xml Parser::parse()
{
    auto pos = m_str->find_first_not_of(blanks, m_idx);
    if (pos == std::string::npos)
    {
        throw std::runtime_error("empty file");
    }
    m_idx = pos;
    if (!parse_decl())
    {
        throw std::runtime_error("parsing declaration error");
    }
    if (m_str->compare(m_idx, 4, "<!--") == 0)
    {
        if (!parse_comment())
        {
            throw std::runtime_error("parsing comment error");
        }
    }
    return parse_node();
}

bool Parser::parse_decl()
{
    if (m_str->compare(m_idx, 5, "<?xml") != 0)
    {
        std::cerr << "invalid root begin declaration" << std::endl;
        return false;
    }
    m_idx += 5;
    auto pos = m_str->find("?>",m_idx);
    if (pos == std::string::npos)
    {
        std::cerr << "invalid root end declaration" << std::endl;
        return false;
    }
    m_idx = pos + 2;
    return true;
}

Xml Parser::parse_node()
{
    Xml res;
    auto pos = m_str->find_first_not_of(blanks, m_idx);
    if (pos == std::string::npos)
    {
        //空白文件，返回默认初始化的Xml对象
        return res;
    }
    m_idx = pos;
    auto sz = m_str->size();
    //解析节点名称
    res.name(parse_node_name());
    while (m_idx < sz)
    {
        m_idx = m_str->find_first_not_of(blanks, m_idx);
        if ((*m_str)[m_idx] == '<')
        {
            if ((*m_str)[m_idx + 1] == '/')  //节点结束
            {
                string end_tag = "</" + res.name() + ">";
                auto pos = m_str->find(end_tag, m_idx);
                if (pos == std::string::npos)
                {
                    throw std::runtime_error("can not find correct end tag");
                }
                m_idx += end_tag.size() - 1;
                break;
            }
            else if (m_str->compare(m_idx, 4, "<!--") == 0)  //注释
            {
                if (!parse_comment())
                {
                    throw std::runtime_error("parsing comment error");
                }
            }
            else
            {
                //'<'后是正文，说明是子节点
                res.append(parse_node());
            }
        }
        else if ((*m_str)[m_idx] == '>')  //name声明结束，解析text
        {
            m_idx++;
            res.text(parse_text());
        }
        else if (m_str->compare(m_idx, 2, "/>") == 0) //单节点
        {
            m_idx += 2;
            break;
        }
        else  //name属性解析
        {
            string key = parse_attrs_key();
            m_idx = m_str->find_first_not_of(blanks, m_idx);
            string val = parse_attrs_val();
            res.attr(key, val);
        }
    }
    return res;
}

std::string Parser::parse_node_name()
{
    string res;
    if ((*m_str)[m_idx] != '<')
    {
        throw std::runtime_error("invalid node tag declaration");
    }
    m_idx += 1;
    if (!isalpha((*m_str)[m_idx]) && (*m_str)[m_idx] != '_')
    {
        throw std::runtime_error("invalid first character of node name declaration");
    }
    res.push_back((*m_str)[m_idx]);
    m_idx++;
    while (isalnum((*m_str)[m_idx]) || cha_in(name_symbols))
    {
        res.push_back((*m_str)[m_idx]);
        m_idx++;
    }
    if ((*m_str)[m_idx] != '>' && (*m_str)[m_idx] != '/' && !isspace((*m_str)[m_idx]))
    {
        throw std::runtime_error("invalid begin tag declaration");
    }
    return res;

}

string Parser::parse_attrs_key()
{
    string res;
    auto equal_pos = m_str->find('=',m_idx);
    auto blank_pos = m_str->find_first_of(blanks, m_idx);
    if (equal_pos == std::string::npos)
    {
        throw std::runtime_error("invalid attributes declaration, no \"=\"");
    }
    //如果在=之前有空格
    if (blank_pos < equal_pos)
    {
        if (m_str->find_first_not_of(blanks, blank_pos) != equal_pos)\
        {
            throw std::runtime_error("invalid attributes declaration, unexpected blankspace");
        }
        res = m_str->substr(m_idx, blank_pos - m_idx);
    }
    else
        res = m_str->substr(m_idx, equal_pos - m_idx);
    m_idx = equal_pos + 1;
    return res;
}

string Parser::parse_attrs_val()
{
    string res;
    if ((*m_str)[m_idx] != '"')
    {
        throw std::runtime_error("invalid attributes val declaration");
    }
    m_idx++;
    while ((*m_str)[m_idx] != '"')
    {
        res.push_back((*m_str)[m_idx]);
        m_idx++;
    }
    m_idx++;
    return res;
}

string Parser::parse_text()
{
    string res;
    auto pos1 = m_str->find('<',m_idx);  //找到节点end tag
    auto pos2 = m_str->find_first_not_of(blanks, m_idx);  //找到节点begin tag后第一个非空字符
    res = m_str->substr(pos2, pos1 - pos2);  //text内容，含尾部空白符
    auto i = res.find_last_not_of(blanks) + 1; //找到text最后一个非空字符后一个位置
    res = res.substr(0, i); //text最终内容
    m_idx = pos1;
    return res;
}

bool Parser::parse_comment()
{
    if (m_str->compare(m_idx, 4, "<!--") != 0)
    {
        std::cerr << "parsing comment error1" << std::endl;
        return false;
    }
    auto pos = m_str->find("-->", m_idx + 4);
    if (pos == std::string::npos)
    {
        std::cerr << "Can not find end of comment" << std::endl;
        return false;
    }
    m_idx = pos + 3;
    return true;
}

bool Parser::cha_in(const string& target) const
{
    if (target.find((*m_str)[m_idx]) == std::string::npos)
        return false;
    else
        return true;
}