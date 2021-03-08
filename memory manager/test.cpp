#include <iostream>

#include <cassert>
#include <string>
#include "mm.h"

const int ELEMENTS_COUNT = 100;

struct TestStruct {
  std::string key;
  std::string value1;
  std::string value2;
  TestStruct *next;
};

struct TestListStruct {
  TestStruct *p_head;
  TestStruct *p_end;
};

typedef lab618::CMemoryManager<TestStruct> TestMemoryManager;

static std::string makeRandomString(int minL = 7, int maxL = 14) {
  int length = rand() % maxL + minL;
  std::string s;
  s.reserve(length);
  // сделать случайную строку
  for (int i = 0; i < length - 3; i++) {
    char ch = rand() % 26 + 'a';
    s += ch;
  }
  s += "tmp";
  return s;
}

static void generate(TestStruct *pts) {
  pts->key = makeRandomString();
  pts->value1 = makeRandomString();
  pts->value2 = makeRandomString();
  pts->next = 0;
}

void TestMMFunction() {
  {  // generate and clear
    TestMemoryManager TestTrueMM(20, true);
    for (int i = 0; i < ELEMENTS_COUNT; i++) {
      TestStruct *ts = TestTrueMM.newObject();
      generate(ts);
    }
    TestTrueMM.clear();
  }

  {  // manager with 1-element blocks, generate and delete all elements
    TestListStruct TestL = {0, 0};
    TestMemoryManager TestFalseMM(1, false);
    TestStruct *p_teststruct = TestFalseMM.newObject();
    generate(p_teststruct);
    TestL.p_head = p_teststruct;
    TestL.p_end = p_teststruct;
    for (int i = 1; i < ELEMENTS_COUNT; i++) {
      TestL.p_end->next = TestFalseMM.newObject();
      TestL.p_end = TestL.p_end->next;
      generate(TestL.p_end);
    }
    p_teststruct = TestL.p_head;
    TestStruct *p_tmpnext = p_teststruct;
    for (int j = 0; j < ELEMENTS_COUNT; j++) {
      p_tmpnext = p_teststruct->next;
      TestFalseMM.deleteObject(p_teststruct);
      p_teststruct = p_tmpnext;
    }
    TestFalseMM.clear();
  }
  {  // generate and delete part of elements(in debug mode)
    TestListStruct TestL = {0, 0};
    TestMemoryManager TestTrueMM2(10, true);
    TestStruct *p_teststruct = TestTrueMM2.newObject();
    generate(p_teststruct);
    TestL.p_head = p_teststruct;
    TestL.p_end = p_teststruct;
    for (int i = 1; i < ELEMENTS_COUNT; i++) {
      TestL.p_end->next = TestTrueMM2.newObject();
      TestL.p_end = TestL.p_end->next;
      generate(TestL.p_end);
    }
    p_teststruct = TestL.p_head;
    for (int i = 0; i < 19; i++) {
      p_teststruct = p_teststruct->next;
    }
    TestStruct *p_tmpnext = p_teststruct->next;
    for (int i = 20; i < ELEMENTS_COUNT; i++) {
      p_tmpnext = p_teststruct->next;
      assert(TestTrueMM2.deleteObject(p_teststruct) == true);
      p_teststruct = p_tmpnext;
    }
    TestTrueMM2.clear();
  }
  {  // delete every third element
    TestListStruct TestL2 = {0, 0};
    TestMemoryManager TestFalseMM2(3, false);
    TestStruct *p_teststruct = TestFalseMM2.newObject();
    generate(p_teststruct);
    TestL2.p_head = p_teststruct;
    TestL2.p_end = p_teststruct;
    for (int i = 1; i < ELEMENTS_COUNT; i++) {
      TestL2.p_end->next = TestFalseMM2.newObject();
      TestL2.p_end = TestL2.p_end->next;
      generate(TestL2.p_end);
    }
    p_teststruct = TestL2.p_head;
    TestStruct *p_del = p_teststruct;
    for (int j = 0; j < ELEMENTS_COUNT / 3; j++) {
      p_teststruct = p_teststruct->next;
      p_del = p_teststruct->next;
      p_teststruct->next = p_del->next;
      TestFalseMM2.deleteObject(p_del);
      p_teststruct = p_teststruct->next;
    }
    p_teststruct = TestL2.p_head;
    p_del = p_teststruct;
    for (int j = 0; j < ELEMENTS_COUNT - ELEMENTS_COUNT / 3; j++) {
      p_teststruct = p_del->next;
      TestFalseMM2.deleteObject(p_del);
      p_del = p_teststruct;
    }
    TestFalseMM2.clear();
  }
  {  // delete every second block and create blocks again
    TestListStruct TestL3{0, 0};
    TestMemoryManager TestFalseMM3(5, false);
    TestStruct *p_teststruct = TestFalseMM3.newObject();
    generate(p_teststruct);
    TestL3.p_head = p_teststruct;
    TestL3.p_end = p_teststruct;
    for (int i = 1; i < ELEMENTS_COUNT; i++) {
      TestL3.p_end->next = TestFalseMM3.newObject();
      TestL3.p_end = TestL3.p_end->next;
      generate(TestL3.p_end);
    }
    TestStruct *p_testnextstruct = p_teststruct;
    bool j = 0;
    while (p_teststruct->next != 0) {
      for (int y = 0; y < 5; y++) {
        if (j == 0) {
          p_teststruct = p_testnextstruct;
          p_testnextstruct = p_testnextstruct->next;
        } else {
          TestStruct *p_tmpstruct = p_testnextstruct;
          p_testnextstruct = p_testnextstruct->next;
          TestFalseMM3.deleteObject(p_tmpstruct);
        }
      }
      if (j == 1) p_teststruct->next = p_testnextstruct;
      j = !j;
    }
    for (int i = 0; i < ELEMENTS_COUNT / 2; i++) {
      p_teststruct->next = TestFalseMM3.newObject();
      p_teststruct = p_teststruct->next;
      generate(p_teststruct);
    }
    p_teststruct = TestL3.p_head;
    TestStruct *p_del = p_teststruct;
    for (int j = 0; j < ELEMENTS_COUNT; j++) {
      p_teststruct = p_del->next;
      TestFalseMM3.deleteObject(p_del);
      p_del = p_teststruct;
    }
    TestFalseMM3.clear();
  }
  {  // delete every second element in blocks, then create them again
    TestListStruct TestL4{0, 0};
    TestMemoryManager TestTrueMM3(100, true);
    TestStruct *p_teststruct = TestTrueMM3.newObject();
    generate(p_teststruct);
    TestL4.p_head = p_teststruct;
    TestL4.p_end = p_teststruct;
    for (int i = 1; i < ELEMENTS_COUNT; i++) {
      TestL4.p_end->next = TestTrueMM3.newObject();
      TestL4.p_end = TestL4.p_end->next;
      generate(TestL4.p_end);
    }
    TestStruct **second_pointers = new TestStruct *[ELEMENTS_COUNT / 100];
    for (int j = 0; j < ELEMENTS_COUNT / 100; j++) {
      p_teststruct = p_teststruct->next;
      second_pointers[j] = p_teststruct;
      TestStruct *tmp_pointer = p_teststruct;
      p_teststruct = p_teststruct->next;
      if (!TestTrueMM3.deleteObject(tmp_pointer)) std::cout << "Error";
      for (int y = 2; y < 100; y++) p_teststruct = p_teststruct->next;
    }
    for (int z = 0; z < ELEMENTS_COUNT / 100; z++)
      assert(second_pointers[z] == TestTrueMM3.newObject());
    TestTrueMM3.clear();
  }
  {  // constructor
    TestMemoryManager TestTrueMM{ELEMENTS_COUNT, false};
    TestTrueMM.clear();
    TestMemoryManager TestFalseMM{ELEMENTS_COUNT, true};
    TestFalseMM.clear();
  }
}

int main() { TestMMFunction(); }
