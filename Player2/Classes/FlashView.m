//
//  FlashView.m
//  Player2
//
//  Created by Nicholas Allegra on 6/16/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "FlashView.h"
#import <QuartzCore/QuartzCore.h>
#include "food_rpc2.h"
#include "server.h"


@implementation FlashView
@synthesize rpcfd;
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
	for(UITouch *t in touches) { \
		CGPoint location = [t locationInView:self]; \
		touch(rpcfd, num, location.x, location.y); \
	} \
} 

foo(touchesBegan, kDown_ANPTouchAction)
foo(touchesMoved, kMove_ANPTouchAction)
foo(touchesEnded, kUp_ANPTouchAction)
foo(touchesCancelled, kCancel_ANPTouchAction)

- (void)dealloc {
	// todo actually stop this thread
    [super dealloc];
}

- (void)goToURL:(NSURL *)URL inFrame:(NSString *)frame {
	NSLog(@"goToURL:%@ inFrame:%@", URL, frame);
}


@end
