#include "unique_ptr.h"
template<typename T>
UniquePtr<T>::UniquePtr(T* other) : _p(other) {}

template<typename T>
UniquePtr<T>::UniquePtr() : _p(nullptr) {}

template<typename T>
UniquePtr<T>::~UniquePtr() {
    delete _p;
    _p = nullptr;
}

template<typename T>
T* UniquePtr<T>::get() const {
    return _p;
}

template<typename T>
T& UniquePtr<T>::operator*() {
    return *_p;
}

template<typename T>
T* UniquePtr<T>::operator->() {
    return _p;
}

template<typename T>
void UniquePtr<T>::reset() {
    UniquePtr<T> temp{};
    std::swap(this->_p, temp._p);
}

template<typename T>
void UniquePtr<T>::reset(T* other) {
    UniquePtr<T> temp{other};
    std::swap(this->_p, temp._p);
}

template<typename T>
T* UniquePtr<T>::release() {
    T* temp{nullptr};
    std::swap(this->_p, temp);
    return temp;
}

template <typename T>
UniquePtr<T> make_unique(T&& value) {
    return UniquePtr<T>{new T{std::forward<T>(value)}};
}