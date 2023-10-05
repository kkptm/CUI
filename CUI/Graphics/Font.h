#pragma once
#include <D2D1.h>
#include <vector>
#include <string>
#include <dwrite.h>
#ifndef PROPERTY
#define PROPERTY(t,n) __declspec( property (put = property__set_##n, get = property__get_##n)) t n
#define GET(t,n) t property__get_##n() 
#define SET(t,n) void property__set_##n(t value)

#define READONLY_PROPERTY(t,n) __declspec( property (get = property__get_##n) ) t n
#define WRITEONLY_PROPERTY(t,n) __declspec( property (put = property__set_##n) ) t n
#define _readonly_property(t,n) READONLY_PROPERTY(t,n);GET(t,n)
#endif // !PROPERTY
class Font
{
private:
    float _fontSize;
    std::wstring _fontName;
    IDWriteTextFormat* _fontObject;
public:
    float FontHeight;
    Font(std::wstring fontFamilyName, float _fontsize);
    ~Font();
    READONLY_PROPERTY(IDWriteTextFormat*, FontObject);
    GET(IDWriteTextFormat*, FontObject);
    PROPERTY(float, FontSize);
    GET(float, FontSize);
    SET(float, FontSize);
    PROPERTY(std::wstring, FontName);
    GET(std::wstring, FontName);
    SET(std::wstring, FontName);
    D2D1_SIZE_F GetTextSize(std::wstring str, float w = FLT_MAX, float h = FLT_MAX);
    D2D1_SIZE_F GetTextSize(std::string str, float w = FLT_MAX, float h = FLT_MAX);
    D2D1_SIZE_F GetTextSize(wchar_t c);
    int HitTestTextPosition(std::wstring str, float x, float y);
    int HitTestTextPosition(std::wstring str, float width, float height, float x, float y);
    std::vector<DWRITE_HIT_TEST_METRICS> HitTestTextRange(std::wstring str, UINT32 start, UINT32 len);
    std::vector<DWRITE_HIT_TEST_METRICS> HitTestTextRange(std::wstring str, float width, float height, UINT32 start, UINT32 len);
    int HitTestTextPosition(IDWriteTextLayout* textLayout, float x, float y);
    int HitTestTextPosition(IDWriteTextLayout* textLayout, float width, float height, float x, float y);
    std::vector<DWRITE_HIT_TEST_METRICS> HitTestTextRange(IDWriteTextLayout* textLayout, UINT32 start, UINT32 len);
    std::vector<DWRITE_HIT_TEST_METRICS> HitTestTextRange(IDWriteTextLayout* textLayout, float width, float height, UINT32 start, UINT32 len);
    static std::vector<std::wstring> GetSystemFonts();
};

