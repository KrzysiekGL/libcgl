#include "ResourceManager.hpp"

namespace CGL {

/* Ctor & Dtor */
ResourceManager::ResourceManager() {}
/* Ctor & Dtor */
/* Public Methods */
bool ResourceManager::AddResource(std::shared_ptr<Resource> resource) {
	// Check if Name is not taken
	std::string name = resource->GetName();
	auto it = collection.find(name);
	if(it != collection.end()) {
		std::cout << "CGL::WARNING::RESOURCEMANAGER::ADDRESOURCE() Name " << name << " taken\n";
		return false;
	}
	// If Name is not taken add the resource to the collection
	collection[name] = resource;
	return true;
} /* ResourceManager::AddResource(std::shared_ptr<Resource> resource) */

std::vector<std::string> ResourceManager::GetAllResourcesNames() const {
	std::vector<std::string> names;
	for(auto & pair : collection)
		names.push_back(pair.first);
	return names;
} /* ResourceManager::GetAllResourcsByName() const*/

std::vector<std::shared_ptr<Resource>> ResourceManager::GetAllResourcesByType(Type type) const {
	std::vector<std::shared_ptr<Resource>> resources;
	for(auto & pair : collection)
		if(pair.second->GetType() == type)
			resources.push_back(pair.second);
	return resources;
} /* ResourceManager::GetAllResourcesByType(Type type) const*/

std::shared_ptr<Resource> ResourceManager::GetResourceByName(std::string name) const {
	// Check if resource of the name is present
	auto it = collection.find(name);
	if(it == collection.end()) {
		std::cout << "CGL::WARNING::RESOURCEMANAGER::GETRESOURCESBYNAME Resource with name " << name << " is not present in the colleciton\n";
		return nullptr;
	}
	return it->second;
} /* ResourceManager::GetResourceByName(std::string name) const */

void ResourceManager::DeleteResourcesByNames(std::vector<std::string> names) {
	for(auto name : names) {
		auto it = collection.find(name);
		if(it != collection.end())
			collection.erase(it);
	}
} /* ResourceManager::DeleteResourcesByNames(std::vector<std::string> names) */

void ResourceManager::DeleteResourcesByTypes(std::vector<Type> types) {
	for(auto & pair : collection) {
		for(auto & type : types) {
			if(type == pair.second->GetType()) {
				collection.erase(pair.first);
				break;
			}
		}
	}
} /* ResourceManager::DeleteResourcesByTypes(std::vector<Type> types) */
/* Public Methods */
} /* namespace CGL */
