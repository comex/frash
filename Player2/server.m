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

#define BORING 0
#define FOOD_ROOT "/var/mobile/"
#define FOOD_BINARY FOOD_ROOT "/food"

// C and Obj-C threading don't really play very well together.

static NSMutableDictionary *servers;
static Server *get_server(int rpc_fd) {
	return [servers objectForKey:[NSNumber numberWithInt:rpc_fd]];
}

@interface URLDude : NSThread {
	stream_t stream;
	int rpc_fd;
	NSURLConnection *connection;
	NSURLRequest *request;
	BOOL shouldKeepRunning;
}
@end
@implementation URLDude
- (id)initWithStream:(stream_t)stream_ URL:(const char *)url rpc_fd:(int)rpc_fd_ {
	if(self = [super init]) {
		stream = stream_;
		rpc_fd = rpc_fd_;
		request = [[NSURLRequest alloc] initWithURL:[NSURL URLWithString:[NSString stringWithCString:url encoding:NSUTF8StringEncoding]]];
		
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
	NSData *headers;
	if([response isKindOfClass:[NSHTTPURLResponse class]]) {
		NSMutableData *headers_ = [[NSMutableData alloc] init];
		NSDictionary *headerFields = [(id)response allHeaderFields];
		for(NSString *key in headerFields) {
			NSString *value = [headerFields objectForKey:key];
			[headers_ appendData:[[NSString stringWithFormat:@"%@: %@", key, value] dataUsingEncoding:NSUTF8StringEncoding]];
		}
	} else {
		headers = [[NSData alloc] init];
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
	connection_got_data(rpc_fd, stream, (void *) [data bytes], [data length]);
}

- (void)connectionDidFinishLoading:(NSURLConnection *)connection_ {
	NSLog(@"ok it finished");
	connection_all_done(rpc_fd, stream, true);
	shouldKeepRunning = NO;
}
@end

@implementation Server



static void error(int rpc_fd, int err) {
	Server *self = get_server(rpc_fd);	
	NSString *str;
	if(err < 0) {
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
		[self performSelectorOnMainThread:@selector(dieWithError:)
							   withObject:[NSString stringWithFormat:@"Could not connect: %s", strerror(errno)]
							waitUntilDone:NO];
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
	if(rpc_source) {
		CFRunLoopRemoveSource(CFRunLoopGetMain(), rpc_source, kCFRunLoopCommonModes);	
		rpc_source = NULL;
	}
	if(rpc_fd) {
		close(rpc_fd);
		[servers removeObjectForKey:[NSNumber numberWithInt:rpc_fd]];
		rpc_fd = 0;
	}	

	id objects_ = objects;
	objects = nil;
	[objects_ release];
}

- (void)dealloc {
	[self teardown];
	[super dealloc];
}

int use_surface(int rpc_fd, int surface) {
	Server *self = get_server(rpc_fd);	
	IOSurfaceRef sfc = IOSurfaceLookup(surface);
	if(!sfc) return 1;
	[self->delegate performSelector:@selector(useSurface:) withObject:(id)sfc];
	return 0;
}

int display_sync(int rpc_fd) {
	Server *self = get_server(rpc_fd);
	[self->delegate performSelector:@selector(displaySync)];
	return 0;
}

int new_connection(int rpc_fd, stream_t stream, void *url, size_t url_len, void *target, size_t target_len) {
	if(!memcmp(url, "javascript:", 11)) {
		if(!strcmp(url, "javascript:top.location+\"__flashplugin_unique__\"")) return 1;
		char *ret = "file:///var/mobile/firestorm.html";
		connection_response(rpc_fd, stream, "", 0, strlen(ret));
		connection_got_data(rpc_fd, stream, ret, strlen(ret));
		connection_all_done(rpc_fd, stream, true);
		return 0;
	}
	[[[[URLDude alloc] initWithStream:stream URL:(char *)url rpc_fd:rpc_fd] autorelease] start];
	free(target);
	free(url);
	return 0;
}
	 
int get_parameters(int rpc_fd, void **params, size_t *params_len, int *params_count) {
	Server *self = get_server(rpc_fd);	
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
	Server *self = get_server(rpc_fd);	
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
	Server *self = get_server(rpc_fd);	
	NSString *str = [[NSString alloc] initWithBytes:property length:property_len encoding:NSUTF8StringEncoding];
	id base = [self objectForName:obj];
	id prop = [base valueForKey:str];
	NSLog(@"%@[%@] = %@", base, str, prop);
	*obj2 = [self nameForObject:prop];
	[str release];
	free(property);
	return 0;
}

int get_window_object(int rpc_fd, int *obj) {
	Server *self = get_server(rpc_fd);
	id windowObject = [self->delegate performSelector:@selector(getWindowObject)]; // windowScriptObject
	*obj = [self nameForObject:windowObject];
	NSLog(@"Getting window object %@ -> %d", windowObject, *obj);
	return 0;
}

int evaluate_web_script(int rpc_fd, void *script, size_t script_len, int *obj) {
	Server *self = get_server(rpc_fd);
	NSString *str = [[NSString alloc] initWithBytes:script length:script_len encoding:NSUTF8StringEncoding];	
	id resultObject = [self->delegate performSelector:@selector(evaluateWebScript:) withObject:str];
	*obj = [self nameForObject:resultObject];
	[str release];
	free(script);
	return 0;
}
@synthesize rpc_fd;
@end

