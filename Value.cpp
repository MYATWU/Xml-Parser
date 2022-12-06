#include "Value.h"
#include <sstream>
using std::stringstream;

bool operator==(const Value& lhs, const Value& rhs)
{
    return lhs.m_value == rhs.m_value;
}
bool operator!=(const Value& lhs, const Value& rhs)
{
    return !(lhs == rhs);
}

Value::Value(int value)
{
    *this = value;
}

Value::Value(double value)
{
    *this = value;
}

Value::Value(bool value)
{
    *this = value;
}

Value::Value(const string& value)
{
    *this = value;
}

Value::Value(const char* value)
{
    *this = value;
}

Value& Value::operator=(int value)
{
    stringstream ss;
    ss << value;
    m_value = ss.str();
    return *this;
}

Value& Value::operator=(double value)
{
    stringstream ss;
    ss << value;
    m_value = ss.str();
    return *this;
}

Value& Value::operator=(bool value)
{
    m_value = value ? "true" : "false";
    return *this;
}

Value& Value::operator=(const string& value)
{
    m_value = value;
    return *this;
}

Value& Value::operator=(const char* value)
{
    m_value = value;
    return *this;
}

Value::operator int() const
{
    return std::atoi(m_value.c_str());
}

Value::operator double() const
{
    return std::atof(m_value.c_str());
}

Value::operator bool() const
{
    if (m_value == "true")
    {
        return true;
    }
    else
    {
        return false;
    }
    return false;
}

Value::operator string() const
{
    return m_value;
}
