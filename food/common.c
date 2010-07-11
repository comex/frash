#include <common.h>
#include <pthread.h>
#include <mach/mach.h>
#include <sandbox.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <spawn.h>

int food;

int movie_w = 0, movie_h = -1;
int pending_movie_w = 0, pending_movie_h = -1;

void pattern(void *stuff, uint32_t base, int len) {
    uint32_t *p = stuff;
    len /= 4;
    while(len--) {
        *p++ = base;
        base += 4;
    }
}

static const unsigned int raw_data_isa = 0xdddddddd;
static const unsigned int raw_ptr_isa = 0xbbbbbbbb;
struct raw_data {
    unsigned int isa;
    int retaincount;
    int size;
    char data[];
};

struct raw_ptr {
    unsigned int isa;
    int retaincount;
    void *ptr;
};

void *RawDataCreate(int size) {
    struct raw_data *ret = malloc(sizeof(struct raw_data) + size);
    ret->isa = raw_data_isa;
    ret->retaincount = 0; // lol
    ret->size = size;
    memset(ret->data, 0, size);
    return ret;
}

void *RawDataGetPtr(const void *value) {
    // Haha I suck :<
    struct raw_data *raw = (void *) value;
    if(raw->isa != raw_data_isa) return NULL;
    return raw->data;
}

int RawDataGetSize(const void *value) {
    struct raw_data *raw = (void *) value;
    if(raw->isa != raw_data_isa) return 0;
    return raw->size;
}

void *RawPtrCreate(void *ptr) {
    struct raw_ptr *ret = malloc(sizeof(struct raw_ptr));
    ret->isa = raw_ptr_isa;
    ret->retaincount = 1;
    ret->ptr = ptr;
    return ret;
}

void *RawPtrGet(const void *value) {
    struct raw_ptr *raw = (void *) value;
    if(raw->isa != raw_ptr_isa) return NULL;
    return raw->ptr;
}

static const void *myRetain(CFAllocatorRef allocator, const void *value) {
    struct raw_data *raw = (void *) value;
    if(raw->isa == raw_data_isa || raw->isa == raw_ptr_isa) {
       raw->retaincount++;
    } else {
        CFRetain(value);
    }
    return value;
}

static void myRelease(CFAllocatorRef allocator, const void *value) {
    struct raw_data *raw = (void *) value;
    if(raw->isa == raw_data_isa || raw->isa == raw_ptr_isa) {
       if(--raw->retaincount == 0) free(raw);
    } else {
        CFRelease(value);
    }
}

CFStringRef myCopyDescription(const void *value) {
    const struct raw_data *raw_data = value;
    const struct raw_ptr *raw_ptr = value;
    if(raw_data->isa == raw_data_isa) {
        return CFStringCreateWithFormat(NULL, NULL, CFSTR("<raw data %d>"), raw_data->size);
    } else if(raw_data->isa == raw_ptr_isa) {
        return CFStringCreateWithFormat(NULL, NULL, CFSTR("<raw ptr %p>"), raw_ptr->ptr);
    } else {
        return CFCopyDescription(value);
    }
}

Boolean myEqual(const void *value1, const void *value2) {
    const struct raw_data *raw1 = value1, *raw2 = value2;
    if(raw1->isa != raw_data_isa && raw2->isa != raw_data_isa &&
       raw1->isa != raw_ptr_isa  && raw2->isa != raw_ptr_isa)
       return CFEqual(value1, value2);
    return value1 == value2;
}


CFDictionaryValueCallBacks myValueCallBacks = {
    0,
    myRetain,
    myRelease,
    myCopyDescription,
    myEqual
};

CFMutableDictionaryRef new_dict() {
    return CFDictionaryCreateMutable(NULL, 0, &kCFTypeDictionaryKeyCallBacks, &myValueCallBacks);
}

void logreal_(CFStringRef string) {
    CFIndex len = CFStringGetLength(string) + 1;
    char *buf = malloc(len);
    CFStringGetCString(string, buf, len, kCFStringEncodingASCII);
    CFRelease(string);
    write(STDERR_FILENO, buf, len);
    write(STDERR_FILENO, "\n", 1);
    free(buf);
}

void do_later(void (*func)(CFRunLoopTimerRef, void *), void *info) {
    CFRunLoopTimerContext ctx;
    memset(&ctx, 0, sizeof(ctx));
    ctx.info = info;
    CFRunLoopTimerRef ref = CFRunLoopTimerCreate(NULL,  CFAbsoluteTimeGetCurrent(), 0.0, 0, 0, func, &ctx);
    CFRunLoopAddTimer(CFRunLoopGetMain(), ref, kCFRunLoopCommonModes);
}


static void error(int food_, int err) {
    if(err < 0) {
        fprintf(stderr, "Socket error: %s\n", strerror(-err));
    } else {
        fprintf(stderr, "Internal error: %d\n", err);
    }
    _abort();
}

void rpc_init(const char *rpcname) {
    if(!strcmp(rpcname, "inetd")) {
        food = 3;
      } else {
        int serv = socket(AF_INET, SOCK_STREAM, 0);
        _assert(serv > 0);
        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = htonl(0x7f000001);
        addr.sin_port = htons(atoi(rpcname));

        int tru = 1;
        setsockopt(serv, SOL_SOCKET, SO_REUSEADDR, &tru, sizeof(tru));
        setsockopt(serv, SOL_SOCKET, SO_REUSEPORT, &tru, sizeof(tru));
        _assertZero(bind(serv, (struct sockaddr *) &addr, sizeof(addr)));
        _assertZero(listen(serv, 10));

        char buf[200];
        socklen_t buflen = 200;
        food = accept(serv, (struct sockaddr *) buf, &buflen);
        _assert(food > 0);
    }

    rpcserve(food, error);
    log("RPC hi.");
}

void sandbox_me() {
    char *err;
    int fd = open("food.sb", O_RDONLY);
    _assert(fd > 0);
    size_t len = lseek(fd, 0, SEEK_END);
    char *sandbox = malloc(len + 1);
    pread(fd, sandbox, len, 0);
    sandbox[len] = 0;
    close(fd);

    if(0 != sandbox_init(sandbox, 0, &err)) {
        fprintf(stderr, "Couldn't sandbox: %s\n", err);
        _abort();
    }

    free(sandbox);
}

__attribute__((noreturn))
static void do_abort(char *message) {
    err("%s", message);
    abort_msg(food, message, strlen(message)); // may or may not succeed
    exit(1);
}

void _assert_(bool test, const char *label, const char *file, int line, const char *func) {
    if(!test) {
        char *x;
        asprintf(&x, "Assertion failed: (%s), function %s, file %s, line %d.", label, func, file, line);
        do_abort(x);
    }
}

void _assertZero_(int test, const char *label, const char *file, int line, const char *func) {
    if(test != 0) {
        char *x;
        asprintf(&x, "Assertion failed: !(%s) [it was %d / 0x%x], function %s, file %s, line %d.", label, test, test, func, file, line);
        do_abort(x);
    }
}

void _abort_(const char *file, int line) {
    char *x;
    asprintf(&x, "_abort() %s:%d", file, line);
    do_abort(x);
}

void _abortWithError_(const char *file, int line, const char *error) {
    char *x;
    asprintf(&x, "%s %s:%d", error, file, line);
    do_abort(x);
}

#if TIMING
uint64_t getus() {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return ((uint64_t) tp.tv_sec) * 1000000 + (uint64_t) tp.tv_usec;
}
#endif

bool locked, sfc_dirty;
