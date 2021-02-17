/*
 * Mesh class is designed to keep a mesh data like:
 * - vertices (position, normals, and texture coordinates)
 * - textures with their OpenGL texture ID and their path in a OS
 * This class is also designed to draw a mesh that it stores
 * when appropriate shader is provided
 */
#ifndef MESHH
#define MESHH

#include <GL/glew.h>

#include <glm/glm.hpp>

#include <string>
#include <vector>
#include <memory>

#include "ShaderProgram.h"

namespace CGL {

	/*
	 * A Vertex structure to be hold by a mesh (might be developed in the future)
	 */
	struct Vertex {
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};

	/*
	 * A Texture structure which consist of a OpenGL's ID and a full path of a texture file
	 */
	struct Texture {
		GLuint id;
		std::string type;
		std::string path;
	};

	class Mesh {
	public:

		/*
		 * Creates a mesh from given vertices, indices (for rendering order) and textures
		 */
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

		/*
		 * Render a mesh using given ShaderProgram
		 */
		void Draw(std::shared_ptr<ShaderProgram> shader);

		/*
		 * Mesh data
		 */
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;
	private:

		/*
		 * Variables store OpenGL's ID's for the following:
		 * VAO - Vertex Array Object (VertexArray)
		 * VBO - Vertex Buffer Object (GL_ARRAY_BUFFER)
		 * EBO - Element Buffer Object (GL_ELEMENT_ARRAY_BUFFER)
		 */
		unsigned int VAO, VBO, EBO;

		/*
		 * Create mesh from given vertices and indices and textures.
		 * Generate OpenGL buffers to store this data on the GPU
		 */
		void setupMesh();
	};
} // namespace CGL

#endif // !MESHH 
