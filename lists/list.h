#ifndef TEMPLATES_LIST_2021_02_11
#define TEMPLATES_LIST_2021_02_11

#include <stdexcept>

namespace lab618
{
    template<class T>
    class CSingleLinkedList
    {
    private:
        struct leaf
        {
            T data;
            leaf * pnext;
            leaf(T& _data, leaf * _pnext)
            {
                data = _data;
                pnext = _pnext;
            }
        };
    public:
        class CIterator
        {
        public:
            CIterator()
                : m_pCurrent(0), m_pBegin(0)
            {
            }

            CIterator(leaf *p)
                : m_pCurrent(0), m_pBegin(p)
            {
            }

            CIterator(const CIterator &src)
                : m_pBegin(0), m_pCurrent(0)
            {
                if (src.m_pBegin != 0)
                {
                    m_pBegin = src.m_pBegin;
                }
                if (src.m_pCurrent != 0)
                {
                    m_pCurrent = src.m_pCurrent;
                }
            }

            ~CIterator()
            {
            }

            CIterator& operator = (const CIterator&  src)
            {
                m_pCurrent = src.m_pCurrent;
                m_pBegin = src.m_pBegin;
                return *this;
            }

            bool operator != (const CIterator&  it) const
            {
                if (m_pCurrent == 0)
                    return false;
                if (m_pCurrent == it.m_pCurrent)
                    return false;
                return true;
            }

            void operator++()
            {
                if ((m_pCurrent == 0) && (m_pBegin == 0))
                    return;
                if (m_pCurrent == 0)
                {
                    m_pCurrent = m_pBegin;
                    m_pBegin = 0;
                    return;
                }
                if (m_pCurrent->pnext == 0)
                {
                    m_pCurrent = 0;
                    return;
                }
                m_pCurrent = m_pCurrent->pnext;
                return;
            }

            T& getData()
            {
                if (m_pCurrent == 0)
                    throw std::runtime_error("Error in getData.\nm_pCurrent is zero.\n");
                return m_pCurrent->data;
            }

            T& operator* ()
            {
                return getData();
            }

            leaf* getLeaf()
            {
                if (m_pCurrent == 0)
                    throw std::runtime_error("Error in getLeaf.\nm_pCurrent is zero.\n");
                return m_pCurrent;
            }

            void setLeaf(leaf* p)
            {
                if (m_pCurrent == 0)
                {
                    m_pBegin = 0;
                    m_pCurrent = p;
                    return;
                }
                m_pCurrent = p;
            }

            void setLeafPreBegin(leaf* p)
            {
                m_pCurrent = 0;
                m_pBegin = p;
            }

            bool isValid() {
                if (m_pCurrent != 0)
                    return true;
                return false;
            }

        private:
            //храним голову списка, если мы находимся перед началом
            leaf* m_pBegin;
            // храним текущее положение
            leaf* m_pCurrent;
        };

    public:

        CSingleLinkedList()
            : m_pBegin(0), m_pEnd(0)
        {
        }

        virtual ~CSingleLinkedList()
        {
            clear();
        }

        void pushBack(T& data)
        {
            if ((m_pBegin == 0) && (m_pEnd == 0))
            {
                m_pBegin = new leaf(data, 0);
                m_pEnd = m_pBegin;
                return;
            }
            leaf *p_tmpLeaf = m_pEnd;
            m_pEnd = new leaf(data, 0);
            p_tmpLeaf->pnext = m_pEnd;
        }

        void pushFront(T& data)
        {
            if ((m_pBegin == 0) && (m_pEnd == 0))
            {
                m_pBegin = new leaf(data, m_pBegin);
                m_pEnd = m_pBegin;
                return;
            }
            m_pBegin = new leaf(data, m_pBegin);
        }

        T popFront()
        {
            if (m_pBegin == 0)
                throw std::runtime_error("Error in popFront.\nThe list is empty.\n");
            leaf *p_tmpLeaf = m_pBegin->pnext;
            T data = m_pBegin->data;
            delete m_pBegin;
            if (m_pBegin == m_pEnd)
            {
                m_pBegin = 0;
                m_pEnd = 0;
                return data;
            }
            m_pBegin = p_tmpLeaf;
            return data;
        }

        // изменяет состояние итератора. выставляет предыдущую позицию.
        void erase(CIterator& it)
        {
            if (!it.isValid())
                throw std::runtime_error("Error in erase.\nCIterator isn't valid.\n");
            CIterator inIt(m_pBegin);
            inIt.operator ++();
            if (m_pBegin == it.getLeaf())
            {
                if (m_pBegin == m_pEnd)
                {
                    it = CIterator();
                    delete(m_pBegin);
                    m_pBegin = 0;
                    m_pEnd = 0;
                    return;
                }
                leaf *p_tmpLeaf = m_pBegin->pnext;
                CIterator newIt(p_tmpLeaf);
                it.operator =(newIt);
                delete(m_pBegin);
                m_pBegin = p_tmpLeaf;
                return;
            }
            while ((inIt.getLeaf() != m_pEnd)&&(inIt.getLeaf()->pnext != it.getLeaf()))
            {
                   inIt.operator ++();
            }
            if (inIt.getLeaf() == m_pEnd)
            {
                throw std::runtime_error("Error in erase.\nCannot find CIterator.\n");
            }
            if (it.getLeaf() == m_pEnd)
            {
                delete(m_pEnd);
                m_pEnd = inIt.getLeaf();
                it.operator =(inIt);
                return;
            }
            inIt.getLeaf()->pnext = it.getLeaf()->pnext;
            delete(it.getLeaf());
            it = inIt;
        }

        int getSize()
        {
            if (m_pBegin == 0)
                return 0;
            CIterator it(m_pBegin);
            it.operator ++();
            int size = 0;
            while (it.getLeaf() != m_pEnd)
            {
                size++;
                it.operator ++();
            }
            size++;
            return size;
        }

        void clear()
        {
            int size = getSize();
            while (size != 0)
            {
                size--;
                popFront();
            }
        }

        CIterator begin()
        {
            if (!m_pBegin)
                throw std::runtime_error("Error in begin.\nThe list is empty.\n");
            CIterator it(m_pBegin);
            it.operator ++();
            return it;
        }

    private:
        leaf* m_pBegin, *m_pEnd;
    };

    template<class T>
    class CDualLinkedList
    {
    private:
        struct leaf
        {
            T data;
            leaf * pnext, *pprev;
            leaf(T& _data, leaf * _pprev, leaf * _pnext)
            {
                data = _data;
                pprev = _pprev;
                pnext = _pnext;
            }
        };
    public:
        class CIterator
        {
        public:
            CIterator()
                :m_pBegin(0), m_pCurrent(0), m_pEnd(0)
            {
            }

            CIterator(leaf *p)
                :m_pBegin(p), m_pCurrent(0), m_pEnd(0)
            {
            }

            CIterator(const CIterator &src)
                :m_pBegin(0), m_pCurrent(0), m_pEnd(0)
            {
                if (src.m_pBegin != 0)
                {
                    m_pBegin = src.m_pBegin;
                }
                if (src.m_pCurrent != 0)
                {
                    m_pCurrent = src.m_pCurrent;
                }
                if (src.m_pEnd != 0)
                {
                    m_pEnd = src.m_pEnd;
                }
            }

            ~CIterator()
            {
            }

            CIterator& operator = (const CIterator&  src)
            {
                m_pBegin = src.m_pBegin;
                m_pCurrent = src.m_pCurrent;
                m_pEnd = src.m_pEnd;
                return *this;
            }

            bool operator != (const CIterator&  it) const
            {
                return ((m_pBegin != it.m_pBegin) || (m_pCurrent != it.m_pCurrent) || (m_pEnd != it.m_pEnd));
            }

            void operator++()
            {
                if (m_pBegin != 0)
                {
                    m_pCurrent = m_pBegin;
                    m_pBegin = 0;
                    return;
                }
                if (m_pCurrent != 0)
                {
                    if (m_pCurrent->pnext == 0)
                    {
                        m_pEnd = m_pCurrent;
                        m_pCurrent = 0;
                        return;
                    }
                    m_pCurrent = m_pCurrent->pnext;
                }
            }

            void operator--()
            {
                if (m_pEnd != 0)
                {
                    m_pCurrent = m_pEnd;
                    m_pEnd = 0;
                    return;
                }
                if (m_pCurrent != 0)
                {
                    if (m_pCurrent->pprev == 0)
                    {
                        m_pBegin = m_pCurrent;
                        m_pCurrent = 0;
                        return;
                    }
                    m_pCurrent = m_pCurrent->pprev;
                }
            }

            T& getData()
            {
                if (m_pCurrent)
                    return m_pCurrent->data;
                throw std::runtime_error("Error in getData().\nList is empty.\n");
            }

            T& operator* ()
            {
                if (m_pCurrent)
                    return m_pCurrent->data;
                throw std::runtime_error("Error in operator *().\nList is empty.\n");
            }

            leaf* getLeaf()
            {
                if (m_pCurrent)
                    return m_pCurrent;
                return 0;
            }

            // применяется в erase и eraseAndNext
            void setLeaf(leaf* p)
            {
                m_pCurrent = p;
                m_pBegin = 0;
                m_pEnd = 0;
            }

            // применяется в erase и eraseAndNext
            void setLeafPreBegin(leaf* p)
            {
                m_pBegin = p;
                m_pCurrent = 0;
                m_pEnd = 0;
            }

            // применяется в erase и eraseAndNext
            void setLeafPostEnd(leaf* p)
            {
                m_pBegin = 0;
                m_pCurrent = 0;
                m_pEnd = p;
            }

            bool isValid() {
                return (m_pCurrent != 0);
            }

        private:
            //храним голову списка, если мы находимся перед началом
            leaf* m_pBegin;
            // храним текущее положение
            leaf* m_pCurrent;
            //храним конец списка, если мы находимся после конца
            leaf* m_pEnd;
        };

    public:

        CDualLinkedList()
            : m_pBegin(0), m_pEnd(0)
        {
        }

        virtual ~CDualLinkedList()
        {
            clear();
        }

        void pushBack(T& data)
        {
            if (m_pBegin)
            {
                if (m_pEnd)
                {
                    m_pEnd->pnext = new leaf(data, m_pEnd, 0);
                    m_pEnd = m_pEnd->pnext;
                    return;
                }
                throw std::runtime_error("Error in pushBack().\nm_pEnd is zero, but m_pBegin isn't.\n");
            }
            m_pBegin = new leaf(data, 0, 0);
            m_pEnd = m_pBegin;

        }

        T popBack()
        {
            if (m_pEnd)
            {
                m_pEnd->data;
                T data = m_pEnd->data;
                if (m_pBegin != m_pEnd)
                {
                    leaf *p_tmpLeaf = m_pEnd->pprev;
                    delete(m_pEnd);
                    m_pEnd = p_tmpLeaf;
                    m_pEnd->pnext = 0;
                    return data;
                }
                delete(m_pEnd);
                m_pBegin = 0;
                m_pEnd = 0;
                return data;
            }
            throw std::runtime_error("Error in popBack().\nThe list is empty.\n");
        }

        void pushFront(T& data)
        {
            if (m_pBegin)
            {
                if (m_pEnd)
                {
                    m_pBegin = new leaf(data, 0, m_pBegin);
                    m_pBegin->pnext->pprev = m_pBegin;
                    return;
                }
                throw std::runtime_error("Error in pushFront().\nm_pBegin is-nt zero, but m_pEnd is.");
            }
            m_pBegin = new leaf(data, 0, 0);
            m_pEnd = m_pBegin;
        }

        T popFront()
        {
            if (m_pBegin)
            {
                T data = m_pBegin->data;
                if (m_pBegin != m_pEnd)
                {
                    leaf *p_tmpLeaf = m_pBegin->pnext;
                    delete(m_pBegin);
                    m_pBegin = p_tmpLeaf;
                    return data;
                }
                delete(m_pBegin);
                m_pBegin = 0;
                m_pEnd = 0;
                return data;
            }
            throw std::runtime_error("Error in popFront().\nThe list is empty.\n");        }

        // изменяет состояние итератора. выставляет предыдущую позицию.
        void erase(CIterator& it)
        {
            if (it.isValid())
            {
                leaf *p_tmpLeaf = it.getLeaf();
                it.operator --();
                if (it.isValid())
                {
                    it.getLeaf()->pnext = p_tmpLeaf->pnext;
                    p_tmpLeaf->pnext->pprev = it.getLeaf();
                }
                else
                {
                    m_pBegin = p_tmpLeaf->pnext;
                    if (m_pBegin)
                        m_pBegin->pprev = 0;
                    else
                        m_pEnd = 0;
                    it = CIterator(m_pBegin);
                }
                delete(p_tmpLeaf);
            }
        }

        // изменяет состояние итератора. выставляет следующую позицию.
        void eraseAndNext(CIterator& it)
        {
            if (it.isValid())
            {
                leaf *p_tmpLeaf = it.getLeaf();
                it.operator ++();
                if (it.isValid())
                {
                    it.getLeaf()->pprev = p_tmpLeaf->pprev;
                    p_tmpLeaf->pprev->pnext = it.getLeaf();
                }
                else
                {
                    m_pEnd = (p_tmpLeaf->pprev);
                    if (m_pEnd)
                        m_pEnd->pnext = 0;
                    else
                        m_pBegin = 0;
                    it = CIterator();
                    it.setLeafPostEnd(m_pEnd);
                }
                delete(p_tmpLeaf);
            }
        }

        int getSize()
        {
            int size = 0;
            CIterator it(m_pBegin);
            while (m_pBegin && (it.getLeaf() != m_pEnd))
            {
                it.operator ++();
                size++;
            }
            return size;
        }

        void clear()
        {
            while (m_pBegin && m_pEnd) {
                popBack();
            }
        }

        CIterator begin()
        {
            CIterator it(m_pBegin);
            it.operator ++();
            return it;
        }

        CIterator end()
        {
            CIterator it;
            it.setLeafPostEnd(m_pEnd);
            it.operator --();
            return it;
        }

    private:
        leaf* m_pBegin, *m_pEnd;
    };
}
#endif //#ifndef TEMPLATES_LIST_2021_02_11
