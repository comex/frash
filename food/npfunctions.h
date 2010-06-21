/*
 * Copyright (C) 2007 Apple Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */
#ifndef NPFUNCTIONS_H
#define NPFUNCTIONS_H


#include "npruntime.h"
#include "npapi.h"
#if defined(ANDROID_PLUGINS)
#include "jni.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if defined(XP_WIN)
#define EXPORTED_CALLBACK(_type, _name) _type (__stdcall * _name)
#else
#define EXPORTED_CALLBACK(_type, _name) _type (* _name)
#endif

typedef NPError (*NPN_GetURLNotifyProcPtr)(NPP instance, const char* URL, const char* window, void* notifyData);
typedef NPError (*NPN_PostURLNotifyProcPtr)(NPP instance, const char* URL, const char* window, uint32 len, const char* buf, NPBool file, void* notifyData);
typedef NPError (*NPN_RequestReadProcPtr)(NPStream* stream, NPByteRange* rangeList);
typedef NPError (*NPN_NewStreamProcPtr)(NPP instance, NPMIMEType type, const char* window, NPStream** stream);
typedef int32 (*NPN_WriteProcPtr)(NPP instance, NPStream* stream, int32 len, void* buffer);
typedef NPError (*NPN_DestroyStreamProcPtr)(NPP instance, NPStream* stream, NPReason reason);
typedef void (*NPN_StatusProcPtr)(NPP instance, const char* message);
typedef const char*(*NPN_UserAgentProcPtr)(NPP instance);
typedef void* (*NPN_MemAllocProcPtr)(uint32 size);
typedef void (*NPN_MemFreeProcPtr)(void* ptr);
typedef uint32 (*NPN_MemFlushProcPtr)(uint32 size);
typedef void (*NPN_ReloadPluginsProcPtr)(NPBool reloadPages);
typedef NPError (*NPN_GetValueProcPtr)(NPP instance, NPNVariable variable, void *ret_value);
typedef NPError (*NPN_SetValueProcPtr)(NPP instance, NPPVariable variable, void *value);
typedef void (*NPN_InvalidateRectProcPtr)(NPP instance, NPRect *rect);
typedef void (*NPN_InvalidateRegionProcPtr)(NPP instance, NPRegion region);
typedef void (*NPN_ForceRedrawProcPtr)(NPP instance);
typedef NPError (*NPN_GetURLProcPtr)(NPP instance, const char* URL, const char* window);
typedef NPError (*NPN_PostURLProcPtr)(NPP instance, const char* URL, const char* window, uint32 len, const char* buf, NPBool file);
typedef void* (*NPN_GetJavaEnvProcPtr)(void);
typedef void* (*NPN_GetJavaPeerProcPtr)(NPP instance);
typedef void  (*NPN_PushPopupsEnabledStateProcPtr)(NPP instance, NPBool enabled);
typedef void  (*NPN_PopPopupsEnabledStateProcPtr)(NPP instance);
typedef void (*NPN_PluginThreadAsyncCallProcPtr)(NPP npp, void (*func)(void *), void *userData);
typedef NPError (*NPN_GetValueForURLProcPtr)(NPP npp, NPNURLVariable variable, const char* url, char** value, uint32* len);
typedef NPError (*NPN_SetValueForURLProcPtr)(NPP npp, NPNURLVariable variable, const char* url, const char* value, uint32 len);
typedef NPError (*NPN_GetAuthenticationInfoProcPtr)(NPP npp, const char* protocol, const char* host, int32 port, const char* scheme, const char *realm, char** username, uint32* ulen, char** password, uint32* plen);

typedef uint32 (*NPN_ScheduleTimerProcPtr)(NPP npp, uint32 interval, NPBool repeat, void (*timerFunc)(NPP npp, uint32 timerID));
typedef void (*NPN_UnscheduleTimerProcPtr)(NPP npp, uint32 timerID);
typedef NPError (*NPN_PopUpContextMenuProcPtr)(NPP instance, NPMenu* menu);
typedef NPBool (*NPN_ConvertPointProcPtr)(NPP npp, double sourceX, double sourceY, NPCoordinateSpace sourceSpace, double *destX, double *destY, NPCoordinateSpace destSpace);

typedef void (*NPN_ReleaseVariantValueProcPtr) (NPVariant *variant);

typedef NPIdentifier (*NPN_GetStringIdentifierProcPtr) (const NPUTF8 *name);
typedef void (*NPN_GetStringIdentifiersProcPtr) (const NPUTF8 **names, int32_t nameCount, NPIdentifier *identifiers);
typedef NPIdentifier (*NPN_GetIntIdentifierProcPtr) (int32_t intid);
typedef int32_t (*NPN_IntFromIdentifierProcPtr) (NPIdentifier identifier);
typedef bool (*NPN_IdentifierIsStringProcPtr) (NPIdentifier identifier);
typedef NPUTF8 *(*NPN_UTF8FromIdentifierProcPtr) (NPIdentifier identifier);

typedef NPObject* (*NPN_CreateObjectProcPtr) (NPP, NPClass *aClass);
typedef NPObject* (*NPN_RetainObjectProcPtr) (NPObject *obj);
typedef void (*NPN_ReleaseObjectProcPtr) (NPObject *obj);
typedef bool (*NPN_InvokeProcPtr) (NPP npp, NPObject *obj, NPIdentifier methodName, const NPVariant *args, unsigned argCount, NPVariant *result);
typedef bool (*NPN_InvokeDefaultProcPtr) (NPP npp, NPObject *obj, const NPVariant *args, unsigned argCount, NPVariant *result);
typedef bool (*NPN_EvaluateProcPtr) (NPP npp, NPObject *obj, NPString *script, NPVariant *result);
typedef bool (*NPN_GetPropertyProcPtr) (NPP npp, NPObject *obj, NPIdentifier  propertyName, NPVariant *result);
typedef bool (*NPN_SetPropertyProcPtr) (NPP npp, NPObject *obj, NPIdentifier  propertyName, const NPVariant *value);
typedef bool (*NPN_HasPropertyProcPtr) (NPP, NPObject *npobj, NPIdentifier propertyName);
typedef bool (*NPN_HasMethodProcPtr) (NPP npp, NPObject *npobj, NPIdentifier methodName);
typedef bool (*NPN_RemovePropertyProcPtr) (NPP npp, NPObject *obj, NPIdentifier propertyName);
typedef void (*NPN_SetExceptionProcPtr) (NPObject *obj, const NPUTF8 *message);
typedef bool (*NPN_EnumerateProcPtr) (NPP npp, NPObject *npobj, NPIdentifier **identifier, uint32_t *count);
typedef bool (*NPN_ConstructProcPtr)(NPP npp, NPObject* obj, const NPVariant *args, uint32_t argCount, NPVariant *result);    

typedef NPError (*NPP_NewProcPtr)(NPMIMEType pluginType, NPP instance, uint16 mode, int16 argc, char* argn[], char* argv[], NPSavedData* saved);
typedef NPError (*NPP_DestroyProcPtr)(NPP instance, NPSavedData** save);
typedef NPError (*NPP_SetWindowProcPtr)(NPP instance, NPWindow* window);
typedef NPError (*NPP_NewStreamProcPtr)(NPP instance, NPMIMEType type, NPStream* stream, NPBool seekable, uint16* stype);
typedef NPError (*NPP_DestroyStreamProcPtr)(NPP instance, NPStream* stream, NPReason reason);
typedef void (*NPP_StreamAsFileProcPtr)(NPP instance, NPStream* stream, const char* fname);
typedef int32 (*NPP_WriteReadyProcPtr)(NPP instance, NPStream* stream);
typedef int32 (*NPP_WriteProcPtr)(NPP instance, NPStream* stream, int32_t offset, int32_t len, void* buffer);
typedef void (*NPP_PrintProcPtr)(NPP instance, NPPrint* platformPrint);
typedef int16 (*NPP_HandleEventProcPtr)(NPP instance, void* event);
typedef void (*NPP_URLNotifyProcPtr)(NPP instance, const char* URL, NPReason reason, void* notifyData);
typedef NPError (*NPP_GetValueProcPtr)(NPP instance, NPPVariable variable, void *ret_value);
typedef NPError (*NPP_SetValueProcPtr)(NPP instance, NPNVariable variable, void *value);

typedef void *(*NPP_GetJavaClassProcPtr)(void);
typedef void* JRIGlobalRef; //not using this right now

typedef struct _NPNetscapeFuncs {
    uint16 size; // 0
    uint16 version; // 2
    
    NPN_GetURLProcPtr geturl; // 4
    NPN_PostURLProcPtr posturl; // 8
    NPN_RequestReadProcPtr requestread; // c
    NPN_NewStreamProcPtr newstream; // 10
    NPN_WriteProcPtr write; // 14 
    NPN_DestroyStreamProcPtr destroystream; // 18
    NPN_StatusProcPtr status; // 1c 
    NPN_UserAgentProcPtr uagent; // 20
    NPN_MemAllocProcPtr memalloc; // 24
    NPN_MemFreeProcPtr memfree; // 28
    NPN_MemFlushProcPtr memflush; // 2c
    NPN_ReloadPluginsProcPtr reloadplugins; // 30
    NPN_GetJavaEnvProcPtr getJavaEnv; // 34
    NPN_GetJavaPeerProcPtr getJavaPeer; // 38
    NPN_GetURLNotifyProcPtr geturlnotify; // 3c 
    NPN_PostURLNotifyProcPtr posturlnotify; // 40
    NPN_GetValueProcPtr getvalue; // 44
    NPN_SetValueProcPtr setvalue; // 48
    NPN_InvalidateRectProcPtr invalidaterect; // 4c
    NPN_InvalidateRegionProcPtr invalidateregion; // 50
    NPN_ForceRedrawProcPtr forceredraw; // 54
    
    NPN_GetStringIdentifierProcPtr getstringidentifier; // 58
    NPN_GetStringIdentifiersProcPtr getstringidentifiers; // 5c
    NPN_GetIntIdentifierProcPtr getintidentifier; // 60
    NPN_IdentifierIsStringProcPtr identifierisstring; // 64
    NPN_UTF8FromIdentifierProcPtr utf8fromidentifier; // 68
    NPN_IntFromIdentifierProcPtr intfromidentifier; // 6c
    NPN_CreateObjectProcPtr createobject; // 70
    NPN_RetainObjectProcPtr retainobject; // 74
    NPN_ReleaseObjectProcPtr releaseobject;  // 78
    NPN_InvokeProcPtr invoke; // 7c
    NPN_InvokeDefaultProcPtr invokeDefault; // 80
    NPN_EvaluateProcPtr evaluate; // 84
    NPN_GetPropertyProcPtr getproperty; // 88
    NPN_SetPropertyProcPtr setproperty; // 8c
    NPN_RemovePropertyProcPtr removeproperty; // 90
    NPN_HasPropertyProcPtr hasproperty; // 94
    NPN_HasMethodProcPtr hasmethod; // 98
    NPN_ReleaseVariantValueProcPtr releasevariantvalue; // 9c
    NPN_SetExceptionProcPtr setexception; // a0
    NPN_PushPopupsEnabledStateProcPtr pushpopupsenabledstate; // a4
    NPN_PopPopupsEnabledStateProcPtr poppopupsenabledstate; // a8
    NPN_EnumerateProcPtr enumerate; // ac
    NPN_PluginThreadAsyncCallProcPtr pluginthreadasynccall; // b0
    NPN_ConstructProcPtr construct; // b4
    NPN_GetValueForURLProcPtr getvalueforurl; // b8
    NPN_SetValueForURLProcPtr setvalueforurl; // bc
    NPN_GetAuthenticationInfoProcPtr getauthenticationinfo; // c0
    NPN_ScheduleTimerProcPtr scheduletimer; // c4
    NPN_UnscheduleTimerProcPtr unscheduletimer; // c8
    NPN_PopUpContextMenuProcPtr popupcontextmenu; // cc
    NPN_ConvertPointProcPtr convertpoint; // d0
    // d4
} NPNetscapeFuncs;

typedef struct _NPPluginFuncs {
    uint16 size;
    uint16 version;
    NPP_NewProcPtr newp;
    NPP_DestroyProcPtr destroy;
    NPP_SetWindowProcPtr setwindow;
    NPP_NewStreamProcPtr newstream;
    NPP_DestroyStreamProcPtr destroystream;
    NPP_StreamAsFileProcPtr asfile;
    NPP_WriteReadyProcPtr writeready;
    NPP_WriteProcPtr write;
    NPP_PrintProcPtr print;
    NPP_HandleEventProcPtr event;
    NPP_URLNotifyProcPtr urlnotify;
    JRIGlobalRef javaClass;
    NPP_GetValueProcPtr getvalue;
    NPP_SetValueProcPtr setvalue;
} NPPluginFuncs;

typedef EXPORTED_CALLBACK(NPError, NP_GetEntryPointsFuncPtr)(NPPluginFuncs*);
typedef EXPORTED_CALLBACK(void, NPP_ShutdownProcPtr)(void);    

#if defined(XP_MACOSX)
typedef void (*BP_CreatePluginMIMETypesPreferencesFuncPtr)(void);
typedef NPError (*MainFuncPtr)(NPNetscapeFuncs*, NPPluginFuncs*, NPP_ShutdownProcPtr*);
#endif

#if defined(XP_UNIX)
typedef EXPORTED_CALLBACK(NPError, NP_InitializeFuncPtr)(NPNetscapeFuncs*, NPPluginFuncs*);
typedef EXPORTED_CALLBACK(char*, NP_GetMIMEDescriptionFuncPtr)(void);
#elif defined(ANDROID_PLUGINS)
typedef EXPORTED_CALLBACK(NPError, NP_InitializeFuncPtr)(NPNetscapeFuncs*, NPPluginFuncs*, JNIEnv *java_environment, jobject application_context);
typedef EXPORTED_CALLBACK(char*, NP_GetMIMEDescriptionFuncPtr)(void);
#else
typedef EXPORTED_CALLBACK(NPError, NP_InitializeFuncPtr)(NPNetscapeFuncs*);
#endif

#ifdef __cplusplus
}
#endif

#endif
