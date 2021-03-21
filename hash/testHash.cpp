#include <iostream>

#include <cassert>
#include <cstring>
#include <string>
//#include "hash.h"

const int ELEMENTS_COUNT = 1000;

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
  srand(time(NULL));
  int length = rand() % maxL + minL;
  std::wstring s;
  s.reserve(length);
  // сделать случайную строку
  wchar_t ch;
  srand(time(NULL));
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

unsigned int HashFunc(const TestStruct* pElement) {
  size_t size = pElement->key.size();
  unsigned int hash = 0;
  for (size_t i = 0; i < size; i++) {
    unsigned int el = 0;
    for (size_t j = 0; j < LENGTH; j++) {
      el = static_cast<unsigned int>(pElement->key[i]);
    }
    for (size_t j = 0; j < i; j++) {
      el = el * LENGTH;
    }
    hash += el;
    hash = hash;
  }
  return hash;
}

void TestSortFunction() {
  TestStruct ts;
  generate(&ts);
  unsigned int hash = HashFunc(&ts);
  std::cout << hash / 80;
}

int main() { TestSortFunction(); }
