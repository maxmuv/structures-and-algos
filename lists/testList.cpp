#include <iostream>

#include "list.h"
#include <string>
#include <cassert>

const int ELEMENTS_COUNT = 1000000;

struct TestStruct
{
    std::string key;
    std::string value1;
    std::string value2;
};

typedef lab618::CSingleLinkedList<TestStruct> TestSingleList;
typedef lab618::CDualLinkedList<TestStruct> TestList;

static std::string makeRandomString(int minL = 7, int maxL = 14)
{
    int length = rand() % maxL + minL;
    std::string s;
    s.reserve(length);
    // сделать случайную строку
    for (int i = 0; i < length-3; i++)
    {
        char ch = rand()%26 + 'a';
        s += ch;
    }
    s += "tmp";
    return s;
}

static void generate(TestStruct *pts)
{
    pts->key = makeRandomString();
    pts->value1 = makeRandomString();
    pts->value2 = makeRandomString();
}

void TestListFunction()
{
    // тест односвязного списка

    TestSingleList single_list;
    for (int i = 0; i < ELEMENTS_COUNT; ++i)
    {
        TestStruct ts;
        generate(&ts);
        single_list.pushBack(ts);
    }

    assert(single_list.getSize() == ELEMENTS_COUNT);

    for (TestSingleList::CIterator it = single_list.begin(); it.isValid(); ++it)
    {
        it.getLeaf();
        TestStruct ts = *it;
        single_list.erase(it);
    }

    assert(single_list.getSize() == 0);

    for (int i = 0; i < ELEMENTS_COUNT; ++i)
    {
        TestStruct ts;
        generate(&ts);
        single_list.pushFront(ts);
    }

    assert(single_list.getSize() == ELEMENTS_COUNT);

    TestSingleList::CIterator it = single_list.begin();

    ++it;
    ++it;

    single_list.erase(it);

    int size = single_list.getSize();

    for (int i = 2; i<size; ++i){
        ++it;
        single_list.erase(it);
    }

    assert(single_list.getSize() == 2);

    single_list.clear();

    assert(single_list.getSize() == 0);

    /// тест двусвязного списка

    TestList list;
    for (int i = 0; i < ELEMENTS_COUNT; ++i)
    {
        TestStruct ts;
        generate(&ts);
        list.pushBack(ts);
    }

    assert(list.getSize() == ELEMENTS_COUNT);

    for (TestList::CIterator it = list.begin(); it.isValid(); ++it)
    {
        it.getLeaf();
        TestStruct ts = *it;
        list.erase(it);
    }

    assert(list.getSize() == 0);

    for (int i = 0; i < ELEMENTS_COUNT; ++i)
    {
        TestStruct ts;
        generate(&ts);
        list.pushFront(ts);
    }

    assert(list.getSize() == ELEMENTS_COUNT);

    for (TestList::CIterator it = list.end(); it != list.begin();)
    {
        --it;
        it.getLeaf();
        TestStruct ts = *it;
        list.eraseAndNext(it);
    }

    assert(list.getSize() == 0);

    for (int i = 0; i < ELEMENTS_COUNT; ++i)
    {
        TestStruct ts;
        generate(&ts);
        list.pushFront(ts);
    }

    assert(list.getSize() == ELEMENTS_COUNT);

    TestList::CIterator itList = list.begin();

    ++itList;
    ++itList;
    ++itList;


    list.erase(itList);
    list.eraseAndNext(itList);

    assert(list.getSize() == ELEMENTS_COUNT - 2);


    list.clear();

    assert(list.getSize() == 0);

    for (int i = 0; i < ELEMENTS_COUNT; ++i)
    {
        TestStruct ts;
        generate(&ts);
        list.pushBack(ts);
    }

    assert(list.getSize() == ELEMENTS_COUNT);

    for (int i = 0; i < ELEMENTS_COUNT; ++i)
    {
        list.popFront();
    }

    assert(list.getSize() == 0);

    for (int i = 0; i < ELEMENTS_COUNT; ++i)
    {
        TestStruct ts;
        generate(&ts);
        list.pushBack(ts);
    }

    assert(list.getSize() == ELEMENTS_COUNT);

    for (int i = 0; i < ELEMENTS_COUNT; ++i)
    {
        list.popBack();
    }

    assert(list.getSize() == 0);

}

int main()
{
    TestListFunction();
}
