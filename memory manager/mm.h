#ifndef MEMORY_MANAGER_HEAD_H_2021_02_18
#define MEMORY_MANAGER_HEAD_H_2021_02_18

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>

namespace lab618 {
template <class T>
class CMemoryManager {
 private:
  struct block {
    // ?????? ?????? ?????
    T *pdata;
    // ????? ?????????? ?????
    block *pnext;
    // ?????? ????????? ??????
    int firstFreeIndex;
    // ????? ??????????? ?????
    int usedCount;
  };

 public:
  class CException {
   public:
    CException() {}
  };

 public:
  CMemoryManager(int _default_block_size,
                 bool isDeleteElementsOnDestruct = false)
      : m_blkSize(_default_block_size),
        m_pBlocks(0),
        m_pCurrentBlk(0),
        m_isDeleteElementsOnDestruct(isDeleteElementsOnDestruct) {}

  virtual ~CMemoryManager() {
    if (!m_pBlocks) clear();
  }

  // ???????? ????? ?????? ???????? ?? ?????????
  T *newObject() {
    if (!m_pBlocks) {
      m_pBlocks = newBlock();
      m_pCurrentBlk = m_pBlocks;
      m_pCurrentBlk->usedCount = 1;
      if (m_blkSize == 1)
        m_pCurrentBlk->firstFreeIndex = -1;
      else
        m_pCurrentBlk->firstFreeIndex = 1;

      memset(reinterpret_cast<void *>(m_pCurrentBlk->pdata), 0, sizeof(T));
      ::new (reinterpret_cast<void *>(m_pCurrentBlk->pdata)) T;

      return m_pCurrentBlk->pdata;
    }
    if (m_pCurrentBlk->firstFreeIndex == -1) {
      while (m_pCurrentBlk->pnext != 0) {
        m_pCurrentBlk = m_pCurrentBlk->pnext;
        if (m_pCurrentBlk->firstFreeIndex != -1) break;
      }
      if (m_pCurrentBlk->pnext == 0) {
        m_pCurrentBlk->pnext = newBlock();
        m_pCurrentBlk = m_pCurrentBlk->pnext;
        m_pCurrentBlk->usedCount = 1;
        if (m_blkSize == 1)
          m_pCurrentBlk->firstFreeIndex = -1;
        else
          m_pCurrentBlk->firstFreeIndex = 1;

        memset(reinterpret_cast<void *>(m_pCurrentBlk->pdata), 0, sizeof(T));
        ::new (reinterpret_cast<void *>(m_pCurrentBlk->pdata)) T;

        return m_pCurrentBlk->pdata;
      }
    }

    T *object = &(m_pCurrentBlk->pdata[m_pCurrentBlk->firstFreeIndex]);
    int *newFirstFreeIndex = reinterpret_cast<int *>(
        &(m_pCurrentBlk->pdata[m_pCurrentBlk->firstFreeIndex]));
    m_pCurrentBlk->firstFreeIndex = *newFirstFreeIndex;
    m_pCurrentBlk->usedCount++;

    memset(reinterpret_cast<void *>(object), 0, sizeof(T));
    ::new (reinterpret_cast<void *>(object)) T;

    return object;
  }

  // ?????????? ??????? ? ?????????
  bool deleteObject(T *p) {
    if (m_pBlocks == 0) return false;
    m_pCurrentBlk = 0;
    block *p_tmpblock = m_pBlocks;
    block *p_prevblock = 0;
    int find = -1;
    if (p_tmpblock->firstFreeIndex != -1) m_pCurrentBlk = p_tmpblock;
    for (int i = 0; i < m_blkSize; i++) {
      if (p == &(p_tmpblock->pdata[i])) find = i;
    }
    while ((find == -1) && (p_tmpblock->pnext != 0)) {
      p_prevblock = p_tmpblock;
      p_tmpblock = p_tmpblock->pnext;
      if (p_tmpblock->firstFreeIndex != -1) m_pCurrentBlk = p_tmpblock;
      for (int i = 0; i < m_blkSize; i++) {
        if (p == &(p_tmpblock->pdata[i])) find = i;
      }
    }
    if (find != -1) {
      if (!m_pCurrentBlk) m_pCurrentBlk = p_tmpblock;
      bool *manager_map = new bool[m_blkSize];
      memset(reinterpret_cast<void *>(manager_map), 0,
             m_blkSize * sizeof(bool));
      int freeindex = p_tmpblock->firstFreeIndex;
      int lastfreeindex = freeindex;
      for (; freeindex != -1; freeindex = *reinterpret_cast<int *>(
                                  &(p_tmpblock->pdata[freeindex]))) {
        manager_map[freeindex] = 1;
        lastfreeindex = freeindex;
      }
      if (manager_map[find] == 1) {
        delete[] manager_map;
        manager_map = 0;
        return false;
      }

      (&(p_tmpblock->pdata[find]))->~T();
      memset(reinterpret_cast<void *>(&(p_tmpblock->pdata[find])), 0,
             sizeof(T));

      if (p_tmpblock->firstFreeIndex == -1) {
        p_tmpblock->firstFreeIndex = find;
        *reinterpret_cast<int *>(&(p_tmpblock->pdata[find])) = -1;
        p_tmpblock->usedCount--;
      } else {
        *reinterpret_cast<int *>(&(p_tmpblock->pdata[find])) = -1;
        *reinterpret_cast<int *>(&(p_tmpblock->pdata[lastfreeindex])) = find;
        p_tmpblock->usedCount--;
      }

      if (p_tmpblock->usedCount == 0) {
        block *p_nextblock = p_tmpblock->pnext;
        deleteBlock(p_tmpblock);
        if (p_prevblock) {
          p_prevblock->pnext = p_nextblock;
        } else {
          m_pBlocks = p_nextblock;
        }
        if (!(p_nextblock)) m_pCurrentBlk = p_prevblock;
        p_tmpblock = 0;
      }

      delete[] manager_map;
      manager_map = 0;
      return true;
    }
    m_pCurrentBlk = m_pBlocks;
    return false;
  }

  // ??????? ??????, ??????? ?? m_isDeleteElementsOnDestruct
  void clear() {
    if (!m_pBlocks) return;
    block *p_block = m_pBlocks;
    block *p_nextblock = p_block->pnext;
    m_pBlocks = 0;
    m_pCurrentBlk = 0;

    while (p_nextblock != 0) {
      deleteBlock(p_block);
      p_block = p_nextblock;
      p_nextblock = p_block->pnext;
    }
    deleteBlock(p_block);
  }

 private:
  // ??????? ????? ???? ??????. ??????????? ? newObject
  block *newBlock() {
    block *p_block = new block{0, 0, 0, 0};
    p_block->pdata = (T *)malloc(m_blkSize * sizeof(T));
    for (int i = 0; i < m_blkSize; i++) {
      memset(reinterpret_cast<void *>(&(p_block->pdata[i])), 0, sizeof(T));
    }
    int *pointer;
    for (int i = 0; i < m_blkSize - 1; i++) {
      pointer = reinterpret_cast<int *>(&(p_block->pdata[i]));
      *pointer = i + 1;
    }
    pointer = reinterpret_cast<int *>(&(p_block->pdata[m_blkSize - 1]));
    *pointer = -1;
    return p_block;
  }

  // ?????????? ?????? ????? ??????. ??????????? ? clear
  void deleteBlock(block *p) {
    bool *manager_map = new bool[m_blkSize];
    memset(reinterpret_cast<void *>(manager_map), 0, m_blkSize * sizeof(bool));

    if (!m_isDeleteElementsOnDestruct)
      if (p->usedCount != 0)
        throw std::runtime_error(
            "Error in deleteBlock().\nBlock isn't empty.\n");

    if (m_isDeleteElementsOnDestruct) {
      int freeindex = p->firstFreeIndex;
      for (; freeindex != -1;
           freeindex = *reinterpret_cast<int *>(&(p->pdata[freeindex]))) {
        manager_map[freeindex] = 1;
      }
    }

    if (m_isDeleteElementsOnDestruct) {
      for (int i = 0; i < m_blkSize; i++)
        if (manager_map[i] == 0) {
          (&(p->pdata[i]))->~T();
          memset(reinterpret_cast<void *>(&(p->pdata[i])), 0, sizeof(T));
        } else {
          memset(reinterpret_cast<void *>(&(p->pdata[i])), 0, sizeof(T));
        }
    }
    delete[] manager_map;
    free(p->pdata);
    delete p;
    p = 0;
  }

  // ?????? ?????
  int m_blkSize;
  // ?????? ?????? ??????
  block *m_pBlocks;
  // ??????? ????
  block *m_pCurrentBlk;
  // ??????? ?? ???????? ??? ????????????
  bool m_isDeleteElementsOnDestruct;
};
}  // namespace lab618

#endif  // #define MEMORY_MANAGER_HEAD_H_2021_02_18
