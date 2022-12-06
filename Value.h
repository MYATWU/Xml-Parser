#pragma once
#include <string>
using std::string;

class Value
{
friend bool operator==(const Value& lhs, const Value& rhs);
friend bool operator!=(const Value& lhs, const Value& rhs);
public:
    Value() = default;
    Value(int value);
    Value(double value);
    Value(bool value);
    Value(const string& value);
    Value(const char* value);

    Value& operator=(int value);
    Value& operator=(double value);
    Value& operator=(bool value);
    Value& operator=(const string& value);
    Value& operator=(const char* value);

    operator int() const;
    operator double() const;
    operator bool() const;
    operator string() const;

private:
    string m_value;
};

bool operator==(const Value& lhs, const Value& rhs);
bool operator!=(const Value& lhs, const Value& rhs);