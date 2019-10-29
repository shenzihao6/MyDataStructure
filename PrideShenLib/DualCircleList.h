#ifndef DUALCIRCLELIST_H
#define DUALCIRCLELIST_H

#include "DualLinkList.h"
#include "LinuxList.h"

namespace PrideShenLib
{

template < typename T >
class DualCircleList : public DualLinkList<T>
{
protected:
    struct Node : public Object
    {
        list_head head;
        T value;
    };

    list_head m_header;
    list_head* m_current;

    list_head* position(int i) const
    {
        list_head* ret = const_cast<list_head*>(&m_header);

        for(int p=0; p<i; p++)
        {
            ret = ret->next;
        }

        return ret;
    }

    int mod(int i) const
    {
        return (this->m_length == 0) ? 0 : (i % this->m_length);
    }

public:
    DualCircleList()
    {
        INIT_LIST_HEAD(&m_header);

        m_current = NULL;
        this->m_length = 0;
        this->m_step = 1;

    }

    bool insert(const T& e)
    {
        return insert(this->m_length, e);
    }

    bool insert(int i, const T& e)
    {
        bool ret = true;
        Node* node = new Node();


        i = i % (this->m_length + 1);

        if( node != NULL)
        {
            node->value = e;

            list_add_tail(&node->head, position(i)->next);

            this->m_length++;
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No Memory to insert new element");
        }

        return ret;
    }

    bool remove(int i)
    {
        bool ret = true;

        i = mod(i);//因为可能会循环的访问,所以i会非常大

        ret = ( (0 <= i) && (i < this->m_length) );//为啥要有这个判断？？

        if( ret )
        {
            list_head* toDel = position(i)->next;

            if( m_current == toDel )
            {
                m_current = toDel->next;
            }

            list_del(toDel);


            this->m_length--;

            delete list_entry(toDel, Node, head);//指向的是是结点中的heade成员，所以我们要将它转换成真正的Node类型结点，用list_entry宏.
        }

        return ret;
    }

    bool set(int i, const T& e)
    {
        bool ret = true;
        i = mod(i);

        ret = ( (0 <= i) && (i < this->m_length) );

        if( ret )
        {
            list_entry(position(i)->next, Node, head)->value = e;
        }

        return ret;
    }

    T get(int i) const
    {
        T ret;

        if( get(i, ret) )
        {
            return ret;
        }
        else
        {
            THROW_EXCEPTION(IndexOutOfBoundsException, "Invalid parameter i to get element ...");
        }
    }

    bool get(int i, T& e) const
    {
        bool ret = true;
        i = mod(i);
        ret = ( (0 <= i) && (i < this->m_length) );

        if( ret )
        {
            e = list_entry(position(i)->next, Node, head)->value;
        }

        return ret;
    }

    int find(const T& e) const
    {
        int ret = -1;
        int i = 0;
        list_head* slider = NULL;

        list_for_each(slider, &m_header)
        {
            if( list_entry(slider, Node, head)->value == e )
            {
                ret = i;
                break;
            }

            i++;
        }

        return ret;
    }

    int length() const
    {
        return this->m_length;
    }

    void clear()
    {
        while (this->m_length > 0)//不需要考虑m_current的问题吗
        {
            remove(0);//不影响效率吗
        }
    }

    bool move(int i, int step = 1)
    {
        bool ret = ( step > 0 );

        i = mod(i);

        ret = ret && ( (0 <= i) && (i < this->m_length) );


        if( ret )
        {
            m_current = position(i)->next;

            this->m_step = step;
        }

        return ret;
    }

    bool end()
    {
        return ( m_current == NULL ) || (this->m_length == 0);
    }

    T current()
    {
        if( !end() )
        {
            return list_entry(m_current, Node, head)->value;
        }
        else
        {
            THROW_EXCEPTION(InvalidOperatorException, "No valid at current position");
        }
    }

    bool next()
    {
        int i = 0;

        while( (i < this->m_step) && !end() )
        {
            if( m_current != &m_header )
            {
                m_current = m_current->next;
                i++;
            }
            else
            {
                m_current = m_current->next;
            }
        }

        if( m_current == &m_header )
        {
            m_current = m_current->next;
        }

        return (i == this->m_step);
    }

    bool pre()
    {
        int i = 0;

        while( (i < this->m_step) && !end() )
        {
            if( m_current != &m_header )
            {
                m_current = m_current->prev;
                i++;
            }
            else
            {
                m_current = m_current->prev;
            }
        }

        if( m_current == &m_header )
        {
            m_current = m_current->prev;
        }

        return (i == this->m_step);
    }

    ~DualCircleList()
    {
        clear();
    }
};

}

#endif // DUALCIRCLELIST_H
