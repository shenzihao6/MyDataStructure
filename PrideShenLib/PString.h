#ifndef PSTRING_H
#define PSTRING_H

#include "Object.h"

namespace PrideShenLib
{

class String : Object //做的事情就是利用面向对象的技术封装c语言中的字符串的实现方式
{
protected:
    char* m_str;
    int m_length;

    void init(const char* s);
    bool equal(const char* l, const char* r, int len) const;
    static int* make_pmt(const char* p);      //O(m)   //因为不需要再内部使用this指针调用成员，所以直接写成static更合适
    static int kmp(const char* s, const char* p);   //O(m) + 0(n) = O(m+n)


public:
    String();
    String(char c);
    String(const char* s);
    String(const String& s);

    int length() const;
    const char* str() const;

    bool operator == (const String& s) const;
    bool operator == (const char* s) const;

    bool operator != (const String& s) const;
    bool operator != (const char* s) const;

    bool operator > (const String& s) const;
    bool operator > (const char* s) const;

    bool operator < (const String& s) const;
    bool operator < (const char* s) const;

    bool operator >= (const String& s) const;
    bool operator >= (const char* s) const;

    bool operator <= (const String& s) const;
    bool operator <= (const char* s) const;

    String operator + (const String& s) const;
    String operator + (const char* s) const;

    String& operator += (const String& s);
    String& operator += (const char* s);

    String operator - (const String& s) const;
    String operator - (const char* s) const;

    String& operator -= (const String& s);
    String& operator -= (const char* s);

    String& operator = (const String& s);
    String& operator = (const char* s);
    String& operator = (char c);

    char& operator[] (int i);
    char operator[] (int i) const;

    bool startWith(const char* s) const;
    bool startWith(const String& s) const;
    bool endOf(const char* s) const;
    bool endOf(const String& s) const;

    String& insert(int i, const char* s);
    String& insert(int i, const String& s);

    String& trim();

    int indexOf(const char* s) const;
    int indexOf(const String& s) const;

    String& remove(int i, int len);
    String& remove(const char* s);
    String& remove(const String& s);

    String& replace(const char* t, const char* s);
    String& replace(const String& t, const char* s);
    String& replace(const char* t, String& s);
    String& replace(const String& t, const String& s);

    String sub(int i, int len) const;

    ~String();
};

}
#endif // PSTRING_H
