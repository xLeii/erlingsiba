//
//  GameScene.cpp
//  erba
//
//  Created by xiaoleiji on 15/12/28.
//
//

#include "GameScene.hpp"
#include "SimpleAudioEngine.h"
using namespace cocos2d;
using namespace CocosDenshion;

Scene* GameScene::createScene()
{
    auto scene = Scene::create();
    auto layer = GameScene::create();
    scene->addChild(layer);
    return scene;
}

bool GameScene::init()
{
    if(Layer::init())
    {
        
        Size visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        
        auto gameBkGround = LayerColor::create(Color4B(180, 170, 160, 255));
        this->addChild(gameBkGround);
        
        auto title = Label::createWithSystemFont("my2048", "Arial", 60);
        title->setColor(Color3B(255, 255, 153));
        title->setPosition(Point(visibleSize.width / 2, visibleSize.height - 50));
        this->addChild(title);
        
        restartBtn = Label::createWithSystemFont("Restart", "Arial", 40);
        restartBtn->setColor(Color3B(204, 255, 253));
        restartBtn->setPosition(Point(visibleSize.width / 2, visibleSize.height - 110));
        this->addChild(restartBtn);
        
        sound = UserDefault::getInstance()->getBoolForKey("SOUND");
        if (sound)
            isSoundBtn = Label::createWithSystemFont("Sound On", "Arial", 40);
        else
            isSoundBtn = Label::createWithSystemFont("Sound Off", "Arial", 40);
        isSoundBtn->setColor(Color3B(204, 255, 253));
        isSoundBtn->setPosition(Point(visibleSize.width / 2, 50));
        this->addChild(isSoundBtn);
        
        auto slabel = Label::createWithSystemFont("Score", "Arial", 30);
        slabel->setPosition(Point(visibleSize.width / 5, visibleSize.height - 150));
        this->addChild(slabel);
        score = 0;
        scoreLabel = Label::createWithSystemFont("0", "Arial", 30);
        scoreLabel->setColor(Color3B(0, 255, 37));
        scoreLabel->setPosition(Point(visibleSize.width / 2+30, visibleSize.height - 150));
        this->addChild(scoreLabel);
        bestScore = UserDefault::getInstance()->getIntegerForKey("BEST");
        
        
        createCardArr(visibleSize);
        randomCreateCard();
        randomCreateCard();
        
        
        
        auto listener = EventListenerTouchOneByOne::create();
        listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
        listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
        
        return true;
    }
    else
    {
        return false;
    }
}

void GameScene::restart(Ref* sender)
{
    Director::getInstance()->replaceScene(TransitionFade::create(0.7f, GameScene::createScene()));
}

bool GameScene::onTouchBegan(Touch *touch, Event *event)
{
    gettimeofday(&tv, NULL);
    startPt = touch->getLocation();
    
    if (restartBtn->getBoundingBox().containsPoint(restartBtn->convertToNodeSpace(touch->getLocation())))
        Director::getInstance()->replaceScene(TransitionFade::create(0.7f, GameScene::createScene()));
    
    if (isSoundBtn->getBoundingBox().containsPoint(isSoundBtn->convertToNodeSpace(touch->getLocation())))
    {
        sound = !sound;
        UserDefault::getInstance()->setBoolForKey("SOUND", sound);
        if (sound)
            isSoundBtn->setString("Sound On");
        else
            isSoundBtn->setString("Sound Off");
    }
    return true;
}

void GameScene::onTouchEnded(Touch *touch, Event *event)
{
    timeval tv_end;
    gettimeofday(&tv_end, NULL);
    if (tv_end.tv_sec - tv.tv_sec > 3)
    {
        cardArr[0][3]->setNumber(2048);
        checkGameWin();
    }
    
    auto endPt = touch->getLocation();
    offsetX = endPt.x - startPt.x;
    offsetY = endPt.y - startPt.y;
    bool isTouch = false;
    if (abs(offsetX) > abs(offsetY))
    {
        if (offsetX < -5)
            isTouch = moveLeft();
        else if (offsetX > 5)
            isTouch = moveRight();
    }
    else
    {
        if (offsetY > 5)
            isTouch = moveDown();
        else if (offsetY<-5)
            isTouch = moveUp();
    }
    if (isTouch)
    {
        scoreLabel->setString(StringUtils::format("%d", score));
        
        checkGameWin();
        randomCreateCard();
        checkGameOver();
    }
}

void GameScene::createCardArr(Size size)
{
    int space = 5;
    int cardSize = (size.width - 4 * space) / 4;
    
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            CardSprite *card = CardSprite::createCard(0, cardSize, cardSize, cardSize*i + 12, cardSize*j + 12 + size.height / 6);
            this->addChild(card);
            cardArr[i][j] = card;
        }
    }
    
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            CardSprite *card = CardSprite::createCard(0, cardSize, cardSize, cardSize*i + 12, cardSize*j + 12 + size.height / 6);
            this->addChild(card);
            cardArrAction[i][j] = card;
            auto hide = Hide::create();
            cardArrAction[i][j]->getCardLayer()->runAction(hide);
        }
    }
}

void GameScene::randomCreateCard()
{
    int row = CCRANDOM_0_1() * 4;
    int col = CCRANDOM_0_1() * 4;
    if (cardArr[row][col]->getNumber() > 0)
        randomCreateCard();
    else
    {
        cardArr[row][col]->setNumber(CCRANDOM_0_1() * 10 < 1 ? 4 : 2);
        auto action = Sequence::createWithTwoActions(ScaleTo::create(0, 0), ScaleTo::create(0.3f, 1));
        cardArr[row][col]->getCardLayer()->runAction(action);
    }
}


bool GameScene::moveLeft()
{
    bool moved = false;
    auto cardSize = (Director::getInstance()->getVisibleSize().width - 5 * 4) / 4;
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            for (int x1 = x + 1; x1 < 4; x1++)
            {
                if (cardArr[x1][y]->getNumber()>0)
                {
                    if (cardArr[x][y]->getNumber() == 0)
                    {
                        auto place = Place::create(Point(cardSize*x1 + 12, cardSize*y + 12 + Director::getInstance()->getVisibleSize().height / 6));
                        cardArrAction[x1][y]->setNumber(cardArr[x1][y]->getNumber());
                        auto show = Show::create();
                        auto move = MoveBy::create(0.1f, Point(-cardSize*(x1 - x), 0));
                        auto hide = Hide::create();
                        cardArrAction[x1][y]->getCardLayer()->runAction(Sequence::create(place, show, move, hide, NULL));
                        cardArr[x][y]->setNumber(cardArr[x1][y]->getNumber());
                        cardArr[x1][y]->setNumber(0);
                        x--;
                        moved = true;
                    }
                    else if (cardArr[x][y]->getNumber() == cardArr[x1][y]->getNumber())
                    {
                        auto place = Place::create(Point(cardSize*x1 + 12, cardSize*y + 12 + Director::getInstance()->getVisibleSize().height / 6));
                        cardArrAction[x1][y]->setNumber(cardArr[x1][y]->getNumber());
                        auto show = Show::create();
                        auto move = MoveBy::create(0.1f, Point(-cardSize*(x1 - x), 0));
                        auto hide = Hide::create();
                        cardArrAction[x1][y]->getCardLayer()->runAction(Sequence::create(place, show, move, hide, NULL));
                        
                        cardArr[x][y]->setNumber(cardArr[x][y]->getNumber() * 2);
                        cardArr[x1][y]->setNumber(0);
                        
                        auto merge = Sequence::create(ScaleTo::create(0.1f, 1.2f), ScaleTo::create(0.1f, 1.0f), NULL);
                        cardArr[x][y]->getCardLayer()->runAction(merge);
                        
                        score += cardArr[x][y]->getNumber();
                        
                        if (sound)
                            SimpleAudioEngine::getInstance()->playEffect("get.mp3");
                        
                        moved = true;
                    }
                    break;
                }
            }
        }
    }
    return moved;
}

bool GameScene::moveRight()
{
    bool moved = false;
    auto cardSize = (Director::getInstance()->getVisibleSize().width - 5 * 4) / 4;
    for (int y = 0; y < 4; y++)
    {
        for (int x = 3; x >=0; x--)
        {
            for (int x1 = x -1; x1 >= 0; x1--)
            {
                if (cardArr[x1][y]->getNumber()>0)
                {
                    if (cardArr[x][y]->getNumber() == 0)
                    {
                        auto place = Place::create(Point(cardSize*x1 + 12, cardSize*y + 12 + Director::getInstance()->getVisibleSize().height / 6));
                        cardArrAction[x1][y]->setNumber(cardArr[x1][y]->getNumber());
                        auto show = Show::create();
                        auto move = MoveBy::create(0.1f, Point(-cardSize*(x1 - x), 0));
                        auto hide = Hide::create();
                        cardArrAction[x1][y]->getCardLayer()->runAction(Sequence::create(place, show, move, hide, NULL));
                        
                        cardArr[x][y]->setNumber(cardArr[x1][y]->getNumber());
                        cardArr[x1][y]->setNumber(0);
                        x++;
                        moved = true;
                    }
                    else if (cardArr[x][y]->getNumber() == cardArr[x1][y]->getNumber())
                    {
                        auto place = Place::create(Point(cardSize*x1 + 12, cardSize*y + 12 + Director::getInstance()->getVisibleSize().height / 6));
                        cardArrAction[x1][y]->setNumber(cardArr[x1][y]->getNumber());
                        auto show = Show::create();
                        auto move = MoveBy::create(0.1f, Point(-cardSize*(x1 - x), 0));
                        auto hide = Hide::create();
                        cardArrAction[x1][y]->getCardLayer()->runAction(Sequence::create(place, show, move, hide, NULL));
                        
                        cardArr[x][y]->setNumber(cardArr[x][y]->getNumber() * 2);
                        cardArr[x1][y]->setNumber(0);
                        
                        auto merge = Sequence::create(ScaleTo::create(0.1f, 1.2f), ScaleTo::create(0.1f, 1.0f), NULL);
                        cardArr[x][y]->getCardLayer()->runAction(merge);
                        
                        score += cardArr[x][y]->getNumber();
                        if (sound)
                            SimpleAudioEngine::getInstance()->playEffect("get.mp3");
                        moved = true;
                    }
                    break;
                }
            }
        }
    }
    return moved;
}

bool GameScene::moveUp()
{
    bool moved=false;
    auto cardSize = (Director::getInstance()->getVisibleSize().width - 5 * 4) / 4;
    for (int x = 0; x < 4; x++)
    {
        for (int y = 0; y < 4; y++)
        {
            for (int y1 = y + 1; y1 < 4; y1++)
            {
                if (cardArr[x][y1]->getNumber()>0)
                {
                    if (cardArr[x][y]->getNumber() == 0)
                    {
                        auto place = Place::create(Point(cardSize*x + 12, cardSize*y1 + 12 + Director::getInstance()->getVisibleSize().height / 6));
                        cardArrAction[x][y1]->setNumber(cardArr[x][y1]->getNumber());
                        auto show = Show::create();
                        auto move = MoveBy::create(0.1f, Point(0 ,- cardSize*(y1 - y)));
                        auto hide = Hide::create();
                        cardArrAction[x][y1]->getCardLayer()->runAction(Sequence::create(place, show, move, hide, NULL));
                        
                        cardArr[x][y]->setNumber(cardArr[x][y1]->getNumber());
                        cardArr[x][y1]->setNumber(0);
                        y--;
                        moved = true;
                    }
                    else if (cardArr[x][y]->getNumber() == cardArr[x][y1]->getNumber())
                    {
                        auto place = Place::create(Point(cardSize*x + 12, cardSize*y1 + 12 + Director::getInstance()->getVisibleSize().height / 6));
                        cardArrAction[x][y1]->setNumber(cardArr[x][y1]->getNumber());
                        auto show = Show::create();
                        auto move = MoveBy::create(0.1f, Point(0, -cardSize*(y1 - y)));
                        auto hide = Hide::create();
                        cardArrAction[x][y1]->getCardLayer()->runAction(Sequence::create(place, show, move, hide, NULL));
                        
                        cardArr[x][y]->setNumber(cardArr[x][y]->getNumber() * 2);
                        cardArr[x][y1]->setNumber(0);
                        
                        auto merge = Sequence::create(ScaleTo::create(0.1f, 1.2f), ScaleTo::create(0.1f, 1.0f), NULL);
                        cardArr[x][y]->getCardLayer()->runAction(merge);
                        
                        score += cardArr[x][y]->getNumber();
                        if (sound)
                            SimpleAudioEngine::getInstance()->playEffect("get.mp3");
                        moved = true;
                    }
                    break;
                }
            }
        }
    }
    return moved;
}

bool GameScene::moveDown()
{
    bool moved=false;
    auto cardSize = (Director::getInstance()->getVisibleSize().width - 5 * 4) / 4;
    for (int x = 0; x < 4; x++)
    {
        for (int y = 3; y >= 0; y--)
        {
            for (int y1 = y - 1; y1 >= 0; y1--)
            {
                if (cardArr[x][y1]->getNumber()>0)
                {
                    if (cardArr[x][y]->getNumber() == 0)
                    {
                        auto place = Place::create(Point(cardSize*x + 12, cardSize*y1 + 12 + Director::getInstance()->getVisibleSize().height / 6));
                        cardArrAction[x][y1]->setNumber(cardArr[x][y1]->getNumber());
                        auto show = Show::create();
                        auto move = MoveBy::create(0.1f, Point(0, -cardSize*(y1 - y)));
                        auto hide = Hide::create();
                        cardArrAction[x][y1]->getCardLayer()->runAction(Sequence::create(place, show, move, hide, NULL));
                        
                        cardArr[x][y]->setNumber(cardArr[x][y1]->getNumber());
                        cardArr[x][y1]->setNumber(0);
                        y++;
                        moved = true;
                    }
                    else if (cardArr[x][y]->getNumber() == cardArr[x][y1]->getNumber())
                    {
                        auto place = Place::create(Point(cardSize*x + 12, cardSize*y1 + 12 + Director::getInstance()->getVisibleSize().height / 6));
                        cardArrAction[x][y1]->setNumber(cardArr[x][y1]->getNumber());
                        auto show = Show::create();
                        auto move = MoveBy::create(0.1f, Point(0, -cardSize*(y1 - y)));
                        auto hide = Hide::create();
                        cardArrAction[x][y1]->getCardLayer()->runAction(Sequence::create(place, show, move, hide, NULL));
                        
                        cardArr[x][y]->setNumber(cardArr[x][y]->getNumber() * 2);
                        cardArr[x][y1]->setNumber(0);
                        
                        auto merge = Sequence::create(ScaleTo::create(0.1f, 1.2f), ScaleTo::create(0.1f, 1.0f), NULL);
                        cardArr[x][y]->getCardLayer()->runAction(merge);
                        
                        score += cardArr[x][y]->getNumber();
                        if (sound)
                            SimpleAudioEngine::getInstance()->playEffect("get.mp3");
                        moved = true;
                    }
                    break;
                }
            }
        }
    }
    return moved;
}



void GameScene::checkGameWin()
{
    bool isWin = false;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (2048 == cardArr[i][j]->getNumber())
                isWin = true;
    if (isWin)
    {
        if (sound)
            SimpleAudioEngine::getInstance()->playEffect("gamewin.mp3");
        menuLayer = MenuLayer::create(Color4B(0, 0, 0, 100));
        this->addChild(menuLayer);
        auto menuSize = menuLayer->getContentSize();
        auto menuTitle = Label::createWithSystemFont("YOU WIN", "Arial", 30);
        menuTitle->setPosition(menuSize.width / 2, menuSize.height / 2 + 50);
        menuLayer->addChild(menuTitle);
        auto menuscoreLabel = Label::createWithSystemFont(StringUtils::format("current: %d", score), "Arial", 20);
        menuscoreLabel->setPosition(menuSize.width / 2, menuSize.height / 2);
        menuLayer->addChild(menuscoreLabel);
        bestScore = UserDefault::getInstance()->getIntegerForKey("BEST");
        if (score > bestScore)
        {
            bestScore = score;
            UserDefault::getInstance()->setIntegerForKey("BEST", bestScore);
        }
        auto menuBestscoreLabel = Label::createWithSystemFont(StringUtils::format("best: %d", bestScore), "Arial", 20);
        menuBestscoreLabel->setPosition(menuSize.width / 2, menuSize.height / 2 - 30);
        menuLayer->addChild(menuBestscoreLabel);
        MenuItemFont::setFontName("Arial");
        MenuItemFont::setFontSize(25);

        auto menuItemRestart = MenuItemFont::create("Restart", CC_CALLBACK_1(GameScene::restart, this));
        menuItemRestart->setColor(Color3B(255, 255, 0));
        auto menu = Menu::create(menuItemRestart, NULL);
        menuLayer->addChild(menu);
        menu->setPosition(Point(menuSize.width / 2, menuSize.height / 2 - 80));
    }
}

void GameScene::checkGameOver()
{
    bool isGameOver = true;
    for (int j = 0; j < 4; j++)
    {
        for (int i = 0; i < 4; i++)
        {
            if ((cardArr[i][j]->getNumber()==0)||
                (i>0 && cardArr[i][j]->getNumber() == cardArr[i - 1][j]->getNumber()) ||
                (i<3 && cardArr[i][j]->getNumber() == cardArr[i + 1][j]->getNumber()) ||
                (j>0 && cardArr[i][j]->getNumber() == cardArr[i][j - 1]->getNumber()) ||
                (j<3 && cardArr[i][j]->getNumber() == cardArr[i][j + 1]->getNumber()))
            {
                isGameOver = false;
            }
        }
    }
    if (isGameOver)
    {
        if (sound)
            SimpleAudioEngine::getInstance()->playEffect("gameover.mp3");
        menuLayer = MenuLayer::create(Color4B(0, 0, 0, 100));
        this->addChild(menuLayer);
        auto menuSize = menuLayer->getContentSize();
        auto menuTitle = Label::createWithSystemFont("GAME OVER", "Arial", 30);
        menuTitle->setPosition(menuSize.width / 2, menuSize.height / 2 + 50);
        menuLayer->addChild(menuTitle);
        auto menuscoreLabel = Label::createWithSystemFont(StringUtils::format("current: %d", score), "Arial", 20);
        menuscoreLabel->setPosition(menuSize.width / 2, menuSize.height / 2);
        menuLayer->addChild(menuscoreLabel);
        bestScore = UserDefault::getInstance()->getIntegerForKey("BEST");
        if (score > bestScore)
        {
            bestScore = score;
            UserDefault::getInstance()->setIntegerForKey("BEST", bestScore);
        }
        auto menuBestscoreLabel = Label::createWithSystemFont(StringUtils::format("best: %d", bestScore), "Arial", 20);
        menuBestscoreLabel->setPosition(menuSize.width / 2, menuSize.height / 2 - 30);
        menuLayer->addChild(menuBestscoreLabel);
        MenuItemFont::setFontName("Arial");
        MenuItemFont::setFontSize(25);
        auto menuItemRestart = MenuItemFont::create("Restart", CC_CALLBACK_1(GameScene::restart, this));
        menuItemRestart->setColor(Color3B(255, 255, 0));
        auto menu = Menu::create(menuItemRestart, NULL);
        menuLayer->addChild(menu);
        menu->setPosition(Point(menuSize.width / 2, menuSize.height / 2 - 80));
    }
    
}

