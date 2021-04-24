#include <iostream>

#include <cassert>
#include <cstring>
#include <string>
#include "avltree.h"
#include "sort.h"

const int ELEMENTS_COUNT = 80000;

struct TestStruct {
  std::wstring key;
  std::wstring value1;
  std::wstring value2;
};

const wchar_t ALPHABET[] =
    L"01234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZабвгдеёжзи"
    L"йклмнопрстуфхцчшщьъэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЬЪЭЮЯ";
const int LENGTH = sizeof(ALPHABET) / sizeof(wchar_t);

static std::wstring makeRandomString(int minL = 7, int maxL = 14) {
  setlocale(LC_ALL, "Russian");
  int length = rand() % maxL + minL;
  std::wstring s;
  s.reserve(length);
  // сделать случайную строку
  wchar_t ch;
  for (int i = 0; i < length; i++) {
    int choose = rand() % (LENGTH - 1);
    ch = ALPHABET[choose];
    s += ch;
  }
  return s;
}

static void generate(TestStruct* pts) {
  pts->key = makeRandomString();
  pts->value1 = makeRandomString();
  pts->value2 = makeRandomString();
}

int Compare(const TestStruct* el1, const TestStruct* el2) {
  return wcscmp(el2->key.c_str(), el1->key.c_str());
}

int InvCompare(const TestStruct* el1, const TestStruct* el2) {
  return -wcscmp(el2->key.c_str(), el1->key.c_str());
}

void TestTreeFunction() {
  lab618::CAVLTree<TestStruct, Compare> tree;
  TestStruct testArrayElements[ELEMENTS_COUNT];
  TestStruct* testArrayPointer[ELEMENTS_COUNT];
  for (int i = 0; i < ELEMENTS_COUNT; i++) {
    testArrayPointer[i] = &(testArrayElements[i]);
    generate(testArrayPointer[i]);
  }
  for (int i = 0; i < ELEMENTS_COUNT; i++) {
    tree.add(testArrayPointer[i]);
    tree.correctTree(tree.m_pRoot);
    assert(true == tree.update(testArrayPointer[i]));
    assert(tree.find(*testArrayPointer[i]) == testArrayPointer[i]);
  }
  tree.clear();
  for (int i = 0; i < ELEMENTS_COUNT; i++) {
    tree.update(testArrayPointer[i]);
  }
  for (int i = 0; i < ELEMENTS_COUNT; i++) {
    tree.remove(*testArrayPointer[i]);
    tree.correctTree(tree.m_pRoot);
  }
  assert(nullptr == tree.m_pRoot);
  templates::mergeSort<TestStruct>(testArrayPointer, ELEMENTS_COUNT, Compare);
  for (int i = 0; i < ELEMENTS_COUNT - 1; i++)
    assert(Compare(testArrayPointer[i], testArrayPointer[i + 1]) >= 0);

  for (int i = 0; i < ELEMENTS_COUNT; i++) {
    tree.add(testArrayPointer[i]);
    tree.correctTree(tree.m_pRoot);
    assert(true == tree.update(testArrayPointer[i]));
    assert(tree.find(*testArrayPointer[i]) == testArrayPointer[i]);
  }
  tree.clear();
  for (int i = 0; i < ELEMENTS_COUNT; i++) {
    tree.update(testArrayPointer[i]);
  }
  for (int i = 0; i < ELEMENTS_COUNT; i++) {
    tree.remove(*testArrayPointer[i]);
    tree.correctTree(tree.m_pRoot);
  }
  assert(nullptr == tree.m_pRoot);
  for (int i = ELEMENTS_COUNT - 1; i > -1; i--) {
    tree.add(testArrayPointer[i]);
    tree.correctTree(tree.m_pRoot);
    assert(true == tree.update(testArrayPointer[i]));
    assert(tree.find(*testArrayPointer[i]) == testArrayPointer[i]);
  }
  tree.clear();
  for (int i = ELEMENTS_COUNT - 1; i > -1; i--) {
    tree.update(testArrayPointer[i]);
  }
  for (int i = ELEMENTS_COUNT - 1; i > -1; i--) {
    tree.remove(*testArrayPointer[i]);
    tree.correctTree(tree.m_pRoot);
  }
  assert(nullptr == tree.m_pRoot);
}

int main() { TestTreeFunction(); }
