#pragma once
#include<exception>

template<typename T>
class DynamicArray {
public:
    DynamicArray(const T* data, size_t size);
    DynamicArray(size_t size);
    DynamicArray(const DynamicArray& other);
    DynamicArray(size_t size, const T& obj);
    DynamicArray();

    T& Get(size_t index);
    const T& Get(size_t index) const;
    DynamicArray& operator=(const DynamicArray& other);
    T& operator[](size_t index);
    const T& operator[](size_t index) const;
    void Set(const T& value, size_t index);
    void Resize(size_t new_size);
    size_t GetSize() const;
    void PushBack(const T& elem);

    ~DynamicArray();
private:
    size_t _size;
    size_t _capacity;
    T* _data;
};

template<typename T>
DynamicArray<T>::DynamicArray(const T* data, size_t size) : _size(size), _capacity(_size), _data(new T[_capacity]) {
    for (size_t i = 0; i < _size; ++i) {
        _data[i] = data[i];
    }
}

template<typename T>
DynamicArray<T>::DynamicArray() : _size(0), _capacity(0), _data(nullptr) {}

template<typename T>
DynamicArray<T>::DynamicArray(size_t size) : _size(size), _capacity(_size), _data(new T[_capacity]) {}

template<typename T>
DynamicArray<T>::DynamicArray(size_t size, const T& obj) : DynamicArray(size) {
    for (size_t i = 0; i < size; i++) {
        _data[i] = obj;
    }
}

template<typename T>
DynamicArray<T>::DynamicArray(const DynamicArray& other) : _size(other._size), _capacity(_size), _data(new T[_capacity]) {
    for (size_t i = 0; i < _size; ++i) {
        _data[i] = other._data[i];
    }
}

template<typename T>
T& DynamicArray<T>::Get(size_t index) {
    if (_size <= index) {
        throw std::out_of_range("");
    }
    return _data[index];
}

template<typename T>
const T& DynamicArray<T>::Get(size_t index) const {
    if (_size <= index) {
        throw std::out_of_range("");
    }

    return _data[index];
}

template<typename T>
T& DynamicArray<T>::operator[](size_t index) {
    return Get(index);
}

template<typename T>
const T& DynamicArray<T>::operator[](size_t index) const {
    return Get(index);
}

template<typename T>
DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray& other) {
    if (this == &other) {
        return *this;
    }
    T* new_data = new T[other._size];
    for (size_t i = 0; i < other._size; ++i) {
        new_data[i] = other._data[i];
    }
    _capacity = other._size;
    delete[] _data;
    _data = new_data;
    _size = _capacity;
    return *this;
}

template<typename T>
void DynamicArray<T>::Set(const T& value, size_t index) {
    Get(index) = value;
}

template<typename T>
void DynamicArray<T>::Resize(size_t new_size) {
    if (new_size == _size) {
        return;
    }
    if (new_size < _size) {
        _size = new_size;
        return;
    }
    if (new_size > _capacity) {
        if (_capacity == 0) {
            ++_capacity;
        }
        while (_capacity < new_size) {
            _capacity *= 2;
        }
        T* new_data = new T[_capacity];
        for (size_t i = 0; i < _size; ++i) {
            new_data[i] = _data[i];
        }
        delete[] _data;
        _data = new_data;
    }
    _size = new_size;
}

template<typename T>
size_t DynamicArray<T>::GetSize() const {
    return _size;
}

template<typename T>
DynamicArray<T>::~DynamicArray() {
    delete[] _data;
}


template<typename T>
void DynamicArray<T>::PushBack(const T& elem) {
    if (_size == _capacity) {
        size_t new_capacity;
        if (_capacity == 0) {
            new_capacity = 1;
        }
        else {
            new_capacity = _capacity * 2;
        }
        T* new_data = new T[new_capacity];
        for (size_t i = 0; i < _size; ++i) {
            new_data[i] = _data[i];
        }
        delete[] _data;
        _capacity = new_capacity;
        _data = new_data;
    }
    _data[_size] = elem;
    ++_size;
}