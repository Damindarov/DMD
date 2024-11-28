#ifndef HELPER_FUNCTIONS_HPP
#define HELPER_FUNCTIONS_HPP

#include <optional>
#include <memory>
#include <MRMesh/MRMesh.h>
#include <MRMesh/MRObjectMesh.h>

namespace ExamplePlugin {

	std::optional<std::shared_ptr<MR::Mesh>> convertObjectToMesh(std::shared_ptr<MR::Object> obj);

}; // namespace ExamplePlugin

#endif// HELPER_FUNCTIONS_HPP