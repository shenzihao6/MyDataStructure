#ifndef STATICSTACK_H
#define STATICSTACK_H

#include "Stack.h"
#include "Exception.h"

namespace PrideShenLib
{

template < typename T , int N >
class StackStack : public Stack<T>
{
protected:
    T m_space[N];
    int m_top;
    int m_size;

public:
    StackStack()
    {
        m_top = -1;
        m_size = 0;
    }

    void push(const T& e)
    {
        if( m_size < N )
        {
            m_space[m_top + 1] = e;//为了异常安全，万一e和m_space是类类型，赋值操作符就有可能抛出异常，我们要保证即便发生了异常，栈它内部的状态不能
                                  //发生改变,所以先赋值，成功之后，再让栈标识变量+1, 这里很重要;
            m_top++;
            m_size++;
        }
        else
        {
            THROW_EXCEPTION(InvalidOperatorException, "No space in current stack ...");
        }

    }

    void pop()
    {
        if( m_size > 0 )
        {
            m_top--;
            m_size--;
        }
        else
        {
            THROW_EXCEPTION(InvalidOperatorException, "No space in current stack ...");
        }
    }

    T top() const
    {
        if( m_size > 0 )//这个判断自己的写的时候没有注意
        {
            return m_space[m_size-1];
        }
        else
        {
            THROW_EXCEPTION(InvalidOperatorException, "No space in current stack ...");
        }
    }

    void clear()
    {
        m_size = 0;
        m_top = -1;
    }

    int size() const
    {
        return m_size;
    }

    int capacity() const
    {
        return N;
    }

    ~StackStack()
    {

    }
};

}

#endif // STATICSTACK_H
