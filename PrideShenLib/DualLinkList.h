#ifndef DUALLINKLIST_H
#define DUALLINKLIST_H

#include "List.h"
#include "Exception.h"

namespace PrideShenLib
{

template < typename T >
class DualLinkList : public List<T>
{
protected:
    struct Node : public Object
    {
        T value;
        Node* next;
        Node* pre;
    };

    mutable struct : public Object
    {
        char reserved[sizeof(T)];
        Node* next;
        Node* pre;
    } m_header;

    int m_length;
    Node* m_current;
    int m_step;

    Node* position(int i) const;

    virtual Node* create();
    virtual void destroy(Node* pn);
public:
    DualLinkList();
    ~DualLinkList();
    int find(const T& e) const;
    bool insert(const T& e);
    bool insert(int i, const T& e);
    bool remove(int i);
    bool set(int i, const T& e);
    virtual T get(int i) const;
    bool get(int i, T& e) const;
    int length() const;
    void clear();

    virtual bool move(int i, int step = 1);
    virtual bool end();
    virtual T current();
    virtual bool next();
    virtual bool pre();

};

template < typename T >
typename DualLinkList<T>::Node* DualLinkList<T>::create()
{
    return new Node();
}

template < typename T >
void DualLinkList<T>::destroy(Node* pn)
{
    delete pn;
}

template < typename T >
bool DualLinkList<T>::move(int i, int step)//要实现后面的版本吗
{
    bool ret = ( (0 <= i) && (i < m_length) && (step > 0) );

    if( ret )
    {
        m_current = position(i)->next;
        m_step = step;
    }

    return ret;
}

template < typename T >
bool DualLinkList<T>::end()
{
    return (m_current == nullptr);
}

template < typename T >
T DualLinkList<T>::current()
{
    if( !end() )
    {
        return m_current->value;
    }
    else
    {
        THROW_EXCEPTION(InvalidOperatorException, "No valid at current position");
    }
}

template < typename T >
bool DualLinkList<T>::next()
{
    int i = 0;

    while ( (i < m_step) && !end() )
    {
        m_current = m_current->next;
        i++;
    }

    return ( i == m_step );
}

template < typename T >
bool DualLinkList<T>::pre()
{
    int i = 0;

    while ( (i < m_step) && (!end()) )
    {
        m_current = m_current->pre;
        i++;
    }

    return ( i == m_step );
}


template < typename T >
int DualLinkList<T>::find(const T& e) const
{
    int ret = -1;
    int i = 0;
    Node* node = m_header.next;

    while (node)
    {
        if( node->value == e )
        {
            ret = i;
            break;
        }
        else
        {
            node = node->next;

            i++;
        }
    }

    return ret;
}


template < typename T >
typename DualLinkList<T>::Node* DualLinkList<T>::position(int i) const
{
    Node* ret = reinterpret_cast<Node*>(&m_header);

    for(int p=0; p<i; p++)
    {
        ret = ret->next;
    }

    return ret;
}

template < typename T >
DualLinkList<T>::DualLinkList()
{
    m_header.next = nullptr;
    m_header.pre = nullptr;
    m_length = 0;
    m_step = 1;
    m_current = nullptr;
}

template < typename T >
bool DualLinkList<T>::insert(const T &e)
{
    return insert(m_length, e);
}

template < typename T >
bool DualLinkList<T>::insert(int i, const T& e)
{
    bool ret = ( (0 <= i) && (i <= m_length) );

    if( ret )
    {
        Node* node = create();

        if( node != nullptr )
        {
            Node* current = position(i);
            Node* next = current->next;//课本上是next可以用current->next来代替，但是这样的话插入先断next指针的话，就不知道next的位置
                                       //如果选择先断pre链的话就不用这个next标识current->next指针的位置
            node->value = e;

            node->next = next;//原来这里是current->next全部用next代替了，如果后面用到了很多的current->next，这样做确实会直观很多
            current->next = node;

            if( current != reinterpret_cast<Node*>(&m_header) )
            {
                node->pre = current;
            }
            else
            {
                node->pre = nullptr;
            }

            if( next != nullptr )//插入到最后位置
            {
                next->pre = node;
            }

            m_length++;
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No memory to insert new element...");
        }
    }

    return ret;
}

template < typename T >
bool DualLinkList<T>::remove(int i)
{
    bool ret = ( (0 <= i) && (i < m_length) );

    if( ret )
    {
        Node* current = position(i);
        Node* toDelete = current->next;
        Node* next = toDelete->next;

        if( m_current == toDelete )
        {
            m_current = next;//代码复用，next直接替换了toDelete->next，如果后面用到了很多的toDelete->next，这样做确实会直观很多
        }

        current->next = next;

        if( next != nullptr )//删除最后一个结点
        {
            //next->pre = current;//好像换成current也可以啊
            next->pre = toDelete->pre;
        }

        m_length--;

        delete toDelete;
    }

    return ret;
}

template < typename T >
bool DualLinkList<T>::set(int i, const T& e)
{
    bool ret = ( (0 <= i) && (i < m_length) );

    if( ret )
    {
        position(i)->next->value = e;
    }

    return ret;
}

template < typename T  >
T DualLinkList<T>::get(int i) const
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

template < typename T >
bool DualLinkList<T>::get(int i, T& e) const
{
    bool ret = ( (0 <= i) && (i < m_length) );

    if( ret )
    {
        e = position(i)->next->value;
    }

    return ret;
}

template < typename T >
int DualLinkList<T>::length() const
{
    return m_length;
}

template < typename T >
void DualLinkList<T>::clear()
{
    while( m_length > 0 )
    {
        remove(0);
    }
}

template < typename T >
DualLinkList<T>::~DualLinkList()
{
    clear();
}

}

#endif // DUALLINKLIST_H
