//
//  FlashViewController.h
//  Player2
//
//  Created by Nicholas Allegra on 6/13/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#define IOSFC_BUILDING_IOSFC
#include "IOSurface.h"
@class Server;

@interface FlashViewController : UIViewController {
	IOSurfaceRef sfc;
	CGImageRef oldContents;
	CGDataProviderRef provider;
	int rpcfd;
	Server *server;
}
@property (retain) Server *server;
- (void)useSurface:(IOSurfaceRef)sfc;
- (void)displaySync;
- (CGSize)movieSize;
- (void)diedWithError:(NSString *)error;

@end
