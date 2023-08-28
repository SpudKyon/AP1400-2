#ifndef UNIQUE_PTR
#define UNIQUE_PTR

#include <cstddef>
#include<iostream>
template <typename T>
class UniquePtr {
public:
    UniquePtr()
    {
    }

    UniquePtr(T value)
    {
        ptr = new T(value);
    }

    UniquePtr(T* nptr)
            : ptr(nptr)
    {
    }

    UniquePtr(const UniquePtr& uptr) = delete;
    UniquePtr operator=(const UniquePtr& uptr)=delete;

    UniquePtr(UniquePtr&& uptr)
    {
        ptr=uptr.ptr;
        uptr.ptr=nullptr;
    }

    ~UniquePtr()
    {
        delete ptr;
        ptr = nullptr;
    }

    T& operator*() const{
        return *ptr;
    }

    T* operator->() const{
        return ptr;
    }

    operator bool() const{
        return ptr!=nullptr;
    }

    T* get() const
    {
        return ptr;
    }
    void reset(){
        delete ptr;
        ptr=nullptr;
    }
    void reset(T*nptr){
        delete ptr;
        ptr=nptr;
    }

    T* release(){
        T *temp=ptr;
        ptr=nullptr;
        return temp;
    }

private:
    T* ptr = nullptr;
};

template <typename T>
UniquePtr<T> make_unique(T value)
{
    return UniquePtr<T>(value);
}

#endif // UNIQUE_PTR