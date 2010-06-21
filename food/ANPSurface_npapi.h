#ifndef ANPSurface_npapi_H
#define ANPSurface_npapi_H

#include "android_npapi.h"
#include <jni.h>

struct ANPSurfaceInterfaceV0 : ANPInterface {
    /** Locks the surface from manipulation by other threads and provides a bitmap
        to be written to.  The dirtyRect param specifies which portion of the
        bitmap will be written to.  If the dirtyRect is NULL then the entire
        surface will be considered dirty.  If the lock was successful the function
        will return true and the bitmap will be set to point to a valid bitmap.
        If not the function will return false and the bitmap will be set to NULL.
     */
    bool (*lock)(JNIEnv* env, jobject surface, ANPBitmap* bitmap, ANPRectI* dirtyRect);
    /** Given a locked surface handle (i.e. result of a successful call to lock)
        the surface is unlocked and the contents of the bitmap, specifically
        those inside the dirtyRect are written to the screen.
     */
    void (*unlock)(JNIEnv* env, jobject surface);
};

#endif //ANPSurface_npapi_H
