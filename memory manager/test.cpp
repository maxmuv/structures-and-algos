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
    TestStruct *next;
};

struct TestListStruct
{
    TestStruct *p_head;
    TestStruct *p_end;
};

typedef lab618::CMemoryManager<TestStruct> TestMemoryManager;

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
    pts->next = 0;
}

void TestMMFunction()
{
    TestListStruct TestL;
    TestMemoryManager TestTrueMM(20, true);
    for (int i = 0; i < ELEMENTS_COUNT; i++)
    {
        TestStruct *ts = TestTrueMM.newObject();
        generate(ts);
    }
    TestTrueMM.clear();
    TestMemoryManager TestFalseMM(1, false);
    TestStruct *p_teststruct = TestFalseMM.newObject();
    generate(p_teststruct);
    TestL.p_head = p_teststruct;
    TestL.p_end = p_teststruct;
    for (int i = 1; i < ELEMENTS_COUNT; i++){
        TestL.p_end->next = TestFalseMM.newObject();
        TestL.p_end = TestL.p_end->next;
        generate(TestL.p_end);
    }
    p_teststruct = TestL.p_head;
    TestStruct *p_tmpnext;
    for (int i = 0; i < ELEMENTS_COUNT; i++) {
        p_tmpnext = p_teststruct->next;
        assert(TestFalseMM.deleteObject(p_teststruct) == true);
        p_teststruct = p_tmpnext;
    }
    TestFalseMM.clear();
    TestMemoryManager TestTrueMM2(10, true);
    p_teststruct = TestTrueMM2.newObject();
    generate(p_teststruct);
    TestL.p_head = p_teststruct;
    TestL.p_end = p_teststruct;
    for (int i = 1; i < ELEMENTS_COUNT; i++){
        TestL.p_end->next = TestTrueMM2.newObject();
        TestL.p_end = TestL.p_end->next;
        generate(TestL.p_end);
    }
    p_teststruct = TestL.p_head;
    for (int i = 0; i < 19; i++) {
        p_teststruct = p_teststruct->next;
    }
    for (int i = 20; i < ELEMENTS_COUNT; i++) {
        p_tmpnext = p_teststruct->next;
        assert(TestTrueMM2.deleteObject(p_teststruct) == true);
        p_teststruct = p_tmpnext;
    }
    TestTrueMM2.clear();
}

int main()
{
    TestMMFunction();
}
