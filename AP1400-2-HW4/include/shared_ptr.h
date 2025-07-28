#ifndef SHARED_PTR
#define SHARED_PTR
template <typename T>
class SharedPtr {
public:
    SharedPtr(T*);
    SharedPtr();
    ~SharedPtr();

    SharedPtr(const SharedPtr<T>&);
    SharedPtr<T>& operator=(const SharedPtr<T>&);
    
    size_t use_count() const;

    T* get() const;
    T operator*();
    T* operator->();

    void reset();
    void reset(T*);

    explicit operator bool() const {
        return _p != nullptr;
    }

private:
    T* _p;
    size_t* ptrCount;
};

template<typename T>
SharedPtr<T> make_shared(T&&);

#include "shared_ptr.hpp"
#endif //SHARED_PTR