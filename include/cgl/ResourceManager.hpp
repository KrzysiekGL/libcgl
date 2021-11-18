/*
 * ResourceManager class is designed to keep track of all Resource type objects
 */

#ifndef RESOURCEMANAGER_H_
#define RESOURCEMANAGER_H_

#include "Resource.hpp"
#include "ShaderProgram.hpp"
#include "Model.hpp"
#include "Actor.hpp"
#include "Camera.hpp"

#include <map>
#include <string>
#include <memory>
#include <vector>
#include <iostream>

namespace CGL {

class ResourceManager {
public:
	ResourceManager();

	/*
	 * Prevent from accidental copy
	 */
	ResourceManager(const ResourceManager &other) = delete;
	ResourceManager& operator=(const ResourceManager &other) = delete;

	/*
	 * Add resource to the collection
	 * Return true if success, otherwise return false
	 */
	bool AddResource(std::shared_ptr<Resource> resource);

	/*
	 * Getters:
	 * for vectors - return empty vector if nothing
	 * for shared_ptr - return nullptr if nothing
	 */
	std::vector<std::string> GetAllResourcesNames() const;
	std::vector<std::shared_ptr<Resource>> GetAllResourcesByType(Type type) const;
	std::shared_ptr<Resource> GetResourceByName(std::string name) const;

	/*
	 * Delete
	 */
	void DeleteResourcesByNames(std::vector<std::string> names);
	void DeleteResourcesByTypes(std::vector<Type> types);

private:
	/*
	 * key - name of a resource; value - shared_ptr to the memory where object is located
	 */
	std::map<std::string, std::shared_ptr<Resource>> collection;
};

} /* namespace CGL */

#endif /* RESOURCEMANAGER_H_ */
