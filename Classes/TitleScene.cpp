//
//  TitleScene.cpp
//  t07
//
//  Created by 野口一也 on 2014/06/24.
//
//

#include "TitleScene.h"
#include "GameScene.h"

cocos2d::Scene* TitleScene::createScene()
{
    auto scene = Scene::create();
    auto layer = TitleScene::create();
    scene->addChild(layer);
    return scene;
}

TitleScene::TitleScene()
: _touchListener(nullptr)
, _rateButton(nullptr)
, _playButton(nullptr)
, _scoreButton(nullptr)
, _checkButton(nullptr)
{
    
}

TitleScene::~TitleScene()
{
    this->getBird()->stopAllActions();
    this->getLand1()->stopAllActions();
    this->getLand2()->stopAllActions();
}

bool TitleScene::init()
{
    if (!Layer::init()) {
        return false;
    }
    auto director = Director::getInstance();
    director->getOpenGLView()->setDesignResolutionSize(288, 512, ResolutionPolicy::EXACT_FIT);
//    Size visibleSize = this->getVisibleSize();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    this->setVisibleSize(visibleSize);
    
    auto layer = this->createBgLayer();
    this->addChild(layer);
    
    auto logo = Sprite::createWithSpriteFrameName("title.png");
    logo->setPosition(Vec2(visibleSize.width/2, visibleSize.height * 2 /3));
    this->addChild(logo);
    
    auto bird = this->createBird();
    bird->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2 + 25));
    bird->runAction(this->createFlyAction(bird->getPosition()));
    this->addChild(bird);
    this->setBird(bird);
    
    auto rate = Sprite::createWithSpriteFrameName("button_rate.png");
    auto play = Sprite::createWithSpriteFrameName("button_play.png");
    auto score = Sprite::createWithSpriteFrameName("button_score.png");
    // rate button
    rate->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2 - 35));
    this->addChild(rate, TITLE_BG_ZORDER);
    this->setRateButton(rate);
    // play button
    play->setPosition(Vec2(visibleSize.width/4, visibleSize.height/2 - 120));
    this->addChild(play, TITLE_BG_ZORDER);
    this->setPlayButton(play);
    // score button
    score->setPosition(Vec2(visibleSize.width * 3 /4, visibleSize.height/2 - 120));
    this->addChild(score, TITLE_BG_ZORDER);
    this->setScoreButton(score);
    enableTouchEvent(true);
    return true;
}

void TitleScene::onExit()
{
    enableTouchEvent(false);
    Layer::onExit();
}

void TitleScene::enableTouchEvent(bool enabled)
{
    if (this->_touchListener != nullptr) {
        Director::getInstance()->getEventDispatcher()->removeEventListener(this->_touchListener);
        this->_touchListener = nullptr;
    }
    if (enabled) {
        this->_touchListener = EventListenerTouchOneByOne::create();
        _touchListener->onTouchBegan = CC_CALLBACK_2(TitleScene::onTouchBegan, this);
        _touchListener->onTouchMoved = CC_CALLBACK_2(TitleScene::onTouchMoved, this);
        _touchListener->onTouchEnded = CC_CALLBACK_2(TitleScene::onTouchEnded, this);
        _touchListener->onTouchCancelled = CC_CALLBACK_2(TitleScene::onTouchCancelled, this);
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_touchListener, this);
    }
}

bool TitleScene::checkMenuButton(Sprite* checkButton, Vec2 touchPosition)
{
    Size buttonSize = checkButton->getContentSize();
    Vec2 buttonPos = checkButton->getPosition();
    if (abs(touchPosition.x - buttonPos.x) < buttonSize.width / 2
        && abs(touchPosition.y - buttonPos.y) < buttonSize.height / 2) {
        this->setCheckButton(checkButton);
        return true;
    }
    return false;
}

bool TitleScene::onTouchBegan(Touch *touch, Event *event)
{
    Vec2 location = touch->getLocation();
    this->setTouchBeganPoint(location);
    if (this->getRateButton() != nullptr) {
        checkMenuButton(this->getRateButton(), this->getTouchBeganPoint());
    }
    if (this->getPlayButton() != nullptr) {
        checkMenuButton(this->getPlayButton(), this->getTouchBeganPoint());
    }
    if (this->getScoreButton() != nullptr) {
        checkMenuButton(this->getScoreButton(), this->getTouchBeganPoint());
    }
    if (this->getCheckButton() != nullptr) {
        this->getCheckButton()->setPosition(Vec2(this->getCheckButton()->getPositionX(), this->getCheckButton()->getPositionY() - 3));
    }
    return true;
}

void TitleScene::onTouchMoved(Touch *touch, Event *event)
{
    
}

void TitleScene::onTouchEnded(Touch *touch, Event *event)
{
    this->setTouchBeganPoint(Vec2::ZERO);
    if (this->getCheckButton() != nullptr) {
        this->getCheckButton()->setPosition(Vec2(this->getCheckButton()->getPositionX(), this->getCheckButton()->getPositionY() + 3));
        if (this->getCheckButton() == this->getRateButton()) {
        } else if (this->getCheckButton() == this->getPlayButton()) {
            auto play = GameScene::createScene();
            Director::getInstance()->replaceScene(play);
        } else if (this->getCheckButton() == this->getScoreButton()) {
            
        }
        this->setCheckButton(nullptr);
    }
}

void TitleScene::onTouchCancelled(Touch *touch, Event *event)
{
    this->setTouchBeganPoint(Vec2::ZERO);
}

void TitleScene::rateClicked(Ref * sender)
{
    
}

void TitleScene::playClicked(Ref * sender)
{
    
}

void TitleScene::scoreClicked(Ref * sender)
{
    
}
