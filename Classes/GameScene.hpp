//
//  GameScene.hpp
//  erba
//
//  Created by xiaoleiji on 15/12/28.
//
//

#ifndef GameScene_hpp
#define GameScene_hpp

#include "cocos2d.h"
#include "CardSprite.hpp"
#include "MenuLayer.hpp"

class GameScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(GameScene);
    
    virtual bool onTouchBegan(Touch *touch, Event *event);
    virtual void onTouchEnded(Touch *touch, Event *event);
    
    bool moveLeft();
    bool moveRight();
    bool moveUp();
    bool moveDown();
    
    void createCardArr(Size size);
    void randomCreateCard();
    
    void checkGameWin();
    void checkGameOver();
    void restart(Ref *sender);
    
private:
    int score;
    int bestScore;
    bool sound;
    Label *scoreLabel;
    Label *restartBtn;
    Label *isSoundBtn;
    CardSprite *cardArr[4][4];
    CardSprite *cardArrAction[4][4];
    Point startPt;
    int offsetX, offsetY;
    MenuLayer *menuLayer;
    timeval tv;
    
};

#endif /* GameScene_hpp */
