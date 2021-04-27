#ifndef POINTER_H
#define POINTER_H

#include "Object.h"

namespace PrideShenLib
{

template < typename T >
class Pointer : public Object
{
protected:
    T* m_pointer;

public:
    Pointer(T* p = nullptr)
    {
        m_pointer = p;
    }

    T* operator-> ()
    {
        return m_pointer;
    }

    T& operator* ()
    {
        return *m_pointer;
    }

    const T* operator-> () const
    {
        return m_pointer;
    }

    const T& operator* () const
    {
        return *m_pointer;
    }


    bool isnullptr() const
    {
        return (m_pointer == nullptr);
    }

    T* get() const
    {
        return m_pointer;
    }

    virtual ~Pointer() = 0;//老师说不重写~Pointer虚析构函数，Pointer类也是抽象类，因为继承自Object
                           //但是我实验的结果来看，不重写虚析构函数，它就不是抽象类啊
};

template < typename T >
Pointer<T>::~Pointer()
{

}

}

#endif // POINTER_H
