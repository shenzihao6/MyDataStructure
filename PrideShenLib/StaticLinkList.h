#ifndef STATICLINKLIST_H
#define STATICLINKLIST_H

#include "LinkList.h"
#include <typeinfo>
#include <iostream>

using namespace std;

namespace PrideShenLib
{

template < typename T, int N >
class StaticLinkList : public LinkList<T>
{
protected:
    typedef  typename LinkList<T>::Node Node;

    struct SNode : public Node
    {
        void* operator new(unsigned int size, void* loc)
        {
            (void)size;
            return loc;//只返回指定地址，不申请空间
        }
    };
    unsigned char m_space[sizeof(SNode) * N];//编译器没有办法知道这样访问到底是类型还是一个静态成员变量
                                                                         //所以编译器要报错，所以要加上typename关键字指明::后面是一个类型
                                                                         //因为Node类型里有一个类型是T类型，所以要指明LinkList<T>::Node，
                                                                         //如果这个类型里没有T，就直接Node类型就可以了因为需要指明Node类型
                                                                         //中的T是跟SaticLinkLsit中的T是一个类型
    int m_used[N];

    Node* create()
    {
        SNode* ret = NULL;

        for(int i=0; i<N; i++)
        {
            if( !m_used[i] )
            {
                m_used[i] = 1;

                ret = reinterpret_cast<SNode*>(m_space) + i;
                ret = new(ret)SNode();//在ret这段内存空间中调用SNode()构造函数,这里的new调用的SNode的重载版本的new
                break;
            }
        }

        return ret;
    }

    void destroy(Node* pn)
    {
        SNode* space = reinterpret_cast<SNode*>(m_space);
        SNode* psn = dynamic_cast<SNode*>(pn);//因为SNode继承自Node,所以父类中的Node类型转换为子类中的SNode类型，所以用dynamic_cast

        for(int i=0; i<N; i++)
        {
            if( psn == (space + i) )
            {
                m_used[i] = 0;

                psn->~SNode();//调用了Replacement new函数之后，就是要自己析构SNode

                break;
            }
        }
    }

public:
    StaticLinkList()
    {
        for(int i=0; i<N; i++)
        {
            m_used[i] = 0;
        }
    }

    int capacity()
    {
        return N;
    }

    ~StaticLinkList()
    {
        this->clear();//子类调用完析构函数的时候，父类调用析构函数中clear函数时，由于子类这里clear过了，所以
                 //调用父类clear函数时不会进入while循环，所以就不会调用父类的destroy函数
    }
};
}

#endif // STATICLINKLIST_H
