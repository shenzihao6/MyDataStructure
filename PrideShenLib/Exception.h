#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "Object.h"

namespace PrideShenLib
{
#define THROW_EXCEPTION(e, m) (throw e(m, __FILE__, __LINE__)) //把__FILE__和__LINE__封装好，以后在用的时候只要提供异常名和异常信息就行了

class Exception : public Object //object如果不在同一个namespace呢？
{   
    void init(const char* message, const char* file, int line);//因为这几个重载构造函数在内部逻辑上差不多的，为了代码的复用，所以用一个函数代替

protected:
    char* m_message;
    char* m_location;

public:
    Exception(const char* message);
    Exception(const char* file, int line);
    Exception(const char* message, const char* file, int line);

    Exception(const Exception& e);
    Exception& operator= (const Exception& e);

    virtual const char* message() const;
    virtual const char* location() const;

    virtual ~Exception() = 0;
};

class ArithmeticException :public Exception//c++对象模型
{
public:
    ArithmeticException(const char* message) : Exception(message) {}
    ArithmeticException(const char* file, int line) : Exception(file, line) {}
    ArithmeticException(const char* message, const char* file, int line) : Exception(message, file, line) {}
    ArithmeticException(const ArithmeticException& e) : Exception(e) {}

    ArithmeticException& operator= (const ArithmeticException& e)
    {
        Exception::operator=(e);

        return *this;
    }

};

class nullptrPointerException : public Exception//c++对象模型
{
public:
    nullptrPointerException(const char* message) : Exception(message) {}
    nullptrPointerException(const char* file, int line) : Exception(file, line) {}
    nullptrPointerException(const char* message, const char* file, int line) : Exception(message, file, line) {}
    nullptrPointerException(const nullptrPointerException& e) : Exception(e) {}

    nullptrPointerException& operator =(const nullptrPointerException& e)
    {
        Exception::operator=(e);

        return *this;
    }

};

class IndexOutOfBoundsException :public Exception//c++对象模型
{
public:
    IndexOutOfBoundsException(const char* message) : Exception(message) {}
    IndexOutOfBoundsException(const char* file, int line) : Exception(file, line) {}
    IndexOutOfBoundsException(const char* message, const char* file, int line) : Exception(message, file, line) {}
    IndexOutOfBoundsException(const IndexOutOfBoundsException& e) : Exception(e) {}

    IndexOutOfBoundsException& operator= (const IndexOutOfBoundsException& e)
    {
        Exception::operator=(e);

        return *this;
    }
};

class NoEnoughMemoryException : public Exception//c++对象模型
{
public:
    NoEnoughMemoryException(const char* message) : Exception(message) {}
    NoEnoughMemoryException(const char* file, int line) : Exception(file, line) {}
    NoEnoughMemoryException(const char* message, const char* file, int line) : Exception(message, file, line) {}
    NoEnoughMemoryException(const NoEnoughMemoryException& e) : Exception(e) {}

    NoEnoughMemoryException& operator= (const NoEnoughMemoryException& e)
    {
        Exception::operator=(e);

        return *this;
    }
};

class InvalidParameterException : public Exception//c++对象模型
{
public:
    InvalidParameterException(const char* message) : Exception(message) {}
    InvalidParameterException(const char* file, int line) : Exception(file, line) {}
    InvalidParameterException(const char* message, const char* file, int line) : Exception(message, file, line) {}
    InvalidParameterException(const InvalidParameterException& e) : Exception(e) {}

    InvalidParameterException& operator= (const InvalidParameterException& e)
    {
        Exception::operator=(e);

        return *this;
    }
};

class InvalidOperatorException : public Exception//c++对象模型
{
public:
    InvalidOperatorException(const char* message) : Exception(message) {}
    InvalidOperatorException(const char* file, int line) : Exception(file, line) {}
    InvalidOperatorException(const char* message, const char* file, int line) : Exception(message, file, line) {}
    InvalidOperatorException(const InvalidOperatorException& e) : Exception(e) {}

    InvalidOperatorException& operator= (const InvalidOperatorException& e)
    {
        Exception::operator=(e);

        return *this;
    }
};

}
#endif // EXCEPTION_H
