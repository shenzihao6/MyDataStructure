#ifndef CIRCLELIST_H
#define CIRCLELIST_H

#include "LinkList.h"

namespace PrideShenLib
{

template < typename T >
class CircleList : public LinkList<T>
{
    typedef typename LinkList<T>::Node Node;//使用父类成员有模板参数要指明LinkList<T>,但是编译器不知道Node到底是静态变量还是类型，
                                            //所以需要typename来指明类型
protected:

    int mod(int i) const
    {
        return (this->m_length == 0) ? 0 : (i % this->m_length);
    }

    Node* last() const
    {
            //return  LinkList<T>::position(this->m_length-1)->next;
        return this->position(this->m_length-1)->next;//使用父类成员有模板参数要指明LinkList<T>,但是position定义时没有用到模板参数
                                                      //所以只能用this指针访问父类成员这行代码后面可能会用到，而且虽然代码只有一行，
                                                      //但代码太长可读性不好，所以重构了一下
    }

    void last_to_first() const
    {
        last()->next = this->m_header.next;
    }

public:
    bool insert(const T& e)
    {
        return insert(this->m_length, e);
    }

    bool insert(int i, const T& e)
    {
        bool ret = true;

        i = i % (this->m_length + 1); //因为可能会循环的访问,所以i会非常大 为啥+1？ 因为可以在最后一个元素后面可以插入
                                      //所以规定最后一个元素后面的一个元素才会第0个结点元素的位置，同时保证了m_length永不等于0

        ret = LinkList<T>::insert(i, e);//因为insert的父类包含模板参数了，所以需要LinkList<T>来访问insert函数

        if( ret && (i == 0) )
        {
            last_to_first();
        }

        return ret;
    }

    bool remove(int i)
    {
        bool ret = true;

        i = mod(i);//因为可能会循环的访问,所以i会非常大

        if( i == 0 )
        {
            Node* toDel = this->m_header.next;

            if( toDel != nullptr )//因为不像linkList中，有i关于m_length的合法性判断
            {
                this->m_header.next = toDel->next;

                this->m_length--;

                if( this->m_length > 0 )
                {
                    last_to_first();

                if( this->m_current == toDel )
                {
                        this->m_current = toDel->next;
                    }
                }
                else
                {
                    this->m_header.next = nullptr;//这里不清空的话，头结点就指向自己了
                    this->m_current = nullptr;
                }

                this->destroy(toDel);
            }
            else
            {
                ret = false;
            }
        }
        else
        {
            ret = LinkList<T>::remove(i);//因为insert的父类包含模板参数了，所以需要LinkList<T>来访问insert函数
        }

        return ret;
    }

    bool set(int i, const T& e)
    {
        return LinkList<T>::set(mod(i), e);//由于是循环链表，所以i会特别大，会用mod取余
    }

    bool get(int i, const T e) const//因为相对父类改变了，所以要重写
    {
        return LinkList<T>::get(mod(i), e);
    }

    T get(int i) const//因为相对父类改变了，所以要重写
    {
        return LinkList<T>::get(mod(i));
    }

    int find(const T& e) const//因为相对父类改变了，所以要重写
    {
            /*这样搞的确很精妙但是却是不对的，因为这样改的话，它就是一个单链表了啊，所以要再次还原成循环链表才行
             *所以调用last_to_first函数，但是有一个问题，find函数中==操作符是我们自己重载的，万一我们一不小心
             * 在==操作符抛出了异常的话，last_to_first就调用不到了啊,循环链表就变成单链表了啊 ，改变了链表的状态
             * 了，所以就不是异常安全的了。有同学肯定会说，好解决啊，我们在这里调用try catch啊？但是我们在开发的时候
             * 已经声明了，在实现的时候不允许使用try catch，因为我们需要更好的移植性，所以不使用try catch,所以我们
             * 只能重新实现find函数了
            int ret = -1;

            last()->next = nullptr;

            ret = LinkList<T>::find(e);

            last_to_first();

            return ret;
            */

        int ret = -1;
        Node* slider = this->m_header.next;

        for(int i=0; i<this->m_length; i++)
        {
            if( slider->value == e )
            {
                ret = i;
                break;
            }

            slider = slider->next;
        }

        return ret;
    }

    void clear()
    {
        /*这样子实现也是不可以的，道理是和find函数是类似的,这是这里 抛出异常的可能是clear函数内的destroy函数，清空第一个元素
        *的时候就有可能发生异常，那么循环链表的性质就会发生改变了
        if( this->m_length > 0 )
        {
            last()->next = nullptr;

            LinkList<T>::clear();

            last_to_first();

        }
        */
        while( this->m_length > 1 )
        {
        /*这里为啥remove(1)而不是remove(0)，是因为效率，如果说是remove(0)也没啥错，如果说是remove(0)，也就是每次都要调用last_to_first()
            而last_to_first()函数的内部要调用last函数，而last函数要调用position函数，所以说我们remove(0)的话就会导致大量的移动，如果说这地方是
            remove(1),也就是说会走到else分支，避开了0,直接调用LinkList<T>::remove函数了，效率就极大的提高了，一直remove到只有首结点的时候
                               有时候我们可以通过多写代码来提高程序的效率，这便是空间换时间的思想

        */
            remove(1);
        }

        if( this->m_length == 1)
        {
            Node* toDel = this->m_header.next;

            this->m_header.next = nullptr;
            this->m_length = 0;
            this->m_current = nullptr;

            this->destroy(toDel);
        }
    }

    bool move(int i, int step)
    {
        return LinkList<T>::move(mod(i), step);
    }

    bool end()
    {
        return (this->m_length == 0) || (this->m_current == nullptr);//this->m_length==0这个限定条件是因为有可能头结点next指向头结点本身
    }

    ~CircleList()
    {
        clear();
    }
};

}















































#endif // CIRCLELIST_H
