/**
  ******************************************************************************
  * @file           : queueEx.h
  * @brief          : queue expansion
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#ifndef QUEUEEX_h
#define QUEUEEX_h

#include "queue.h"

class QueueEx : public Queue{
private:
public:
    QueueEx(int iSize, int iCount) : Queue(iSize, iCount){
    }
    bool PeekAll(void* pvItem){
        unsigned char *pv = (unsigned char*)pvItem;
        unsigned char *p = m_pnRdIndex;
        int n = m_iLclCount;
        if (n > 0){
            do {
                memcpy( pv, p, m_iSize );
                pv += m_iSize;
                p += m_iSize;
                if ( p >= m_pnTail ) p = m_pnHead;
            } while(--n);
            return true;
        }
        return false;
    }
    bool Peek(int index, void* pvItem){
        unsigned char *p = m_pnRdIndex;
        if (index < m_iLclCount){
            for(int i = 0; i < index; i++){
                p += m_iSize;
                if ( p >= m_pnTail ) p = m_pnHead;
            }
            memcpy( pvItem, p, m_iSize );
            return true;
        }
        return false;
    }
};

#endif/*QUEUEEX_h*/

