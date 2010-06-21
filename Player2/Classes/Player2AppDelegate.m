//
//  Player2AppDelegate.m
//  Player2
//
//  Created by Nicholas Allegra on 6/3/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//
#include <mach/mach.h>
#import <QuartzCore/QuartzCore.h>
#import "Player2AppDelegate.h"
#import "FlashViewController.h"
#import "FlashView.h"
#include "server.h"


@implementation Player2AppDelegate

@synthesize window;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {    

    // Override point for customization after application launch
	window.backgroundColor = [UIColor purpleColor];
    [window makeKeyAndVisible];	
	UIViewController *viewController = [[FlashViewController alloc] init];	

	viewController.view.frame = window.bounds;
	window.autoresizesSubviews = YES;
	window.autoresizingMask = UIViewAutoresizingFlexibleLeftMargin |
	UIViewAutoresizingFlexibleRightMargin |
	UIViewAutoresizingFlexibleTopMargin |
	UIViewAutoresizingFlexibleBottomMargin |
	UIViewAutoresizingFlexibleWidth |
	UIViewAutoresizingFlexibleHeight;
	[window addSubview:viewController.view];	
	return YES;
}


- (void)dealloc {
    [window release];
    [super dealloc];
}

@end

