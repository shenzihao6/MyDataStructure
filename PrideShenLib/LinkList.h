#ifndef LINKLIST_H
#define LINKLIST_H

#include "List.h"
#include "Exception.h"

namespace PrideShenLib
{

template < typename T >
class LinkList : public List<T>
{
protected:
    struct Node : public Object //Node不是模板的原因是tempate <typenam T>定义之后的那个类才能是模板类，在这个模板类中再次定义类的话都
                                //不会是模板类，类比的看下 template <typename T> class Blob{
                                // template <typename It> Blob(It b, It b)}这样的话可以知道Blob是类模板，参数It是模板的另一个参
                                //数类型，它们都紧挨着template的下面，另外一个例子 template <typename T> template <typename It>
                                //Blot<T>::Blot(It b, It e)这种紧挨着两个template下面的，那么这个类是模板类，参数是It的模板参数
    {
        T value;
        Node* next;
    };

    mutable struct : public Object
    {
        char reserved[sizeof(T)];//我们是想让此类中内存的布局和Node的布局一样，所以做的这步操作，这样T value的内存布局和reserved布局相同了，这里的内存布局不仅仅限定于成员变量，还有一些局部变量的信息等等
        Node* next;
    } m_header;

    //mutable Node m_header; //Node不加<>实参，编译器会认为我们使用的类型和模板参数一致，这样做显然符合逻辑了
    int m_length;
    /*定义游标*/
    Node* m_current;
    int m_step; //记录游标每次移动的数目

    Node* position(int i) const; //因为mutable的原因const版本的函数返回值依然可用是Node*，所以const和非const版本的函数是一样的，所以定义成一个const函数版本

    virtual Node* create();
    virtual void destroy(Node* pn);
public:
    LinkList();
    ~LinkList();
    int find(const T& e) const;
    bool insert(const T& e);
    bool insert(int i, const T& e);
    bool remove(int i);
    bool set(int i, const T& e);
    virtual T get(int i) const;//此get函数是在LinkList中重载的，而不是在List中重载的
    bool get(int i, T& e) const; //const对象是不能修改成员变量的值的，可能是不需要修改成员变量,所以可以声明成const，其它的抽象函数不满足要求
    int length() const; //const对象是不能修改成员变量的值的，可能是不需要修改成员变量,所以可以声明成const，其它的抽象函数不满足要求
    void clear();
    /*关于游标的共有函数*/
    virtual bool move(int i, int step = 1);
    virtual bool end();
    virtual T current();
    virtual bool next();
    /*
    T& operator[] (int i);
    T operator[] (int i) const;
    */
};

template < typename T >
typename LinkList<T>::Node* LinkList<T>::create()
{
    return new Node();
}

template < typename T >
void LinkList<T>::destroy(Node* pn)//这里的Node* 不要加typenam和作用域限定符，可能是识别到了destory函数也就识别了Node类型
{
    delete pn;
}

template < typename T >
bool LinkList<T>::move(int i, int step)
{
    bool ret = ( (0 <= i) && (i < m_length) && (step > 0) );

    if( ret )
    {
        m_current = position(i)->next;//m_current定位到目标位置处
        m_step = step;
    }

    return ret;
}

template < typename T >
bool LinkList<T>::end()
{
    return (m_current == NULL);
}

template < typename T >
T LinkList<T>::current()//不返回T的引用合理吗？
{
    if( !end() )//==(m_current != NULL)//等价于当前的遍历没有结束，在遍历中貌似这个条件(!end())没作用，但这个条件是有必要的，万一你误用current呢了
    {
        return m_current->value;
    }
    else
    {
        THROW_EXCEPTION(InvalidOperatorException, "No valid at current position");
    }
}

template < typename T >
bool LinkList<T>::next()
{
    int i = 0;

    while ( (i < m_step) && !end() )//循环条件必须很严格,循环m_step次，并且循环过程中游标不为NULL,不加!end()条件其实也可以，但是这样做不严格
                                    //就类似于多做了几次NULL = NULL，而此时i肯定是等于m_step
    {
        m_current = m_current->next;
        i++;
    }

    return ( i == m_step );
}


template < typename T >
int LinkList<T>::find(const T& e) const
{
    int ret = -1;
    int i = 0;
    Node* node = m_header.next;

    while (node) //因为不知道循环体的长度，所以用while来进行
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
    /*这样做时间复杂度太低，也有可能是错的，因为学着老师的做法，看到后面老师游标的做法
    for(int i=0; i<length(); i++)
    {
        if( e == position(i)->next->value )
        {
            ret = position(i)->next->value;
            break;
        }
    }
    */

    return ret;
}


template < typename T >
typename LinkList<T>::Node* LinkList<T>::position(int i) const//模板类中的某个“类型”（class或者typedef都算）表示的时候要在前面加typename
                                                        //，不然编译器会认为它是成员变量或者成员函数,因此Node不是模板类型
{
    Node* ret = reinterpret_cast<Node*>(&m_header);

    for(int p=0; p<i; p++)//目标位置为i, current就移动i次，到达地下标i-1的结点
    {
        ret = ret->next;
    }

    return ret;
}

template < typename T >
LinkList<T>::LinkList()
{
    m_header.next = NULL;//之前的元素有没有被销毁，不是我们在创建的时候考虑的，创建的时候，本来链表就是空的
    m_length = 0;
    m_step = 1;
    m_current = NULL;
}

template < typename T >
bool LinkList<T>::insert(const T &e)
{
    return insert(m_length, e);
}

template < typename T >
bool LinkList<T>::insert(int i, const T& e)
{
    bool ret = ( (0 <= i) && (i <= m_length) );

    if( ret )
    {
        Node* node = create();

        if( node != NULL )
        {
            /*
            Node* current = reinterpret_cast<Node*>(&m_header);

            for(int p=0; p<i; p++)
            {
                current = current->next;
            }
            */

            Node* current = position(i);

            node->value = e;
            node->next = current->next;

            current->next = node;

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
bool LinkList<T>::remove(int i)
{
    bool ret = ( (0 <= i) && (i < m_length) );

    if( ret )
    {
        /*
        Node* current = reinterpret_cast<Node*>(&m_header);//如果这里直接header = header.next,那不就相当于改变头结点了吗，
                                                            //那要再进行一次插入的话，头结点位置不就改变了吗？？所以不能直接改
                                                            //变header,这跟clear那里不同，clear的话要从header开始从第一个
                                                            //位置一个一个清除掉结点，所以可以直接从头结点开始直接删除掉
        for(int p=0; p<i; p++)//以头结点为起始处定位位置，由于头结点的辅助，定位到哪个位置就循环多少次
        {
            current = current->next;
        }
        */

        Node* current = position(i);
        Node* toDelete = current->next;

        if( m_current == toDelete )
        {
            m_current = toDelete->next;
        }

        current->next = toDelete->next;

        m_length--;

        delete toDelete;//这里可能不要考虑异常，因为Node类型已知道，是我们自己定义的，析构函数有没有抛出异常我们是能够事先知道的
    }

    return ret;
}

template < typename T >
bool LinkList<T>::set(int i, const T& e)
{
    bool ret = ( (0 <= i) && (i < m_length) );

    if( ret )
    {
        /*
        Node* current = reinterpret_cast<Node*>(&m_header);

        for(int p=0; p<i; p++)
        {
            current = current->next;
        }

        current->next->value = e;
        */

        position(i)->next->value = e;
    }

    return ret;
}

template < typename T  >
T LinkList<T>::get(int i) const
{
    T ret; //由于不清楚T类型，所以ret可以不赋初值

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
bool LinkList<T>::get(int i, T& e) const
{
    bool ret = ( (0 <= i) && (i < m_length) );

    if( ret )
    {
        /*
        Node* current = reinterpret_cast<Node*>(&m_header);//m_header声明为mutable是因为，在const成员函数中对m_header取地址，
                                  //编译器会认为你想要
                                  //修改m_header的值，所以编译器会报错，这个跟m_header是const变量差不多,不同的是const变量的话
                                  //current指向这个const变量的时候编译器不会报错，通过curent修改const变量的值也不会报错，只有我们
                                  //人为的修改了const变量的值，编译器才会报错，所以前者编辑器的行为更加严格
                                 //解决方案就是m_header加上mutable关键字，让const函数中的成员变量永远处于可改变的状态
                                  //const函数中的成员变量和const只读变量还有有区别的，前者编译器要求更加严格
        for(int p=0; p<i; p++)
        {
            current = current->next;
        }

        e = current->next->value;
        */

        e = position(i)->next->value;
    }

    return ret;
}

template < typename T >
int LinkList<T>::length() const
{
    return m_length;
}

template < typename T >
void LinkList<T>::clear()
{
    while( m_header.next )//因为这个时候链表已经有全部的元素了，所以就直接从头结点开始一个个的删除元素
    {
        Node* toDelete = m_header.next;

        m_header.next = toDelete->next;//不用node指向m_header吗

        m_length--;

        destroy(toDelete);
    }
}
/*
template < typename T >
T& LinkList<T>::operator[] (int i)
{

}
template < typename T >
T LinkList<T>::operator[] (int i) const
{

}
*/
template < typename T >
LinkList<T>::~LinkList()//如果单链表需要销毁，那在销毁单链表对象前，先将单链表对象当中每一个结点给删除，最后摧毁当前的单链表对象，所以直接调用clear()函数
{
    clear();
}

}

#endif // LINKLIST_H
