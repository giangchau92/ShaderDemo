#include "BlurNode2Pass.h"


BlurNode2Pass::BlurNode2Pass(void)
	: _textureBall(NULL),
	_shaderHorizontal(NULL),
	_shaderVertical(NULL),
	_textureRender(NULL),
	_textureOld(NULL)
{
}


BlurNode2Pass::~BlurNode2Pass(void)
{
	CC_SAFE_RELEASE(_textureBall);
	CC_SAFE_RELEASE(_shaderHorizontal);
	CC_SAFE_RELEASE(_shaderVertical);
	CC_SAFE_RELEASE(_textureRender);
	CC_SAFE_RELEASE(_textureOld);
}

bool BlurNode2Pass::init()
{
	if (!Node::init())
		return false;
	// shader
	_shaderHorizontal = GLProgram::createWithFilenames("ccShader_blur.vert", "Blur_horizontal.frag");
	if (!_shaderHorizontal)
		return false;
	_shaderHorizontal->retain();
	_shaderVertical = GLProgram::createWithFilenames("ccShader_blur.vert", "Blur_vertical.frag");
	if (!_shaderVertical)
		return false;
	_shaderVertical->retain();
	
	// texture renderer
	_textureBall = TextureCache::getInstance()->addImage("cballs.png");
	if (!_textureBall)
		return false;
	_textureBall->retain();
	setContentSize(_textureBall->getContentSize());
	Size size = getContentSize();
	//

	_textureRender = RenderTexture::create(size.width, size.height);
	_textureRender->retain();
	_textureOld = _textureRender->getSprite()->getTexture();
	if (!_textureOld)
		return false;
	_textureOld->retain();

	_vertices.push_back(Point::ZERO);
	_vertices.push_back(Point(size.width, 0));
	_vertices.push_back(Point(0, size.height));
	_vertices.push_back(Point(size.width, size.height));

	_texCoords.push_back(Point(0, 1));
	_texCoords.push_back(Point(1, 1));
	_texCoords.push_back(Point::ZERO);
	_texCoords.push_back(Point(1, 0));

	_texCoords2.push_back(Point::ZERO);
	_texCoords2.push_back(Point(1, 0));
	_texCoords2.push_back(Point(0, 1));
	_texCoords2.push_back(Point(1, 1));

	scheduleUpdate();

	return true;
}

BlurNode2Pass* BlurNode2Pass::create()
{
	BlurNode2Pass* result = new BlurNode2Pass();
	if (result && result->init())
	{
		result->autorelease();
	} else {
		delete result;
		result = NULL;
	}
	return result;
}

void BlurNode2Pass::update( float delta )
{
	CCLOG("Time: %f", delta);
	_textureRender->beginWithClear(1, 0, 0, 1);
	_customCommandH.init(_globalZOrder);
	_customCommandH.func = [=]()->void {
		_shaderHorizontal->use();
		_shaderHorizontal->setUniformsForBuiltins(Mat4::IDENTITY);

		GLint uResolution = _shaderHorizontal->getUniformLocation("u_resolution");
		Size winSize = this->getBoundingBox().size;
		glUniform2f(uResolution, winSize.width, winSize.height);
		GL::bindTexture2D(_textureBall->getName());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_TEX_COORD);
		glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0,  &_vertices[0]);
		glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, 0, &_texCoords2[0]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, _vertices.size());
	};
	Director::getInstance()->getRenderer()->addCommand(&_customCommandH);
	_textureRender->end();
}

void BlurNode2Pass::draw( Renderer *renderer, const Mat4& transform, uint32_t flags )
{
	_customCommandV.init(_globalZOrder);
	_customCommandV.func = [=]()->void {
		_shaderVertical->use();
		_shaderVertical->setUniformsForBuiltins(_modelViewTransform);
		GLint uLocation = _shaderVertical->getUniformLocation("u_resolution");
		Size winSize = this->getBoundingBox().size;
		glUniform2f(uLocation, winSize.width, winSize.height);
		GL::bindTexture2D(_textureOld->getName());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_TEX_COORD);
		glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0,  &_vertices[0]);
		glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, 0, &_texCoords[0]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, _vertices.size());

	};
	renderer->addCommand(&_customCommandV);
}
