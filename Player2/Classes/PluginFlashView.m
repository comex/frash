//
//  PluginFlashView.m
//  Player2
//
//  Created by Nicholas Allegra on 6/21/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "PluginFlashView.h"
#import <QuartzCore/QuartzCore.h>

@interface NSObject (FakeMethodsToMakeGCCShutUp)

- (id)webFrame;
- (id)windowObject;

@end


@implementation PluginFlashView



- (id)initWithArguments:(NSDictionary *)arguments_ {
	if(self = [super init]) {
		arguments = [arguments_ retain];
		self.backgroundColor = [UIColor grayColor];
	}
	return self;	   
}

+ (UIView *)plugInViewWithArguments:(NSDictionary *)arguments
{
	
	NSLog(@"IPAD WEB VIEW ARGS = %@", [arguments description]);
	
	//NSDictionary *pluginDict = [newArguments objectForKey:@"WebPlugInAttributesKey"];	
	//NSString *flashURL = [pluginDict objectForKey:@"src"];
	
    return [[[self alloc] initWithArguments:arguments] autorelease];
}



- (id)getWindowObject {
	return [[[arguments objectForKey:@"WebPlugInContainerKey"] webFrame] windowObject];
}

- (id)evaluateWebScript:(NSString *)script {
	NSLog(@"Evaluate %@", script);
	return [[self getWindowObject] evaluateWebScript:script];
}

- (void)useSurface:(IOSurfaceRef)sfc_ {
	sfc = sfc_;
	if(provider) CGDataProviderRelease(provider);
	provider = CGDataProviderCreateWithData(NULL, IOSurfaceGetBaseAddress(sfc), IOSurfaceGetAllocSize(sfc), NULL);
	oldContents = NULL;
	
}

- (void)displaySync {
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
	self.layer.contents = (id) image;
	if(oldContents) CGImageRelease(oldContents);
	oldContents = image;
}

- (NSDictionary *)paramsDict {
	return [arguments objectForKey:@"WebPlugInAttributesKey"];
}

- (CGSize)movieSize {
	return self.frame.size;
}

- (void)diedWithError:(NSString *)error {
	NSLog(@"Error: %@", error);
	
	label = [[UILabel alloc] init];
	label.backgroundColor = [UIColor colorWithWhite:0.0 alpha:0.3]; // dim the image
	label.text = error;		
	label.frame = self.bounds;
	label.textAlignment = UITextAlignmentCenter;
	label.shadowColor = [UIColor blackColor];
	label.shadowOffset = CGSizeMake(0, 1);
	[self addSubview:label];
}


- (void)dealloc {
	[arguments release];
	[label release];
	if(oldContents) CGImageRelease(oldContents);	
    [super dealloc];
}

@synthesize server;
@end
