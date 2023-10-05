#pragma once
#include <windows.h>
#include <vector>
#ifndef PROPERTY
#define PROPERTY(t,n) __declspec( property (put = property__set_##n, get = property__get_##n)) t n
#define READONLY_PROPERTY(t,n) __declspec( property (get = property__get_##n) ) t n
#define WRITEONLY_PROPERTY(t,n) __declspec( property (put = property__set_##n) ) t n
#define GET(t,n) t property__get_##n() 
#define SET(t,n) void property__set_##n(t value)
#define _readonly_property(t,n) READONLY_PROPERTY(t,n);GET(t,n)
#endif // !PROPERTY

#pragma warning(disable: 4267)
#pragma warning(disable: 4244)
#pragma warning(disable: 4018)
template<typename T, class _Alloc = std::allocator<T>>
class List : public std::vector<T, _Alloc>
{
public:
    List() {}
    List(T* val, int len)
    {
        for (int i = 0; i < len; i++)
        {
            this->push_back(val[i]);
        }
    }
    List(std::initializer_list<T> val)
    {
        for (int i = 0; i < val.size(); i++)
        {
            this->push_back(val.begin()[i]);
        }
    }
    List(std::vector< T> val)
    {
        for (int i = 0; i < val.size(); i++)
        {
            this->push_back(val[i]);
        }
    }
    ~List() {}
    void operator=(std::vector<T>& val)
    {
        *(std::vector<T>*)this = val;
	}
    void operator=(std::vector<T>* val)
    {
		*(std::vector<T>*)this = *val;
    }
    T& operator[](int index)
    {
        return this->data()[index];
    }
    PROPERTY(int, Count);
    GET(int, Count)
    {
        return this->size();
    }
    SET(int, Count)
    {
        this->resize(value);
    }
    void Add(T val)
    {
        this->push_back(val);
    }
    void Clear()
    {
        this->clear();
    }
    void AddRange(T* val, int len)
    {
        this->insert(this->end(), val, val + len);
    }
    void AddRange(std::initializer_list<T> val)
    {
        this->insert(this->end(), val);
    }
    void AddRange(List<T> val)
    {
        this->insert(this->end(), val.data(), val.data() + val.size());
    }
    void AddRange(List<T>* val)
    {
        this->insert(this->end(), val->data(), val->data() + val->size());
    }
    void AddRange(std::vector<T>& val)
    {
        this->insert(this->end(), val.data(), val.data() + val.size());
    }
    void AddRange(std::vector<T>* val)
    {
        this->insert(this->end(), val->data(), val->data() + val->size());
    }
    void Insert(int index, T val)
    {
        if (index >= this->size())
        {
            this->push_back(val);
        }
        else if (index >= 0)
        {
            this->insert(this->begin() + index, val);
        }
    }
    void Insert(int index, std::initializer_list<T>& val)
    {
        if (index >= this->size())
        {
            this->AddRange(val);
        }
        else if (index >= 0)
        {
            this->insert(this->begin() + index, val.begin(), val.end() - 1);
        }
    }
    void Insert(int index, std::vector<T>& val)
    {
        if (index >= this->size())
        {
            this->AddRange(val);
        }
        else if (index >= 0)
        {
            this->insert(this->begin() + index, val.begin(), val.end() - 1);
        }
    }
    void Insert(int index, std::vector<T>* val)
    {
        if (index >= this->size())
        {
            this->AddRange(val);
        }
        else if (index >= 0)
        {
            this->insert(this->begin() + index, val->begin(), val->end() - 1);
        }
    }
    void Insert(int index, List<T>& val)
    {
        if (index >= this->size())
        {
            this->AddRange(val);
        }
        else if (index >= 0)
        {
            this->insert(this->begin() + index, val.begin(), val.end() - 1);
        }
    }
    void Insert(int index, List<T>* val)
    {
        if (index >= this->size())
        {
            this->AddRange(val);
        }
        else if (index >= 0)
        {
            this->insert(this->begin() + index, val->begin(), val->end() - 1);
        }
    }
    void RemoveAt(int index)
    {
        if (index >= 0 && index < this->size())
        {
            this->erase(this->begin() + index);
        }
    }
    int IndexOf(T& value)
    {
        for (int i = 0; i < this->Count; i++)
        {
            if (this->at(i) == value)
            {
                return i;
            }
        }
        return -1;
    }
    bool Contains(T& value)
    {
        return IndexOf(value) >= 0;
    }
    int LastIndexOf(T& value)
    {
        for (int i = this->Count - 1; i >= 0; i--)
        {
            if (this->at(i) == value)
            {
                return i;
            }
        }
        return -1;
    }
    int Remove(T item)
    {
        int num = 0;
        for (int i = this->Count; i >= 0; i--)
        {
            if (this->data()[i] == item)
            {
                RemoveAt(i);
                num += 1;
            }
        }
        return num;
    }
    void Swap(int from, int to)
    {
        std::swap(this->data()[from], this->data()[to]);
    }
    std::vector<T>& GetVector()
    {
        return *(std::vector<T>*)this;
    }
    T* Pointer()
    {
        return this->data();
    }
    T& get(int i)
    {
        return this->data()[i];
    }
    void set(int i, T val)
    {
        this->data()[i] = val;
    }
    T& First()
    {
        return this->data()[0];
    }
    T& Last()
    {
        return this->data()[this->size() - 1];
    }
    void Reverse()
	{
		std::reverse(this->begin(), this->end());
	}
};