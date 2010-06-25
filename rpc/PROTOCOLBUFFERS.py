# Less annoying RPC.
import sys, re
from cStringIO import StringIO

base = sys.argv[1].replace('.proto', '') + '_rpc'
hfiles = [open('%s1.h' % base, 'w'), open('%s2.h' % base, 'w')]
cfiles = [open('%s1.c' % base, 'w'), open('%s2.c' % base, 'w')]

realstdout = sys.stdout

for hfile in hfiles:
    sys.stdout = hfile
    print '#pragma once'
    print '#include <stdint.h>'
    print '#include <stdbool.h>'
    print '#include <stdlib.h>'
    print
    print 'void rpcserve(int rpcfd, void (*error)(int, int));'
    print

for i in xrange(2):
    sys.stdout = cfiles[i]
    print '#include "%s%d.h"' % (base, i + 1)
    print '''
#include <sys/socket.h>
#include <CoreFoundation/CoreFoundation.h>
#include <pthread.h>
#include <signal.h>
#include <libkern/OSAtomic.h>
#include <fcntl.h>

struct hdr {
    int magic;
    size_t msgsize;
    int is_resp;
    int msgid;
    int funcid_or_status;
    char extra[0];
} __attribute__((packed));

static int msgids;
static CFMutableDictionaryRef rpc_states;
static bool rpcserve_alive;
static pthread_mutex_t rpc_states_mutex = PTHREAD_MUTEX_INITIALIZER;
static CFMutableArrayRef pending_sends;
// Unnecessary?
static pthread_mutex_t send_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t send_cond = PTHREAD_COND_INITIALIZER;
static pthread_once_t send_once = PTHREAD_ONCE_INIT;

__attribute__((constructor))
static void init_rpc_states() {
    rpc_states = CFDictionaryCreateMutable(NULL, 0, NULL, NULL);
}

// Why can't I just have an unlimited buffer size? (whine, whine)

static void *xsend_thread(void *blah) {
    pthread_mutex_lock(&send_mutex);
    while(1) {
        struct hdr *hdr;
        while(1) {
            if(!CFArrayGetCount(pending_sends)) {
                break;
            }
            hdr = (void *) CFArrayGetValueAtIndex(pending_sends, 0);
            CFArrayRemoveValueAtIndex(pending_sends, 0);
            pthread_mutex_unlock(&send_mutex);
            int fd = hdr->magic;
            hdr->magic = 0x12345678;
            const char *p = (void *) hdr;
            size_t len = hdr->msgsize;
            //fprintf(stderr, "Ok I'm sending %p.  It has %d bytes of data\\n", hdr, (int) len);
            while(len) {
                int bs = send(fd, p, len, 0);
                //fprintf(stderr, "bs = %d\\n", bs);
                if(bs < 0) {
                    fprintf(stderr, "ERROR in xsend: %s\\n", strerror(errno));
                    break;
                }
                len -= bs;
                p += bs;
            }
            free(hdr);
            pthread_mutex_lock(&send_mutex);
        }
        pthread_cond_wait(&send_cond, &send_mutex);
    }
    pthread_mutex_unlock(&send_mutex);
    return NULL;
}

static void xsend_startup() {
    pending_sends = CFArrayCreateMutable(NULL, 0, NULL);
    pthread_t thread;
    pthread_create(&thread, NULL, xsend_thread, NULL);
}

static int xsend(void *buf) {
    pthread_once(&send_once, xsend_startup);
    pthread_mutex_lock(&send_mutex);
    CFArrayAppendValue(pending_sends, buf);
    pthread_cond_signal(&send_cond);
    pthread_mutex_unlock(&send_mutex);
    return 0;
}


'''
server_files = [StringIO(), StringIO()]

funcid = 0
for line in open(sys.argv[1]):
    line = re.sub('#.*$', '', line).strip()
    if line == '': continue
    funcid += 1
    assert line.endswith(';')
    bits = filter(lambda x: x, re.split('([ \(\),])', line[:-1]))
    direction = bits.pop(0)
    assert direction in ('<', '>')
    if direction == '>':
        client_cfile = cfiles[0]
        client_hfile = hfiles[0]
        server_hfile  = hfiles[1]
        server_file = server_files[1]
    else:
        client_cfile = cfiles[1]
        client_hfile = hfiles[1]
        server_hfile  = hfiles[0]
        server_file = server_files[0]
    assert bits.pop(0) == ' '
    syncness = bits.pop(0) 
    assert syncness in ('sync', 'async')
    sync = syncness == 'sync'
    assert bits.pop(0) == ' '
    funcname = bits.pop(0)
    assert bits.pop(0) == '('
    args = []
    while True:
        argtype = bits.pop(0)
        if argtype in (' ', ','): continue
        if argtype == ')': break
        if argtype == 'out':
            out = True
            assert bits.pop(0) == ' '
            argtype = bits.pop(0)
        else:
            out = False
        assert bits.pop(0) == ' '
        argname = bits.pop(0)
        args.append((out, argtype, argname))
    assert len(bits) == 0

    sig = '%s(int rpcfd' % funcname
    for out, argtype, argname in args:
        sig += ', '
        if argtype == 'buf':
            if out:
                sig += 'void **%s, size_t *%s_len' % (argname, argname)
            else:
                sig += 'void *%s, size_t %s_len' % (argname, argname)
        else:
            sig += '%s %s%s' % (argtype, '*' if out else '', argname)
    sig += ')'

    csig = 'int ' + sig
    ssig = 'int ' + sig
        
    print >> client_hfile, csig + ';'
    print >> server_hfile, ssig + ';'

    for hfile in hfiles:
        sys.stdout = hfile
        print 'struct %s_req {' % funcname
        print '\tint magic;'
        print '\tsize_t msgsize;'
        print '\tint is_resp;'
        print '\tint msgid;'
        print '\tint funcid;'
        for out, argtype, argname in args:
            if not out:
                if argtype == 'buf':
                    print '\tsize_t %s_len;' % argname
                else:
                    print '\t%s %s;' % (argtype, argname)
        print '\tchar extra[0];'
        print '} __attribute__((packed));'
        
        if sync:
            print 'struct %s_resp {' % funcname
            print '\tint magic;'
            print '\tsize_t msgsize;'
            print '\tint is_resp;'
            print '\tint msgid;'
            print '\tint status;'
            for out, argtype, argname in args:
                if out:
                    if argtype == 'buf':
                        print '\tsize_t %s_len;' % argname
                    else:
                        print '\t%s %s;' % (argtype, argname)
            print '\tchar extra[0];'
            print '} __attribute__((packed));'

    sys.stdout = client_cfile

    size = 'sizeof(struct %s_req)' % funcname
    for out, argtype, argname in args:
        if not out and argtype == 'buf':
            size += ' + %s_len' % argname
    print csig + ' {'
    print '\tsize_t size = %s;' % size
    print '\tstruct %s_req *req = malloc(size);' % funcname
    print '\treq->magic = rpcfd;'
    print '\treq->msgsize = size;'
    print '\treq->is_resp = false;'
    print '\treq->msgid = OSAtomicIncrement32(&msgids);'
    print '\treq->funcid = %d;' % funcid
    print '\t//fprintf(stderr, "funcid = %d\\n", req->funcid);'
    for out, argtype, argname in args:
        if not out:
            if argtype == 'buf': argname += '_len'
            print '\treq->%s = %s;' % (argname, argname)
    print '\tchar *extrap = req->extra;'
    for out, argtype, argname in args:
        if not out and argtype == 'buf':
            print '\tmemcpy(extrap, %s, %s_len);' % (argname, argname)
            print '\textrap += %s_len;' % argname
    print '\tsignal(SIGPIPE, SIG_IGN);'
    print '\tssize_t bs;' 
    print '\tpthread_mutex_lock(&rpc_states_mutex);'
    if sync:
        print '\tif(!rpcserve_alive) {'
        print '\t\tpthread_mutex_unlock(&rpc_states_mutex);'
        print '\t\tfree(req);'
        print '\t\treturn 2;'
        print '\t}'

        print '\tstruct { pthread_cond_t cond; pthread_mutex_t mut; int src_fd; struct %s_resp *msg; } s;' % funcname
        print '\tpthread_mutex_init(&s.mut, NULL);'
        print '\tpthread_cond_init(&s.cond, NULL);'
        print '\ts.msg = NULL;'
        print '\ts.src_fd = rpcfd;'
        #print '\tfprintf(stderr, "-> %d\\n", req->msgid);'
        print '\tCFDictionarySetValue(rpc_states, (void *) req->msgid, (void *) &s);'
    print '\tpthread_mutex_unlock(&rpc_states_mutex);'
    print '\tif(xsend(req)) { return 1; };'
    if sync:
        print '\tpthread_mutex_lock(&s.mut);'
        print '\twhile(!s.msg) {'
        print '\t\tpthread_cond_wait(&s.cond, &s.mut);'
        print '\t}'
        print '\tif(s.msg->status) {'
        print '\t\tint status = s.msg->status;'
        print '\t\tfree(s.msg);'
        print '\t\treturn status;'
        print '\t}'
        print '\textrap = s.msg->extra;'
        plus2 = 'sizeof(*s.msg)'
        for out, argtype, argname in args:
            if out and argtype == 'buf':
                plus2 += ' + s.msg->%s_len' % argname
        print '\tif(s.msg->msgsize < sizeof(*s.msg) || s.msg->msgsize < %s) { free(s.msg); return 3; }' % plus2
        for out, argtype, argname in args:
            if out:
                if argtype == 'buf':
                    print '\tif(%s_len) *%s_len = s.msg->%s_len;' % (argname, argname, argname)
                    print '\tif(%s) {' % argname
                    print '\t\t*%s = malloc(s.msg->%s_len + 1);' % (argname, argname)
                    print '\t\t*((char *)(*%s) + s.msg->%s_len) = 0;' % (argname, argname)
                    print '\t\tmemcpy(*%s, extrap, s.msg->%s_len);' % (argname, argname)
                    print '\t}'
                    print '\textrap += s.msg->%s_len;' % argname
                else:
                    print '\tif(%s) *%s = s.msg->%s;' % (argname, argname, argname)

        print '\tpthread_cond_destroy(&s.cond);'
        print '\tpthread_mutex_destroy(&s.mut);'
        print '\tfree(s.msg);'
    print '\treturn 0;'
    print '}'
    print

    sys.stdout = server_file

    print 'case %d: {' % funcid
    print '\tstruct %s_req *msg = (void *) msg_;' % funcname
    if sync:
        print '\tstruct %s_resp sresp;' % funcname
        print '\t\tsresp.msgid = msg->msgid;'
        print '\t\tsresp.is_resp = true;'
    size = 'sizeof(*msg)'
    for out, argtype, argname in args:
        if argtype == 'buf':
            if out:
                print '\t\tsresp.%s_len = 0x7fffffff;' % argname
            else:
                size += ' + msg->%s_len' % argname
    print '\tif(msg->msgsize >= sizeof(*msg) && msg->msgsize >= %s) {' % size
    print '\t\tchar *extrap = msg->extra;'
    for out, argtype, argname in args:
        if argtype == 'buf':
            if out:
                print '\t\tvoid *%s;' % argname
            else:
                print '\t\tchar *%s = malloc(1 + msg->%s_len);' % (argname, argname)
                print '\t\tmemcpy(%s, extrap, msg->%s_len);' % (argname, argname)
                print '\t\t%s[msg->%s_len] = 0;' % (argname, argname)
                print '\t\textrap += msg->%s_len;' % argname
                #print '\t\textrap;' % argname
                #This is more efficeint but I like padding
                #print '\t\textrap += msg->%s_len;' % argname
    ar = 'rpcfd'
    for out, argtype, argname in args:
        ar += ', '
        if out:
            if argtype == 'buf':
                ar += '&%s, &sresp.%s_len' % (argname, argname)
            else:
                ar += '&sresp.%s' % argname
        else:
            if argtype == 'buf':
                ar += '%s, msg->%s_len' % (argname, argname)
            else:
                ar += 'msg->%s' % argname
    print '\t\tint _status = %s(%s);' % (funcname, ar)
    #for out, argtype, argname in args:
    #    if not out and argtype == 'buf':
    #        print '\t\tfree(%s);' % argname
    if sync:
        size = 'sizeof(struct %s_resp)' % funcname
        for out, argtype, argname in args:
            if out and argtype == 'buf':
                print '\t\tassert(sresp.%s_len != 0x7fffffff);' % argname
                size += ' + sresp.%s_len' % argname
        print '\t\tsize_t size = %s;' % size
        print '\t\tsresp.magic = rpcfd;'
        print '\t\tsresp.msgsize = size;'
        print '\t\tsresp.status = _status;'
        print '\t\tstruct %s_resp *resp = malloc(size);' % funcname
        print '\t\tmemcpy(resp, &sresp, sizeof(sresp));'
        print '\t\textrap = resp->extra;'
        for out, argtype, argname in args:
            if out and argtype == 'buf':
                print '\t\tif(%s) memcpy(extrap, %s, resp->%s_len);' % (argname, argname, argname)
                print '\t\textrap += resp->%s_len;' % argname
        print '\t\txsend(resp);'
    print '\t}'
    print '} break;'
    print

for i in xrange(2):
    sys.stdout = cfiles[i] 
    server = server_files[i].getvalue()
    print '''

int rpcserve_thread_(int, CFRunLoopSourceRef, pthread_mutex_t *, CFMutableArrayRef);

static void rpc_applier(const void *key, const void *value, void *context) {
    struct hdr *fake_resp = malloc(sizeof(struct hdr));
    fake_resp->magic = 0x12345678;
    fake_resp->is_resp = true;
    fake_resp->msgid = (int) key;
    fake_resp->funcid_or_status = 2;
    struct { pthread_cond_t cond; pthread_mutex_t mut; int src_fd; void *msg; } *s = (void *) value;
    s->msg = fake_resp;
    pthread_mutex_lock(&s->mut);
    pthread_cond_signal(&s->cond);
    pthread_mutex_unlock(&s->mut);
}

struct rpcserve_info {
    int rpcfd;
    void (*error)(int, int);
};


struct rpcctx_info {
    pthread_mutex_t mut;
    pthread_mutex_t bigmut; // So that the context is not destroyed before rpcctx_perform is done
    CFMutableArrayRef messages;
    int rpcfd;
};

void rpcctx_perform(void *info_) {
    struct rpcctx_info *info = info_;
    pthread_mutex_lock(&info->bigmut);
    int rpcfd = info->rpcfd;
    while(1) {
        pthread_mutex_lock(&info->mut);
        if(!CFArrayGetCount(info->messages)) {
            pthread_mutex_unlock(&info->mut);
            break;
        }
        
        struct hdr *msg_ = (void *) CFArrayGetValueAtIndex(info->messages, 0);
        CFArrayRemoveValueAtIndex(info->messages, 0);
        pthread_mutex_unlock(&info->mut);
        switch(msg_->funcid_or_status) {
            SERVER
        }
        pthread_mutex_destroy(&info->mut);
        free(msg_);
    }
    pthread_mutex_unlock(&info->bigmut);
}

struct rpcerrctx_info {
    int rpcfd;
    CFRunLoopSourceRef src;
    int ret;
    void (*error)(int, int);
};

void rpcerrctx_perform(void *info_) {
    struct rpcerrctx_info *info = info_;
    CFRunLoopSourceInvalidate(info->src);
    CFRelease(info->src);
    info->error(info->rpcfd, info->ret);

    free(info);
}

void *rpcserve_thread(void *info_) {
    struct rpcserve_info *info = info_;
    pthread_mutex_lock(&rpc_states_mutex);
    rpcserve_alive = true;
    pthread_mutex_unlock(&rpc_states_mutex);


    CFRunLoopSourceContext ctx;
    memset(&ctx, 0, sizeof(ctx));
    struct rpcctx_info *rpcinfo = malloc(sizeof(*rpcinfo));
    ctx.info = rpcinfo;
    ctx.perform = rpcctx_perform;

    pthread_mutex_init(&rpcinfo->mut, NULL);
    pthread_mutex_init(&rpcinfo->bigmut, NULL);
    rpcinfo->rpcfd = info->rpcfd;
    rpcinfo->messages = CFArrayCreateMutable(NULL, 0, NULL);

    CFRunLoopSourceRef asrc = CFRunLoopSourceCreate(NULL, 0, &ctx);
    CFRunLoopAddSource(CFRunLoopGetMain(), asrc, kCFRunLoopCommonModes);


    int ret = rpcserve_thread_(info->rpcfd, asrc, &rpcinfo->mut, rpcinfo->messages);
    
    CFRunLoopSourceInvalidate(asrc);
    CFRelease(asrc);

    // Mark failure
    pthread_mutex_lock(&rpc_states_mutex);
    rpcserve_alive = false;
    pthread_mutex_unlock(&rpc_states_mutex);
    CFDictionaryApplyFunction(rpc_states, rpc_applier, NULL);
    if(info->error) {
        CFRunLoopSourceContext ctx;
        memset(&ctx, 0, sizeof(ctx));
        struct rpcerrctx_info *errinfo = malloc(sizeof(*errinfo));
        ctx.info = errinfo;
        ctx.perform = rpcerrctx_perform;
        CFRunLoopSourceRef src = CFRunLoopSourceCreate(NULL, 0, &ctx);
        errinfo->src = src;
        errinfo->rpcfd = info->rpcfd;
        errinfo->ret = ret;
        errinfo->error = info->error;
        CFRunLoopAddSource(CFRunLoopGetMain(), src, kCFRunLoopCommonModes);
        CFRunLoopSourceSignal(src);
        CFRunLoopWakeUp(CFRunLoopGetMain());
    }
    pthread_mutex_lock(&rpcinfo->bigmut);
    pthread_mutex_unlock(&rpcinfo->bigmut);
    pthread_mutex_destroy(&rpcinfo->bigmut);
    pthread_mutex_destroy(&rpcinfo->mut);
    CFRelease(rpcinfo->messages);
    free(rpcinfo);
    free(info);
    return NULL;
}

void rpcserve(int rpcfd, void (*error)(int, int)) {
    pthread_t thread;
    struct rpcserve_info *info = malloc(sizeof(*info));
    info->rpcfd = rpcfd;
    info->error = error;
    pthread_create(&thread, NULL, rpcserve_thread, info);
}

int rpcserve_thread_(int rpcfd, CFRunLoopSourceRef src, pthread_mutex_t *mut, CFMutableArrayRef array) {
    while(1) {
        struct hdr hdr;
        if(recv(rpcfd, &hdr, sizeof(hdr), MSG_WAITALL) != sizeof(hdr)) {
            if(errno == EAGAIN) continue;
            return -errno;
        }
        if(hdr.magic != 0x12345678) {
            fprintf(stderr, "WARNING WARNING: bad magic\\n");
            return 1;
        }
        //fprintf(stderr, "msgsize = %d\\n", hdr.msgsize);
        if(hdr.msgsize < sizeof(hdr)) {
            fprintf(stderr, "WARNING WARNING: tiny message (%zd)\\n", hdr.msgsize);
            return 2;
        }
        if(hdr.msgsize >= 1048576*50) return 3;
        struct hdr *msg_ = malloc(hdr.msgsize);
        memcpy(msg_, &hdr, sizeof(hdr));
        if(recv(rpcfd, msg_->extra, hdr.msgsize - sizeof(*msg_), MSG_WAITALL) != hdr.msgsize - sizeof(*msg_)) return -errno;
        if(hdr.is_resp) {
            pthread_mutex_lock(&rpc_states_mutex);
            struct { pthread_cond_t cond; pthread_mutex_t mut; int src_fd; void *msg; } *s = \\
            (void *) CFDictionaryGetValue(rpc_states, (void *) hdr.msgid);
            CFDictionaryRemoveValue(rpc_states, (void *) hdr.msgid);
            pthread_mutex_unlock(&rpc_states_mutex);
            //fprintf(stderr, "got a response for msgid %d (%p)\\n", hdr.msgid, s);
            if(s && s->src_fd == rpcfd) {
                s->msg = msg_;
                pthread_mutex_lock(&s->mut);
                pthread_cond_signal(&s->cond);
                pthread_mutex_unlock(&s->mut);
            } else {
                free(msg_);
            }
            continue;
        }
        // This part (only) should add something to a run loop
        pthread_mutex_lock(mut);
        CFArrayAppendValue(array, msg_);
        pthread_mutex_unlock(mut);
        CFRunLoopSourceSignal(src);
        CFRunLoopWakeUp(CFRunLoopGetMain());
    }
}

'''.replace('SERVER', server.replace('\n', '\n\t\t\t').replace('\t', '    '))

