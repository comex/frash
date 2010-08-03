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
- (void)webPlugInContainerLoadRequest:(NSURLRequest *)request inFrame:(NSString *)target;
- (CGFloat)scale;

@end

@implementation PluginFlashView

- (void)setSizeIfNecessary {
	NSLog(@"setSizeIfNecessary: I am %@", self);
	CGSize size = [self movieSize];
	if(size.width && size.height && rpcfd)
		set_movie_size(rpcfd, size.width, size.height);	
}	


- (void)webPlugInStop {
	if(!on) { started = false; return; }
	if(sfc) {
		CFRelease(sfc);
		sfc = NULL;
	}
	[self.server teardown];
	if(errorLabel) {
		[errorLabel removeFromSuperview];
		[errorLabel release];
		errorLabel = nil;
	}
	self.layer.contents = nil;
	rpcfd = 0;
}


- (void)webPlugInStart {
	if(!on) { started = YES; return; }
	NSLog(@"webPlugInStart");
	[self webPlugInStop];	
	self.server = [[[FServer alloc] initWithDelegate:self] autorelease];
	rpcfd = self.server.rpc_fd;	
	[self setSizeIfNecessary];	
}

- (BOOL)isOn {
	return on;
}

- (void)initialClicked {
	if(on) return;
	initialButton.userInteractionEnabled = NO;
	[UIView beginAnimations:nil context:nil];
	[UIView setAnimationDuration:0.15];
	[UIView setAnimationDelegate:self];
	[UIView setAnimationDidStopSelector:@selector(animationDidStop:finished:context:)];
	initialButton.alpha	= 0;
	[UIView commitAnimations];
	on = YES;
	if(started) [self webPlugInStart];
}


- (void)animationDidStop:(NSString *)animationID finished:(NSNumber *)finished context:(void *)context {
	[initialButton removeFromSuperview];
	[initialButton release];
	initialButton = nil;
}

- (void)initialDown {
	initialButton.backgroundColor = [UIColor colorWithWhite:(0x93/256.0) alpha:1.0];
}

- (void)initialUp {
	initialButton.backgroundColor = [UIColor colorWithWhite:(0xc3/256.0) alpha:1.0];
}

- (id)initWithArguments:(NSDictionary *)arguments_ {
	if(self = [super init]) {
		arguments = [arguments_ retain];
		id attributes = [arguments objectForKey:@"WebPlugInAttributesKey"];
		id w = [attributes objectForKey:@"width"];
		id h = [attributes objectForKey:@"height"];
		if(w && h) {
			int w_ = [w intValue];
			int h_ = [h intValue];
			//self.frame = CGRectMake (self.frame.origin.x, self.frame.origin.y, [w floatValue], [h floatValue]);
			if(w_ < 30 || h_ < 30) {
				on = YES;
				return self;
			}
		}
		initialButton = [[UIButton alloc] initWithFrame:self.frame];
		initialButton.backgroundColor = [UIColor colorWithWhite:(0xc3/256.0) alpha:1.0];
		[initialButton setTitleColor:[UIColor colorWithWhite:(0x3c/256.0) alpha:1.0] forState:UIControlStateNormal];
		[initialButton setTitleShadowColor:[UIColor colorWithWhite:1.0 alpha:0.53] forState:UIControlStateNormal];
		initialButton.titleLabel.shadowOffset = CGSizeMake(0.0, 1.0);
		[initialButton setTitle:@"Flash" forState:UIControlStateNormal];
		initialButton.reversesTitleShadowWhenHighlighted = YES;
		initialButton.hidden = NO;
		[initialButton addTarget:self action:@selector(initialClicked) forControlEvents:UIControlEventTouchUpInside];
		[initialButton addTarget:self action:@selector(initialDown) forControlEvents:UIControlEventTouchDown];
		[initialButton addTarget:self action:@selector(initialUp) forControlEvents:UIControlEventTouchUpOutside];
		[self addSubview:initialButton];
        self.autoresizesSubviews = YES;
        initialButton.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;

		//self.contentMode = UIViewContentModeRedraw;		
		//self.backgroundColor = [UIColor grayColor];
	}
	return self;	   
}

- (void)setFrame:(CGRect)frame {
	[super setFrame:frame];
	// For some reason, the frame is constantly fluctating to +/-1 height.
	if(oldFrameValid && fabsf(frame.size.height - oldFrame.size.height) < 1.5
					 && fabsf(frame.size.width - oldFrame.size.width) < 1.5) {
       // NSLog(@"Ignoring frame changae to %fx%f", frame.size.width, frame.size.height);
		return;
	}
	oldFrame = frame;
	oldFrameValid = YES;	
	NSLog(@"Frame changed (now %fx%f)", frame.size.width, frame.size.height);
	[self setSizeIfNecessary];
	initialButton.frame = self.bounds;
	errorLabel.frame = self.bounds;	
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
	self.layer.contents = (id) sfc;
	
}

- (void)displaySyncInRect:(CGRect *)rect {
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
	self.layer.opacity = self.layer.opacity == 1.000 ? 0.9999 : 1.000;
	//[self.layer performSelector:@selector(setContentsChanged)];
	/*if(rect->size.width) {
		[self.layer setNeedsDisplayInRect:*rect];
	} else {
		[self.layer setNeedsDisplay];
	}*/
}

- (NSDictionary *)paramsDict {
	return [arguments objectForKey:@"WebPlugInAttributesKey"];
}

- (CGSize)movieSize {
	CGSize origSize = self.frame.size;
	return CGSizeMake(origSize.width * scale, origSize.height * scale);
}

- (void)diedWithError:(NSString *)error {
	NSLog(@"Error: %@", error);
	if(errorLabel) { [errorLabel removeFromSuperview]; [errorLabel release]; }
	errorLabel = [[UILabel alloc] init];
	errorLabel.backgroundColor = [UIColor colorWithWhite:0.5 alpha:0.3]; // dim the image
	errorLabel.text = error;		
	errorLabel.frame = self.bounds;
	errorLabel.textAlignment = UITextAlignmentCenter;
	errorLabel.textColor = [UIColor whiteColor];
	[self addSubview:errorLabel];
}


- (void)dealloc {
	[arguments release];
	[initialButton release];	
	[errorLabel release];
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

        UIScreen *screen = [UIScreen mainScreen];
        if(![screen respondsToSelector:@selector(scale)]) {
            scale = 1.0;
        } else {
            scale = [screen scale];
        }
    }
    return self;
}

#define foo(func, num) \
- (void)func:(NSSet *)touches withEvent:(UIEvent *)event { \
	if(rpcfd) {\
		for(UITouch *t in touches) {\
			CGPoint location = [t locationInView:self]; \
			touch(rpcfd, num, location.x * scale, location.y * scale); \
		} \
	}\
} 

foo(touchesBegan, kDown_ANPTouchAction)
foo(touchesMoved, kMove_ANPTouchAction)
foo(touchesEnded, kUp_ANPTouchAction)
foo(touchesCancelled, kCancel_ANPTouchAction)

- (void)goToURL:(NSURL *)URL inFrame:(NSString *)frameName {
	[[arguments objectForKey:@"WebPlugInContainerKey"] webPlugInContainerLoadRequest: [NSURLRequest requestWithURL:URL] inFrame: frameName];
}

- (UIImage *)scaleImage:(UIImage *) image maxWidth:(float) maxWidth maxHeight:(float) maxHeight
{
	CGImageRef imgRef = image.CGImage;
	CGFloat width = CGImageGetWidth(imgRef);
	CGFloat height = CGImageGetHeight(imgRef);

	if (width <= maxWidth && height <= maxHeight)
	{
		return image;
	}

	CGAffineTransform transform = CGAffineTransformIdentity;
	CGRect bounds = CGRectMake(0, 0, width, height);

	if (width > maxWidth || height > maxHeight)
	{
		CGFloat ratio = width/height;

		if (ratio > 1)
		{
			bounds.size.width = maxWidth;
			bounds.size.height = bounds.size.width / ratio;
		}
		else
		{
			bounds.size.height = maxHeight;
			bounds.size.width = bounds.size.height * ratio;
		}
	}

	CGFloat scaleRatio = bounds.size.width / width;
	UIGraphicsBeginImageContext(bounds.size);
	CGContextRef context = UIGraphicsGetCurrentContext();
	CGContextScaleCTM(context, scaleRatio, -scaleRatio);
	CGContextTranslateCTM(context, 0, -height);
	CGContextConcatCTM(context, transform);
	CGContextDrawImage(UIGraphicsGetCurrentContext(), CGRectMake(0, 0, width, height), imgRef);

	UIImage *imageCopy = UIGraphicsGetImageFromCurrentImageContext();
	UIGraphicsEndImageContext();

	return imageCopy;

}

@synthesize server;
@end
