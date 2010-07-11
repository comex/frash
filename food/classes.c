#include "classes.h"
#include "myjni.h"
#include "multistuff.h"
#include <fcntl.h>
#include <unistd.h>
#include <mach/mach.h>

static dict named_dict_(CFStringRef name) {
    dict ret = new_dict();
    s(ret, ".name", name);
    return ret;
}
#define named_dict(name) named_dict_(CFSTR(name))


extern CFMutableDictionaryRef classes;
extern JNIEnv env;

IOSurfaceRef sfc = NULL;

void refresh_size();
static int already_created_surface = 0;


struct method_generic { void *v; void *a; };

void *make_av(void *a, void *v) {
    void *ret = RawDataCreate(sizeof(struct method_generic));
    struct method_generic *entry = RawDataGetPtr(ret);
    entry->a = a ? stub(a) : NULL;
    entry->v = v ? stub(v) : NULL;
    return ret;
}

void *make_a(void *func) {
    return make_av(func, NULL);
}

void *make_v(void *func) {
    return make_av(NULL, func);
}

jobject android_context;

dict new_stringobject(const char *string) {
    dict ret = named_dict("aString");
    s(ret, "theString", CFStringCreateWithCString(NULL, string, kCFStringEncodingUTF8));
    return ret;
}

jobject new_displaymetrics(jclass cls, ...) {
    dict dm = named_dict("aDisplayMetrics");
    return new_jobject(dm);
}

void getMetrics(jobject obj, jvalue *a) {
    dict props = a[0].l;
    s(props, "xdpi[F]", floatToTypeRef(42));
    s(props, "ydpi[F]", floatToTypeRef(42));
}

jobject getDefaultDisplay(jobject obj, jvalue *a) {
    notice("getDefaultDisplay");
    dict dsp = named_dict("aDisplay");
    s(dsp, ".class", CFSTR("android/view/Display"));
    s(dsp, "getMetrics[(Landroid/util/DisplayMetrics;)V]", make_a(getMetrics));
    return new_jobject(dsp);
}

jboolean isScreenOn(jobject obj, jvalue *a) {
    return true;
}

void userActivity(jobject obj, jvalue *a) {
    return;
}

static bool held = false;

void acquireWakeLock(jobject obj, jvalue *a) {
    notice("Acquiring wake lock");
    held = true;
}

void releaseWakeLock(jobject obj, jvalue *a) {
    notice("Releasing wake lock");
    held = false;
}

jboolean isWakeLockHeld(jobject obj, jvalue *a) {
    return held;
}

jobject newWakeLock(jobject obj, jvalue *a) {
    jint type = a[0].i;
    jstring tag = a[1].l;
    _assert(type == 6); // SCREEN_DIM_WAKE_LOCK
    notice("newWakeLock: tag=%@", tag);
    dict lock = named_dict("aWakeLock");
    s(lock, "acquire[()V]", make_a(acquireWakeLock));
    s(lock, "release[()V]", make_a(releaseWakeLock));
    s(lock, "isHeld[()Z]", make_a(isWakeLockHeld));
    s(lock, ".class", CFSTR("android/PowerManager/WakeLock"));
    return new_jobject(lock);
}

jobject getSystemService(jobject obj, jvalue *a) {
    CFStringRef cls = g(a[0].l, "theString");
    notice("getSystemService %@", cls);
    if(CFEqual(cls, CFSTR("CLIPBOARD_SERVICE"))) {
        dict mgr = named_dict("aClipboardManager");
        return new_jobject(mgr);
    } else if(CFEqual(cls, CFSTR("WINDOW_SERVICE"))) {
        dict mgr = named_dict("aWindowManager");
        s(mgr, "getDefaultDisplay[()Landroid/view/Display;]", make_a(getDefaultDisplay));
        s(mgr, ".class", CFSTR("android/view/WindowManager"));
        return new_jobject(mgr);
    } else if(CFEqual(cls, CFSTR("POWER_SERVICE"))) {
        dict mgr = named_dict("aPowerManager");
        s(mgr, "isScreenOn[()Z]", make_a(isScreenOn));
        s(mgr, "userActivity[(JZ)V]", make_a(userActivity));
        s(mgr, "newWakeLock[(ILjava/lang/String;)Landroid/os/PowerManager$WakeLock;]", make_a(newWakeLock));
        s(mgr, ".class", CFSTR("android/view/PowerManager"));
        return new_jobject(mgr);
    }
    _abort();
}

jobject createPackageContext(jobject obj, va_list v) {
    notice("createPackageContext");
    return obj;
}

// SystemCapabilities

jboolean hasTrackBall(jobject obj, jvalue *a) {
    return JNI_FALSE;
}

jint getScreenHRes(jobject obj, jvalue *a) {
    return movie_w;
}

jint getScreenVRes(jobject obj, jvalue *a) {
    return movie_h;
}

jint getBitsPerPixel(jobject obj, jvalue *a) {
    return 32;
}
// RCR

jlong afdGetStartOffset(jobject obj, jvalue *a) {
    return 0;
}

jlong afdGetLength(jobject obj, jvalue *a) {
    int fd = typeRefToInt(g(obj, "fd"));
    off_t end = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    return (jlong) end;
}

// (ctx, string)
void afdClose(jobject obj, jvalue *a) {
    close(typeRefToInt(g(obj, "fd")));
}

jobject afdGetFD(jobject obj, jvalue *a) {
    dict descriptor = named_dict("aFileDescriptor");
    //s(descriptor,
    s(descriptor, "descriptor[I]", CFRetain(g(obj, "fd")));
    return new_jobject(descriptor);
}

static int oemcfg_fd, devnull_fd;

void fds_init() {
    // Do this before we get sandy
    oemcfg_fd = open("oem.cfg", O_RDONLY);
    devnull_fd = open("/dev/null", O_RDONLY);
}

jobject getAssetFileDescriptor(jclass cls, jvalue *a) {
    CFStringRef astName = g(a[1].l, "theString");
    dict descriptor = named_dict("anAssetFileDescriptor");
    s(descriptor, "astName", astName);
    s(descriptor, "close[()V]", make_a(afdClose));
    s(descriptor, "getFileDescriptor[()Ljava/io/FileDescriptor;]", make_a(afdGetFD));
    s(descriptor, "getStartOffset[()J]", make_a(afdGetStartOffset));
    s(descriptor, "getLength[()J]", make_a(afdGetLength));

    int fd;
    notice("opening %@", astName);
    if(CFEqual(astName, CFSTR("oem.cfg"))) {
        fd = oemcfg_fd;
    } else {
        fd = devnull_fd;
    }

    _assert(fd > 0);
    s(descriptor, "fd", intToTypeRef(fd));

    return new_jobject(descriptor);
}

// com.adobe.flashplayer.FlashPaintSurface

static jobject fps_obj;
static NPP fps_npp;

void toggleFullScreen(jobject obj, jvalue *val) {
    jboolean on = val[0].z;
    notice("toggleFullScreen -> %d", (int) on);
    _abort();
}

void getLocationOnScreen(jobject obj, jvalue *val) {
    void *ary = val[0].l;
    _assert(RawDataGetSize(ary) == 2 * sizeof(int));
    int *ary_ = RawDataGetPtr(ary);
    ary_[0] = 0;
    ary_[1] = 0;
}

jobject new_fps(jclass cls, va_list v) {
    log("new_fps");
    jobject ctx = va_arg(v, jobject);
    jint one = va_arg(v, jint);
    jint two = va_arg(v, jint);
    jint three = va_arg(v, jint);

    dict fps = named_dict("aFlashPaintSurface");
    s(fps, "getLocationOnScreen[([I)V]", make_a(getLocationOnScreen));
    s(fps, ".class", CFSTR("com/adobe/flashplayer/FlashPaintSurface"));
    s(fps, "npp", intToTypeRef(one));
    s(fps, "toggleFullScreen[(Z)V]", make_a(toggleFullScreen));
    notice("new_fps cls=%p ctx=%p %lx %ld %ld", cls, ctx, one, two, three);
    do_jni_onload();

    fps_obj = new_jobject(fps);
    fps_npp = (void *) one;
    refresh_size();

    return fps_obj;
}

//

jclass system_impl_loadJavaClass(NPP instance, const char* className);

jobject loadClass(jobject obj, va_list v) {
    CFStringRef className = g(va_arg(v, jstring), "theString");
    char *cs = malloc(CFStringGetLength(className)+1);
    assert(CFStringGetCString(className, cs, CFStringGetLength(className)+1, kCFStringEncodingUTF8));
    jobject ret = system_impl_loadJavaClass(NULL, cs);
    free(cs);
    return ret;
}

jobject getClassLoader(jobject obj, va_list v) {
    dict ldr = named_dict("aClassLoader");
    s(ldr, "loadClass[(Ljava/lang/String;)Ljava/lang/Class;]", make_v(loadClass));
    return new_jobject(ldr);
}

jobject localeToString(jobject obj, jvalue *a) {
    return new_jobject(new_stringobject("en-US"));
}

jobject localeGetDefault(jclass cls, jvalue *a) {
    dict lcl = named_dict("aLocale");
    s(lcl, "toString[()Ljava/lang/String;]", make_a(localeToString));
    return new_jobject(lcl);
}

//

jlong uptimeMillis(jclass cls, jvalue *a) {
    notice("uptimeMillis");
    return 42;
}

jobject new_clock(jclass cls, jvalue *a) {
    dict clk = named_dict("aSystemClock");
    s(clk, ".class", CFSTR("android/os/SystemClock"));
    return new_jobject(clk);
}

void init_classes() {
    classes = new_dict();

    dict displaymetricscls = named_dict("DisplayMetrics");
    s(displaymetricscls, "<init>[()V]", make_av(new_displaymetrics, new_displaymetrics));
    s(classes, "android/util/DisplayMetrics", displaymetricscls);
    dict stringcls = named_dict("String");
    s(classes, "java/lang/String", stringcls);
    dict contextcls = named_dict("Context");
    s(contextcls, "CLIPBOARD_SERVICE[Ljava/lang/String;]", new_stringobject("CLIPBOARD_SERVICE"));
    s(contextcls, "WINDOW_SERVICE[Ljava/lang/String;]", new_stringobject("WINDOW_SERVICE"));
    s(contextcls, "POWER_SERVICE[Ljava/lang/String;]", new_stringobject("POWER_SERVICE"));
    s(classes, "android/content/Context", contextcls);

    dict wmcls = named_dict("WindowManager");
    s(classes, "android/view/WindowManager", wmcls);

    dict pmcls = named_dict("WindowManager");
    s(pmcls, "SCREEN_DIM_WAKE_LOCK[I]", intToTypeRef(6));
    s(pmcls, "SCREEN_BRIGHT_WAKE_LOCK[I]", intToTypeRef(6));
    s(classes, "android/view/PowerManager", pmcls);

    dict wlcls = named_dict("WakeLock");
    s(classes, "android/PowerManager/WakeLock", wlcls);

    dict dspcls = named_dict("Display");
    s(classes, "android/view/Display", dspcls);

    dict context = named_dict("aContext");
    s(context, "getSystemService[(Ljava/lang/String;)Ljava/lang/Object;]", make_a(getSystemService));
    s(context, "createPackageContext[(Ljava/lang/String;I)Landroid/content/Context;]", make_v(createPackageContext));
    s(context, "getClassLoader[()Ljava/lang/ClassLoader;]", make_v(getClassLoader));
    android_context = new_jobject(context);

    dict loader = named_dict("ClassLoader");
    s(classes, "java/lang/ClassLoader", loader);
    dict afd = named_dict("AssetFileDescriptor");
    s(classes, "android/content/res/AssetFileDescriptor", afd);
    dict fd = named_dict("FileDescriptor");
    s(classes, "java/io/FileDescriptor", fd);
    dict locale = named_dict("Locale");
    s(locale, "getDefault[()Ljava/util/Locale;]", make_a(localeGetDefault));
    s(classes, "java/util/Locale", locale);

    dict clock = named_dict("SystemClock");
    s(clock, "<init>[()V]", make_a(new_clock));
    s(clock, "uptimeMillis[()J]", make_a(uptimeMillis));
    s(classes, "android/os/SystemClock", clock);

    dict surface = named_dict("FlashPaintSurface");
    s(surface, "<init>[(Landroid/content/Context;III)V]", make_v(new_fps));
    s(classes, "com/adobe/flashplayer/FlashPaintSurface", surface);

    dict sc = named_dict("SystemCapabilities");
    s(sc, "HasTrackBall[(Landroid/content/Context;)Z]", make_a(hasTrackBall));
    s(sc, "GetScreenHRes[(Landroid/content/Context;)I]", make_a(getScreenHRes));
    s(sc, "GetScreenVRes[(Landroid/content/Context;)I]", make_a(getScreenVRes));
    s(sc, "GetBitsPerPixel[(Landroid/content/Context;)I]", make_a(getBitsPerPixel));
    s(classes, "com/adobe/flashplayer/SystemCapabilities", sc);

    dict rcr = named_dict("RawConfigResources");
    s(rcr, "GetAssetFileDescriptor[(Landroid/content/Context;Ljava/lang/String;)Landroid/content/res/AssetFileDescriptor;]", make_a(getAssetFileDescriptor));
    s(classes, "com/adobe/flashplayer/RawConfigResources", rcr);

}

static IOSurfaceRef make_iosurface() {
    CFMutableDictionaryRef dict;
    int pitch = movie_w * 4;
    int allocSize = pitch * movie_h + 128;
    int bytesPerElement = 4;
    char pixelFormat[4] = {'A', 'R', 'G', 'B'};

    dict = CFDictionaryCreateMutable(kCFAllocatorDefault, 0,
     &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
    CFDictionarySetValue(dict, kIOSurfaceIsGlobal, kCFBooleanTrue);
    CFDictionarySetValue(dict, kIOSurfaceBytesPerRow,
    CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &pitch));
    CFDictionarySetValue(dict, kIOSurfaceBytesPerElement,
    CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &bytesPerElement));
    CFDictionarySetValue(dict, kIOSurfaceWidth,
    CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &movie_w));
    CFDictionarySetValue(dict, kIOSurfaceHeight,
    CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &movie_h));
    CFDictionarySetValue(dict, kIOSurfacePixelFormat,
    CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, pixelFormat));
    CFDictionarySetValue(dict, kIOSurfaceAllocSize,
    CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &allocSize));
    CFDictionarySetValue(dict, kIOSurfaceIsGlobal, kCFBooleanTrue);

    IOSurfaceRef ret = IOSurfaceCreate(dict);
    _assert(ret);
    return ret;
}


void do_jni_surface_created(NPP npp_, jobject j) {
    log("do_jni_surface_created");
    int npp = typeRefToInt(g(j, "npp"));
    dict cls = (dict) g(classes, "com/adobe/flashplayer/FlashPaintSurface");
    assert(cls);
    //jboolean (*nioge)(JNIEnv *, jobject, jint) = RawPtrGet(g(cls, "nativeIsOpenGLEnabled[(I)Z]"));
    //jboolean (*nias)(JNIEnv *, jobject, jint) = RawPtrGet(g(cls, "nativeIsARGBSurface[(I)Z]"));
    void (*nsc)(JNIEnv *, jobject, jint) = RawPtrGet(g(cls, "nativeSurfaceCreated[(I)V]"));
    void (*segl)(JNIEnv *, jobject, jobject) = RawPtrGet(g(cls, "nativeSetEGL[(ILcom/adobe/flashplayer/FlashEGL;)V]"));

    nsc(&env, j, npp);
    dict egl = named_dict("aFlashEGL");
    segl(&env, j, new_jobject(egl));

    already_created_surface = 1;
}


void do_jni_surface_changed(NPP npp, jobject j) {
    if(pending_movie_w < 2 || pending_movie_h < 2) return;
    movie_w = pending_movie_w;
    movie_h = pending_movie_h;
    log("do_jni_surface_changed");
    if(!already_created_surface) do_jni_surface_created(npp, j);
    dict cls = (dict) g(classes, "com/adobe/flashplayer/FlashPaintSurface");
    void (*nsch)(JNIEnv *, jobject, jint, jint, jint, jint) = RawPtrGet(g(cls, "nativeSurfaceChanged[(IIII)V]"));
    nsch(&env, j, (int) npp, 1 /* rgba 888; 565 => 2; see FPS$2.ddx */, movie_w, movie_h);
    notice("ok I called nsch %p", nsch);

    sfc_dirty = true;
    if(sfc) CFRelease(sfc);
    sfc = make_iosurface();
}

jclass system_impl_loadJavaClass(NPP instance, const char* className) {
    notice("loadJavaClass: %s", className);

    char *cn = strdup(className), *p = cn;
    while(1) {
        if(!*p) break;
        if(*p == '.') *p = '/';
        p++;
    }
    CFStringRef str = CFStringCreateWithCStringNoCopy(NULL, cn, kCFStringEncodingUTF8, kCFAllocatorNull);
    dict ret = (void *) CFDictionaryGetValue(classes, str);
    CFRelease(str);
    free(cn);
    _assert(ret);
    return new_jobject(ret);
}

//

void refresh_size() {
    log("refresh_size");
    do_jni_surface_changed(fps_npp, fps_obj);
}

int set_movie_size(int rpcfd, int w, int h) {
    if(pending_movie_w == w && pending_movie_h == h) return 0;
    log("set_movie_size: %dx%d", w, h);
    pending_movie_w = w;
    pending_movie_h = h;
    if((fps_npp && !already_created_surface) || !locked) refresh_size();
    return 0;
}
