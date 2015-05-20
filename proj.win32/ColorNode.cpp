#include "ColorNode.h"


ColorNode::ColorNode(void)
	:_texture(NULL), _textureRenderer(NULL),
	_sprite(NULL)
{
	_enable = false;
}


ColorNode::~ColorNode(void)
{
	CC_SAFE_RELEASE(_texture);
	CC_SAFE_RELEASE(_textureRenderer);
	CC_SAFE_RELEASE(_sprite);
}

bool ColorNode::init()
{
	if (!Node::init())
		return false;

	setShaderProgram(GLProgram::createWithFilenames("ccShader_blur.vert", "Color.frag"));

	_sprite = Sprite::create("bird.png");
	if (!_sprite)
		return false;
	_sprite->retain();
	_sprite->setAnchorPoint(Point::ZERO);
	setContentSize(_sprite->getContentSize());

	Size size = getContentSize();
	_textureRenderer = RenderTexture::create(size.width, size.height);
	_textureRenderer->retain();
	_texture = _textureRenderer->getSprite()->getTexture();
	if (!_texture)
		return false;
	_texture->retain();

	_vertices.push_back(Point::ZERO);
	_vertices.push_back(Point(size.width, 0));
	_vertices.push_back(Point(0, size.height));
	_vertices.push_back(Point(size.width, size.height));

	_texCoords.push_back(Point::ZERO);
	_texCoords.push_back(Point(1, 0));
	_texCoords.push_back(Point(0, 1));
	_texCoords.push_back(Point(1, 1));

	scheduleUpdate();



	return true;
}

ColorNode* ColorNode::create()
{
	ColorNode* result = new ColorNode();
	if (result && result->init())
	{
		result->autorelease();
	} else {
		delete result;
		result = nullptr;
	}
	return result;
}

void ColorNode::update(float delta)
{
	_textureRenderer->beginWithClear(0, 0, 0, 0);
	_sprite->visit();
	_textureRenderer->end();
}

void ColorNode::draw(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
	_customCommand.init(_globalZOrder);
	_customCommand.func = [=]()->void {
		CC_NODE_DRAW_SETUP();
		GLint uEnable = getShaderProgram()->getUniformLocation("u_enable");
		glUniform1i(uEnable, _enable);
		Size winSize = this->getBoundingBox().size;
		GL::bindTexture2D(_texture->getName());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_TEX_COORD);
		glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0,  &_vertices[0]);
		glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, 0, &_texCoords[0]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, _vertices.size());
	};
	Director::getInstance()->getRenderer()->addCommand(&_customCommand);
}

void ColorNode::switchEnable()
{
	_enable = !_enable;
}
