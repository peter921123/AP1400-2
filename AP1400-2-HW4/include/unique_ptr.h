#ifndef UNIQUE_PTR
#define UNIQUE_PTR
template <typename T>
class UniquePtr {
public:
    UniquePtr(T*);
    UniquePtr();
    ~UniquePtr();
    UniquePtr(const UniquePtr<T>&) = delete;
    UniquePtr& operator=(const UniquePtr<T>&) = delete;

    T* get() const;
    T& operator*();
    T* operator->();

    void reset();
    void reset(T*);
    T* release();

    explicit operator bool() const {
        return _p != nullptr;
    }

private:
    T* _p;
};

template <typename T>
UniquePtr<T> make_unique(T&&);
#include "unique_ptr.hpp"
#endif //UNIQUE_PTR
