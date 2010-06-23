import re
q = open('multistuff.settings.h', 'w')
s = ''
s += ('''#include "myjni.h"
#include <CoreFoundation/CoreFoundation.h>
#include "common.h"
#include <assert.h>
''')
s += ('''jboolean typeRefToBoolean(CFTypeRef val) {
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
''')

things = {
    'boolean': None,
    'object': None,
    'byte': 'kCFNumberSInt8Type',
    'char': 'kCFNumberSInt16Type',
    'short': 'kCFNumberSInt16Type',
    'int': 'kCFNumberLongType',
    'long': 'kCFNumberLongLongType',
    'float': 'kCFNumberFloatType',
    'double': 'kCFNumberDoubleType',
}

def impl(x, *alts):
    q.write('{\n')
    q.write('    void *x = stub(impl_%s);\n' % x)
    for a in [x] + list(alts):
        q.write('    env_functions.%s = x;\n' % a)
    q.write('}\n')

for jname, cfname in things.items():
    if cfname is not None:
        s += ('''j%s typeRefTo%s(CFTypeRef val) {
    j%s ret;
    _assert(CFNumberGetValue((CFNumberRef) val, %s, &ret));
    return ret;
}
''' % (jname, jname.capitalize(), jname, cfname))
        s += ('''CFTypeRef %sToTypeRef(j%s val) {
    return CFNumberCreate(NULL, %s, &val);
}
''' % (jname, jname, cfname))
    
    # GetXField
    # SetXField
    # GetStaticXField
    # SetStaticXField
    # CallXMethod[,V,A]
    # CallNonvirtualXMethod[,V,A]
    # CallStaticXMethod[,V,A]

    s += ('struct method_%s { j%s (*v)(jobject, va_list); j%s (*a)(jobject, const jvalue *); };\n' % (jname, jname, jname))

    s += ('''j%s impl_Call%sMethodA(JNIEnv *env, jobject obj, jmethodID methodID, const jvalue *args) {
    const void *meth = CFDictionaryGetValue(obj, (CFStringRef) methodID);
    if(!meth) { log("Unknown method (A): %%@", methodID); _abort(); }
    struct method_%s *st;
    _assert(st = RawDataGetPtr(meth));
    if(!st->a) { log("No A impl: %%@", methodID); _abort(); }
    return st->a(obj, args);
}
''' % (jname, jname.capitalize(), jname))
    if jname == 'object':
        impl('CallObjectMethodA', 'CallNonvirtualObjectMethodA', 'CallStaticObjectMethodA', 'NewObjectA')
    else:
        impl('Call%sMethodA' % jname.capitalize(), 'CallNonvirtual%sMethodA' % jname.capitalize(), 'CallStatic%sMethodA' % jname.capitalize())

    s += ('''j%s impl_Call%sMethodV(JNIEnv *env, jobject obj, jmethodID methodID, va_list args) {
    const void *meth = CFDictionaryGetValue(obj, (CFStringRef) methodID);
    if(!meth) { log("Unknown method (V): %%@", methodID); _abort(); }
    struct method_%s *st;
    _assert(st = RawDataGetPtr(meth));
    if(!st->v) { log("No V impl: %%@", methodID); _abort(); }
    return st->v(obj, args);
}
''' % (jname, jname.capitalize(), jname))
    if jname == 'object':
        impl('CallObjectMethodV', 'CallNonvirtualObjectMethodV', 'CallStaticObjectMethodV', 'NewObjectV')
    else:
        impl('Call%sMethodV' % jname.capitalize(), 'CallNonvirtual%sMethodV' % jname.capitalize(), 'CallStatic%sMethodV' % jname.capitalize())

    s += ('''j%s impl_Call%sMethod(JNIEnv *env, jobject obj, jmethodID methodID, ...) {
    va_list args;
    va_start(args, methodID);
    j%s ret = impl_Call%sMethodV(env, obj, methodID, args);
    va_end(args);
    return ret;
}
''' % (jname, jname.capitalize(), jname, jname.capitalize()))
    if jname == 'object':
        impl('CallObjectMethod', 'CallNonvirtualObjectMethod', 'CallStaticObjectMethod', 'NewObject')
    else:
        impl('Call%sMethod' % jname.capitalize(), 'CallNonvirtual%sMethod' % jname.capitalize(), 'CallStatic%sMethod' % jname.capitalize())
    
    
    s += ('''void impl_Set%sField(JNIEnv *env, jobject obj, jfieldID fieldID, j%s val) {
    CFTypeRef ref = %sToTypeRef(val);
    CFDictionarySetValue(obj, (CFStringRef) fieldID, ref);
    CFRelease(ref);
}
''' % (jname.capitalize(), jname, jname))
    impl('Set%sField' % jname.capitalize(), 'SetStatic%sField' % jname.capitalize())
    
    s += ('''j%s impl_Get%sField(JNIEnv *env, jobject obj, jfieldID fieldID) {
        CFTypeRef ref = CFDictionaryGetValue(obj, (CFStringRef) fieldID);
        if(!ref) { log("Unknown property: %%@", fieldID); _abort(); }
        return typeRefTo%s(ref);
}
''' % (jname, jname.capitalize(), jname.capitalize()))
    impl('Get%sField' % jname.capitalize(), 'GetStatic%sField' % jname.capitalize())

    if jname == 'object': continue
    



s += ('''
struct method_void { void (*v)(jobject, va_list); void (*a)(jobject, const     jvalue *); };
void impl_CallVoidMethodA(JNIEnv *env, jobject obj, jmethodID methodID, const    jvalue *args) {
    const void *meth = CFDictionaryGetValue(obj, (CFStringRef)          
methodID);
    if(!meth) { log("Unknown method (A): %%@", methodID); _abort(); }
    struct method_void *st; 
    _assert(st = RawDataGetPtr(meth));
    if(!st->a) { log("No A impl: %%@", methodID); _abort(); }
    st->a(obj, args);
}
void impl_CallVoidMethodV(JNIEnv *env, jobject obj, jmethodID methodID, va_list  args) {
    const void *meth = CFDictionaryGetValue(obj, (CFStringRef)          
methodID);
    if(!meth) { log("Unknown method (V): %%@", methodID); _abort(); }
    struct method_void *st;
    _assert(st = RawDataGetPtr(meth));
    if(!st->v) { log("No V impl: %%@", methodID); _abort(); }
    st->v(obj, args);
}
void impl_CallVoidMethod(JNIEnv *env, jobject obj, jmethodID methodID, ...) {
    va_list args;
    va_start(args, methodID);
    impl_CallVoidMethodV(env, obj, methodID, args);
    va_end(args);
}

''' % ())
impl('CallVoidMethod', 'CallNonvirtualVoidMethod', 'CallStaticVoidMethod')
impl('CallVoidMethodA', 'CallNonvirtualVoidMethodA', 'CallStaticVoidMethodA')
impl('CallVoidMethodV', 'CallNonvirtualVoidMethodV', 'CallStaticVoidMethodV')

f = open('multistuff.c', 'w')
f.write(s)
f.close()

g = open('multistuff.h', 'w')
for stuff in re.findall(re.compile('^.*{$', re.M), s):
    g.write(stuff[:-2] + ';\n')


