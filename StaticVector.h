#ifndef STATICVECTOR_H
#define STATICVECTOR_H

// StaticVector is a class that represents a vector with a fixed size.
template <typename T, size_t MaxSize>
class StaticVector {
private:
    T data[MaxSize];
    size_t length;

public:
    StaticVector() : length(0) {}

    size_t size() const {
        return length;
    }

    bool empty() const {
        return length == 0;
    }

    void push_back(const T& value) {
        if (length < MaxSize) {
            data[length++] = value;
        }
    }

    void clear() {
        length = 0;
        //data[0] = 0;
    }

    // Returns the element at the given index.
    const T& operator[](size_t index) const {
        return data[index];
    }

    // Returns the element at the given index.
    T& operator[](size_t index) {
        return data[index];
    }

    // Returns a pointer to the last element.
    const T* end() const {
        return data + length;
    }

    // Returns a pointer to the last element.
    T* end() {
        return data + length;
    }

    // Returns a pointer to the last element.
    const T& back() const {
        return data[length - 1];
    }

    // Returns a reference to the last element.
    T& back() {
        return data[length - 1];
    } 

    // Returns a reference to the first element.
    const T& front() const {
        return data[0];
    }

    // Returns a pointer to the first element.
    T& front() {
        return data[0];
    }

    const T& pop_back() {
        if (length == 0) {
            return data[0];
        }
        return data[--length];
    }

    const T* begin() const {
        return data;
    }

    T* begin() {
        return data;
    }

    const T& pop_front() {
        if (length == 0) {
            return data[0];
        }
        T temp = data[0];
        for (size_t i = 0; i < length - 1; i++) {
            data[i] = data[i + 1];
        }
        length--;
        return temp;
    };

};

#endif // STATICVECTOR_H