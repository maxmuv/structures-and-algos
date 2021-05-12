#ifndef AVL_HEAD_H_2021_04_01
#define AVL_HEAD_H_2021_04_01

#include <cassert>
#include "memory manager/mm.h"

namespace lab618 {
template <class T, int (*Compare)(const T* pElement, const T* pElement2)>
class CAVLTree {
 private:
  struct leaf {
    T* pData;
    leaf* pLeft;
    leaf* pRight;
    int balanceFactor;
  };

 public:
  class CException {
   public:
    CException() {}
  };

 public:
  CAVLTree() : m_Memory(100000, true), m_pRoot(nullptr) {}

  virtual ~CAVLTree() { clear(); }

  // добавляем рекурсивным алгоритмом
  bool add(T* pElement) { return innerAdd(pElement, true); }

  bool update(T* pElement) { return innerAdd(pElement, false); }

  T* find(const T& pElement) {
    if (nullptr == m_pRoot) return nullptr;
    leaf* curLeaf = m_pRoot;
    int cmp = Compare(curLeaf->pData, &pElement);
    if (cmp > 0) {  // ключ в данных корня меньше, чем у элемента
      m_pRoot = curLeaf->pRight;
    } else if (cmp < 0) {  // ключ в данных корня больше, чем у элемента
      m_pRoot = curLeaf->pLeft;
    } else {
      return curLeaf->pData;
    }
    T* f = find(pElement);
    m_pRoot = curLeaf;
    return f;
  }

  bool remove(const T& element) {
    bool find = true;
    innerRemove(element, nullptr, find);
    return find;
  }

  void clear() {
    m_Memory.clear();
    m_pRoot = nullptr;
  }
  // методы для проверки корректности работы
  void correctTree() { innerCorrectTree(m_pRoot); }
  bool isEmpty() { return (nullptr == m_pRoot); }

 private:
  int innerCorrectTree(
      leaf* node) {  // функция для теста, возвращает высоту дерева, в случае
    // несбалансированного дерева срабатывает assert
    if (nullptr == node) return 0;
    leaf* leftNode = node->pLeft;
    leaf* rightNode = node->pRight;
    int leftHeight = innerCorrectTree(leftNode);
    int rightHeight = innerCorrectTree(rightNode);
    assert(node->balanceFactor == rightHeight - leftHeight);
    assert(node->balanceFactor > -2);
    assert(node->balanceFactor < 2);
    return (rightHeight > leftHeight) ? rightHeight + 1 : leftHeight + 1;
  }

 private:
  bool innerAdd(T* pElement,
                bool add) {  // если корень пустой, то добавляем элемент
    if (nullptr == m_pRoot) {
      try {
        m_pRoot = m_Memory.newObject();
      } catch (...) {
        throw CException();
      }
      m_pRoot->balanceFactor = 0;
      m_pRoot->pData = pElement;
      m_pRoot->pLeft = nullptr;
      m_pRoot->pRight = nullptr;
      if (add) return true;
      return false;
    }

    //для того, чтобы знать менять ли баланс фактор у корня, ввёдем две
    //переменые
    int oldChildBF;
    int newChildBF;
    bool right;
    // изменим корень для дальнейшего рекурсивного спуска
    leaf* curLeaf = m_pRoot;
    int cmp = Compare(curLeaf->pData, pElement);
    if (cmp > 0) {  // ключ в данных корня меньше, чем у элемента
      m_pRoot = curLeaf->pRight;
      right = true;
      if (nullptr != curLeaf->pRight) {
        oldChildBF = curLeaf->pRight->balanceFactor;
        newChildBF = 0;
      } else {  // случай, когда правого потомка нет (особый случай)
        oldChildBF = 0;
        newChildBF = 1;
      }
    } else if (cmp < 0) {  // ключ в данных корня больше, чем у элемента
      m_pRoot = curLeaf->pLeft;
      right = false;
      if (nullptr != curLeaf->pLeft) {
        oldChildBF = curLeaf->pLeft->balanceFactor;
        newChildBF = 0;
      } else {  // случай, когда левого потомка нет(особый случай)
        oldChildBF = 0;
        newChildBF = -1;
      }
    } else {
      if (add) return false;
      m_pRoot->pData = pElement;
      return true;
    }

    // теперь рекурсивно добавляем элемент
    try {
      if (add) {
        if (false == innerAdd(pElement, add)) {
          m_pRoot = curLeaf;
          return false;
        }
      } else {
        if (true == innerAdd(pElement, add)) {
          m_pRoot = curLeaf;
          return true;
        }
      }
    } catch (...) {
      throw CException();
    }

    // изменяем curLeaf
    if (right)
      curLeaf->pRight = m_pRoot;
    else
      curLeaf->pLeft = m_pRoot;
    if (0 == newChildBF) {
      newChildBF = m_pRoot->balanceFactor;
    }
    assert((oldChildBF > -2) && (oldChildBF < 2));
    assert((newChildBF > -2) && (newChildBF < 2));
    if (newChildBF - oldChildBF == 2) {  //возможно в результате балансировки
      if (right)
        curLeaf->balanceFactor++;
      else
        curLeaf->balanceFactor--;
    } else if (newChildBF - oldChildBF == -2) {
      if (right)
        curLeaf->balanceFactor++;
      else
        curLeaf->balanceFactor--;
    } else if (0 != newChildBF) {
      if (0 != newChildBF - oldChildBF)
        curLeaf->balanceFactor += (right) ? 1 : -1;
      assert((newChildBF - oldChildBF < 2) && (newChildBF - oldChildBF > -2));
    }
    assert((curLeaf->balanceFactor > -3) && (curLeaf->balanceFactor < 3));

    // выполняем балансировку, если нужно
    curLeaf = balancing(curLeaf);

    //выставляем m_pRoot и возвращаем true
    m_pRoot = curLeaf;
    if (add) return true;
    return false;
  }

  bool innerRemove(const T& element, leaf* target,
                   bool& find) {  // возвращает true, если поддерево
                                  // уменьшилось, false - иначе.
    if (nullptr == m_pRoot) {
      find = false;
      return false;
    }

    if (nullptr == target) {
      bool right;
      leaf* curLeaf;
      int cmp = Compare(m_pRoot->pData, &element);
      if (cmp > 0) {
        right = true;
        curLeaf = m_pRoot;
        m_pRoot = curLeaf->pRight;
      } else if (cmp < 0) {
        right = false;
        curLeaf = m_pRoot;
        m_pRoot = curLeaf->pLeft;
      } else {  // нашли элемент теперь, заменим его самым маленьким по значению
                // элементом правого поддерева. Если правого поддерева нет, то
                // самым большим левого. Если это лист, то удалим.
        if ((nullptr == m_pRoot->pRight) && (nullptr == m_pRoot->pLeft)) {
          m_Memory.deleteObject(m_pRoot);
          m_pRoot = nullptr;
          return true;
        }
        if (nullptr != m_pRoot->pRight) {
          if (nullptr == m_pRoot->pRight->pLeft) {
            leaf* newLeaf = m_pRoot->pRight;
            newLeaf->pLeft = m_pRoot->pLeft;
            newLeaf->balanceFactor = m_pRoot->balanceFactor - 1;
            m_Memory.deleteObject(m_pRoot);
            m_pRoot = newLeaf;

            m_pRoot = balancing(m_pRoot);
            if (0 == m_pRoot->balanceFactor) return true;
            return false;
          } else {
            target = m_pRoot;
            curLeaf = m_pRoot;
            m_pRoot = curLeaf->pRight;
            right = true;
          }
        } else {
          leaf* newLeaf = m_pRoot->pLeft;
          newLeaf->balanceFactor = m_pRoot->balanceFactor + 1;
          m_Memory.deleteObject(m_pRoot);
          m_pRoot = newLeaf;

          m_pRoot = balancing(m_pRoot);
          if (0 == m_pRoot->balanceFactor) return true;
          return false;
        }
      }

      bool del = innerRemove(element, target, find);

      (right) ? curLeaf->pRight = m_pRoot : curLeaf->pLeft = m_pRoot;

      if (del) {
        curLeaf->balanceFactor += (right) ? -1 : 1;
      } else {
        m_pRoot = curLeaf;
        return false;
      }

      curLeaf = balancing(curLeaf);

      m_pRoot = curLeaf;

      if (0 == curLeaf->balanceFactor) {
        return true;
      }
      return false;
    } else {
      if (nullptr == m_pRoot->pLeft) {
        target->pData = m_pRoot->pData;
        leaf* delLeaf = m_pRoot;
        m_pRoot = m_pRoot->pRight;
        m_Memory.deleteObject(delLeaf);
        return true;
      }
      leaf* curLeaf = m_pRoot;
      m_pRoot = curLeaf->pLeft;
      bool del = innerRemove(element, target, find);
      if (del) {
        curLeaf->pLeft = m_pRoot;
        curLeaf->balanceFactor++;
      } else {
        curLeaf->pLeft = m_pRoot;
        m_pRoot = curLeaf;
        return false;
      }
      curLeaf = balancing(curLeaf);
      m_pRoot = curLeaf;
      if (0 == curLeaf->balanceFactor) {
        return true;
      }
      return false;
    }
  }

  leaf* balancing(leaf* l) {
    if (l->balanceFactor == 2) {
      leaf* a = l;
      leaf* b = l->pRight;

      assert(nullptr != b);
      assert((b->balanceFactor > -2) && (b->balanceFactor < 2));

      if (0 == b->balanceFactor) {
        a->pRight = b->pLeft;
        b->pLeft = a;
        a->balanceFactor = 1;
        b->balanceFactor = -1;
        l = b;
      } else if (1 == b->balanceFactor) {
        a->pRight = b->pLeft;
        b->pLeft = a;
        a->balanceFactor = 0;
        b->balanceFactor = 0;
        l = b;
      } else {
        leaf* c = b->pLeft;

        assert(nullptr != c);
        assert((c->balanceFactor > -2) && (c->balanceFactor < 2));

        a->pRight = c->pLeft;
        b->pLeft = c->pRight;
        c->pLeft = a;
        c->pRight = b;
        l = c;

        if (0 == c->balanceFactor) {
          c->balanceFactor = 0;
          a->balanceFactor = 0;
          b->balanceFactor = 0;
        } else if (1 == c->balanceFactor) {
          c->balanceFactor = 0;
          b->balanceFactor = 0;
          a->balanceFactor = -1;
        } else {
          c->balanceFactor = 0;
          b->balanceFactor = 1;
          a->balanceFactor = 0;
        }
      }
    }
    if (l->balanceFactor == -2) {  // правое вращение
      leaf* a = l;
      leaf* b = l->pLeft;

      assert(nullptr != b);
      assert((b->balanceFactor > -2) && (b->balanceFactor < 2));

      if (0 == b->balanceFactor) {
        a->pLeft = b->pRight;
        b->pRight = a;
        a->balanceFactor = -1;
        b->balanceFactor = 1;
        l = b;
      } else if (-1 == b->balanceFactor) {
        a->pLeft = b->pRight;
        b->pRight = a;
        a->balanceFactor = 0;
        b->balanceFactor = 0;
        l = b;
      } else {
        leaf* c = b->pRight;

        assert(nullptr != c);
        assert((c->balanceFactor > -2) && (c->balanceFactor < 2));

        a->pLeft = c->pRight;
        b->pRight = c->pLeft;
        c->pRight = a;
        c->pLeft = b;
        l = c;

        if (0 == c->balanceFactor) {
          c->balanceFactor = 0;
          a->balanceFactor = 0;
          b->balanceFactor = 0;
        } else if (-1 == c->balanceFactor) {
          c->balanceFactor = 0;
          b->balanceFactor = 0;
          a->balanceFactor = 1;
        } else {
          c->balanceFactor = 0;
          b->balanceFactor = -1;
          a->balanceFactor = 0;
        }
      }
    }
    return l;
  }

 private:
  leaf* m_pRoot;
  CMemoryManager<leaf> m_Memory;
};
}  // namespace lab618

#endif  // #define AVL_HEAD_H_2021_04_01
