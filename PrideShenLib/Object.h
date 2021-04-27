#ifndef OBJECT_H
#define OBJECT_H

namespace PrideShenLib
{

class Object
{
public:
    void* operator new (unsigned long long  size) noexcept;
    void operator delete (void* p);
    void* operator new[] (unsigned long long size) noexcept;
    void operator delete[] (void* p);
    bool operator == (const Object& obj);
    bool operator != (const Object& obj);

    virtual ~Object() = 0;
};

}
#endif // OBEJECT_H
