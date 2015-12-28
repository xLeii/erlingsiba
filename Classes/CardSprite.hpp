//
//  CardSprite.hpp
//  erba
//
//  Created by xiaoleiji on 15/12/28.
//
//

#ifndef CardSprite_hpp
#define CardSprite_hpp

#include "cocos2d.h"
USING_NS_CC;
class CardSprite : public cocos2d::Sprite
{
public:
    static CardSprite* createCard(int num, int cardWidth, int cardHeight, float px, float py);
    virtual bool init();
    CREATE_FUNC(CardSprite);
    
    void setNumber(int num);
    int getNumber();
    LayerColor* getCardLayer();
    
private:
    int number;
    void cardInit(int num, int cardWidth, int cardHeight, float px, float py);
    Label *cardLabel;
    LayerColor *cardBgColour;
    
};

#endif /* CardSprite_hpp */
