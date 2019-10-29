#ifndef SHAREDPOINTER_H
#define SHAREDPOINTER_H

#include "Pointer.h"
#include "Exception.h"
#include "cstdlib"

using namespace std;

namespace PrideShenLib
{

template < typename T >
class SharedPointer : public Pointer<T>
{
protected:
    int* m_ref;

    void assign(const SharedPointer<T>& obj)
    {
        this->m_pointer = obj.m_pointer;
        m_ref = obj.m_ref;

        if( m_ref )//这一步其实是判断了当前对象和obj对象申请堆空间的p和m_ref都合法时才进行m_ref++;
        {
            (*m_ref)++;
        }
    }

public:
    SharedPointer(T* p = NULL) : m_ref(NULL)
    {
        if( p )
        {
            m_ref = static_cast<int*>(malloc(sizeof(int)));

            if( m_ref )
            {
                this->m_pointer = p;

                *m_ref = 1;
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create SharedPointer object ...");
            }
        }
    }

    SharedPointer(const SharedPointer<T>& obj) : Pointer<T>(NULL)
    {
        assign(obj);
    }

    SharedPointer<T>& operator =(const SharedPointer<T>& obj)
    {
        if( this != &obj)
        {
            clear();

            assign(obj);
        }

        return *this;
    }

    void clear()
    {
        /*我的这种写法不能保证在构造函数遗漏的p为NULL的情况，m_pointer和m_ref为随机值，拷贝构造中m_ref和m_pointer被拷贝为随机值的情况
        可能老师没我想的那么复杂哈哈
        if( m_ref )
        {
            (*m_ref)--;

            if( *m_ref == 0 )
            {
                free(this->m_pointer);//而且在这里也不能这么写
                free(m_ret);
                this->m_pointer = NULL;
                m_ref = NULL;
            }
        }
        */
        T* toDel = this->m_pointer;
        int* ref = this->m_ref;

        this->m_pointer = NULL;
        this->m_ref = NULL;

        if( ref )
        {
            (*ref)--;

            if( *ref == 0 )
            {
                delete(toDel);
                free(ref);
            }
        }
    }

    ~SharedPointer()
    {
        clear();
    }
};

template < typename T >//通过全局函数的形式重载操作符
bool operator == (const SharedPointer<T>& l, const SharedPointer<T>& r)
{
    return ( l.get() == r.get() );
}

template < typename T >//通过全局函数的形式重载操作符
bool operator != (const SharedPointer<T>& l, const SharedPointer<T>& r)
{
    return !( l == r );//利用相等的全局重载函数来重载不等的重载操作符
}


}

#endif // SHAREDPOINTER_H
