#include <stdint.h>
#include <stdlib.h>
#include <utils/RefBase.h>
#include <utils/Vector.h>
#include <stdio.h>
namespace android {
class MediaSource {
public:
    MediaSource();
    ~MediaSource();
};

MediaSource::MediaSource() {
    printf("MediaSource\n");
}
MediaSource::~MediaSource() {
    printf("~MediaSource\n");
}

class MetaData {
public:
    MetaData();
    void setData(unsigned int, unsigned int, const void *, unsigned int);
    bool findInt64(unsigned int, int64_t *);
    bool findInt32(unsigned int, int32_t *);
    bool findCString(unsigned int, const char **);
    bool setInt64(unsigned int, int64_t);
    bool setInt32(unsigned int, int32_t);
    bool setCString(unsigned int, const char *);
};

    
MetaData::MetaData() { }
void MetaData::setData(unsigned int, unsigned int, const void *, unsigned int) {
    printf("setData called\n");
}
bool MetaData::findInt64(unsigned int, int64_t *) {
    printf("findInt64 called\n");
    return false;
}
bool MetaData::findInt32(unsigned int, int32_t *) {
    printf("findInt32 called\n");
    return false;
}
bool MetaData::findCString(unsigned int, const char **) {
    printf("findCString called\n");
    return false;
}
bool MetaData::setInt64(unsigned int, int64_t) {
    printf("setInt64 called\n");
    return false;
}
bool MetaData::setInt32(unsigned int, int32_t) {
    printf("setInt32 called\n");
    return false;
}
bool MetaData::setCString(unsigned int, const char *) {
    printf("setCString called\n");
    return false;
}

class MediaBuffer {
public:
    MediaBuffer(unsigned int);
    void release();
    sp<MetaData> meta_data();
    void *data() const;
    size_t size() const;
    size_t range_offset() const;
    size_t range_length() const;
    void set_range(unsigned int offset, unsigned int length);
private:
    size_t _range_offset, _range_length;
};
MediaBuffer::MediaBuffer(unsigned int x) {
    printf("MediaBuffer(%d)\n", x);
    _range_offset = _range_length = 0;
}
void MediaBuffer::release() {
    printf("MediaBuffer::release\n");
}
sp<MetaData> MediaBuffer::meta_data() {
    abort();
}
void *MediaBuffer::data() const {
    return (void *) 0xdeadbeef;
}
size_t MediaBuffer::range_offset() const {
    return _range_offset;
}
size_t MediaBuffer::range_length() const {
    return _range_length;
}
void MediaBuffer::set_range(unsigned int offset, unsigned int length) {
    _range_offset = offset;
    _range_length = length;
}
size_t MediaBuffer::size() const {
    return 0;
}

class IOMX;
class CodecCapabilities;
status_t QueryCodecs(
        const sp<IOMX> &omx,
        const char *mimeType, bool queryDecoders,
        Vector<CodecCapabilities> *results) {
    printf("QueryCodecs called\n");
    return -1;
}
int MEDIA_MIMETYPE_AUDIO_AAC = 42;
int MEDIA_MIMETYPE_VIDEO_AVC = 43;
class OMXClient {
public:
    OMXClient();
    int connect();
};

OMXClient::OMXClient() {
    printf("OMXClient\n");
}

int OMXClient::connect() {
    printf("OMXClient::connect\n");
    return -1;
}


class OMXCodec {
public:
    static sp<OMXCodec> Create(
            const sp<IOMX> &omx,
            const sp<MetaData> &meta, bool createEncoder,
            const sp<MediaSource> &source,
            const char *matchComponentName,
            unsigned int wtf);

};

sp<OMXCodec> OMXCodec::Create(
            const sp<IOMX> &omx,
            const sp<MetaData> &meta, bool createEncoder,
            const sp<MediaSource> &source,
            const char *matchComponentName,
            unsigned int wtf) {
    printf("OMXCodec::Create\n");
    abort();
}


} // android

extern "C" {
void *_Znwj(unsigned int a) {
    printf("operator new: %u\n", a);
    return (operator new)((unsigned long)a);
}
void *_Znaj(unsigned int a) {
    printf("operator new[]: %u\n", a);
    return (operator new[])((unsigned long)a);
}
}
