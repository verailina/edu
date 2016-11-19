#include "shared_ptr.h"
#include <iostream>
#include <assert.h>
#include <string>

using smart_pointers::shared_ptr;
using smart_pointers::DefaultDeleter;
using smart_pointers::ArrayDeleter;

void ConstructorsTest()
{
    shared_ptr<int> p1;
    assert(p1.use_count() == 0);
    shared_ptr<int> p2(nullptr);
    assert(p2.use_count() == 0);
    shared_ptr<int> p3(new int(10));
    assert(p3.use_count() == 1);
    shared_ptr<int, DefaultDeleter<int>> p4(new int(1000), DefaultDeleter<int>());
    assert(p4.use_count() == 1);
    shared_ptr<int, ArrayDeleter<int>> p5(new int[3]{1, 2, 3}, ArrayDeleter<int>());
    assert(p5.use_count() == 1);
}

void DestructorTest()
{
    shared_ptr<int> p1(new int(5));
    for (int i = 0; i < 3; ++i)
    {
        assert(p1.use_count() == 1);
        shared_ptr<int> p2 = p1;
    }
    assert(p1.use_count() == 1);
}

void AssigmentTest()
{
    shared_ptr<int> p1(new int(5));
    assert(p1.use_count() == 1);
    shared_ptr<int> p2;
    p2 = p1;
    assert(p1.use_count() == 2);
    shared_ptr<int> p3 = p2;
    assert(p1.use_count() == 3);
    assert(p2.use_count() == 3);
    p2.reset();
    assert(p1.use_count() == 2);
    assert(p2.use_count() == 0);
    assert(p3.use_count() == 2);
}

void SwapTest()
{
    shared_ptr<int> p1(new int(5));
    shared_ptr<int> p2 = p1;
    shared_ptr<int> p3 = p2;

    shared_ptr<int> q1(new int(15));
    shared_ptr<int> q2 = q1; 
    
    p1.swap(p2);
    assert(p1.use_count() == 3);
    assert(p2.use_count() == 3);
    assert(p3.use_count() == 3);
    assert(*p1 == 5);

    p1.swap(q1);

    assert(*q1 == 5);
    assert(q1.use_count() == 3);

    assert(*p1 == 15);
    assert(p1.use_count() == 2);   
}

void ResetTest()
{
    shared_ptr<int> p1;
    p1.reset();
    assert(p1.use_count() == 0);

    shared_ptr<int> p2(new int(5));
    p2.reset();
    assert(p2.use_count() == 0);
    assert(!(p2));

    shared_ptr<int> p3(new int(5));
    shared_ptr<int> p4 = p3;
    p3.reset();
    assert(p4.use_count() == 1);
}

void AccessTest()
{
    shared_ptr<int> p1(new int(15));
    std::string *test_string = new std::string("Hello");
    shared_ptr<std::string> p2(test_string);
    assert(*p1 == 15);

    assert(p2->length() == 5);
    assert(p2.get() == test_string);

    shared_ptr<int> p3;
    assert(p3.get() == nullptr);    
}

void CounterAccesTest()
{
    shared_ptr<int> p1(new int(15));
    assert(p1.use_count() == 1);
    assert(p1.unique() == true);
    assert((p1));

    shared_ptr<int> p2 = p1;
    assert(p2.use_count() == 2);
    assert(p2.unique() == false);
    assert((p2));

    shared_ptr<int> p3;
    assert(p3.use_count() == 0);
    assert(p3.unique() == false);
    assert(!(p3));

}

int main()
{
    ConstructorsTest();
    DestructorTest();

    AssigmentTest();
    SwapTest();
    ResetTest();

    AccessTest();
    CounterAccesTest();

    return 0;
}