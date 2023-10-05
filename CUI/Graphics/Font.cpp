#include "Font.h"
#include "Factory.h"

#pragma warning(disable: 4267)
#pragma warning(disable: 4244)
#pragma warning(disable: 4018)

#ifndef PROPERTY_CPP
#define GET_CPP(c,t,n) t c::property__get_##n() 
#define SET_CPP(c,t,n) void c::property__set_##n(t value)
#endif // !PROPERTY_CPP

Font::Font(std::wstring fontFamilyName, float _fontsize)
{
    this->_fontSize = _fontsize;
    this->_fontName = fontFamilyName;
    _DWriteFactory->CreateTextFormat(
        this->_fontName.c_str(),
        NULL,
        DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        this->_fontSize,
        L"",
        &_fontObject);
    this->FontHeight = this->GetTextSize(L"I").height;
}
Font::~Font()
{
    if (this->_fontObject) this->_fontObject->Release();
}
GET_CPP(Font, IDWriteTextFormat*, FontObject)
{
    return this->_fontObject;
}
GET_CPP(Font, float, FontSize)
{
    return this->_fontSize;
}
SET_CPP(Font, float, FontSize)
{
    if (value != this->_fontSize && this->_fontObject)
    {
        this->_fontObject->Release();
        this->_fontObject = NULL;
        _DWriteFactory->CreateTextFormat(
            this->_fontName.c_str(),
            NULL,
            DWRITE_FONT_WEIGHT_NORMAL,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            this->FontSize,
            L"",
            &_fontObject);
    }
    this->_fontSize = value;
    this->FontHeight = this->GetTextSize(L"I").height;
}
GET_CPP(Font, std::wstring, FontName)
{
    return this->_fontName;
}
SET_CPP(Font, std::wstring, FontName)
{
    if (value != this->_fontName && this->_fontObject)
    {
        this->_fontObject->Release();
        this->_fontObject = NULL;
        _DWriteFactory->CreateTextFormat(
            this->_fontName.c_str(),
            NULL,
            DWRITE_FONT_WEIGHT_NORMAL,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            this->FontSize,
            L"",
            &_fontObject);
    }
    this->_fontName = value;
    this->FontHeight = this->GetTextSize(L"I").height;
}
D2D1_SIZE_F Font::GetTextSize(std::wstring str, float w, float h)
{
    D2D1_SIZE_F minSize = { 0,0 };
    IDWriteTextLayout* textLayout = 0;
    HRESULT hr = _DWriteFactory->CreateTextLayout(
        str.c_str(),
        str.size(),
        this->_fontObject,
        w,
        h,
        &textLayout);
    if (SUCCEEDED(hr))
    {
        DWRITE_TEXT_METRICS metrics;
        hr = textLayout->GetMetrics(&metrics);
        textLayout->Release();
        if (SUCCEEDED(hr))
        {
            minSize = D2D1::Size((float)ceil(metrics.widthIncludingTrailingWhitespace), (float)ceil(metrics.height));
            return minSize;
        }
    }
    return { 0,0 };
}
D2D1_SIZE_F Font::GetTextSize(std::string str, float w, float h)
{
    if (str.size() == 0)return { 0,0 };
    int iLength = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size() + 1, NULL, 0);
    wchar_t* tchar = new wchar_t[sizeof(wchar_t) * iLength];
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size() + 1, tchar, iLength);
    IDWriteTextLayout* textLayout = 0;
    HRESULT hr = _DWriteFactory->CreateTextLayout(
        tchar,
        str.size(),
        this->_fontObject,
        w,
        h,
        &textLayout);
    if (SUCCEEDED(hr))
    {
        DWRITE_TEXT_METRICS metrics;
        hr = textLayout->GetMetrics(&metrics);
        textLayout->Release();
        if (SUCCEEDED(hr))
        {
            D2D1_SIZE_F minSize = D2D1::Size((float)ceil(metrics.widthIncludingTrailingWhitespace), (float)ceil(metrics.height));
            delete[] tchar;
            return minSize;
        }
    }
    delete[] tchar;
    return { 0,0 };
}
D2D1_SIZE_F Font::GetTextSize(wchar_t c)
{
    D2D1_SIZE_F minSize = { 0,0 };
    IDWriteTextLayout* textLayout = 0;
    HRESULT hr = _DWriteFactory->CreateTextLayout(
        &c,
        1,
        this->_fontObject,
        FLT_MAX,
        FLT_MAX,
        &textLayout);
    if (SUCCEEDED(hr))
    {
        DWRITE_TEXT_METRICS metrics;
        hr = textLayout->GetMetrics(&metrics);
        textLayout->Release();
        if (SUCCEEDED(hr))
        {
            minSize = D2D1::Size((float)ceil(metrics.widthIncludingTrailingWhitespace), (float)ceil(metrics.height));
            return minSize;
        }
    }
    return { 0,0 };
}
int Font::HitTestTextPosition(std::wstring str, float x, float y)
{
    if (str.size() == 0) return -1;
    IDWriteTextLayout* textLayout = NULL;
    HRESULT hr = _DWriteFactory->CreateTextLayout(
        str.c_str(),
        str.size(),
        this->_fontObject,
        FLT_MAX,
        FLT_MAX,
        &textLayout);
    if (SUCCEEDED(hr))
    {
        BOOL isTrailingHit;
        BOOL isInside;
        DWRITE_HIT_TEST_METRICS caretMetrics;
        textLayout->HitTestPoint(
            x,
            y,
            &isTrailingHit,
            &isInside,
            &caretMetrics
        );
        textLayout->Release();
        return isTrailingHit ? caretMetrics.textPosition + 1 : caretMetrics.textPosition;
    }
    return -1;
}
int Font::HitTestTextPosition(std::wstring str, float width, float height, float x, float y)
{
    if (str.size() == 0) return -1;
    IDWriteTextLayout* textLayout = NULL;
    HRESULT hr = _DWriteFactory->CreateTextLayout(
        str.c_str(),
        str.size(),
        this->_fontObject,
        width,
        height,
        &textLayout);
    if (SUCCEEDED(hr))
    {
        BOOL isTrailingHit;
        BOOL isInside;
        DWRITE_HIT_TEST_METRICS caretMetrics;
        textLayout->HitTestPoint(
            x,
            y,
            &isTrailingHit,
            &isInside,
            &caretMetrics
        );
        textLayout->Release();
        if (caretMetrics.width > 0.0f && x - caretMetrics.left >= caretMetrics.width * 0.5f)
        {
            caretMetrics.textPosition += 1;
        }
        return caretMetrics.textPosition;
    }
    return -1;
}
int Font::HitTestTextPosition(IDWriteTextLayout* textLayout, float x, float y)
{
    if (textLayout)
    {
        BOOL isTrailingHit;
        BOOL isInside;
        DWRITE_HIT_TEST_METRICS caretMetrics;
        textLayout->HitTestPoint(
            x,
            y,
            &isTrailingHit,
            &isInside,
            &caretMetrics
        );
        return isTrailingHit ? caretMetrics.textPosition + 1 : caretMetrics.textPosition;
    }
    return -1;
}
int Font::HitTestTextPosition(IDWriteTextLayout* textLayout, float width, float height, float x, float y)
{
    if (textLayout)
    {
        BOOL isTrailingHit;
        BOOL isInside;
        DWRITE_HIT_TEST_METRICS caretMetrics;
        textLayout->HitTestPoint(
            x,
            y,
            &isTrailingHit,
            &isInside,
            &caretMetrics
        );
        return caretMetrics.textPosition;
    }
    return -1;
}
std::vector<DWRITE_HIT_TEST_METRICS> Font::HitTestTextRange(std::wstring str, UINT32 start, UINT32 len)
{
    std::vector<DWRITE_HIT_TEST_METRICS> hitTestMetrics;
    if (str.size() == 0) return hitTestMetrics;
    IDWriteTextLayout* textLayout = NULL;
    HRESULT hr = _DWriteFactory->CreateTextLayout(
        str.c_str(),
        str.size(),
        this->_fontObject,
        FLT_MAX,
        FLT_MAX,
        &textLayout);
    if (SUCCEEDED(hr))
    {
        UINT32 actualHitTestCount = 0;
        hr = textLayout->HitTestTextRange(
            start,
            len,
            0.0f, // x
            0.0f, // y
            NULL,
            0, // metrics count
            &actualHitTestCount
        );
        hitTestMetrics.resize(actualHitTestCount);
        UINT32 textLen = len;
        hr = textLayout->HitTestTextRange(
            start,
            len,
            0.0f, // x
            0.0f, // y
            hitTestMetrics.data(),
            hitTestMetrics.size(),
            &actualHitTestCount
        );
        textLayout->Release();
    }
    return hitTestMetrics;
}
std::vector<DWRITE_HIT_TEST_METRICS> Font::HitTestTextRange(std::wstring str, float width, float height, UINT32 start, UINT32 len)
{
    std::vector<DWRITE_HIT_TEST_METRICS> hitTestMetrics;
    if (str.size() == 0)
    {
        hitTestMetrics.push_back(DWRITE_HIT_TEST_METRICS{ 0 });
        return hitTestMetrics;
    }
    IDWriteTextLayout* textLayout = NULL;
    HRESULT hr = _DWriteFactory->CreateTextLayout(
        str.c_str(),
        str.size(),
        this->_fontObject,
        width,
        height,
        &textLayout);
    if (SUCCEEDED(hr))
    {
        UINT32 actualHitTestCount = 0;
        hr = textLayout->HitTestTextRange(
            start,
            len,
            0.0f, // x
            0.0f, // y
            NULL,
            0, // metrics count
            &actualHitTestCount
        );
        hitTestMetrics.resize(actualHitTestCount);
        UINT32 textLen = len;
        hr = textLayout->HitTestTextRange(
            start,
            len,
            0.0f, // x
            0.0f, // y
            hitTestMetrics.data(),
            hitTestMetrics.size(),
            &actualHitTestCount
        );
        textLayout->Release();
    }
    if (hitTestMetrics.size() == 0)
    {
        hitTestMetrics.push_back(DWRITE_HIT_TEST_METRICS{ 0 });
    }
    return hitTestMetrics;
}
std::vector<DWRITE_HIT_TEST_METRICS> Font::HitTestTextRange(IDWriteTextLayout* textLayout, UINT32 start, UINT32 len)
{
    std::vector<DWRITE_HIT_TEST_METRICS> hitTestMetrics;
    UINT32 actualHitTestCount = 0;
    if (textLayout)
    {
        auto hr = textLayout->HitTestTextRange(
            start,
            len,
            0.0f, // x
            0.0f, // y
            NULL,
            0, // metrics count
            &actualHitTestCount
        );
        hitTestMetrics.resize(actualHitTestCount);
        UINT32 textLen = len;
        hr = textLayout->HitTestTextRange(
            start,
            len,
            0.0f, // x
            0.0f, // y
            hitTestMetrics.data(),
            hitTestMetrics.size(),
            &actualHitTestCount
        );
    }
    return hitTestMetrics;
}
std::vector<DWRITE_HIT_TEST_METRICS> Font::HitTestTextRange(IDWriteTextLayout* textLayout, float width, float height, UINT32 start, UINT32 len)
{
    std::vector<DWRITE_HIT_TEST_METRICS> hitTestMetrics;
    if (textLayout)
    {
        UINT32 actualHitTestCount = 0;
        auto hr = textLayout->HitTestTextRange(
            start,
            len,
            0.0f, // x
            0.0f, // y
            NULL,
            0, // metrics count
            &actualHitTestCount
        );
        hitTestMetrics.resize(actualHitTestCount);
        UINT32 textLen = len;
        hr = textLayout->HitTestTextRange(
            start,
            len,
            0.0f, // x
            0.0f, // y
            hitTestMetrics.data(),
            hitTestMetrics.size(),
            &actualHitTestCount
        );
    }
    return hitTestMetrics;
}
std::vector<std::wstring> Font::GetSystemFonts()
{
    static std::vector<std::wstring> result = std::vector<std::wstring>();
    if (result.size() == 0)
    {
        IDWriteFontCollection* pFontCollection = NULL;
        auto hr = _DWriteFactory->GetSystemFontCollection(&pFontCollection);
        UINT32 familyCount = 0;
        if (SUCCEEDED(hr))
        {
            familyCount = pFontCollection->GetFontFamilyCount();
            for (UINT32 i = 0; i < familyCount; ++i)
            {
                IDWriteFontFamily* pFontFamily = NULL;
                if (SUCCEEDED(hr))
                {
                    hr = pFontCollection->GetFontFamily(i, &pFontFamily);
                    IDWriteLocalizedStrings* pFamilyNames = NULL;
                    if (SUCCEEDED(hr))
                    {
                        hr = pFontFamily->GetFamilyNames(&pFamilyNames);
                        UINT32 index = 0;
                        BOOL exists = false;
                        wchar_t localeName[LOCALE_NAME_MAX_LENGTH];
                        if (SUCCEEDED(hr))
                        {
                            int defaultLocaleSuccess = GetUserDefaultLocaleName(localeName, LOCALE_NAME_MAX_LENGTH);
                            if (defaultLocaleSuccess)
                            {
                                hr = pFamilyNames->FindLocaleName(localeName, &index, &exists);
                            }
                            if (SUCCEEDED(hr) && !exists)
                            {
                                hr = pFamilyNames->FindLocaleName(L"en-us", &index, &exists);
                            }
                        }
                        if (!exists)
                            index = 0;
                        UINT32 length = 0;
                        if (SUCCEEDED(hr))
                        {
                            hr = pFamilyNames->GetStringLength(index, &length);
                        }
                        wchar_t* name = new wchar_t[length + 1];
                        if (name == NULL)
                        {
                            hr = E_OUTOFMEMORY;
                        }
                        if (SUCCEEDED(hr))
                        {
                            hr = pFamilyNames->GetString(index, name, length + 1);
                            result.push_back(std::wstring(name));
                        }
                        delete[] name;
                    }
                }
            }
        }
    }
    return result;
}