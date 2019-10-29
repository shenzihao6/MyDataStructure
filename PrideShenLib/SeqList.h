#ifndef SEQLIST_H
#define SEQLIST_H

#include "List.h"
#include "Exception.h"

namespace PrideShenLib
{

template <typename T> //父类是模板，继承后的子类也要是模板
class SeqList : public List<T>
{
protected:
    T* m_array;
    int m_length;

public:
    int find(const T& e) const;
    bool insert(const T& e);
    bool insert(int i, const T& e);
    bool remove(int i);
    bool set(int i, const T& e);
    bool get(int i, T&e) const;//按照常规的首选的设计是将目标元素的值作为返回值返回，但是这里应该是将目标位置数据元素的值通过一个引用参数e来返回，
                                //这样设计的原因，目标位置处的i不合法,目标位置i不合法我们就返回false，那换句话说，get这个操作的返回值用来表示当
                                //前获取操作是否成功，如果成功，目标位置处的值就通过这个参数e返回了。
    int length() const;
    void clear();

    T& operator[] (int i);
    T operator[] (int i) const; //因为const成员函数内部才能调用const对象，且成员变量不能被修改，所以只返回类型，而不是引用，貌似是这样,如果返回引用编译器会报错貌似，因为你取址代表你想修改const成员函数的成员变量的值，编译器对它比const成员检查要严格，可能就会直接报错了

    virtual int capacity() const = 0; //这里其实也可以组合貌似，因为当代软件架构实践中的经验都是能用组合尽量不用继承，但是用组合实现了貌似更麻烦了，因为老师又说过是不是组合或者继承要跟生活直接映射才行
};

template < typename T >
int SeqList<T>::find(const T &e) const
{
    int ret = -1;

    for(int i=0; i<length(); i++)
    {
        if( m_array[i] == e )
        {
            ret = i;
            break;
        }
    }

    return ret;
}

template < typename T >
bool SeqList<T>::insert(int i, const T& e)
{
    bool ret = ( (0 <= i) && (i <= m_length) );
    ret = ret && ( m_length + 1 <= capacity() ); //这里的capacity可以在子类实现，但是现在却可以先使用一下先，这个技巧要记住

    if( ret )
    {
        for(int p=m_length-1; p>=i; p--)
        {
            m_array[p + 1] = m_array[p];
        }

        m_array[i] = e;
        m_length++;
    }

    return ret;
}

template < typename T >
bool SeqList<T>::insert(const T& e)
{
    return insert(this->m_length, e); //代码复用的思想，和在最后一个位置插入的方法，这个方法可以追溯到Qt模型视图设计模式那里
}


template <typename T>
bool SeqList<T>::remove(int i)
{
    bool ret = ( (0 <= i) && (i < m_length) );

    if( ret )
    {
        for(int p=i; p<m_length-1; p++)
        {
            m_array[p] = m_array[p+1];
        }

        m_length--;
    }

    return ret;
}

template <typename T>
bool SeqList<T>::set(int i, const T& e)
{
    bool ret = ( (0 <= i) && (i < m_length) );

    if( ret )
    {
        m_array[i] = e;
    }

    return ret;
}

template <typename T>
bool SeqList<T>::get(int i, T& e) const
{
    bool ret = ( (0 <= i) && (i < m_length) );

    if( ret )
    {
        e = m_array[i];
    }

    return ret;
}

template <typename T>
int SeqList<T>::length() const
{
    return m_length;
}

template <typename T>
void SeqList<T>::clear()
{
    m_length = 0;
}

template <typename T>
T& SeqList<T>::operator[] (int i)
{
    if( (0 <= i) && (i < m_length) )
    {
        return m_array[i];  //因为else是抛出异常，所以异常可以写在这，
    }
    else//数组操作符再加个形参返回目标位置数据元素的值不太好，因为这时数组操作符的语义就变了，所以返回值返回目标位置数据元素，如果此时数据越界，就只能跑异常了
    {
        THROW_EXCEPTION(IndexOutOfBoundsException, "parameter i is invalid ...");
    }
}

template <typename T>
T SeqList<T>::operator[] (int i) const
{
    return const_cast<SeqList<T>&>(*this)[i];//太精妙
}

}

#endif // SEQLIST_H
