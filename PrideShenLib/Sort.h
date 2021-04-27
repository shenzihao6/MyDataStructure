#ifndef SORT_H
#define SORT_H

#include "Object.h"

namespace PrideShenLib
{

class Sort : public Object
{
private:
    Sort();
    Sort(const Sort&rhs);

    Sort& operator = (const Sort&);

    template < typename T > //为啥在这来加模板声明？？
    static void Swap(T& a, T& b)
    {
        T c = a;//这样会编译错吗 T c(a)才是正确的？;
        a = b;
        b = c;
    }

    template < typename T >
    static void Merge(T src[], T helper[], int begin, int mid, int end, bool min2max = true)
    {
        int i = begin;
        int j = mid + 1;
        int k = begin;

        while( (i <= mid) && (j <= end) )
        {
            if( src[i] < src[j] )
            {
                helper[k++] = src[i++];
            }
            else
            {
                helper[k++] = src[j++];
            }
        }

        while( i < mid )
        {
            helper[k++] = src[i++];
        }

        while( j <= end )
        {
            helper[k++] = src[j++];
        }

        for(i=begin; i<=end; i++)
        {
            src[i] = helper[i];
        }
    }

    template < typename T >
    static void Merge(T src[], T helper[], int begin, int end, bool min2max = true)
    {
        if( begin == end )
        {
            return;
        }
        else
        {
            int mid = (begin + end) / 2;
            Merge(src, helper, begin, mid, min2max);
            Merge(src, helper, mid+1, end, min2max);
            Merge(src, helper, begin, mid, end, min2max);
        }
    }

    template < typename T >
    static int Partition(T array[], int begin, int end, bool min2max = true)
    {
        T pv = array[begin];

        while( begin < end )
        {
            while( (begin < end) && (array[end] > pv) )
            {
                end--;
            }

            Swap(array[begin], array[end]);

            while ( (begin < end) && (array[end] < pv) )
            {
                begin++;
            }

            Swap(array[begin], array[end]);
        }

        array[begin] = pv;

        return begin;
    }

    template < typename T >
    static void Quick(T array[], int begin, int end, bool min2max = true)
    {
        if( begin == end )
        {
            int pivot = Partition(array, begin, end, min2max);

            Quick(array, begin, pivot-1, min2max);
            Quick(array, pivot+1, end, min2max);
        }
    }

public:
    template < typename T >
    static void Select(T array[], int len, bool min2max = true)
    {

        for(int i=0; i<len; i++)
        {
            int min = i;

            for(int j=i+1; j<len; j++)//j的初始值就是i+1的原因是要与当前的min进行比较的时候后面的一个元素啊
            {
                if( min2max ? (array[min] > array[j]) : (array[min] < array[j]) ) //排序的最大值最小值只需要改变比较中的大于小于
                                                                                  //号就可以了，所以这里用了一个三目运算符来判断，
                                                                                  //通过标志位作为参数来用户决定排序最大值
                {
                    min = j;
                }
            }

            if( min != i )
            {
                Swap(array[i], array[min]);//如果本来这个i就是最小值了，那就不交换，不加这个if也行，但是我们追求的是一种高效，因为交换费时
                                           //同时避免了课件中25与25交换的情景，稳定排序，因为这个Swap函数的T类型可能是一个复制的类类型，
                                           //所以在交换的时候进行深拷贝相当耗时，因为cpu很快的可以完成比较，所以说我们能用比较操作避开交换
                                           //操作，我们就用比较操作避开交换操作，这样效率会更好
            }
        }

    }

    template < typename T >
    static void Insert(T array[], int len, bool min2max = true)
    {
        for(int i=1; i<len; i++)//从1开始的原因，是第一个元素就是默认是有序的序列了，这也是一个实现优化，省了时间了
        {
            int k = i;
            T e = array[i];

            for(int j=i-1; (j>=0) && (min2max ? (array[j] > e) : (array[j] < e)); j--)
            {
                array[j+1] = array[j];
                k = j;//因为空出的位置很有可能就是e应该在的位置
            }

            if( k != i )
            {
                array[k] = e;
            }
        }
    }

    template < typename T >
    static void Bubble(T array[], int len, bool min2max = true)
    {
        bool exchange = true;

        for(int i=0; (i<len) && (exchange); i++)
        {
            exchange = false;

            for(int j=len-1; j>i; j--)
            {
                if( min2max ? (array[j] < array[j-1]) : (array[j] > array[j-1]) )
                {
                    Swap(array[j], array[j-1]);
                    exchange = true;
                }
            }
        }
    }

    template < typename T >
    static void Shell(T array[], int len, bool min2max = true)
    {
        int d = len;

        do
        {
            d = d / 3 + 1;//d--无法提高效率//通过数学的角度来说d的这种方式减小是有困难的，但是实践证明这样子d的减小方法是比较好的

            bool exchange = true;

            for(int i=0; (i<len) && (exchange); i+=d)
            {
                exchange = false;

                for(int j=(len-1) - ((len-1) % d); j>i; j-=d)
                {
                    if( min2max ? (array[j] < array[j-d]) : (array[j] > array[j-d]) )
                    {
                        Swap(array[j], array[j-d]);
                        exchange = true;
                    }
                }
            }
        } while( d > 1 );
    }

    template < typename T >
    static void Merge(T array[], int len, bool min2max)
    {
        T* helper = new T[len];

        if( helper != nullptr )
        {
            Merge(array, helper, 0, len-1, min2max);
        }
    }

    template < typename T >
    static void Quick(T array[], int len, bool min2max)
    {
        Quick(array, 0, len-1, min2max);
    }

};

}

#endif // SORT_H
