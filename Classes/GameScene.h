//
//  GameScene.h
//  t07
//
//  Created by 野口一也 on 2014/06/24.
//
//

#ifndef __t07__GameScene__
#define __t07__GameScene__

#include "cocos2d.h"
#include "Constants.h"
#include "CommonLayer.h"

USING_NS_CC;

class GameScene : public CommonLayer
{
public:
    GameScene();
    ~GameScene();
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(GameScene);
    virtual void onExit();
    virtual bool onTouchBegan(Touch *touch, Event *event);
    virtual void onTouchMoved(Touch *touch, Event *event);
    virtual void onTouchEnded(Touch *touch, Event *event);
    virtual void onTouchCancelled(Touch *touch, Event *event);
    
    CC_SYNTHESIZE(bool, _ready, Ready);
    CC_SYNTHESIZE(int, _totalScore, TotalScore);
    CC_SYNTHESIZE(bool, _gameOver, GameOver);
    CC_SYNTHESIZE(Sprite*, _bird, Bird);
    CC_SYNTHESIZE(Node*, _readyNode, ReadyNode);
    CC_SYNTHESIZE(Sprite*, _readyText, ReadyText);
    CC_SYNTHESIZE(Sprite*, _readyImage, ReadyImage);
    CC_SYNTHESIZE(Layer*, _layerBg, LayerBg);
    CC_SYNTHESIZE(Node*, _scoreNode, ScoreNode);
    CC_SYNTHESIZE(Sprite*, _whiteBlink, WhiteBlink);
    std::vector<Node*> _pipes;
    std::vector<int> _pipeState;
    CC_SYNTHESIZE(Node*, _groundNode, GroundNode);
    CC_SYNTHESIZE(EventListenerPhysicsContactWithBodies*, _contactListener, ContactListener);
    CC_SYNTHESIZE(Sprite*, _gameOverLogo, GameOverLogo);
    CC_SYNTHESIZE(Sprite*, _scorePanel, ScorePanel);
    CC_SYNTHESIZE(Node*, _nowScoreNode, NowScoreNode);
    CC_SYNTHESIZE(Node*, _bestScoreNode, BestScoreNode);
    CC_SYNTHESIZE(Sprite*, _newInfoSprite, NewInfoSprite);
    CC_SYNTHESIZE(Sprite*, _playButton, PlayButton);
    CC_SYNTHESIZE(Sprite*, _rankButton, RankButton);
    CC_SYNTHESIZE(Layer*, _overlayer, OverLayer);
    CC_SYNTHESIZE(Node*, _actionNode, ActionNode);
    CC_SYNTHESIZE(int, _showScoreTmpScore, ShowScoreTmpScore);
    
    /** Touch listener */
    void enableTouchEvent(bool enabled);
    CC_SYNTHESIZE(EventListenerTouchOneByOne*, _touchListener, TouchListener);
    CC_SYNTHESIZE(Vec2, _touchBeganPoint, TouchBeganPoint);
    bool checkMenuButton(Sprite* checkButton, Vec2 touchPosition);
    
    /** Collision Listener */
    void enableCollisionEvent(bool enabled);
    CC_SYNTHESIZE(Sprite*, _checkButton, CheckButton);
    bool onContactBegin(PhysicsContact& contact);
    
    void resetGameSceneValue();
    void initReady();
    void hideReady();
    void initScore();
    void refreshScore();
    void createScore(Node* rootNode, int score, int size, int alignType);
    Sprite* createScoreDigit(int type, int digit);
    void blinkWhite();
    void GameOver();
    void showGameOverLayer();
    void rotateBird(float dt);
    void checkHit(float dt);
    void initPipe(Node* layer);
    void createPipe(float dt);
    void createPipes(Node* layer);
    void initOverItems();
    void showOverItems();
    void showGameOverLogo();
    void showScorePanel();
    void showNewRecordInfo();
    void showMedal();
    void showSingleNumber(float dt);
    void showScoreNumber();
    void showBottomButton();
};

#endif /* defined(__t07__GameScene__) */
