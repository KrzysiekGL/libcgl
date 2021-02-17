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

#include "ShaderProgram.h"
#include "Camera.h"
#include "Model.h"

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <vector>
#include <map>
#include <iterator>
#include <memory>

namespace CGL {

struct Actor {
	std::shared_ptr<Model> sharedPtrModel;
	std::shared_ptr<ShaderProgram> sharedPtrShaderProgram;
	glm::mat4 modelMatrix;
	bool transparent;
};

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
	 * Scene can contain many Cameras. References to them are
	 * stored in the cameraCollection map<std::string, std::shared_ptr<Camera>>.
	 */
	void AddCamera(std::string camera_name, glm::vec3 camera_position=glm::vec3(0.f, 0.f, 3.f), float pith=0.f, float yaw=-90.f, float camera_sensitivity=.1f, float camera_speed=2.f);

	/*
	 * ShaderPrograms are stored in shaderColleciton map<std::string, std::shared_ptr<ShaderProgram>>
	 * and Models are stored in modelCollection map<std::string, std::shared_ptr<Model>>.
	 * Each ShaderProgram and Model is associated with a name (std::string).
	 */
	void AddShaderProgram(std::string shader_name, std::string vert_path, std::string frag_path);
	void AddModel(std::string model_name, std::string model_path);

	// TODO Add method to modify Actor's model matrix
	/*
	 * This method adds Actor to the scene.
	 * Both Model and ShaderProgram objects has to be present at the time
	 * of calling AddActor().
	 * References to all Actors are stored in the actorCollection
	 * map<std::string, std::shared_ptr<Actor>>.
	 *
	 * Actor consist of a Model and information about with which
	 * ShaderProgram it should be rendered.
	 *
	 * If Model/ShaderProgram with the given name doesn't exist,
	 * `false` will be returned, otherwise `true`.
	 */
	bool AddActor(std::string model_name, std::string shader_name, glm::mat4 model_matrix=glm::mat4(1.f), bool isTransparnt=false);

	// TODO Implement physics somewhere over here
	// TODO Integrate UI
	/*
	 * Update information about screen, process input events,
	 * render all actors.
	 */
	void RunScene(GLFWwindow* window, float deltaFrame, bool freeCam);


	/*
	 * Get names of ShaderPrograms/Models/Actors loaded into scene
	 */
	std::vector<std::string> GetShaderProgramCollectionNames();
	std::vector<std::string> GetModelCollectionNames();
	std::vector<std::string> GetActorCollectionNames();
	std::vector<std::string> GetCameraCollectionNames();

private:
	/*
	 * Collection of Models/ShaderPrograms/Actors/Cameras
	 * present in the Scene in form of a map<std::string, std::shared_ptr<T>>.
	 *
	 * Model - 3D/2D models placed in the world space
	 *
	 * ShaderProgram - GL shader programs to render Models
	 *
	 * Actor - pair of Model and ShaderProgram with information
	 *         with information if it's transparent and what
	 *         is it's Model Matrix
	 *
	 * Camera - object to move and interact with a scene
	 *          also generates perspective and view matrices
	 */
	std::map<std::string, std::shared_ptr<Model>> modelCollection;
	std::map<std::string, std::shared_ptr<ShaderProgram>> shaderProgramCollection;
	// TODO Add actor collection sorting with regard to transparency in the AddActor()
	std::map<std::string, std::shared_ptr<Actor>> actorCollection;
	std::map<std::string, std::shared_ptr<Camera>> cameraCollection;
	// Currently used Camera
	std::shared_ptr<Camera> current_camera;
	// Is freeCam mode enabled (affect all Cameras)
	bool freeCam;

	/*
	 * Screen width and height from GLFW frame buffer
	 * for calculating projection matrix.
	 */
	float scr_width; float scr_height;

	/*
	 * Draw all actors with respect of their model matrices.
	 */
	void draw();

	/*
	 * Get screen size from GLFW frame buffer.
	 */
	void updateSceneParameters(GLFWwindow* window);

	// TODO Modify scene with keyboard
	/*
	 * In freeCam mode pass keyboard input to a Camera object.
	 * Otherwise take control over Actors and Scene.
	 */
	void handleKeyboardInput(GLFWwindow* window, float deltaTime);

	// TODO Mouse picking with Camera object
	/*
	 * In freeCam mode pass mouse input to a Camera object.
	 * Otherwise take control over Actors and Scene.
	 */
	void handleMouseInput(GLFWwindow* window);
};

} // namespace CGL

#endif // SCENE_H_