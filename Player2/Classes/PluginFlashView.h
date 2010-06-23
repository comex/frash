//
//  PluginFlashView.h
//  Player2
//
//  Created by Nicholas Allegra on 6/21/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#define IOSFC_BUILDING_IOSFC
#import "IOSurface.h"
#import "server.h"

@interface PluginFlashView : UIView {
	NSDictionary *arguments;
	UILabel *label;
	
	IOSurfaceRef sfc;
	int rpcfd;
	Server *server;
}
@property (retain) Server *server;

- (void)useSurface:(IOSurfaceRef)sfc;
- (void)displaySync;
- (CGSize)movieSize;
- (void)diedWithError:(NSString *)error;

@end
