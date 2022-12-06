#include <iostream>
#include "xml.h"
#include "Parser.h"
using namespace std;

int main()
{
    Xml root;
    root.load("./test.xml");

    const string & name = root["student"]["name"].text();
    std::cout << "name=" << name << std::endl;

    int id = root["student"].attr("id");
    std::cout << "id=" << id << std::endl;

    const string & age = root["student"]["age"].text();
    std::cout << "age=" << age << std::endl;

    const string & name1 = root[1]["name"].text();
    std::cout << "name1 = " << name1 << std::endl;

    int id1 = root[1].attr("id");
    std::cout << "id1=" << id1 << std::endl;

    const string & age1 = root[1]["age"].text();
    std::cout << "age1=" << age1 << std::endl;

    for (Xml::iterator it = root.begin(); it != root.end(); it++)
    {
        std::cout << *it << std::endl;
    }

    Xml mystudent("student");
    mystudent.attr("id", 7);
    mystudent.attr("class", "7");
    Xml mname;
    mname.name("name");
    mname.text("mayue");
    Xml mage("age");
    mage.text("25");
    Xml mgender("gender");
    mgender.text("male");
    mystudent["name"] = mname;
    mystudent.append(mage);
    mystudent.append(mgender);
    root.append(mystudent);
    root.save(".\\output.xml");

    return 0;
}