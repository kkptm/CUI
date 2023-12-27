#include "DataPack.h"
#pragma once
#include <Windows.h>
#include <string>
#include <vector>

enum class DataPachKey : BYTE
{
    FileStart = 0x81,
    FileEnd = 0x82,
    IdStart = 0xC7,
    IdEnd = 0xC8,
    ValueStart = 0x55,
    ValueEnd = 0x56,
    ChildStart = 0xD4,
    ChildEnd = 0xD5,
};

DataPack& DataPack::operator[](int index)
{
    return this->Child[index];
}
DataPack& DataPack::operator[](std::string id)
{
    for (int i = 0; i < Child.size(); i++)
    {
        if (this->Child[i].Id == id)
        {
            return this->Child[i];
        }
    }
    Child.push_back(DataPack(id, 0));
    return this->Child[this->Child.size() - 1];
}
GET_CPP(DataPack, int, Count)
{
    return this->Child.size();
}
SET_CPP(DataPack, int, Count)
{
    this->Child.resize(value);
}

void DataPack::operator=(const std::initializer_list<BYTE> data)
{
    this->Value.resize(data.size());
    memcpy(this->Value.data(), data.begin(), data.size());
}
void DataPack::operator=(const std::initializer_list<BYTE>* data)
{
    this->Value.resize(data->size());
    memcpy(this->Value.data(), data->begin(), data->size());
}
void DataPack::operator=(const char* data)
{
    std::string str = data;
    this->Value.resize(str.size());
    memcpy(this->Value.data(), str.c_str(), str.size());
}
void DataPack::operator=(const wchar_t* data)
{
    std::wstring str = data;
    this->Value.resize(str.size() * 2);
    memcpy(this->Value.data(), str.c_str(), str.size() * 2);
}
void DataPack::operator=(char* data)
{
    std::string str = data;
    this->Value.resize(str.size());
    memcpy(this->Value.data(), str.c_str(), str.size());
}
void DataPack::operator=(wchar_t* data)
{
    std::wstring str = data;
    this->Value.resize(str.size() * 2);
    memcpy(this->Value.data(), str.c_str(), str.size() * 2);
}
void DataPack::operator=(std::string data)
{
    this->Value.resize(data.size());
    memcpy(this->Value.data(), data.c_str(), data.size());
}
void DataPack::operator=(std::wstring data)
{
    this->Value.resize(data.size() * 2);
    memcpy(this->Value.data(), data.c_str(), data.size() * 2);
}
void DataPack::RemoveAt(int index)
{
    this->Child.erase(this->Child.begin() + index);
}
DataPack::DataPack() :Id(""), Value(std::vector<BYTE>()) {}
DataPack::DataPack(const BYTE* data, int data_len)
{
    if (data_len < 6)
    {
        //throw std::runtime_error("Invalid data format");
        return;
    }
    if (data[0] != (BYTE)DataPachKey::FileStart)
    {
        //throw std::runtime_error("Invalid data Start");
        return;
    }
    int totalLen = *(int*)&data[1];
    if (data[totalLen - 1] != (BYTE)DataPachKey::FileEnd)
    {
        //throw std::runtime_error("Invalid data End");
        return;
    }
    int index = 5;
    while (index < data_len - 1)
    {
        DataPachKey key = (DataPachKey)data[index];
        switch (key)
        {
        case DataPachKey::IdStart:
        {
            index += 1;
            int idLen = *(int*)&data[index];
            index += 4;
            this->Id.assign((char*)&data[index], idLen);
            index += idLen + 1;
            break;
        }
        case DataPachKey::ValueStart:
        {
            index += 1;
            int valueLen = *(int*)&data[index];
            index += 4;
            this->Value.assign(&data[index], &data[index + valueLen]);
            index += valueLen + 1;
            break;
        }
        case DataPachKey::ChildStart:
        {
            index += 1;
            int childLen = *(int*)&data[index];
            index += 4;
            DataPack childPack(&data[index], childLen);
            this->Child.push_back(childPack);
            index += childLen + 1;
            break;
        }
        default:
        {
            //throw std::runtime_error("Unknown key");
            return;
        }
        }
    }
}
DataPack::DataPack(std::string id, BYTE* data, int len)
{
    this->Id = id;
    this->Value.resize(len);
    RtlCopyMemory(this->Value.data(), data, len);
}
DataPack::DataPack(std::vector<BYTE> data) :DataPack(data.data(), data.size()) {}
DataPack::DataPack(std::initializer_list<BYTE> data) :DataPack((BYTE*)data.begin(), data.size()) {}
DataPack::DataPack(std::string id, std::string data)
{
    this->Id = id;
    this->Value.resize(data.size());
    memcpy(this->Value.data(), data.c_str(), data.size());
}
DataPack::DataPack(std::string id, std::wstring data)
{
    this->Id = id;
    this->Value.resize(data.size() * 2);
    memcpy(this->Value.data(), data.c_str(), data.size() * 2);
}
DataPack::DataPack(std::string id, char* data)
{
    this->Id = id;
    std::string str = data;
    this->Value.resize(str.size());
    memcpy(this->Value.data(), str.c_str(), str.size());
}
DataPack::DataPack(std::string id, const char* data)
{
    this->Id = id;
    std::string str = data;
    this->Value.resize(str.size());
    memcpy(this->Value.data(), str.c_str(), str.size());
}
DataPack::DataPack(std::string id, wchar_t* data)
{
    this->Id = id;
    std::wstring str = data;
    this->Value.resize(str.size() * 2);
    memcpy(this->Value.data(), str.c_str(), str.size() * 2);
}
DataPack::DataPack(std::string id, const wchar_t* data)
{
    this->Id = id;
    std::wstring str = data;
    this->Value.resize(str.size() * 2);
    memcpy(this->Value.data(), str.c_str(), str.size() * 2);
}
void DataPack::clear()
{
    this->Child.clear();
}
int DataPack::size()
{
    return this->Child.size();
}

std::vector<BYTE> DataPack::GetBytes()
{
    std::vector<BYTE> list = std::vector<BYTE>();
    list.resize(6);
    list[0] = (BYTE)DataPachKey::FileStart;
    list[5] = (BYTE)DataPachKey::IdStart;
    int idlen = this->Id.size();
    list.insert(list.end(), (BYTE*)&idlen, (BYTE*)&idlen + 4);
    list.insert(list.end(), this->Id.c_str(), this->Id.c_str() + this->Id.size());
    list.push_back((BYTE)DataPachKey::IdEnd);
    list.push_back((BYTE)DataPachKey::ValueStart);
    int vvlen = this->Value.size();
    list.insert(list.end(), (BYTE*)&vvlen, (BYTE*)&vvlen + 4);
    list.insert(list.end(), this->Value.data(), this->Value.data() + this->Value.size());
    list.push_back((BYTE)DataPachKey::ValueEnd);
    for (auto sub : this->Child)
    {
        auto dta = sub.GetBytes();
        list.push_back((BYTE)DataPachKey::ChildStart);
        int childLen = dta.size();
        list.insert(list.end(), (BYTE*)&childLen, (BYTE*)&childLen + 4);
        list.insert(list.end(), dta.data(), dta.data() + dta.size());
        list.push_back((BYTE)DataPachKey::ChildEnd);
    }
    list.push_back((BYTE)DataPachKey::FileEnd);
    *(int*)&list[1] = list.size();
    return list;
}