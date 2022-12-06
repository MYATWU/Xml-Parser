#pragma once

#include "Value.h"
#include <string>
#include <map>
#include <list>
#include <iostream>

class Xml
{
friend bool operator==(const Xml& lhs, const Xml& rhs);
friend bool operator!=(const Xml& lhs, const Xml& rhs);
friend std::ostream& operator<<(std::ostream& os, const Xml& xml);
public:
    Xml();
    Xml(const string& str);
    Xml(const char* str);
    Xml(const Xml& lhs);

    Xml& operator=(const Xml& lhs);

    void clear();

    int size() const;

    string name() const;
    void name(const string& str);

    string text() const;
    bool text(const string& str);

    Value& attr(const string& key);
    void attr(const string& key, const Value& value);

    string str() const;

    void append(const Xml& child);
    Xml& operator[](int index);
    Xml& operator[](const char* name);
    Xml& operator[](const string& name);
    void remove(int index);
    void remove(const char* name);
    void remove(const string& name);

    using iterator = std::list<Xml>::iterator;
    using const_iterator = std::list<Xml>::const_iterator;
    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

    iterator erase(iterator it);

    bool load(const string& filename);
    bool save(const string& filename);
    bool parse(const string& str);

private:  
    string *m_name;                      //结点名称
    string *m_text;                      //结点内容
    std::map<string, Value> *m_attr;     //结点属性
    std::list<Xml> *m_child;             //子节点链表
};

//为了使友元函数对类的用户可见，在此进行声明
bool operator==(const Xml& lhs, const Xml& rhs);
bool operator!=(const Xml& lhs, const Xml& rhs);
std::ostream& operator<<(std::ostream& os, const Xml& xml);