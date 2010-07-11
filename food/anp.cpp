#define ANDROID 1
#include <stdlib.h>
#define IOSFC_BUILDING_IOSFC
#include <CoreFoundation/CoreFoundation.h>
#include <IOSurface/IOSurface.h>
#include <CoreGraphics/CoreGraphics.h>
#include <jni.h>
#include "npapi.h"
#include "android_npapi.h"
#include "ANPSystem_npapi.h"
#include "ANPSurface_npapi.h"
#include <math.h>
#include <mach/mach_port.h>
#define AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER
#include <CoreText/CoreText.h>
#include <CoreText/CTStringAttributes.h>
#include "ANPSurface_npapi.h"
#include "common.h"
#include <AudioToolbox/AudioQueue.h>
#include <pthread.h>

static CGContextRef randomctx;
__attribute__((constructor))
void init_randomctx() {
    randomctx = CGBitmapContextCreate(malloc(50*50*4), 50, 50, 8, 50*4, CGColorSpaceCreateDeviceRGB(),  kCGBitmapByteOrder32Big | kCGImageAlphaPremultipliedLast);
    _assert(randomctx);
}


CGRect rectFromRectF(const ANPRectF rectf) {
    return CGRectMake(rectf.left, movie_h - rectf.bottom, rectf.right - rectf.left, rectf.bottom - rectf.top);
}

CGRect rectFromRectI(const ANPRectI recti) {
    return CGRectMake(recti.left, movie_h - recti.bottom, recti.right - recti.left, recti.bottom - recti.top);
}

ANPRectF rectFFromRect(const CGRect rect) {
    ANPRectF result;
    result.left   = rect.origin.x;
    result.right  = rect.origin.x + rect.size.width;
    result.bottom = movie_w - rect.origin.y;
    result.top    = result.top - rect.size.height;
    return result;
}

struct ANPBitmap bitmapFromIOSurface(IOSurfaceRef surface) {
    struct ANPBitmap result;
    result.baseAddr = IOSurfaceGetBaseAddress(surface);
    result.width = IOSurfaceGetWidth(surface);
    result.height = IOSurfaceGetHeight(surface);
    result.rowBytes = IOSurfaceGetBytesPerRow(surface);
    result.format = kRGBA_8888_ANPBitmapFormat; // XXX
    return result;
}

struct ANPTypeface {
    CFStringRef name;
    ANPTypefaceStyle style;
    int refcount;
};
    

// ANPBitmapInterfaceV0

bool bitmap_impl_getPixelPacking(ANPBitmapFormat, ANPPixelPacking* packing) {
    // XXX
    notice("bitmap_impl_getPixelPacking");
    _abort();
    //return true;
}


// ANPLogInterfaceV0

void log_impl_log(ANPLogType type, const char format[], ...) {
    switch(type) {
    case kDebug_ANPLogType:
        fprintf(stderr, "[DBG] "); break;
    case kWarning_ANPLogType:
        fprintf(stderr, "[WRN] "); break;
    case kError_ANPLogType:
        fprintf(stderr, "[ERR] "); break;
    default:
        fprintf(stderr, "[???] "); break;
    }
    va_list ap;
    va_start(ap, format);
    vfprintf(stderr, format, ap);
    va_end(ap);
}


// ANPAudioTrackInterfaceV0

struct ANPAudioTrack {
    ANPAudioCallbackProc callbackProc;
    AudioQueueRef aq;
    AudioQueueBufferRef buf1, buf2, buf3;
    int32_t channelCount;
    ANPSampleFormat sampleFormat;
    void *user;
    bool stopped;
    pthread_mutex_t mu;
};

void aqtry_(OSStatus ret, const char *name) {
    if(!ret) return;
    err("Audio error in %s:", name);
    err("%d", (int) ret);
    _abort();
}
#define aqtry(x) aqtry_(x, #x)


void trackCallbackProc (
   void                 *inUserData,
   AudioQueueRef        inAQ,
   AudioQueueBufferRef  inBuffer
) {
    notice("trackCallbackProc");
    ANPAudioTrack *result = (ANPAudioTrack *) inUserData;
    pthread_mutex_lock(&result->mu);
    ANPAudioBuffer buffer;
    buffer.channelCount = result->channelCount;
    buffer.format = result->sampleFormat;
    buffer.bufferData = inBuffer->mAudioData;
    buffer.size = inBuffer->mAudioDataBytesCapacity;
    /* call proc */
    result->callbackProc(kMoreData_ANPAudioEvent, result->user, &buffer);
    inBuffer->mAudioDataByteSize = buffer.size;
    notice("%p potential: %d actual: %d", inBuffer, inBuffer->mAudioDataBytesCapacity, inBuffer->mAudioDataByteSize);
    aqtry(AudioQueueEnqueueBuffer(result->aq, inBuffer, 0, NULL));
    pthread_mutex_unlock(&result->mu);
}

/** Create a new audio track, or NULL on failure. The track is initially in
    the stopped state and therefore ANPAudioCallbackProc will not be called
    until the track is started.
 */
ANPAudioTrack*  audiotrack_impl_newTrack(uint32_t sampleRate,    // sampling rate in Hz
                            ANPSampleFormat sampleFormat,
                            int channelCount,       // MONO=1, STEREO=2
                            ANPAudioCallbackProc callbackProc,
                            void* user) {
    notice("channelCount: %d", channelCount);
    notice("sampleFormat: %d", sampleFormat);
    ANPAudioTrack *result = new ANPAudioTrack;
    result->callbackProc = callbackProc;
    result->channelCount = channelCount;
    result->sampleFormat = sampleFormat;
    result->user = user;
    struct AudioStreamBasicDescription fmt;

    int bits;
    switch(sampleFormat) {
    case kPCM16Bit_ANPSampleFormat: bits = 16; break;
    case kPCM8Bit_ANPSampleFormat:  bits =  8; break;
    default:                        _abort();
    }
    FillOutASBDForLPCM(fmt, sampleRate, channelCount, bits, bits, false, false, false);
    aqtry(AudioQueueNewOutput(&fmt, trackCallbackProc, result, NULL, NULL, 0, &result->aq));
    aqtry(AudioQueueAllocateBuffer(result->aq, 65536, &result->buf1));
    aqtry(AudioQueueAllocateBuffer(result->aq, 65536, &result->buf2));
    aqtry(AudioQueueAllocateBuffer(result->aq, 65536, &result->buf3));
    result->stopped = true;
    pthread_mutex_init(&result->mu, NULL);
    return result;
}
/** Deletes a track that was created using newTrack.  The track can be
    deleted in any state and it waits for the ANPAudioCallbackProc thread
    to exit before returning.
 */
void audiotrack_impl_deleteTrack(ANPAudioTrack *track) {
    notice("audiotrack_impl_deleteTrack");
    aqtry(AudioQueueDispose(track->aq, false));
    delete track;
}

void *audiotrack_starter(void *track_) {
    ANPAudioTrack *track = (ANPAudioTrack *) track_;
    trackCallbackProc(track, track->aq, track->buf1);
    notice("OK TRACK CALLBACK IS DONE");
    trackCallbackProc(track, track->aq, track->buf2);
    //trackCallbackProc(track, track->aq, track->buf3);
    return NULL;
}

void audiotrack_impl_start(ANPAudioTrack *track) {
    notice("audiotrack_impl_start %p", track->aq);
    //return;
    track->stopped = false;
    pthread_t thread;
    pthread_create(&thread, NULL, audiotrack_starter, track);
    aqtry(AudioQueueStart(track->aq, NULL));
    
}
void audiotrack_impl_pause(ANPAudioTrack *track) {
    notice("audiotrack_impl_pause %p", track);
    track->stopped = true;
    aqtry(AudioQueuePause(track->aq));
}
void audiotrack_impl_stop(ANPAudioTrack *track) {
    notice("audiotrack_impl_stop %p", track);
    track->stopped = true;
    aqtry(AudioQueueStop(track->aq, false));
}
/** Returns true if the track is not playing (e.g. pause or stop was called,
    or start was never called.
 */
bool audiotrack_impl_isStopped(ANPAudioTrack *track) {
    return track->stopped;
}


// ANPPaintInterfaceV0

struct ANPPaint {
    ANPPaintJoin join;
    ANPPaintFlags flags;
    ANPPaintStyle style;
    ANPPaintCap cap;
    // XXX
    ANPPaintAlign align;
    ANPTextEncoding encoding;
    ANPTypeface *typeface;
    ANPTypefaceStyle typefaceStyle;
    float textSkewX, textScaleX, textSize;
    //
    ANPColor color;
    float strokeWidth, strokeMiter;
};

void typeface_impl_ref(ANPTypeface *typeface);
void typeface_impl_unref(ANPTypeface *typeface);

/** Return a new paint object, which holds all of the color and style
    attributes that affect how things (geometry, text, bitmaps) are drawn
    in a ANPCanvas.

    The paint that is returned is not tied to any particular plugin
    instance, but it must only be accessed from one thread at a time.
 */
ANPPaint*   paint_impl_newPaint() {
    ANPPaint *result = new ANPPaint;
    memset(result, 0, sizeof(ANPPaint));
    result->textScaleX = 1;
    result->textSize = 12;
    return result;
}

void        paint_impl_deletePaint(ANPPaint *paint) {
    if(paint->typeface) typeface_impl_unref(paint->typeface);
    delete paint;
}

ANPPaintFlags paint_impl_getFlags(const ANPPaint *paint) {
    notice("%s: flags = %d", __func__, paint->flags);
    return paint->flags;
}

void        paint_impl_setFlags(ANPPaint *paint, ANPPaintFlags flags) {
    notice("%s: flags => %d", __func__, flags);
    paint->flags = flags;
}

ANPColor    paint_impl_getColor(const ANPPaint *paint) {
    return paint->color;
}
void        paint_impl_setColor(ANPPaint *paint, ANPColor color) {
    paint->color = color;
}

ANPPaintStyle paint_impl_getStyle(const ANPPaint *paint) {
    return paint->style;
}
void        paint_impl_setStyle(ANPPaint *paint, ANPPaintStyle style) {
    paint->style = style;
}

float       paint_impl_getStrokeWidth(const ANPPaint *paint) {
    return paint->strokeWidth;
}
float       paint_impl_getStrokeMiter(const ANPPaint *paint) {
    return paint->strokeMiter;
}
ANPPaintCap paint_impl_getStrokeCap(const ANPPaint *paint) {
    return paint->cap;
}
ANPPaintJoin paint_impl_getStrokeJoin(const ANPPaint *paint) {
    return paint->join;
}
void        paint_impl_setStrokeWidth(ANPPaint *paint, float width) {
    paint->strokeWidth = width;
}
void        paint_impl_setStrokeMiter(ANPPaint *paint, float miter) {
    paint->strokeMiter = miter;
}
void        paint_impl_setStrokeCap(ANPPaint *paint, ANPPaintCap cap) {
    paint->cap = cap;
}
void        paint_impl_setStrokeJoin(ANPPaint *paint, ANPPaintJoin join) {
    paint->join = join;
}

ANPTextEncoding paint_impl_getTextEncoding(const ANPPaint *paint) {
    return paint->encoding;
}
ANPPaintAlign paint_impl_getTextAlign(const ANPPaint *paint) {
    return paint->align;
}
float       paint_impl_getTextSize(const ANPPaint *paint) {
    return paint->textSize;
}
float       paint_impl_getTextScaleX(const ANPPaint *paint) {
    return paint->textScaleX;
}
float       paint_impl_getTextSkewX(const ANPPaint *paint) {
    return paint->textSkewX;
}
void        paint_impl_setTextEncoding(ANPPaint *paint, ANPTextEncoding encoding) {
    paint->encoding = encoding;
}
void        paint_impl_setTextAlign(ANPPaint *paint, ANPPaintAlign align) {
    paint->align = align;
}
void        paint_impl_setTextSize(ANPPaint *paint, float size) {
    paint->textSize = size;
}
void        paint_impl_setTextScaleX(ANPPaint *paint, float scaleX) {
    paint->textScaleX = scaleX;
}
void        paint_impl_setTextSkewX(ANPPaint *paint, float skewX) {
    paint->textSkewX = skewX;
}

/** Return the typeface ine paint, or null if there is none. This does not
    modify the owner count of the returned typeface.
 */
ANPTypeface* paint_impl_getTypeface(const ANPPaint *paint) {
    return paint->typeface;
}

/** Set the paint's typeface. If the paint already had a non-null typeface,
    its owner count is decremented. If the new typeface is non-null, its
    owner count is incremented.
 */
void paint_impl_setTypeface(ANPPaint *paint, ANPTypeface *typeface) {
    if(paint->typeface) typeface_impl_unref(paint->typeface);
    if(typeface) typeface_impl_ref(typeface);
    paint->typeface = typeface;
}



CTFontRef paint_mkfont(const ANPPaint *paint) {
    notice("(paint_mkfont)");
    CGAffineTransform transform = CGAffineTransformMake(1, 0, paint->textSkewX, 1, 0, 0);
    transform = CGAffineTransformScale(transform, paint->textScaleX, 1);
    CTFontRef base = CTFontCreateWithName(paint->typeface->name, paint->textSize, &transform);
    _assert(base);
    CTFontSymbolicTraits traits = 0;
    if(paint->style & kBold_ANPTypefaceStyle)
        traits |= kCTFontBoldTrait;
    if(paint->style & kItalic_ANPTypefaceStyle)
        traits |= kCTFontItalicTrait;
    return base;
    // XXX this returns null?
    CTFontRef result = CTFontCreateCopyWithSymbolicTraits(base, 0.0, NULL, traits, traits);
    CFRelease(base);
    return result;
}

CGColorRef color_getref(ANPColor color);


CFAttributedStringRef paint_mkats(const ANPPaint *paint, const void *text, uint32_t byteLength) {
    CFStringRef string = CFStringCreateWithBytes(NULL, (const UInt8 *) text, byteLength, paint->encoding == kUTF16_ANPTextEncoding ? kCFStringEncodingUTF16 : kCFStringEncodingUTF8, false);
    
    CFMutableDictionaryRef dict = CFDictionaryCreateMutable(NULL, 0, &kCFCopyStringDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
    CFNumberRef num;
    float f;

    f = paint->strokeWidth;
    num = CFNumberCreate(NULL, kCFNumberCGFloatType, &f);
    CFDictionarySetValue(dict, kCTStrokeWidthAttributeName, num);
    CFRelease(num);

    
    CGColorRef color = color_getref(paint->color);
    CFDictionarySetValue(dict, kCTStrokeColorAttributeName, color);
    CFDictionarySetValue(dict, kCTForegroundColorAttributeName, color);
    CGColorRelease(color);

    CTFontRef font = paint_mkfont(paint);
    _assert(font);
    CFDictionarySetValue(dict, kCTFontAttributeName, font);
    CFRelease(font);

    CFAttributedStringRef result = CFAttributedStringCreate(NULL, string, dict);
    CFRelease(dict);
    CFRelease(string);
    return result;
}


/** Return the width of the text. If bounds is not null, return the bounds
    of the text in that rectangle.
 */

float paint_impl_measureText(ANPPaint *paint, const void* text, uint32_t byteLength,
                     ANPRectF* bounds) {
    CTFontRef font = paint_mkfont(paint);
    CFAttributedStringRef ats = paint_mkats(paint, text, byteLength);
    CTLineRef line = CTLineCreateWithAttributedString(ats);

    if(bounds) {
        *bounds = rectFFromRect(CTLineGetImageBounds(line, randomctx));
    }

    float ret = CTLineGetTypographicBounds(line, NULL, NULL, NULL);

    CFRelease(line);
    CFRelease(font);
    return ret;

    /*
    CTFramesetterRef fs = CTFramesetterCreateWithAttributedString(ats);
    CFRange fitRange;
    CGSize size = CTFramesetterSuggestFrameSizeWithConstraints(fs, CFRangeMake(0, 0), NULL, CGSizeMake(CGFLOAT_MAX, CGFLOAT_MAX), &fitRange);
    CFRelease(font);
    CFRelease(ats);
    CFRelease(fs);
    if(bounds) {
        bounds->left = 0;
        bounds->top = 0;
        bounds->right = size.width;
        bounds->bottom = size.height;
    }
    return size.width;
    */
}

/** Return the number of unichars specifed by the text.
    If widths is not null, returns the array of advance widths for each
        unichar.
    If bounds is not null, returns the array of bounds for each unichar.
 */
int paint_impl_getTextWidths(ANPPaint *paint, const void* text, uint32_t byteLength,
                     float widths[], ANPRectF bounds[]) {
    // This is not quite correct, because unichar != glyph.
    // Don't kill me.
    CTFontRef font = paint_mkfont(paint);
    CFAttributedStringRef ats = paint_mkats(paint, text, byteLength);
    CTLineRef line = CTLineCreateWithAttributedString(ats);
   
    CFArrayRef runs = CTLineGetGlyphRuns(line);
    _assert(CFArrayGetCount(runs) == 1);

    CTRunRef run = (CTRunRef) CFArrayGetValueAtIndex(runs, 0);
    int glyphs = CTRunGetGlyphCount(run); 
    
    if(widths) {
        CGSize *buf = (CGSize *) malloc(sizeof(CGSize) * glyphs);
        CTRunGetAdvances(run, CFRangeMake(0, 0), buf);
        for(int i = 0; i < glyphs; i++) {
            widths[i] = buf[i].width;
        }
        free(buf);
    }
    if(bounds) {
        for(int i = 0; i < glyphs; i++) {
            CGRect rect = CTRunGetImageBounds(run, randomctx, CFRangeMake(i, 1));
            bounds[i] = rectFFromRect(rect);
        }
    }

    CFRelease(run);
    CFRelease(line);
    CFRelease(font);
    return glyphs;
}

/** Return in metrics the spacing values for text, respecting the paint's
    typeface and pointsize, and return the spacing between lines
    (descent - ascent + leading). If metrics is NULL, it will be ignored.
 */
float paint_impl_getFontMetrics(ANPPaint *paint, ANPFontMetrics* metrics) {
    CTFontRef font = paint_mkfont(paint);
    CFAttributedStringRef ats = paint_mkats(paint, "AAA", 3);
    CTLineRef line = CTLineCreateWithAttributedString(ats);
    
    CGFloat ascent, descent, leading;
    CTLineGetTypographicBounds(line, &ascent, &descent, &leading);
    if(metrics) {
        metrics->fAscent = ascent;
        metrics->fDescent = descent;
        metrics->fLeading = leading;
    }

    CGRect imageBounds = CTLineGetImageBounds(line, randomctx);
    metrics->fTop = imageBounds.origin.y;
    metrics->fBottom = imageBounds.size.height;

    CFRelease(line);
    CFRelease(font);

    return descent - ascent + leading;
}


// ANPTypefaceInterfaceV0

ANPTypeface* typeface_impl_createFromName(const char name[], ANPTypefaceStyle style) {
    notice("createFromName: %s", name);
    ANPTypeface *result = new ANPTypeface;
    CFStringRef str = CFStringCreateWithCString(NULL, name, kCFStringEncodingASCII);
    result->name = str;
    result->refcount = 1;
    result->style = style;
    return result;
}

ANPTypeface* typeface_impl_createFromTypeface(const ANPTypeface* family,
                                   ANPTypefaceStyle style) {
    ANPTypeface *result = new ANPTypeface;
    result->name = family->name;
    result->refcount = 1;
    result->style = style;
    CFRetain(result->name);
    return result;
}

/** Return the owner count of the typeface. A newly created typeface has an
    owner count of 1. When the owner count is reaches 0, the typeface is
    deleted.
 */
int32_t typeface_impl_getRefCount(const ANPTypeface *typeface) {
    return typeface->refcount;
}

/** Increment the owner count on the typeface
 */
void typeface_impl_ref(ANPTypeface *typeface) {
    typeface->refcount++;
}

/** Decrement the owner count on the typeface. When the count goes to 0,
    the typeface is deleted.
 */
void typeface_impl_unref(ANPTypeface *typeface) {
    if(--typeface->refcount == 0) {
        CFRelease(typeface->name);
        delete typeface;
    }
}

/** Return the style bits for the specified typeface
 */
ANPTypefaceStyle typeface_impl_getStyle(const ANPTypeface *typeface) {
    return typeface->style;
}

/** Some fonts are stored in files. If that is true for the fontID, then
    this returns the byte length of the full file path. If path is not null,
    then the full path is copied into path (allocated by the caller), up to
    length bytes. If index is not null, then it is set to the truetype
    collection index for this font, or 0 if the font is not in a collection.

    Note: getFontPath does not assume that path is a null-terminated string,
    so when it succeeds, it only copies the bytes of the file name and
    nothing else (i.e. it copies exactly the number of bytes returned by the
    function. If the caller wants to treat path[] as a C string, it must be
    sure that it is allocated at least 1 byte larger than the returned size,
    and it must copy in the terminating 0.

    If the fontID does not correspond to a file, then the function returns
    0, and the path and index parameters are ignored.

    @param fontID  The font whose file name is being queried
    @param path    Either NULL, or storage for receiving up to length bytes
                   of the font's file name. Allocated by the caller.
    @param length  The maximum space allocated in path (by the caller).
                   Ignored if path is NULL.
    @param index   Either NULL, or receives the TTC index for this font.
                   If the font is not a TTC, then will be set to 0.
    @return The byte length of th font's file name, or 0 if the font is not
            baked by a file.
 */
int32_t typeface_impl_getFontPath(const ANPTypeface*, char path[], int32_t length,
                       int32_t* index) {
    return 0;
}

/** Return a UTF8 encoded path name for the font directory, or NULL if not
    supported. If returned, this string address will be valid for the life
    of the plugin instance. It will always end with a '/' character.
 */
const char* typeface_impl_getFontDirectoryPath() {
    return NULL;
}

// ANPWindowInterfaceV0
//
// inval may be null
// todo: send the rect along over ipc


void window_impl_setVisibleRects(NPP instance, const ANPRectI rects[], int32_t count) {
    notice("%s", __func__);
    // Just ignore it.
    //_abort();
}
/** Clears any rectangles that are being tracked as a result of a call to
    setVisibleRects. This call is equivalent to setVisibleRect(inst, NULL, 0).
 */
void    window_impl_clearVisibleRects(NPP instance) {
    notice("%s", __func__);
    _abort();
}
/** Given a boolean value of true the device will be requested to provide
    a keyboard. A value of false will result in a request to hide the
    keyboard. Further, the on-screen keyboard will not be displayed if a
    physical keyboard is active.
 */
void    window_impl_showKeyboard(NPP instance, bool value) {
    notice("%s", __func__);
}
/** Called when a plugin wishes to enter into full screen mode. The plugin's
    Java class (set using kSetPluginStubJavaClassName_ANPSetValue) will be
    called asynchronously to provide a View object to be displayed full screen.
 */
void    window_impl_requestFullScreen(NPP instance) {
    notice("%s", __func__);
}

void    window_impl_exitFullScreen(NPP instance) {
    notice("%s", __func__);
    _abort();
}

void    window_impl_requestCenterFitZoom(NPP instance) {
    notice("%s", __func__);
    _abort();
}
// ANPSystemInterfaceV0

const char* system_impl_getApplicationDataDirectory() {
    return "/tmp/";
}

extern "C" jclass system_impl_loadJavaClass(NPP instance, const char* className);

// ANPPathInterfaceV0
struct ANPPath {
    CGMutablePathRef path;
};
/** Return a new path */
ANPPath* path_impl_newPath() {
    ANPPath *result = new ANPPath;
    result->path = CGPathCreateMutable();
    return result;
}

/** Delete a path previously allocated by ANPPath() */
void path_impl_deletePath(ANPPath *path) {
    CGPathRelease(path->path);
}

/** Make a deep copy of the src path, into the dst path (already allocated
    by the caller).
 */
void path_impl_copy(ANPPath* dst, const ANPPath* src) {
    dst->path = CGPathCreateMutableCopy(src->path);
}

/** Returns true if the two paths are the same (i.e. have the same points)
 */
bool path_impl_equal(const ANPPath* path0, const ANPPath* path1) {
    return CGPathEqualToPath(path0->path, path1->path);
}

/** Remove any previous points, initializing the path back to empty. */
void path_impl_reset(ANPPath *path) {
    CGPathRelease(path->path);
    path->path = CGPathCreateMutable();
}

/** Return true if the path is empty (has no lines, quads or cubics). */
bool path_impl_isEmpty(const ANPPath *path) {
    return CGPathIsEmpty(path->path);
}

/** Return the path's bounds in bounds. */
void path_impl_getBounds(const ANPPath *path, ANPRectF* bounds) {
    *bounds = rectFFromRect(CGPathGetBoundingBox(path->path));
}

void path_impl_moveTo(ANPPath *path, float x, float y) {
    CGPathMoveToPoint(path->path, NULL, x, y);
}
void path_impl_lineTo(ANPPath *path, float x, float y) {
    CGPathAddLineToPoint(path->path, NULL, x, y);
}

void path_impl_quadTo(ANPPath *path, float x0, float y0, float x1, float y1) {
    CGPathAddQuadCurveToPoint(path->path, NULL, x0, y0, x1, y1);
}

void path_impl_cubicTo(ANPPath *path, float x0, float y0, float x1, float y1,
                float x2, float y2) {
    CGPathAddCurveToPoint(path->path, NULL, x0, y0, x1, y1, x2, y2);
}

void path_impl_close(ANPPath *path) {
    CGPathCloseSubpath(path->path);
}

/** Offset the src path by [dx, dy]. If dst is null, apply the
    change directly to the src path. If dst is not null, write the
    changed path into dst, and leave the src path unchanged. In that case
    dst must have been previously allocated by the caller.
 */
void path_impl_offset(ANPPath* src, float dx, float dy, ANPPath* dst) {
    /*CGPathRef newPath = CGPathCreateMutable();
    CGPathApply(path, (void *) newPath, offset_applier_func*/
    notice("%s", __func__);
    _abort();
}

/** Transform the path by the matrix. If dst is null, apply the
    change directly to the src path. If dst is not null, write the
    changed path into dst, and leave the src path unchanged. In that case
    dst must have been previously allocated by the caller.
 */
void path_impl_transform(ANPPath* src, const ANPMatrix*, ANPPath* dst) {
    notice("%s", __func__);
    _abort();
}

// ANPMatrixInterfaceV0
struct ANPMatrix {
    CGAffineTransform transform;
};

ANPMatrix*  matrix_impl_newMatrix() {
   ANPMatrix *result = new ANPMatrix; 
   result->transform = CGAffineTransformIdentity;
   return result;
}

void        matrix_impl_deleteMatrix(ANPMatrix *matrix) {
    delete matrix;
}

ANPMatrixFlag matrix_impl_getFlags(const ANPMatrix*) {
    return 7;
}

void        matrix_impl_copy(ANPMatrix* dst, const ANPMatrix* src) {
    *dst = *src;
}

/** Return the matrix values in a float array (allcoated by the caller),
    where the values are treated as follows:
    w  = x * [6] + y * [7] + [8];
    x' = (x * [0] + y * [1] + [2]) / w;
    y' = (x * [3] + y * [4] + [5]) / w;
 */
void        matrix_impl_get3x3(const ANPMatrix*, float[9]) {
    notice("get3x3");
    _abort();
}
/** Initialize the matrix from values in a float array,
    where the values are treated as follows:
     w  = x * [6] + y * [7] + [8];
     x' = (x * [0] + y * [1] + [2]) / w;
     y' = (x * [3] + y * [4] + [5]) / w;
 */
void        matrix_impl_set3x3(ANPMatrix*, const float[9]) {
    notice("set3x3");
    _abort();
}

void        matrix_impl_setIdentity(ANPMatrix *matrix) {
    matrix->transform = CGAffineTransformIdentity;
}
void        matrix_impl_preTranslate(ANPMatrix *matrix, float tx, float ty) {
    matrix->transform = CGAffineTransformConcat(CGAffineTransformMakeTranslation(tx, ty), matrix->transform);
}
void        matrix_impl_postTranslate(ANPMatrix *matrix, float tx, float ty){ 
    matrix->transform = CGAffineTransformConcat(matrix->transform, CGAffineTransformMakeTranslation(tx, ty));
}
void        matrix_impl_preScale(ANPMatrix *matrix, float sx, float sy) {
    matrix->transform = CGAffineTransformConcat(CGAffineTransformMakeScale(sx, sy), matrix->transform);
}
void        matrix_impl_postScale(ANPMatrix *matrix, float sx, float sy) {
    matrix->transform = CGAffineTransformConcat(matrix->transform, CGAffineTransformMakeScale(sx, sy));
}
void        matrix_impl_preSkew(ANPMatrix *matrix, float kx, float ky) {
    notice("skew");
    _abort();
}
void        matrix_impl_postSkew(ANPMatrix *matrix, float kx, float ky) {
    notice("skew");
    _abort();
}
void        matrix_impl_preRotate(ANPMatrix *matrix, float degrees) {
   matrix->transform = CGAffineTransformConcat(CGAffineTransformMakeRotation((degrees / 180.0) * M_PI), matrix->transform); 
}
void        matrix_impl_postRotate(ANPMatrix *matrix, float degrees) {
   matrix->transform = CGAffineTransformConcat(matrix->transform, CGAffineTransformMakeRotation((degrees / 180.0) * M_PI));
}
void        matrix_impl_preConcat(ANPMatrix *matrix, const ANPMatrix *matrix2) {
    matrix->transform = CGAffineTransformConcat(matrix2->transform, matrix->transform);
}
void        matrix_impl_postConcat(ANPMatrix *matrix, const ANPMatrix *matrix2) {
    matrix->transform = CGAffineTransformConcat(matrix->transform, matrix2->transform);
}

/** Return true if src is invertible, and if so, return its inverse in dst.
    If src is not invertible, return false and ignore dst.
 */
bool        matrix_impl_invert(ANPMatrix* dst, const ANPMatrix* src) {
    dst->transform = CGAffineTransformInvert(src->transform);
    return true;
}

/** Transform the x,y pairs in src[] by this matrix, and store the results
    in dst[]. The count parameter is treated as the number of pairs in the
    array. It is legal for src and dst to point to the same memory, but
    illegal for the two arrays to partially overlap.
 */
void        matrix_impl_mapPoints(ANPMatrix *matrix, float dst[], const float src[],
                         int32_t count) {
    for(int i = 0; i < count; i++) {
        CGPoint ret = CGPointApplyAffineTransform(CGPointMake(src[2*i], src[2*i+1]), matrix->transform);
        dst[2*i] = ret.x;
        dst[2*i+1] = ret.y;
    }
}


// ANPCanvasInterfaceV0
struct ANPCanvas {
    CGContextRef ctx;
    bool clipped;
    ANPPaintStyle style;
};

ANPCanvas*  canvas_impl_newCanvas(ANPBitmap *bitmap) {
    ANPCanvas *result = new ANPCanvas;
    notice("bitmap: %p width:%d height:%d baseAddr:%p rowBytes:%d format:%d", bitmap, bitmap->width, bitmap->height, bitmap->baseAddr, bitmap->rowBytes, bitmap->format);
    
    //_assert(bitmap->format == kRGBA_8888_ANPBitmapFormat);
    if(bitmap->format != kRGBA_8888_ANPBitmapFormat) {
        bitmap->width = 1;
        bitmap->height = 1;
        bitmap->baseAddr = malloc(4);
        bitmap->rowBytes = 4;
        bitmap->format = kRGBA_8888_ANPBitmapFormat;
        notice("CRAP");
    }
    result->ctx = CGBitmapContextCreate(bitmap->baseAddr, bitmap->width, bitmap->height, 8, bitmap->rowBytes, CGColorSpaceCreateDeviceRGB(), kCGBitmapByteOrder32Big | kCGImageAlphaPremultipliedLast); 
    result->clipped = false;
    result->style = kFillAndStroke_ANPPaintStyle;
    return result;
}

void        canvas_impl_deleteCanvas(ANPCanvas *canvas) {
    notice("deleting canvas %p", canvas);
    notice("its ctx:%p clipped:%d", canvas->ctx, canvas->clipped);
    CGContextRelease(canvas->ctx);
    delete canvas;
}

void        canvas_impl_save(ANPCanvas *canvas) {
    CGContextSaveGState(canvas->ctx);
}
void        canvas_impl_restore(ANPCanvas *canvas) {
    CGContextRestoreGState(canvas->ctx);
}

void        canvas_impl_translate(ANPCanvas *canvas, float tx, float ty) {
    CGContextTranslateCTM(canvas->ctx, tx, ty);
}

void        canvas_impl_scale(ANPCanvas *canvas, float sx, float sy) {
    CGContextScaleCTM(canvas->ctx, sx, sy);
}

void        canvas_impl_rotate(ANPCanvas *canvas, float degrees) {
    CGContextRotateCTM(canvas->ctx, (degrees / 180.0) * M_PI);
}

void        canvas_impl_skew(ANPCanvas *canvas, float kx, float ky) {
    notice("canvas_impl_skew: stub");
    _abort();
}

void        canvas_impl_concat(ANPCanvas *canvas, const ANPMatrix *matrix) {
    CGContextConcatCTM(canvas->ctx, matrix->transform);
}

void        canvas_impl_clipRect(ANPCanvas *canvas, const ANPRectF *rectf) {
    notice("clipRect{%f, %f, %f, %f}", rectf->left, rectf->top, rectf->right, rectf->bottom);
    CGContextClipToRect(canvas->ctx, rectFromRectF(*rectf));
    canvas->clipped = true;
}

void        canvas_impl_clipPath(ANPCanvas *canvas, const ANPPath *path) {
    notice("%s", __func__); _abort();
    CGContextAddPath(canvas->ctx, path->path);
    CGContextClip(canvas->ctx);
    canvas->clipped = true;
}

void        canvas_impl_getTotalMatrix(ANPCanvas *canvas, ANPMatrix *matrix) {
    matrix->transform = CGContextGetCTM(canvas->ctx);
}

bool        canvas_impl_getLocalClipBounds(ANPCanvas *canvas, ANPRectF *bounds, bool aa) {
    // ignore aa
    notice("%s", __func__); _abort();
    if(!canvas->clipped) return false;
    *bounds = rectFFromRect(CGContextGetClipBoundingBox(canvas->ctx));
    return true;
}

/** Return the current clip bounds in device coordinates in bounds. If the
    current clip is empty, return false and ignore the bounds argument.
 */
bool        canvas_impl_getDeviceClipBounds(ANPCanvas*, ANPRectI* bounds) {
    notice("%s", __func__);
    _abort();
}

void        canvas_impl_drawColor(ANPCanvas *canvas, ANPColor color) {
    CGFloat components[4];
    components[0] = (1.0/256) * (color & 0x00ff0000);
    components[1] = (1.0/256) * (color & 0x0000ff00);
    components[2] = (1.0/256) * (color & 0x000000ff);
    components[3] = (1.0/256) * (color & 0xff000000);   
    CGContextSetFillColor(canvas->ctx, components);
    int w = CGBitmapContextGetWidth(canvas->ctx);
    int h = CGBitmapContextGetHeight(canvas->ctx);
    log("canvas_impl_drawColor: w=%d h=%d", w, h);
    if(w > 0 && h > 0) {
        CGContextFillRect(canvas->ctx, CGRectMake(0, 0, w, h));
    }
}

CGColorRef color_getref(ANPColor color) {
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    CGFloat components[4];
    components[0] = (1.0/256) * (color & 0x00ff0000);
    components[1] = (1.0/256) * (color & 0x0000ff00);
    components[2] = (1.0/256) * (color & 0x000000ff);
    components[3] = (1.0/256) * (color & 0xff000000);   
    CGColorRef result = CGColorCreate(colorSpace, components);
    CGColorSpaceRelease(colorSpace);
    return result;
    
}
// note unpremultiplied crap

void do_paint(ANPCanvas *canvas, const ANPPaint *paint) {
    CGContextSetShouldAntialias(canvas->ctx, paint->flags & kAntiAlias_ANPPaintFlag);
    CGContextSetAllowsAntialiasing(canvas->ctx, paint->flags & kAntiAlias_ANPPaintFlag);

    CGLineJoin join;
    switch(paint->join) {
    case kMiter_ANPPaintJoin:
        join = kCGLineJoinMiter;
    case kRound_ANPPaintJoin:
        join = kCGLineJoinRound;
    case kBevel_ANPPaintJoin:
    default:
        join = kCGLineJoinBevel;
    }
    CGContextSetLineJoin(canvas->ctx, join);

    CGLineCap cap;
    switch(paint->cap) {
    case kButt_ANPPaintCap:
        cap = kCGLineCapButt;
    case kRound_ANPPaintCap:
        cap = kCGLineCapRound;
    case kSquare_ANPPaintCap:
    default:
        cap = kCGLineCapSquare;
    }
    CGContextSetLineCap(canvas->ctx, cap);

    CGContextSetLineWidth(canvas->ctx, paint->strokeWidth); 

    canvas_impl_drawColor(canvas, paint->color);

    canvas->style = paint->style;
}

CGPathDrawingMode get_dm(ANPCanvas *canvas) {
    switch(canvas->style) {
    case kFill_ANPPaintStyle:
        return kCGPathFill;
    case kStroke_ANPPaintStyle:
        return kCGPathStroke;
    case kFillAndStroke_ANPPaintStyle:
    default:
        return kCGPathFillStroke;
    }
}

void        canvas_impl_drawPaint(ANPCanvas *canvas, const ANPPaint *paint) {
    do_paint(canvas, paint);
}

void        canvas_impl_drawLine(ANPCanvas *canvas, float x0, float y0, float x1, float y1,
                        const ANPPaint *paint) {
    
    do_paint(canvas, paint);
    CGContextMoveToPoint(canvas->ctx, x0, y0);
    CGContextAddLineToPoint(canvas->ctx, x1, y1);
    CGContextDrawPath(canvas->ctx, get_dm(canvas));
}

void        canvas_impl_drawRect(ANPCanvas *canvas, const ANPRectF *rect, const ANPPaint *paint) {
    do_paint(canvas, paint);
    CGContextAddRect(canvas->ctx, rectFromRectF(*rect));
    CGContextDrawPath(canvas->ctx, get_dm(canvas));
}

void        canvas_impl_drawOval(ANPCanvas *canvas, const ANPRectF*, const ANPPaint *paint) {
    notice("%s", __func__);
    _abort();
}
    
void        canvas_impl_drawPath(ANPCanvas *canvas, const ANPPath *path, const ANPPaint *paint) {
    do_paint(canvas, paint);
    CGContextAddPath(canvas->ctx, path->path);
    CGContextDrawPath(canvas->ctx, get_dm(canvas));
}

void        canvas_impl_drawText(ANPCanvas *canvas, const void* text, uint32_t byteLength,
                        float x, float y, const ANPPaint *paint) {
    notice("%s", __func__);
    CGContextSaveGState(canvas->ctx);
    CFAttributedStringRef ats = paint_mkats(paint, text, byteLength);
    CTLineRef line = CTLineCreateWithAttributedString(ats);
    CGContextSetTextMatrix(canvas->ctx, CGAffineTransformIdentity);
    y = movie_h - y;
    CGContextSetTextPosition(canvas->ctx, x, y);
    CTLineDraw(line, canvas->ctx);
    CFRelease(line);
    CGContextRestoreGState(canvas->ctx);
}

void       canvas_impl_drawPosText(ANPCanvas *canvas, const void* text, uint32_t byteLength,
                           const float xy[], const ANPPaint *paint) {
    notice("%s", __func__);
    _abort();
}

void        canvas_impl_drawBitmapRect(ANPCanvas *canvas, const ANPBitmap *bitmap,
                              const ANPRectI* src, const ANPRectF* dst,
                              const ANPPaint *paint) {
    notice("%s: src={%d, %d, %d, %d}, dst={%f, %f, %f, %f}", __func__, src->left, src->top, src->right, src->bottom,
                                                                         dst->left, dst->top, dst->right, dst->bottom);

    _abort();
    CGDataProviderRef provider = CGDataProviderCreateWithData(NULL, bitmap->baseAddr, bitmap->rowBytes * bitmap->height, NULL);
    CGImageRef image = CGImageCreate(bitmap->width, bitmap->height, 8, 32, bitmap->rowBytes, CGColorSpaceCreateDeviceRGB(),  kCGBitmapByteOrder32Big | kCGImageAlphaPremultipliedLast, provider, NULL, false, kCGRenderingIntentDefault);
    CGImageRef image2 = CGImageCreateWithImageInRect(image, rectFromRectI(*src));
    do_paint(canvas, paint);
    CGContextDrawImage(canvas->ctx, rectFromRectF(*dst), image);
    CGImageRelease(image2);
    CGImageRelease(image);
    CGDataProviderRelease(provider);
}

void        canvas_impl_drawBitmap(ANPCanvas *canvas, const ANPBitmap *bitmap, float x, float y,
                          const ANPPaint *paint) {
    _abort();
    CGDataProviderRef provider = CGDataProviderCreateWithData(NULL, bitmap->baseAddr, bitmap->rowBytes * bitmap->height, NULL);
    CGImageRef image = CGImageCreate(bitmap->width, bitmap->height, 8, 32, bitmap->rowBytes, CGColorSpaceCreateDeviceRGB(),  kCGBitmapByteOrder32Big | kCGImageAlphaPremultipliedLast, provider, NULL, false, kCGRenderingIntentDefault);
    do_paint(canvas, paint);
    CGContextDrawImage(canvas->ctx, CGRectMake(x, y, bitmap->width, bitmap->height), image);
    CGImageRelease(image);
    CGDataProviderRelease(provider);
}

// ANPEventInterfacev0
//

extern "C" void handle_event(void *event);

static void postEventCB(CFRunLoopTimerRef timer, void *info) {
    ANPEvent *event = (ANPEvent *) info;
    notice("postEventCB... type = %d", event->eventType);
    handle_event(info);
    free(info);
    CFRunLoopTimerInvalidate(timer);
    CFRelease(timer);
}

void event_postEvent(NPP inst, const ANPEvent *event) {
    _assert(event->inSize >= sizeof(ANPEvent));
    ANPEvent *ev2 = (ANPEvent *) malloc(event->inSize);
    memcpy(ev2, event, event->inSize);
    CFRunLoopTimerContext ctx;
    memset(&ctx, 0, sizeof(ctx));
    ctx.info = (void *) ev2;
    CFRunLoopTimerRef ref = CFRunLoopTimerCreate(NULL,  CFAbsoluteTimeGetCurrent(), 0, 0, 0, postEventCB, &ctx);
    CFRunLoopAddTimer(CFRunLoopGetMain(), ref, kCFRunLoopCommonModes);

}

void event_impl_postEvent(NPP inst, const ANPEvent* event) {
    // I'll need to deal with the multithreading stuff
    notice("%s [%d]", __func__, event->eventType);
    if(event->eventType == kCustom_ANPEventType) {
        notice("    subtype: %d", event->data.other[0]);
    }
    event_postEvent(inst, event);
}

static void *temp;
static size_t temp_sz;
static ANPRectI cur_dirty;
static bool cur_dirty_valid;

// ANPSurfaceInterfaceV0
extern "C" void refresh_size();
bool surface_impl_lock(JNIEnv* env, jobject surface, ANPBitmap* bitmap, ANPRectI* dirtyRect) {
    notice("%s surface=%p", __func__, surface);
    if(temp_sz != IOSurfaceGetAllocSize(sfc)) {
        log("alloc %d", temp_sz);
        if(temp) free(temp);
        temp = calloc(1, IOSurfaceGetAllocSize(sfc));
        //memset(temp, 0xff, IOSurfaceGetAllocSize(sfc));
        temp_sz = IOSurfaceGetAllocSize(sfc);
    }
    //log("dirtyRect=%p", dirtyRect);
    // sanity, not security check, fwiw
    if(dirtyRect && !(dirtyRect->left >= (int)IOSurfaceGetWidth(sfc) || dirtyRect->top >= (int)IOSurfaceGetHeight(sfc) || dirtyRect->right >= (int)IOSurfaceGetWidth(sfc) || dirtyRect->bottom >= (int)IOSurfaceGetHeight(sfc))) {
        cur_dirty_valid = true;
        cur_dirty = *dirtyRect;
    } else {
        cur_dirty_valid = false;
    }
    bitmap->baseAddr = temp;
    bitmap->format = kRGBA_8888_ANPBitmapFormat;
    bitmap->width = sfc ? IOSurfaceGetWidth(sfc) : 0;
    bitmap->height = sfc ? IOSurfaceGetHeight(sfc) : 0;
    bitmap->rowBytes = bitmap->width * 4;
    locked = true;
    return true;
}

//#include <fcntl.h>
extern "C" void rgba_bgra_copy(void *dest, void *src, void *end);


void surface_impl_unlock(JNIEnv* env, jobject surface) {
    notice("%s surface=%p", __func__, surface);
    if(!sfc || !IOSurfaceGetWidth(sfc) || !IOSurfaceGetHeight(sfc)) return;
    if(cur_dirty_valid) {
        int rowsize = (cur_dirty.right - cur_dirty.left) * 4;
        int fullrowsize = IOSurfaceGetBytesPerRow(sfc);
        int rowoffset = cur_dirty.top * fullrowsize + cur_dirty.left * 4;
        char *start = ((char *) IOSurfaceGetBaseAddress(sfc)) + rowoffset;
        char *tempstart = ((char *) temp) + rowoffset;
        char *end = tempstart + rowsize;
        for(int y = cur_dirty.top; y < cur_dirty.bottom; y++) {
            rgba_bgra_copy((void *) start, (void *) tempstart, (void *) end);
            start += fullrowsize;
            tempstart += fullrowsize;
            end += fullrowsize;
        }
    } else {
        void *end = (void *) ((char *)temp + IOSurfaceGetAllocSize(sfc));
        rgba_bgra_copy(IOSurfaceGetBaseAddress(sfc), temp, end);
    }
    
    if(sfc_dirty) {
        sfc_dirty = false;
        _assertZero(use_surface(food, (int) IOSurfaceGetID(sfc)));
    }

    if(cur_dirty_valid) {
        _assertZero(display_sync(food, cur_dirty.left, cur_dirty.top, cur_dirty.right, cur_dirty.bottom));
    } else {
        _assertZero(display_sync(food, 0, 0, IOSurfaceGetWidth(sfc), IOSurfaceGetHeight(sfc)));
    }
    //memcpy(IOSurfaceGetBaseAddress(sfc), temp, IOSurfaceGetAllocSize(sfc));
    /*int fd = open("foo.txt", O_WRONLY | O_CREAT, 0755);
    write(fd, IOSurfaceGetBaseAddress(sfc), IOSurfaceGetAllocSize(sfc));
    close(fd);*/
    if(pending_movie_w != movie_w || pending_movie_h != movie_h) {
        refresh_size();
    }
    locked = false;
}

extern "C"
void iface_getvalue(NPNVariable typ, ANPInterface *ptr) {
    switch(typ) {
    case kLogInterfaceV0_ANPGetValue: {
        ANPLogInterfaceV0 *iface = (ANPLogInterfaceV0 *) ptr;
        _assert(iface->inSize == sizeof(*iface));
        *((void **) (&iface->log)) = stub(log_impl_log);
        break; }

    case kBitmapInterfaceV0_ANPGetValue: {
        ANPBitmapInterfaceV0 *iface = (ANPBitmapInterfaceV0 *) ptr;
        _assert(iface->inSize == sizeof(*iface));
        *((void **) (&iface->getPixelPacking)) = stub(bitmap_impl_getPixelPacking);
        break; }

    case kMatrixInterfaceV0_ANPGetValue: {
        ANPMatrixInterfaceV0 *iface = (ANPMatrixInterfaceV0 *) ptr;
        _assert(iface->inSize == sizeof(*iface));
        *((void **) (&iface->newMatrix)) = stub(matrix_impl_newMatrix);
        *((void **) (&iface->deleteMatrix)) = stub(matrix_impl_deleteMatrix);
        *((void **) (&iface->getFlags)) = stub(matrix_impl_getFlags);
        *((void **) (&iface->copy)) = stub(matrix_impl_copy);
        *((void **) (&iface->get3x3)) = stub(matrix_impl_get3x3);
        *((void **) (&iface->set3x3)) = stub(matrix_impl_set3x3);
        *((void **) (&iface->setIdentity)) = stub(matrix_impl_setIdentity);
        *((void **) (&iface->preTranslate)) = stub(matrix_impl_preTranslate);
        *((void **) (&iface->postTranslate)) = stub(matrix_impl_postTranslate);
        *((void **) (&iface->preScale)) = stub(matrix_impl_preScale);
        *((void **) (&iface->postScale)) = stub(matrix_impl_postScale);
        *((void **) (&iface->preSkew)) = stub(matrix_impl_preSkew);
        *((void **) (&iface->postSkew)) = stub(matrix_impl_postSkew);
        *((void **) (&iface->preRotate)) = stub(matrix_impl_preRotate);
        *((void **) (&iface->postRotate)) = stub(matrix_impl_postRotate);
        *((void **) (&iface->preConcat)) = stub(matrix_impl_preConcat);
        *((void **) (&iface->postConcat)) = stub(matrix_impl_postConcat);
        *((void **) (&iface->invert)) = stub(matrix_impl_invert);
        *((void **) (&iface->mapPoints)) = stub(matrix_impl_mapPoints);
        break; }

    case kPathInterfaceV0_ANPGetValue: {
        ANPPathInterfaceV0 *iface = (ANPPathInterfaceV0 *) ptr;
        _assert(iface->inSize == sizeof(*iface));
        *((void **) (&iface->newPath)) = stub(path_impl_newPath);
        *((void **) (&iface->deletePath)) = stub(path_impl_deletePath);
        *((void **) (&iface->copy)) = stub(path_impl_copy);
        *((void **) (&iface->equal)) = stub(path_impl_equal);
        *((void **) (&iface->reset)) = stub(path_impl_reset);
        *((void **) (&iface->isEmpty)) = stub(path_impl_isEmpty);
        *((void **) (&iface->getBounds)) = stub(path_impl_getBounds);
        *((void **) (&iface->moveTo)) = stub(path_impl_moveTo);
        *((void **) (&iface->lineTo)) = stub(path_impl_lineTo);
        *((void **) (&iface->quadTo)) = stub(path_impl_quadTo);
        *((void **) (&iface->cubicTo)) = stub(path_impl_cubicTo);
        *((void **) (&iface->close)) = stub(path_impl_close);
        *((void **) (&iface->offset)) = stub(path_impl_offset);
        *((void **) (&iface->transform)) = stub(path_impl_transform);
        break; }

    case kTypefaceInterfaceV0_ANPGetValue: {
        ANPTypefaceInterfaceV0 *iface = (ANPTypefaceInterfaceV0 *) ptr;
        _assert(iface->inSize == sizeof(*iface));
        *((void **) (&iface->createFromName)) = stub(typeface_impl_createFromName);
        *((void **) (&iface->createFromTypeface)) = stub(typeface_impl_createFromTypeface);
        *((void **) (&iface->getRefCount)) = stub(typeface_impl_getRefCount);
        *((void **) (&iface->ref)) = stub(typeface_impl_ref);
        *((void **) (&iface->unref)) = stub(typeface_impl_unref);
        *((void **) (&iface->getStyle)) = stub(typeface_impl_getStyle);
        *((void **) (&iface->getFontPath)) = stub(typeface_impl_getFontPath);
        *((void **) (&iface->getFontDirectoryPath)) = stub(typeface_impl_getFontDirectoryPath);
        break; }

    case kPaintInterfaceV0_ANPGetValue: {
        ANPPaintInterfaceV0 *iface = (ANPPaintInterfaceV0 *) ptr;
        _assert(iface->inSize == sizeof(*iface));
        *((void **) (&iface->newPaint)) = stub(paint_impl_newPaint);
        *((void **) (&iface->deletePaint)) = stub(paint_impl_deletePaint);
        *((void **) (&iface->getFlags)) = stub(paint_impl_getFlags);
        *((void **) (&iface->setFlags)) = stub(paint_impl_setFlags);
        *((void **) (&iface->getColor)) = stub(paint_impl_getColor);
        *((void **) (&iface->setColor)) = stub(paint_impl_setColor);
        *((void **) (&iface->getStyle)) = stub(paint_impl_getStyle);
        *((void **) (&iface->setStyle)) = stub(paint_impl_setStyle);
        *((void **) (&iface->getStrokeWidth)) = stub(paint_impl_getStrokeWidth);
        *((void **) (&iface->getStrokeMiter)) = stub(paint_impl_getStrokeMiter);
        *((void **) (&iface->getStrokeCap)) = stub(paint_impl_getStrokeCap);
        *((void **) (&iface->getStrokeJoin)) = stub(paint_impl_getStrokeJoin);
        *((void **) (&iface->setStrokeWidth)) = stub(paint_impl_setStrokeWidth);
        *((void **) (&iface->setStrokeMiter)) = stub(paint_impl_setStrokeMiter);
        *((void **) (&iface->setStrokeCap)) = stub(paint_impl_setStrokeCap);
        *((void **) (&iface->setStrokeJoin)) = stub(paint_impl_setStrokeJoin);
        *((void **) (&iface->getTextEncoding)) = stub(paint_impl_getTextEncoding);
        *((void **) (&iface->getTextAlign)) = stub(paint_impl_getTextAlign);
        *((void **) (&iface->getTextSize)) = stub(paint_impl_getTextSize);
        *((void **) (&iface->getTextScaleX)) = stub(paint_impl_getTextScaleX);
        *((void **) (&iface->getTextSkewX)) = stub(paint_impl_getTextSkewX);
        *((void **) (&iface->setTextEncoding)) = stub(paint_impl_setTextEncoding);
        *((void **) (&iface->setTextAlign)) = stub(paint_impl_setTextAlign);
        *((void **) (&iface->setTextSize)) = stub(paint_impl_setTextSize);
        *((void **) (&iface->setTextScaleX)) = stub(paint_impl_setTextScaleX);
        *((void **) (&iface->setTextSkewX)) = stub(paint_impl_setTextSkewX);
        *((void **) (&iface->getTypeface)) = stub(paint_impl_getTypeface);
        *((void **) (&iface->setTypeface)) = stub(paint_impl_setTypeface);
        *((void **) (&iface->measureText)) = stub(paint_impl_measureText);
        *((void **) (&iface->getTextWidths)) = stub(paint_impl_getTextWidths);
        *((void **) (&iface->getFontMetrics)) = stub(paint_impl_getFontMetrics);
        break; }

    case kCanvasInterfaceV0_ANPGetValue: {
        ANPCanvasInterfaceV0 *iface = (ANPCanvasInterfaceV0 *) ptr;
        _assert(iface->inSize == sizeof(*iface));
        *((void **) (&iface->newCanvas)) = stub(canvas_impl_newCanvas);
        *((void **) (&iface->deleteCanvas)) = stub(canvas_impl_deleteCanvas);
        *((void **) (&iface->save)) = stub(canvas_impl_save);
        *((void **) (&iface->restore)) = stub(canvas_impl_restore);
        *((void **) (&iface->translate)) = stub(canvas_impl_translate);
        *((void **) (&iface->scale)) = stub(canvas_impl_scale);
        *((void **) (&iface->rotate)) = stub(canvas_impl_rotate);
        *((void **) (&iface->skew)) = stub(canvas_impl_skew);
        *((void **) (&iface->concat)) = stub(canvas_impl_concat);
        *((void **) (&iface->clipRect)) = stub(canvas_impl_clipRect);
        *((void **) (&iface->clipPath)) = stub(canvas_impl_clipPath);
        *((void **) (&iface->getTotalMatrix)) = stub(canvas_impl_getTotalMatrix);
        *((void **) (&iface->getLocalClipBounds)) = stub(canvas_impl_getLocalClipBounds);
        *((void **) (&iface->getDeviceClipBounds)) = stub(canvas_impl_getDeviceClipBounds);
        *((void **) (&iface->drawColor)) = stub(canvas_impl_drawColor);
        *((void **) (&iface->drawPaint)) = stub(canvas_impl_drawPaint);
        *((void **) (&iface->drawLine)) = stub(canvas_impl_drawLine);
        *((void **) (&iface->drawRect)) = stub(canvas_impl_drawRect);
        *((void **) (&iface->drawOval)) = stub(canvas_impl_drawOval);
        *((void **) (&iface->drawPath)) = stub(canvas_impl_drawPath);
        *((void **) (&iface->drawText)) = stub(canvas_impl_drawText);
        *((void **) (&iface->drawPosText)) = stub(canvas_impl_drawPosText);
        *((void **) (&iface->drawBitmap)) = stub(canvas_impl_drawBitmap);
        *((void **) (&iface->drawBitmapRect)) = stub(canvas_impl_drawBitmapRect);
        break; }

    case kWindowInterfaceV0_ANPGetValue: {
        ANPWindowInterfaceV0 *iface = (ANPWindowInterfaceV0 *) ptr;
        _assert(iface->inSize == sizeof(*iface));
        *((void **) (&iface->setVisibleRects)) = stub(window_impl_setVisibleRects);
        *((void **) (&iface->clearVisibleRects)) = stub(window_impl_clearVisibleRects);
        *((void **) (&iface->showKeyboard)) = stub(window_impl_showKeyboard);
        *((void **) (&iface->requestFullScreen)) = stub(window_impl_requestFullScreen);
        *((void **) (&iface->exitFullScreen)) = stub(window_impl_exitFullScreen);
        *((void **) (&iface->requestCenterFitZoom)) = stub(window_impl_requestCenterFitZoom);
        break; }

    case kAudioTrackInterfaceV0_ANPGetValue: {
        ANPAudioTrackInterfaceV0 *iface = (ANPAudioTrackInterfaceV0 *) ptr;
        _assert(iface->inSize == sizeof(*iface));
        *((void **) (&iface->newTrack)) = stub(audiotrack_impl_newTrack);
        *((void **) (&iface->deleteTrack)) = stub(audiotrack_impl_deleteTrack);
        *((void **) (&iface->start)) = stub(audiotrack_impl_start);
        *((void **) (&iface->pause)) = stub(audiotrack_impl_pause);
        *((void **) (&iface->stop)) = stub(audiotrack_impl_stop);
        *((void **) (&iface->isStopped)) = stub(audiotrack_impl_isStopped);
        break; }

    case kEventInterfaceV0_ANPGetValue: {
        ANPEventInterfaceV0 *iface = (ANPEventInterfaceV0 *) ptr;
        _assert(iface->inSize == sizeof(*iface));
        *((void **) (&iface->postEvent)) = stub(event_impl_postEvent);
        break; }

    case kSystemInterfaceV0_ANPGetValue: {
        ANPSystemInterfaceV0 *iface = (ANPSystemInterfaceV0 *) ptr;
        _assert(iface->inSize == sizeof(*iface));
        *((void **) (&iface->getApplicationDataDirectory)) = stub(system_impl_getApplicationDataDirectory);
        *((void **) (&iface->loadJavaClass)) = stub(system_impl_loadJavaClass);
        break; }

    case kSurfaceInterfaceV0_ANPGetValue: {
        ANPSurfaceInterfaceV0 *iface = (ANPSurfaceInterfaceV0 *) ptr;
        _assert(iface->inSize == sizeof(*iface));
        *((void **) (&iface->lock)) = stub(surface_impl_lock);
        *((void **) (&iface->unlock)) = stub(surface_impl_unlock);
        break; }


    default:
        warn("Unknown interface type %d", typ);
    }
}

extern "C" void post_draw_event() {
    ANPEvent *event = new ANPEvent;
    event->inSize = sizeof(*event);
    event->eventType = kDraw_ANPEventType;
    event->data.draw.model = kSurface_ANPDrawingModel;
    event->data.draw.clip = (struct ANPRectI) {0, 0, movie_w, movie_h};
    memset(&event->data.draw.data.bitmap, 0xfe, sizeof(event->data.draw.data.bitmap));
    event_postEvent(NULL, event);
    delete event;
}

extern "C" void post_lifecycle_event(ANPLifecycleAction action) {
    ANPEvent *event = new ANPEvent;
    event->inSize = sizeof(*event);
    event->eventType = kLifecycle_ANPEventType;
    event->data.lifecycle.action = action;
    event_postEvent(NULL, event);
    delete event;
}

extern "C" void post_key_event(bool isUp, int32_t nativeCode, int32_t virtualCode, uint32_t modifiers, int32_t repeatCount, int32_t unichar) {
    ANPEvent *event = new ANPEvent;
    event->inSize = sizeof(*event);
    event->eventType = kKey_ANPEventType;
    event->data.key.action = isUp ? kUp_ANPKeyAction : kDown_ANPKeyAction;
    event->data.key.nativeCode = (int32_t) nativeCode;
    event->data.key.virtualCode = virtualCode;
    event->data.key.modifiers = (ANPKeyModifier) modifiers;
    event->data.key.unichar = unichar;
    event_postEvent(NULL, event);
    delete event;
}

extern "C" void post_touch_event(ANPTouchAction action, ANPKeyModifier modifiers, int32_t x, int32_t y) {
    ANPEvent *event = new ANPEvent;
    event->inSize = sizeof(*event);
    event->eventType = kTouch_ANPEventType;
    event->data.touch.action = action;
    event->data.touch.modifiers = modifiers;
    event->data.touch.x = x;
    event->data.touch.y = y;
    event_postEvent(NULL, event);
    delete event;

}

extern "C" void post_custom_event(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f, int32_t g, int32_t h) {
    ANPEvent *event = new ANPEvent;
    event->inSize = sizeof(*event);
    event->eventType = kCustom_ANPEventType;
    event->data.other[0] = a;
    event->data.other[1] = b;
    event->data.other[2] = c;
    event->data.other[3] = d;
    event->data.other[4] = e;
    event->data.other[5] = f;
    event->data.other[6] = g;
    event->data.other[7] = h;
    event_postEvent(NULL, event);
    delete event;
}

extern "C" void post_mouse_event(ANPMouseAction action, int32_t x, int32_t y) {
    ANPEvent *event = new ANPEvent;
    event->inSize = sizeof(*event);
    event->eventType = kMouse_ANPEventType;
    event->data.mouse.action = action;
    event->data.mouse.x = x;
    event->data.mouse.y = y;
    event_postEvent(NULL, event);
    delete event;


}

extern "C" int touch(int rpcfd, int action, int x, int y) {
    log("touch! (%d, %d)", x, y);
    ANPEvent *event = new ANPEvent;
    event->inSize = sizeof(*event);
    event->eventType = kTouch_ANPEventType;
    event->data.touch.action = action;
    event->data.touch.modifiers = 0;
    event->data.touch.x = x;
    event->data.touch.y = y;
    handle_event(event);
    delete event;
    notice("now doing the other thing");

    notice("ok");
    return 0;
}

