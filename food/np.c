#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#undef XP_MACOSX
#undef XP_MAC
#define ANDROID_PLUGINS
#include "common.h"
#include "npfunctions.h"
#include "myjni.h"
#include <CoreFoundation/CoreFoundation.h>
#include "npapi.h"
#include <mach/mach.h>

extern void post_draw_event();
extern void post_lifecycle_event(ANPLifecycleAction);
extern void post_key_event(bool, int32_t, int32_t, uint32_t, int32_t, int32_t);
extern void post_touch_event(int32_t action, uint32_t modifiers, int32_t x, int32_t y);
extern void post_custom_event(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f, int32_t g, int32_t h);
extern void post_mouse_event(int32_t action, int32_t x, int32_t y);

static NPPluginFuncs pluginfuncs;
static NPNetscapeFuncs funcs;
NPP_t nppt;
static NPObject *scriptable;

static NPWindow window;

// NPIdentifier stuff
static CFMutableSetRef identifier_set;
__attribute__((constructor))
void init_identifier_dict() {
    identifier_set = CFSetCreateMutable(NULL, 0, &kCFTypeSetCallBacks);
}

struct NPFuckedUpVariant {
    NPVariantType type;
    union {
        struct {
            int dunnoWhatIsUpWithPaddingRules;
            NPString value;
        } string;
        struct {
            NPObject *whoknows1;
            NPObject *whoknows2;
            NPObject *whoknows3;
        } object;
        struct {
            int32_t whoknows1;
            int32_t whoknows2;
            int32_t whoknows3;
        } int_;
    };
};

struct IDProxyObject {
    NPObject obj;
    int id;
};


static NPClass idproxy_class;

NPObject *new_idproxy_object(int id) {
    struct IDProxyObject *ret = malloc(sizeof(struct IDProxyObject));
    ret->obj._class = &idproxy_class;
    ret->obj.referenceCount = 1;
    ret->id = id;
    return (NPObject *) ret;
}

static struct NPFuckedUpVariant variant_for_object_name(int name) {
    bool valid;
    void *value; size_t value_len;
    _assertZero(get_string_value(food, name, &valid, &value, &value_len));
    struct NPFuckedUpVariant ret;
    log("variant_for_object_name(%d): valid = %d", name, (int) valid);
    if(valid) {
        ret.type = NPVariantType_String;
        ret.string.value = (NPString) {value, value_len};
    } else {
        ret.type = NPVariantType_Object;
        ret.object.whoknows1 = \
        ret.object.whoknows2 = \
        ret.object.whoknows3 = new_idproxy_object(name);
    }
    return ret;
}


NPIdentifier NPN_GetStringIdentifier(const NPUTF8 *name) {
    //notice("psst: GetStringIdentifier '%s'", name);
    CFDataRef data = CFDataCreate(NULL, (unsigned char *) name, strlen(name));
    CFDataRef ret = CFSetGetValue(identifier_set, data);
    if(!ret) {
        CFSetAddValue(identifier_set, data);
        ret = data;
    }
    CFRelease(data);
    return (NPIdentifier) ret;
}

NPIdentifier NPN_GetIntIdentifier(int32_t intid) {
    notice("Get*INT*Identifier: %d", intid);
    CFNumberRef number = CFNumberCreate(NULL, kCFNumberIntType, &intid);
    CFNumberRef ret = CFSetGetValue(identifier_set, number);
    if(!ret) {
        CFSetAddValue(identifier_set, number);
        ret = number;
    }
    CFRelease(number);
    return (NPIdentifier) ret;
}

bool NPN_IdentifierIsString(NPIdentifier identifier) {
    return CFGetTypeID(identifier) == CFDataGetTypeID();
}

NPUTF8 *NPN_UTF8FromIdentifier(NPIdentifier identifier) {
    unsigned char *ret = malloc(CFDataGetLength(identifier) + 1);
    CFDataGetBytes(identifier, CFRangeMake(0, CFDataGetLength(identifier)), ret);
    ret[CFDataGetLength(identifier)] = 0;
    return (NPUTF8 *) ret;
}

int32_t NPN_IntFromIdentifier(NPIdentifier identifier) {
    int ret;
    _assert(CFNumberGetValue(identifier, kCFNumberIntType, &ret));
    return ret;
}

bool NPN_GetProperty(NPP npp, NPObject *npobj, NPIdentifier propertyName,
                     NPVariant *result) {
    notice("NPN_GetProperty %s", CFDataGetBytePtr(propertyName));
    return npobj->_class->getProperty(npobj, propertyName, result);
}

bool NPN_Invoke(NPP npp, NPObject *npobj, NPIdentifier methodName,
                const NPVariant *args, uint32_t argCount, NPVariant *result) {
    notice("NPN_Invoke %s", CFDataGetBytePtr(methodName));
    return npobj->_class->invoke(npobj, methodName, args, argCount, result);
}

void NPN_ReleaseVariantValue(NPVariant *variant) {

}

NPObject *idproxyAllocate(NPP npp, NPClass *aClass) {
    err("! fakeAllocate");
    _abort();
}
void idproxyDeallocate(NPObject *obj) {
    free(obj);
}
void idproxyInvalidate(NPObject *obj) {
    err("! fakeInvalidate");
    _abort();
}
bool idproxyHasMethod(NPObject *obj, NPIdentifier name) {
    err("! fakeHasMethod: %@", name);
    _abort();
}
bool idproxyInvoke(NPObject *obj, NPIdentifier name, const NPVariant *args, uint32_t argCount, NPVariant *result) {
    err("! fakeInvoke: %@", name);
    struct IDProxyObject *obj_ = (void *) obj;
    int prop;
    int *args_ = malloc(argCount * sizeof(int));
    int i; for(i = 0; i < argCount; i++) {
        const struct NPFuckedUpVariant *arg = &(((const struct NPFuckedUpVariant *) args)[i]);
        int arg_;
        switch(arg->type) {
        case NPVariantType_String:
            assert(!get_string_object(food, (void *) arg->string.value.UTF8Characters, arg->string.value.UTF8Length, &arg_));
            break;
        case NPVariantType_Int32:
        case NPVariantType_Object:
        default:
            log("called with unhandled argument type");
            abort();
        }
    }
    _assertZero(invoke_object_property(food, obj_->id, (void *) CFDataGetBytePtr(name), CFDataGetLength(name), args_, argCount * sizeof(int), &prop));
    free(args_);
    if(prop) {
        *((struct NPFuckedUpVariant *) result) = variant_for_object_name(prop);
        return true;
    } else {
        return false;
    }
}
bool idproxyInvokeDefault(NPObject *npobj, const NPVariant *args, uint32_t argCount, NPVariant *result) {
    err("! fakeInvokeDefault");
    _abort();
}
bool idproxyHasProperty(NPObject *obj, NPIdentifier name) {
    err("! idproxyHasProperty: %@", name);
    _abort();
}
bool idproxyGetProperty(NPObject *obj, NPIdentifier name, NPVariant *result) {
    int prop;
    struct IDProxyObject *obj_ = (void *) obj;
    _assertZero(get_object_property(food, obj_->id, (void *) CFDataGetBytePtr(name), CFDataGetLength(name), &prop));
    if(prop) {
        *((struct NPFuckedUpVariant *) result) = variant_for_object_name(prop);
        return true;
    } else {
        return false;
    }
}
bool idproxySetProperty(NPObject *obj, NPIdentifier name, const NPVariant *value) {
    err("! idproxySetProperty: %@", name);
    _abort();
}
bool idproxyRemoveProperty(NPObject *npobj, NPIdentifier name) {
    err("! idproxyRemoveProperty: %@", name);
    _abort();
}
bool idproxyEnumeration(NPObject *npobj, NPIdentifier **value, uint32_t *count) {
    err("! idproxyEnumeration");
    _abort();
}
bool idproxyConstruct(NPObject *npobj, const NPVariant *args, uint32_t argCount,     NPVariant *result) {
    err("! idproxyConstruct");
    _abort();
}

static NPClass idproxy_class = {
    NP_CLASS_STRUCT_VERSION,
    idproxyAllocate,
    idproxyDeallocate,
    (void *) 0xdeadbee3,
    idproxyHasMethod,
    idproxyInvoke,
    idproxyInvokeDefault,
    idproxyHasProperty,
    idproxyGetProperty,
    idproxySetProperty,
    idproxyRemoveProperty,
    idproxyEnumeration,
    idproxyConstruct
};

extern void iface_getvalue(NPNVariable variable, void *ret_value);

NPError NPN_GetValue(NPP instance, NPNVariable variable, void *ret_value) {
    notice("...getValue: %d", (int) variable);
    {
        uint32_t *p = ret_value;
        notice("   %x %x %x %x", p[0], p[1], p[2], p[3]);
    }
    if(variable >= ((NPNVariable)1000) && variable <= ((NPNVariable)1011)) {
        iface_getvalue(variable, ret_value);
        return 0;
    }
    switch(variable) {
    case kSupportedDrawingModel_ANPGetValue:
        // This is not a mistake: it replicates a bug in Android.
        *((uint32_t *) ret_value) = kBitmap_ANPDrawingModel & kSurface_ANPDrawingModel;
        break;
    case kJavaContext_ANPGetValue:
        notice("kJavaContext_ANPGetValue");
        *((jobject *) ret_value) = android_context;
        break;
    case NPNVWindowNPObject: {
        int window_id;
        if(get_window_object(food, &window_id)) {
            perror("get_window_object");
            _abort();
        }
        *((NPObject **) ret_value) = new_idproxy_object(window_id);
        break;
    }
    default:
        notice("unknown GetValue! returning with error");
        *((uint32_t *) ret_value) = 0xdeadbee1;
        return NPERR_GENERIC_ERROR;
    }
    return 0;
}
NPError NPN_SetValue(NPP instance, NPPVariable variable, void *value) {
    notice("...setValue: %d => %p", (int) variable, value);
    switch(variable) {
    case kRequestDrawingModel_ANPSetValue:
        _assert(value == (void *) kSurface_ANPDrawingModel);
        return 0;
    case kAcceptEvents_ANPSetValue:
        notice("acceptEvents: %d", *((int *) value));
        return 0;
    default:
        return NPERR_GENERIC_ERROR;
    }
}


NPObject *NPN_CreateObject(NPP npp, NPClass *class) {
    NPObject *ret;
    notice("NPN_CreateObject");
    if(class->allocate) {
        ret = class->allocate(npp, class);
    } else {
        notice("NPN_CreateObject: using default allocator");
        ret = calloc(1, sizeof(NPObject));
        ret->referenceCount = 1;
    }
    ret->_class = class;
    return ret;
}

NPObject *NPN_RetainObject (NPObject *obj) {
    obj->referenceCount++;
    return obj;
}

void NPN_ReleaseObject (NPObject *obj) {
    if(0 == --obj->referenceCount) {
        notice("NPN_ReleaseObject: object %p to be destroyed", obj);
        if(obj->_class->deallocate) {
            obj->_class->deallocate(obj);
        } else {
            notice("NPN_ReleaseObject: using free()");
            free(obj);
        }
    }
}

void *NPN_MemAlloc (uint32 size) {
    return calloc(1, size);
}

void NPN_MemFree (void *ptr) {
    free(ptr);
}

int connection_response(int rpcfd, NPStream *stream, void *headers, size_t headers_len, int64_t expected_content_length) {
    log("connection_response [%p] [%s]", stream, (char *) headers);
    _assertZero(stream->headers);
    if(headers) {
        if(headers_len) {
            stream->headers = headers;
        } else {
            free(headers);
            headers = NULL;
        }
    }
    stream->end = (uint32_t) expected_content_length;
    log("stream->end = %d", stream->end);
    // XXX lastmodified
    stream->lastmodified = 1277171069;
    int *offset_p = (int *) &stream->ndata;
    *offset_p = 0;
    uint16_t stype;
    _assertZero(pluginfuncs.newstream(&nppt, (void *) 0xdeadbeef, stream, false, &stype));
    _assert(stype == NP_NORMAL);
    return 0;
}

// FIXME: We really need to make all this stuff async
int connection_got_data(int rpcfd, NPStream *stream, void *data, size_t data_len) {
    int avail, remain, ret, *p;

    notice("connection_got_data [%p]: %d bytes", stream, data_len);

    avail = pluginfuncs.writeready(&nppt, stream);
    _assert(avail >= data_len);
    notice("avail = %d", avail);

    remain = data_len;
    p = (int *) &stream->ndata;
    do {
        ret = pluginfuncs.write(&nppt, stream, *p, data_len, data);
        *p += ret;
        remain -= ret;
    } while (remain > 0);

    notice("sent.");
    free(data);

    return 0;
}

static void even_later(CFRunLoopTimerRef, void *);
int connection_all_done(int rpcfd, NPStream *stream, bool successful) {
    log("connection_all_done [%p]: successful=%d", stream, successful);
    if(!successful) {
        _abortWithError("The connection failed.");
    }
    NPReason reason = successful ? NPRES_DONE : NPRES_NETWORK_ERR;
    int ret = pluginfuncs.destroystream(&nppt, stream, reason);
    _assertZero(ret);
    if(stream->notifyData == MAP_FAILED) { // dummy value
        do_later(even_later, NULL);
    } else if(stream->notifyData) {
        pluginfuncs.urlnotify(&nppt, stream->url, reason, stream->notifyData);
    }
    if(stream->url) free((void*)stream->url);
    if(stream->headers) free((void*)stream->headers);
    free(stream);
    return 0;
}

NPError NPN_PostURLNotify(NPP    instance,
                         const  char* url,
                         const  char* target,
                         uint32      len,
                         const char* buf,
                         NPBool      file,
                         void*   notifyData) {
    NPStream *stream = malloc(sizeof(NPStream));
    stream->pdata = NULL;
    stream->ndata = NULL;
    stream->end = 0;
    stream->lastmodified = 0;
    stream->notifyData = notifyData;
    stream->headers = NULL;
    if(!target) target = "";
    log("PostURLNotify: [%s] [%s] (stream=%p)", url, target, stream);
    _assertZero(new_post_connection (food, stream, (char *) url, strlen(url), (char *) target, strlen(target), file, (char *) buf, len, (void **) &stream->url, NULL));
    return 0;
}

NPError NPN_GetURLNotify(NPP    instance,
                         const  char* url,
                         const  char* target,
                         void*   notifyData) {
    NPStream *stream = malloc(sizeof(NPStream));
    stream->pdata = NULL;
    stream->ndata = NULL;
    stream->end = 0;
    stream->lastmodified = 0;
    stream->notifyData = notifyData;
    stream->headers = NULL;
    if(!target) target = "";
    log("GetURLNotify: [%s] [%s] (stream=%p)", url, target, stream);
    _assertZero(new_get_connection(food, stream, (char *) url, strlen(url), (char *) target, strlen(target), (void **) &stream->url, NULL));
    return 0;
}

static void npnTimerCB(CFRunLoopTimerRef ref, void *info) {
    //notice("npnTimerCB %p", info);
    void (*timerFunc)(NPP, uint32) = info;
    timerFunc(&nppt/*x*/, (uint32) ref);
}

uint32 NPN_ScheduleTimer(NPP npp, uint32 interval, NPBool repeat, void (*timerFunc)(NPP npp, uint32 timerID)) {
    notice("ScheduleTimer interval=%u repeat=%s cb=%p", (unsigned int) interval, repeat ? "YES" : "NO", timerFunc);
    CFRunLoopTimerContext ctx;
    memset(&ctx, 0, sizeof(ctx));
    ctx.info = timerFunc;
    double ival = 0.001 * interval;
    notice("[%p] ival=%f", timerFunc, (float) ival);
    CFRunLoopTimerRef ref = CFRunLoopTimerCreate(NULL,  CFAbsoluteTimeGetCurrent() + ival, repeat ? ival : 0.0, 0, 0, npnTimerCB, &ctx);
    CFRunLoopAddTimer(CFRunLoopGetMain(), ref, kCFRunLoopCommonModes);
    return (uint32) ref;
}

void NPN_UnscheduleTimer(NPP npp, uint32 timerID) {
    notice("UnscheduleTimer");
    CFRunLoopTimerRef ref = (void *) timerID;
    CFRunLoopTimerInvalidate(ref);
    CFRelease(ref); // account for the initial creation
}

bool NPN_Evaluate(NPP npp, NPObject *obj, NPString *script, NPVariant *result) {
    int ret;
    _assertZero(evaluate_web_script(food, (void *) script->UTF8Characters, script->UTF8Length, &ret));
    *((struct NPFuckedUpVariant *) result) = variant_for_object_name(ret);
    return true;
}

void NPN_PushPopupsEnabledState(NPP npp, NPBool enabled) {
}

void NPN_PopPopupsEnabledState(NPP npp) {
}

void handle_event(void *event) {
    notice("%s: handling event %p", __func__, event);
    int16 ret = pluginfuncs.event(&nppt, event);
    notice("%s: the plugin %s the event %p)", __func__, ret ? "HANDLED" : "IGNORED", pluginfuncs.event);
}


void foo() { return; }

static void later(CFRunLoopTimerRef a, void *b);

static void init_idproxy_class() {
    idproxy_class.allocate = stub(idproxyAllocate);
    idproxy_class.deallocate = stub(idproxyDeallocate);
    idproxy_class.invalidate = (void *) 0xdeadbee3;
    idproxy_class.hasMethod = stub(idproxyHasMethod);
    idproxy_class.invoke = stub(idproxyInvoke);
    idproxy_class.invokeDefault = stub(idproxyInvokeDefault);
    idproxy_class.hasProperty = stub(idproxyHasProperty);
    idproxy_class.getProperty = stub(idproxyGetProperty);
    idproxy_class.setProperty = stub(idproxySetProperty);
    idproxy_class.removeProperty = stub(idproxyRemoveProperty);
    idproxy_class.enumerate = stub(idproxyEnumeration);
    idproxy_class.construct = stub(idproxyConstruct);
}

static void init_funcs() {
    pattern(&funcs, 0x40404040, sizeof(funcs));
    funcs.size = sizeof(funcs);
    funcs.version = 9999;
    funcs.getvalue = stub(NPN_GetValue);
    funcs.setvalue = stub(NPN_SetValue);
    funcs.createobject = stub(NPN_CreateObject);
    funcs.retainobject = stub(NPN_RetainObject);
    funcs.releaseobject = stub(NPN_ReleaseObject);
    funcs.memalloc = stub(NPN_MemAlloc);
    funcs.memfree = stub(NPN_MemFree);
    funcs.getstringidentifier = stub(NPN_GetStringIdentifier);
    funcs.getintidentifier = stub(NPN_GetIntIdentifier);
    funcs.identifierisstring = stub(NPN_IdentifierIsString);
    funcs.utf8fromidentifier = stub(NPN_UTF8FromIdentifier);
    funcs.intfromidentifier = stub(NPN_IntFromIdentifier);
    funcs.getproperty = stub(NPN_GetProperty);
    funcs.releasevariantvalue = stub(NPN_ReleaseVariantValue);
    funcs.geturlnotify = stub(NPN_GetURLNotify);
    funcs.posturlnotify = stub(NPN_PostURLNotify);
    funcs.scheduletimer = stub(NPN_ScheduleTimer);
    funcs.unscheduletimer = stub(NPN_UnscheduleTimer);
    funcs.evaluate = stub(NPN_Evaluate);
    funcs.invoke = stub(NPN_Invoke);
    funcs.pushpopupsenabledstate = stub(NPN_PushPopupsEnabledState);
    funcs.poppopupsenabledstate = stub(NPN_PopPopupsEnabledState);

    pattern(&pluginfuncs, 0xbeef0000, sizeof(pluginfuncs));
    pluginfuncs.size = sizeof(pluginfuncs);
}

static char *src;

void go(NP_InitializeFuncPtr NP_Initialize_ptr, void *JNI_OnLoad_ptr_) {
    init_idproxy_class();

    JNI_OnLoad_ptr = JNI_OnLoad_ptr_;

    init_funcs();

    init_jni();
    // take the plunge
    notice("Taking the plunge to %p", NP_Initialize_ptr);
    foo();
    int ret = NP_Initialize_ptr(&funcs, &pluginfuncs, &env, (void *) 0xdeaddead);
    notice("NP_Initialize return value: %d", ret);
    _assertZero(ret);

    char *parameters; size_t parameters_len;
    int parameters_count;

    _assertZero(get_parameters(food, (void **) &parameters, &parameters_len, &parameters_count));

    int real_count = 0;

    char **argn = malloc((parameters_count + 3) * sizeof(char *));
    char **argv = malloc((parameters_count + 3) * sizeof(char *));

    argn[real_count] = "salign";
    argv[real_count++] = "tl";

    while(parameters_count--) {
        char *k = parameters;
        parameters += strlen(k) + 1;
        char *v = parameters;
        parameters += strlen(v) + 1;
        log("%s -> %s", k, v);
        if(strcmp(k, "salign")) {
            argn[real_count] = k;
            argv[real_count++] = v;
        }
        if(!strcmp(k, "src") || !strcmp(k, "movie")) {
            src = v;
        }
    }

    _assert(src);

    ret = pluginfuncs.newp("application/x-shockwave-flash", &nppt, NP_EMBED, real_count, argn, argv, NULL);
    notice("NPP_New return value: %d", ret);
    _assertZero(ret);

    window.width = movie_w;
    window.height = movie_h;
    window.x = 0;
    window.y = 0;
    window.clipRect = (NPRect) {0, 0, window.width, window.height};
    window.type = NPWindowTypeDrawable;
    _assertZero(pluginfuncs.setwindow(&nppt, &window));

    _assertZero(pluginfuncs.getvalue(&nppt, NPPVpluginScriptableNPObject, &scriptable));

    jobject js;
    ret = pluginfuncs.getvalue(&nppt, kJavaSurface_ANPGetValue, &js);
    _assertZero(ret);

    //post_lifecycle_event(kResume_ANPLifecycleAction);
    //post_lifecycle_event(kEnterFullScreen_ANPLifecycleAction);
    //post_lifecycle_event(kOffScreen_ANPLifecycleAction);
    //post_lifecycle_event(kOnScreen_ANPLifecycleAction);
    do_later(later, NULL);
    CFRunLoopRun();
}

static void even_later(CFRunLoopTimerRef a, void *b) {
    log("---- EVEN LATER ----");
}

static void later(CFRunLoopTimerRef a, void *b) {
    log("---- LATER ----");
    post_lifecycle_event(kOnScreen_ANPLifecycleAction);
    post_lifecycle_event(kGainFocus_ANPLifecycleAction);
    post_lifecycle_event(kOnLoad_ANPLifecycleAction);
    NPN_GetURLNotify(&nppt, src, NULL, MAP_FAILED);
}
