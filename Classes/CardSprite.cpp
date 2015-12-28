//
//  CardSprite.cpp
//  erba
//
//  Created by xiaoleiji on 15/12/28.
//
//

#include "CardSprite.hpp"

bool CardSprite::init()
{
    if (!Sprite::init())
        return false;
    else
        return true;
}

void CardSprite::cardInit(int num, int cardWidth, int cardHeight, float px, float py)
{
    number = num;
    cardBgColour = LayerColor::create(Color4B(200, 190, 180, 255), cardWidth - 5, cardHeight - 5);
    cardBgColour->setPosition(Point(px, py));
    
    if (number > 0)
    {
        cardLabel = Label::createWithSystemFont(StringUtils::format("%d", number), "Arial", 50);
        cardLabel->setPosition(cardBgColour->getContentSize().width / 2, cardBgColour->getContentSize().height / 2);
        cardBgColour->addChild(cardLabel);
    }
    else
    {
        cardLabel = Label::createWithSystemFont("", "Arial", 50);
        cardLabel->setPosition(cardBgColour->getContentSize().width / 2, cardBgColour->getContentSize().height / 2);
        cardBgColour->addChild(cardLabel);
    }
    this->addChild(cardBgColour);
}

CardSprite* CardSprite::createCard(int num, int cardWidth, int cardHeight, float px, float py)
{
    CardSprite *card = new CardSprite();
    if (!card->init())
    {
        CC_SAFE_RELEASE(card);
        return NULL;
    }
    card->autorelease();
    card->cardInit(num, cardWidth, cardHeight, px, py);
    return card;
}

void CardSprite::setNumber(int num)
{
    number = num;
    if (number >= 0 && number < 16)
    {
        cardLabel->setSystemFontSize(50);
    }
    if (number >= 16 && number < 128)
    {
        cardLabel->setSystemFontSize(40);
    }
    if (number >= 128 && number < 1024)
    {
        cardLabel->setSystemFontSize(30);
    }
    if (number >= 1024)
    {
        cardLabel->setSystemFontSize(20);
    }
    if (number == 0)
    {
        cardBgColour->setColor(Color3B(200, 190, 180));
    }
    if (number == 2)
    {
        cardBgColour->setColor(Color3B(240, 230, 220));
    }
    if (number == 4)
    {
        cardBgColour->setColor(Color3B(51, 153, 51));
    }
    if (number == 8){
        cardBgColour->setColor(Color3B(255, 153, 102));
    }
    if (number == 16)
    {
        cardBgColour->setColor(Color3B(153, 204, 153));
    }
    if (number == 32)
    {
        cardBgColour->setColor(Color3B(153, 204, 255));
    }
    if (number == 64)
    {
        cardBgColour->setColor(Color3B(255, 204, 204));
    }
    if (number == 128)
    {
        cardBgColour->setColor(Color3B(204, 102, 0));
    }
    if (number == 256)
    {
        cardBgColour->setColor(Color3B(153, 204, 51));
    }
    if (number == 512)
    {
        cardBgColour->setColor(Color3B(255, 102, 102));
    }
    if (number == 1024)
    {
        cardBgColour->setColor(Color3B(204, 204, 255));
    }
    if (number == 2048)
    {
        cardBgColour->setColor(Color3B(255, 204, 00));
    }
    
    if (number > 0)
        cardLabel->setString(StringUtils::format("%d", number));
    else
        cardLabel->setString("");
}

int CardSprite::getNumber()
{
    return number;
}

LayerColor* CardSprite::getCardLayer()
{
    return cardBgColour;
}