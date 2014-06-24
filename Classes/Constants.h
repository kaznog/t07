//
//  Constants.h
//  t07
//
//  Created by 野口一也 on 2014/06/24.
//
//

#ifndef t07_Constants_h
#define t07_Constants_h

#define FLY_SPEED 2.5
#define FLY_OFFSET 5
#define FLY_TAG 1000
#define TITLE_BG_ZORDER 100
#define GAME_BIRD_ZORDER 1000
#define GAME_WHITE_ZORDER 2000
#define GAME_SCORE_ZORDER 10000
#define GAME_PIPE_ZORDER 10
#define GAME_OVER_ZORDER 1000

#define BIRD_X 90
#define BIRD_SIZE 15
#define PIPE_COUNT 5
#define PIPE_HEIGHT 320
#define PIPE_DISTANCE 100
#define LAND_HEIGHT 112

#define TAP_V 260
#define SYSTEM_GRAVITY -900

#define PIPE_NEW 0
#define PIPE_PASS 1

#define MID_X Director::getInstance()->getVisibleSize().width/2
#define MID_Y Director::getInstance()->getVisibleSize().height/2


#endif
