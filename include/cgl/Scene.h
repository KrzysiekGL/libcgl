/*
 * Scene class was designed to arrange and manipulate actors. An Actor is a Model associated with
 * a ShaderProgram with witch it should be rendered and a Model Matrix to modify it's position in the "world space".
 *
 * - add a Model
 * - add a ShaderProgram
 * - create a new Actor (optionally with a model matrix)
 * - Scene object has it's own Camera object, thus It also have to pass to it's Camera any control input
 *
 * To create a simple Scene and run it, do the following:
 * 1. create a Scene object
 * 2. add a Model with it's name
 * 3. add a ShaderProgram with it's name
 * 4. create a new Actor wit added Model and ShaderProgram
 * 5. Run cycle of the Scene with RunScene() (normally inside your "Game Loop")
 */

#ifndef SCENE_H_
#define SCENE_H_

#include "ResourceManager.h"
#include "ShaderProgram.h"
#include "Camera.h"
#include "Model.h"
#include "Actor.h"

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <btBulletDynamicsCommon.h>

#include <vector>
#include <map>
#include <iterator>

namespace CGL {

class Scene {
public:

	/*
	 * Scene consists of a Camera (or many Cameras) object(s),
	 * screen size parameters and camera settings,
	 * collection of 2D/3D models,
	 * and collection of shader programs to render with
	 */
	Scene();

	/*
	 * Delete dynamic world ptr and all of it's dependencies
	 */
	~Scene();

	/*
	 * Delete Copy Constructor and operator=
	 * to prevent from memory leaks with pointers
	 */
	Scene(const Scene & other) = delete;
	Scene & operator=(const Scene & other) = delete;

	/*
	 * Scene can contain many Cameras. References to them are
	 * stored in the cameraCollection map<std::string, Camera* >.
	 */
	std::string AddCamera(
			std::string camera_name,
			glm::vec3 camera_position=glm::vec3(0.f, 0.f, 3.f),
			float pith=0.f, float yaw=-90.f,
			float camera_sensitivity=.1f,
			float camera_speed=2.f);

	/*
	 * Model (loaded wit Assimp) and ShaderProgram for rendering
	 */
	std::string AddShaderProgram(std::string shader_name, std::string vert_path, std::string frag_path);
	std::string AddModel(std::string model_name, std::string model_path);


	/*
	 * This method adds Actor to the scene.
	 * Model, ShaderProgram and PrimitiveShape objects has to be
	 * present at the time of calling AddActor()
	 */
	std::string AddActor(std::string actor_name, std::string model_name, std::string shaderProgram_name, std::string primitiveShape_name, bool isTransparent=false);

	/*
	 * Add physics primitives for actors
	 */
	std::string AddPrimitivePlane(std::string body_name, glm::mat4 modelMatrix, btVector3 planeNormal, btScalar planeConstatnt);
	std::string AddPrimitiveBox(std::string body_name, glm::mat4 modelMatrix, btScalar mass, btVector3 boxDimensions);
	std::string AddPrimitiveSphere(std::string body_name, glm::mat4 modelMatrix, btScalar mass, btScalar sphereRadius);

	/*
	 * Delete an Actor from the collection
	 */
	void DelActor(std::string actorName);

	/*
	 * Set Acotr's linear velocity in Bullet
	 */
	void SetActorLinearVelocity(std::string actor_name, glm::vec3 direction, float value);

	/*
	 * Update information about screen, process input events,
	 * make Bullet dynamic world simulation step and render all actors.
	 */
	void RunScene(GLFWwindow* window, float deltaFrame, bool freeze, bool freeCam);

	/*
	 * Get names of Resources of a given Type loaded into the ResourceManger
	 */
	std::vector<std::string> GetCollectionNames(Type type) const;

	/*
	 * Get current camera parameters
	 */
	glm::vec3 GetCameraPosition() const;
	glm::vec3 GetCameraFront() const;

private:
	/*
	 * Screen width and height from GLFW frame buffer
	 * for calculating projection matrix.
	 */
	float scr_width; float scr_height;

	/*
	 * Collection of Models/ShaderPrograms/Actors/Cameras in a ResourceManager
	 */
	std::shared_ptr<ResourceManager> rman;
	std::shared_ptr<Camera> current_camera;
	// Is freeCam mode enabled (affect all Cameras)
	bool freeCam;

	/*
	 * Bullet Dynamic World with it's dependencies
	 * (in creation order; delete in reveres order)
	 * All of them are deleted in the dtor
	 */
	btDefaultCollisionConfiguration * collisionConfiguration;
	btCollisionDispatcher * dispatcher;
	btBroadphaseInterface * broadphaseInterface;
	btSequentialImpulseConstraintSolver * solver;
	btDiscreteDynamicsWorld * dynamicWorld;

	/*
	 * Get shared_ptr to specific resources
	 */
	std::shared_ptr<ShaderProgram> getShaderProgram(std::string shaderProgram_name);
	std::shared_ptr<PrimitiveShape> getPrimitiveShape(std::string primitiveShape_name);
	std::shared_ptr<Model> getModel(std::string model_name);
	std::shared_ptr<Camera> getCamera(std::string camera_name);
	std::shared_ptr<Actor> getActor(std::string actor_name);

	/*
	 * Draw all actors with respect of their model matrices.
	 */
	void draw();

	/*
	 * Get screen size from GLFW frame buffer.
	 */
	void updateSceneParameters(GLFWwindow* window);

	/*
	 * In freeCam mode pass keyboard input to a Camera object.
	 * Otherwise take control over Actors and Scene.
	 */
	void handleKeyboardInput(GLFWwindow* window, float deltaTime);

	/*
	 * In freeCam mode pass mouse input to a Camera object.
	 * Otherwise take control over Actors and Scene.
	 */
	void handleMouseInput(GLFWwindow* window);
};

} // namespace CGL

#endif // SCENE_H_
