#ifndef LEGOLAND_H
#define LEGOLAND_H

#include <basetsd.h>

#define STUB() ((void)0)

#if defined(_MSC_VER) && !defined(__clang__)
#define LEGO_EXPORT __declspec(dllexport)
#else
#define LEGO_EXPORT
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif

/* [port] WebAssembly passes a union by value as a pointer to a copy, where x86 passed these two bytes as an unsigned
 * short; callbacks taking a TileId are called through pointers typed with an unsigned short or an unsigned int, which
 * then read the tile from the wrong place. Transparent, a TileId is passed as its first member, as on x86. */
#if defined(LEGOLAND_PORT) && defined(__wasm__)
#define TILEID_PORT_ALIGN __attribute__((aligned(2)))
#define TILEID_PORT_TRANSPARENT __attribute__((transparent_union))
#else
#define TILEID_PORT_ALIGN
#define TILEID_PORT_TRANSPARENT
#endif

typedef union TileId {
    unsigned short id;
    struct TILEID_PORT_ALIGN {
        unsigned char x;
        unsigned char y;
    } pos;
} TILEID_PORT_TRANSPARENT TileId;

/* Windows' ACMSTREAMHEADER (msacm.h): its user fields are pointer-sized and its driver area is 15 dwords on 64-bit
 * Windows; the ACM checks cbStruct against that size. */
#if defined(_WIN64) || defined(__LP64__)
#define ACM_DRV_RESERVED 15
#else
#define ACM_DRV_RESERVED 10
#endif

struct AcmHeader {
    unsigned int cbStruct;
    unsigned int fdwStatus;
    UINT_PTR dwUser;
    unsigned char *pbSrc;
    unsigned int cbSrcLength;
    unsigned int cbSrcLengthUsed;
    UINT_PTR dwSrcUser;
    unsigned char *pbDst;
    unsigned int cbDstLength;
    unsigned int cbDstLengthUsed;
    UINT_PTR dwDstUser;
    unsigned int reserved[ACM_DRV_RESERVED];
};

struct Bloke;

/* What the cursor is over (Hover at 0x004bdd00): a type code (0x100 nothing, 0x103 an object, ...),
   the thing itself and its tile.  Passed by value to PopUpInfoSetUp. */
struct HoverInfo {
    int type;
    struct Bloke *ptr; /* or, for an object, its struct Element */
    union {
        unsigned int value;
        TileId tile;
    } data;
};

typedef int (*ScriptCommandFn)(char **args, int nargs, int flags);
struct ScriptCommand {
    const char *name;
    ScriptCommandFn fn;
};

#endif
