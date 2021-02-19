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

	/*
	 * Delete shape ptr
	 */
	~Actor();

	/*
	 * Delete Copy Constructor and operator=
	 */
	Actor(const Actor & other) = delete;
	Actor & operator=(const Actor & other) = delete;

	void SetParameters(
			std::shared_ptr<Model> sharedModel,
			std::shared_ptr<ShaderProgram> sharedShaderProgram,
			btCollisionShape * shape,
			btRigidBody * body,
			glm::mat4 modelMatrix,
			bool isTransparent);

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
	void SetModelMatrix(glm::mat4 modelMatrix);

private:
	std::shared_ptr<Model> sharedModel;
	std::shared_ptr<ShaderProgram> sharedShaderProgram;
	btCollisionShape * shape;
	btRigidBody * body;
	glm::mat4 modelMatrix;
	bool isTransparent;
};

}

#endif //ACTOR_H_
