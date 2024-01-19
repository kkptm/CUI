#pragma once
#include "Veil.h"
#ifndef PROPERTY
#define PROPERTY(t,n) __declspec( property (put = Set##n, get = Get##n)) t n
#define READONLY_PROPERTY(t,n) __declspec( property (get = Get##n) ) t n
#define WRITEONLY_PROPERTY(t,n) __declspec( property (put = Set##n) ) t n
#define GET(t,n) t Get##n() 
#define SET(t,n) void Set##n(t value)
#define PROPERTY_CPP(t,n) __declspec( property (put = Set##n, get = Get##n)); t nt Get##n();t Get##n();
#define GET_CPP(c,t,n) t c::Get##n() 
#define SET_CPP(c,t,n) void c::Set##n(t value)
#endif // !PROPERTY
#ifndef typeof
#define typeof(x) decltype(x)
#endif