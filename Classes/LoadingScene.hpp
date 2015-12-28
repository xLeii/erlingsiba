//
//  LoadingScene.hpp
//  erba
//
//  Created by xiaoleiji on 15/12/28.
//
//

#ifndef LoadingScene_hpp
#define LoadingScene_hpp

#include "cocos2d.h"

class LoadingScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(LoadingScene);
    void loadingCallBack(float dt);
};

#endif /* LoadingScene_hpp */
