#pragma once

#include "Header.h"
#include "Types.h"

namespace Rml {

struct  IVertex
{
	virtual float* getPos() = 0;
	//virtual void setPos(float x, float y) = 0;
	virtual float* getTexcord() = 0;
	//virtual void setTexcord(float x, float y) = 0;
};

/**
    The element that makes up all geometry sent to the renderer.
 */

struct RMLUICORE_API Vertex : IVertex {
	/// Two-dimensional position of the vertex (usually in pixels).
	Vector2f position;
	/// RGBA-ordered 8-bit/channel colour with premultiplied alpha.
	ColourbPremultiplied colour;
	/// Texture coordinate for any associated texture.
	Vector2f tex_coord;

	friend bool operator==(const Vertex& lhs, const Vertex& rhs)
	{
		return lhs.position == rhs.position && lhs.colour == rhs.colour && lhs.tex_coord == rhs.tex_coord;
	}
	friend bool operator!=(const Vertex& lhs, const Vertex& rhs) { return !(lhs == rhs); }

	virtual float* getPos() override
	{
		float pos[2] = {position.x, position.y};
		return &pos[0];
	}
	/*virtual void setPos(float x, float y) override
	{
		position.x = x;
		position.y = y;
	}*/
	virtual float* getTexcord() override
	{
		float pos[2] = {tex_coord.x, tex_coord.y};
		return &pos[0];
	}
	/*virtual void setTexcord(float x, float y) override
	{
		tex_coord.x = x;
		tex_coord.y = y;
	}*/
};

} // namespace Rml
