#pragma once
#include"DynamicArray.h"
//#include"LinkedList.h"

template <typename T>
class Sequence {
public:
    virtual Sequence<T>* Create() = 0;
    virtual Sequence<T>* Copy() = 0;

    virtual T Get(size_t index) const = 0;
    virtual T GetFirst() const = 0;
    virtual T GetLast() const = 0;
    virtual T& operator[](size_t index) = 0;
    virtual const T& operator[](size_t index) const = 0;
    virtual void Set(const T& value, size_t index) = 0;
    virtual size_t GetSize() const = 0;
    virtual Sequence<T>* GetSubSequence(size_t start_index, size_t end_index) const = 0;
    virtual void Append(const T& elem) = 0;
    virtual void Prepend(const T& elem) = 0;
    virtual void InsertAt(const T& elem, size_t index) = 0;
    virtual Sequence* Concat(const Sequence* sequence) const = 0;
    virtual ~Sequence() = default;
};

template<typename T>
class ArraySequence : public Sequence<T> {
public:
    ArraySequence();
    ArraySequence(const T* data, size_t size);
    ArraySequence(const ArraySequence& other) = default;

    T Get(size_t index) const override;
    T GetFirst()const  override;
    T GetLast() const override;
    T& operator[](size_t index) override;
    const T& operator[](size_t index) const override;
    void Set(const T& value, size_t index) override;
    size_t GetSize() const override;
    Sequence<T>* GetSubSequence(size_t start_index, size_t end_index) const override;
    void Append(const T& elem) override;
    void Prepend(const T& elem) override;
    void InsertAt(const T& elem, size_t index) override;
    Sequence<T>* Concat(const Sequence<T>* sequence) const override;

    ~ArraySequence() = default;
private:
    DynamicArray<T> _data;
};

template<typename T>
ArraySequence<T>::ArraySequence() : _data(0) {}

template<typename T>
ArraySequence<T>::ArraySequence(const T* data, size_t size) : _data(data, size) {}

template<typename T>
T ArraySequence<T>::Get(size_t index) const {
    return _data.Get(index);
}

template<typename T>
T ArraySequence<T>::GetFirst() const {
    return _data[0];
}

template<typename T>
T ArraySequence<T>::GetLast() const {
    return _data[_data.GetSize() - 1];
}

template<typename T>
T& ArraySequence<T>::operator[](size_t index) {
    return _data[index];
}

template<typename T>
const T& ArraySequence<T>::operator[](size_t index) const {
    return _data[index];
}

template<typename T>
void ArraySequence<T>::Set(const T& value, size_t index) {
    _data.Set(value, index);
}

template<typename T>
size_t ArraySequence<T>::GetSize() const {
    return _data.GetSize();
}

template<typename T>
Sequence<T>* ArraySequence<T>::GetSubSequence(size_t start_index, size_t end_index) const {
    if (start_index > end_index) {
        throw std::out_of_range("");
    }
    if (end_index >= _data.GetSize()) {
        throw std::out_of_range("");
    }
    ArraySequence* result = new ArraySequence;
    result->_data.Resize(end_index - start_index + 1);
    for (size_t i = start_index; i <= end_index; ++i) {
        result->_data[i - start_index] = _data[i];
    }
    return result;
}

template<typename T>
void ArraySequence<T>::Append(const T& elem) {
    _data.Resize(_data.GetSize() + 1);
    _data[_data.GetSize() - 1] = elem;
}

template<typename T>
void ArraySequence<T>::Prepend(const T& elem) {
    _data.Resize(_data.GetSize() + 1);
    for (size_t i = _data.GetSize() - 1; i > 0; --i) {
        _data[i] = _data[i - 1];
    }
    _data[0] = elem;
}

template<typename T>
void ArraySequence<T>::InsertAt(const T& elem, size_t index) {
    if (_data.GetSize() <= index) {
        throw std::out_of_range("");
    }
    _data.Resize(_data.GetSize() + 1);
    for (size_t i = _data.GetSize() - 1; i > index; --i) {
        _data[i] = _data[i - 1];
    }
    _data[index] = elem;
}

template<typename T>
Sequence<T>* ArraySequence<T>::Concat(const Sequence<T>* sequence) const {
    ArraySequence* result = new ArraySequence(*this);
    result->_data.Resize(result->GetSize() + sequence->GetSize());
    for (size_t i = 0; i < sequence->GetSize(); ++i) {
        result->_data[i + _data.GetSize()] = (*sequence)[i];
    }
    return result;
}


/*
template<typename T>
class LinkedListSequence : public Sequence<T> {
public:
    LinkedListSequence() = default;
    LinkedListSequence(const T* data, size_t size);
    LinkedListSequence(const LinkedListSequence& other) = default;

    T Get(size_t index) const override;
    T GetFirst()const  override;
    T GetLast() const override;
    T& operator[](size_t index) override;
    const T& operator[](size_t index) const override;
    void Set(const T& value, size_t index) override;
    size_t GetSize() const override;
    Sequence<T>* GetSubSequence(size_t start_index, size_t end_index) const override;
    void Append(const T& elem) override;
    void Prepend(const T& elem) override;
    void InsertAt(const T& elem, size_t index) override;
    Sequence<T>* Concat(const Sequence<T>* sequence) const override;

    void PopLast();
    void PopFirst();

    ~LinkedListSequence() = default;
private:
    LinkedList<T> _data;
};

template<typename T>
void LinkedListSequence<T>::PopLast() {
    _data.PopLast();
}

template<typename T>
void LinkedListSequence<T>::PopFirst() {
    _data.PopFirst();
}
template<typename T>
LinkedListSequence<T>::LinkedListSequence(const T* data, size_t size) : _data(data, size) {}

template<typename T>
T LinkedListSequence<T>::Get(size_t index) const {
    return _data.Get(index);
}

template<typename T>
T LinkedListSequence<T>::GetFirst() const {
    return _data.GetFirst();
}

template<typename T>
T LinkedListSequence<T>::GetLast() const {
    return _data.GetLast();
}

template<typename T>
T& LinkedListSequence<T>::operator[](size_t index) {
    return _data[index];
}

template<typename T>
const T& LinkedListSequence<T>::operator[](size_t index) const {
    return _data[index];
}

template<typename T>
void LinkedListSequence<T>::Set(const T& value, size_t index) {
    _data[index] = value;
}

template<typename T>
size_t LinkedListSequence<T>::GetSize() const {
    return _data.GetSize();
}

template<typename T>
Sequence<T>* LinkedListSequence<T>::GetSubSequence(size_t start_index, size_t end_index) const {
    LinkedList<T>* res_data = _data.GetSubList(start_index, end_index);
    LinkedListSequence<T>* res = new LinkedListSequence;
    for (size_t i = 0; i < res_data->GetSize(); ++i) {
        res->Append(res_data->Get(i));
    }
    delete res_data;
    return res;
}

template<typename T>
void LinkedListSequence<T>::Append(const T& elem) {
    _data.Append(elem);
}

template<typename T>
void LinkedListSequence<T>::Prepend(const T& elem) {
    _data.Prepend(elem);
}

template<typename T>
void LinkedListSequence<T>::InsertAt(const T& elem, size_t index) {
    _data.InsertAt(elem, index);
}

template<typename T>
Sequence<T>* LinkedListSequence<T>::Concat(const Sequence<T>* sequence) const {
    LinkedListSequence<T>* res = new LinkedListSequence(*this);
    for (size_t i = 0; i < sequence->GetSize(); ++i) {
        res->Append(sequence->Get(i));
    }
    return res;
}*/