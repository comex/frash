#pragma once
#define _jobject __CFDictionary
#include "jni.h"
#include "npapi.h"
#include <CoreFoundation/CoreFoundation.h>

extern jobject new_jobject(CFMutableDictionaryRef properties);
extern void do_jni_onload();

extern jobject android_context;
extern JNIEnv env;
extern void init_jni();
extern void *JNI_OnLoad_ptr;
extern void do_jni_surface_created(NPP, jobject);
