TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    Exception.cpp \
    Object.cpp \
    PString.cpp

HEADERS += \
    SmartPointer.h \
    Exception.h \
    Object.h \
    List.h \
    SeqList.h \
    StaticList.h \
    DynamicList.h \
    Array.h \
    StaticArray.h \
    DynamicArray.h \
    LinkList.h \
    StaticLinkList.h \
    Pointer.h \
    SharedPointer.h \
    CircleList.h \
    DualLinkList.h \
    LinuxList.h \
    DualCircleList.h \
    Stack.h \
    StaticStack.h \
    LinkStack.h \
    Queue.h \
    StaticQueue.h \
    LinkQueue.h \
    PString.h \
    Sort.h
