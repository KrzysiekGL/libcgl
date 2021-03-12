/*
 * This class is a base class for any resource type object.
 * It consists of a name and type of a resource.
 */

#ifndef RESOURCE_H_
#define RESOURCE_H_

#include <string>

namespace CGL {

enum class Type {
	CAMERA,
	SHADERPROGRAM,
	MODEL,
	ACTOR,
	PHYSICSBODY,
};

class Resource {
public:
	virtual ~Resource();

	/*
	 * Getters
	 */
	Type GetType();
	std::string GetName();

protected:
	Type mType;
	std::string mName;

	/*
	 * Setters
	 */
	void setType(Type type);
	void setName(std::string name);
};

} /* namespace CGL */

#endif /* RESOURCE_H_ */
