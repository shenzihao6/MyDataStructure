#ifndef STATICQUEUE_H
#define STATICQUEUE_H

#include "Queue.h"
#include "Exception.h"

namespace PrideShenLib
{
template < typename T, int N >
class StaticQueue : public Queue<T>
{
protected:
    T m_space[N];
    int m_fornt;
    int m_rear;
    int m_length;

public:
    StaticQueue()
    {
        m_fornt = 0;
        m_rear = 0;
        m_length = 0;
    }

    int capacity() const
    {
        return N;
    }

    void add(const T& e) //O(1)
    {
        if( m_length < N )
        {
            m_space[m_rear] = e;
            m_rear = (m_rear + 1) % N;
            m_length++;
        }
        else
        {
            THROW_EXCEPTION(InvalidOperatorException, "No space in current queue ...");
        }
    }

    void remove() //O(1)
    {
        if( m_length > 0 )
        {
            m_fornt = (m_fornt + 1) % N;
            m_length--;
        }
        else
        {
            THROW_EXCEPTION(InvalidOperatorException, "No element in current queue ...");
        }
    }

    T front() const  //O(1)
    {
        if( m_length > 0 )
        {
            return m_space[m_fornt];
        }
        else
        {
            THROW_EXCEPTION(InvalidOperatorException, "No element in current queue ...");
        }
    }

    void clear() //O(1)
    {
        m_fornt = 0;
        m_rear = 0;
        m_length = 0;
    }

    int length() const  //O(1)
    {
        return m_length;
    }
};

}

#endif // STATICQUEUE_H
