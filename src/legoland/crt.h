#pragma once

#include <stdarg.h>

extern int FUN_0049e4b2(void);
extern void FUN_0049e4d0(void *block);
extern int FUN_0049e573(char *buffer, const char *format, ...);
extern void FUN_0049e5c5(const char *msg);
extern int FUN_0049efee(void *stream);
extern unsigned int FUN_0049f044(void *ptr, unsigned int size, unsigned int count, void *stream);
extern void *FUN_0049f330(const char *filename, const char *mode);
extern unsigned int FUN_0049f4ca(int fd, void *buffer, unsigned int count);
extern unsigned int FUN_0049fdc2(void *block);
extern int FUN_0049fdeb(char *buffer, const char *format, va_list ap);
extern void *FUN_004a020e(unsigned int count, unsigned int size);
extern __declspec(noreturn) void FUN_004a02b8(int status);
extern int FUN_004a04b9(const char *str);
extern unsigned int FUN_004a56c3(unsigned int a, unsigned int b, unsigned int c);
extern int FUN_004a63e4(int fd, void *buffer, unsigned int count);

extern unsigned int strlen(const char *s);
extern char *strcpy(char *dst, const char *src);
extern void *memcpy(void *dst, const void *src, unsigned int n);
extern void *memset(void *dst, int c, unsigned int n);

extern void *_malloc(unsigned int size);
extern char *_strncpy(char *dest, const char *src, unsigned int count);
extern char *_strrchr(const char *s, int c);
extern int __strcmpi(const char *s1, const char *s2);
extern char *_strstr(const char *haystack, const char *needle);
extern char *FUN_004a0600(char *s);
