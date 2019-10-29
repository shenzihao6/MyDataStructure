#include "PString.h"
#include <cstring>
#include <cstdlib>
#include "Exception.h"

namespace PrideShenLib
{

int* String::make_pmt(const char* p)      //O(m)
{
    int len = strlen(p);
    int* ret = static_cast<int*>(malloc(sizeof(int) * len));

    if( ret != NULL )
    {
        int ll = 0;

        ret[0] = 0;

        for(int i=1; i<len; i++)
        {
            while( (ll > 0) && (p[ll] != p[i]) )
            {
                ll = ret[ll-1];
            }

            if( p[ll] == p[i] )
            {
                ll++;
            }

            ret[i] = ll;
        }
    }

    return ret;
}

int String::kmp(const char* s, const char* p)    //O(m) + 0(n) = O(m+n)
{
    int ret = -1;
    int sl = strlen(s);
    int pl = strlen(p);
    int* pmt = make_pmt(p);

    if( (pmt != NULL) && (0 < pl) && (pl <= sl) )
    {
        for(int i=0, j=0; i<sl; i++)
        {
            while( (j > 0) && s[i] != p[j] )
            {
                j = pmt[j-1];
            }

            if( s[i] == p[j] )
            {
                j++;
            }

            if( j == pl )
            {
                ret = i + 1 - pl;
                break;
            }
        }
    }

    free(pmt);

    return ret;
}

void String::init(const char* s)
{
    m_str = strdup(s);

    if( m_str )
    {
        m_length = strlen(m_str);
    }
    else
    {
        THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create String object ...");
    }
}

String::String()
{
    init("");
}

String::String(char c)
{
    char s[] = {c, '\0'};//字符数组模拟字符串了

    init(s);
}

String::String(const char* s)
{
    init( s ? s : "" ); //防止空指针的情况，将空指针转换成空字符串,类比exception那里的init设置，这个三目运算符是在init函数体实现的很巧妙，
                       //因为exception中init的message参数在init调用的时候出现了两次，而在init函数体修改的话只需要改一次就行，而这里的三目
                       //运算符写在参数里更加合理，这些都是细节
}

String::String(const String& s)
{
    init(s.m_str);
}

int String::length() const
{
    return m_length;
}

const char* String::str() const
{
    return m_str;
}

bool String::operator == (const String& s) const
{
    return (strcmp(m_str, s.m_str) == 0);
}

bool String::operator == (const char* s) const
{
    return (strcmp(m_str, s ? s : "") == 0);
}

bool String::operator != (const String& s) const
{
    return !(*this == s);
}

bool String::operator != (const char* s) const
{
    return !(*this == s);
}

bool String::operator > (const String& s) const
{
    return (strcmp(m_str, s.m_str) > 0);
}

bool String::operator > (const char* s) const
{
    return (strcmp(m_str, s ? s : "") > 0);
}

bool String::operator < (const String& s) const
{
    return (strcmp(m_str, s.m_str) < 0);
}

bool String::operator < (const char* s) const
{
    return (strcmp(m_str, s ? s : "") < 0);
}

bool String::operator >= (const String& s) const
{
    return (strcmp(m_str, s.m_str) >= 0);

}

bool String::operator >= (const char* s) const
{
    return (strcmp(m_str, s ? s : "") >= 0);

}

bool String::operator <= (const String& s) const
{
    return (strcmp(m_str, s.m_str) <= 0);

}
bool String::operator <= (const char* s) const
{
    return (strcmp(m_str, s ? s : "") <= 0);
}

String String::operator + (const String& s) const
{
    return (*this + s.m_str);
}

String String::operator + (const char* s) const //可以先实现const char*版本的+重载函数再去实现
{
    String ret;
    int len = m_length + strlen(s ? s : "");

    char* str = reinterpret_cast<char*>(malloc(len + 1)); //在堆空间申请的原因是不能返回局部指针，要自己控制生命周期

    if( str )
    {
        strcpy(str, m_str);
        strcat(str, s ? s : "");

        free(ret.m_str);

        ret.m_str = str;

        ret.m_length = len;
    }
    else
    {
        THROW_EXCEPTION(NoEnoughMemoryException, "No memory to add String value ...");
    }

    return ret;
}

String& String::operator += (const String& s)
{
    return (*this = *this + s);
}

String& String::operator += (const char* s)
{
    return (*this = *this + s);
}

String String::operator - (const String& s) const
{
    return String(*this).remove(s);//String产生的是一个临时对象，临时对象把子串删除，这样做当前的字符串也是没有被改变的
}

String String::operator - (const char* s) const
{
    return String(*this).remove(s);
}

String& String::operator -= (const String& s)
{
    return remove(s);
}

String& String::operator -= (const char* s)
{
    return remove(s);
}

String& String::operator = (const String& s)
{
    return (*this = s.m_str);
}

String& String::operator = (const char* s)
{
    if( m_str != s )
    {
        char* str = strdup(s ? s : "");

        if( str )
        {
            free(m_str);
            m_str = str;
            m_length = strlen(m_str);//这里的长度赋值以m_str为准
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No memory to assign new String value ...");
        }
    }

    return *this;
}

String& String::operator = (char c)
{
    char s[] = {c, '\0'};

    return (*this = s);
}

char& String::operator[] (int i)
{
    if( (0 <= i) && (i < m_length) )
    {
        return m_str[i]; //真的可以改变吗
    }
    else
    {
        THROW_EXCEPTION(IndexOutOfBoundsException, "Parameter i is invalid ...");
    }
}

char String::operator[] (int i) const
{
    return (const_cast<String&>(*this))[i];
}

bool String::equal(const char* l, const char* r, int len) const
{
    bool ret = true;

    for(int i=0; i<len && ret; i++)
    {
        ret = ret && (l[i] == r[i]);
    }

    return ret;
}

bool String::startWith(const char* s) const
{
    bool ret = ( s != NULL );

    if( ret )
    {
        int len = strlen(s);

        ret = (len < m_length) && equal(m_str, s, len);
    }

    return ret;
}

bool String::startWith(const String& s) const
{
    return startWith(s.m_str);
}

bool String::endOf(const char* s) const
{
    bool ret = ( s != NULL );

    if( ret )
    {
        int len = strlen(s);
        char* str = m_str + (m_length - len);

        ret = (len < m_length) && equal(str, s, len);
    }

    return ret;
}

bool String::endOf(const String& s) const
{
    return endOf(s.m_str);
}

String& String::insert(int i, const char* s) //返回引用的目的是可以作为左值进行链式应用
{
    if( (0 <= i) && (i <= m_length) )
    {
        if( (s != NULL) && (s[0] != '\0') )
        {
            int len = strlen(s);
            char* str = reinterpret_cast<char*>(malloc(m_length + len + 1));

            if( str != NULL )
            {
                strncpy(str, m_str, i);
                strncpy(str + i, s, len);
                strncpy(str + i + len, m_str + i, m_length - i);

                str[m_length + len] = '\0';

                free(m_str);

                m_str = str;

                m_length = m_length + len;
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No enough memory to insert string value ...");
            }
        }
    }
    else
    {
        THROW_EXCEPTION(IndexOutOfBoundsException, "Parameter i is invalid ...");
    }

    return *this;
}

String& String::insert(int i, const String& s)
{
    return insert(i, s.m_str);
}

String& String::trim()
{
    int b = 0;
    int e = m_length - 1;

    while( m_str[b] == ' ' ) b++;
    while( m_str[e] == ' ' ) e--;

    if( b == 0 )
    {
        m_str[e+1] = '\0';

        m_length = e + 1;
    }
    else
    {
        for(int i=0, j=b; j<=e; i++, j++)
        {
            m_str[i] = m_str[j];
        }

        m_str[e - b + 1] = '\0';

        m_length = e - b + 1;
    }

    return *this;
}

int String::indexOf(const char* s) const
{
    return kmp(m_str, s ? s : "");
}

int String::indexOf(const String& s) const
{
    return kmp(m_str, s.m_str); //直接调用kmp函数效率会高一点,而且重要的是都是一行代码实现的，当然直接调用kmp会好一些
                                //而不是进行调用已有重载函数来完成重载
}

String& String::remove(int i, int len)//因为insert函数与remove函数是两个相反的函数,insert函数返回值是String&,所以remove函数也是这样
{
    if( (0 <= i) && (i < m_length) )
    {
        int n = i;
        int m = i + len; //其实用一个变量也许，但是这里用了两个变量，其实两个变量更加直观，但是消耗了内存

        while( (n < m) && (m < m_length) )
        {
            m_str[n++] = m_str[m++];
        }

        m_str[n] = '\0';
        m_length = n;
    }//不需要抛出异常吗？第二遍的时候想想

    return *this;
}

String& String::remove(const char* s)
{
    return remove(indexOf(s), strlen(s ? s : "")); //删除一个null字符也是没有意义的啊？跟insert对比是这么样的说法
}

String& String::remove(const String& s)
{
    return remove(indexOf(s), strlen(s.m_str));//这样效率更高？因为少了一次函数的调用和根本不需要判断为NULL的条件
}

String& String::replace(const char* t, const char* s)
{
    int index = indexOf(t);

    if( index >= 0 )
    {
        remove(t);
        insert(index, s);
    }

    return *this;
}

String& String::replace(const String& t, const char* s)
{
    return replace(t.m_str, s);
}

String& String::replace(const char* t, String& s)
{
    return replace(t, s.m_str);
}

String& String::replace(const String& t, const String& s)
{
    return replace(t.m_str, s.m_str);
}

String String::sub(int i, int len) const
{
    String ret;

    if( (0 <= i) && (i < m_length) )
    {
        if( len < 0 )    len = 0; //归一化为0
        if( len + i > m_length ) len = m_length - i;

        char* str = reinterpret_cast<char*>(malloc(len + 1));

        if( str )
        {
            strncpy(str, m_str + i, len);

            str[len] = '\0';

            ret = str;
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No enough memory to sub string value");
        }

    }
    else
    {
        THROW_EXCEPTION(IndexOutOfBoundsException, "Parameter i is invalid ...");
    }

    return ret;
}


String::~String()
{
    free(m_str);
}

}


