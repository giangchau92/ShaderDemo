#pragma once
#include <cocos2d.h>
#include <vector>

using namespace cocos2d;
using namespace std;

class BlurNode : public Node
{
private:
	Texture2D* _texture; // texture2D for drawing
	CustomCommand _customComand;
	vector<Point> _vertices;
	vector<Vec2> _texCoord;

	GLint _uSolutionLocation; // save location of uniform variable u_soulution
public:
	virtual bool init();
	virtual void update(float delta);
	virtual void draw(Renderer *renderer, const Mat4& transform, uint32_t flags);
	BlurNode(void);
	~BlurNode(void);

	static BlurNode* create();
};

