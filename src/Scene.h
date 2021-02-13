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
 * 1. create a Scene with a Camera as a parameter
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

namespace CGL {

struct Actor {
	std::map<std::string, Model>::iterator modelIterator;
	std::map<std::string, ShaderProgram>::iterator shaderProgramIterator;
	glm::mat4 modelMatrix;
	// TODO Add actor collection sorting with regard to transparency
	bool transparent;
};

class Scene {
public:

	/*
	 * Scene consists of a Camera object and screen size parameters.
	 */
	Scene();
	Scene(Camera camera);
	~Scene();

	/*
	 * ShaderPrograms are stored in shaderColleciton map and Models are stored in modelCollection map.
	 * Each ShaderProgram and Model is associated with a name.
	 */
	void AddShaderProgram(std::string shaderProgram_name, ShaderProgram shaderProgram);
	void AddModel(std::string model_name, Model model);

	// TODO Add method to modify Actor's model matrix
	/*
	 * This method adds Actor to the scene.
	 * Both Model and ShaderProgram objects has to be present at the time
	 * of calling AddActor().
	 * If Model/ShaderProgram with the given name doesn't exist,
	 * `false` will be returned, otherwise `true`.
	 */
	bool AddActor(std::string model_name, std::string shader_name);

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

private:
	/*
	 * Collection of loaded Models/ShaderPrograms/Actors into scene in form
	 * of a map {std::string, T}
	 */
	std::map<std::string, Model> modelCollection;
	std::map<std::string, ShaderProgram> shaderProgramCollection;
	std::map<std::string, Actor> actorCollection;

	/*
	 * Camera object (and freeCam mode flag) to move in a scene
	 * and interact with it.
	 */
	Camera camera;
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
