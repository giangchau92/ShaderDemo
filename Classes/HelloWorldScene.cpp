#include "HelloWorldScene.h"
#include "proj.win32\BlurNode.h"
#include "proj.win32\PostProcessingNode.h"
#include "proj.win32\BlurNode2Pass.h"
#include "proj.win32\BlurNodeSampling.h"
#include "proj.win32\ColorNode.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	Size winSize = Director::getInstance()->getWinSize();
	ColorNode* node = ColorNode::create();
	node->setAnchorPoint(Point(0.5f, 0.5f));
	node->setPosition(winSize.width / 2, winSize.height / 2);
	node->setName("color_node");
	addChild(node);

	auto touchListener = EventListenerTouchOneByOne::create();

	touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    return true;
}

void HelloWorld::onTouchEnded(cocos2d::Touch*, cocos2d::Event*)
{
	((ColorNode*)getChildByName("color_node"))->switchEnable();
}

bool HelloWorld::onTouchBegan(Touch *touch, Event *unused_event)
{
	return true;
}
