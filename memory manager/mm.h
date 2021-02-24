#ifndef MEMORY_MANAGER_HEAD_H_2021_02_18
#define MEMORY_MANAGER_HEAD_H_2021_02_18

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <cstring>

namespace lab618
{
    template <class T>
    class CMemoryManager
    {
    private:
        struct block
        {
            // ?????? ?????? ?????
            T* pdata;
            // ????? ?????????? ?????
            block *pnext;
            // ?????? ????????? ??????
            int firstFreeIndex;
            // ????? ??????????? ?????
            int usedCount;
        };
    public:
        class CException
        {
        public:
            CException()
            {
            }
        };

    public:
        CMemoryManager(int _default_block_size, bool isDeleteElementsOnDestruct = false)
            : m_blkSize(_default_block_size), m_pBlocks(0), m_pCurrentBlk(0), m_isDeleteElementsOnDestruct(isDeleteElementsOnDestruct)
        {
        }

        virtual ~CMemoryManager()
        {
            if (!m_pBlocks)
                clear();
        }

        // ???????? ????? ?????? ???????? ?? ?????????
        T* newObject()
        {
            if (!m_pBlocks)
            {
                m_pBlocks = newBlock();
                m_pCurrentBlk = m_pBlocks;
                m_pCurrentBlk->usedCount++;
                m_pCurrentBlk->firstFreeIndex++;
                memset(reinterpret_cast<void*>(m_pCurrentBlk->pdata), 0, sizeof(T));
                ::new(reinterpret_cast<void*>(m_pCurrentBlk->pdata)) T;
                return m_pCurrentBlk->pdata;
            }
            if (m_pCurrentBlk->firstFreeIndex == -1)
            {
                m_pCurrentBlk->pnext = newBlock();
                m_pCurrentBlk = m_pCurrentBlk->pnext;
                m_pCurrentBlk->usedCount++;
                m_pCurrentBlk->firstFreeIndex++;
                memset(reinterpret_cast<void*>(m_pCurrentBlk->pdata), 0, sizeof(T));
                ::new(reinterpret_cast<void*>(m_pCurrentBlk->pdata)) T;
                return m_pCurrentBlk->pdata;
            }
            T* object = &(m_pCurrentBlk->pdata[m_pCurrentBlk->firstFreeIndex]);
            int *newFirstFreeIndex = reinterpret_cast<int *>(&(m_pCurrentBlk->pdata[m_pCurrentBlk->firstFreeIndex]));
            m_pCurrentBlk->firstFreeIndex = *newFirstFreeIndex;
            m_pCurrentBlk->usedCount++;
            memset(reinterpret_cast<void*>(object), 0, sizeof(T));
            ::new(reinterpret_cast<void*>(object)) T;
            return object;
        }

        // ?????????? ??????? ? ?????????
        bool deleteObject(T* p)
        {
        }

        // ??????? ??????, ??????? ?? m_isDeleteElementsOnDestruct
        void clear()
        {
            if (!m_pBlocks)
                return;
            block *p_block = m_pBlocks;
            block *p_nextblock = p_block->pnext;
            while (p_nextblock != 0)
            {
                deleteBlock(p_block);
                p_block = p_nextblock;
                p_nextblock = p_block->pnext;
            }
            deleteBlock(p_block);
        }
    private:

        // ??????? ????? ???? ??????. ??????????? ? newObject
        block* newBlock()
        {
            block* p_block = new block{0, 0, 0, 0};
            p_block->pdata = (T*) malloc(m_blkSize * sizeof(T));
            for (int i = 0; i < m_blkSize; i++)
            {
                memset(reinterpret_cast<void*>(&(p_block->pdata[i])), 0, sizeof(T));
            }
            int *pointer;
            for (int i = 0; i < m_blkSize-1; i++)
            {
                pointer = reinterpret_cast<int*>(&(p_block->pdata[i]));
                *pointer = i+1;
            }
            pointer = reinterpret_cast<int*>(&(p_block->pdata[m_blkSize-1]));
            *pointer = -1;
            return p_block;
        }

        // ?????????? ?????? ????? ??????. ??????????? ? clear
        void deleteBlock(block *p)
        {
            if (m_isDeleteElementsOnDestruct)
            {
                while (p->firstFreeIndex != -1)
                {
                    int tmp_index = *reinterpret_cast<int *>(&(p->pdata[p->firstFreeIndex]));
                    memset(reinterpret_cast<void*>(&p->pdata[p->firstFreeIndex]), 0, sizeof(T));
                    ::new(reinterpret_cast<void*>(&p->pdata[p->firstFreeIndex])) T;
                    p->firstFreeIndex = tmp_index;
                }
                for (int i = 0; i < m_blkSize; i++)
                {
                    (&(p->pdata[i]))->~T();
                    memset(reinterpret_cast<void*>(&(p->pdata[i])), 0, sizeof(T));
                }
                free(p->pdata);
                delete p;
                return;
            }
            if (p->firstFreeIndex != -1)
                throw std::runtime_error("Error in deleteBlock().\nBlock isn't empty.\n");
            free(p->pdata);
            delete p;
        }

        // ?????? ?????
        int m_blkSize;
        // ?????? ?????? ??????
        block* m_pBlocks;
        // ??????? ????
        block *m_pCurrentBlk;
        // ??????? ?? ???????? ??? ????????????
        bool m_isDeleteElementsOnDestruct;
    };
} // namespace lab618

#endif // #define MEMORY_MANAGER_HEAD_H_2021_02_18
