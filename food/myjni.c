#include "common.h"
#include "myjni.h"
#include "classes.h"

JNIEnv env;
CFMutableDictionaryRef classes;
static struct JNINativeInterface_ env_functions;
static JavaVM vm;
static struct JNIInvokeInterface_ vm_functions;

jobject new_jobject(CFMutableDictionaryRef properties) {
    return properties;
    /*// todo actually manage refs
    if(!properties) return NULL;
    jobject j = calloc(1, sizeof(struct _jobject));
    j = properties;
    return j;*/
}

// Env (aka JNINativeInterface)

jint impl_GetJavaVM(JNIEnv *this, JavaVM **vm_) {
    notice("GetJavaVM");
    *vm_ = &vm;
    return 0;
}

jobject impl_NewGlobalRef(JNIEnv *this, jobject lobj) {
    notice("NewGlobalRef: %p", lobj);
    return lobj;
}

void impl_DeleteGlobalRef(JNIEnv *this, jobject gref) {
    notice("DeleteGlobalRef: %p", gref);
}


jint impl_PushLocalFrame(JNIEnv *env, jint capacity) {
    notice("PushLocalFrame: %d", (int) capacity);
    return 0;
}

jobject impl_PopLocalFrame(JNIEnv *env, jobject result) {
    notice("PopLocalFrame");
    return result;
}

jclass impl_FindClass(JNIEnv *env, const char *name) {
    notice("FindClass: %s", name);
    CFStringRef name_ = CFStringCreateWithCStringNoCopy(NULL, name, kCFStringEncodingUTF8, kCFAllocatorNull);
    jclass ret = new_jobject((CFMutableDictionaryRef) CFDictionaryGetValue(classes, name_));
    CFRelease(name_);
    if(!ret) {
        warn("(%s wasn't found)", name);
        ret = (jclass) 0xbadbadba;
    }
    return ret;
}

jclass impl_GetObjectClass(JNIEnv *env, jobject obj) {
    CFStringRef clsname = g(obj, ".class");
    if(!clsname) {
        notice("object has no class: %@", obj);
        _abort();
    }
    notice("GetObjectClass");
    return new_jobject((CFMutableDictionaryRef) CFDictionaryGetValue(classes, clsname));
}

jmethodID impl_GetXID (JNIEnv *env, jobject x, const char *name, const char *sig) {
    notice("GetXID: %s[%s]", name, sig);
    return (jmethodID) CFStringCreateWithFormat(NULL, NULL, CFSTR("%s[%s]"), name, sig);
}

#include "multistuff.h"

jstring impl_NewStringUTF(JNIEnv *env, const char *bytes) {
    return new_jobject(new_stringobject(bytes));
}

jboolean impl_ExceptionCheck(JNIEnv *env) {
    return JNI_FALSE;
}

const char *impl_GetStringUTFChars(JNIEnv *env, jstring string,
jboolean *isCopy) {
    CFStringRef str = g(string, "theString");
    const char *ret = CFStringGetCStringPtr(str, kCFStringEncodingUTF8);
    if(ret) {
        if(isCopy) *isCopy = true;
    } else {
        if(isCopy) *isCopy = false;
        // lol this won't handle actual UTF8 chars
        char *ret_ = malloc(CFStringGetLength(str)+1);
        _assert(CFStringGetCString(str, ret_, CFStringGetLength(str)+1, kCFStringEncodingUTF8));
        ret = ret_;
    }
    return ret;
}

void impl_ReleaseStringUTFChars(JNIEnv *env, jstring string,
const char *utf) {
    CFStringRef str = g(string, "theString");
    if(!CFStringGetCStringPtr(str, kCFStringEncodingUTF8))
        free((char *) utf);
}

jint impl_RegisterNatives(JNIEnv *env, jclass clazz, const JNINativeMethod *methods, jint nMethods) {
    while(nMethods--) {
        notice("NATIVE: %s[%s] = %p", methods->name, methods->signature, methods->fnPtr);
        CFStringRef name = CFStringCreateWithFormat(NULL, NULL, CFSTR("%s[%s]"), methods->name, methods->signature);
        notice("clazz = %p", clazz);
        CFDictionarySetValue(clazz, name, RawPtrCreate(methods->fnPtr));
        methods++;
    }
    return 0;
}

// todo: make this part of multistuff
jintArray impl_NewIntArray(JNIEnv *env, jsize len) {
    notice("newIntArray %d", len);
    void *ret = RawDataCreate(len * sizeof(int));
    memset(RawDataGetPtr(ret), 0xfe, RawDataGetSize(ret));
    return new_jobject(ret);
}

jint *impl_GetIntArrayElements(JNIEnv *env, jintArray array, jboolean *isCopy) {
    if(isCopy) *isCopy = false;
    return RawDataGetPtr(array);
}

void impl_ReleaseIntArrayElements(JNIEnv *env, jintArray array, jint *elems, jint mode) {
    // We never copy
    _assert(mode != JNI_ABORT);
}

// VM (aka JNIInvokeInterface)

jint impl_GetEnv(JavaVM *this, void **penv, jint version) {
    _assert(version == JNI_VERSION_1_4);
    *penv = &env;
    return 0;
}

jint impl_AttachCurrentThread(JavaVM *this, void **penv, void *args) {
    *penv = &env;
    return 0;
}

jint impl_DetachCurrentThread(JavaVM *this) {
    return 0;
}

extern void init_classes();

void init_jni() {
    pattern(&env_functions, 0xf0f00000, sizeof(env_functions));
    env_functions.GetJavaVM = stub(impl_GetJavaVM);
    env_functions.NewGlobalRef = stub(impl_NewGlobalRef);
    env_functions.DeleteGlobalRef = stub(impl_DeleteGlobalRef);

    {
        void *x = stub(impl_GetXID);
        env_functions.GetMethodID = x;
        env_functions.GetStaticMethodID = x;
        env_functions.GetFieldID = x;
        env_functions.GetStaticFieldID = x;
    }

    env_functions.NewStringUTF = stub(impl_NewStringUTF);
    env_functions.GetStringUTFChars = stub(impl_GetStringUTFChars);
    env_functions.ReleaseStringUTFChars = stub(impl_ReleaseStringUTFChars);

#include "multistuff.settings.h"

    env_functions.PushLocalFrame = stub(impl_PushLocalFrame);
    env_functions.PopLocalFrame = stub(impl_PopLocalFrame);
    env_functions.FindClass = stub(impl_FindClass);
    env_functions.ExceptionCheck = stub(impl_ExceptionCheck);
    env_functions.GetObjectClass = stub(impl_GetObjectClass);
    env_functions.RegisterNatives = stub(impl_RegisterNatives);
    env_functions.NewIntArray = stub(impl_NewIntArray);
    env_functions.GetIntArrayElements = stub(impl_GetIntArrayElements);
    env_functions.ReleaseIntArrayElements = stub(impl_ReleaseIntArrayElements);
    env = &env_functions;

    pattern(&vm_functions, 0xfdfd0000, sizeof(vm_functions));
    vm_functions.GetEnv = stub(impl_GetEnv);
    vm_functions.AttachCurrentThread = stub(impl_AttachCurrentThread);
    vm_functions.DetachCurrentThread = stub(impl_DetachCurrentThread);
    vm = &vm_functions;

    init_classes();
}

void *JNI_OnLoad_ptr;
void do_jni_onload() {
    extern void do_jni_onload(void *);

    notice("calling JNI_OnLoad...");
    if(JNI_OnLoad_ptr) {
        jint version = ((jint (*)(JavaVM *, void *)) JNI_OnLoad_ptr)(&vm, NULL);
        notice("do_jni_onload: version = %x", (unsigned int) version);
    } else {
        notice("do_jni_onload: no JNI_OnLoad ...");
    }

}

