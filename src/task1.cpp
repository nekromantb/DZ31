#include "task1.h"
#include <iostream>
#include <string>

class Toy
{
    std::string name;
public:
    Toy(std::string inName)
    {
        name = inName;
    }
    std::string get_toy_name()
    {
        return name;
    }
};

class shared_ptr_toy_counter {
    Toy *obj;
    int links = 0;
public:
    int get_links() {
        return links;
    }

    void increase_links() {
        ++links;
    }

    void decrease_links() {
        --links;
    }

    Toy *get_obj() {
        return obj;
    }

    shared_ptr_toy_counter(std::string name) {
        obj = new Toy(name);
        links = 1;
    }
    ~shared_ptr_toy_counter()
    {
        std::cout << "toy " << obj->get_toy_name()<<" was deleted" <<std::endl;
        delete obj;
    }
};

class shared_ptr_toy
{
    shared_ptr_toy_counter* obj;

    shared_ptr_toy(std::string inName)
    {
        obj = new shared_ptr_toy_counter(inName);
    }

    shared_ptr_toy(Toy& inToy)
    {
        obj = new shared_ptr_toy_counter(inToy.get_toy_name());
    }
public:
    shared_ptr_toy(const shared_ptr_toy& inToy)
    {
        obj = inToy.obj;
        obj->increase_links();
    }

    shared_ptr_toy& operator= (const shared_ptr_toy& inToy)
    {
        if (this == &inToy)
            return *this;
        if (obj != nullptr) {
            obj->decrease_links();
            if (obj->get_links() == 0)
                delete obj;
        }
        obj = inToy.obj;
        obj->increase_links();
    }
    ~shared_ptr_toy()
    {
        obj->decrease_links();
        if (obj->get_links() == 0)
            delete obj;
        else obj = nullptr;
    }
    Toy* get_Toy()
    {
        return obj->get_obj();
    }
    friend shared_ptr_toy make_shared_ptr_toy (std::string name);
    friend shared_ptr_toy make_shared_ptr_toy (shared_ptr_toy inToy);
    friend shared_ptr_toy make_shared_ptr_toy (Toy& inToy);
};

shared_ptr_toy make_shared_ptr_toy(std::string name)
{
    return shared_ptr_toy(name);
}

shared_ptr_toy make_shared_ptr_toy(shared_ptr_toy inToy)
{
    return shared_ptr_toy(inToy);
}

shared_ptr_toy make_shared_ptr_toy(Toy& inToy)
{
    return shared_ptr_toy(inToy);
}

void task1() {
    std::cout<<"Rakamakafo!" <<std::endl;
    shared_ptr_toy toy = make_shared_ptr_toy("Rakamakafo");
    shared_ptr_toy toy2 = toy;
    Toy toy10("balalyka");
    shared_ptr_toy toy3 = make_shared_ptr_toy(toy10);
    shared_ptr_toy toy4(toy2);
    toy4 = toy3;
}