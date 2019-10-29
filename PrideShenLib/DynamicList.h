#ifndef DYNAMICLIST_H
#define DYNAMICLIST_H

#include "SeqList.h"
#include "Exception.h"

namespace PrideShenLib
{

template < typename T >
class DynamicList : public SeqList<T>
{
protected:
    int m_capacity;
public:
    DynamicList(int capacity);
    int capacity() const;
    /*重新设置顺序空间的大小*/
    void resize(int capacity);
    ~DynamicList();
};

template < typename T >
DynamicList<T>::DynamicList(int capacity)
{
    this->m_array = new T[capacity];

    if( this->m_array != NULL )
    {
        this->m_length = 0;
        this->m_capacity = capacity;
    }
    else //构造函数能抛异常？？
    {
        THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create DynamicList object ...");
    }
}

template < typename T >
int DynamicList<T>::capacity() const
{
    return m_capacity;
}

template < typename T >
void DynamicList<T>::resize(int capacity)
{
    if( capacity != m_capacity ) //只有当需要重新设置顺序存储空间的大小与当前的存储空间大小不同时重新设置，一样的时候不需要重置，因为没有意义。
    {
        T* array = new T[capacity]; //思考：为啥没有直接操作m_array，而是重新定义了一个指针变量？是因为重置后的空间需要之前空间的元素进行赋值啊即array[i]=m_array[i]

        if( array != NULL )
        {
            int length = (this->m_length < capacity ? this->m_length : capacity);

            for(int i=0; i<length; i++)
            {
                array[i] = this->m_array[i];
            }

            T* temp = this->m_array;

            this->m_array = array;
            this->m_length = length;
            this->m_capacity = capacity;

            delete[] temp; //temp释放的是重置前的顺序存储空间了。
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No memory to resize DynamicList object ...");
        }
    }
}

template < typename T >
DynamicList<T>::~DynamicList()
{
    delete[] this->m_array;
}

}

#endif // DYNAMICLIST_H
/*构造函数抛异常的问题*/ //答案：可能是因为异常安全了，如果有内存问题可以通过智能指针销毁
