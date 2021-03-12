/*
 * ResourceManager class is designed to keep track of all Resource type objects
 */

#ifndef RESOURCEMANAGER_H_
#define RESOURCEMANAGER_H_

#include "Resource.h"
#include "ShaderProgram.h"
#include "Model.h"
#include "Actor.h"
#include "Camera.h"

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
	 */
	void AddResource(std::shared_ptr<Resource> resource);

	/*
	 * Getters
	 */
	std::vector<std::string> GetAllResourcsByName() const;
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
