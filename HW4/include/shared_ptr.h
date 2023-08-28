#ifndef SHARED_PTR
#define SHARED_PTR
#include <iostream>
template <typename T>
class SharedPtr {
public:
    SharedPtr()
    {
        use_num=new size_t(0);
        // use_num++;
    }

    SharedPtr(T value)
    {
        use_num=new size_t(1);
        _p = new T(value);
    }

    SharedPtr(T* ptr)
    {
        use_num=new size_t(1);
        _p = ptr;
    }

    SharedPtr(const SharedPtr& sptr)
    {
        use_num=sptr.use_num;
        _p = sptr._p;
        ++(*use_num);
    }

    SharedPtr& operator=(const SharedPtr& sptr)
    {
        if(_p==sptr._p){
            return *this;
        }

        if(_p){
            --(*use_num);
            if((*use_num)<=0){
                delete _p;
                delete use_num;
            }

        }

        use_num=sptr.use_num;
        _p = sptr._p;
        ++(*use_num);

        return *this;
    }

    ~SharedPtr()
    {
        --(*use_num);
        if ((*use_num )<= 0) {
            delete _p;
            // delete use_num;

            _p = nullptr;
            // use_num=nullptr;
        }
    }

    T& operator*() const{
        return *_p;
    }

    T* operator->() const{
        return _p;
    }

    operator bool() const{
        return _p!=nullptr;
    }



    T* get()
    {
        return _p;
    }

    size_t use_count()
    {
        return *use_num;
    }

    void reset(){
        use_num=new size_t(0);
        delete _p;
        _p=nullptr;
    }

    void reset(T*nptr){
        delete _p;
        _p=nptr;
    }

private:
    T* _p = nullptr;
    size_t* use_num = nullptr;
};

template <typename T>
SharedPtr<T> make_shared(T value)
{
    return SharedPtr<T>(value);
}

#endif // SHARED_PTR