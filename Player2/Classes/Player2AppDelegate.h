//
//  Player2AppDelegate.h
//  Player2
//
//  Created by Nicholas Allegra on 6/3/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import <UIKit/UIKit.h>
#define IOSFC_BUILDING_IOSFC
#import "IOSurfaceAPI.h"
#import "FlashView.h"

@interface Player2AppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
	FlashView *view;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;

@end

