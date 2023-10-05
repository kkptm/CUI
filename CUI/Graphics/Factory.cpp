#include "Factory.h"
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "Dwrite.lib")
#pragma comment(lib, "windowscodecs.lib")
ID2D1Factory* Factory::D2DFactory()
{
    static ID2D1Factory* pD2DFactory = nullptr;
    if (!pD2DFactory)
        D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &pD2DFactory);
    return pD2DFactory;
}
IDWriteFactory* Factory::DWriteFactory()
{
    static IDWriteFactory* pDWriteFactory = nullptr;
    if (!pDWriteFactory)
        DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&pDWriteFactory));
    return pDWriteFactory;
}
IWICImagingFactory* Factory::ImageFactory()
{
    static IWICImagingFactory* _pImageFactory = nullptr;
    if (!_pImageFactory) {
        auto m = LoadLibraryA("Ole32.dll");
        if (m) {
            decltype(CoInitialize)* f = (decltype(CoInitialize)*)GetProcAddress(m, "CoInitialize");
            decltype(CoCreateInstance)* f1 = (decltype(CoCreateInstance)*)GetProcAddress(m, "CoCreateInstance");
            auto hr = f(NULL);
            hr = f1(CLSID_WICImagingFactory1, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)&_pImageFactory);
        }
    }
    return _pImageFactory;
}