#ifndef TEMPLATES_LIST_2021_02_11
#define TEMPLATES_LIST_2021_02_11

#include <stdexcept>
#include <utility>

namespace lab618 {
template <class T>
class CSingleLinkedList {
 private:
  struct leaf {
    T data;
    leaf* pnext;
    leaf(T& _data, leaf* _pnext) : data(_data), pnext(_pnext) {}
  };

 public:
  class CIterator {
   public:
    CIterator() : m_pCurrent(0), m_pBegin(0) {}

    CIterator(leaf* p) : m_pCurrent(p), m_pBegin(0) {}

    CIterator(const CIterator& src)
        : m_pBegin(src.m_pBegin), m_pCurrent(src.m_pCurrent) {}

    ~CIterator() {}

    CIterator& operator=(const CIterator& src) {
      m_pBegin = src.m_pBegin;
      m_pCurrent = src.m_pCurrent;
      return *this;
    }

    bool operator!=(const CIterator& it) const {
      return !((m_pCurrent == it.m_pCurrent) && (m_pBegin == it.m_pBegin));
    }

    void operator++() {
      if (m_pCurrent == 0) {
        if (m_pBegin != 0) {
          m_pCurrent = m_pBegin;
          m_pBegin = 0;
        }
        return;
      }
      m_pCurrent = m_pCurrent->pnext;
    }

    T& getData() {
      if (m_pCurrent == 0)
        throw std::runtime_error("Error in getData.\nm_pCurrent is zero.\n");
      return m_pCurrent->data;
    }

    T& operator*() {
      if (m_pCurrent == 0)
        throw std::runtime_error("Error in getData.\nm_pCurrent is zero.\n");
      return m_pCurrent->data;
    }

    leaf* getLeaf() {
      if (m_pCurrent == 0)
        throw std::runtime_error("Error in getLeaf.\nm_pCurrent is zero.\n");
      return m_pCurrent;
    }

    void setLeaf(leaf* p) {
      m_pBegin = 0;
      m_pCurrent = p;
    }

    void setLeafPreBegin(leaf* p) {
      m_pCurrent = 0;
      m_pBegin = p;
    }

    bool isValid() {
      if (m_pCurrent != 0) return true;
      return false;
    }

   private:
    //храним голову списка, если мы находимся перед началом
    leaf* m_pBegin;
    // храним текущее положение
    leaf* m_pCurrent;
  };

 public:
  CSingleLinkedList() : m_pBegin(0), m_pEnd(0) {}

  virtual ~CSingleLinkedList() { clear(); }

  void pushBack(T& data) {
    if ((m_pBegin == 0) && (m_pEnd == 0)) {
      m_pBegin = new leaf(data, 0);
      m_pEnd = m_pBegin;
      return;
    }
    leaf* p_tmpLeaf = m_pEnd;
    m_pEnd = new leaf(data, 0);
    p_tmpLeaf->pnext = m_pEnd;
  }

  void pushFront(T& data) {
    if ((m_pBegin == 0) && (m_pEnd == 0)) {
      m_pBegin = new leaf(data, m_pBegin);
      m_pEnd = m_pBegin;
      return;
    }
    m_pBegin = new leaf(data, m_pBegin);
  }

  T popFront() {
    if (m_pBegin == 0)
      throw std::runtime_error("Error in popFront.\nThe list is empty.\n");
    leaf* pTmp = m_pBegin;

    leaf* new_begin = m_pBegin->pnext;
    m_pBegin = new_begin;
    if (m_pBegin == 0) m_pEnd = 0;

    T tmp = std::move(pTmp->data);
    delete pTmp;
    pTmp = 0;

    return tmp;
  }

  // изменяет состояние итератора. выставляет предыдущую позицию.
  void erase(CIterator& it) {
    if (!it.isValid()) return;
    leaf* p = it.getLeaf();

    leaf* pprev = 0;
    if (p == m_pBegin) {
      m_pBegin = p->pnext;
      it.setLeafPreBegin(m_pBegin);
    } else {
      // find prev element
      pprev = m_pBegin;
      while (pprev != 0) {
        if (pprev->pnext == p) break;
        pprev = pprev->pnext;
      }
      it.setLeaf(pprev);
    }
    if (pprev != 0) pprev->pnext = p->pnext;
    if (!p->pnext)  // in the end
      m_pEnd = pprev;
    p->pnext = 0;
    delete p;
    p = 0;
  }

  int getSize() {
    size_t size = 0;
    for (leaf* pCurrent = m_pBegin; pCurrent != 0; pCurrent = pCurrent->pnext)
      ++size;
    return size;
  }

  void clear() {
    leaf* pCurrent = m_pBegin;

    m_pBegin = 0;
    m_pEnd = 0;

    while (pCurrent != 0) {
      leaf* pNext = pCurrent->pnext;
      delete pCurrent;
      pCurrent = pNext;
    }
    pCurrent = 0;
  }

  CIterator begin() { return CIterator(m_pBegin); }

 private:
  leaf *m_pBegin, *m_pEnd;
};

template <class T>
class CDualLinkedList {
 private:
  struct leaf {
    T data;
    leaf *pnext, *pprev;
    leaf(T& _data, leaf* _pprev, leaf* _pnext) {
      data = _data;
      pprev = _pprev;
      pnext = _pnext;
    }
  };

 public:
  class CIterator {
   public:
    CIterator() : m_pBegin(0), m_pCurrent(0), m_pEnd(0) {}

    CIterator(leaf* p) : m_pBegin(0), m_pCurrent(p), m_pEnd(0) {}

    CIterator(const CIterator& src)
        : m_pBegin(src.m_pBegin),
          m_pCurrent(src.m_pCurrent),
          m_pEnd(src.m_pEnd) {}

    ~CIterator() {}

    CIterator& operator=(const CIterator& src) {
      m_pBegin = src.m_pBegin;
      m_pCurrent = src.m_pCurrent;
      m_pEnd = src.m_pEnd;
      return *this;
    }

    bool operator!=(const CIterator& it) const {
      return (!((m_pCurrent == it.m_pCurrent) && (m_pBegin == it.m_pBegin) &&
                (m_pEnd == it.m_pEnd)));
    }

    void operator++() {
      if (m_pCurrent == 0) {
        if (m_pBegin != 0) {
          m_pCurrent = m_pBegin;
          m_pBegin = 0;
        }
        return;
      } else {
        if (m_pCurrent->pnext == 0) m_pEnd = m_pCurrent;
        m_pCurrent = m_pCurrent->pnext;
      }
    }

    void operator--() {
      if (m_pCurrent == 0) {
        if (m_pEnd != 0) {
          m_pCurrent = m_pEnd;
          m_pEnd = 0;
        }
        return;
      } else {
        if (m_pCurrent->pprev == 0) m_pBegin = m_pCurrent;
        m_pCurrent = m_pCurrent->pprev;
      }
    }

    T& getData() {
      if (m_pCurrent) return m_pCurrent->data;
      if (m_pEnd) return m_pEnd->data;
      throw std::runtime_error("Error in getData().\nList is empty.\n");
    }

    T& operator*() {
      if (m_pCurrent) return m_pCurrent->data;
      if (m_pEnd) return m_pEnd->data;
      throw std::runtime_error("Error in operator *().\nList is empty.\n");
    }

    leaf* getLeaf() {
      if (m_pCurrent) return m_pCurrent;
      return 0;
    }

    // применяется в erase и eraseAndNext
    void setLeaf(leaf* p) {
      m_pBegin = 0;
      m_pCurrent = p;
      m_pEnd = 0;
    }

    // применяется в erase и eraseAndNext
    void setLeafPreBegin(leaf* p) {
      m_pBegin = p;
      m_pCurrent = 0;
      m_pEnd = 0;
    }

    // применяется в erase и eraseAndNext
    void setLeafPostEnd(leaf* p) {
      m_pBegin = 0;
      m_pCurrent = 0;
      m_pEnd = p;
    }

    bool isValid() { return ((m_pCurrent != 0) || (m_pEnd != 0)); }

   private:
    //храним голову списка, если мы находимся перед началом
    leaf* m_pBegin;
    // храним текущее положение
    leaf* m_pCurrent;
    //храним конец списка, если мы находимся после конца
    leaf* m_pEnd;
  };

 public:
  CDualLinkedList() : m_pBegin(0), m_pEnd(0) {}

  virtual ~CDualLinkedList() { clear(); }

  void pushBack(T& data) {
    if (m_pBegin && m_pEnd) {
      m_pEnd->pnext = new leaf(data, m_pEnd, 0);
      m_pEnd = m_pEnd->pnext;
      return;
    }
    m_pBegin = new leaf(data, 0, 0);
    m_pEnd = m_pBegin;
  }

  T popBack() {
    if (m_pEnd == 0)
      throw std::runtime_error("Error in popBack().\nThe list is empty.\n");
    leaf* pTmp = m_pEnd;

    leaf* new_end = m_pEnd->pprev;
    m_pEnd = new_end;
    if (m_pEnd == 0) m_pBegin = 0;

    T tmp = std::move(pTmp->data);
    delete pTmp;
    pTmp = 0;

    return tmp;
  }

  void pushFront(T& data) {
    if ((m_pBegin == 0) && (m_pEnd == 0)) {
      m_pBegin = new leaf(data, 0, 0);
      m_pEnd = m_pBegin;
      return;
    }
    m_pBegin->pprev = new leaf(data, 0, m_pBegin);
    m_pBegin = m_pBegin->pprev;
  }

  T popFront() {
    if (m_pBegin == 0)
      throw std::runtime_error("Error in popFront.\nThe list is empty.\n");
    leaf* pTmp = m_pBegin;

    leaf* new_begin = m_pBegin->pnext;
    m_pBegin = new_begin;
    if (m_pBegin == 0) m_pEnd = 0;

    T tmp = std::move(pTmp->data);
    delete pTmp;
    pTmp = 0;

    return tmp;
  }

  // изменяет состояние итератора. выставляет предыдущую позицию.
  void erase(CIterator& it) {
    if (!it.isValid()) return;
    leaf* p = it.getLeaf();

    leaf* pprev = 0;
    if (p == m_pBegin) {
      m_pBegin = p->pnext;
      it.setLeafPreBegin(m_pBegin);
    } else {
      // find prev element
      pprev = m_pBegin;
      while (pprev != 0) {
        if (pprev->pnext == p) break;
        pprev = pprev->pnext;
      }
      it.setLeaf(pprev);
    }
    if (pprev != 0) pprev->pnext = p->pnext;
    if (!p->pnext) {  // in the end
      m_pEnd = pprev;
    } else {
      p->pnext->pprev = pprev;
    }
    p->pprev = 0;
    p->pnext = 0;
    delete p;
    p = 0;
  }

  // изменяет состояние итератора. выставляет следующую позицию.
  void eraseAndNext(CIterator& it) {
    leaf* p = it.getLeaf();
    if (p == 0) return;

    leaf* pnext = 0;
    if (p == m_pEnd) {
      m_pEnd = p->pprev;
      it.setLeafPostEnd(m_pEnd);
    } else {
      // find next element
      pnext = m_pEnd;
      while (pnext != 0) {
        if (pnext->pprev == p) break;
        pnext = pnext->pprev;
      }
      it.setLeaf(pnext);
    }
    if (pnext != 0) pnext->pprev = p->pprev;
    if (!p->pprev) {  // in the begin
      m_pBegin = pnext;
    } else {
      p->pprev->pnext = pnext;
    }
    p->pprev = 0;
    p->pnext = 0;
    delete p;
    p = 0;
  }

  int getSize() {
    size_t size = 0;
    for (leaf* pCurrent = m_pBegin; pCurrent != 0; pCurrent = pCurrent->pnext)
      ++size;
    return size;
  }

  void clear() {
    leaf* pCurrent = m_pBegin;

    m_pBegin = 0;
    m_pEnd = 0;

    while (pCurrent != 0) {
      leaf* pNext = pCurrent->pnext;
      delete pCurrent;
      pCurrent = pNext;
    }
    pCurrent = 0;
  }

  CIterator begin() { return CIterator(m_pBegin); }

  CIterator end() {
    CIterator it;
    it.setLeafPostEnd(m_pEnd);
    return it;
  }

 private:
  leaf *m_pBegin, *m_pEnd;
};
}  // namespace lab618
#endif  //#ifndef TEMPLATES_LIST_2021_02_11
