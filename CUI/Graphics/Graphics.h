#pragma once
#include "Font.h"
#include "Colors.h"
class Graphics
{
    enum class target_type
    {
        __hwnd,
        __surface,
        __swapchain,
        __wic,
        __bitmap
    };
private:
    target_type _Target_Type = target_type::__hwnd;
    IWICBitmap* pWICBitmap = NULL;
    int brush_get_index = 0;
    ID2D1RenderTarget* pRenderTarget = nullptr;
    ID2D1SolidColorBrush* Default_Brush = NULL;
    inline ID2D1SolidColorBrush* GetBrush();
public:
    HWND WindowHandle;
    Font* DefaultFontObject = nullptr;
    Graphics(IDXGISurface* dxgiSurface);
    Graphics(IDXGISwapChain* swap_chain_ptr);
    Graphics(HWND hWnd);
    Graphics(int width, int height);
    Graphics(IWICBitmap* wic);
    Graphics(ID2D1Bitmap* bmp);
    Graphics(ID2D1BitmapRenderTarget* target);
    ~Graphics();
    ID2D1SolidColorBrush* GetColorBrush(D2D1_COLOR_F newcolor);
    ID2D1SolidColorBrush* GetColorBrush(COLORREF newcolor);
    ID2D1SolidColorBrush* GetColorBrush(int r, int g, int b);
    ID2D1SolidColorBrush* GetColorBrush(float r, float g, float b, float a);
    ID2D1LinearGradientBrush* CreateLinearGradientBrush(D2D1_GRADIENT_STOP* stops, unsigned int stopcount);
    ID2D1RadialGradientBrush* CreateRadialGradientBrush(D2D1_GRADIENT_STOP* stops, unsigned int stopcount, D2D1_POINT_2F center);
    ID2D1SolidColorBrush* CreateSolidColorBrush(D2D1_COLOR_F color);
    Graphics* CreateSubGraphics(int width, int height);
    D2D1_SIZE_F Size();
    void Resize();
    void ResizeSubGraphics(int width, int height);
    void BeginRender(CONST D2D1_COLOR_F clearColor);
    void BeginRender();
    void EndRender();
    void DrawLine(D2D1_POINT_2F p1, D2D1_POINT_2F p2, D2D1_COLOR_F color, float linewidth = 1.0f);
    void DrawLine(float p1_x, float p1_y, float p2_x, float p2_y, D2D1_COLOR_F color, float linewidth = 1.0f);
    void DrawLine(D2D1_POINT_2F p1, D2D1_POINT_2F p2, ID2D1Brush* brush, float linewidth = 1.0f);
    void DrawLine(float p1_x, float p1_y, float p2_x, float p2_y, ID2D1Brush* brush, float linewidth = 1.0f);
    void DrawRect(float left, float top, float width, float height, D2D1_COLOR_F color, float linewidth = 1.0f);
    void DrawRoundRect(float left, float top, float width, float height, D2D1_COLOR_F color, float linewidth = 1.0f, float r = 0.5f);
    void FillRect(float left, float top, float width, float height, D2D1_COLOR_F color);
    void FillRoundRect(float left, float top, float width, float height, D2D1_COLOR_F color, float r = 0.5f);
    void DrawEllipse(D2D1_POINT_2F cent, float xr, float yr, D2D1_COLOR_F color, float linewidth = 1.0f);
    void DrawEllipse(D2D1_ELLIPSE ellipse, D2D1_COLOR_F color, float linewidth = 1.0f);
    void DrawEllipse(float x, float y, float xr, float yr, D2D1_COLOR_F color, float linewidth = 1.0f);
    void FillEllipse(D2D1_ELLIPSE ellipse, D2D1_COLOR_F color);
    void FillEllipse(D2D1_POINT_2F cent, float xr, float yr, D2D1_COLOR_F color);
    void FillEllipse(float cx, float cy, float xr, float yr, D2D1_COLOR_F color);
    void DrawGeometry(ID2D1Geometry* geo, D2D1_COLOR_F color, float linewidth);
    void FillGeometry(ID2D1Geometry* geo, D2D1_COLOR_F color);
    void DrawGeometry(ID2D1Geometry* geo, ID2D1Brush* brush, float linewidth);
    void FillGeometry(ID2D1Geometry* geo, ID2D1Brush* brush);
    void FillMesh(ID2D1Mesh* mesh, D2D1_COLOR_F color);
    void DrawBitmap(ID2D1Bitmap* bmp, float x, float y, float opacity = 1.0f, D2D1_COLOR_F border = Colors::Opacity, float borderwidth = 1.0f);
    void DrawBitmap(ID2D1Bitmap* bmp, float x, float y, float w, float h, float opacity = 1.0f, D2D1_COLOR_F border = Colors::Opacity, float borderwidth = 1.0f);
    void DrawBitmap(ID2D1Bitmap* bmp, float sx, float sy, float sw, float sh, float x, float y, float w, float h, float opacity = 1.0f, D2D1_COLOR_F border = Colors::Black, float borderwidth = 1.0f);
    void DrawString(const char* str, float x, float y, ID2D1Brush* brush, Font* font = nullptr);
    void DrawString(const wchar_t* str, float x, float y, ID2D1Brush* brush, Font* font = nullptr);
    void DrawString(const char* str, float x, float y, D2D1_COLOR_F color, Font* font = nullptr);
    void DrawString(const wchar_t* str, float x, float y, D2D1_COLOR_F color, Font* font = nullptr);
    void DrawString(std::string str, float x, float y, D2D1_COLOR_F color, Font* font = nullptr);
    void DrawString(std::string str, float x, float y, ID2D1Brush* brush, Font* font = nullptr);
    void DrawString(std::wstring str, float x, float y, D2D1_COLOR_F color, Font* font = nullptr);
    void DrawString(std::wstring str, float x, float y, ID2D1Brush* brush, Font* font = nullptr);
    IDWriteTextLayout* CreateStringLayout(std::string str, Font* font = nullptr);
    IDWriteTextLayout* CreateStringLayout(std::wstring str, Font* font = nullptr);
    IDWriteTextLayout* CreateStringLayout(std::wstring str, float width, float height, Font* font = nullptr);
    void DrawStringLayout(IDWriteTextLayout* layout, float x, float y, D2D1_COLOR_F color);
    void DrawStringLayout(IDWriteTextLayout* layout, float x, float y, ID2D1Brush* brush);
    D2D1_SIZE_F DrawStringLayoutCent(IDWriteTextLayout* layout, float x, float y, D2D1_COLOR_F color, D2D1_COLOR_F back = Colors::Opacity);
    D2D1_SIZE_F DrawStringCent(std::string str, float x, float y, D2D1_COLOR_F color, IDWriteTextLayout** _recodeLayout = nullptr, D2D1_COLOR_F back = Colors::Opacity, Font* font = nullptr);
    D2D1_SIZE_F DrawStringCent(std::wstring str, float x, float y, D2D1_COLOR_F color, IDWriteTextLayout** _recodeLayout = nullptr, D2D1_COLOR_F back = Colors::Opacity, Font* font = nullptr);
    D2D1_SIZE_F DrawStringLayoutCentHorizontal(IDWriteTextLayout* layout, float x, float y, D2D1_COLOR_F color, D2D1_COLOR_F back = Colors::Opacity);
    D2D1_SIZE_F DrawStringCentHorizontal(std::string str, float x, float y, D2D1_COLOR_F color, IDWriteTextLayout** _recodeLayout = nullptr, D2D1_COLOR_F back = Colors::Opacity, Font* font = nullptr);
    D2D1_SIZE_F DrawStringCentHorizontal(std::wstring str, float x, float y, D2D1_COLOR_F color, IDWriteTextLayout** _recodeLayout = nullptr, D2D1_COLOR_F back = Colors::Opacity, Font* font = nullptr);
    D2D1_SIZE_F DrawStringLayoutCentVertical(IDWriteTextLayout* layout, float x, float y, D2D1_COLOR_F color, D2D1_COLOR_F back = Colors::Opacity);
    D2D1_SIZE_F DrawStringCentVertical(std::string str, float x, float y, D2D1_COLOR_F color, IDWriteTextLayout** _recodeLayout = nullptr, D2D1_COLOR_F back = Colors::Opacity, Font* font = nullptr);
    D2D1_SIZE_F DrawStringCentVertical(std::wstring str, float x, float y, D2D1_COLOR_F color, IDWriteTextLayout** _recodeLayout = nullptr, D2D1_COLOR_F back = Colors::Opacity, Font* font = nullptr);
    void DrawStringLayout(std::string str, float x, float y, D2D1_COLOR_F color, D2D1_COLOR_F back = Colors::Opacity, Font* font = nullptr);
    void DrawStringLayout(std::wstring str, float x, float y, D2D1_COLOR_F color, D2D1_COLOR_F back = Colors::Opacity, Font* font = nullptr);
    void DrawStringLayout(std::wstring str, float x, float y, float w, float h, D2D1_COLOR_F color, D2D1_COLOR_F back = Colors::Opacity, Font* font = nullptr);
    void DrawStringLayout(std::wstring str, float x, float y, D2D1_COLOR_F color, DWRITE_TEXT_RANGE subRange, D2D1_COLOR_F fontBack = Colors::Opacity, Font* font = nullptr);
    void DrawStringLayout(std::wstring str, float x, float y, float w, float h, D2D1_COLOR_F color, DWRITE_TEXT_RANGE subRange, D2D1_COLOR_F fontBack = Colors::Opacity, Font* font = nullptr);
    void DrawStringLayout(IDWriteTextLayout* textLayout, float x, float y, D2D1_COLOR_F color, DWRITE_TEXT_RANGE subRange, D2D1_COLOR_F fontBack = Colors::Opacity);
    void DrawStringLayout(std::wstring str, float x, float y, float w, float h, ID2D1Brush* brush, DWRITE_TEXT_RANGE subRange, D2D1_COLOR_F fontBack = Colors::Opacity, Font* font = nullptr);
    void DrawStringLayout(std::wstring str, float x, float y, float w, float h, D2D1_COLOR_F color, Font* font = nullptr);
    void DrawStringLayout(std::wstring str, float x, float y, float w, float h, ID2D1Brush* brush, Font* font = nullptr);
    void DrawStringLayout(IDWriteTextLayout* textLayout, float x, float y, float w, float h, D2D1_COLOR_F color, DWRITE_TEXT_RANGE subRange, D2D1_COLOR_F fontBack = Colors::Opacity, Font* font = nullptr);
    void FillTriangle(D2D1_TRIANGLE triangle, D2D1_COLOR_F color);
    void DrawTriangle(D2D1_TRIANGLE triangle, D2D1_COLOR_F color, float width);
    void FillPolygon(std::initializer_list<D2D1_POINT_2F> points, D2D1_COLOR_F color);
    void DrawPolygon(std::initializer_list<D2D1_POINT_2F> points, D2D1_COLOR_F color, float width);
    void DrawPolygon(std::vector<D2D1_POINT_2F> points, D2D1_COLOR_F color, float width);
    void DrawArc(D2D1_POINT_2F center, float size, float sa, float ea, D2D1_COLOR_F color, float width);
    void DrawArcCounter(D2D1_POINT_2F center, float size, float sa, float ea, D2D1_COLOR_F color, float width);
    D2D1_SIZE_F GetTextSize(std::wstring s, Font* font = nullptr);
    D2D1_SIZE_F GetTextSize(wchar_t c, Font* font = nullptr);
    void PushDrawRect(float left, float top, float width, float height);
    void PopDrawRect();
    void SetAntialiasMode(D2D1_ANTIALIAS_MODE antialiasMode);
    void SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE antialiasMode);
    //如果是直接使用大小创建的内存渲染对象的话这里必须传入一个目标Graphics对象,否则返回的位图无法在其他渲染对象上使用
    ID2D1Bitmap* ToBitmap(Graphics* targetUsing = NULL);
    ID2D1Bitmap* CreateBitmap(IWICBitmap* wb);
    ID2D1Bitmap* CreateBitmap(const char* path);
    ID2D1Bitmap* CreateBitmap(const wchar_t* path);
    ID2D1Bitmap* CreateBitmap(unsigned char* data, int size);
    ID2D1Bitmap* CreateBitmap(HBITMAP hb);
    ID2D1Bitmap* CreateBitmap(HICON hb);
    ID2D1Bitmap* CreateBitmap(int width, int height);
    static IWICBitmap* CreateWICBitmap(ID2D1Bitmap* bmp);
    static IWICBitmap* CreateWICBitmap(const char* path);
    static IWICBitmap* CreateWICBitmap(unsigned char* data, int size);
    static IWICBitmap* CreateWICBitmap(HBITMAP hb);
    static IWICBitmap* CreateWICBitmap(int width, int height);
    static IWICBitmap* CreateWICBitmap(HICON hb);
    static void SaveBitmap(IWICBitmap* bmp, const wchar_t* path);
    std::vector<ID2D1Bitmap*> CreateBitmapFromGif(const char* path);
    std::vector<ID2D1Bitmap*> CreateBitmapFromGifBuffer(unsigned char* data, int size);
    static ID2D1PathGeometry* CreateGeomtry();
    static HBITMAP GetHBITMAPFromImageFile(const wchar_t* path);
    static HBITMAP CopyFromScreen(int x, int y, int width, int height);
    static HBITMAP CopyFromWidnow(HWND handle, int x, int y, int width, int height);
    static SIZE GetScreenSize(int index = 0);
    ID2D1Bitmap* ToBitmapFromSvg(const char* data);
};

