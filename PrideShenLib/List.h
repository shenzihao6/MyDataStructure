#ifndef LIST_H
#define LIST_H

#include "Object.h"

namespace PrideShenLib
{

template <typename T>
class List : public Object
{
protected:
    List(const List&); //难道不是List(const List<T>&);
    List& operator= (const List&);//难道不是List<T>& operator= (const List<T>&);

public:
    List() {}
    virtual int find(const T& e) const = 0;
    virtual bool insert(const T& e) = 0;
    virtual bool insert(int i, const T& e) = 0;
    virtual bool remove(int i) = 0;
    virtual bool set(int i, const T& e) = 0;
    virtual bool get(int i, T& e) const = 0; //const对象是不能修改成员变量的值的，可能是不需要修改成员变量,所以可以声明成const，其它的抽象函数不满足要求
    virtual int length() const = 0; //const对象是不能修改成员变量的值的，可能是不需要修改成员变量,所以可以声明成const，其它的抽象函数不满足要求
    virtual void clear() = 0;
};

}
/*构造函数不是List(const List<T>&);*/
#endif // LIST_H
