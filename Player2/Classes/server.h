#pragma once
#import <Foundation/Foundation.h>

@interface Server : NSObject {
	id delegate;
	CFRunLoopSourceRef serv_source;
	int serv_fd;
	CFRunLoopSourceRef rpc_source;
	int rpc_fd;
	pid_t pid;	
	NSMutableArray *objects;
}
- (id)initWithDelegate:(id)delegate;
// private
- (void)dieWithError:(NSString *)error;
- (void)teardown;
@property (assign) int rpc_fd;
@end

