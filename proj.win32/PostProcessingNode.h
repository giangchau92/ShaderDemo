#pragma once
#include <cocos2d.h>
#include <vector>

using namespace cocos2d;
using namespace std;

class Shape
{
public:
	Point position;
	Vec2 velocity;
	float r;
public:
	Shape(Point pos, Vec2 vel, float r);
};

class PostProcessingNode : public Node
{
public:
	PostProcessingNode(void);
	~PostProcessingNode(void);

	virtual bool init();
	virtual void update(float delta);
	virtual void draw(Renderer *renderer, const Mat4& transform, uint32_t flags);

	static PostProcessingNode* create();
private:
	void initVector();
private:
	vector<Shape*> _listPoints;
	CustomCommand _customCommand;
	RenderTexture* _textureRenderer;
	Sprite* _sprite;
	Sprite* _sprite2;
};
