#include "../../Include/RmlUi/Core/Transform.h"
#include "../../Include/RmlUi/Core/Property.h"
#include "../../Include/RmlUi/Core/StyleSheetSpecification.h"
#include "../../Include/RmlUi/Core/TransformPrimitive.h"

namespace Rml {

Transform::Transform() {}

Transform::Transform(PrimitiveList primitives) : primitives(std::move(primitives)) {}

extern "C" RMLUICORE_API Property Transform_MakeProperty(TransformPrimitive* p_prims, int len)
{
	Rml::Transform::PrimitiveList list;
	list.reserve(len);
	for (long unsigned int i=0; i<len * sizeof(TransformPrimitive); i += sizeof(TransformPrimitive))
	{
		intptr_t ptr = (intptr_t)p_prims + i;
		TransformPrimitive* elm = (TransformPrimitive*)ptr;
		list.push_back(*elm);
	}
	return Transform::MakeProperty(list);
}

Property Transform::MakeProperty(PrimitiveList primitives)
{
	Property p(MakeShared<Transform>(std::move(primitives)), Unit::TRANSFORM);
	p.definition = StyleSheetSpecification::GetProperty(PropertyId::Transform);
	return p;
}

void Transform::ClearPrimitives()
{
	primitives.clear();
}

void Transform::AddPrimitive(const TransformPrimitive& p)
{
	primitives.push_back(p);
}

int Transform::GetNumPrimitives() const noexcept
{
	return (int)primitives.size();
}

const TransformPrimitive& Transform::GetPrimitive(int i) const noexcept
{
	return primitives[i];
}

} // namespace Rml
