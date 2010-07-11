#include "elf.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/mman.h>
#include <sys/param.h>
#include <fcntl.h>
#include <stdint.h>
#include <errno.h>
#include <dlfcn.h>
#include <unistd.h>
#include <string.h>
#include <libgen.h>
#include <CoreFoundation/CoreFoundation.h>
#include "common.h"
#include <pthread.h>
#include <mach/mach.h>

uint32_t *stubs;
void *stubs_base;
#define STUBS_SIZE 0x20000
#define STUB_DEBUG 0

// http://simplemachines.it/doc/aaelf.pdf
void *reloc_base;
size_t reloc_size;

void *v2v(Elf32_Addr its) {
    return (void *) (its + reloc_base);
}

void xread(int fd, void *buf, size_t nbyte) {
    errno = 0;
    int ret = read(fd, buf, nbyte);
    if(errno) perror("xread");
    _assert(ret == nbyte);
}

static void add(char *fp) {
    void *handle = dlopen(fp, RTLD_NOW);
    if(!handle) {
        perror(fp);
        _abort();
    }
}

static int zero;

CFMutableDictionaryRef existing_stubs;

void *stubify(void *addy, const char *id, bool needs_mprotect) {
    void *existing = (void *) CFDictionaryGetValue(existing_stubs, addy);
    if(existing) return existing;
    uint32_t p = (uint32_t) addy;
    uint32_t *base;
    if(needs_mprotect) {
        // Multithreading sucks.
        base = mmap(NULL, getpagesize(), PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
        _assert(base != MAP_FAILED);
    } else {
        base = stubs;
    }
    void *ret = base;

    if(id && STUB_DEBUG) {
        // Debug fun!
        char *dstr; asprintf(&dstr, "%s called from %%p\n", id);
        *base++ = 0x43f0e92d;
        *base++ = 0xe89db007;
        *base++ = 0xb08c01f0;
        *base++ = 0x01f0e88d;
        *base++ = 0xf8dfb40f;
        *base++ = 0x48069018;
        *base++ = 0x47c84671;
        *base++ = 0xf8dfbc0f;
        *base++ = 0x47c89014;
        *base++ = 0xe8bdb005;
        *base++ = 0x000083f0;

        *base++ = (uint32_t) printf;
        *base++ = (uint32_t) dstr;
        *base++ = p;
    } else {
        *base++ = 0x43f0e92d;
        *base++ = 0xe89db007;
        *base++ = 0xb08c01f0;
        *base++ = 0x01f0e88d;
        *base++ = 0x9008f8df;
        *base++ = 0xb00547c8;
        *base++ = 0x83f0e8bd;

        *base++ = p;
    }

    if(needs_mprotect) {
        _assertZero(mprotect(ret, getpagesize(), PROT_READ | PROT_EXEC));
    } else {
        stubs = base;
    }

    ret = (char *)ret + 1;
    CFDictionarySetValue(existing_stubs, addy, ret);
    return ret;
}

/*
 * Flash appears to try to fprintf to some FILE* handle in the sandbox
 * which dies in flockfile, but I havn't been able to track down exactly
 * where yet, and extending the sandbox doesn't seem to fix it? wtf.
 * Lets do this lameness for now so youtube doesn't crash and muchmusic works
 */
int hook_fprintf(FILE *stream, const char *format, ...) {
    fprintf(stderr, "DEBUG_HOOK: ");
    va_list v;
    va_start(v, format);
    vfprintf(stderr, format, v);
    va_end(v);

    return 0;
}

static void *getsym(char *id) {
    if(!strcmp(id, "fprintf")) id = "hook_fprintf";
    if(!strcmp(id, "__errno")) id = "__error";
    if(!strcmp(id, "mmap")) id = "rmmap";
    if(!strcmp(id, "mprotect")) id = "rmprotect";
    if(!strcmp(id, "sysconf")) id = "rsysconf";
    if(!strcmp(id, "uname")) id = "runame";
    if(!strcmp(id, "__dso_handle")) return &zero;
    if(!strcmp(id, "statfs")) id = "rstatfs";
    if(!memcmp(id, "pthread", 7)) id[0] = 'r';
    void *real = dlsym(RTLD_DEFAULT, id);
    // Why can't I get thumb-2 out of the damn assembler?
    // oh well
    if(!(((uint32_t) real) & 1)) {
        // Erm, probably data?
        // This is a nasty hack
        if(memcmp(id, "__", 2)) {
            return real;
        }
    }
    return stubify(real, id, false);

}

void logpatched(int x, const char *format, ...) {
    fprintf(stderr, "(logpatched %d) ", x);
    va_list v;
    va_start(v, format);
    vfprintf(stderr, format, v);
    va_end(v);
}

static void do_patches() {
    //*((uint32_t *) 0x0213dfd4) = 0x46c04778;
    //*((uint32_t *) 0x0213dfd8) = 0xe51ff004;
    //*((void **)    0x0213dfdc) = stubify(logpatched, "logpatched", false);
}

static void base_load_elf(int fd, Elf32_Sym **symtab, int *symtab_size, void ***init_array, Elf32_Word *init_array_size, char **strtab) {
    Elf32_Ehdr ehdr;
    xread(fd, &ehdr, sizeof(ehdr));

    _assert(ehdr.e_type == ET_DYN);
    _assert(ehdr.e_machine == EM_ARM);

    _assert(ehdr.e_phentsize >= sizeof(Elf32_Phdr));

    size_t sz = 0;

    lseek(fd, ehdr.e_phoff, SEEK_SET);
    int phnum = ehdr.e_phnum;
    while(phnum--) {
        Elf32_Phdr *ph = (Elf32_Phdr *) malloc(ehdr.e_phentsize);
        xread(fd, ph, ehdr.e_phentsize);
        size_t sz_ = ph->p_vaddr + ph->p_memsz;
        if(sz_ > sz) sz = sz_;
        free(ph);
    }

    sz = (sz + 0xfff) & ~0xfff;

    reloc_base = mmap(NULL, sz, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    reloc_size = sz;
    _assert(reloc_base != MAP_FAILED);

    notice("--> reloc_base = %p", reloc_base);


    lseek(fd, ehdr.e_phoff, SEEK_SET);
    phnum = ehdr.e_phnum;
    while(phnum--) {
        Elf32_Phdr *ph = (Elf32_Phdr *) malloc(ehdr.e_phentsize);
        xread(fd, ph, ehdr.e_phentsize);
        notice("type:%x offset:%x vaddr:%x filesz:%x memsz:%x end:%x align:%x", (int)ph->p_type, ph->p_offset, ph->p_vaddr, ph->p_filesz, ph->p_memsz, ph->p_vaddr + ph->p_memsz, ph->p_align);
        // Cheat
        _assert(ph->p_filesz <= ph->p_memsz);
        if(ph->p_filesz > 0) {
            ssize_t br = pread(fd, v2v(ph->p_vaddr), ph->p_filesz, ph->p_offset);
            _assert(ph->p_filesz == br);
            /**** WTF.
             * If I use mmap for those cases, pread acts very mysteriously.
             * It hangs, but if I ctrl-c and cont in gdb, it then fails.
             * I don't think it ought to?
            if(ph->p_align != 0x1000) {
            } else {
                _assert((ph->p_vaddr & 0xfff) == (ph->p_offset & 0xfff));
                void *ret = mmap(v2v(ph->p_vaddr & ~0xfff), ph->p_filesz, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_FIXED, fd, ph->p_offset & ~0xfff);
                _assert(ret != MAP_FAILED);
            } */
        }
        free(ph);
    }

    _assert(ehdr.e_shentsize >= sizeof(Elf32_Shdr));
    *symtab = NULL;
    *strtab = NULL;
    Elf32_Word shstrtab = 0;

    lseek(fd, ehdr.e_shoff, SEEK_SET);
    int shnum = ehdr.e_shnum;
    while(shnum--) {
        Elf32_Shdr *sh = (Elf32_Shdr *) malloc(ehdr.e_shentsize);
        xread(fd, sh, ehdr.e_shentsize);
        if(sh->sh_type == SHT_DYNSYM) {
            *symtab = v2v(sh->sh_addr);
            *symtab_size = sh->sh_size;
        } else if(sh->sh_type == SHT_STRTAB) {
            if(*strtab) {
                shstrtab = sh->sh_offset;
            } else {
                *strtab = v2v(sh->sh_addr);
            }
        }
        free(sh);
    }

    _assert(*symtab);
    _assert(*strtab);
    _assert(shstrtab);

    *init_array = NULL;

    lseek(fd, ehdr.e_shoff, SEEK_SET);
    shnum = ehdr.e_shnum;
    while(shnum--) {
        Elf32_Shdr *sh = (Elf32_Shdr *) malloc(ehdr.e_shentsize);
        xread(fd, sh, ehdr.e_shentsize);
        _assert(sh->sh_type != SHT_RELA);
        char buf[12];
        pread(fd, buf, 12, shstrtab + sh->sh_name);
        if(!memcmp(buf, ".init_array", 12)) {
            *init_array = v2v(sh->sh_addr);
            *init_array_size = sh->sh_size;
        }
        if(sh->sh_type == SHT_REL) {
            notice("SHT_REL");
            void *base = v2v(sh->sh_addr);
            char *p = (char *) base;
            while(p - (char *) base < sh->sh_size) {
                Elf32_Rel *rel = (Elf32_Rel *) p;
                Elf32_Word *offset = v2v(rel->r_offset);
                Elf32_Word sym = ELF32_R_SYM(rel->r_info);
                Elf32_Word type = ELF32_R_TYPE(rel->r_info);

                if(type == R_ARM_RELATIVE) {
                    // notice("Increasing %x which was %x -> %x", offset, *offset, (*offset + (Elf32_Word) reloc_base));
                    *offset += (Elf32_Word) reloc_base;
                } else {
                    char *name = *strtab + (*symtab)[sym].st_name;
                    void *S = getsym(name);
                    if(!S) {
                        notice("Could not find %s", name);
                    }
                    *offset = (uint32_t) S;
                }

                p += sh->sh_entsize;
            }
        }
        free(sh);
    }

    do_patches();

    lseek(fd, ehdr.e_phoff, SEEK_SET);
    phnum = ehdr.e_phnum;
    while(phnum--) {
        Elf32_Phdr *ph = (Elf32_Phdr *) malloc(ehdr.e_phentsize);
        xread(fd, ph, ehdr.e_phentsize);
        if(ph->p_memsz != 0 && ph->p_align == 0x1000) {
            int prot = 0;
            if(ph->p_flags & PF_R) prot |= PROT_READ;
            if(ph->p_flags & PF_W) prot |= PROT_WRITE;
            if(ph->p_flags & PF_X) prot |= PROT_EXEC;
            int size = ph->p_memsz;
            char *addr = v2v(ph->p_vaddr);
            uint32_t diff = ((uint32_t)addr & 0xfff);
            _assertZero(mprotect(addr - diff, size + diff, prot));
        }
        free(ph);
    }

    // Stubs away
    _assertZero(mprotect(stubs_base, STUBS_SIZE, PROT_READ | PROT_EXEC));
}

extern void fds_init();
extern void go(void *NP_Initialize_ptr, void *JNI_OnLoad_ptr);

extern void GSFontInitialize();

int main(int argc, char **argv) {
    chdir(dirname(argv[0]));
    setvbuf(stdout, NULL, _IONBF, 0);

    char *rpcname = NULL;

    argc--; argv++;
    while(argc--) {
        char *p = *argv++;
        if(!strcmp(p, "-q")) {
            fclose(stdout);
            fclose(stderr);
        } else {
            _assert(p[0] != '-');
            rpcname = p;
        }
    }

    _assert(rpcname);

    GSFontInitialize(); // magic sauce; if this is not called, CTFontCreate* crashes

    rpc_init(rpcname);

    stubs_base = mmap(NULL, STUBS_SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    stubs = stubs_base;

    existing_stubs = CFDictionaryCreateMutable(NULL, 0, NULL, NULL);

    add("/usr/lib/libcrypto.dylib");
    add("/usr/lib/libssl.dylib");
    add("/usr/lib/libm.dylib");
    add("/usr/lib/libSystem.B.dylib");
    add("./libcutils.dylib");
    add("./libutils.dylib");
    add("./libgccstuff.dylib");

    add("./libicudata.42.1.dylib");
    add("./libicui18n.42.1.dylib");
    add("./libicuio.42.1.dylib");
    add("./libicule.42.1.dylib");
    add("./libiculx.42.1.dylib");
    add("./libicutu.42.1.dylib");
    add("./libicuuc.42.1.dylib");


    int fd = open("libflashplayer.so", O_RDONLY);
    _assert(fd > 0);

    fds_init();

    sandbox_me();

    int symtab_size;
    Elf32_Sym *symtab;
    void **init_array;
    Elf32_Word init_array_size;
    char *strtab;
    TIME(base_load_elf(fd, &symtab, &symtab_size, &init_array, &init_array_size, &strtab));

    // Call the init funcs
    _assert(init_array);
    while(init_array_size >= 4) {
        void (*x)() = *init_array++;
        notice("Calling %p", x);
        x();
        init_array_size -= 4;
    }

    void *NP_Initialize_ptr = NULL;
    void *JNI_OnLoad_ptr = NULL;

    int syms = symtab_size / sizeof(Elf32_Sym);
    Elf32_Sym *p = symtab;
    while(syms--) {
        char *name = strtab + p->st_name;
        // notice("%s -> %x", name, p->st_value);
        if(!strcmp(name, "NP_Initialize"))
            NP_Initialize_ptr = v2v(p->st_value);
        if(!strcmp(name, "JNI_OnLoad"))
            JNI_OnLoad_ptr = v2v(p->st_value);
        p++;
    }

    go(NP_Initialize_ptr, JNI_OnLoad_ptr);
    return 0;
}
