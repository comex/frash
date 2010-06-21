#pragma once
#include <stdint.h>
#include <CoreFoundation/CoreFoundation.h>
#include <servers/bootstrap.h>
typedef struct {
    int oid;
    char str_data[0];
} idp_t;
typedef struct _NPStream *stream_t;

#define IOSFC_BUILDING_IOSFC
#include <IOSurface/IOSurface.h>

#ifndef android_npapi_H
// android_npapi.h requires C++ so here are some defs
#define kSupportedDrawingModel_ANPGetValue  ((NPNVariable)2000)
#define kJavaContext_ANPGetValue            ((NPNVariable)2001)
#define kAcceptEvents_ANPSetValue           ((NPPVariable)1001)
#define kRequestDrawingModel_ANPSetValue    ((NPPVariable)1000)
#define kJavaSurface_ANPGetValue            ((NPPVariable)2000)
enum ANPDrawingModels {
    kBitmap_ANPDrawingModel  = 1 << 0,
    kSurface_ANPDrawingModel = 1 << 1,
};
enum ANPLifecycleActions {
    kPause_ANPLifecycleAction           = 0,
    kResume_ANPLifecycleAction          = 1,
    kGainFocus_ANPLifecycleAction       = 2,
    kLoseFocus_ANPLifecycleAction       = 3,
    kFreeMemory_ANPLifecycleAction      = 4,
    kOnLoad_ANPLifecycleAction          = 5,
    kEnterFullScreen_ANPLifecycleAction = 6,
    kExitFullScreen_ANPLifecycleAction  = 7,
    kOnScreen_ANPLifecycleAction        = 8,
    kOffScreen_ANPLifecycleAction       = 9,
};
typedef uint32_t ANPLifecycleAction;

#endif

#ifdef __cplusplus
extern "C" {
#endif

#include <food_rpc1.h>

extern boolean_t Doof_server(
        mach_msg_header_t *InHeadP,
        mach_msg_header_t *OutHeadP);
extern boolean_t Stuff_server(
        mach_msg_header_t *InHeadP,
        mach_msg_header_t *OutHeadP);



#define padd(a, b) ((void *) ((char *)(a) + b))
void pattern(void *stuff, uint32_t base, int len);

CFMutableDictionaryRef new_dict();

//#define stub(x) stubify((void *) (x), NULL, true)
#define stub(x) stubify((void *) (x), #x, true)

#define s(a, b, c) CFDictionarySetValue(a, CFSTR(b), c)
#define g(a, b) CFDictionaryGetValue(a, CFSTR(b))
typedef CFMutableDictionaryRef dict;

void *RawDataCreate(int size);
void *RawDataGetPtr(const void *raw);
int RawDataGetSize(const void *raw);
void *RawPtrCreate(void *ptr);
void *RawPtrGet(const void *raw);

void *stubify(void *addy, const char *id, bool needs_mprotect);

void logreal_(CFStringRef str);

void do_later(void (*func)(CFRunLoopTimerRef, void *), void *info);

void rpc_init(const char *rpcname);

void sandbox_me();

#ifdef __cplusplus
}
#endif

extern IOSurfaceRef sfc;
extern int food;
extern int movie_w, movie_h, pending_movie_w, pending_movie_h;


#define logreal(fmt, args...)  logreal_(CFStringCreateWithFormat(kCFAllocatorDefault, NULL, CFSTR(fmt), ##args)) 

#define logx(level, args...) (LOG_LEVEL >= (level) ? (logreal(args)) : (void)0)
#define LOG_LEVEL 1
#define warn(args...) logx(2, args)
#define notice(args...) logx(3, args)
#define log(args...) logx(1, args)
#define err(args...) logx(0, args)

