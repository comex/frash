//
//  FlashView.h
//  Player2
//
//  Created by Nicholas Allegra on 6/16/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface FlashViewThread : NSThread {
}
@end

@interface FlashView : UIView {
	NSThread *sendThread;
	int rpcfd;
}

@property (assign) int rpcfd;

@end
