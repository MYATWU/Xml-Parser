#pragma once
#include <string>
#include "Xml.h"

class Parser
{
public:
    Parser() : m_str(nullptr), m_idx(0) { }
    ~Parser();

    bool load_file(const std::string& filename);
    bool load_str(const std::string& xmlstr);
    Xml parse();
private:
    bool parse_decl();
    Xml parse_node();
    string parse_node_name();
    string parse_attrs_key();
    string parse_attrs_val();
    string parse_text();
    bool parse_comment();
    bool cha_in(const string& target) const;

private:
    std::string *m_str;
    int m_idx;
};