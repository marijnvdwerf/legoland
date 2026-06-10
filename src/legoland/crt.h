#pragma once

#include <stdarg.h>

extern int rand(void);
extern void free(void *block);
extern int sprintf(char *buffer, const char *format, ...);
extern void printf(const char *msg);
extern int fclose(void *stream);
extern unsigned int fread(void *ptr, unsigned int size, unsigned int count, void *stream);
extern unsigned int fwrite(const void *ptr, unsigned int size, unsigned int count, void *stream);
extern void *fopen(const char *filename, const char *mode);
extern int fseek(void *stream, long offset, int origin);
extern long ftell(void *stream);

typedef struct _iobuf {
    /* 0x00 */ char *_ptr;
    /* 0x04 */ int _cnt;
    /* 0x08 */ char *_base;
    /* 0x0c */ int _flag;
    /* 0x10 */ int _file;
    /* 0x14 */ int _charbuf;
    /* 0x18 */ int _bufsiz;
    /* 0x1c */ char *_tmpfname;
} FILE;
#define feof(stream)   ((stream)->_flag & 0x10)
#define ferror(stream) ((stream)->_flag & 0x20)
extern unsigned int _read(int fd, void *buffer, unsigned int count);
extern unsigned int _msize(void *block);
extern int vsprintf(char *buffer, const char *format, va_list ap);
extern void *calloc(unsigned int count, unsigned int size);
extern void exit(int status);
extern int atoi(const char *str);
extern unsigned int _lseek(unsigned int a, unsigned int b, unsigned int c);
extern int _tell(int fd);
extern int _write(int fd, void *buffer, unsigned int count);
extern int _rmdir(const char *path);
extern char *_getcwd(char *buffer, int maxlen);
extern int _chdir(const char *dirname);

struct _finddata_t {
    unsigned int attrib;
    long time_create;
    long time_access;
    long time_write;
    unsigned long size;
    char name[260];
};

extern long _findfirst(const char *spec, struct _finddata_t *data);
extern int _findnext(long handle, struct _finddata_t *data);
extern int _findclose(long handle);

extern unsigned int strlen(const char *s);
extern char *strcpy(char *dst, const char *src);
extern char *strcat(char *dst, const char *src);
extern int strcmp(const char *s1, const char *s2);
extern void *memcpy(void *dst, const void *src, unsigned int n);
extern void *memset(void *dst, int c, unsigned int n);
extern unsigned short *wcscpy(unsigned short *dst, const unsigned short *src);

extern void *malloc(unsigned int size);
extern void *realloc(void *block, unsigned int size);
extern char *strncpy(char *dest, const char *src, unsigned int count);
extern char *strrchr(const char *s, int c);
extern int _stricmp(const char *s1, const char *s2);
extern char *strstr(const char *haystack, const char *needle);
extern char *_strupr(char *s);
extern int toupper(int c);
extern int tolower(int c);
extern int abs(int n);
extern int _memicmp(const void *s1, const void *s2, unsigned int n);
extern void _exit(int status);
extern void _splitpath(const char *path, char *drive, char *dir, char *fname, char *ext);
