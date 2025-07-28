#ifndef SHARED_PTR
#define SHARED_PTR
template <typename T>
class SharedPtr {
public:
    SharedPtr(T*);
    SharedPtr();
    ~SharedPtr();

    SharedPtr(const SharedPtr<T>&);
    SharedPtr& operator=(const SharedPtr<T>&);
    
    size_t use_count() const;

    T* get() const;
    T operator*();
    T* operatpr->();

    void reset();
    void reset(T*);

    explicit operator bool() const {
        return _p != nullptr
    }

private:
    T* _p;
    size_t count;
};

#include "shared_ptr.hpp"
#endif //SHARED_PTR