#include "PostProcessingNode.h"


PostProcessingNode::PostProcessingNode(void)
	:_textureRenderer(NULL),
	_sprite(NULL)
{
}


PostProcessingNode::~PostProcessingNode(void)
{
	CC_SAFE_RELEASE(_textureRenderer);
	CC_SAFE_RELEASE(_sprite);
}

bool PostProcessingNode::init()
{
	if (!Node::init())
		return false;

	Size winSize = Director::getInstance()->getWinSize();
	initVector();
	_textureRenderer = RenderTexture::create(winSize.width, winSize.height);
	_textureRenderer->retain();
	Size rect = _textureRenderer->getSprite()->getTexture()->getContentSize();
	_sprite = Sprite::createWithTexture(_textureRenderer->getSprite()->getTexture(), Rect(0, 0, rect.width, rect.height));
	_sprite->retain();
	_sprite2 = Sprite::create("HelloWorld.png");
	_sprite2->setAnchorPoint(Point::ZERO);
	_sprite2->retain();
	addChild(_sprite);
	scheduleUpdate();

	return true;
}

void PostProcessingNode::initVector()
{
	Size winSize = Director::getInstance()->getWinSize();
	float delta = 30;
	float R = 50;
	float minR = 100;
	for (int i = 0; i < 750; i++)
	{
		Point pos = Point(CCRANDOM_0_1() * winSize.width, CCRANDOM_0_1() * winSize.height);
		Point vel = Point(CCRANDOM_MINUS1_1() * delta, CCRANDOM_MINUS1_1() * delta);
		float r = CCRANDOM_0_1() * R + minR;
		Shape* shape = new Shape(pos, vel, r);
		_listPoints.push_back(shape);
	}
}

void PostProcessingNode::draw( Renderer *renderer, const Mat4& transform, uint32_t flags )
{
	//_customCommand.init(_globalZOrder);
	//_customCommand.func = [&]()->void {
	//	for (int i = 0; i < this->_listPoints.size(); i++)
	//	{
	//		Shape* shape = this->_listPoints[i];
	//		DrawPrimitives::drawCircle(shape->position, shape->r, M_PI * 2, 180, false);
	//	}
	//};
	//renderer->addCommand(&_customCommand);
}

void PostProcessingNode::update( float delta )
{
	Size winSize = Director::getInstance()->getWinSize();
	_textureRenderer->beginWithClear(0, 0, 0, 1);
	_customCommand.init(_globalZOrder);
	_customCommand.func = [=]()->void {
		for (int i = 0; i < this->_listPoints.size(); i++)
		{
			Shape* shape = this->_listPoints[i];
			if (shape->position.x > winSize.width || shape->position.x < 0)
				shape->velocity.x = -shape->velocity.x;
			if (shape->position.y > winSize.height || shape->position.y < 0)
				shape->velocity.y = -shape->velocity.y;
			shape->position += shape->velocity * delta;
			DrawPrimitives::drawCircle(shape->position, shape->r, M_PI * 2, 180, false);
		}
	};
	Director::getInstance()->getRenderer()->addCommand(&_customCommand);
	_textureRenderer->end();
	//for (int i = 0; i < this->_listPoints.size(); i++)
	//{
	//	Shape* shape = this->_listPoints[i];
	//	if (shape->position.x > winSize.width || shape->position.x < 0)
	//		shape->velocity.x = -shape->velocity.x;
	//	if (shape->position.y > winSize.height || shape->position.y < 0)
	//		shape->velocity.y = -shape->velocity.y;
	//	shape->position += shape->velocity * delta;
	//}
}

PostProcessingNode* PostProcessingNode::create()
{
	PostProcessingNode* result = new PostProcessingNode();
	if (!result || !result->init())
	{
		delete result;
		result = NULL;
	} else {
		result->autorelease();
	}
	return result;
}

Shape::Shape( Point pos, Vec2 vel, float R )
	: position(pos), velocity(vel), r(R)
{

}
