//
//  CommonLayer.cpp
//  t07
//
//  Created by 野口一也 on 2014/06/24.
//
//

#include "CommonLayer.h"

CommonLayer::CommonLayer()
: _land1(nullptr)
, _land2(nullptr)
, _visibleSize(Director::getInstance()->getVisibleSize())
{}

CommonLayer::~CommonLayer()
{
}

Layer* CommonLayer::createBgLayer()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    auto layer = Layer::create();
    int randomBgIndex = rand()%2;
    std::string bgName = "bg_day.png";
    if (randomBgIndex) {
        bgName = "bg_night.png";
    }
    auto bg = Sprite::createWithSpriteFrameName(bgName);
    bg->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    layer->addChild(bg);
    
    // first moving land
    this->_land1 = Sprite::createWithSpriteFrameName("land.png");
    float landHeight = this->_land1->getContentSize().height;
    this->_land1->setPosition(Vec2(visibleSize.width/2, landHeight/2));
    layer->addChild(this->_land1, TITLE_BG_ZORDER);
    
    auto move_1 = MoveTo::create(2.0f, Vec2(- visibleSize.width/2, landHeight/2));
    auto reset_1 = Place::create(Vec2(visibleSize.width/2, landHeight/2));
    this->_land1->runAction(RepeatForever::create(Sequence::create(move_1, reset_1, NULL)));
    
    // second moving land
    this->_land2 = Sprite::createWithSpriteFrameName("land.png");
    this->_land2->setPosition(Vec2(visibleSize.width * 3 / 2, landHeight/2));
    layer->addChild(this->_land2, TITLE_BG_ZORDER);
    
    auto move_2 = MoveTo::create(2.0f, Vec2(visibleSize.width/2, landHeight/2));
    auto reset_2 = Place::create(Vec2(visibleSize.width * 3 / 2, landHeight/2));
    this->_land2->runAction(RepeatForever::create(Sequence::create(move_2, reset_2, NULL)));
    
    return layer;
}

Sprite* CommonLayer::createBird()
{
    auto cache = SpriteFrameCache::getInstance();
    int randomIndex = rand()%3;
    std::string frameName = "";
    Vector<SpriteFrame*> birdFrame;
    for (int i = 0; i < 3; i++) {
        frameName = "bird" + std::to_string(randomIndex) + "_" + std::to_string(i) + ".png";
        auto frame = cache->getSpriteFrameByName(frameName);
        birdFrame.pushBack(frame);
    }
    auto bird = Sprite::createWithSpriteFrame(birdFrame.at(1));
    auto animation = Animation::createWithSpriteFrames(birdFrame);
    animation->setDelayPerUnit(0.1);
    auto animate = Animate::create(animation);
    bird->runAction(RepeatForever::create(animate));
    return bird;
}

ActionInterval* CommonLayer::createFlyAction(Vec2 position)
{
    auto moveUp = MoveTo::create(1.0f / FLY_SPEED, Vec2(position.x, position.y + FLY_OFFSET));
    auto moveDown = MoveTo::create(1.0f/ FLY_SPEED, Vec2(position.x, position.y - FLY_OFFSET));
    
    auto flyAction = RepeatForever::create(Sequence::create(moveUp, moveDown, nullptr));
    flyAction->setTag(FLY_TAG);
    return flyAction;
}