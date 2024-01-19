#pragma once
#include "defines.h"
#include <string>
#include <vector>
class DataPack
{
public:
    std::string Id;
    std::vector<BYTE> Value;
    std::vector<DataPack> Child = std::vector<DataPack>();
    DataPack& operator[](int index);
    DataPack& operator[](std::string id);
    __declspec(property (put = SetCount, get = GetCount)) int Count;
    int GetCount();
    void SetCount(int value);
    void operator=(const std::initializer_list<BYTE> data);
    void operator=(const std::initializer_list<BYTE>* data);
    template<typename T>
    void operator=(T data)
    {
        this->Value.resize(sizeof(T));
        memcpy(this->Value.data(), &data, sizeof(T));
    }
    template<typename T>
    void operator=(std::vector<T> data)
    {
        this->Value.resize(data.size() * sizeof(T));
        memcpy(this->Value.data(), data.data(), data.size() * sizeof(T));
    }
    template<typename T>
    void operator=(std::initializer_list<T> data)
    {
        this->Value.resize(data.size());
        memcpy(this->Value.data(), data.begin(), data.size() * sizeof(T));
    }
    template<typename T>
    T operator=(DataPack data)
    {
        T result = T();
        memcpy(&result, data.Value.data(), sizeof(T));
        return result;
    }
    void operator=(const char* data);
    void operator=(const wchar_t* data);
    void operator=(char* data);
    void operator=(wchar_t* data);
    void operator=(std::string data);
    void operator=(std::wstring data);
    template<typename T>
    DataPack(T data)
    {
        this->Id = "";
        this->Value.resize(sizeof(T));
        memcpy(this->Value.data(), &data, sizeof(T));
    }
    template<typename T>
    DataPack(std::string id, T data)
    {
        this->Id = id;
        this->Value.resize(sizeof(T));
        memcpy(this->Value.data(), &data, sizeof(T));
    }
    DataPack();
    DataPack(const char* key);
    DataPack(const BYTE* data, int data_len);
    DataPack(std::string id, BYTE* data, int len);
    DataPack(std::vector<BYTE> data);
    DataPack(std::initializer_list<BYTE> data);
    DataPack(std::string id, std::string data);
    DataPack(std::string id, std::wstring data);
    DataPack(std::string id, char* data);
    DataPack(std::string id, const char* data);
    DataPack(std::string id, wchar_t* data);
    DataPack(std::string id, const wchar_t* data);

    void Add(DataPack val);
    template<typename T>
    DataPack& Add(std::string key, T val)
    {
        DataPack item = DataPack(key, val);
        item.Id = key;
        this->Child.push_back(item);
        return item;
    }
    template<typename T>
    DataPack& Add(T val)
    {
        DataPack item = DataPack("", val);
        this->Child.push_back(item);
        return item;
    }
    template<typename T>
    T convert()
    {
        return *(T*)this->Value.data();
    }
    void RemoveAt(int index);
    std::vector<BYTE> GetBytes();
    void clear();
    int size();
};