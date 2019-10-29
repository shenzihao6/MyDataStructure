#include "Exception.h"
#include <cstring>
#include <cstdlib>
#include <malloc.h>

using namespace std;

namespace PrideShenLib
{

Exception::Exception(const char* message)
{
    init(message, NULL, 0);
}

Exception::Exception(const char* file, int line)
{
    init(NULL, file, line);
}

Exception::Exception(const char* message, const char* file, int line)
{
    init(message, file, line);
}

Exception::Exception(const Exception& e)//实现拷贝构造函数一般都是为了深拷贝
{
    m_message = strdup(e.m_message);
    m_location = strdup(e.m_location);
}

Exception& Exception::operator= (const Exception& e)
{
    if(this != &e)
    {
        free(m_message); //malloc搭配free
        free(m_location);

        m_message = strdup(e.m_message);
        m_location = strdup(e.m_location);
    }

    return *this;
}

void Exception::init(const char* message, const char* file, int line)//message这个指针指向的字符串可能是在栈上面，堆上面，全局的数据区上，
                                                          //所以不能直接赋值给m_message
{                                                         //不能控制message所指向的外部字符串的生命周期，这样写是不安全的。
    m_message = (message ? strdup(message) : NULL);//相当于自己malloc了一个空间，这个内存的大小为message内存的大小，内存空间为message的内容 ,记得进行内存释放

    if( file != NULL ) //发生异常的文件名
    {
        char sl[16] = {0};

        itoa(line, sl, 10);//类似于string s = QSTring::number(i);
        /*这里使用malloc的原因是m_location是指针，如果是局部变量的话，局部变量内存空间释放，m_location就会是野指针了
        如果有公司明令指针使用c++标准库，这里从malloc换成我们重载过后的new操作符也是可以的，
        这里直接写的malloc是因为在写这段代码时，我们并没有重载new,为了不抛出异常，就换成了malloc了*/
        m_location = static_cast<char*>(malloc(strlen(file) + strlen(sl) + 2));//这里2是因为存储":"和"\0"这两个字符,但是为啥要static_cast

        if( m_location != NULL)
        {
            m_location = strcpy(m_location, file);
            m_location = strcat(m_location, ":");
            m_location = strcat(m_location, sl);//为啥这里要动态内存申请
        }

        //else
        //{
            /*可以抛一个异常出来啊,但是父类对象中抛出一个子类异常时不可以的,老爸没诞生，儿子先出来了,逻辑上讲不通啊*/
            /*从运行机理上来说这样子运行的话就会死循环了，所以最好的做法就是else分支不要*/
            //THROW_EXCEPTION(NoEnoughMemoryException, "Exception::init");
        //}

    }
    else
    {
        m_location = NULL;
    }
}
const char* Exception::message() const
{
    return m_message;
}

const char* Exception::location() const
{
    return m_location;
}

Exception::~Exception()
{
    free(m_message); //为啥要free，strdup内部是实现了malloc
    free(m_location);
}

}
