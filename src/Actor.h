#ifndef ACTOR_H_
#define ACTOR_H_

#include "Model.h"
#include "ShaderProgram.h"

#include <glm/glm.hpp>

#include <btBulletDynamicsCommon.h>

#include <memory>

namespace CGL {

class Scene; // forward declaration

class Actor {
public:
	Actor() {}

	Actor(
		Scene * rootScene,
		std::string shaderProgramName,
		std::string modelName,
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
	 * Set actor name
	 */
	void SetActorName(std::string actorName);

	/*
	 * Getters
	 */
	std::string GetModelName() const;
	std::string GetShaderProgramName() const;
	btRigidBody * const GetRigidBody() const;
	glm::mat4 GetModelMatrix() const;

	/*
	 * Setters
	 */
	//void SetModelMatrix(glm::mat4 modelMatrix);

private:
	Scene * rootScene;
	std::string actorName;
	std::string shaderProgramName;
	std::string modelName;
	btRigidBody * body;
	bool isTransparent;
};

}

#endif //ACTOR_H_
