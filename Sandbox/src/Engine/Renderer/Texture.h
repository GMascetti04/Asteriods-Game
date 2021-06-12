#pragma once
#include "xpch.h"
#include "Engine/Utilities/Vec.h"
#include "Engine/Debug/Profiler.h"


#include "glm/glm.hpp"

class Texture;

class SpriteTexture {
public:
	virtual std::array<glm::vec2,2> getTextureCords() = 0; //bottom left, then top right
	virtual Texture* getbaseTexture() = 0;
	virtual ~SpriteTexture() {}
private:
};

/*
* The Texture class is an abstraction for the OpenGL texture and is modified and
* adapted from code in the Hazel Github repository (see Game.cpp citation [9])
*/
class Texture : public SpriteTexture {
public:
	~Texture();

	static Texture* ImageTexture(const char* path);
	
	/*
	* draw a colored rectangle inside of the texture
	* note: bottom left is (0,0)
	*/
	void paintColoredQuad(int x_offset, int y_offset, int height, int width, Vec4<unsigned char>color);

	//create a white blank texture of a specified size
	static Texture* BlankTexture(int width, int height);

	static Texture* createRenderTexture(int width, int height, unsigned int format);

	int getWidth() { return m_width; }
	int getHeight() { return m_height; }

	static int numberTextureSlots();

	 unsigned int getId() { return m_id; }


	 virtual std::array<glm::vec2, 2> getTextureCords() override { //bottom left, then top right
		 return { glm::vec2{0,0}, glm::vec2{1,1} };

	 }
	 virtual Texture* getbaseTexture() override {
		 return this;
	 }


private:
	Texture(unsigned char* data, int height, int width, unsigned int format);

	friend class Renderer;

	void bind(unsigned int Tslot);
	void unbind(unsigned int Tslot);

	unsigned int m_id;
	unsigned char* m_data;
	int m_width;
	int m_height;
};


class SubTexture : public SpriteTexture{
public:
	/*
	* minWidth: defines how many pixels xUnit will be
	* minHeight: defines how many pixels yUnit will be
	* x: bottom left x-cord of the sub texture (in xUnits)
	* y: bottom left y-cord of the sub texture (in yUnits)
	* xUnits: number of xUnits to make the width
	* yUnits: number of yUnits to make the height
	*/
	SubTexture(Texture* baseTexture, int minWidth, int minHeight, int x, int y, int xUnits, int yUnits, int xBuffer, int yBuffer) {
		m_baseTexture = baseTexture;
		m_pos = {x,y};
		m_dimensions = { xUnits, yUnits };
		m_units = {minWidth, minHeight};
	//	m_textureCordBottomLeft = {  (minWidth*x+0.5f)/(float)baseTexture->getWidth(), (minHeight * y+0.5f)/(float)baseTexture->getHeight()};
		m_textureCordBottomLeft = { ((2*xBuffer+minWidth)*x + 0.5f) / (float)baseTexture->getWidth(), 
			((2*yBuffer+minHeight)* y + 0.5f) / (float)baseTexture->getHeight() };

		m_textureCordTopRight = { ((2 * xBuffer + minWidth) * x + 0.5f + minWidth*xUnits )/(double)baseTexture->getWidth()
			, ((2 * yBuffer + minHeight) * y + 0.5f + minHeight * yUnits) / (double)baseTexture->getHeight() };

		std::cout << m_textureCordBottomLeft.x <<" "<< m_textureCordBottomLeft.y<< " ------ " << m_textureCordTopRight.x << "" << m_textureCordTopRight.y << std::endl;
		std::cout << m_baseTexture->getWidth() << " " << m_baseTexture->getHeight() << std::endl;
	}


	virtual std::array<glm::vec2, 2> getTextureCords() override { //bottom left, then top right
		return { m_textureCordBottomLeft, m_textureCordTopRight};

	}
	virtual Texture* getbaseTexture() override {
		return m_baseTexture;
	}

private:
	//pointer to the originial texture
	glm::vec2 m_pos;
	glm::vec2 m_dimensions;
	glm::vec2 m_units;
	glm::vec2 m_buffer;

	glm::vec2 m_textureCordBottomLeft;
	glm::vec2 m_textureCordTopRight;
	
	Texture* m_baseTexture;
};


struct SpriteSheet {
public:
	SpriteSheet(Texture* baseTexture, int minWidth, int minHeight, int xBuffer, int yBuffer) : m_baseTexture(baseTexture), m_minWidth(minWidth), m_minHeight(minHeight), m_xBuffer(xBuffer), m_yBuffer(yBuffer)
	{

	}

	SubTexture* makeSprite(int x, int y, int xUnits = 1, int yUnits = 1)
	{
		return new SubTexture(m_baseTexture, m_minWidth, m_minHeight, x, y, xUnits, yUnits, m_xBuffer, m_yBuffer);
	}

private:
	Texture* m_baseTexture;
	int m_minWidth;
	int m_minHeight;
	int m_xBuffer;
	int m_yBuffer;
};