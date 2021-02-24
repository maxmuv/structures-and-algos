#include <iostream>

#include "mm.h"
#include <string>
#include <cassert>

const int ELEMENTS_COUNT = 1000000;

struct TestStruct
{
    std::string key;
    std::string value1;
    std::string value2;
};

typedef lab618::CMemoryManager<TestStruct> TestMemoryManger;

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

void TestMMFunction()
{
    TestMemoryManger TestMM(20, true);
    for (int i = 0; i < ELEMENTS_COUNT; i++)
    {
        TestStruct *ts = TestMM.newObject();
        generate(ts);
    }
    TestMM.clear();
}

int main()
{
    TestMMFunction();
}
