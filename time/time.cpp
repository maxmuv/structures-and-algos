#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

#include "timer.h"

#include "avltree/avltree.h"
#include "hash/hash.h"
#include "sort/sort.h"

struct TestStruct {
  std::wstring key1;
  std::wstring key2;
  std::wstring value1;
};

const wchar_t ALPHABET[] =
    L"01234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZабвгдеёжзи"
    L"йклмнопрстуфхцчшщьъэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЬЪЭЮЯ";
const int LENGTH = sizeof(ALPHABET) / sizeof(wchar_t);

static std::wstring makeRandomString(int minL = 7, int maxL = 20) {
  setlocale(LC_ALL, "Russian");
  int length = rand() % maxL + minL;
  std::wstring s;
  s.reserve(length);
  wchar_t ch;
  for (int i = 0; i < length; i++) {
    int choose = rand() % (LENGTH - 1);
    ch = ALPHABET[choose];
    s += ch;
  }
  return s;
}

static void generate(TestStruct* pts) {
  pts->key1 = makeRandomString();
  pts->key2 = makeRandomString();
  pts->value1 = makeRandomString();
}

int Compare(const TestStruct* el1, const TestStruct* el2) {
  int cmp = wcscmp(el2->key1.c_str(), el1->key1.c_str());
  if (0 != cmp) return cmp;
  std::wstring el1_cs = el1->key1 + el1->key2;
  std::wstring el2_cs = el2->key1 + el2->key2;
  return wcscmp(el1_cs.c_str(), el2_cs.c_str());
}

unsigned int HashFunc(const TestStruct* pElement) {
  std::wstring str = pElement->key1 + pElement->key2;
  size_t size = str.size();
  unsigned int hash = 5230;
  for (size_t i = 0; i < size; i++) {
    hash = ((hash << 5) + hash) + str[i];
  }
  return hash;
}

void generateArray(TestStruct* pArray, int count) {
  for (int i = 0; i < count; i++) {
    generate(&pArray[i]);
    int j = 0;
    while (i != j) {
      if (0 == Compare(&pArray[i], &pArray[j])) {
        j = 0;
        generate(&pArray[i]);
        continue;
      }
      j++;
    }
  }
}

TestStruct* binarySearch(const TestStruct& element, TestStruct** pPArray,
                         int size) {
  if (0 == size) return nullptr;
  TestStruct el = element;
  int cmp = Compare(&el, pPArray[size / 2]);
  if (cmp > 0) {
    return binarySearch(element, pPArray, size / 2);
  } else if (cmp < 0) {
    if (size == 1) return nullptr;
    return binarySearch(element, pPArray + size / 2, size - size / 2);
  } else {
    return pPArray[size / 2];
  }
}

void timeMeasurement(const int ELEMENTS_COUNT, TestStruct* nElementsArray,
                     TestStruct* doubleNELementsArray) {
  std::cout << ELEMENTS_COUNT << std::endl;
  std::ofstream out("tm.csv", std::ios::app);
  out << ELEMENTS_COUNT << ", ";

  // создание структур
  TestStruct** nPElementsArray = new TestStruct*[ELEMENTS_COUNT];
  for (int i = 0; i < ELEMENTS_COUNT; i++) {
    nPElementsArray[i] = &(nElementsArray[i]);
  }

  lab618::CAVLTree<TestStruct, Compare> tree;

  lab618::CHash<TestStruct, HashFunc, Compare> h_table(ELEMENTS_COUNT, 100000);

  // размещение массива данных в структуры
  Timer timer;
  timer.reset();
  double time;

  templates::mergeSort(nPElementsArray, ELEMENTS_COUNT, Compare);
  time = timer.elapsed();
  out << time << ", ";

  timer.reset();
  for (int i = 0; i < ELEMENTS_COUNT; i++) {
    tree.add(&nElementsArray[i]);
  }
  time = timer.elapsed();
  out << time << ", ";

  timer.reset();
  for (int i = 0; i < ELEMENTS_COUNT; i++) {
    h_table.add(&nElementsArray[i]);
  }
  time = timer.elapsed();
  out << time << ", ";

  // поиск
  // Найти один раз все объекты из исходных данных;

  //проверка перед измерением
  for (int i = 0; i < ELEMENTS_COUNT; i++) {
    assert(&(nElementsArray[i]) ==
           binarySearch(nElementsArray[i], nPElementsArray, ELEMENTS_COUNT));
  }
  timer.reset();
  for (int i = 0; i < ELEMENTS_COUNT; i++) {
    binarySearch(nElementsArray[i], nPElementsArray, ELEMENTS_COUNT);
  }
  time = timer.elapsed();
  out << time << ", ";

  for (int i = 0; i < ELEMENTS_COUNT; i++) {
    assert(&(nElementsArray[i]) == tree.find(nElementsArray[i]));
  }
  timer.reset();
  for (int i = 0; i < ELEMENTS_COUNT; i++) {
    tree.find(nElementsArray[i]);
  }
  time = timer.elapsed();
  out << time << ", ";

  for (int i = 0; i < ELEMENTS_COUNT; i++) {
    assert(&(nElementsArray[i]) == h_table.find(nElementsArray[i]));
  }
  timer.reset();
  for (int i = 0; i < ELEMENTS_COUNT; i++) {
    h_table.find(nElementsArray[i]);
  }
  time = timer.elapsed();
  out << time << ", ";

  // поиск 2N объектов
  timer.reset();
  for (int i = 0; i < 2 * ELEMENTS_COUNT; i++) {
    binarySearch(doubleNELementsArray[i], nPElementsArray, ELEMENTS_COUNT);
  }
  time = timer.elapsed();
  out << time << ", ";

  timer.reset();
  for (int i = 0; i < 2 * ELEMENTS_COUNT; i++) {
    tree.find(doubleNELementsArray[i]);
  }
  time = timer.elapsed();
  out << time << ", ";

  timer.reset();
  for (int i = 0; i < 2 * ELEMENTS_COUNT; i++) {
    h_table.find(doubleNELementsArray[i]);
  }
  time = timer.elapsed();
  out << time << ", ";

  // удаление
  timer.reset();
  delete[] nPElementsArray;
  time = timer.elapsed();
  out << time << ", ";

  timer.reset();
  tree.clear();
  time = timer.elapsed();
  out << time << ", ";

  timer.reset();
  h_table.clear();
  time = timer.elapsed();
  out << time << ", ";

  // поэлементное удаление с проверкой
  for (int i = 0; i < ELEMENTS_COUNT; i++) {
    assert(true == tree.add(&nElementsArray[i]));
  }
  for (int i = 0; i < ELEMENTS_COUNT; i++) {
    assert(true == tree.remove(nElementsArray[i]));
  }
  for (int i = 0; i < ELEMENTS_COUNT; i++) {
    assert(true == tree.add(&nElementsArray[i]));
  }
  timer.reset();
  for (int i = 0; i < ELEMENTS_COUNT; i++) {
    tree.remove(nElementsArray[i]);
  }
  time = timer.elapsed();
  out << time << ", ";

  for (int i = 0; i < ELEMENTS_COUNT; i++) {
    assert(true == h_table.add(&nElementsArray[i]));
  }
  for (int i = 0; i < ELEMENTS_COUNT; i++) {
    assert(true == h_table.remove(nElementsArray[i]));
  }
  for (int i = 0; i < ELEMENTS_COUNT; i++) {
    assert(true == h_table.add(&nElementsArray[i]));
  }
  timer.reset();
  for (int i = 0; i < ELEMENTS_COUNT; i++) {
    h_table.remove(nElementsArray[i]);
  }
  time = timer.elapsed();
  out << time << "\n";

  out.close();
}

int main() {
  // готовим файл для записи в формате csv
  // генерация объектов
  TestStruct* nElementsArray = new TestStruct[1000000];
  TestStruct* doubleNELementsArray = new TestStruct[2000000];
  generateArray(nElementsArray, 1000000);
  generateArray(doubleNELementsArray, 2000000);
  for (int i = 10000; i < 1000000; i = int(double(i) * 1.1f)) {
    timeMeasurement(i, nElementsArray, doubleNELementsArray);
  }
  delete[] nElementsArray;
  delete[] doubleNELementsArray;
  return 0;
}
