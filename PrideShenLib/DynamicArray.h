#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H

#include "Array.h"
#include "Exception.h"

namespace PrideShenLib
{

template < typename T >
class DynamicArray : public Array<T>
{
protected:
    int m_length;

    T* copy(T* array, int len, int newLen)
    {
        T* ret = new T[newLen];

        if( ret != nullptr )
        {
            int size = (len < newLen ? len : newLen); //谁小用谁，要保证吧array中指定的元素的部分全部赋值上去,不越界赋值

            for(int i=0; i<size; i++)
            {
                ret[i] = array[i];
            }
        }

        return ret;
    }

    void update(T* array, int length)
    {
        if( array != nullptr )
        {
            T* temp = this->m_array;

            this->m_array = array;
            this->m_length = length;

            delete[] temp;
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No memory to update DynamicArray object");
        }
    }

    void init(T* array, int length)//虽然init函数和update比还有一部分重复的代码，但是不需要重写这些重复，因为这些代码在逻辑上不能算作一个
                                   //独立的函数模块，我们的目的是每一个函数实现一个功能，每一个函数的实现都是非常清晰的，从而消除重复代码
    {
        if( array != nullptr)
        {
            this->m_array = array;
            this->m_length = length;
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create DynamicArray object");
        }
    }

public:
    DynamicArray(int length = 0)
    {
        /*
        this->m_array = new T[length];

        if( this->m_array != nullptr )
        {
            m_length = length;
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create DynamicArray object");
        }
        */
        /*重构代码 这个在参数上直接转递new[length]的操作真的很棒，要是自己肯定想不到，所以这段代码要多学学*/
        init(new T[length], length);

    }

    DynamicArray(const DynamicArray<T>& obj)
    {

        /*
        this->m_array = new T[obj.m_length];//这里已经是深拷贝了

        if( this->m_array != nullptr )
        {
            m_length = obj.m_array;

            for(int i=0; i<obj.m_length; i++)//这里用m_length和obj.m_length都可以，但是这里却用了obj.m_length,原因就是为了和下面代码保持一致，从而代码复用
            {
                this->m_array[i] = obj.m_array[i];
            }
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create DynamicArray object");
        }*/

        init(copy(obj.m_array, obj.m_length, obj.m_length), obj.m_length);

    }



    DynamicArray<T>& operator= (const DynamicArray<T>& obj)
    {
        /*
        if( this != &obj )
        {
            delete[] this->m_array;//忘记了，这样做的话其实异常不安全了

            this->m_array = new T[obj.m_length];//这里已经是深拷贝了

            if( this->m_array != nullptr )
            {
                m_length = obj.m_array;

                for(int i=0; i<obj.m_length; i++)
                {
                    this->m_array[i] = obj.m_array[i];
                }
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create DynamicArray object");
            }
        }

        return *this;
        */
        /*法一：这样写也是不对的
        if( this != &obj )
        {
            T* temp = this->m_array;

            if( array != nullptr )
            {
                for(int i=0; i<obj.m_length; i++)
                {
                    this->m_array[i] = obj.m_array[i]; //m_array的length可能是改变的了,会越界
                }

                this->m_length = obj.m_length;
                delete[] temp;
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No memory to copy object");
            }
        }

        return *this;
        */
        /*法二：最终方案用的是法二
        if( this != &obj )
        {
            T* array = new T[obj.m_length];

            if( array != nullptr )
            {
                for(int i=0; i<obj.m_length; i++)
                {
                    array[i] = obj.m_array[i];
                }

                T* temp = this->m_array;

                this->m_array = array;
                this->m_length = obj.m_length;

                delete[] temp;
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No memory to copy object");
            }
        }

        return *this;
        */
        /*法三：代码优化，这段代码中把赋值的过程和更新作为了两个函数封装，而不是一个函数，以前要是我做的话肯定会封装成一个函数
         因为拷贝构造的过程和resize函数操作的过程这部分代码是相同的，但是我忘记了前面的构造函数也是有部分代码也是跟此函数中的
        copy重构的代码相同，所以应该重构成两个代码片段。并且copy的返回值又能作为参数传递到update函数中，这样做看起来就像
        一个函数一样，这点比较精妙要多学点*/

        if( this != &obj )
        {
            update(copy(obj.m_array, obj.m_length, obj.m_length), obj.m_length);
        }

        return *this;
    }

    void resize(int length)
    {
        if( length != m_length )
        {
            /*
            T* array = new T[length];

            if( array != nullptr )
            {
                int size = (length < m_length ? length : m_length);//这行代码时this->array赋值给array所做的限制
                                                                   //obj.m_array赋值给array不需要限制因为就相当于
                                                                   //int size = (length < m_length ? length : m_length) length == m_length

                for(int i=0; i<size; i++)
                {
                    array[i] = this->m_array[i];
                }

                T* temp = this->m_array;

                this->m_array = array;
                this->m_length = length;

                delete[] temp;
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No memory to resize object");
            }
            */
            /*代码优化复用*/
            update(copy(this->m_array, m_length, length), length);
        }
    }

    int length() const
    {
        return m_length;
    }

    ~DynamicArray()
    {
        delete[] this->m_array;
    }
};

}

#endif // DYNAMICARRAY_H
