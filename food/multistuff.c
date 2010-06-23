#include "myjni.h"
#include <CoreFoundation/CoreFoundation.h>
#include "common.h"
#include <assert.h>
jboolean typeRefToBoolean(CFTypeRef val) {
    return (jboolean) CFBooleanGetValue(val);
}
CFTypeRef booleanToTypeRef(jboolean val) {
    return val ? kCFBooleanTrue : kCFBooleanFalse;
}
jobject typeRefToObject(CFTypeRef val) {
    return (void *) val;
}
CFTypeRef objectToTypeRef(jobject val) {
    return val;
}
jchar typeRefToChar(CFTypeRef val) {
    jchar ret;
    _assert(CFNumberGetValue((CFNumberRef) val, kCFNumberSInt16Type, &ret));
    return ret;
}
CFTypeRef charToTypeRef(jchar val) {
    return CFNumberCreate(NULL, kCFNumberSInt16Type, &val);
}
struct method_char { jchar (*v)(jobject, va_list); jchar (*a)(jobject, const jvalue *); };
jchar impl_CallCharMethodA(JNIEnv *env, jobject obj, jmethodID methodID, const jvalue *args) {
    const void *meth = CFDictionaryGetValue(obj, (CFStringRef) methodID);
    if(!meth) { log("Unknown method (A): %@", methodID); _abort(); }
    struct method_char *st;
    _assert(st = RawDataGetPtr(meth));
    if(!st->a) { log("No A impl: %@", methodID); _abort(); }
    return st->a(obj, args);
}
jchar impl_CallCharMethodV(JNIEnv *env, jobject obj, jmethodID methodID, va_list args) {
    const void *meth = CFDictionaryGetValue(obj, (CFStringRef) methodID);
    if(!meth) { log("Unknown method (V): %@", methodID); _abort(); }
    struct method_char *st;
    _assert(st = RawDataGetPtr(meth));
    if(!st->v) { log("No V impl: %@", methodID); _abort(); }
    return st->v(obj, args);
}
jchar impl_CallCharMethod(JNIEnv *env, jobject obj, jmethodID methodID, ...) {
    va_list args;
    va_start(args, methodID);
    jchar ret = impl_CallCharMethodV(env, obj, methodID, args);
    va_end(args);
    return ret;
}
void impl_SetCharField(JNIEnv *env, jobject obj, jfieldID fieldID, jchar val) {
    CFTypeRef ref = charToTypeRef(val);
    CFDictionarySetValue(obj, (CFStringRef) fieldID, ref);
    CFRelease(ref);
}
jchar impl_GetCharField(JNIEnv *env, jobject obj, jfieldID fieldID) {
        CFTypeRef ref = CFDictionaryGetValue(obj, (CFStringRef) fieldID);
        if(!ref) { log("Unknown property: %@", fieldID); _abort(); }
        return typeRefToChar(ref);
}
jfloat typeRefToFloat(CFTypeRef val) {
    jfloat ret;
    _assert(CFNumberGetValue((CFNumberRef) val, kCFNumberFloatType, &ret));
    return ret;
}
CFTypeRef floatToTypeRef(jfloat val) {
    return CFNumberCreate(NULL, kCFNumberFloatType, &val);
}
struct method_float { jfloat (*v)(jobject, va_list); jfloat (*a)(jobject, const jvalue *); };
jfloat impl_CallFloatMethodA(JNIEnv *env, jobject obj, jmethodID methodID, const jvalue *args) {
    const void *meth = CFDictionaryGetValue(obj, (CFStringRef) methodID);
    if(!meth) { log("Unknown method (A): %@", methodID); _abort(); }
    struct method_float *st;
    _assert(st = RawDataGetPtr(meth));
    if(!st->a) { log("No A impl: %@", methodID); _abort(); }
    return st->a(obj, args);
}
jfloat impl_CallFloatMethodV(JNIEnv *env, jobject obj, jmethodID methodID, va_list args) {
    const void *meth = CFDictionaryGetValue(obj, (CFStringRef) methodID);
    if(!meth) { log("Unknown method (V): %@", methodID); _abort(); }
    struct method_float *st;
    _assert(st = RawDataGetPtr(meth));
    if(!st->v) { log("No V impl: %@", methodID); _abort(); }
    return st->v(obj, args);
}
jfloat impl_CallFloatMethod(JNIEnv *env, jobject obj, jmethodID methodID, ...) {
    va_list args;
    va_start(args, methodID);
    jfloat ret = impl_CallFloatMethodV(env, obj, methodID, args);
    va_end(args);
    return ret;
}
void impl_SetFloatField(JNIEnv *env, jobject obj, jfieldID fieldID, jfloat val) {
    CFTypeRef ref = floatToTypeRef(val);
    CFDictionarySetValue(obj, (CFStringRef) fieldID, ref);
    CFRelease(ref);
}
jfloat impl_GetFloatField(JNIEnv *env, jobject obj, jfieldID fieldID) {
        CFTypeRef ref = CFDictionaryGetValue(obj, (CFStringRef) fieldID);
        if(!ref) { log("Unknown property: %@", fieldID); _abort(); }
        return typeRefToFloat(ref);
}
struct method_boolean { jboolean (*v)(jobject, va_list); jboolean (*a)(jobject, const jvalue *); };
jboolean impl_CallBooleanMethodA(JNIEnv *env, jobject obj, jmethodID methodID, const jvalue *args) {
    const void *meth = CFDictionaryGetValue(obj, (CFStringRef) methodID);
    if(!meth) { log("Unknown method (A): %@", methodID); _abort(); }
    struct method_boolean *st;
    _assert(st = RawDataGetPtr(meth));
    if(!st->a) { log("No A impl: %@", methodID); _abort(); }
    return st->a(obj, args);
}
jboolean impl_CallBooleanMethodV(JNIEnv *env, jobject obj, jmethodID methodID, va_list args) {
    const void *meth = CFDictionaryGetValue(obj, (CFStringRef) methodID);
    if(!meth) { log("Unknown method (V): %@", methodID); _abort(); }
    struct method_boolean *st;
    _assert(st = RawDataGetPtr(meth));
    if(!st->v) { log("No V impl: %@", methodID); _abort(); }
    return st->v(obj, args);
}
jboolean impl_CallBooleanMethod(JNIEnv *env, jobject obj, jmethodID methodID, ...) {
    va_list args;
    va_start(args, methodID);
    jboolean ret = impl_CallBooleanMethodV(env, obj, methodID, args);
    va_end(args);
    return ret;
}
void impl_SetBooleanField(JNIEnv *env, jobject obj, jfieldID fieldID, jboolean val) {
    CFTypeRef ref = booleanToTypeRef(val);
    CFDictionarySetValue(obj, (CFStringRef) fieldID, ref);
    CFRelease(ref);
}
jboolean impl_GetBooleanField(JNIEnv *env, jobject obj, jfieldID fieldID) {
        CFTypeRef ref = CFDictionaryGetValue(obj, (CFStringRef) fieldID);
        if(!ref) { log("Unknown property: %@", fieldID); _abort(); }
        return typeRefToBoolean(ref);
}
jint typeRefToInt(CFTypeRef val) {
    jint ret;
    _assert(CFNumberGetValue((CFNumberRef) val, kCFNumberLongType, &ret));
    return ret;
}
CFTypeRef intToTypeRef(jint val) {
    return CFNumberCreate(NULL, kCFNumberLongType, &val);
}
struct method_int { jint (*v)(jobject, va_list); jint (*a)(jobject, const jvalue *); };
jint impl_CallIntMethodA(JNIEnv *env, jobject obj, jmethodID methodID, const jvalue *args) {
    const void *meth = CFDictionaryGetValue(obj, (CFStringRef) methodID);
    if(!meth) { log("Unknown method (A): %@", methodID); _abort(); }
    struct method_int *st;
    _assert(st = RawDataGetPtr(meth));
    if(!st->a) { log("No A impl: %@", methodID); _abort(); }
    return st->a(obj, args);
}
jint impl_CallIntMethodV(JNIEnv *env, jobject obj, jmethodID methodID, va_list args) {
    const void *meth = CFDictionaryGetValue(obj, (CFStringRef) methodID);
    if(!meth) { log("Unknown method (V): %@", methodID); _abort(); }
    struct method_int *st;
    _assert(st = RawDataGetPtr(meth));
    if(!st->v) { log("No V impl: %@", methodID); _abort(); }
    return st->v(obj, args);
}
jint impl_CallIntMethod(JNIEnv *env, jobject obj, jmethodID methodID, ...) {
    va_list args;
    va_start(args, methodID);
    jint ret = impl_CallIntMethodV(env, obj, methodID, args);
    va_end(args);
    return ret;
}
void impl_SetIntField(JNIEnv *env, jobject obj, jfieldID fieldID, jint val) {
    CFTypeRef ref = intToTypeRef(val);
    CFDictionarySetValue(obj, (CFStringRef) fieldID, ref);
    CFRelease(ref);
}
jint impl_GetIntField(JNIEnv *env, jobject obj, jfieldID fieldID) {
        CFTypeRef ref = CFDictionaryGetValue(obj, (CFStringRef) fieldID);
        if(!ref) { log("Unknown property: %@", fieldID); _abort(); }
        return typeRefToInt(ref);
}
jdouble typeRefToDouble(CFTypeRef val) {
    jdouble ret;
    _assert(CFNumberGetValue((CFNumberRef) val, kCFNumberDoubleType, &ret));
    return ret;
}
CFTypeRef doubleToTypeRef(jdouble val) {
    return CFNumberCreate(NULL, kCFNumberDoubleType, &val);
}
struct method_double { jdouble (*v)(jobject, va_list); jdouble (*a)(jobject, const jvalue *); };
jdouble impl_CallDoubleMethodA(JNIEnv *env, jobject obj, jmethodID methodID, const jvalue *args) {
    const void *meth = CFDictionaryGetValue(obj, (CFStringRef) methodID);
    if(!meth) { log("Unknown method (A): %@", methodID); _abort(); }
    struct method_double *st;
    _assert(st = RawDataGetPtr(meth));
    if(!st->a) { log("No A impl: %@", methodID); _abort(); }
    return st->a(obj, args);
}
jdouble impl_CallDoubleMethodV(JNIEnv *env, jobject obj, jmethodID methodID, va_list args) {
    const void *meth = CFDictionaryGetValue(obj, (CFStringRef) methodID);
    if(!meth) { log("Unknown method (V): %@", methodID); _abort(); }
    struct method_double *st;
    _assert(st = RawDataGetPtr(meth));
    if(!st->v) { log("No V impl: %@", methodID); _abort(); }
    return st->v(obj, args);
}
jdouble impl_CallDoubleMethod(JNIEnv *env, jobject obj, jmethodID methodID, ...) {
    va_list args;
    va_start(args, methodID);
    jdouble ret = impl_CallDoubleMethodV(env, obj, methodID, args);
    va_end(args);
    return ret;
}
void impl_SetDoubleField(JNIEnv *env, jobject obj, jfieldID fieldID, jdouble val) {
    CFTypeRef ref = doubleToTypeRef(val);
    CFDictionarySetValue(obj, (CFStringRef) fieldID, ref);
    CFRelease(ref);
}
jdouble impl_GetDoubleField(JNIEnv *env, jobject obj, jfieldID fieldID) {
        CFTypeRef ref = CFDictionaryGetValue(obj, (CFStringRef) fieldID);
        if(!ref) { log("Unknown property: %@", fieldID); _abort(); }
        return typeRefToDouble(ref);
}
jbyte typeRefToByte(CFTypeRef val) {
    jbyte ret;
    _assert(CFNumberGetValue((CFNumberRef) val, kCFNumberSInt8Type, &ret));
    return ret;
}
CFTypeRef byteToTypeRef(jbyte val) {
    return CFNumberCreate(NULL, kCFNumberSInt8Type, &val);
}
struct method_byte { jbyte (*v)(jobject, va_list); jbyte (*a)(jobject, const jvalue *); };
jbyte impl_CallByteMethodA(JNIEnv *env, jobject obj, jmethodID methodID, const jvalue *args) {
    const void *meth = CFDictionaryGetValue(obj, (CFStringRef) methodID);
    if(!meth) { log("Unknown method (A): %@", methodID); _abort(); }
    struct method_byte *st;
    _assert(st = RawDataGetPtr(meth));
    if(!st->a) { log("No A impl: %@", methodID); _abort(); }
    return st->a(obj, args);
}
jbyte impl_CallByteMethodV(JNIEnv *env, jobject obj, jmethodID methodID, va_list args) {
    const void *meth = CFDictionaryGetValue(obj, (CFStringRef) methodID);
    if(!meth) { log("Unknown method (V): %@", methodID); _abort(); }
    struct method_byte *st;
    _assert(st = RawDataGetPtr(meth));
    if(!st->v) { log("No V impl: %@", methodID); _abort(); }
    return st->v(obj, args);
}
jbyte impl_CallByteMethod(JNIEnv *env, jobject obj, jmethodID methodID, ...) {
    va_list args;
    va_start(args, methodID);
    jbyte ret = impl_CallByteMethodV(env, obj, methodID, args);
    va_end(args);
    return ret;
}
void impl_SetByteField(JNIEnv *env, jobject obj, jfieldID fieldID, jbyte val) {
    CFTypeRef ref = byteToTypeRef(val);
    CFDictionarySetValue(obj, (CFStringRef) fieldID, ref);
    CFRelease(ref);
}
jbyte impl_GetByteField(JNIEnv *env, jobject obj, jfieldID fieldID) {
        CFTypeRef ref = CFDictionaryGetValue(obj, (CFStringRef) fieldID);
        if(!ref) { log("Unknown property: %@", fieldID); _abort(); }
        return typeRefToByte(ref);
}
struct method_object { jobject (*v)(jobject, va_list); jobject (*a)(jobject, const jvalue *); };
jobject impl_CallObjectMethodA(JNIEnv *env, jobject obj, jmethodID methodID, const jvalue *args) {
    const void *meth = CFDictionaryGetValue(obj, (CFStringRef) methodID);
    if(!meth) { log("Unknown method (A): %@", methodID); _abort(); }
    struct method_object *st;
    _assert(st = RawDataGetPtr(meth));
    if(!st->a) { log("No A impl: %@", methodID); _abort(); }
    return st->a(obj, args);
}
jobject impl_CallObjectMethodV(JNIEnv *env, jobject obj, jmethodID methodID, va_list args) {
    const void *meth = CFDictionaryGetValue(obj, (CFStringRef) methodID);
    if(!meth) { log("Unknown method (V): %@", methodID); _abort(); }
    struct method_object *st;
    _assert(st = RawDataGetPtr(meth));
    if(!st->v) { log("No V impl: %@", methodID); _abort(); }
    return st->v(obj, args);
}
jobject impl_CallObjectMethod(JNIEnv *env, jobject obj, jmethodID methodID, ...) {
    va_list args;
    va_start(args, methodID);
    jobject ret = impl_CallObjectMethodV(env, obj, methodID, args);
    va_end(args);
    return ret;
}
void impl_SetObjectField(JNIEnv *env, jobject obj, jfieldID fieldID, jobject val) {
    CFTypeRef ref = objectToTypeRef(val);
    CFDictionarySetValue(obj, (CFStringRef) fieldID, ref);
    CFRelease(ref);
}
jobject impl_GetObjectField(JNIEnv *env, jobject obj, jfieldID fieldID) {
        CFTypeRef ref = CFDictionaryGetValue(obj, (CFStringRef) fieldID);
        if(!ref) { log("Unknown property: %@", fieldID); _abort(); }
        return typeRefToObject(ref);
}
jlong typeRefToLong(CFTypeRef val) {
    jlong ret;
    _assert(CFNumberGetValue((CFNumberRef) val, kCFNumberLongLongType, &ret));
    return ret;
}
CFTypeRef longToTypeRef(jlong val) {
    return CFNumberCreate(NULL, kCFNumberLongLongType, &val);
}
struct method_long { jlong (*v)(jobject, va_list); jlong (*a)(jobject, const jvalue *); };
jlong impl_CallLongMethodA(JNIEnv *env, jobject obj, jmethodID methodID, const jvalue *args) {
    const void *meth = CFDictionaryGetValue(obj, (CFStringRef) methodID);
    if(!meth) { log("Unknown method (A): %@", methodID); _abort(); }
    struct method_long *st;
    _assert(st = RawDataGetPtr(meth));
    if(!st->a) { log("No A impl: %@", methodID); _abort(); }
    return st->a(obj, args);
}
jlong impl_CallLongMethodV(JNIEnv *env, jobject obj, jmethodID methodID, va_list args) {
    const void *meth = CFDictionaryGetValue(obj, (CFStringRef) methodID);
    if(!meth) { log("Unknown method (V): %@", methodID); _abort(); }
    struct method_long *st;
    _assert(st = RawDataGetPtr(meth));
    if(!st->v) { log("No V impl: %@", methodID); _abort(); }
    return st->v(obj, args);
}
jlong impl_CallLongMethod(JNIEnv *env, jobject obj, jmethodID methodID, ...) {
    va_list args;
    va_start(args, methodID);
    jlong ret = impl_CallLongMethodV(env, obj, methodID, args);
    va_end(args);
    return ret;
}
void impl_SetLongField(JNIEnv *env, jobject obj, jfieldID fieldID, jlong val) {
    CFTypeRef ref = longToTypeRef(val);
    CFDictionarySetValue(obj, (CFStringRef) fieldID, ref);
    CFRelease(ref);
}
jlong impl_GetLongField(JNIEnv *env, jobject obj, jfieldID fieldID) {
        CFTypeRef ref = CFDictionaryGetValue(obj, (CFStringRef) fieldID);
        if(!ref) { log("Unknown property: %@", fieldID); _abort(); }
        return typeRefToLong(ref);
}
jshort typeRefToShort(CFTypeRef val) {
    jshort ret;
    _assert(CFNumberGetValue((CFNumberRef) val, kCFNumberSInt16Type, &ret));
    return ret;
}
CFTypeRef shortToTypeRef(jshort val) {
    return CFNumberCreate(NULL, kCFNumberSInt16Type, &val);
}
struct method_short { jshort (*v)(jobject, va_list); jshort (*a)(jobject, const jvalue *); };
jshort impl_CallShortMethodA(JNIEnv *env, jobject obj, jmethodID methodID, const jvalue *args) {
    const void *meth = CFDictionaryGetValue(obj, (CFStringRef) methodID);
    if(!meth) { log("Unknown method (A): %@", methodID); _abort(); }
    struct method_short *st;
    _assert(st = RawDataGetPtr(meth));
    if(!st->a) { log("No A impl: %@", methodID); _abort(); }
    return st->a(obj, args);
}
jshort impl_CallShortMethodV(JNIEnv *env, jobject obj, jmethodID methodID, va_list args) {
    const void *meth = CFDictionaryGetValue(obj, (CFStringRef) methodID);
    if(!meth) { log("Unknown method (V): %@", methodID); _abort(); }
    struct method_short *st;
    _assert(st = RawDataGetPtr(meth));
    if(!st->v) { log("No V impl: %@", methodID); _abort(); }
    return st->v(obj, args);
}
jshort impl_CallShortMethod(JNIEnv *env, jobject obj, jmethodID methodID, ...) {
    va_list args;
    va_start(args, methodID);
    jshort ret = impl_CallShortMethodV(env, obj, methodID, args);
    va_end(args);
    return ret;
}
void impl_SetShortField(JNIEnv *env, jobject obj, jfieldID fieldID, jshort val) {
    CFTypeRef ref = shortToTypeRef(val);
    CFDictionarySetValue(obj, (CFStringRef) fieldID, ref);
    CFRelease(ref);
}
jshort impl_GetShortField(JNIEnv *env, jobject obj, jfieldID fieldID) {
        CFTypeRef ref = CFDictionaryGetValue(obj, (CFStringRef) fieldID);
        if(!ref) { log("Unknown property: %@", fieldID); _abort(); }
        return typeRefToShort(ref);
}

struct method_void { void (*v)(jobject, va_list); void (*a)(jobject, const     jvalue *); };
void impl_CallVoidMethodA(JNIEnv *env, jobject obj, jmethodID methodID, const    jvalue *args) {
    const void *meth = CFDictionaryGetValue(obj, (CFStringRef)          
methodID);
    if(!meth) { log("Unknown method (A): %@", methodID); _abort(); }
    struct method_void *st; 
    _assert(st = RawDataGetPtr(meth));
    if(!st->a) { log("No A impl: %@", methodID); _abort(); }
    st->a(obj, args);
}
void impl_CallVoidMethodV(JNIEnv *env, jobject obj, jmethodID methodID, va_list  args) {
    const void *meth = CFDictionaryGetValue(obj, (CFStringRef)          
methodID);
    if(!meth) { log("Unknown method (V): %@", methodID); _abort(); }
    struct method_void *st;
    _assert(st = RawDataGetPtr(meth));
    if(!st->v) { log("No V impl: %@", methodID); _abort(); }
    st->v(obj, args);
}
void impl_CallVoidMethod(JNIEnv *env, jobject obj, jmethodID methodID, ...) {
    va_list args;
    va_start(args, methodID);
    impl_CallVoidMethodV(env, obj, methodID, args);
    va_end(args);
}

