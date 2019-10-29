#ifndef STATICLIST_H
#define STATICLIST_H

#include "SeqList.h"

namespace PrideShenLib
{

template <typename T, int N>
class StaticList : public SeqList<T>
{
    T m_space[N];

public:
    StaticList()
    {
        this->m_array = m_space;//模板类中如果要表示父类的成员就要加上this
        this->m_length = 0;
    }

    int capacity() const
    {
        return N;
    }
};

}

#endif // STATICLIST_H
