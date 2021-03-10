#ifndef SORT_HEAD_H_2021_02_25
#define SORT_HEAD_H_2021_02_25

namespace templates {
/** Определение типа функции сравнения элементов
\return
\li > 0  - pElem1 меньше pElem2
\li 0   - элементы pElem1 и pElem2 равны
\li < 0 - pElem1 больше pElem2
*/
typedef int(CompareSortType)(const void *pElem1, const void *pElem2);

void mergeSort(void **ppArray, int length, CompareSortType pCompareFunc) {
  if (length > 1) {
    int left_size = length / 2;
    int right_size = length - length / 2;

    mergeSort(ppArray, left_size, pCompareFunc);
    mergeSort(ppArray + left_size, right_size, pCompareFunc);

    int left_border = 0, right_border = left_size, i = 0;
    void **ppTmpArray = new void *[length];

    while (left_border < left_size || right_border < length) {
      if (pCompareFunc(ppArray[left_border], ppArray[right_border]) > 0) {
        ppTmpArray[i++] = std::move(ppArray[left_border]);
        left_border++;
      } else {
        ppTmpArray[i++] = std::move(ppArray[right_border]);
        right_border++;
      }

      if (left_border == left_size) {
        for (int j = right_border; j < length; j++, i++)
          ppTmpArray[i] = ppArray[j];
        break;
      }

      if (right_border == length) {
        for (int j = left_border; j < left_size; j++, i++)
          ppTmpArray[i] = ppArray[j];
        break;
      }
    }

    for (i = 0; i < length; i++) ppArray[i] = ppTmpArray[i];

    delete[] ppTmpArray;
  }
}
void heapSort(void **ppArray, int length, CompareSortType pCompareFunc);

template <class T>
void mergeSort(T **ppArray, int length,
               int(pCompareFunc)(const T *pElem1, const T *pElem2)) {
  mergeSort(reinterpret_cast<void **>(ppArray), length,
            (CompareSortType *)pCompareFunc);
}
template <class T>
void heapSort(T **ppArray, int length,
              int(pCompareFunc)(const T *pElem1, const T *pElem2)) {
  heapSort(reinterpret_cast<void **>(ppArray), length,
           (CompareSortType *)pCompareFunc);
}
}  // namespace templates

#endif  // #define SORT_HEAD_H_2021_02_25
