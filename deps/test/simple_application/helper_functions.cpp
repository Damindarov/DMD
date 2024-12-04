#include "helper_functions.hpp"

std::optional<std::shared_ptr<MR::Mesh>> ExamplePlugin::convertObjectToMesh(std::shared_ptr<MR::Object> obj)
{
	if (obj->typeName() == "ObjectMesh")
	{
		return std::static_pointer_cast<MR::ObjectMesh>(obj)->varMesh();
	}
	return std::nullopt;
}
