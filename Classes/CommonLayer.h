//
//  CommonLayer.h
//  t07
//
//  Created by 野口一也 on 2014/06/24.
//
//

#ifndef __t07__CommonLayer__
#define __t07__CommonLayer__

#include "cocos2d.h"
#include "Constants.h"

USING_NS_CC;

class CommonLayer : public Layer
{
public:
    CommonLayer();
    ~CommonLayer();
    Layer* createBgLayer();
    Sprite* createBird();
    ActionInterval* createFlyAction(Vec2 position);
    
    CC_SYNTHESIZE(Sprite*, _land1, Land1);
    CC_SYNTHESIZE(Sprite*, _land2, Land2);
    CC_SYNTHESIZE(Size, _visibleSize, VisibleSize);
};

#endif /* defined(__t07__CommonLayer__) */
