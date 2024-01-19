#pragma once
#include "DataPack.h"
#include <string>
#include <vector>

enum class DataPachKey : BYTE
{
    FileStart = 0x81,
    FileEnd = 0x98,
    IdStart = 0xC7,
    IdEnd = 0xC8,
    ValueStart = 0x55,
    ValueEnd = 0x56,
    ValueStart_Small = 0x57,
    ValueStart_Small_X = 0x58,
    ChildStart = 0xD4,
    ChildEnd = 0xD5,
    ChildStart_Small = 0xD6,
    ChildStart_Small_X = 0xD7,
};

int DataPack::GetCount()
{
    return this->Child.size();
}
void DataPack::SetCount(int value)
{
    this->Child.resize(value);
}
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
    int index = 0;
    int bufferSize = 0;
    if (data_len < 4)//´íÎóµÄ¸ñÊ½
        return;
    if (data[0] == (BYTE)DataPachKey::FileStart)
    {
        bufferSize = *(int*)&data[1];
        index = 5;
    }
    else//´íÎóµÄÍ·²¿
    {
        return;
    }
    if (data[bufferSize - 1] != (BYTE)DataPachKey::FileEnd)//´íÎóµÄ½áÊø·û
    {
        return;
    }
    while (index < bufferSize - 1)
    {
        DataPachKey key = (DataPachKey)data[index];
        switch (key)
        {
        case DataPachKey::IdStart:
        {
            index += 1;
            uint16_t idLen = *(uint16_t*)&data[index];
            index += 2;
            if (data[index + idLen] != (BYTE)DataPachKey::IdEnd)//´íÎóµÄ½áÊø·û
                return;
            this->Id.assign((char*)&data[index], idLen);
            index += idLen + 1;
            break;
        }
        case DataPachKey::ValueStart:
        {
            index += 1;
            int valueLen = *(int*)&data[index];
            index += 4;
            if (data[index + valueLen] != (BYTE)DataPachKey::ValueEnd)//´íÎóµÄ½áÊø·û
                return;
            this->Value.assign(&data[index], &data[index + valueLen]);
            index += valueLen + 1;
            break;
        }
        case DataPachKey::ValueStart_Small:
        {
            index += 1;
            uint16_t valueLen = *(uint16_t*)&data[index];
            index += 2;
            if (data[index + valueLen] != (BYTE)DataPachKey::ValueEnd)//´íÎóµÄ½áÊø·û
                return;
            this->Value.assign(&data[index], &data[index + valueLen]);
            index += valueLen + 1;
            break;
        }
        case DataPachKey::ValueStart_Small_X:
        {
            index += 1;
            BYTE valueLen = *(BYTE*)&data[index];
            index += 1;
            if (data[index + valueLen] != (BYTE)DataPachKey::ValueEnd)//´íÎóµÄ½áÊø·û
                return;
            this->Value.assign(&data[index], &data[index + valueLen]);
            index += valueLen + 1;
            break;
        }
        case DataPachKey::ChildStart:
        {
            index += 1;
            int childLen = *(int*)&data[index];
            index += 4;
            if (data[index + childLen] != (BYTE)DataPachKey::ChildEnd)//´íÎóµÄ½áÊø·û
                return;
            DataPack childPack(&data[index], childLen);
            this->Child.push_back(childPack);
            index += childLen + 1;
            break;
        }
        case DataPachKey::ChildStart_Small:
        {
            index += 1;
            uint16_t childLen = *(uint16_t*)&data[index];
            index += 2;
            if (data[index + childLen] != (BYTE)DataPachKey::ChildEnd)//´íÎóµÄ½áÊø·û
                return;
            DataPack childPack(&data[index], childLen);
            this->Child.push_back(childPack);
            index += childLen + 1;
            break;
        }
        case DataPachKey::ChildStart_Small_X:
        {
            index += 1;
            BYTE childLen = *(BYTE*)&data[index];
            index += 1;
            if (data[index + childLen] != (BYTE)DataPachKey::ChildEnd)//´íÎóµÄ½áÊø·û
                return;
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

DataPack::DataPack(const char* key)
{
    this->Id = key;
    this->Value.resize(0);
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
void DataPack::Add(DataPack val)
{
    this->Child.push_back(val);
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
    std::vector<BYTE> list = std::vector<BYTE>(1);
    list[0] = (BYTE)DataPachKey::FileStart;
    if (this->Id.size() > 0)
    {
        list.resize(6);
        list[5] = (BYTE)DataPachKey::IdStart;
        uint16_t idlen = this->Id.size();
        list.insert(list.end(), (BYTE*)&idlen, (BYTE*)&idlen + 2);
        list.insert(list.end(), this->Id.c_str(), this->Id.c_str() + this->Id.size());
        list.push_back((BYTE)DataPachKey::IdEnd);
    }
    else
    {
        list.resize(5);
    }
    if (this->Value.size() > 0)
    {
        if (this->Value.size() > UINT16_MAX)
        {
            list.push_back((BYTE)DataPachKey::ValueStart);
            int vvlen = this->Value.size();
            list.insert(list.end(), (BYTE*)&vvlen, (BYTE*)&vvlen + 4);
        }
        else
        {
            if (this->Value.size() > UINT8_MAX)
            {
                list.push_back((BYTE)DataPachKey::ValueStart_Small);
                uint16_t vvlen = this->Value.size();
                list.insert(list.end(), (BYTE*)&vvlen, (BYTE*)&vvlen + 2);
            }
            else
            {
                list.push_back((BYTE)DataPachKey::ValueStart_Small_X);
                BYTE vvlen = this->Value.size();
                list.insert(list.end(), (BYTE*)&vvlen, (BYTE*)&vvlen + 1);
            }
        }
        list.insert(list.end(), this->Value.data(), this->Value.data() + this->Value.size());
        list.push_back((BYTE)DataPachKey::ValueEnd);
    }
    for (auto sub : this->Child)
    {
        auto dta = sub.GetBytes();
        if (dta.size() > UINT16_MAX)
        {
            list.push_back((BYTE)DataPachKey::ChildStart);
            int childLen = dta.size();
            list.insert(list.end(), (BYTE*)&childLen, (BYTE*)&childLen + 4);
        }
        else
        {
            if (dta.size() > UINT8_MAX)
            {
                list.push_back((BYTE)DataPachKey::ChildStart_Small);
                uint16_t childLen = dta.size();
                list.insert(list.end(), (BYTE*)&childLen, (BYTE*)&childLen + 2);
            }
            else
            {
                list.push_back((BYTE)DataPachKey::ChildStart_Small_X);
                BYTE childLen = dta.size();
                list.insert(list.end(), (BYTE*)&childLen, (BYTE*)&childLen + 1);
            }
        }
        list.insert(list.end(), dta.data(), dta.data() + dta.size());
        list.push_back((BYTE)DataPachKey::ChildEnd);
    }
    list.push_back((BYTE)DataPachKey::FileEnd);
    *(int*)&list[1] = list.size();
    return list;
}