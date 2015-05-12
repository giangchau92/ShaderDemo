#pragma once
#include <cocos2d.h>
#include <vector>

using namespace cocos2d;
using namespace std;

class BlurNode : public Node
{
private:
	Texture2D* _texture;
	CustomCommand _customComand;
	vector<Point> _vertices;
	vector<Vec2> _texCoord;

	GLint _uSolutionLocation;
public:
	virtual bool init();
	virtual void draw(Renderer *renderer, const Mat4& transform, uint32_t flags);
	BlurNode(void);
	~BlurNode(void);

	static BlurNode* create();
};

