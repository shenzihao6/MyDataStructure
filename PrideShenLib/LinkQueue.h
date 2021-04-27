#ifndef LINKQUEUE_H
#define LINKQUEUE_H

#include "Queue.h"
#include "LinuxList.h"
#include "Exception.h"

namespace PrideShenLib
{

template < typename T >
class LinkQueue : public Queue<T>
{
protected:
    struct Node : public Object
    {
        list_head head;
        T value;
    };

    list_head m_header;
    int m_length = 0;

public:
    LinkQueue() //O(1)
    {
        m_length = 0;

        INIT_LIST_HEAD(&m_header);
    }

    void add(const T& e) // O(1)
    {
        Node* node = new Node();

        if( node != nullptr )
        {
            node->value = e;

            list_add_tail(&node->head, &m_header);  //O(1)

            m_length++;
        }
        else
        {
            THROW_EXCEPTION(InvalidOperatorException, "No memory to add new element ...");
        }
    }

    void remove()        //O(1)
    {
        if( m_length > 0 )
        {
            list_head* toDel = m_header.next;

            list_del(toDel);

            m_length--;

            delete list_entry(toDel, Node, head);
        }
        else
        {
            THROW_EXCEPTION(InvalidOperatorException, "No element in current queue ...");
        }
    }

    T front() const      //O(1)
    {
        if( m_length > 0 )
        {
            return list_entry(m_header.next, Node, head)->value;
        }
        else
        {
            THROW_EXCEPTION(InvalidOperatorException, "No element in current queue ...");
        }
    }

    void clear()      //O(n)
    {
        while( m_length > 0 )
        {
            remove();
        }
    }

    int length() const   //O(1)
    {
        return m_length;
    }

    ~LinkQueue()     //O(n)
    {
        clear();
    }
};

}
#endif // LINKQUEUE_H
