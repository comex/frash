    //
//  FlashViewController.m
//  Player2
//
//  Created by Nicholas Allegra on 6/13/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "FlashViewController.h"
#include "Player2AppDelegate.h"
#include "food_rpc2.h"
#include "server.h"
#import <QuartzCore/QuartzCore.h>

@implementation FlashViewController

/*
 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
    }
    return self;
}
*/

/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/

- (void)clicked:(id)sender {
	[self.server teardown];
	self.server = [[[FServer alloc] initWithDelegate:self] autorelease];
	rpcfd = ((FlashView *)self.view).rpcfd = self.server.rpc_fd;		
	CGSize size = [self movieSize];
	set_movie_size(rpcfd, size.width, size.height);
	
}

- (void)loadView {
	self.view = [[FlashView alloc] init];
	UIButton *button = [UIButton buttonWithType:UIButtonTypeRoundedRect];
	[button setTitle:@"Refresh" forState:UIControlStateNormal];
	button.frame = CGRectMake(0, 0, 100, 100);
	button.hidden = NO;
	[button addTarget:self action:@selector(clicked:) forControlEvents:UIControlEventTouchUpInside];
	NSLog(@"%@", button);
	[self.view addSubview:button];
}

- (void)viewDidAppear:(BOOL)animated {
	NSLog(@"viewDidAppear.");
	[self clicked:nil];
}




// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return YES;
}

- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation {
	CGSize size = [self movieSize];
	set_movie_size(rpcfd, size.width, size.height);
}

- (id)getWindowObject {
	return [NSDictionary dictionaryWithObject:@"file:///var/mobile/firestorm.html" forKey:@"location"];
}

- (NSDictionary *)paramsDict {
	return [NSDictionary dictionaryWithObject:@"file:///var/mobile/firestorm2.swf" forKey:@"src"];
}


- (id)evaluateWebScript:(NSString *)script {
	NSLog(@"Evaluate %@", script);
	return @"complete";
}

- (void)useSurface:(IOSurfaceRef)sfc_ {
	sfc = sfc_;
	if(provider) CGDataProviderRelease(provider);
	provider = CGDataProviderCreateWithData(NULL, IOSurfaceGetBaseAddress(sfc), IOSurfaceGetAllocSize(sfc), NULL);
	oldContents = NULL;
	
}

- (NSURL *)baseURL {
	return nil;
}

- (void)displaySyncInRect:(CGRect *)rect {
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
	self.view.layer.contents = (id) image;
	if(oldContents) CGImageRelease(oldContents);
	oldContents = image;
}

- (CGSize)movieSize {
	return self.view.frame.size;
}

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

- (void)diedWithError:(NSString *)error {
	NSLog(@"Error: %@", error);
}

- (void)dealloc {
    [super dealloc];
}

@synthesize server;
@end
