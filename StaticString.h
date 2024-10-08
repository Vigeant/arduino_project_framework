#ifndef STATICSTRING_H
#define STATICSTRING_H

#include <cstring>

template <int maxSize>
class StaticString
{
private:
    char data[maxSize];
    int strLength;

public:
    StaticString();
    StaticString(const char *str);
    StaticString(const StaticString &other);
    ~StaticString();

    int length() const;
    int size() const;
    const char *getData() const;
    const char *c_str() const;
    void setCharAt(const int index, char c);
    void removeCharAt(const int index);

    StaticString &operator=(const StaticString &other);
    StaticString<maxSize> &operator=(const char *str);
    StaticString operator+(const StaticString &other) const;
    bool operator==(const StaticString &other) const;
    bool compare(const StaticString &other) const;
    bool operator!=(const StaticString &other) const;
    StaticString<maxSize> substring(StaticString<maxSize> &result, int start, int length) const;
    // char& operator[](int index);
    const char &operator[](int index) const;
    StaticString<maxSize> &operator+=(const StaticString &other);
    StaticString<maxSize> &operator+=(const char c);
};

template <int maxSize>
StaticString<maxSize>::StaticString() : strLength(0)
{
    data[0] = '\0';
}

template <int maxSize>
StaticString<maxSize>::StaticString(const char *str) : strLength(strlen(str))
{
    strncpy(data, str, maxSize - 1);
    data[maxSize - 1] = '\0';
}

template <int maxSize>
StaticString<maxSize>::StaticString(const StaticString &other) : strLength(other.strLength)
{
    strncpy(data, other.data, maxSize);
}

template <int maxSize>
StaticString<maxSize>::~StaticString()
{
    // No dynamic memory to deallocate
}

template <int maxSize>
int StaticString<maxSize>::length() const
{
    return strLength;
}

template <int maxSize>
int StaticString<maxSize>::size() const
{
    return strLength;
}

template <int maxSize>
const char *StaticString<maxSize>::getData() const
{
    return data;
}

template <int maxSize>
const char *StaticString<maxSize>::c_str() const
{
    return data;
}

template <int maxSize>
void StaticString<maxSize>::setCharAt(const int index, char c)
{
    if (index >= 0 && index < strLength)
    {
        data[index] = c;
    }
}

template <int maxSize>
void StaticString<maxSize>::removeCharAt(const int index)
{
    if (index >= 0 && index < strLength)
    {
        for (int i = index; i < strLength - 1; i++)
        {
            data[i] = data[i + 1];
        }
        data[strLength - 1] = '\0';
        strLength--;
    }
}

template <int maxSize>
StaticString<maxSize> &StaticString<maxSize>::operator=(const StaticString &other)
{
    if (this != &other)
    {
        strLength = other.strLength;
        strncpy(data, other.data, maxSize);
    }
    return *this;
}

template <int maxSize>
StaticString<maxSize> &StaticString<maxSize>::operator=(const char *str)
{
    strLength = strlen(str);
    strncpy(data, str, maxSize - 1);
    data[maxSize - 1] = '\0';
    return *this;
}

template <int maxSize>
StaticString<maxSize> StaticString<maxSize>::operator+(const StaticString &other) const
{
    StaticString<maxSize> result;
    strncpy(result.data, data, maxSize);
    strncat(result.data, other.data, maxSize - strLength);
    result.strLength = strLength + other.strLength;
    return result;
}

template <int maxSize>
bool StaticString<maxSize>::operator==(const StaticString &other) const
{
    return (strLength == other.strLength) && (strncmp(data, other.data, maxSize) == 0);
}

template <int maxSize>
bool StaticString<maxSize>::compare(const StaticString &other) const
{
    return strncmp(data, other.data, maxSize);
}

template <int maxSize>
bool StaticString<maxSize>::operator!=(const StaticString &other) const
{
    return !(*this == other);
}

template <int maxSize>
inline StaticString<maxSize> StaticString<maxSize>::substring(StaticString<maxSize> &result, int start, int length) const
{
    result = "";
    char tempData[256];
    memset(tempData, 0, sizeof(tempData));
    if (start < 0 || start >= strLength || length <= 0)
    {
        return result;
    }
    if (start + length > strLength)
    {
        length = strLength - start;
    }
    if (length > maxSize)
    {
        length = maxSize;
    } else if (length > sizeof(tempData) - 1){
        length = sizeof(tempData) - 1;
    }
    strncpy(tempData, data + start, length);
    result = tempData;
    return result;
}

/*
template <int maxSize>
inline char &StaticString<maxSize>::operator[](int index)
{
    return data[index];
}*/

template <int maxSize>
inline const char &StaticString<maxSize>::operator[](int index) const
{
    return data[index];
}

template <int maxSize>
StaticString<maxSize> &StaticString<maxSize>::operator+=(const StaticString &other)
{
    if (strLength + other.strLength < maxSize)
    {
        strncat(data, other.data, other.strLength);
        strLength += other.strLength;
    }
    else
    {
        strncat(data, other.data, maxSize - strLength - 1);
        strLength = maxSize - 1;
    }
    return *this;
}

template <int maxSize>
StaticString<maxSize> &StaticString<maxSize>::operator+=(const char c) {
    if (strLength < maxSize - 1) {
        data[strLength++] = c;
        data[strLength] = '\0';
    }
    return *this;
}

#endif // STATICSTRING_H
