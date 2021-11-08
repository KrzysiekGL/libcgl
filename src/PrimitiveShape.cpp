#include "PrimitiveShape.hpp"

namespace CGL {
/* Ctor & Dtor */
PrimitiveShape::PrimitiveShape(std::string name, Shape shape) {
	// Resource configuration
	setName(name); setType(Type::PHYSICSBODY);

	// Physics body configuration
	type = shape;
	body = nullptr;
} /* PrimitiveShape::PrimitiveShape(std::string name, Shape shape) */
/* Ctor & Dtor */
/* Public Methods */
void PrimitiveShape::SetupPlane(btDiscreteDynamicsWorld * dynamicWorld, glm::mat4 initialModelMatrix, btVector3 planeNormal, btScalar planeConstnt) {
	if(type != Shape::PLANE) {
		std::cout << "CGL::WARNING::PRIMITIVESHAPE::SETUPPLANE() This shape is NOT A PLANE, it cant't be setup like one\n";
		return;
	}

	// Shape
	btCollisionShape * bulletShape = new btStaticPlaneShape(planeNormal, planeConstnt);

	// Rigid body setup
	setupRigidBody(dynamicWorld, bulletShape, initialModelMatrix, 0.f);
} /* PrimitiveShape::SetupPlane(btDiscreteDynamicsWorld * dynamicWorld, glm::mat4 initialModelMatrix, btVector3 planeNormal, btScalar planeConstnt) */

void PrimitiveShape::SetupBox(btDiscreteDynamicsWorld * dynamicWorld, glm::mat4 initialModelMatrix, btScalar mass, btVector3 boxDimensions) {
	if(type!=Shape::BOX){
		std::cout << "CGL::WARNING::PRIMITIVESHAPE::SETUPPLANE() This shape is NOT A BOX, it cant't be setup like one\n";
		return;
	}

	// Shape
	btCollisionShape * bulletShape = new btBoxShape(boxDimensions);

	// Rigid body setup
	setupRigidBody(dynamicWorld, bulletShape, initialModelMatrix, mass);
} /* PrimitiveShape::SetupBox(btDiscreteDynamicsWorld * dynamicWorld, glm::mat4 initialModelMatrix, btScalar mass, btVector3 boxDimensions) */

void PrimitiveShape::SetupSpeher(btDiscreteDynamicsWorld * dynamicWorld, glm::mat4 initialModelMatrix, btScalar mass, btScalar sphereRadius) {
	if(type!=Shape::SPHERE){
		std::cout << "CGL::WARNING::PRIMITIVESHAPE::SETUPPLANE() This shape is NOT A SPHERE, it cant't be setup like one\n";
		return;
	}

	// Shape
	btCollisionShape * bulletShape = new btSphereShape(sphereRadius);

	// Rigid body setup
	setupRigidBody(dynamicWorld, bulletShape, initialModelMatrix, mass);
} /* PrimitiveShape::SetupSpeher(btDiscreteDynamicsWorld * dynamicWorld, glm::mat4 initialModelMatrix, btScalar mass, btScalar sphereRadius) */

glm::mat4 PrimitiveShape::GetModelMatrix() const {
	btTransform transform;
	if(body && body->getMotionState())
		body->getMotionState()->getWorldTransform(transform);
	else
		transform = body->getWorldTransform();

	glm::mat4 modelMatrix;
	transform.getOpenGLMatrix(glm::value_ptr(modelMatrix));
	return modelMatrix;
} /* PrimitiveShape::GetModelMatrix(glm::mat4 & matrix) */

void PrimitiveShape::SetLinearVelocity(btVector3 vector, btScalar value) {
	body->setLinearVelocity(value*vector);
} /* PrimitiveShape::SetLinearVelocity(btVector3 vector, btScalar value) */
/* Public Methods */
/* Private Methods */
void PrimitiveShape::setupRigidBody(btDiscreteDynamicsWorld * dynamicWorld, btCollisionShape * bulletShape, glm::mat4 initialModelMatrix, btScalar mass) {
	// Initial world transformation
	btTransform transform;
	transform.setFromOpenGLMatrix(glm::value_ptr(initialModelMatrix));

	// Dynamics
	btVector3 localInertia(0.f, 0.f, 0.f);
	if(mass!=0.f)
		bulletShape->calculateLocalInertia(mass, localInertia);

	// Motionstate
	btDefaultMotionState * motionState = new btDefaultMotionState(transform);

	// Rigid body
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, bulletShape, localInertia);
	body = new btRigidBody(rbInfo);

	// Add to the dynamic world
	dynamicWorld->addRigidBody(body);
} /* PrimitiveShape::setupRigidBody(btDiscreteDynamicsWorld * dynamicWorld) */
/* Private Methods */
} /* namespace CGL */
