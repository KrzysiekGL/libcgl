#ifndef ACTOR_H_
#define ACTOR_H_

#include "Model.h"
#include "ShaderProgram.h"

#include <glm/glm.hpp>

#include <btBulletDynamicsCommon.h>

#include <memory>

namespace CGL {

class Actor {
public:
	Actor();

	Actor(
		std::shared_ptr<Model> sharedModel,
		std::shared_ptr<ShaderProgram> sharedShaderProgram,
		btRigidBody * body,
		bool isTransparent);

	/*
	 * Delete shape ptr
	 */
	~Actor();

	/*
	 * Delete Copy Constructor and operator=
	 */
	Actor(const Actor & other) = delete;
	Actor & operator=(const Actor & other) = delete;

	/*
	 * Draw an actor
	 */
	void Draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

	/*
	 *  Set linear velocity of this actor
	 */
	void SetLinearVelocity(glm::vec3 direction, float value);

	/*
	 * Getters
	 */
	std::shared_ptr<Model> GetSharedModel() const;
	std::shared_ptr<ShaderProgram> GetSharedShaderProgram() const;
	btRigidBody * const GetRigidBody() const;
	glm::mat4 GetModelMatrix() const;

	/*
	 * Setters
	 */
	//void SetModelMatrix(glm::mat4 modelMatrix);

private:
	std::shared_ptr<Model> sharedModel;
	std::shared_ptr<ShaderProgram> sharedShaderProgram;
	btRigidBody * body;
	bool isTransparent;
};

}

#endif //ACTOR_H_
