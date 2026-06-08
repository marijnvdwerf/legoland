#pragma once

#include <stdarg.h>

extern int rand(void);
extern void free(void *block);
extern int sprintf(char *buffer, const char *format, ...);
extern void printf(const char *msg);
extern int fclose(void *stream);
extern unsigned int fread(void *ptr, unsigned int size, unsigned int count, void *stream);
extern void *fopen(const char *filename, const char *mode);
extern unsigned int _read(int fd, void *buffer, unsigned int count);
extern unsigned int _msize(void *block);
extern int vsprintf(char *buffer, const char *format, va_list ap);
extern void *calloc(unsigned int count, unsigned int size);
extern __declspec(noreturn) void exit(int status);
extern int atoi(const char *str);
extern unsigned int _lseek(unsigned int a, unsigned int b, unsigned int c);
extern int _write(int fd, void *buffer, unsigned int count);

extern unsigned int strlen(const char *s);
extern char *strcpy(char *dst, const char *src);
extern int strcmp(const char *s1, const char *s2);
extern void *memcpy(void *dst, const void *src, unsigned int n);
extern void *memset(void *dst, int c, unsigned int n);

extern void *malloc(unsigned int size);
extern char *strncpy(char *dest, const char *src, unsigned int count);
extern char *strrchr(const char *s, int c);
extern int _stricmp(const char *s1, const char *s2);
extern char *strstr(const char *haystack, const char *needle);
extern char *_strupr(char *s);
