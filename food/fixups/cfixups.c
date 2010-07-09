#include <stdint.h>
#include <libkern/OSAtomic.h>
#include <libkern/OSCacheControl.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/mman.h>
#include <string.h>
#include <stdlib.h>
#include <sys/utsname.h>
#include <sys/param.h>
#include <sys/mount.h>
#include <assert.h>
#include <unistd.h>

#define _U  0x01
#define _L  0x02
#define _N  0x04
#define _S  0x08
#define _P  0x10
#define _C  0x20
#define _X  0x40
#define _B  0x80

int32_t __atomic_inc(volatile int32_t* addr) {
    if(!addr) return 0;
    return OSAtomicIncrement32Barrier((int32_t*)addr)-1;
}

int32_t __atomic_dec(volatile int32_t* addr) {
    if(!addr) return 0;
    return OSAtomicDecrement32Barrier((int32_t*)addr)+1;
}



/*NPNetscapeFuncs
NPPluginFuncs*/

int prctl(int option, unsigned long arg2, unsigned long arg3 , unsigned long arg4, unsigned long arg5) {
    //fprintf(stderr, "prctl called: option=%x\n", option);
    switch(option) {
    case 15: {
        char buf[16];
        strncpy(buf, (void *) arg2, 16);
        //fprintf(stderr, "PR_SET_NAME %s\n", buf);
        return 0; }
    default:
        fprintf(stderr, "Unknown prctl option %d!\n", option);
    };
    return -1;
}

int cacheflush(char *start, char *end, int flags) {
    //fprintf(stderr, "cacheflush %p %p %d\n", start, end, flags);
    sys_dcache_flush(start, (size_t) (end - start));
    sys_icache_invalidate(start, (size_t) (end - start));
    return 0;
}

int __cxa_atexit(void (*)(void *), void *, void *);

int __aeabi_atexit (void *object, void (*destructor) (void *), void *dso_handle)
{
    return __cxa_atexit(destructor, object, dso_handle);
}

#define CTYPE_NUM_CHARS       256


const short _C_toupper_[1 + CTYPE_NUM_CHARS] = {
    EOF,
    0x00,   0x01,   0x02,   0x03,   0x04,   0x05,   0x06,   0x07,
    0x08,   0x09,   0x0a,   0x0b,   0x0c,   0x0d,   0x0e,   0x0f,
    0x10,   0x11,   0x12,   0x13,   0x14,   0x15,   0x16,   0x17,
    0x18,   0x19,   0x1a,   0x1b,   0x1c,   0x1d,   0x1e,   0x1f,
    0x20,   0x21,   0x22,   0x23,   0x24,   0x25,   0x26,   0x27,
    0x28,   0x29,   0x2a,   0x2b,   0x2c,   0x2d,   0x2e,   0x2f,
    0x30,   0x31,   0x32,   0x33,   0x34,   0x35,   0x36,   0x37,
    0x38,   0x39,   0x3a,   0x3b,   0x3c,   0x3d,   0x3e,   0x3f,
    0x40,   0x41,   0x42,   0x43,   0x44,   0x45,   0x46,   0x47,
    0x48,   0x49,   0x4a,   0x4b,   0x4c,   0x4d,   0x4e,   0x4f,
    0x50,   0x51,   0x52,   0x53,   0x54,   0x55,   0x56,   0x57,
    0x58,   0x59,   0x5a,   0x5b,   0x5c,   0x5d,   0x5e,   0x5f,
    0x60,   'A',    'B',    'C',    'D',    'E',    'F',    'G',
    'H',    'I',    'J',    'K',    'L',    'M',    'N',    'O',
    'P',    'Q',    'R',    'S',    'T',    'U',    'V',    'W',
    'X',    'Y',    'Z',    0x7b,   0x7c,   0x7d,   0x7e,   0x7f,
    0x80,   0x81,   0x82,   0x83,   0x84,   0x85,   0x86,   0x87,
    0x88,   0x89,   0x8a,   0x8b,   0x8c,   0x8d,   0x8e,   0x8f,
    0x90,   0x91,   0x92,   0x93,   0x94,   0x95,   0x96,   0x97,
    0x98,   0x99,   0x9a,   0x9b,   0x9c,   0x9d,   0x9e,   0x9f,
    0xa0,   0xa1,   0xa2,   0xa3,   0xa4,   0xa5,   0xa6,   0xa7,
    0xa8,   0xa9,   0xaa,   0xab,   0xac,   0xad,   0xae,   0xaf,
    0xb0,   0xb1,   0xb2,   0xb3,   0xb4,   0xb5,   0xb6,   0xb7,
    0xb8,   0xb9,   0xba,   0xbb,   0xbc,   0xbd,   0xbe,   0xbf,
    0xc0,   0xc1,   0xc2,   0xc3,   0xc4,   0xc5,   0xc6,   0xc7,
    0xc8,   0xc9,   0xca,   0xcb,   0xcc,   0xcd,   0xce,   0xcf,
    0xd0,   0xd1,   0xd2,   0xd3,   0xd4,   0xd5,   0xd6,   0xd7,
    0xd8,   0xd9,   0xda,   0xdb,   0xdc,   0xdd,   0xde,   0xdf,
    0xe0,   0xe1,   0xe2,   0xe3,   0xe4,   0xe5,   0xe6,   0xe7,
    0xe8,   0xe9,   0xea,   0xeb,   0xec,   0xed,   0xee,   0xef,
    0xf0,   0xf1,   0xf2,   0xf3,   0xf4,   0xf5,   0xf6,   0xf7,
    0xf8,   0xf9,   0xfa,   0xfb,   0xfc,   0xfd,   0xfe,   0xff
};

const short *_toupper_tab_ = _C_toupper_;

const short _C_tolower_[1 + CTYPE_NUM_CHARS] = {
    EOF,
    0x00,   0x01,   0x02,   0x03,   0x04,   0x05,   0x06,   0x07,
    0x08,   0x09,   0x0a,   0x0b,   0x0c,   0x0d,   0x0e,   0x0f,
    0x10,   0x11,   0x12,   0x13,   0x14,   0x15,   0x16,   0x17,
    0x18,   0x19,   0x1a,   0x1b,   0x1c,   0x1d,   0x1e,   0x1f,
    0x20,   0x21,   0x22,   0x23,   0x24,   0x25,   0x26,   0x27,
    0x28,   0x29,   0x2a,   0x2b,   0x2c,   0x2d,   0x2e,   0x2f,
    0x30,   0x31,   0x32,   0x33,   0x34,   0x35,   0x36,   0x37,
    0x38,   0x39,   0x3a,   0x3b,   0x3c,   0x3d,   0x3e,   0x3f,
    0x40,   'a',    'b',    'c',    'd',    'e',    'f',    'g',
    'h',    'i',    'j',    'k',    'l',    'm',    'n',    'o',
    'p',    'q',    'r',    's',    't',    'u',    'v',    'w',
    'x',    'y',    'z',    0x5b,   0x5c,   0x5d,   0x5e,   0x5f,
    0x60,   0x61,   0x62,   0x63,   0x64,   0x65,   0x66,   0x67,
    0x68,   0x69,   0x6a,   0x6b,   0x6c,   0x6d,   0x6e,   0x6f,
    0x70,   0x71,   0x72,   0x73,   0x74,   0x75,   0x76,   0x77,
    0x78,   0x79,   0x7a,   0x7b,   0x7c,   0x7d,   0x7e,   0x7f,
    0x80,   0x81,   0x82,   0x83,   0x84,   0x85,   0x86,   0x87,
    0x88,   0x89,   0x8a,   0x8b,   0x8c,   0x8d,   0x8e,   0x8f,
    0x90,   0x91,   0x92,   0x93,   0x94,   0x95,   0x96,   0x97,
    0x98,   0x99,   0x9a,   0x9b,   0x9c,   0x9d,   0x9e,   0x9f,
    0xa0,   0xa1,   0xa2,   0xa3,   0xa4,   0xa5,   0xa6,   0xa7,
    0xa8,   0xa9,   0xaa,   0xab,   0xac,   0xad,   0xae,   0xaf,
    0xb0,   0xb1,   0xb2,   0xb3,   0xb4,   0xb5,   0xb6,   0xb7,
    0xb8,   0xb9,   0xba,   0xbb,   0xbc,   0xbd,   0xbe,   0xbf,
    0xc0,   0xc1,   0xc2,   0xc3,   0xc4,   0xc5,   0xc6,   0xc7,
    0xc8,   0xc9,   0xca,   0xcb,   0xcc,   0xcd,   0xce,   0xcf,
    0xd0,   0xd1,   0xd2,   0xd3,   0xd4,   0xd5,   0xd6,   0xd7,
    0xd8,   0xd9,   0xda,   0xdb,   0xdc,   0xdd,   0xde,   0xdf,
    0xe0,   0xe1,   0xe2,   0xe3,   0xe4,   0xe5,   0xe6,   0xe7,
    0xe8,   0xe9,   0xea,   0xeb,   0xec,   0xed,   0xee,   0xef,
    0xf0,   0xf1,   0xf2,   0xf3,   0xf4,   0xf5,   0xf6,   0xf7,
    0xf8,   0xf9,   0xfa,   0xfb,   0xfc,   0xfd,   0xfe,   0xff
};

const short *_tolower_tab_ = _C_tolower_;

const char _C_ctype_[1 + CTYPE_NUM_CHARS] = {
    0,
    _C, _C, _C, _C, _C, _C, _C, _C,
    _C, _C|_S,  _C|_S,  _C|_S,  _C|_S,  _C|_S,  _C, _C,
    _C, _C, _C, _C, _C, _C, _C, _C,
    _C, _C, _C, _C, _C, _C, _C, _C,
   _S|(char)_B, _P, _P, _P, _P, _P, _P, _P,
    _P, _P, _P, _P, _P, _P, _P, _P,
    _N, _N, _N, _N, _N, _N, _N, _N,
    _N, _N, _P, _P, _P, _P, _P, _P,
    _P, _U|_X,  _U|_X,  _U|_X,  _U|_X,  _U|_X,  _U|_X,  _U,
    _U, _U, _U, _U, _U, _U, _U, _U,
    _U, _U, _U, _U, _U, _U, _U, _U,
    _U, _U, _U, _P, _P, _P, _P, _P,
    _P, _L|_X,  _L|_X,  _L|_X,  _L|_X,  _L|_X,  _L|_X,  _L,
    _L, _L, _L, _L, _L, _L, _L, _L,
    _L, _L, _L, _L, _L, _L, _L, _L,
    /* determine printability based on the IS0 8859 8-bit standard */
    _L, _L, _L, _P, _P, _P, _P, _C,

    _C, _C, _C, _C, _C, _C, _C, _C, /* 80 */
    _C, _C, _C, _C, _C, _C, _C, _C, /* 88 */
    _C, _C, _C, _C, _C, _C, _C, _C, /* 90 */
    _C, _C, _C, _C, _C, _C, _C, _C, /* 98 */
    _P, _P, _P, _P, _P, _P, _P, _P, /* A0 */
    _P, _P, _P, _P, _P, _P, _P, _P, /* A8 */
    _P, _P, _P, _P, _P, _P, _P, _P, /* B0 */
    _P, _P, _P, _P, _P, _P, _P, _P, /* B8 */
    _P, _P, _P, _P, _P, _P, _P, _P, /* C0 */
    _P, _P, _P, _P, _P, _P, _P, _P, /* C8 */
    _P, _P, _P, _P, _P, _P, _P, _P, /* D0 */
    _P, _P, _P, _P, _P, _P, _P, _P, /* D8 */
    _P, _P, _P, _P, _P, _P, _P, _P, /* E0 */
    _P, _P, _P, _P, _P, _P, _P, _P, /* E8 */
    _P, _P, _P, _P, _P, _P, _P, _P, /* F0 */
    _P, _P, _P, _P, _P, _P, _P, _P  /* F8 */
};

const char *_ctype_ = _C_ctype_;

void *rmmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset) {
    // Linux and OSX use different values for MAP_ANON(YMOUS)
    if(flags & 0x1000) {
        //fprintf(stderr, "MAP_EXECUTAbLE\n");
        flags &= ~0x1000;
    }
    if(flags & 0x20) {
        //fprintf(stderr, "MAP_RENAME\n");
        flags &= ~0x20;
        flags |= MAP_ANON;
    }
    void *ret = mmap(addr, len, prot, flags, fd, offset);
    //fprintf(stderr, "addr:%p len:%d prot:%d flags:%x fd:%d offset:%x\n", addr, (int) len, prot, flags, fd, (int) offset);
    //fprintf(stderr, "mmap ret: %p\n", ret);
    return ret;
}

int rmprotect(void *addr, size_t len, int prot) {
    //fprintf(stderr, "mprotect %p (%d): %x\n", addr, (int) len, prot);
    return mprotect(addr, len, prot);
}

long rsysconf(int name) {
    fprintf(stderr, "rsysconf: %d\n", name);
    switch(name) {
    case 0x27:
        return getpagesize();
        break;
    default:
        fprintf(stderr, "Unknown!\n");
        abort();
    }
}

#define SYS_NMLN 65

struct bionic_utsname {
    char  sysname   [SYS_NMLN];
    char  nodename  [SYS_NMLN];
    char  release   [SYS_NMLN];
    char  version   [SYS_NMLN];
    char  machine   [SYS_NMLN];
    char  domainname[SYS_NMLN];
};

int runame(struct bionic_utsname *name) {
    struct utsname u;
    int ret;
    if((ret = uname(&u))) return ret;
    strlcpy(name->sysname, u.sysname, SYS_NMLN);
    strlcpy(name->nodename, u.nodename, SYS_NMLN);
    strlcpy(name->release, u.release, SYS_NMLN);
    strlcpy(name->version, u.version, SYS_NMLN);
    strlcpy(name->machine, u.machine, SYS_NMLN);
    strlcpy(name->domainname, "foo", SYS_NMLN);
    return ret;
}

struct bionic_statfs {
    uint32_t f_type;
    uint32_t f_bsize;
    uint32_t f_blocks;
    uint32_t f_bfree;
    uint32_t f_bavail;
    uint32_t f_files;
    uint32_t f_ffree;
    fsid_t f_fsid;
    uint32_t f_namelen;
    uint32_t f_frsize;
    uint32_t f_spare[5];
};



int rstatfs(const char *path, struct bionic_statfs *buf) {
    struct statfs sfs;
    int ret = statfs(path, &sfs);
    memset(buf, 0, sizeof(*buf));
    buf->f_type = sfs.f_type;
    buf->f_bsize = sfs.f_bsize;
    buf->f_blocks = sfs.f_blocks;
    buf->f_bfree = sfs.f_bfree;
    buf->f_bavail = sfs.f_bavail;
    buf->f_files = sfs.f_files;
    buf->f_ffree = sfs.f_ffree;
    buf->f_fsid = sfs.f_fsid;
    buf->f_namelen = MNAMELEN;
    buf->f_frsize = 0;
    return ret;
}
