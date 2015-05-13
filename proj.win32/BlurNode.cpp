#include "BlurNode.h"


BlurNode::BlurNode(void)
{
	_texture = NULL;
}


BlurNode::~BlurNode(void)
{
	CC_SAFE_RELEASE(_texture);
}

bool BlurNode::init()
{
	if (!Node::init())
		return false;
	// read and add my shader
	GLProgram* program = GLProgram::createWithFilenames("ccShader_blur.vert", "BlurNormal.frag");
	if (!program)
		return false;
	setShaderProgram(program);
	// get uniform variable location
	_uSolutionLocation = program->getUniformLocation("u_resolution");
	// load texture2D
	_texture = TextureCache::getInstance()->addImage("cballs.png");
	if (!_texture)
		return false;
	_texture->retain();
	setContentSize(_texture->getContentSize());
	Size size = getContentSize();
	// init vertices array and texCoord array
	_vertices.push_back(Point::ZERO);
	_vertices.push_back(Point(size.width, 0));
	_vertices.push_back(Point(0, size.height));
	_vertices.push_back(Point(size.width, size.height));

	_texCoord.push_back(Point(0, 1));
	_texCoord.push_back(Point(1, 1));
	_texCoord.push_back(Point::ZERO);
	_texCoord.push_back(Point(1, 0));
	
	scheduleUpdate();

	return true;
}

void BlurNode::draw( Renderer *renderer, const Mat4& transform, uint32_t flags )
{
	// custom drawing
	_customComand.init(_globalZOrder);
	_customComand.func = [&]()->void {
		CC_NODE_DRAW_SETUP();
		Size winSize = getContentSize();
		glUniform2f(_uSolutionLocation, winSize.width, winSize.height);
		GL::bindTexture2D(_texture->getName());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_TEX_COORD);
		glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0,  &_vertices[0]);
		glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, 0, &_texCoord[0]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, _vertices.size());
		
	};
	renderer->addCommand(&_customComand);
}

BlurNode* BlurNode::create()
{
	BlurNode* result = new BlurNode();
	if (result && result->init())
	{
		result->autorelease();
	} else {
		delete result;
		result = nullptr;
	}
	return result;
}

void BlurNode::update( float delta )
{
	CCLOG("Time: %f", delta);
}
