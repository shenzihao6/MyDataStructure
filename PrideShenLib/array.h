#ifndef ARRAY_H
#define ARRAY_H

#include "Object.h"
#include "Exception.h"

namespace PrideShenLib
{

template < typename T >
class Array : public Object
{
protected:
    T* m_array;

public:
    virtual bool set(int i, const T& e);//因为我们要实现的是staticArray和dynamicArray,是有可能子类对象代替父类对象的情况的，
                                        //万一遇到了父类指针指向子类对象的情况而准备的，但是我们真正的期望的是调用子类的重写函数
    virtual bool get(int i, T& e) const;//因为我们要实现的是staticArray和dynamicArray,是有可能子类对象代替父类对象的情况的，
                                        //万一遇到了父类指针指向子类对象的情况而准备的，但是我们真正的期望的是调用子类的重写函数
    T& operator[] (int i);  //要作为左值所以重载const和非const的两个版本
    T operator[] (int i) const;
    virtual int length() const = 0;
};

template < typename T >
bool Array<T>::set(int i, const T &e)
{
    bool ret = ( (0 <= i) && (i <length()) );

    if( ret )
    {
        m_array[i] = e;
    }

    return ret;
}

template < typename T >
bool Array<T>::get(int i, T &e) const
{
    bool ret = ( (0 <= i) && (i <length()) );

    if( ret )
    {
        e = m_array[i];
    }

    return ret;
}

template < typename T >
T& Array<T>::operator[] (int i)
{
    bool ret = ( (0 <= i) && (i <length()) );

    if( ret )
    {
        return m_array[i];
    }
    else
    {
        THROW_EXCEPTION(IndexOutOfBoundsException, "Parameter i is invalid ...");
    }
}

template < typename T >
T Array<T>::operator[] (int i) const
{
    return const_cast<Array<T>&>(*this)[i];
}

}

#endif // ARRAY_H
