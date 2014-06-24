//
//  GameScene.cpp
//  t07
//
//  Created by 野口一也 on 2014/06/24.
//
//

#include "GameScene.h"

cocos2d::Scene* GameScene::createScene()
{
    // 物理シーンを作成
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setGravity(Vec2(0, SYSTEM_GRAVITY));
    auto layer = GameScene::create();
    scene->addChild(layer);
    return scene;
}

GameScene::GameScene()
: _ready(false)
, _totalScore(0)
, _gameOver(false)
, _bird(nullptr)
, _readyNode(nullptr)
, _readyText(nullptr)
, _readyImage(nullptr)
, _layerBg(nullptr)
, _scoreNode(nullptr)
, _whiteBlink(nullptr)
, _groundNode(nullptr)
, _contactListener(nullptr)
{
}

GameScene::~GameScene()
{
}

bool GameScene::init()
{
    if (!Layer::init()) {
        return false;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();
    this->resetGameSceneValue();
    
    auto layer = this->createBgLayer();
    auto groundNode = Node::create();
    groundNode->setPhysicsBody(PhysicsBody::createEdgeSegment(Vec2(0, LAND_HEIGHT), Vec2(228, LAND_HEIGHT)));
    layer->addChild(groundNode);
    
    // add bird
    auto bird = this->createBird();
    bird->setPosition(Vec2(BIRD_X, visibleSize.height / 2 + 10));
    bird->setPhysicsBody(PhysicsBody::createCircle(BIRD_SIZE));
    bird->getPhysicsBody()->setEnable(false);
    bird->runAction(this->createFlyAction(bird->getPosition()));
    layer->addChild(bird, GAME_BIRD_ZORDER);
    
    auto whiteBlink = Sprite::createWithSpriteFrameName("white.png");
    whiteBlink->setScale(100);
    whiteBlink->setOpacity(0);
    layer->addChild(whiteBlink, GAME_WHITE_ZORDER);
    
    this->setGroundNode(groundNode);
    this->setWhiteBlink(whiteBlink);
    this->setBird(bird);
    this->setLayerBg(layer);
    
    initReady();
    
    this->setScoreNode(Node::create());
    
    this->initScore();
    this->refreshScore();
    this->enableTouchEvent(true);
    this->enableCollisionEvent(true);
    this->addChild(layer);
    return true;
}

void GameScene::onExit()
{
    enableCollisionEvent(false);
    enableTouchEvent(false);
    Layer::onExit();
}

bool GameScene::checkMenuButton(Sprite* checkButton, Vec2 touchPosition)
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

bool GameScene::onTouchBegan(Touch *touch, Event *event)
{
    Vec2 location = touch->getLocation();
    this->setTouchBeganPoint(location);
    
    if (this->getGameOver()) {
        if (this->getPlayButton() != nullptr) {
            checkMenuButton(this->getPlayButton(), this->getTouchBeganPoint());
        }
        if (this->getRankButton() != nullptr) {
            checkMenuButton(this->getRankButton(), this->getTouchBeganPoint());
        }
        if (this->getCheckButton() != nullptr) {
            this->getCheckButton()->setPosition(Vec2(this->getCheckButton()->getPositionX(), this->getCheckButton()->getPositionY() - 3));
        }
        
    } else {
        if (!this->getReady()) {
            this->setReady(true);
            hideReady();
            // create pipes
            this->createPipes(this->getLayerBg());
            
            this->getBird()->getPhysicsBody()->setEnable(true);
            this->getBird()->getPhysicsBody()->setVelocity(Vec2(0, TAP_V));
            
            auto scheduler = Director::getInstance()->getScheduler();
            scheduler->schedule(schedule_selector(GameScene::checkHit), this, 0, false);
            scheduler->schedule(schedule_selector(GameScene::rotateBird), this, 0, false);
//            scheduler->scheduleSelector(schedule_selector(GameScene::checkHit), this, 0, false);
//            scheduler->scheduleSelector(schedule_selector(GameScene::rotateBird), this, 0, false);
        } else {
            this->getBird()->getPhysicsBody()->setVelocity(Vec2(0, TAP_V));
        }
    }
    return true;
}

void GameScene::onTouchMoved(Touch *touch, Event *event)
{
    
}

void GameScene::onTouchEnded(Touch *touch, Event *event)
{
    this->setTouchBeganPoint(Vec2::ZERO);
    if (this->getGameOver()) {
        if (this->getCheckButton() != nullptr) {
            this->getCheckButton()->setPosition(Vec2(this->getCheckButton()->getPositionX(), this->getCheckButton()->getPositionY() + 3));
            if (this->getCheckButton() == this->getPlayButton()) {
                auto play = GameScene::createScene();
                Director::getInstance()->replaceScene(play);
            } else if (this->getCheckButton() == this->getRankButton()) {
                
            }
            this->setCheckButton(nullptr);
        }
    }
}

void GameScene::onTouchCancelled(Touch *touch, Event *event)
{
    this->setTouchBeganPoint(Vec2::ZERO);
}

void GameScene::enableTouchEvent(bool enabled)
{
    if (this->_touchListener != nullptr) {
        Director::getInstance()->getEventDispatcher()->removeEventListener(this->_touchListener);
        this->_touchListener = nullptr;
    }
    if (enabled) {
        this->_touchListener = EventListenerTouchOneByOne::create();
        _touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
        _touchListener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
        _touchListener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
        _touchListener->onTouchCancelled = CC_CALLBACK_2(GameScene::onTouchCancelled, this);
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_touchListener, this);
    }
}

void GameScene::enableCollisionEvent(bool enabled)
{
    if (this->getContactListener() != nullptr) {
        this->getLayerBg()->getEventDispatcher()->removeEventListener(this->getContactListener());
        this->setContactListener(nullptr);
    }
    if (enabled) {
        auto contactListener = EventListenerPhysicsContactWithBodies::create(this->getGroundNode()->getPhysicsBody(), this->getBird()->getPhysicsBody());
        contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);
        this->getLayerBg()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this->getLayerBg());
        this->setContactListener(contactListener);
    }
}

bool GameScene::onContactBegin(PhysicsContact& contact)
{
    if (!this->getGameOver()) {
        this->setGameOver(true);
        this->GameOver();
    }
    this->getBird()->getPhysicsBody()->setEnable(false);
    this->getBird()->stopAllActions();
    this->getBird()->setRotation(-90);
    
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(GameScene::rotateBird), this);
    return true;
}

void GameScene::resetGameSceneValue()
{
    this->setTotalScore(0);
    this->setGameOver(false);
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(GameScene::rotateBird), this);
}

void GameScene::initReady()
{
    Size visibleSize = this->getVisibleSize();
    auto readyText = Sprite::createWithSpriteFrameName("text_ready.png");
    readyText->setPosition(Vec2(0, 100));
    auto readyImage = Sprite::createWithSpriteFrameName("tutorial.png");
    
    auto readyNode = Node::create();
    readyNode->addChild(readyText);
    readyNode->addChild(readyImage);readyNode->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    this->setReadyNode(readyNode);
    
    this->setReadyText(readyText);
    this->setReadyImage(readyImage);
    this->getLayerBg()->addChild(readyNode);
}

void GameScene::hideReady()
{
    this->getBird()->stopActionByTag(FLY_TAG);
    this->getReadyText()->runAction(FadeOut::create(0.2));
    this->getReadyImage()->runAction(FadeOut::create(0.2));
}

Sprite* GameScene::createScoreDigit(int type, int digit)
{
    std::string digitName = "";
    if (type == 1) {
        digitName = "font_0" + std::to_string(48 + digit) + ".png";
    } else {
        digitName = "number_score_0" + std::to_string(digit) + ".png";
    }
    return Sprite::createWithSpriteFrameName(digitName);
}

void GameScene::createScore(Node* rootNode, int score, int size, int alignType)
{
    rootNode->removeAllChildren();
    
    int distance = 20;
    if (size == 2) {
        distance = 15;
    }
    
    int tmpscore = score;
    std::vector<int> digits;
    int dig = tmpscore % 10;
    digits.push_back(dig);
    while (floor(tmpscore/10) != 0) {
        tmpscore = floor(tmpscore/10);
        dig = tmpscore % 10;
        digits.push_back(dig);
    }
    int nowOffset = (digits.size() - 1) * distance / 2;
    if (alignType == 2) {
        nowOffset = 0;
    }
    for (std::vector<int>::iterator it = digits.begin(); it != digits.end(); it++) {
        int dig = (int)*it;
        auto digitSprite = createScoreDigit(size, dig);
        digitSprite->setPosition(Vec2(nowOffset, 0));
        rootNode->addChild(digitSprite);
        nowOffset -= distance;
    }
}

void GameScene::initScore()
{
    this->getScoreNode()->setPosition(this->getVisibleSize().width/2, this->getVisibleSize().height * 4 / 5 + 10);
    this->getLayerBg()->addChild(this->getScoreNode(), GAME_SCORE_ZORDER);
}

void GameScene::refreshScore()
{
    createScore(this->getScoreNode(), this->getTotalScore(), 1, 1);
}

void GameScene::blinkWhite()
{
    auto fadeIn = FadeIn::create(0.1);
    auto fadeOut = FadeOut::create(0.1);
    
    this->getWhiteBlink()->runAction(Sequence::create(fadeIn, fadeOut, NULL));
}

void GameScene::GameOver()
{
    this->setCheckButton(nullptr);
    this->setGameOver(true);
    for (int i = 1; i < PIPE_COUNT; i++) {
        this->_pipes[i]->stopAllActions();
    }
    this->getLand1()->stopAllActions();
    this->getLand2()->stopAllActions();
    this->getBird()->stopAllActions();
    this->getBird()->getPhysicsBody()->setVelocity(Vec2(0, 0));
    
    // unschedule
    auto scheduler = Director::getInstance()->getScheduler();
    scheduler->unschedule(schedule_selector(GameScene::createPipe), this);
    scheduler->unschedule(schedule_selector(GameScene::checkHit), this);
//    scheduler->unscheduleSelector(schedule_selector(GameScene::createPipe), this);
//    scheduler->unscheduleSelector(schedule_selector(GameScene::checkHit), this);
    
    this->blinkWhite();
    // remove event listener
    this->enableCollisionEvent(false);
    //    this->enableTouchEvent(false);
    
    this->showGameOverLayer();
}

void GameScene::rotateBird(float dt)
{
    Vec2 v = this->getBird()->getPhysicsBody()->getVelocity();
    this->getBird()->setRotation(MIN(MAX(-90, v.y * 0.2 + 60), 30));
}

void GameScene::checkHit(float dt)
{
    if (this->getBird()->getPositionY() < LAND_HEIGHT + BIRD_SIZE) {
        GameOver();
        return;
    }
    for (int i = 1; i < PIPE_COUNT; i++) {
        if (abs(this->_pipes[i]->getPositionX() - BIRD_X) < (BIRD_SIZE + 26)) {
            if (this->getBird()->getPositionY() < this->_pipes[i]->getPositionY() + PIPE_HEIGHT / 2 + BIRD_SIZE) {
                GameOver();
                return;
            }
            if (this->getBird()->getPositionY() > this->_pipes[i]->getPositionY() + PIPE_HEIGHT / 2 + PIPE_DISTANCE - BIRD_SIZE) {
                GameOver();
                return;
            }
        }
        if (this->_pipeState[i] == PIPE_NEW && this->_pipes[i]->getPositionX() < BIRD_X) {
            this->_pipeState[i] = PIPE_PASS;
            this->setTotalScore(this->getTotalScore()+1);
            refreshScore();
        }
    }
}

void GameScene::initPipe(Node* layer)
{
    this->_pipes.clear();
    this->_pipeState.clear();
    this->_pipes.push_back(nullptr);
    this->_pipeState.push_back(0);
    for (int i = 1; i < PIPE_COUNT; i++) {
        auto downPipe = Sprite::createWithSpriteFrameName("pipe_down.png");
        auto upPipe   = Sprite::createWithSpriteFrameName("pipe_up.png");
        
        downPipe->setPosition(Vec2(0, PIPE_HEIGHT + PIPE_DISTANCE));
        auto singlePipe = Node::create();
        singlePipe->addChild(downPipe);
        singlePipe->addChild(upPipe);
        singlePipe->setPosition(Vec2(-this->getVisibleSize().width * 2, 0));
        
        layer->addChild(singlePipe, GAME_PIPE_ZORDER);
        
        this->_pipes.push_back(singlePipe);
        this->_pipeState.push_back(PIPE_PASS);
    }
}

void GameScene::createPipe(float dt)
{
    Size visibleSize = this->getVisibleSize();
    for (int i = 1; i < PIPE_COUNT; i++) {
        if (this->_pipes[i]->getPositionX() < -100) {
            auto pipeNode = this->_pipes.at(i);
            this->_pipeState[i] = PIPE_NEW;
            
            pipeNode->stopAllActions();
            int randomHeight = rand() % 4;
            pipeNode->setPosition(Vec2(visibleSize.width * 2, 50 * randomHeight));
            pipeNode->setTag(randomHeight);
            
            auto move = MoveTo::create(8.0f, Vec2(-visibleSize.width * 2, 50 * randomHeight));
            pipeNode->runAction(move);
            break;
        }
    }
}

void GameScene::createPipes(Node* layer)
{
    initPipe(layer);
    auto scheduler = Director::getInstance()->getScheduler();
//    scheduler->scheduleSelector(schedule_selector(GameScene::createPipe), this, 1.3f, false);
    scheduler->schedule(schedule_selector(GameScene::createPipe), this, 1.3f, false);
}

void GameScene::showGameOverLayer()
{
    auto overlayer = Layer::create();
    auto actionNode = Node::create();
    overlayer->addChild(actionNode);
    
    auto gameOverLogo = Sprite::createWithSpriteFrameName("text_game_over.png");
    auto scorePanel = Sprite::createWithSpriteFrameName("score_panel.png");
    
    auto nowScoreNode = Node::create();
    auto bestScoreNode = Node::create();
    
    auto newInfoSprite = Sprite::createWithSpriteFrameName("new.png");
    
    auto playButton = Sprite::createWithSpriteFrameName("button_play.png");
    auto rankButton = Sprite::createWithSpriteFrameName("button_score.png");
    
    this->setOverLayer(overlayer);
    this->setActionNode(actionNode);
    this->setGameOverLogo(gameOverLogo);
    this->setScorePanel(scorePanel);
    this->setNowScoreNode(nowScoreNode);
    this->setBestScoreNode(bestScoreNode);
    this->setNewInfoSprite(newInfoSprite);
    this->setPlayButton(playButton);
    this->setRankButton(rankButton);
    
    initOverItems();
    showOverItems();
    this->addChild(overlayer);
}

void GameScene::initOverItems()
{
    // game over
    this->getGameOverLogo()->setPosition(Vec2(MID_X, MID_Y + 100));
    this->getGameOverLogo()->setVisible(false);
    
    this->getOverLayer()->addChild(this->getGameOverLogo());
    
    // score panel
    this->getScorePanel()->setPosition(Vec2(MID_X, MID_Y));
    this->getScorePanel()->setVisible(false);
    
    // score node
    this->getNowScoreNode()->setPosition(Vec2(200, 80));
    this->getScorePanel()->addChild(this->getNowScoreNode());
    
    this->getBestScoreNode()->setPosition(Vec2(200, 40));
    this->getScorePanel()->addChild(this->getBestScoreNode());
    
    // new Info
    this->getNewInfoSprite()->setPosition(Vec2(155, 60));
    this->getNewInfoSprite()->setVisible(false);
    this->getScorePanel()->addChild(this->getNewInfoSprite());
    
    this->getOverLayer()->addChild(this->getScorePanel());
    
    // bottom buttons
    // play button
    this->getPlayButton()->setPosition(Vec2(MID_X/2, MID_Y - 120));
    this->getPlayButton()->setVisible(false);
    this->getOverLayer()->addChild(this->getPlayButton(), GAME_OVER_ZORDER);
    
    // rank button
    this->getRankButton()->setPosition(Vec2(this->getVisibleSize().width * 3 / 4, MID_Y - 120));
    this->getRankButton()->setVisible(false);
    this->getOverLayer()->addChild(this->getRankButton(), GAME_OVER_ZORDER);
}

void GameScene::showOverItems()
{
    auto delay1 = DelayTime::create(1);
    auto showOverLogoFunc = CallFunc::create(CC_CALLBACK_0(GameScene::showGameOverLogo, this));
    auto showScorePanelFunc = CallFunc::create(CC_CALLBACK_0(GameScene::showScorePanel, this));
    auto showMedalFunc = CallFunc::create(CC_CALLBACK_0(GameScene::showMedal, this));
    auto showScoreNumberFunc = CallFunc::create(CC_CALLBACK_0(GameScene::showScoreNumber, this));
    auto showBottomButtonFunc = CallFunc::create(CC_CALLBACK_0(GameScene::showBottomButton, this));
    
    auto action = Sequence::create(delay1, showOverLogoFunc, delay1, showScorePanelFunc, showMedalFunc, showScoreNumberFunc, delay1, showBottomButtonFunc, NULL);
    this->getActionNode()->runAction(action);
}

void GameScene::showGameOverLogo()
{
    this->getScoreNode()->setVisible(false);
    this->getGameOverLogo()->setVisible(true);
    auto place = Place::create(Vec2(MID_X, MID_Y + 120));
    auto move1 = MoveTo::create(0.2f, Vec2(MID_X, MID_Y + 96));
    auto move2 = MoveTo::create(0.2f, Vec2(MID_X, MID_Y + 100));
    
    this->getGameOverLogo()->runAction(Sequence::create(place, move1, move2, NULL));
}

void GameScene::showScorePanel()
{
    this->getScorePanel()->setVisible(true);
    auto place = Place::create(Vec2(MID_X, -100));
    auto move1 = MoveTo::create(0.1f, Vec2(MID_X, MID_Y));
    this->getScorePanel()->runAction(Sequence::create(place, move1, NULL));
}

void GameScene::showNewRecordInfo()
{
    this->getNewInfoSprite()->setVisible(true);
}

void GameScene::showMedal()
{
    if (this->getTotalScore() < 10) {
        return;
    }
    std::string medalStr = "medals_" + std::to_string(MAX(4 - (int)floor(this->getTotalScore()/10), 0)) + ".png";
    auto medalSprite = Sprite::createWithSpriteFrameName(medalStr);
    medalSprite->setPosition(Vec2(55, 60));
    this->getScorePanel()->addChild(medalSprite);
}

void GameScene::showSingleNumber(float dt)
{
    int tmpScore = this->getShowScoreTmpScore();
    createScore(this->getNowScoreNode(), tmpScore, 2, 2);
    tmpScore++;
    this->setShowScoreTmpScore(tmpScore);
    if (tmpScore > this->getTotalScore()) {
        Director::getInstance()->getScheduler()->unschedule(schedule_selector(GameScene::showSingleNumber), this);
    }
}

void GameScene::showScoreNumber()
{
    auto pUserDefault = UserDefault::getInstance();
    int savedBestScore = pUserDefault->getIntegerForKey("bestScore", 0);
    if (this->getTotalScore() > savedBestScore) {
        savedBestScore = this->getTotalScore();
        // show new
        showNewRecordInfo();
        // save score
        pUserDefault->setIntegerForKey("bestScore", this->getTotalScore());
        pUserDefault->flush();
    }
    createScore(this->getBestScoreNode(), savedBestScore, 2, 2);
    createScore(this->getNowScoreNode(), 0, 2, 2);
    this->setShowScoreTmpScore(0);
    
//    Director::getInstance()->getScheduler()->scheduleSelector(schedule_selector(GameScene::showSingleNumber), this, 0.5 / this->getTotalScore(), false);
    Director::getInstance()->getScheduler()->schedule(schedule_selector(GameScene::showSingleNumber), this, 0.5 / this->getTotalScore(), false);
}

void GameScene::showBottomButton()
{
    this->getPlayButton()->setVisible(true);
    this->getRankButton()->setVisible(true);
}