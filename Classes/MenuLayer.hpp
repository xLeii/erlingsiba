//
//  MenuLayer.hpp
//  erba
//
//  Created by xiaoleiji on 15/12/28.
//
//

#ifndef MenuLayer_hpp
#define MenuLayer_hpp

#include "cocos2d.h"

class MenuLayer :public cocos2d::Layer
{
public:
    static MenuLayer* create(const cocos2d::Color4B& color);
    virtual bool initWithColor(const cocos2d::Color4B& color);
    
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
};

#endif /* MenuLayer_hpp */
