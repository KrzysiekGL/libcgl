/*
 * Pure virtual class to be inherited by Physics Body shapes
 * It also inherits from Resource, so it could be managed by ResourceManager
 */

#ifndef PRIMITIVESHAPE_H_
#define PRIMITIVESHAPE_H_

#include "Resource.hpp"

#include <btBulletDynamicsCommon.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>

namespace CGL {

enum class Shape {
	PLANE,
	BOX,
	SPHERE,
};

class PrimitiveShape : public Resource {
public:
	PrimitiveShape(std::string name, Shape shape);

	/*
	 * Delete to prevent from accidental copying
	 */
	PrimitiveShape(const PrimitiveShape &other) = delete;
	PrimitiveShape& operator=(const PrimitiveShape &other) = delete;

	/*
	 * Setup a PLANE (Only static for now)
	 */
	void SetupPlane(btDiscreteDynamicsWorld * dynamicWorld, glm::mat4 initialModelMatrix, btVector3 planeNormal, btScalar planeConstnt);

	/*
	 * Setup a BOX
	 */
	void SetupBox(btDiscreteDynamicsWorld * dynamicWorld, glm::mat4 initialModelMatrix, btScalar mass, btVector3 boxDimensions);

	/*
	 * Setup a Sphere
	 */
	void SetupSpeher(btDiscreteDynamicsWorld * dynamicWorld, glm::mat4 initialModelMatrix, btScalar mass, btScalar sphereRadius);

	/*
	 * Get rigid body
	 */
	btRigidBody * GetRigidBody() const;

	/*
	 * Get model matrix from body motion state
	 */
	glm::mat4 GetModelMatrix() const;

	/*
	 * Set linear velocity of a body
	 */
	void SetLinearVelocity(btVector3 vector, btScalar value);

private:
	Shape type;
	btRigidBody * body;

	void setupRigidBody(btDiscreteDynamicsWorld * dynamicWorld, btCollisionShape * bulletShape, glm::mat4 initialModelMatrix, btScalar mass);

};
} /* namespace CGL */

#endif /* PRIMITIVESHAPE_H_ */
