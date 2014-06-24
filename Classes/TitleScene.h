//
//  TitleScene.h
//  t07
//
//  Created by 野口一也 on 2014/06/24.
//
//

#ifndef __t07__TitleScene__
#define __t07__TitleScene__

#include "cocos2d.h"
#include "Constants.h"
#include "CommonLayer.h"

USING_NS_CC;

class TitleScene : public CommonLayer
{
public:
    TitleScene();
    ~TitleScene();
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(TitleScene);
    void rateClicked(Ref * sender);
    void playClicked(Ref * sender);
    void scoreClicked(Ref * sender);
    virtual void onExit();
    virtual bool onTouchBegan(Touch *touch, Event *event);
    virtual void onTouchMoved(Touch *touch, Event *event);
    virtual void onTouchEnded(Touch *touch, Event *event);
    virtual void onTouchCancelled(Touch *touch, Event *event);
private:
    //    CC_SYNTHESIZE(Size, _visibleSize, VisibleSize);
    /** Touch listener */
    void enableTouchEvent(bool enabled);
    CC_SYNTHESIZE(EventListenerTouchOneByOne*, _touchListener, TouchListener);
    
    CC_SYNTHESIZE(Sprite*, _rateButton, RateButton);
    CC_SYNTHESIZE(Sprite*, _playButton, PlayButton);
    CC_SYNTHESIZE(Sprite*, _scoreButton, ScoreButton);
    CC_SYNTHESIZE(Sprite*, _checkButton, CheckButton);
    CC_SYNTHESIZE(Vec2, _touchBeganPoint, TouchBeganPoint);
    CC_SYNTHESIZE(Sprite*, _bird, Bird);
    bool checkMenuButton(Sprite* checkButton, Point touchPosition);
};

#endif /* defined(__t07__TitleScene__) */
