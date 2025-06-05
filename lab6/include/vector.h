#pragma once
#include <algorithm>
#include <stdexcept>

/*
 * Class template Vector
 */
template<class T>
class Vector {
public:
    Vector();
    Vector(int size);
    Vector(const Vector<T>& r);
    ~Vector();

    T& operator[](int index);
    T& at(int index);

    int size() const;
    void push_back(const T& x);
    void clear();
    bool empty() const;

private:
    void inflate();

    T *m_pElements;     // The pointer to the first element in the vector
    int m_nSize;        // The number of elements in the vector
    int m_nCapacity;    // The length of the allocated memory
};


/*
 * @brief Default constructor. Creates an empty vector.
 */
template<class T>
Vector<T>::Vector(): m_pElements(nullptr), m_nSize(0), m_nCapacity(0) {}

/*
 * @brief Creates a new vector with the given size. The elements are default-constructed.
 * @param size The size of the vector.
 */
template<class T>
Vector<T>::Vector(int size): m_nSize(size), m_nCapacity(size) {
    m_pElements = new T[size];
}

/*
 * @brief Copy constructor. Creates a new vector with the same elements as the given vector.
 */
template<class T>
Vector<T>::Vector(const Vector<T>& r): m_nSize(r.m_nSize), m_nCapacity(r.m_nCapacity) {
    m_pElements = new T[r.m_nCapacity];
    std::copy(r.m_pElements, r.m_pElements + r.m_nSize, m_pElements);
}

/*
 * @brief Destructor. Deallocates the memory used by the vector.
 */
template<class T>
Vector<T>::~Vector() {
    delete[] m_pElements;
}

/*
 * @brief Return the element at the given index.
 * @param index The index of the element to return.
 */
template<class T>
T& Vector<T>::operator[](int index) {
    return m_pElements[index];
}

/*
 * @brief Return the element at the given index. If the index is out of range, throw an exception.
 * @param index The index of the element to return.
 */
template<class T>
T& Vector<T>::at(int index) {
    if (index < 0 || index >= m_nSize) {
        throw std::out_of_range("Index out of range");
    }
    return m_pElements[index];
}

/*
 * @brief Return the number of elements in the vector.
 */
template<class T>
int Vector<T>::size() const {
    return m_nSize;
}

/*
 * @brief Append a new element to the end of the vector.
 * @param value The value of the new element.
 */
template<class T>
void Vector<T>::push_back(const T& value) {
    if (m_nSize == m_nCapacity) {
        inflate();
    }
    m_pElements[m_nSize++] = value;
}

/*
 * @brief Clear the vector and deallocate the memory.
 */
template<class T>
void Vector<T>::clear() {
    delete[] m_pElements;
    m_pElements = nullptr;
    m_nSize = 0;
    m_nCapacity = 0;
}

/*
 * @brief Return true if the vector is empty, false otherwise.
 */
template<class T>
bool Vector<T>::empty() const {
    return m_nSize == 0;
}

/*
 * @brief Inflates the capacity of the vector by doubling it.
 */
template<class T>
void Vector<T>::inflate() {
    if (m_nCapacity == 0) {
        m_nCapacity = 1;
        m_pElements = new T[m_nCapacity];
    } else {
        m_nCapacity *= 2;
        T *new_pElements = new T[m_nCapacity];
        std::copy(m_pElements, m_pElements + m_nSize, new_pElements);
        delete[] m_pElements;
        m_pElements = new_pElements;
    }
}
