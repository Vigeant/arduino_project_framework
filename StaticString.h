#ifndef STATICSTRING_H
#define STATICSTRING_H

#include <cstring>

template <int maxSize>
class StaticString {
private:
    char data[maxSize];
    int strLength;

public:
    StaticString();
    StaticString(const char* str);
    StaticString(const StaticString& other);
    ~StaticString();

    int length() const;
    int size() const;
    const char* getData() const;
    const char* c_str() const;

    StaticString& operator=(const StaticString& other);
    StaticString<maxSize> &operator=(const char *str);
    StaticString operator+(const StaticString &other) const;
    bool operator==(const StaticString& other) const;
    bool compare(const StaticString& other) const;
    bool operator!=(const StaticString& other) const;
    StaticString<maxSize> substring(StaticString<maxSize>& result, int start, int length) const;
    char& operator[](int index);
    const char& operator[](int index) const;

};



template <int maxSize>
StaticString<maxSize>::StaticString() : strLength(0) {
    data[0] = '\0';
}

template <int maxSize>
StaticString<maxSize>::StaticString(const char* str) : strLength(strlen(str)) {
    strncpy(data, str, maxSize - 1);
    data[maxSize - 1] = '\0';
}

template <int maxSize>
StaticString<maxSize>::StaticString(const StaticString& other) : strLength(other.strLength) {
    strncpy(data, other.data, maxSize);
}

template <int maxSize>
StaticString<maxSize>::~StaticString() {
    // No dynamic memory to deallocate
}

template <int maxSize>
int StaticString<maxSize>::length() const {
    return strLength;
}

template <int maxSize>
int StaticString<maxSize>::size() const {
    return strLength;
}

template <int maxSize>
const char* StaticString<maxSize>::getData() const {
    return data;
}

template <int maxSize>
const char* StaticString<maxSize>::c_str() const {
    return data;
}

template <int maxSize>
StaticString<maxSize>& StaticString<maxSize>::operator=(const StaticString& other) {
    if (this != &other) {
        strLength = other.strLength;
        strncpy(data, other.data, maxSize);
    }
    return *this;
}

template <int maxSize>
StaticString<maxSize>& StaticString<maxSize>::operator=(const char* str) {
    strLength = strlen(str);
    strncpy(data, str, maxSize - 1);
    data[maxSize - 1] = '\0';
    return *this;
}

template <int maxSize>
StaticString<maxSize> StaticString<maxSize>::operator+(const StaticString& other) const {
    StaticString<maxSize> result;
    strncpy(result.data, data, maxSize);
    strncat(result.data, other.data, maxSize - strLength);
    result.strLength = strLength + other.strLength;
    return result;
}

template <int maxSize>
bool StaticString<maxSize>::operator==(const StaticString& other) const {
    return (strLength == other.strLength) && (strncmp(data, other.data, maxSize) == 0);
}

template <int maxSize>
bool StaticString<maxSize>::compare (const StaticString& other) const {
    return (strLength == other.strLength) && (strncmp(data, other.data, maxSize) == 0);
}

template <int maxSize>
bool StaticString<maxSize>::operator!=(const StaticString& other) const {
    return !(*this == other);
}

template <int maxSize>
inline StaticString<maxSize> StaticString<maxSize>::substring(StaticString<maxSize> &result, int start, int length) const
{
    result = "";
    char tempData[256];
    if (start < 0 || start >= strLength || length <= 0) {
        return result;
    }
    if (start + length > strLength) {
        length = strLength - start;
    }
    strncpy(tempData, data + start, length);
    result = tempData;
    return result;
}

template <int maxSize>
inline char &StaticString<maxSize>::operator[](int index)
{
    return data[index];
}

template <int maxSize>
inline const char &StaticString<maxSize>::operator[](int index) const
{
    return data[index];
}

#endif // STATICSTRING_H
