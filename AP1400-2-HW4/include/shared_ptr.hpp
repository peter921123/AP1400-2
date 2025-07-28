#include "shared_ptr.h"

template<typename T>
SharedPtr<T>::SharedPtr() : _p(nullptr), ptrCount(new size_t{1}) {}

template<typename T>
SharedPtr<T>::SharedPtr(T* value) : _p(value), ptrCount(new size_t{1}) {}

template<typename T>
SharedPtr<T>::~SharedPtr() {
    if (ptrCount) {
        --(*ptrCount);
        if (*ptrCount <= 0) {
            delete ptrCount;
            delete _p;
        }
    }
    ptrCount = nullptr;
    _p = nullptr;
}

template<typename T>
SharedPtr<T>::SharedPtr(const SharedPtr<T>& other) : _p(other._p), ptrCount(other.ptrCount) {
    ++(*ptrCount);
}

template<typename T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<T>& other) {
    if (this != &other) {
        delete ptrCount;
        delete _p;
        _p = other._p;
        ptrCount = other.ptrCount;
        ++(*ptrCount);
    }
    return *this;
}

template<typename T>
size_t SharedPtr<T>::use_count() const {
    return ptrCount == nullptr ? 0 : *ptrCount ;
}

template<typename T>
T* SharedPtr<T>::get() const {
    return _p;
}

template<typename T>
T SharedPtr<T>::operator*() {
    return *_p;
}

template<typename T>
T* SharedPtr<T>::operator->() {
    return _p;
}

template<typename T>
void SharedPtr<T>::reset() {
    this->~SharedPtr();
}

template<typename T>
void SharedPtr<T>::reset(T* value) {
    SharedPtr<T> temp{value};
    std::swap(temp._p, this->_p);
    std::swap(temp.ptrCount, this->ptrCount);
}

template<typename T>
SharedPtr<T> make_shared(T&& value) {
    return SharedPtr<T>{new T{std::forward<T>(value)}};
}