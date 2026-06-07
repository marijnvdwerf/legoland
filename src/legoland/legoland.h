#ifndef LEGOLAND_H
#define LEGOLAND_H

#define STUB() ((void)0)

#if defined(_MSC_VER) && !defined(__clang__)
#define LEGO_EXPORT __declspec(dllexport)
#else
#define LEGO_EXPORT
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif

#endif
