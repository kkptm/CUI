#pragma once
#include <D2D1.h>
#include <DWrite.h>
#include <wincodec.h>

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