#ifndef STATICARRAY_H
#define STATICARRAY_H
#include "Array.h"

namespace PrideShenLib
{

template < typename T, int N >
class StaticArray : public Array<T>
{
protected:
    T m_space[N];

public:
    StaticArray()
    {
        this->m_array = m_space;
    }

    StaticArray(const StaticArray<T, N>& obj)//拷贝构造只能子类实现，在父类实现没有什么意义，
                                             //而且不管是赋值操作还是拷贝构造对成员m_space进行深拷贝构造，对m_array没有进行拷贝构造和赋值操作
    {
        this->m_array = m_space;

        for(int i=0; i<N; i++)
        {
            m_space[i] = obj.m_space[i];//m_space的长度一定与obj.space的T,N一样，不然的话它们就是不同的类型，不能进行拷贝,否则编译器就直接报错
                                        //与dynamicArray不同dynamicArray的话可以改变m_space的长度，因为m_space的指定是通过构造函数完成的，
                                        //不是类型的一部分
        }
    }

    StaticArray<T, N>& operator= (const StaticArray<T, N>& obj) //赋值操作只能子类实现，在父类实现没有什么意义，
                                                                //m_space的长度一定与obj.space的T,N一样，不然的话它们就是不同的类型，
                                                                 //不能进行赋值,否则编译器就直接报错，
                                            //而且不管是赋值操作还是拷贝构造对成员m_space进行深拷贝构造，对m_array没有进行拷贝构造和赋值操作
                                           //与dynamicArray不同dynamicArray的话可以改变m_space的长度，因为m_space的指定是通过构造函数完成的，
                                           //不是类型的一部分
    {
        if( this != &obj )
        {
            for(int i=0; i<N; i++)
            {
                m_space[i] = obj.m_space[i];
            }
        }

        return *this;
    }

    int length() const
    {
        return N;
    }

};

}

#endif // STATICARRAY_H
