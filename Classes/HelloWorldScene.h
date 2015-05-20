#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

#define SHADER_BLUR

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

	virtual void onTouchEnded(cocos2d::Touch*, cocos2d::Event*);
	virtual bool onTouchBegan( cocos2d::Touch *touch, cocos2d::Event *unused_event );
    
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
