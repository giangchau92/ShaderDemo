#pragma once
#include <cocos2d.h>
#include <vector>

using namespace cocos2d;
using namespace std;

class BlurNodeSampling : public Node
{
public:
	BlurNodeSampling(void);
	~BlurNodeSampling(void);
	virtual bool init();
	virtual void update(float delta);
	virtual void draw(Renderer *renderer, const Mat4& transform, uint32_t flags);
	static BlurNodeSampling* create();

private:
	vector<Point> _vertices;
	vector<Vec2> _texCoords;
	vector<Vec2> _texCoords2;
	RenderTexture* _textureRender;

	Texture2D* _textureBall;
	Texture2D* _textureOld;
	CustomCommand _customCommandV;
	CustomCommand _customCommandH;
	GLint _uSolutionLocation;

	GLProgram* _shaderHorizontal;
	GLProgram* _shaderVertical;
};

