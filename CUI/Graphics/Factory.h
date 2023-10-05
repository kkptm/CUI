#pragma once
#include <D2D1.h>
#include <DWrite.h>
#include <wincodec.h>

#ifndef PROPERTY
#define PROPERTY(t,n) __declspec( property (put = property__set_##n, get = property__get_##n)) t n
#define GET(t,n) t property__get_##n() 
#define SET(t,n) void property__set_##n(t value)

#define READONLY_PROPERTY(t,n) __declspec( property (get = property__get_##n) ) t n
#define WRITEONLY_PROPERTY(t,n) __declspec( property (put = property__set_##n) ) t n
#define _readonly_property(t,n) READONLY_PROPERTY(t,n);GET(t,n)
#endif // !PROPERTY

class Factory
{
public:
    static ID2D1Factory* D2DFactory();
    static IDWriteFactory* DWriteFactory();
    static IWICImagingFactory* ImageFactory();
};

#define _D2DFactory     Factory::D2DFactory()
#define _DWriteFactory  Factory::DWriteFactory()
#define _ImageFactory   Factory::ImageFactory()