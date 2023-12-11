#include "Graphics.h"
#include "nanosvg.h"
#include "Factory.h"
#include "../Utils/Convert.h"

#pragma warning(disable: 4267)
#pragma warning(disable: 4244)
#pragma warning(disable: 4018)
inline ID2D1SolidColorBrush* Graphics::GetBrush()
{
    return Default_Brush;
}
Graphics::Graphics(IDXGISurface* dxgiSurface)
{
    _Target_Type = target_type::__surface;
    D2D1_RENDER_TARGET_PROPERTIES rtDesc = D2D1::RenderTargetProperties(
        D2D1_RENDER_TARGET_TYPE_DEFAULT,
        D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED)
    );
    auto err = _D2DFactory->CreateDxgiSurfaceRenderTarget(
        dxgiSurface,
        &rtDesc,
        &pRenderTarget
    );
    this->DefaultFontObject = new Font(Font::GetSystemFonts()[0].c_str(), 14);
    _D3DCOLORVALUE color = { 0.5f,0.6f,0.7f,1.0f };
    pRenderTarget->CreateSolidColorBrush(color, &this->Default_Brush);
    this->Default_Brush->SetColor(color);
    WindowHandle = NULL;
}
Graphics::Graphics(IDXGISwapChain* swap_chain_ptr)
{
    _Target_Type = target_type::__swapchain;
    IDXGISurface* pBackBuffer;
    swap_chain_ptr->GetBuffer(
        0,
        IID_PPV_ARGS(&pBackBuffer)
    );
    if (!pBackBuffer)
        throw std::runtime_error("Graphics::Graphics() : pBackBuffer is NILL!");
    D2D1_RENDER_TARGET_PROPERTIES rtDesc = D2D1::RenderTargetProperties(
        D2D1_RENDER_TARGET_TYPE_HARDWARE,
        D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED)
    );
    auto err = _D2DFactory->CreateDxgiSurfaceRenderTarget(
        pBackBuffer,
        &rtDesc,
        &pRenderTarget
    );
    if (FAILED(err))
        throw std::runtime_error("Graphics::Graphics() : CreateDxgiSurfaceRenderTarget Failed!");
    this->DefaultFontObject = new Font(Font::GetSystemFonts()[0].c_str(), 14);
    _D3DCOLORVALUE color = { 0.5f,0.6f,0.7f,1.0f };
    pRenderTarget->CreateSolidColorBrush(color, &this->Default_Brush);
    this->Default_Brush->SetColor(color);
    WindowHandle = NULL;
}
Graphics::Graphics(HWND hWnd)
{
    _Target_Type = target_type::__hwnd;
    this->WindowHandle = hWnd;
    D2D1_RENDER_TARGET_PROPERTIES rtDesc = D2D1::RenderTargetProperties(
        D2D1_RENDER_TARGET_TYPE_HARDWARE,
        D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED)
    );
    RECT rc = { 0,0,0,0 };
    GetClientRect(this->WindowHandle, &rc);
    auto err = _D2DFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(this->WindowHandle, D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)),
        (ID2D1HwndRenderTarget**)&pRenderTarget
    );
    this->DefaultFontObject = new Font(Font::GetSystemFonts()[0].c_str(), 14);
    _D3DCOLORVALUE color = { 0.5f,0.6f,0.7f,1.0f };
    pRenderTarget->CreateSolidColorBrush(color, &this->Default_Brush);
    this->Default_Brush->SetColor(color);
}
Graphics::Graphics(int width, int height)
{
    _Target_Type = target_type::__wic;
    auto st = _ImageFactory->CreateBitmap(
        width,
        height,
        GUID_WICPixelFormat32bppPBGRA,
        WICBitmapCacheOnLoad,
        &pWICBitmap
    );
    D2D1_RENDER_TARGET_PROPERTIES rtProps = D2D1::RenderTargetProperties();
    rtProps.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED);
    rtProps.usage = D2D1_RENDER_TARGET_USAGE_NONE;
    rtProps.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
    st = _D2DFactory->CreateWicBitmapRenderTarget(
        pWICBitmap,
        rtProps,
        &pRenderTarget
    );
    this->DefaultFontObject = new Font(Font::GetSystemFonts()[0].c_str(), 14);
    _D3DCOLORVALUE color = { 0.5f,0.6f,0.7f,1.0f };
    pRenderTarget->CreateSolidColorBrush(color, &this->Default_Brush);
    this->Default_Brush->SetColor(color);
    WindowHandle = NULL;
}
Graphics::Graphics(IWICBitmap* wic)
{
    UINT w, h;
    wic->GetSize(&w, &h);
    _Target_Type = target_type::__wic;
    auto st = _ImageFactory->CreateBitmap(
        w,
        h,
        GUID_WICPixelFormat32bppPBGRA,
        WICBitmapCacheOnLoad,
        &pWICBitmap
    );
    st = _D2DFactory->CreateWicBitmapRenderTarget(
        pWICBitmap,
        D2D1::RenderTargetProperties(),
        &pRenderTarget
    );
    this->DefaultFontObject = new Font(Font::GetSystemFonts()[0].c_str(), 14);
    _D3DCOLORVALUE color = { 0.5f,0.6f,0.7f,1.0f };
    pRenderTarget->CreateSolidColorBrush(color, &this->Default_Brush);
    this->Default_Brush->SetColor(color);
    auto bbmp = this->CreateBitmap(wic);
    this->BeginRender();
    this->DrawBitmap(bbmp, 0, 0, w, h);
    this->EndRender();
    WindowHandle = NULL;
}
Graphics::Graphics(ID2D1Bitmap* bmp)
{
    auto size = bmp->GetSize();
    _Target_Type = target_type::__wic;
    auto st = _ImageFactory->CreateBitmap(
        (UINT)size.width,
        (UINT)size.height,
        GUID_WICPixelFormat32bppPBGRA,
        WICBitmapCacheOnLoad,
        &pWICBitmap
    );
    st = _D2DFactory->CreateWicBitmapRenderTarget(
        pWICBitmap,
        D2D1::RenderTargetProperties(),
        &pRenderTarget
    );
    this->DefaultFontObject = new Font(Font::GetSystemFonts()[0].c_str(), 14);
    _D3DCOLORVALUE color = { 0.5f,0.6f,0.7f,1.0f };
    pRenderTarget->CreateSolidColorBrush(color, &this->Default_Brush);
    this->Default_Brush->SetColor(color);
    this->BeginRender();
    this->DrawBitmap(bmp, 0, 0, size.width, size.height);
    this->EndRender();
    WindowHandle = NULL;
}
Graphics::Graphics(ID2D1BitmapRenderTarget* target)
{
    _Target_Type = target_type::__bitmap;
    pRenderTarget = target;
    this->DefaultFontObject = new Font(Font::GetSystemFonts()[0].c_str(), 14);
    _D3DCOLORVALUE color = { 0.5f,0.6f,0.7f,1.0f };
    pRenderTarget->CreateSolidColorBrush(color, &this->Default_Brush);
    this->Default_Brush->SetColor(color);
    WindowHandle = NULL;
}
Graphics::~Graphics()
{
    if (this->pWICBitmap) pWICBitmap->Release();
    if (this->DefaultFontObject)
    {
        delete DefaultFontObject;
    }
    if (this->pRenderTarget) pRenderTarget->Release();
    if (this->Default_Brush) Default_Brush->Release();
}
ID2D1SolidColorBrush* Graphics::GetColorBrush(D2D1_COLOR_F newcolor)
{
    auto brush = this->GetBrush();
    brush->SetColor(newcolor);
    return brush;
}
ID2D1SolidColorBrush* Graphics::GetColorBrush(COLORREF newcolor)
{
    auto brush = this->GetBrush();
    D2D1_COLOR_F _newcolor = { GetRValue(newcolor) / 255.0f,GetGValue(newcolor) / 255.0f,GetBValue(newcolor) / 255.0f,1.0f };
    brush->SetColor(_newcolor);
    return brush;
}
ID2D1SolidColorBrush* Graphics::GetColorBrush(int r, int g, int b)
{
    auto brush = this->GetBrush();
    D2D1_COLOR_F _newcolor = { r / 255.0f,g / 255.0f,b / 255.0f,1.0f };
    brush->SetColor(_newcolor);
    return brush;
}
ID2D1SolidColorBrush* Graphics::GetColorBrush(float r, float g, float b, float a)
{
    auto brush = this->GetBrush();
    D2D1_COLOR_F _newcolor = { r,g,b,a };
    brush->SetColor(_newcolor);
    return brush;
}
ID2D1LinearGradientBrush* Graphics::CreateLinearGradientBrush(D2D1_GRADIENT_STOP* stops, unsigned int stopcount)
{
    ID2D1GradientStopCollection* collection;
    if SUCCEEDED(pRenderTarget->CreateGradientStopCollection(stops, stopcount, &collection))
    {
        D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES props = {};
        ID2D1LinearGradientBrush* m_LinearBrush;
        if SUCCEEDED(pRenderTarget->CreateLinearGradientBrush(props, collection, &m_LinearBrush))
        {
            collection->Release();
            return m_LinearBrush;
        }
        collection->Release();
    }
    return NULL;
}
ID2D1RadialGradientBrush* Graphics::CreateRadialGradientBrush(D2D1_GRADIENT_STOP* stops, unsigned int stopcount, D2D1_POINT_2F center)
{
    ID2D1GradientStopCollection* collection;
    if SUCCEEDED(pRenderTarget->CreateGradientStopCollection(stops, stopcount, &collection))
    {
        D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES props = {};
        props.center = center;
        ID2D1RadialGradientBrush* brush;
        if SUCCEEDED(pRenderTarget->CreateRadialGradientBrush(props, collection, &brush))
        {
            collection->Release();
            return brush;
        }
        collection->Release();
    }
    return NULL;
}
ID2D1SolidColorBrush* Graphics::CreateSolidColorBrush(D2D1_COLOR_F color)
{
    ID2D1SolidColorBrush* result = NULL;
    pRenderTarget->CreateSolidColorBrush(color, &result);
    return result;
}
Graphics* Graphics::CreateSubGraphics(int width, int height)
{
    ID2D1BitmapRenderTarget* target = NULL;
    this->pRenderTarget->CreateCompatibleRenderTarget(D2D1_SIZE_F{ (float)width ,(float)height }, &target);
    return new Graphics(target);
}
D2D1_SIZE_F Graphics::Size()
{
    return this->pRenderTarget->GetSize();
}
void Graphics::Resize()
{
    if (this->WindowHandle)
    {
        RECT rc = { 0,0,0,0 };
        GetClientRect(this->WindowHandle, &rc);
        ((ID2D1HwndRenderTarget*)this->pRenderTarget)->Resize(
            D2D1_SIZE_U{
                (UINT32)rc.right - (UINT32)rc.left,
                (UINT32)rc.bottom - (UINT32)rc.top
            }
        );
    }
}
void Graphics::ResizeSubGraphics(int width, int height)
{
    DefaultFontObject->~Font();
    this->pRenderTarget->Release();
    ID2D1BitmapRenderTarget* target = NULL;
    this->pRenderTarget->CreateCompatibleRenderTarget(D2D1_SIZE_F{ (float)width ,(float)height }, &target);
    _Target_Type = target_type::__bitmap;
    pRenderTarget = target;
    this->DefaultFontObject = new Font(Font::GetSystemFonts()[0].c_str(), 14);
    _D3DCOLORVALUE color = { 0.5f,0.6f,0.7f,1.0f };
    pRenderTarget->CreateSolidColorBrush(color, &this->Default_Brush);
    this->Default_Brush->SetColor(color);
}
void Graphics::BeginRender(CONST D2D1_COLOR_F clearColor)
{
    pRenderTarget->BeginDraw();
    this->pRenderTarget->Clear(clearColor);
}
void Graphics::BeginRender()
{
    pRenderTarget->BeginDraw();
}
void Graphics::EndRender()
{
    pRenderTarget->EndDraw();
}
void Graphics::DrawLine(D2D1_POINT_2F p1, D2D1_POINT_2F p2, D2D1_COLOR_F color, float linewidth)
{
    this->pRenderTarget->DrawLine(p1, p2, this->GetColorBrush(color), linewidth);
}
void Graphics::DrawLine(float p1_x, float p1_y, float p2_x, float p2_y, D2D1_COLOR_F color, float linewidth)
{
    this->pRenderTarget->DrawLine({ p1_x,p1_y }, { p2_x,p2_y }, this->GetColorBrush(color), linewidth);
}
void Graphics::DrawLine(D2D1_POINT_2F p1, D2D1_POINT_2F p2, ID2D1Brush* brush, float linewidth)
{
    this->pRenderTarget->DrawLine(p1, p2, brush, linewidth);
}
void Graphics::DrawLine(float p1_x, float p1_y, float p2_x, float p2_y, ID2D1Brush* brush, float linewidth)
{
    this->pRenderTarget->DrawLine({ p1_x,p1_y }, { p2_x,p2_y }, brush, linewidth);
}
void Graphics::DrawRect(float left, float top, float width, float height, D2D1_COLOR_F color, float linewidth)
{
    this->pRenderTarget->DrawRectangle({ left,top,left + width,top + height }, this->GetColorBrush(color), linewidth);
}
void Graphics::DrawRoundRect(float left, float top, float width, float height, D2D1_COLOR_F color, float linewidth, float r)
{
    D2D1_ROUNDED_RECT round = D2D1::RoundedRect(
        D2D1::RectF(left, top, left + width, top + height),
        r,
        r
    );
    this->pRenderTarget->DrawRoundedRectangle(round, this->GetColorBrush(color), linewidth);
}
void Graphics::FillRect(float left, float top, float width, float height, D2D1_COLOR_F color)
{
    this->pRenderTarget->FillRectangle({ left,top,left + width,top + height }, this->GetColorBrush(color));
}
void Graphics::FillRoundRect(float left, float top, float width, float height, D2D1_COLOR_F color, float r)
{
    D2D1_ROUNDED_RECT round = D2D1::RoundedRect(
        D2D1::RectF(left, top, left + width, top + height),
        r,
        r
    );
    this->pRenderTarget->FillRoundedRectangle(round, this->GetColorBrush(color));
}
void Graphics::DrawEllipse(D2D1_POINT_2F cent, float xr, float yr, D2D1_COLOR_F color, float linewidth)
{
    this->pRenderTarget->DrawEllipse({ cent ,xr,yr }, this->GetColorBrush(color), linewidth = 1.0f);
}
void Graphics::DrawEllipse(D2D1_ELLIPSE ellipse, D2D1_COLOR_F color, float linewidth)
{
    this->pRenderTarget->DrawEllipse(ellipse, this->GetColorBrush(color), linewidth);
}
void Graphics::DrawEllipse(float x, float y, float xr, float yr, D2D1_COLOR_F color, float linewidth)
{
    this->pRenderTarget->DrawEllipse({ {x,y} ,xr,yr }, this->GetColorBrush(color), linewidth = 1.0f);
}
void Graphics::FillEllipse(D2D1_ELLIPSE ellipse, D2D1_COLOR_F color)
{
    this->pRenderTarget->FillEllipse(ellipse, this->GetColorBrush(color));
}
void Graphics::FillEllipse(D2D1_POINT_2F cent, float xr, float yr, D2D1_COLOR_F color)
{
    this->pRenderTarget->FillEllipse({ cent ,xr,yr }, this->GetColorBrush(color));
}
void Graphics::FillEllipse(float cx, float cy, float xr, float yr, D2D1_COLOR_F color)
{
    this->pRenderTarget->FillEllipse({ {cx,cy} ,xr,yr }, this->GetColorBrush(color));
}
void Graphics::DrawGeometry(ID2D1Geometry* geo, D2D1_COLOR_F color, float linewidth)
{
    if (geo)
        this->pRenderTarget->DrawGeometry(geo, this->GetColorBrush(color), linewidth);
}
void Graphics::FillGeometry(ID2D1Geometry* geo, D2D1_COLOR_F color)
{
    if (geo)
        this->pRenderTarget->FillGeometry(geo, this->GetColorBrush(color));
}
void Graphics::DrawGeometry(ID2D1Geometry* geo, ID2D1Brush* brush, float linewidth)
{
    if (geo)
        this->pRenderTarget->DrawGeometry(geo, brush, linewidth);
}
void Graphics::FillGeometry(ID2D1Geometry* geo, ID2D1Brush* brush)
{
    if (geo)
        this->pRenderTarget->FillGeometry(geo, brush);
}
void Graphics::FillMesh(ID2D1Mesh* mesh, D2D1_COLOR_F color)
{
    this->pRenderTarget->FillMesh(mesh, this->GetColorBrush(color));
}
void Graphics::DrawBitmap(ID2D1Bitmap* bmp, float x, float y, float opacity, D2D1_COLOR_F border, float borderwidth)
{
    if (this->pRenderTarget && bmp)
    {
        this->pRenderTarget->DrawBitmap(
            bmp,
            {
                x,y,
                bmp->GetSize().width + x,
                bmp->GetSize().height + y
            },
            opacity, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
    }
}
void Graphics::DrawBitmap(ID2D1Bitmap* bmp, float x, float y, float w, float h, float opacity, D2D1_COLOR_F border, float borderwidth)
{
    if (this->pRenderTarget && bmp)
    {
        this->pRenderTarget->DrawBitmap(
            bmp,
            {
                x,y,
                w == 0 ? bmp->GetSize().width + x : w + x,
                h == 0 ? bmp->GetSize().height + y : h + y
            },
            opacity, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
        if (border.a > 0.0f)
        {
            this->DrawRect(x, y, w, h, border, borderwidth);
        }
    }
}
void Graphics::DrawBitmap(ID2D1Bitmap* bmp, float sx, float sy, float sw, float sh, float x, float y, float w, float h, float opacity, D2D1_COLOR_F border, float borderwidth)
{
    if (this->pRenderTarget && bmp)
    {
        D2D1_RECT_F srec = {
                sx,sy,
                sw + sx,
                sh + sy
        };
        D2D1_RECT_F drec = {
                x,y,
                w == 0 ? bmp->GetSize().width + x : w + x,
                h == 0 ? bmp->GetSize().height + y : h + y
        };
        this->pRenderTarget->DrawBitmap(
            bmp,
            drec,
            opacity,
            D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
            srec);
        if (border.a > 0.0f)
        {
            this->DrawRect(x, y, w, h, border, borderwidth);
        }
    }
}
void Graphics::DrawString(const char* str, float x, float y, ID2D1Brush* brush, Font* font)
{

    auto tmp = Convert::string_to_wstring(str);
    this->pRenderTarget->DrawTextW(
        tmp.c_str(),
        tmp.size(),
        font ? font->FontObject : this->DefaultFontObject->FontObject,
        D2D1::RectF(x, y, FLT_MAX, FLT_MAX),
        brush);
}
void Graphics::DrawString(const wchar_t* str, float x, float y, ID2D1Brush* brush, Font* font)
{
    this->pRenderTarget->DrawTextW(
        str,
        wcslen(str),
        font ? font->FontObject : this->DefaultFontObject->FontObject,
        D2D1::RectF(x, y, FLT_MAX, FLT_MAX),
        brush);
}
void Graphics::DrawString(const char* str, float x, float y, D2D1_COLOR_F color, Font* font)
{
    auto tmp = Convert::string_to_wstring(str);
    this->pRenderTarget->DrawTextW(
        tmp.c_str(),
        tmp.size(),
        font ? font->FontObject : this->DefaultFontObject->FontObject,
        D2D1::RectF(x, y, FLT_MAX, FLT_MAX),
        this->GetColorBrush(color));
}
void Graphics::DrawString(const wchar_t* str, float x, float y, D2D1_COLOR_F color, Font* font)
{
    this->pRenderTarget->DrawTextW(
        str,
        wcslen(str),
        font ? font->FontObject : this->DefaultFontObject->FontObject,
        D2D1::RectF(x, y, FLT_MAX, FLT_MAX),
        this->GetColorBrush(color));
}
void Graphics::DrawString(std::string str, float x, float y, D2D1_COLOR_F color, Font* font)
{
    auto tmp = Convert::string_to_wstring(str);
    this->pRenderTarget->DrawTextW(
        tmp.c_str(),
        tmp.size(),
        font ? font->FontObject : this->DefaultFontObject->FontObject,
        D2D1::RectF(x, y, FLT_MAX, FLT_MAX),
        this->GetColorBrush(color));
}
void Graphics::DrawString(std::string str, float x, float y, ID2D1Brush* brush, Font* font)
{
    auto tmp = Convert::string_to_wstring(str);
    this->pRenderTarget->DrawTextW(
        tmp.c_str(),
        tmp.size(),
        font ? font->FontObject : this->DefaultFontObject->FontObject,
        D2D1::RectF(x, y, FLT_MAX, FLT_MAX),
        brush);
}
void Graphics::DrawString(std::wstring str, float x, float y, D2D1_COLOR_F color, Font* font)
{
    this->pRenderTarget->DrawTextW(
        str.c_str(),
        str.size(),
        font ? font->FontObject : this->DefaultFontObject->FontObject,
        D2D1::RectF(x, y, FLT_MAX, FLT_MAX),
        this->GetColorBrush(color));
}
void Graphics::DrawString(std::wstring str, float x, float y, ID2D1Brush* brush, Font* font)
{
    this->pRenderTarget->DrawTextW(
        str.c_str(),
        str.size(),
        font ? font->FontObject : this->DefaultFontObject->FontObject,
        D2D1::RectF(x, y, FLT_MAX, FLT_MAX),
        brush);
}
IDWriteTextLayout* Graphics::CreateStringLayout(std::string str, Font* font)
{
    IDWriteTextLayout* textLayout = 0;
    auto tmp = Convert::string_to_wstring(str);
    HRESULT hr = _DWriteFactory->CreateTextLayout(
        tmp.c_str(),
        tmp.size(),
        font ? font->FontObject : this->DefaultFontObject->FontObject,
        FLT_MAX,
        FLT_MAX,
        &textLayout);
    return textLayout;
}
IDWriteTextLayout* Graphics::CreateStringLayout(std::wstring str, Font* font)
{
    IDWriteTextLayout* textLayout = 0;
    HRESULT hr = _DWriteFactory->CreateTextLayout(
        str.c_str(),
        str.size(),
        font ? font->FontObject : this->DefaultFontObject->FontObject,
        FLT_MAX,
        FLT_MAX,
        &textLayout);
    return textLayout;
}
IDWriteTextLayout* Graphics::CreateStringLayout(std::wstring str, float width, float height, Font* font)
{
    IDWriteTextLayout* textLayout = 0;
    HRESULT hr = _DWriteFactory->CreateTextLayout(
        str.c_str(),
        str.size(),
        font ? font->FontObject : this->DefaultFontObject->FontObject,
        width,
        height,
        &textLayout);
    return textLayout;
}
void Graphics::DrawStringLayout(IDWriteTextLayout* layout, float x, float y, D2D1_COLOR_F color)
{
    this->pRenderTarget->DrawTextLayout(
        { x,y },
        layout,
        this->GetColorBrush(color));
}
void Graphics::DrawStringLayout(IDWriteTextLayout* layout, float x, float y, ID2D1Brush* brush)
{
    this->pRenderTarget->DrawTextLayout(
        { x,y },
        layout,
        brush);
}
D2D1_SIZE_F Graphics::DrawStringLayoutCent(IDWriteTextLayout* layout, float x, float y, D2D1_COLOR_F color, D2D1_COLOR_F back)
{
    DWRITE_TEXT_METRICS metrics;
    auto hr = layout->GetMetrics(&metrics);
    auto tsize = D2D1::Size((float)ceil(metrics.widthIncludingTrailingWhitespace), (float)ceil(metrics.height));
    D2D1_POINT_2F loc = { x - (tsize.width / 2.0f),y - (tsize.height / 2.0f) };
    if (back.a > 0.0f)
    {
        auto brush = this->GetColorBrush(back);
        this->pRenderTarget->DrawTextLayout({ loc.x - 1,loc.y - 1 }, layout, brush);
        this->pRenderTarget->DrawTextLayout({ loc.x + 1,loc.y + 1 }, layout, brush);
        this->pRenderTarget->DrawTextLayout({ loc.x - 1,loc.y + 1 }, layout, brush);
        this->pRenderTarget->DrawTextLayout({ loc.x + 1,loc.y - 1 }, layout, brush);
    }
    this->pRenderTarget->DrawTextLayout({ loc.x ,loc.y }, layout, this->GetColorBrush(color));
    return tsize;
}
D2D1_SIZE_F Graphics::DrawStringCent(std::string str, float x, float y, D2D1_COLOR_F color, IDWriteTextLayout** _recodeLayout, D2D1_COLOR_F back, Font* font)
{
    D2D1_SIZE_F tsize = { 0,0 };
    IDWriteTextLayout* textLayout = CreateStringLayout(str, font ? font : this->DefaultFontObject);
    if (textLayout)
    {
        DWRITE_TEXT_METRICS metrics;
        auto hr = textLayout->GetMetrics(&metrics);
        if (SUCCEEDED(hr))
        {
            tsize = D2D1::Size((float)ceil(metrics.widthIncludingTrailingWhitespace), (float)ceil(metrics.height));
            D2D1_POINT_2F loc = { x - (tsize.width / 2.0f),y - (tsize.height / 2.0f) };
            if (back.a > 0.0f)
            {
                auto brush = this->GetColorBrush(back);
                this->pRenderTarget->DrawTextLayout({ loc.x - 1,loc.y - 1 }, textLayout, brush);
                this->pRenderTarget->DrawTextLayout({ loc.x + 1,loc.y + 1 }, textLayout, brush);
                this->pRenderTarget->DrawTextLayout({ loc.x - 1,loc.y + 1 }, textLayout, brush);
                this->pRenderTarget->DrawTextLayout({ loc.x + 1,loc.y - 1 }, textLayout, brush);
            }
            this->pRenderTarget->DrawTextLayout(
                loc,
                textLayout,
                this->GetColorBrush(color));
        }
        if (_recodeLayout) *_recodeLayout = textLayout;
        else textLayout->Release();
    }
    return tsize;
}
D2D1_SIZE_F Graphics::DrawStringCent(std::wstring str, float x, float y, D2D1_COLOR_F color, IDWriteTextLayout** _recodeLayout, D2D1_COLOR_F back, Font* font)
{
    D2D1_SIZE_F tsize = { 0,0 };
    IDWriteTextLayout* textLayout = CreateStringLayout(str, font ? font : this->DefaultFontObject);
    if (textLayout)
    {
        DWRITE_TEXT_METRICS metrics;
        auto hr = textLayout->GetMetrics(&metrics);
        if (SUCCEEDED(hr))
        {
            tsize = D2D1::Size((float)ceil(metrics.widthIncludingTrailingWhitespace), (float)ceil(metrics.height));
            D2D1_POINT_2F loc = { x - (tsize.width / 2.0f),y - (tsize.height / 2.0f) };
            if (back.a > 0.0f)
            {
                auto brush = this->GetColorBrush(back);
                this->pRenderTarget->DrawTextLayout({ loc.x - 1,loc.y - 1 }, textLayout, brush);
                this->pRenderTarget->DrawTextLayout({ loc.x + 1,loc.y + 1 }, textLayout, brush);
                this->pRenderTarget->DrawTextLayout({ loc.x - 1,loc.y + 1 }, textLayout, brush);
                this->pRenderTarget->DrawTextLayout({ loc.x + 1,loc.y - 1 }, textLayout, brush);
            }
            this->pRenderTarget->DrawTextLayout(loc, textLayout, this->GetColorBrush(color));
        }
        if (_recodeLayout) *_recodeLayout = textLayout;
        else textLayout->Release();
    }
    return tsize;
}
D2D1_SIZE_F Graphics::DrawStringLayoutCentHorizontal(IDWriteTextLayout* layout, float x, float y, D2D1_COLOR_F color, D2D1_COLOR_F back)
{
    DWRITE_TEXT_METRICS metrics;
    auto hr = layout->GetMetrics(&metrics);
    auto tsize = D2D1::Size((float)ceil(metrics.widthIncludingTrailingWhitespace), (float)ceil(metrics.height));
    D2D1_POINT_2F loc = { x - (tsize.width / 2.0f),y };
    if (back.a > 0.0f)
    {
        auto brush = this->GetColorBrush(back);
        this->pRenderTarget->DrawTextLayout({ loc.x - 1,loc.y - 1 }, layout, brush);
        this->pRenderTarget->DrawTextLayout({ loc.x + 1,loc.y + 1 }, layout, brush);
        this->pRenderTarget->DrawTextLayout({ loc.x - 1,loc.y + 1 }, layout, brush);
        this->pRenderTarget->DrawTextLayout({ loc.x + 1,loc.y - 1 }, layout, brush);
    }
    this->pRenderTarget->DrawTextLayout({ loc.x ,loc.y }, layout, this->GetColorBrush(color));
    return tsize;
}
D2D1_SIZE_F Graphics::DrawStringCentHorizontal(std::string str, float x, float y, D2D1_COLOR_F color, IDWriteTextLayout** _recodeLayout, D2D1_COLOR_F back, Font* font)
{
    D2D1_SIZE_F tsize = { 0,0 };
    IDWriteTextLayout* textLayout = CreateStringLayout(str, font ? font : this->DefaultFontObject);
    if (textLayout)
    {
        DWRITE_TEXT_METRICS metrics;
        auto hr = textLayout->GetMetrics(&metrics);
        if (SUCCEEDED(hr))
        {
            tsize = D2D1::Size((float)ceil(metrics.widthIncludingTrailingWhitespace), (float)ceil(metrics.height));
            D2D1_POINT_2F loc = { x - (tsize.width / 2.0f),y };
            if (back.a > 0.0f)
            {
                auto brush = this->GetColorBrush(back);
                this->pRenderTarget->DrawTextLayout({ loc.x - 1,loc.y - 1 }, textLayout, brush);
                this->pRenderTarget->DrawTextLayout({ loc.x + 1,loc.y + 1 }, textLayout, brush);
                this->pRenderTarget->DrawTextLayout({ loc.x - 1,loc.y + 1 }, textLayout, brush);
                this->pRenderTarget->DrawTextLayout({ loc.x + 1,loc.y - 1 }, textLayout, brush);
            }
            this->pRenderTarget->DrawTextLayout(
                loc,
                textLayout,
                this->GetColorBrush(color));
        }
        if (_recodeLayout) *_recodeLayout = textLayout;
        else textLayout->Release();
    }
    return tsize;
}
D2D1_SIZE_F Graphics::DrawStringCentHorizontal(std::wstring str, float x, float y, D2D1_COLOR_F color, IDWriteTextLayout** _recodeLayout, D2D1_COLOR_F back, Font* font)
{
    D2D1_SIZE_F tsize = { 0,0 };
    IDWriteTextLayout* textLayout = CreateStringLayout(str, font ? font : this->DefaultFontObject);
    if (textLayout)
    {
        DWRITE_TEXT_METRICS metrics;
        auto hr = textLayout->GetMetrics(&metrics);
        if (SUCCEEDED(hr))
        {
            tsize = D2D1::Size((float)ceil(metrics.widthIncludingTrailingWhitespace), (float)ceil(metrics.height));
            D2D1_POINT_2F loc = { x - (tsize.width / 2.0f),y };
            if (back.a > 0.0f)
            {
                auto brush = this->GetColorBrush(back);
                this->pRenderTarget->DrawTextLayout({ loc.x - 1,loc.y - 1 }, textLayout, brush);
                this->pRenderTarget->DrawTextLayout({ loc.x + 1,loc.y + 1 }, textLayout, brush);
                this->pRenderTarget->DrawTextLayout({ loc.x - 1,loc.y + 1 }, textLayout, brush);
                this->pRenderTarget->DrawTextLayout({ loc.x + 1,loc.y - 1 }, textLayout, brush);
            }
            this->pRenderTarget->DrawTextLayout(loc, textLayout, this->GetColorBrush(color));
        }
        if (_recodeLayout) *_recodeLayout = textLayout;
        else textLayout->Release();
    }
    return tsize;
}
D2D1_SIZE_F Graphics::DrawStringLayoutCentVertical(IDWriteTextLayout* layout, float x, float y, D2D1_COLOR_F color, D2D1_COLOR_F back)
{
    DWRITE_TEXT_METRICS metrics;
    auto hr = layout->GetMetrics(&metrics);
    auto tsize = D2D1::Size((float)ceil(metrics.widthIncludingTrailingWhitespace), (float)ceil(metrics.height));
    D2D1_POINT_2F loc = { x ,y - (tsize.height / 2.0f) };
    if (back.a > 0.0f)
    {
        auto brush = this->GetColorBrush(back);
        this->pRenderTarget->DrawTextLayout({ loc.x - 1,loc.y - 1 }, layout, brush);
        this->pRenderTarget->DrawTextLayout({ loc.x + 1,loc.y + 1 }, layout, brush);
        this->pRenderTarget->DrawTextLayout({ loc.x - 1,loc.y + 1 }, layout, brush);
        this->pRenderTarget->DrawTextLayout({ loc.x + 1,loc.y - 1 }, layout, brush);
    }
    this->pRenderTarget->DrawTextLayout({ loc.x ,loc.y }, layout, this->GetColorBrush(color));
    return tsize;
}
D2D1_SIZE_F Graphics::DrawStringCentVertical(std::string str, float x, float y, D2D1_COLOR_F color, IDWriteTextLayout** _recodeLayout, D2D1_COLOR_F back, Font* font)
{
    D2D1_SIZE_F tsize = { 0,0 };
    IDWriteTextLayout* textLayout = CreateStringLayout(str, font ? font : this->DefaultFontObject);
    if (textLayout)
    {
        DWRITE_TEXT_METRICS metrics;
        auto hr = textLayout->GetMetrics(&metrics);
        if (SUCCEEDED(hr))
        {
            tsize = D2D1::Size((float)ceil(metrics.widthIncludingTrailingWhitespace), (float)ceil(metrics.height));
            D2D1_POINT_2F loc = { x ,y - (tsize.height / 2.0f) };
            if (back.a > 0.0f)
            {
                auto brush = this->GetColorBrush(back);
                this->pRenderTarget->DrawTextLayout({ loc.x - 1,loc.y - 1 }, textLayout, brush);
                this->pRenderTarget->DrawTextLayout({ loc.x + 1,loc.y + 1 }, textLayout, brush);
                this->pRenderTarget->DrawTextLayout({ loc.x - 1,loc.y + 1 }, textLayout, brush);
                this->pRenderTarget->DrawTextLayout({ loc.x + 1,loc.y - 1 }, textLayout, brush);
            }
            this->pRenderTarget->DrawTextLayout(
                loc,
                textLayout,
                this->GetColorBrush(color));
        }
        if (_recodeLayout) *_recodeLayout = textLayout;
        else textLayout->Release();
    }
    return tsize;
}
D2D1_SIZE_F Graphics::DrawStringCentVertical(std::wstring str, float x, float y, D2D1_COLOR_F color, IDWriteTextLayout** _recodeLayout, D2D1_COLOR_F back, Font* font)
{
    D2D1_SIZE_F tsize = { 0,0 };
    IDWriteTextLayout* textLayout = CreateStringLayout(str, font ? font : this->DefaultFontObject);
    if (textLayout)
    {
        DWRITE_TEXT_METRICS metrics;
        auto hr = textLayout->GetMetrics(&metrics);
        if (SUCCEEDED(hr))
        {
            tsize = D2D1::Size((float)ceil(metrics.widthIncludingTrailingWhitespace), (float)ceil(metrics.height));
            D2D1_POINT_2F loc = { x ,y - (tsize.height / 2.0f) };
            if (back.a > 0.0f)
            {
                auto brush = this->GetColorBrush(back);
                this->pRenderTarget->DrawTextLayout({ loc.x - 1,loc.y - 1 }, textLayout, brush);
                this->pRenderTarget->DrawTextLayout({ loc.x + 1,loc.y + 1 }, textLayout, brush);
                this->pRenderTarget->DrawTextLayout({ loc.x - 1,loc.y + 1 }, textLayout, brush);
                this->pRenderTarget->DrawTextLayout({ loc.x + 1,loc.y - 1 }, textLayout, brush);
            }
            this->pRenderTarget->DrawTextLayout(loc, textLayout, this->GetColorBrush(color));
        }
        if (_recodeLayout) *_recodeLayout = textLayout;
        else textLayout->Release();
    }
    return tsize;
}
void Graphics::DrawStringLayout(std::string str, float x, float y, D2D1_COLOR_F color, D2D1_COLOR_F back, Font* font)
{
    IDWriteTextLayout* textLayout = CreateStringLayout(str, font ? font : this->DefaultFontObject);
    if (textLayout)
    {
        if (back.a > 0.0f)
        {
            auto brush = this->GetColorBrush(back);
            this->pRenderTarget->DrawTextLayout({ x - 1,y - 1 }, textLayout, brush);
            this->pRenderTarget->DrawTextLayout({ x + 1,y + 1 }, textLayout, brush);
            this->pRenderTarget->DrawTextLayout({ x + 1,y - 1 }, textLayout, brush);
            this->pRenderTarget->DrawTextLayout({ x - 1,y + 1 }, textLayout, brush);
        }
        this->pRenderTarget->DrawTextLayout({ x,y }, textLayout, this->GetColorBrush(color));
        textLayout->Release();
    }
}
void Graphics::DrawStringLayout(std::wstring str, float x, float y, D2D1_COLOR_F color, D2D1_COLOR_F back, Font* font)
{
    IDWriteTextLayout* textLayout = CreateStringLayout(str, font ? font : this->DefaultFontObject);
    if (textLayout)
    {
        if (back.a > 0.0f)
        {
            auto brush = this->GetColorBrush(back);
            this->GetColorBrush(back);
            this->pRenderTarget->DrawTextLayout({ x - 1,y - 1 }, textLayout, brush);
            this->pRenderTarget->DrawTextLayout({ x + 1,y + 1 }, textLayout, brush);
            this->pRenderTarget->DrawTextLayout({ x + 1,y - 1 }, textLayout, brush);
            this->pRenderTarget->DrawTextLayout({ x - 1,y + 1 }, textLayout, brush);
        }
        this->pRenderTarget->DrawTextLayout({ x,y }, textLayout, this->GetColorBrush(color));
        textLayout->Release();
    }
}
void Graphics::DrawStringLayout(std::wstring str, float x, float y, float w, float h, D2D1_COLOR_F color, D2D1_COLOR_F back, Font* font)
{
    IDWriteTextLayout* textLayout = CreateStringLayout(str, w, h, font ? font : this->DefaultFontObject);
    if (textLayout)
    {
        if (back.a > 0.0f)
        {
            auto brush = this->GetColorBrush(back);
            this->GetColorBrush(back);
            this->pRenderTarget->DrawTextLayout({ x - 1,y - 1 }, textLayout, brush);
            this->pRenderTarget->DrawTextLayout({ x + 1,y + 1 }, textLayout, brush);
            this->pRenderTarget->DrawTextLayout({ x + 1,y - 1 }, textLayout, brush);
            this->pRenderTarget->DrawTextLayout({ x - 1,y + 1 }, textLayout, brush);
        }
        this->pRenderTarget->DrawTextLayout({ x,y }, textLayout, this->GetColorBrush(color));
        textLayout->Release();
    }
}
void Graphics::DrawStringLayout(std::wstring str, float x, float y, D2D1_COLOR_F color, DWRITE_TEXT_RANGE subRange, D2D1_COLOR_F fontBack, Font* font)
{
    IDWriteTextLayout* textLayout = CreateStringLayout(str, font ? font : this->DefaultFontObject);
    if (textLayout)
    {
        textLayout->SetDrawingEffect(this->GetColorBrush(fontBack), subRange);
        this->pRenderTarget->DrawTextLayout({ x,y }, textLayout, this->GetColorBrush(color));
        textLayout->Release();
    }
}
void Graphics::DrawStringLayout(std::wstring str, float x, float y, float w, float h, D2D1_COLOR_F color, DWRITE_TEXT_RANGE subRange, D2D1_COLOR_F fontBack, Font* font)
{
    IDWriteTextLayout* textLayout = CreateStringLayout(str, w, h, font ? font : this->DefaultFontObject);
    if (textLayout)
    {
        textLayout->SetDrawingEffect(this->GetColorBrush(fontBack), subRange);
        this->pRenderTarget->DrawTextLayout({ x,y }, textLayout, this->GetColorBrush(color));
        textLayout->Release();
    }
}
void Graphics::DrawStringLayout(IDWriteTextLayout* textLayout, float x, float y, float w, float h, D2D1_COLOR_F color, DWRITE_TEXT_RANGE subRange, D2D1_COLOR_F fontBack, Font* font)
{
    if (textLayout)
    {
        textLayout->SetDrawingEffect(this->GetColorBrush(fontBack), subRange);
        this->pRenderTarget->DrawTextLayout({ x,y }, textLayout, this->GetColorBrush(color));
    }
}
void Graphics::DrawStringLayout(IDWriteTextLayout* textLayout, float x, float y, D2D1_COLOR_F color, DWRITE_TEXT_RANGE subRange, D2D1_COLOR_F fontBack)
{
    if (textLayout)
    {
        textLayout->SetDrawingEffect(this->GetColorBrush(fontBack), subRange);
        this->pRenderTarget->DrawTextLayout({ x,y }, textLayout, this->GetColorBrush(color));
    }
}
void Graphics::DrawStringLayout(std::wstring str, float x, float y, float w, float h, ID2D1Brush* brush, DWRITE_TEXT_RANGE subRange, D2D1_COLOR_F fontBack, Font* font)
{
    IDWriteTextLayout* textLayout = CreateStringLayout(str, w, h, font ? font : this->DefaultFontObject);
    if (textLayout)
    {
        textLayout->SetDrawingEffect(this->GetColorBrush(fontBack), subRange);
        this->pRenderTarget->DrawTextLayout({ x,y }, textLayout, brush);
        textLayout->Release();
    }
}
void Graphics::DrawStringLayout(std::wstring str, float x, float y, float w, float h, D2D1_COLOR_F color, Font* font)
{
    IDWriteTextLayout* textLayout = CreateStringLayout(str, w, h, font ? font : this->DefaultFontObject);
    if (textLayout)
    {
        this->pRenderTarget->DrawTextLayout({ x,y }, textLayout, this->GetColorBrush(color));
        textLayout->Release();
    }
}
void Graphics::DrawStringLayout(std::wstring str, float x, float y, float w, float h, ID2D1Brush* brush, Font* font)
{
    IDWriteTextLayout* textLayout = CreateStringLayout(str, w, h, font ? font : this->DefaultFontObject);
    if (textLayout)
    {
        this->pRenderTarget->DrawTextLayout({ x,y }, textLayout, brush);
        textLayout->Release();
    }
}
void Graphics::FillTriangle(D2D1_TRIANGLE triangle, D2D1_COLOR_F color)
{
    ID2D1PathGeometry* geo = nullptr;
    if (SUCCEEDED(_D2DFactory->CreatePathGeometry(&geo)))
    {
        ID2D1GeometrySink* tmp = NULL;
        if (SUCCEEDED(geo->Open(&tmp)))
        {
            tmp->BeginFigure(triangle.point1, D2D1_FIGURE_BEGIN_FILLED);
            tmp->AddLine(triangle.point2);
            tmp->AddLine(triangle.point3);
            tmp->AddLine(triangle.point1);
            tmp->EndFigure(D2D1_FIGURE_END::D2D1_FIGURE_END_CLOSED);
            tmp->Close();
            this->pRenderTarget->FillGeometry(geo, this->GetColorBrush(color));
            tmp->Release();
        }
        geo->Release();
    }
}
void Graphics::DrawTriangle(D2D1_TRIANGLE triangle, D2D1_COLOR_F color, float width)
{
    this->DrawLine(triangle.point1, triangle.point2, color, width);
    this->DrawLine(triangle.point2, triangle.point3, color, width);
    this->DrawLine(triangle.point3, triangle.point1, color, width);
}
void Graphics::FillPolygon(std::initializer_list<D2D1_POINT_2F> points, D2D1_COLOR_F color)
{
    ID2D1PathGeometry* geo = nullptr;
    if (SUCCEEDED(_D2DFactory->CreatePathGeometry(&geo)))
    {
        ID2D1GeometrySink* tmp = NULL;
        if (SUCCEEDED(geo->Open(&tmp)))
        {
            if (points.size() > 2)
            {
                tmp->BeginFigure(points.begin()[0], D2D1_FIGURE_BEGIN_FILLED);
                for (int i = 1; i < points.size(); i++)
                {
                    tmp->AddLine(points.begin()[i]);
                }
                tmp->EndFigure(D2D1_FIGURE_END::D2D1_FIGURE_END_CLOSED);
                tmp->Close();
                this->pRenderTarget->FillGeometry(geo, this->GetColorBrush(color));
                tmp->Release();
            }
        }
        geo->Release();
    }
}
void Graphics::DrawPolygon(std::initializer_list<D2D1_POINT_2F> points, D2D1_COLOR_F color, float width)
{
    ID2D1PathGeometry* geo = nullptr;
    if (SUCCEEDED(_D2DFactory->CreatePathGeometry(&geo)))
    {
        ID2D1GeometrySink* tmp = NULL;
        if (SUCCEEDED(geo->Open(&tmp)))
        {
            if (points.size() > 2)
            {
                tmp->BeginFigure(points.begin()[0], D2D1_FIGURE_BEGIN_FILLED);
                for (int i = 1; i < points.size(); i++)
                {
                    tmp->AddLine(points.begin()[i]);
                }
                tmp->EndFigure(D2D1_FIGURE_END::D2D1_FIGURE_END_OPEN);
                tmp->Close();
                this->pRenderTarget->DrawGeometry(geo, this->GetColorBrush(color), width);
                tmp->Release();
            }
        }
        this->DrawGeometry(geo, color, width);
        geo->Release();
    }
}
void Graphics::DrawPolygon(std::vector<D2D1_POINT_2F> points, D2D1_COLOR_F color, float width)
{
    ID2D1PathGeometry* geo = nullptr;
    if (SUCCEEDED(_D2DFactory->CreatePathGeometry(&geo)))
    {
        ID2D1GeometrySink* tmp = NULL;
        if (SUCCEEDED(geo->Open(&tmp)))
        {
            if (points.size() > 2)
            {
                tmp->BeginFigure(points[0], D2D1_FIGURE_BEGIN_FILLED);
                for (int i = 1; i < points.size(); i++)
                {
                    tmp->AddLine(points[i]);
                }
                tmp->EndFigure(D2D1_FIGURE_END::D2D1_FIGURE_END_OPEN);
                tmp->Close();
                this->pRenderTarget->DrawGeometry(geo, this->GetColorBrush(color), width);
                tmp->Release();
            }
        }
        this->DrawGeometry(geo, color, width);
        geo->Release();
    }
}
void Graphics::DrawArc(D2D1_POINT_2F center, float size, float sa, float ea, D2D1_COLOR_F color, float width)
{
    const auto __AngleToPoint = [](D2D1_POINT_2F Cent, float Angle, float Len) {
        if (Len <= 0)return Cent;
        return D2D1_POINT_2F{ (Cent.x + (sin(Angle * 3.1415926535897931F / 180.0f) * Len)), (Cent.y - (cos(Angle * 3.1415926535897931F / 180.0f) * Len)) };
        };
    ID2D1PathGeometry* geo = nullptr;
    if (SUCCEEDED(_D2DFactory->CreatePathGeometry(&geo)))
    {
        float ts = sa;
        float te = ea;
        if (te < ts) te += 360.0f;
        D2D1_ARC_SIZE sweep = (te - ts < 180.0f) ? D2D1_ARC_SIZE_SMALL : D2D1_ARC_SIZE_LARGE;
        ID2D1GeometrySink* pSink = NULL;
        if (SUCCEEDED(geo->Open(&pSink)))
        {
            auto start = __AngleToPoint(center, sa, size);
            auto end = __AngleToPoint(center, ea, size);
            pSink->BeginFigure(start, D2D1_FIGURE_BEGIN_FILLED);
            pSink->AddArc(
                D2D1::ArcSegment
                (
                    end,
                    D2D1::SizeF(size, size),
                    0.0f,
                    D2D1_SWEEP_DIRECTION_CLOCKWISE,
                    sweep
                )
            );
            pSink->EndFigure(D2D1_FIGURE_END::D2D1_FIGURE_END_OPEN);
            pSink->Close();
            this->pRenderTarget->DrawGeometry(geo, this->GetColorBrush(color), width);
            pSink->Release();
        }
        geo->Release();
    }
}
void Graphics::DrawArcCounter(D2D1_POINT_2F center, float size, float sa, float ea, D2D1_COLOR_F color, float width)
{
    const auto __AngleToPoint = [](D2D1_POINT_2F Cent, float Angle, float Len) {
        if (Len <= 0)return Cent;
        return D2D1_POINT_2F{ (Cent.x + (sin(Angle * 3.1415926535897931F / 180.0f) * Len)), (Cent.y - (cos(Angle * 3.1415926535897931F / 180.0f) * Len)) };
        };
    ID2D1PathGeometry* geo = nullptr;
    if (SUCCEEDED(_D2DFactory->CreatePathGeometry(&geo)))
    {
        float ts = sa;
        float te = ea;
        if (te < ts) te += 360.0f;
        D2D1_ARC_SIZE sweep = (te - ts < 180.0f) ? D2D1_ARC_SIZE_SMALL : D2D1_ARC_SIZE_LARGE;
        ID2D1GeometrySink* pSink = NULL;
        if (SUCCEEDED(geo->Open(&pSink)))
        {
            auto start = __AngleToPoint(center, sa, size);
            auto end = __AngleToPoint(center, ea, size);
            pSink->BeginFigure(start, D2D1_FIGURE_BEGIN_FILLED);
            pSink->AddArc(
                D2D1::ArcSegment
                (
                    end,
                    D2D1::SizeF(size, size),
                    0.0f,
                    D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE,
                    sweep
                )
            );
            pSink->EndFigure(D2D1_FIGURE_END::D2D1_FIGURE_END_OPEN);
            pSink->Close();
            this->pRenderTarget->DrawGeometry(geo, this->GetColorBrush(color), width);
            pSink->Release();
        }
        geo->Release();
    }
}
D2D1_SIZE_F Graphics::GetTextSize(std::wstring s, Font* font)
{
    return (font ? font : this->DefaultFontObject)->GetTextSize(s);
}
D2D1_SIZE_F Graphics::GetTextSize(wchar_t c, Font* font)
{
    return (font ? font : this->DefaultFontObject)->GetTextSize(c);
}
void Graphics::PushDrawRect(float left, float top, float width, float height)
{
    this->pRenderTarget->PushAxisAlignedClip(
        {
            left,
            top,
            left + width,
            top + height
        },
        D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
}
void Graphics::PopDrawRect()
{
    this->pRenderTarget->PopAxisAlignedClip();
}
void Graphics::SetAntialiasMode(D2D1_ANTIALIAS_MODE antialiasMode)
{
    pRenderTarget->SetAntialiasMode(antialiasMode);
}
void Graphics::SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE antialiasMode)
{
    pRenderTarget->SetTextAntialiasMode(antialiasMode);
}
ID2D1Bitmap* Graphics::ToBitmap(Graphics* targetUsing)
{
    if (_Target_Type == target_type::__bitmap)
    {
        ID2D1Bitmap* bmp = NULL;
        ((ID2D1BitmapRenderTarget*)pRenderTarget)->GetBitmap(&bmp);
        return bmp;
    }
    else if (_Target_Type == target_type::__wic)
    {
        return (targetUsing ? targetUsing : this)->CreateBitmap(pWICBitmap);
    }
    else
    {
        IWICBitmap* _WICBitmap = NULL;
        auto size = pRenderTarget->GetSize();
        _ImageFactory->CreateBitmap((UINT)size.width, (UINT)size.height, GUID_WICPixelFormat32bppPBGRA, WICBitmapCacheOnLoad, &_WICBitmap);
        auto bmp = this->CreateBitmap(_WICBitmap);
        D2D1_POINT_2U p = D2D1_POINT_2U{ (UINT)size.width, (UINT)size.height };
        D2D1_RECT_U r = { 0, 0 ,(UINT)size.width, (UINT)size.height };
        auto st = bmp->CopyFromRenderTarget(&p, pRenderTarget, &r);
        _WICBitmap->Release();
        return bmp;
    }
}
ID2D1Bitmap* Graphics::CreateBitmap(IWICBitmap* wb)
{
    ID2D1Bitmap* bmp;
    auto hr = this->pRenderTarget->CreateBitmapFromWicBitmap(wb, &bmp);
    return bmp;
}
ID2D1Bitmap* Graphics::CreateBitmap(const char* path)
{
    ID2D1Bitmap* bmp = nullptr;
    IWICBitmapDecoder* bitmapdecoder = NULL;
    _ImageFactory->CreateDecoderFromFilename(Convert::string_to_wstring(path).c_str(), NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &bitmapdecoder);//
    if (bitmapdecoder)
    {
        IWICBitmapFrameDecode* pframe = NULL;
        bitmapdecoder->GetFrame(0, &pframe);
        IWICFormatConverter* fmtcovter = NULL;
        _ImageFactory->CreateFormatConverter(&fmtcovter);
        fmtcovter->Initialize(pframe, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom);
        this->pRenderTarget->CreateBitmapFromWicBitmap(fmtcovter, NULL, &bmp);
        pframe->Release();
        fmtcovter->Release();
        bitmapdecoder->Release();
    }
    return bmp;
}
ID2D1Bitmap* Graphics::CreateBitmap(const wchar_t* path)
{
    ID2D1Bitmap* bmp = nullptr;
    IWICBitmapDecoder* bitmapdecoder = NULL;
    _ImageFactory->CreateDecoderFromFilename(path, NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &bitmapdecoder);//
    if (bitmapdecoder)
    {
        IWICBitmapFrameDecode* pframe = NULL;
        bitmapdecoder->GetFrame(0, &pframe);
        IWICFormatConverter* fmtcovter = NULL;
        _ImageFactory->CreateFormatConverter(&fmtcovter);
        fmtcovter->Initialize(pframe, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom);
        this->pRenderTarget->CreateBitmapFromWicBitmap(fmtcovter, NULL, &bmp);
        pframe->Release();
        fmtcovter->Release();
        bitmapdecoder->Release();
    }
    return bmp;
}
ID2D1Bitmap* Graphics::CreateBitmap(unsigned char* data, int size)
{
    ID2D1Bitmap* bmp = nullptr;
    IWICBitmapDecoder* bitmapdecoder = NULL;
    IWICStream* pStream;
    auto errmsg = _ImageFactory->CreateStream(&pStream);
    if (!SUCCEEDED(errmsg))
    {
        return nullptr;
    }
    errmsg = pStream->InitializeFromMemory(data, size);
    if (!SUCCEEDED(errmsg))
    {
        return nullptr;
    }
    errmsg = _ImageFactory->CreateDecoderFromStream(pStream, NULL, WICDecodeMetadataCacheOnDemand, &bitmapdecoder);
    if (!SUCCEEDED(errmsg))
    {
        return nullptr;
    }
    if (bitmapdecoder)
    {
        IWICBitmapFrameDecode* pframe = NULL;
        bitmapdecoder->GetFrame(0, &pframe);
        IWICFormatConverter* fmtcovter = NULL;
        _ImageFactory->CreateFormatConverter(&fmtcovter);
        fmtcovter->Initialize(pframe, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom);
        this->pRenderTarget->CreateBitmapFromWicBitmap(fmtcovter, NULL, &bmp);
        pframe->Release();
        fmtcovter->Release();
        bitmapdecoder->Release();
    }
    return bmp;
}
ID2D1Bitmap* Graphics::CreateBitmap(HBITMAP hb)
{
    IWICBitmap* wb = nullptr;
    ID2D1Bitmap* bmp;
    _ImageFactory->CreateBitmapFromHBITMAP(hb, 0, WICBitmapUsePremultipliedAlpha, &wb);
    auto hr = this->pRenderTarget->CreateBitmapFromWicBitmap(wb, &bmp);
    wb->Release();
    return bmp;
}
ID2D1Bitmap* Graphics::CreateBitmap(HICON hb)
{
    IWICBitmap* wb = nullptr;
    ID2D1Bitmap* bmp = nullptr;
    _ImageFactory->CreateBitmapFromHICON(hb, &wb);
    this->pRenderTarget->CreateBitmapFromWicBitmap(wb, &bmp);
    wb->Release();
    return bmp;
}
ID2D1Bitmap* Graphics::CreateBitmap(int width, int height)
{
    IWICBitmap* wb = nullptr;
    ID2D1Bitmap* bmp;
    _ImageFactory->CreateBitmap(width, height, GUID_WICPixelFormat32bppPBGRA, WICBitmapCacheOnDemand, &wb);
    auto hr = this->pRenderTarget->CreateBitmapFromWicBitmap(wb, &bmp);
    wb->Release();
    return bmp;
}
IWICBitmap* Graphics::CreateWICBitmap(ID2D1Bitmap* bmp)
{
    IWICBitmap* wb = nullptr;
    _ImageFactory->CreateBitmap(bmp->GetSize().width, bmp->GetSize().height, GUID_WICPixelFormat32bppPBGRA, WICBitmapCacheOnLoad, &wb);
    return wb;
}
IWICBitmap* Graphics::CreateWICBitmap(const char* path)
{
    IWICBitmap* wb = NULL;
    IWICBitmapDecoder* bitmapdecoder = NULL;
    _ImageFactory->CreateDecoderFromFilename(Convert::string_to_wstring(path).c_str(), NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &bitmapdecoder);
    if (bitmapdecoder)
    {
        IWICBitmapFrameDecode* pframe = NULL;
        bitmapdecoder->GetFrame(0, &pframe);
        IWICFormatConverter* fmtcovter = NULL;
        _ImageFactory->CreateFormatConverter(&fmtcovter);
        fmtcovter->Initialize(pframe, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom);
        pframe->Release();
        bitmapdecoder->Release();
        return (IWICBitmap*)fmtcovter;
    }
    return wb;
}
IWICBitmap* Graphics::CreateWICBitmap(unsigned char* data, int size)
{
    ID2D1Bitmap* bmp = nullptr;
    IWICBitmapDecoder* bitmapdecoder = NULL;
    IWICStream* pStream;
    auto errmsg = _ImageFactory->CreateStream(&pStream);
    if (!SUCCEEDED(errmsg))
    {
        return nullptr;
    }
    errmsg = pStream->InitializeFromMemory(data, size);
    if (!SUCCEEDED(errmsg))
    {
        return nullptr;
    }
    errmsg = _ImageFactory->CreateDecoderFromStream(pStream, NULL, WICDecodeMetadataCacheOnDemand, &bitmapdecoder);
    if (!SUCCEEDED(errmsg))
    {
        return nullptr;
    }
    if (bitmapdecoder)
    {
        IWICBitmapFrameDecode* pframe = NULL;
        bitmapdecoder->GetFrame(0, &pframe);
        IWICFormatConverter* fmtcovter = NULL;
        _ImageFactory->CreateFormatConverter(&fmtcovter);
        fmtcovter->Initialize(pframe, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom);
        pframe->Release();
        bitmapdecoder->Release();
        return (IWICBitmap*)fmtcovter;
    }
    return NULL;
}
IWICBitmap* Graphics::CreateWICBitmap(HBITMAP hb)
{
    IWICBitmap* wb = nullptr;
    _ImageFactory->CreateBitmapFromHBITMAP(hb, 0, WICBitmapUsePremultipliedAlpha, &wb);
    return wb;
}
IWICBitmap* Graphics::CreateWICBitmap(int width, int height)
{
    IWICBitmap* wb = nullptr;
    _ImageFactory->CreateBitmap(width, height, GUID_WICPixelFormat32bppPBGRA, WICBitmapCacheOnDemand, &wb);
    return wb;
}
IWICBitmap* Graphics::CreateWICBitmap(HICON hb)
{
    IWICBitmap* wb = nullptr;
    _ImageFactory->CreateBitmapFromHICON(hb, &wb);
    return wb;
}
void Graphics::SaveBitmap(IWICBitmap* bmp, const wchar_t* path)
{
    UINT w, h;
    bmp->GetSize(&w, &h);
    IWICBitmapEncoder* pEncoder = NULL;
    IWICStream* pStream = NULL;
    IWICBitmapFrameEncode* pFrameEncode = NULL;
    WICPixelFormatGUID format = GUID_WICPixelFormat32bppPBGRA;
    _ImageFactory->CreateStream(&pStream);
    _ImageFactory->CreateEncoder(GUID_ContainerFormatPng, NULL, &pEncoder);
    pStream->InitializeFromFilename(path, GENERIC_WRITE);
    pEncoder->Initialize(pStream, WICBitmapEncoderNoCache);
    pEncoder->CreateNewFrame(&pFrameEncode, NULL);
    pFrameEncode->Initialize(NULL);
    pFrameEncode->SetSize(w, h);
    pFrameEncode->SetPixelFormat(&format);
    pFrameEncode->WriteSource(bmp, NULL);
    pFrameEncode->Commit();
    pEncoder->Commit();
    pFrameEncode->Release();
    pStream->Release();
    pEncoder->Release();
}
std::vector<ID2D1Bitmap*> Graphics::CreateBitmapFromGif(const char* path)
{
    std::vector<ID2D1Bitmap*> result = std::vector<ID2D1Bitmap*>();
    ID2D1Bitmap* bmp = nullptr;
    IWICBitmapDecoder* bitmapdecoder = NULL;
    _ImageFactory->CreateDecoderFromFilename(Convert::string_to_wstring(path).c_str(), NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &bitmapdecoder);
    if (bitmapdecoder)
    {
        UINT count = 0;
        bitmapdecoder->GetFrameCount(&count);
        for (int i = 0; i < count; i++)
        {
            IWICBitmapFrameDecode* pframe = NULL;
            bitmapdecoder->GetFrame(i, &pframe);
            IWICFormatConverter* fmtcovter = NULL;
            _ImageFactory->CreateFormatConverter(&fmtcovter);
            fmtcovter->Initialize(pframe, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom);
            this->pRenderTarget->CreateBitmapFromWicBitmap(fmtcovter, NULL, &bmp);
            pframe->Release();
            fmtcovter->Release();
            result.push_back(bmp);
        }
        bitmapdecoder->Release();
    }
    return result;
}
std::vector<ID2D1Bitmap*> Graphics::CreateBitmapFromGifBuffer(unsigned char* data, int size)
{
    std::vector<ID2D1Bitmap*> result = std::vector<ID2D1Bitmap*>();
    ID2D1Bitmap* bmp = nullptr;
    IWICBitmapDecoder* bitmapdecoder = NULL;
    IWICStream* pStream;
    auto errmsg = _ImageFactory->CreateStream(&pStream);
    if (!SUCCEEDED(errmsg))
    {
        return result;
    }
    errmsg = pStream->InitializeFromMemory(data, size);
    if (!SUCCEEDED(errmsg))
    {
        return result;
    }
    errmsg = _ImageFactory->CreateDecoderFromStream(pStream, NULL, WICDecodeMetadataCacheOnDemand, &bitmapdecoder);
    if (!SUCCEEDED(errmsg))
    {
        return result;
    }
    if (bitmapdecoder)
    {
        UINT count = 0;
        bitmapdecoder->GetFrameCount(&count);
        for (int i = 0; i < count; i++)
        {
            IWICBitmapFrameDecode* pframe = NULL;
            bitmapdecoder->GetFrame(i, &pframe);
            IWICFormatConverter* fmtcovter = NULL;
            _ImageFactory->CreateFormatConverter(&fmtcovter);
            fmtcovter->Initialize(pframe, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom);
            this->pRenderTarget->CreateBitmapFromWicBitmap(fmtcovter, NULL, &bmp);
            pframe->Release();
            fmtcovter->Release();
            result.push_back(bmp);
        }
        bitmapdecoder->Release();
    }
    return result;
}
ID2D1PathGeometry* Graphics::CreateGeomtry()
{
    ID2D1PathGeometry* geo = nullptr;
    HRESULT hr = _D2DFactory->CreatePathGeometry(&geo);
    if (SUCCEEDED(hr))
        return geo;
    else
        return nullptr;
}
HBITMAP Graphics::GetHBITMAPFromImageFile(const wchar_t* path)
{
    HBITMAP hBitmap = (HBITMAP)LoadImageW(NULL, path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    return hBitmap;
}
HBITMAP Graphics::CopyFromScreen(int x, int y, int width, int height)
{
    HDC hScreen = GetDC(NULL);
    HDC hDC = CreateCompatibleDC(hScreen);
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, width, height);
    HGDIOBJ old_obj = SelectObject(hDC, hBitmap);
    BOOL bRet = BitBlt(hDC, x, y, width, height, hScreen, 0, 0, SRCCOPY);
    SelectObject(hDC, old_obj);
    DeleteDC(hDC);
    ReleaseDC(NULL, hScreen);
    return hBitmap;
}
HBITMAP Graphics::CopyFromWidnow(HWND handle, int x, int y, int width, int height)
{
    HDC hScreen = GetDC(handle);
    HDC hDC = CreateCompatibleDC(hScreen);
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, width, height);
    HGDIOBJ old_obj = SelectObject(hDC, hBitmap);
    BOOL bRet = BitBlt(hDC, x, y, width, height, hScreen, 0, 0, SRCCOPY);
    SelectObject(hDC, old_obj);
    DeleteDC(hDC);
    ReleaseDC(NULL, hScreen);
    return hBitmap;
}
SIZE Graphics::GetScreenSize(int index)
{
    std::vector<SIZE> sizes = std::vector<SIZE>();
    auto callback = [](HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
        {
            std::vector<SIZE>* tmp = (std::vector<SIZE>*)dwData;
            MONITORINFO info;
            info.cbSize = sizeof(info);
            if (GetMonitorInfoW(hMonitor, &info))
            {
                tmp->push_back({ info.rcMonitor.right - info.rcMonitor.left,info.rcMonitor.bottom - info.rcMonitor.top });
            }
            return TRUE;
        };
    EnumDisplayMonitors(NULL, NULL, (MONITORENUMPROC)callback, (LPARAM)&sizes);
    if (sizes.size() > index)
    {
        return sizes[index];
    }
    return { GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN) };
}
ID2D1Bitmap* Graphics::ToBitmapFromSvg(const char* data)
{
    int len = strlen(data) + 1;
    char* svg_text = new char[len];
    memcpy(svg_text, data, len);
    NSVGimage* image = nsvgParse(svg_text, "px", 96.0f);
    float percen = 1.0f;
    if (image->width > 4096 || image->height > 4096)
    {
        float maxv = image->width > image->height ? image->width : image->height;
        percen = 4096.0f / maxv;
    }
    auto subg = this->CreateSubGraphics(image->width * percen, image->height * percen);
    NSVGshape* shape;
    NSVGpath* path;
    subg->BeginRender();
    for (shape = image->shapes; shape != NULL; shape = shape->next)
    {
        auto geo = Graphics::CreateGeomtry();
        if (geo)
        {
            ID2D1GeometrySink* skin = NULL;
            geo->Open(&skin);
            if (skin)
            {
                for (path = shape->paths; path != NULL; path = path->next)
                {
                    for (int i = 0; i < path->npts - 1; i += 3)
                    {
                        float* p = &path->pts[i * 2];
                        if (i == 0)
                        {
                            skin->BeginFigure({ p[0] * percen, p[1] * percen }, D2D1_FIGURE_BEGIN_FILLED);
                        }
                        skin->AddBezier({ {p[2] * percen, p[3] * percen}, {p[4] * percen, p[5] * percen}, {p[6] * percen, p[7] * percen} });
                    }
                    skin->EndFigure(path->closed ? D2D1_FIGURE_END_CLOSED : D2D1_FIGURE_END_OPEN);
                }
            }
            skin->Close();
        }
        auto _get_svg_brush = [](NSVGpaint paint, float opacity, Graphics* g) ->ID2D1Brush*
            {
                const auto ic2fc = [](int colorInt, float opacity)->D2D1_COLOR_F
                    {
                        D2D1_COLOR_F ret = { (float)GetRValue(colorInt) / 255.0f ,(float)GetGValue(colorInt) / 255.0f ,(float)GetBValue(colorInt) / 255.0f ,opacity };
                        return ret;
                    };
                switch (paint.type)
                {
                case NSVG_PAINT_NONE:
                {
                    return NULL;
                }
                break;
                case NSVG_PAINT_COLOR:
                {
                    return g->CreateSolidColorBrush(ic2fc(paint.color, opacity));
                }
                break;
                case NSVG_PAINT_LINEAR_GRADIENT:
                {
                    std::vector<D2D1_GRADIENT_STOP> cols;
                    for (int i = 0; i < paint.gradient->nstops; i++)
                    {
                        auto stop = paint.gradient->stops[i];
                        cols.push_back({ stop.offset, ic2fc(stop.color, opacity) });
                    }
                    return g->CreateLinearGradientBrush(cols.data(), cols.size());
                }
                break;
                case NSVG_PAINT_RADIAL_GRADIENT:
                {
                    std::vector<D2D1_GRADIENT_STOP> cols;
                    for (int i = 0; i < paint.gradient->nstops; i++)
                    {
                        auto stop = paint.gradient->stops[i];
                        cols.push_back({ stop.offset, ic2fc(stop.color, opacity) });
                    }
                    return g->CreateRadialGradientBrush(cols.data(), cols.size(), { paint.gradient->fx,paint.gradient->fy });
                }
                break;
                }
                return NULL;
            };
        ID2D1Brush* brush = _get_svg_brush(shape->fill, shape->opacity, subg);
        if (brush)
        {
            subg->FillGeometry(geo, brush);
            brush->Release();
        }
        brush = _get_svg_brush(shape->stroke, shape->opacity, subg);
        if (brush)
        {
            subg->DrawGeometry(geo, brush, shape->strokeWidth);
            brush->Release();
        }
        geo->Release();
    }
    nsvgDelete(image);
    subg->EndRender();
    auto result = (ID2D1Bitmap*)subg->ToBitmap();
    delete subg;
    return result;
}