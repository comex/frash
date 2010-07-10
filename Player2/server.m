#include <stdio.h>
#include "food_rpc2.h"
#include <stdlib.h>
#include <netinet/in.h>
#include <assert.h>
#include <pthread.h>
#include <fcntl.h>
#include <spawn.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <CoreFoundation/CoreFoundation.h>
#define IOSFC_BUILDING_IOSFC
#include "IOSurface.h"
#include "server.h"
#import <Foundation/Foundation.h>
#include <unistd.h>
#undef __BLOCKS__
#include <notify.h>
#include <QuartzCore/QuartzCore.h>

#define BORING 0
#define FOOD_ROOT "/var/mobile/"
#define FOOD_BINARY FOOD_ROOT "/food"

// C and Obj-C threading don't really play very well together.

static NSMutableDictionary *servers;
static FServer *get_server(int rpc_fd) {
	return [servers objectForKey:[NSNumber numberWithInt:rpc_fd]];
}
/*@interface WebUndefined
+ (id)undefined;
@end*/
@interface URLDude : NSThread {
	stream_t stream;
	int rpc_fd;
	NSURLConnection *connection;
	NSMutableURLRequest *request;
	BOOL shouldKeepRunning;
}

@property (readonly) NSMutableURLRequest *request;

@end
@implementation URLDude

@synthesize request;
- (id)initWithStream:(stream_t)stream_ URL:(NSURL *)url rpc_fd:(int)rpc_fd_ {
	if(self = [super init]) {
		stream = stream_;
		rpc_fd = rpc_fd_;
		request = [[NSMutableURLRequest alloc] initWithURL:url];
		
	}
	return self;
}
- (void)dealloc {
	[connection release];
	[request release];
	[super dealloc];
}
- (void)main {
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	connection = [[NSURLConnection alloc] initWithRequest:request delegate:self];
	if(!connection) {
		NSLog(@"It didn't even start. :(");
		connection_all_done(rpc_fd, stream, false);
		return;
	} else {
		NSLog(@"FYI I made a connection %@", connection);
		NSRunLoop *theRL = [NSRunLoop currentRunLoop];
		shouldKeepRunning = YES;
		while (shouldKeepRunning && [theRL runMode:NSDefaultRunLoopMode beforeDate:[NSDate distantFuture]]);
	}
	[pool release];
	
}
- (void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)response {
	NSLog(@"did receive response");
	NSMutableData *headers = [[NSMutableData alloc] init];
	[headers appendData:[@"HTTP/1.1 200 OK\n" dataUsingEncoding:NSUTF8StringEncoding]];
	if([response isKindOfClass:[NSHTTPURLResponse class]]) {
		NSDictionary *headerFields = [(id)response allHeaderFields];
		for(NSString *key in headerFields) {
			NSString *value = [headerFields objectForKey:key];
			[headers appendData:[[NSString stringWithFormat:@"%@: %@\n", key, value] dataUsingEncoding:NSUTF8StringEncoding]];
		}
	}
	connection_response(rpc_fd, stream, (char *) [headers bytes], [headers length], [response expectedContentLength]);
	[headers release];
}

- (void)connection:(NSURLConnection *)connection_ didFailWithError:(NSError *)error {
	NSLog(@"Error: %@", error);
	connection_all_done(rpc_fd, stream, false);
	shouldKeepRunning = NO;
}

- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data {
	//NSLog(@"connectionDidReceiveData");
	connection_got_data(rpc_fd, stream, (void *) [data bytes], [data length]);
}

- (void)connectionDidFinishLoading:(NSURLConnection *)connection_ {
	NSLog(@"connectionDidFinishLoading");
	connection_all_done(rpc_fd, stream, true);
	shouldKeepRunning = NO;
}
@end

@implementation FServer



static void error(int rpc_fd, int err) {
	FServer *self = get_server(rpc_fd);
	NSString *str;
	if(err == 0) {
		str = [NSString stringWithFormat:@"Unexpected error"];
	} else if(err < 0) {
		str = [NSString stringWithFormat:@"Socket error: %s", strerror(-err)];
	} else {
		str = [NSString stringWithFormat:@"Internal error: %d", err];
	}
	[self dieWithError:str];
}

- (id)initWithDelegate:(id)delegate_ {
	self = [super init];
	
	objects = [[NSMutableArray alloc] init];

	delegate = delegate_;
	rpc_fd = socket(AF_INET, SOCK_STREAM, 0);
    assert(rpc_fd > 0);
	if(!servers) servers = [[NSMutableDictionary alloc] init];
	[servers setObject:self forKey:[NSNumber numberWithInt:rpc_fd]];
	
	struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(0x7f000001);
	addr.sin_port = htons(4242);

	struct timeval tv;
	tv.tv_sec = 100;
	tv.tv_usec = 0;
	assert(!setsockopt(rpc_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)));
	assert(!setsockopt(rpc_fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv)));
			   

	int ret = connect(rpc_fd, (struct sockaddr *) &addr, sizeof(addr));
	if(ret != 0) {
		[self dieWithError:[NSString stringWithFormat:@"Could not connect: %s", strerror(errno)]];
		return self;
	}
	int big = 120000;
	assert(!setsockopt(rpc_fd, SOL_SOCKET, SO_SNDBUF, &big, sizeof(big)));
	assert(!setsockopt(rpc_fd, SOL_SOCKET, SO_RCVBUF, &big, sizeof(big)));
	signal(SIGPIPE, SIG_IGN);
	rpcserve(rpc_fd, error);
	
	return self;
}

- (void)dieWithError:(NSString *)error {
	[delegate performSelector:@selector(diedWithError:) withObject:error];
	[self teardown];
}

- (void)teardown {
	if(sekrit) {
		NSLog(@"Posting %s", sekrit);
		notify_post(sekrit);
		free(sekrit);
		sekrit = NULL;
	}
	
	if(rpc_source) {
		CFRunLoopRemoveSource(CFRunLoopGetMain(), rpc_source, kCFRunLoopCommonModes);
		rpc_source = NULL;
	}
	if(rpc_fd) {
		close(rpc_fd);
		[servers removeObjectForKey:[NSNumber numberWithInt:rpc_fd]];
		rpc_fd = 0;
	}
	
	delegate = nil;

	id objects_ = objects;
	objects = nil;
	[objects_ release];
}

- (void)dealloc {
	[self teardown];
	[super dealloc];
}

int set_sekrit(int rpc_fd, void *sekrit_, size_t sekrit_len) {
	FServer *self = get_server(rpc_fd);
	NSLog(@"sekrit: %s", sekrit_);
	if(!self->sekrit) self->sekrit = sekrit_;
	return 0;
}

int abort_msg(int rpc_fd, void *message, size_t message_len) {
	FServer *self = get_server(rpc_fd);
	NSString *str = [[[NSString alloc] initWithBytes:message length:message_len encoding:NSUTF8StringEncoding] autorelease];
	[self dieWithError:str];
	return 0;
}

int use_surface(int rpc_fd, int surface) {
	FServer *self = get_server(rpc_fd);
	IOSurfaceRef sfc = IOSurfaceLookup(surface);
	if(!sfc) return 1;
	[self->delegate performSelector:@selector(useSurface:) withObject:(id)sfc];
	return 0;
}

int display_sync(int rpc_fd, int l, int t, int r, int b) {
	FServer *self = get_server(rpc_fd);
	CGRect rect = CGRectMake(l, t, r-l, b-t);
	[self->delegate performSelector:@selector(displaySyncInRect:) withObject:(id)&rect];
	return 0;
}

int new_post_connection(int rpc_fd, stream_t stream, void *url, size_t url_len, void *target, size_t target_len, bool isfile, void *buf, size_t buf_len, void **url_abs, size_t *url_abs_len) {
	FServer *self = get_server(rpc_fd);
	NSString *str = [[[NSString alloc] initWithBytes:url length:url_len encoding:NSUTF8StringEncoding] autorelease];
	NSURL *nsurl = [NSURL URLWithString:str relativeToURL:[self->delegate baseURL]];

	NSLog(@"posting %i bytes to %s", buf_len, (char *)url);
	if(target_len > 0) {
		// This is even more wrong for posts, what should we do here?
		NSLog(@"new_post_connection: with target %s -- ignoring", target);
	}
	
	NSString *abs = [nsurl absoluteString];
	NSData *data = [abs dataUsingEncoding:NSUTF8StringEncoding];
	*url_abs = (void *) [data bytes];
	*url_abs_len = [data length];
	URLDude *dude = [[URLDude alloc] initWithStream:stream URL:nsurl rpc_fd:rpc_fd];
	[dude.request setHTTPMethod:@"POST"];
	[dude.request setHTTPBody:[NSData dataWithContentsOfFile:[[[NSString alloc] initWithBytes:buf length:buf_len encoding:NSUTF8StringEncoding] autorelease]]];
	[dude start];
	[dude autorelease];
	free(target);
	free(url);
	return 0;
}

int new_get_connection(int rpc_fd, stream_t stream, void *url, size_t url_len, void *target, size_t target_len, void **url_abs, size_t *url_abs_len) {
	FServer *self = get_server(rpc_fd);
	NSString *str = [[[NSString alloc] initWithBytes:url length:url_len encoding:NSUTF8StringEncoding] autorelease];
	NSURL *nsurl = [NSURL URLWithString:str relativeToURL:[self->delegate baseURL]];
	if(target_len > 0) {
		// This is wrong.
		NSLog(@"new_connection with target %s", target);
		
		NSString *str2 = [[[NSString alloc] initWithBytes:target length:target_len encoding:NSUTF8StringEncoding] autorelease];
		[self->delegate performSelector:@selector(goToURL:inFrame:) withObject:nsurl withObject:str2];
		*url_abs = url;
		*url_abs_len = url_len;
		return 0;
	}
	
	if(!memcmp(url, "javascript:", 11)) {
		NSString *ret = [[self->delegate performSelector:@selector(evaluateWebScript:) withObject:str] description];
		NSData *data = [ret dataUsingEncoding:NSUTF8StringEncoding];
		connection_response(rpc_fd, stream, "", 0, [data length]);
		connection_got_data(rpc_fd, stream, (void *) [data bytes], [data length]);
		connection_all_done(rpc_fd, stream, true);
		*url_abs = url;
		*url_abs_len = url_len;
		// leak
		return 0;
	}
	
	NSString *abs = [nsurl absoluteString];
	NSData *data = [abs dataUsingEncoding:NSUTF8StringEncoding];
	*url_abs = (void *) [data bytes];
	*url_abs_len = [data length];
	URLDude *dude = [[URLDude alloc] initWithStream:stream URL:nsurl rpc_fd:rpc_fd];
	[dude start];
	[dude autorelease];
	free(target);
	free(url);
	return 0;
}
	 
int get_parameters(int rpc_fd, void **params, size_t *params_len, int *params_count) {
	NSLog(@"GET_PARAMETERS");
	FServer *self = get_server(rpc_fd);
	NSDictionary *dict = [self->delegate performSelector:@selector(paramsDict)];
	
	*params_count = [dict count];
	NSMutableData *data = [NSMutableData data];
	NSMutableData *nullo = [NSData dataWithBytes:"" length:1];
	for(NSString *key in dict) {
		[data appendData:[key dataUsingEncoding:NSUTF8StringEncoding]];
		[data appendData:nullo];
		[data appendData:[[dict objectForKey:key] dataUsingEncoding:NSUTF8StringEncoding]];
		[data appendData:nullo];
	}
	
	*params = (void *) [data bytes];
	*params_len = [data length];
	
	return 0;
}

// runtime crap
- (int)nameForObject:(id)object {
	if(object == nil) return 0;
	for(int i = 0; i < [objects count]; i++) {
		if([objects objectAtIndex:i] == object) {
			return i + 1;
		}
	}
	int ret = [objects count];
	[objects addObject:object];
	return ret + 1;
}

- (id)objectForName:(int)name {
	if(name == 0 || name > [objects count]) return nil;
	return [objects objectAtIndex:(name - 1)];
}

int get_string_value(int rpc_fd, int obj, bool *valid, void **value, size_t *value_len) {
	FServer *self = get_server(rpc_fd);
	NSString *str = [self objectForName:obj];
	NSLog(@"get_string_value %@", str);
	if([str isKindOfClass:[NSString class]]) {
		*valid = true;
		NSData *data = [str dataUsingEncoding:NSUTF8StringEncoding];
		*value = (void *) [data bytes];
		*value_len = [data length];
	} else {
		*valid = false;
		*value = NULL; *value_len = 0;
	}
	return 0;
}

int get_object_property(int rpc_fd, int obj, void *property, size_t property_len, int *obj2) {
	FServer *self = get_server(rpc_fd);
	if(![self->delegate performSelector:@selector(isOn)]) return 1;
	NSString *str = [[NSString alloc] initWithBytes:property length:property_len encoding:NSUTF8StringEncoding];
	id base = [self objectForName:obj];
	id prop = [base valueForKey:str];
	NSLog(@"%@[%@] = %@", base, str, prop);
	*obj2 = [self nameForObject:prop];
	[str release];
	free(property);
	return 0;
}

int get_string_object(int rpc_fd, void *string, size_t string_len, int *obj2) {
	FServer *self = get_server(rpc_fd);
	*obj2 = [self nameForObject:[[[NSString alloc] initWithBytes:string length:string_len encoding:NSUTF8StringEncoding] autorelease]];
	return 0;
}

int get_int_object(int rpc_fd, int theint, int *obj2) {
	FServer *self = get_server(rpc_fd);
	*obj2 = [self nameForObject:[NSNumber numberWithInt:theint]];
	return 0;
}


int invoke_object_property(int rpc_fd, int obj, void *property, size_t property_len, void *args, size_t args_len, int *obj2) {
	FServer *self = get_server(rpc_fd);
	if(![self->delegate performSelector:@selector(isOn)]) return 1;
	NSString *str = [[NSString alloc] initWithBytes:property length:property_len encoding:NSUTF8StringEncoding];
	free(property);
	id base = [self objectForName:obj];
	NSMutableArray *array = [NSMutableArray array];
	int *p = args;
	while(args_len > sizeof(int)) {
		id obj = [self objectForName:*p++];
		
		[array addObject:obj];
		args_len -= sizeof(int);
	}
	free(args);
	id ret = [base performSelector:@selector(callWebScriptMethod:withArguments:) withObject:str withObject:array];
	/*if(ret == [WebUndefined undefined]) {
		NSLog(@"Invocation returned undefined");
		ret = nil;
	}*/
	NSLog(@"%@[%@](%@) = %@", base, str, array, ret);
	[str release];
	*obj2 = [self nameForObject:ret];
	return 0;
	
}

int get_window_object(int rpc_fd, int *obj) {
	NSLog(@"Getting window object");
	FServer *self = get_server(rpc_fd);
	if(![self->delegate performSelector:@selector(isOn)]) return 1;
	id windowObject = [self->delegate performSelector:@selector(getWindowObject)]; // windowScriptObject
	*obj = [self nameForObject:windowObject];
	NSLog(@"Getting window object %@ -> %d", windowObject, *obj);
	return 0;
}

int evaluate_web_script(int rpc_fd, void *script, size_t script_len, int *obj) {
	FServer *self = get_server(rpc_fd);
	NSString *str = [[NSString alloc] initWithBytes:script length:script_len encoding:NSUTF8StringEncoding];
	id resultObject = [self->delegate performSelector:@selector(evaluateWebScript:) withObject:str];
	*obj = [self nameForObject:resultObject];
	[str release];
	free(script);
	return 0;
}
@synthesize rpc_fd;
@end

