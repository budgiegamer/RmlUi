#pragma once

#include "Header.h"
#include "Types.h"

namespace Rml {

// Odin compatabillity
struct  IVertex
{
	virtual void getPos(float* pos) = 0;
	//virtual void setPos(float x, float y) = 0;
	virtual void getTexcord(float* pos) = 0;
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

	virtual void getPos(float* pos) override
	{
		//float* pos = new float[2];//{position.x, position.y};
		pos[0] = position.x;
		pos[1] = position.y;
		//return &pos[0];
	}
	/*virtual void setPos(float x, float y) override
	{
		position.x = x;
		position.y = y;
	}*/
	virtual void getTexcord(float* pos) override
	{
		//float* pos = new float[2];
		pos[0] = tex_coord.x;
		pos[1] = tex_coord.y;
		//return &pos[0];
	}
	/*virtual void setTexcord(float x, float y) override
	{
		tex_coord.x = x;
		tex_coord.y = y;
	}*/
};

} // namespace Rml
