/*
 * Model class was designed to store an array of meshes from which 3D model is created
 * Assimp third party library is essential for this class
 * All what this class do is:
 * - load a 3D model from a binary file
 * - process its nodes prepared by Assimp
 * - store every mesh as a Mesh class object
 * - draw a 3D model by drawing every mesh with a given ShaderProgram
 */

#ifndef MODELH
#define MODELH

#include <assimp/config.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <SOIL2/SOIL2.h>

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "Mesh.h"
#include "ShaderProgram.h"

namespace CGL {

	/*
	 * Load a texture from file and immediately store it on the GPU
	 * also set OpenGL's texture parameters (glTexParameteri)
	 * and generate Mipmaps
	 * Returns only a OpenGL's texture ID
	 * NOTE: If you are compiling this library on a Windows machine,
	 *       you have to define one of these flags:
	 *       - "WIN32"
	 *       - "_WIN32"
	 *       - "__WIN32"
	 *       Otherwise textures won't be vertically flip thus resulting
	 *       in unexpected behavior after rendering them
	 *       (for some reason it is not required on a GNU/Linux machine)
	 */
	unsigned int TextureFromFile(const char* file, const std::string directory, bool gamma = false);

	class Model {
	public:
		Model() {}

		/*
		 * Load a 3D model binary from given path
		 */
		Model(std::string path);

		/*
		 * Draw all meshes with a given ShaderProgram
		 */
		void Draw(std::shared_ptr<ShaderProgram> shader);

		/*
		 * Get path to the model directory
		 */
		std::string GetDirectory() const;

	private:

		/*
		 * Load a given 3D model with a Assimp importer and process all of Assimp's nodes
		 */
		void loadModel(std::string path);

		/*
		 * Process Assimp's node:
		 * - check if there are any meshes, and if so, process them
		 * - check if this node is a parent node for another node
		 */
		void processNode(aiNode* node, const aiScene* scene);

		/*
		 * Process Assimp's mesh:
		 * - extract all vertices -- position, normal, texture coordinates
		 *   (there are more provided from Assimp, but for now only those three are used)
		 * - extract all indices from a mesh
		 * - get all textures categorized by a texture type
		 *   (here, only DIFFUESE and SPECULAR, but there are more)
		 */
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);

		/*
		 * Extract all of textures by a given TYPE and return them as an array
		 * Skip those that were already loaded
		 */
		std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

		// model data
		std::vector<Mesh> meshes;
		std::vector<Texture> textures_loaded;
		std::string directory;
	};
} // namespace CGL

#endif // !MODELH
