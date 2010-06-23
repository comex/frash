//
//  PluginFlashView.m
//  Player2
//
//  Created by Nicholas Allegra on 6/21/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "PluginFlashView.h"
#import <QuartzCore/QuartzCore.h>
#include "food_rpc2.h"

@interface NSObject (FakeMethodsToMakeGCCShutUp)

- (id)webFrame;
- (id)windowObject;
- (WebFrame *)findFrameNamed:(NSString *)name;

@end


@implementation PluginFlashView

- (void)setSizeIfNecessary {
	NSLog(@"setSizeIfNecessary: I am %@", self);
	CGSize size = [self movieSize];
	if(size.width && size.height && rpcfd)
		set_movie_size(rpcfd, size.width, size.height);	
}	


- (void)webPlugInStop {
	if(sfc) {
		CFRelease(sfc);
		sfc = NULL;
	}
	[self.server teardown];
	if(label) {
		[label removeFromSuperview];
		[label release];
		label = nil;
	}
	self.layer.contents = nil;
	rpcfd = 0;
}


- (void)webPlugInStart {
	NSLog(@"webPlugInStart");
	[self webPlugInStop];	
	self.server = [[[Server alloc] initWithDelegate:self] autorelease];
	rpcfd = self.server.rpc_fd;	
	[self setSizeIfNecessary];	
}


- (id)initWithArguments:(NSDictionary *)arguments_ {
	if(self = [super init]) {
		arguments = [arguments_ retain];
		//self.contentMode = UIViewContentModeRedraw;		
		//self.backgroundColor = [UIColor grayColor];
	}
	return self;	   
}

- (void)setFrame:(CGRect)frame {
	[super setFrame:frame];
	NSLog(@"Frame changed");
	[self setSizeIfNecessary];
	label.frame = self.bounds;	
}

+ (UIView *)plugInViewWithArguments:(NSDictionary *)arguments
{
	
	NSLog(@"IPAD WEB VIEW ARGS = %@", [arguments description]);
	
	//NSDictionary *pluginDict = [newArguments objectForKey:@"WebPlugInAttributesKey"];	
	//NSString *flashURL = [pluginDict objectForKey:@"src"];
	
	id x = [[[PluginFlashView alloc] initWithArguments:arguments] autorelease];
	NSLog(@"It is %@", x);
    return x;
}

- (NSURL *)baseURL {
	return [arguments objectForKey:@"WebPlugInBaseURLKey"]/*[NSURL URLWithString:]*/;
}

- (id)getWindowObject {
	return [[[arguments objectForKey:@"WebPlugInContainerKey"] webFrame] windowObject];
}

- (id)evaluateWebScript:(NSString *)script {
	NSLog(@"Evaluate %@", script);
	return [[self getWindowObject] evaluateWebScript:script];
}

- (void)useSurface:(IOSurfaceRef)sfc_ {
	if(sfc) CFRelease(sfc);
	sfc = sfc_;
	
}

- (void)displaySync {
	if(!sfc || !IOSurfaceGetWidth(sfc)) return;
/*	CGDataProviderRef provider = CGDataProviderCreateWithData(NULL, IOSurfaceGetBaseAddress(sfc), IOSurfaceGetAllocSize(sfc), NULL);
	CGImageRef image = CGImageCreate(
									 IOSurfaceGetWidth(sfc),
									 IOSurfaceGetHeight(sfc),
									 8,
									 32,
									 4 * IOSurfaceGetWidth(sfc),
									 CGColorSpaceCreateDeviceRGB(),
									 kCGBitmapByteOrder32Big | kCGImageAlphaPremultipliedLast,
									 provider,
									 NULL,
									 true,
									 kCGRenderingIntentDefault);
	
	CFRelease(provider);
	CFRelease(data);
	self.layer.contents = (id) image;
 	CFRelease(image);*/
	self.layer.contents = (id) sfc;
	self.layer.opacity = self.layer.opacity == 1.0 ? 0.9999 : 1.0;
}

- (NSDictionary *)paramsDict {
	return [arguments objectForKey:@"WebPlugInAttributesKey"];
}

- (CGSize)movieSize {
	return self.frame.size;
}

- (void)diedWithError:(NSString *)error {
	NSLog(@"Error: %@", error);
	if(label) { [label removeFromSuperview]; [label release]; }
	label = [[UILabel alloc] init];
	label.backgroundColor = [UIColor colorWithWhite:0.5 alpha:0.3]; // dim the image
	label.text = error;		
	label.frame = self.bounds;
	label.textAlignment = UITextAlignmentCenter;
	label.textColor = [UIColor whiteColor];
	//label.shadowColor = [UIColor whiteColor];
	//label.shadowOffset = CGSizeMake(0, 1);
	[self addSubview:label];
}


- (void)dealloc {
	[arguments release];
	[label release];
	if(sfc) CFRelease(sfc);
	[self.server teardown];
    [super dealloc];
}

#define kDown_ANPTouchAction        0
#define kUp_ANPTouchAction          1
#define kMove_ANPTouchAction        2
#define kCancel_ANPTouchAction      3

- (id)initWithFrame:(CGRect)frame {
    if ((self = [super initWithFrame:frame])) {
		CALayer *lyr = self.layer;
		lyr.backgroundColor = [[UIColor blackColor] CGColor];
		self.multipleTouchEnabled = YES;
    }
    return self;
}

#define foo(func, num) \
- (void)func:(NSSet *)touches withEvent:(UIEvent *)event { \
	if(rpcfd) {\
		for(UITouch *t in touches) {\
			CGPoint location = [t locationInView:self]; \
			touch(rpcfd, num, location.x, location.y); \
		} \
	}\
} 

foo(touchesBegan, kDown_ANPTouchAction)
foo(touchesMoved, kMove_ANPTouchAction)
foo(touchesEnded, kUp_ANPTouchAction)
foo(touchesCancelled, kCancel_ANPTouchAction)

- (void)goToURL:(NSURL *)URL inFrame:(NSString *)frameName {
	WebFrame *myframe = [[arguments objectForKey:@"WebPlugInContainerKey"] webFrame];
	WebFrame *frame = [myframe findFrameNamed:frameName];
	if(!frame) {
		// It really should open a new window
		frame = myframe;
	}
	[frame loadRequest:[NSURLRequest requestWithURL:URL]];
}

@synthesize server;
@end
