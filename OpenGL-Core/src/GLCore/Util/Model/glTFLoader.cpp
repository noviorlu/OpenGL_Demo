#include "glpch.h"
#include "glTFLoader.h"


#include "../Common/File.hpp"
#include "../Transform.hpp"

#include "Mesh.h"
#include "../GLAbstract/Texture.h"

#include <glm/gtc/type_ptr.hpp>

namespace GLCore::Utils {
	glTFLoader::glTFLoader(const char* file, Model* model)
	{
		assert(file != nullptr && "File name is nullptr");
		// Check if the file exists
		std::ifstream f(file);
		assert(f.good() && "File does not exist");

		// Make a JSON object
		std::string text = File::ReadFileAsString(file);
		JSON = json::parse(text);

		// Get the binary m_Data
		m_File = std::string(file);
		m_FileDir = m_File.substr(0, m_File.find_last_of('/') + 1);

		m_Data = getData();

		if(model == nullptr) return;
		m_Model = model;
		// Traverse all nodes
		for (size_t i = 0; i < JSON["nodes"].size(); ++i)
		{
			traverseNode(i, &(m_Model->m_Transform));
		}
	}

	std::vector<unsigned char> glTFLoader::getData()
	{
		// Create a place to store the raw text, and get the uri of the .bin m_File
		std::string bytesText;
		std::string uri = JSON["buffers"][0]["uri"];

		// Store raw text m_Data into bytesText
		bytesText = File::ReadFileAsString((m_FileDir + uri).c_str());

		// Transform the raw text m_Data into bytes and put them in a vector
		std::vector<unsigned char> m_Data(bytesText.begin(), bytesText.end());
		return m_Data;
	}

	void glTFLoader::traverseNode(unsigned int nextNode, Transform* prevTrans)
	{
		// Current node
		json node = JSON["nodes"][nextNode];

		bool roFlag = false;
		// Get translation if it exists
		glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
		if (node.find("translation") != node.end())
		{
			float transValues[3];
			for (unsigned int i = 0; i < node["translation"].size(); i++)
				transValues[i] = (node["translation"][i]);
			translation = glm::make_vec3(transValues);
		}
		// Get quaternion if it exists
		glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		if (node.find("rotation") != node.end())
		{
			float rotValues[4] =
			{
				node["rotation"][3],
				node["rotation"][0],
				node["rotation"][1],
				node["rotation"][2]
			};
			rotation = glm::make_quat(rotValues);
		}
		// Get scale if it exists
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
		if (node.find("scale") != node.end())
		{
			float scaleValues[3];
			for (unsigned int i = 0; i < node["scale"].size(); i++)
				scaleValues[i] = (node["scale"][i]);
			scale = glm::make_vec3(scaleValues);
		}
		// Get matrix if it exists 
		// [To notice this will not be stored into Transform]
		// [Hard to seperate a Matrix into Rot, Scale, pos]
		// [Maybe set this node to a static node so readonly Transform]
		glm::mat4 matNode = glm::mat4(1.0f);
		if (node.find("matrix") != node.end())
		{
			float matValues[16];
			for (unsigned int i = 0; i < node["matrix"].size(); i++)
				matValues[i] = (node["matrix"][i]);
			matNode = glm::make_mat4(matValues);

			// Initialize matrices
			glm::mat4 trans = glm::mat4(1.0f);
			glm::mat4 rot = glm::mat4(1.0f);
			glm::mat4 sca = glm::mat4(1.0f);

			// Use translation, rotation, and scale to change the initialized matrices
			trans = glm::translate(trans, translation);
			rot = glm::mat4_cast(rotation);
			sca = glm::scale(sca, scale);

			// Multiply all matrices together
			matNode = matNode * trans * rot * sca;

			roFlag = true;
		}

		// root Node
		Transform* curTrans = nullptr;
		if (roFlag) {
			curTrans = new Transform(matNode, prevTrans);
		}
		else {
			curTrans = new Transform(
				translation, 
				rotation, 
				scale, 
				prevTrans
			);
		}
		
		// Check if the node contains a mesh and if it does load it
		if (node.find("mesh") != node.end())
			loadMesh(node["mesh"], curTrans);

		// Check if the node has children, and if it does, apply this function to them with the matNextNode
		if (node.find("children") != node.end())
		{
			for (unsigned int i = 0; i < node["children"].size(); i++)
				traverseNode(node["children"][i], curTrans);
		}
	}

	void glTFLoader::loadMesh(unsigned int indMesh, Transform* curTrans)
	{
		const json& mesh = JSON["meshes"][indMesh];
		std::string name = mesh["name"];
		std::shared_ptr<Mesh> myMesh;

		// print out the mesh name
		std::cout << "Load Mesh: " << name << std::endl;

		if (m_Model->m_MeshPool.find(name) != m_Model->m_MeshPool.end()) {
			// [TODO]: Mesh already loaded not going to render Here might be 
			// wrong, since node can share same mesh but have different trasnfrom.
			// I will keep it like this for now.
			return;
		}
		else {
			myMesh = std::make_shared<Mesh>(name, curTrans);
			m_Model->m_MeshPool[name] = myMesh;
			m_Model->m_Meshes.push_back(myMesh);
		}

		// Loading SubMeshes that belongs to single mesh, normally only one SubMesh
		for (const auto& primitive : mesh["primitives"])
		{
			unsigned int posAccInd = primitive["attributes"]["POSITION"];
			unsigned int normalAccInd = primitive["attributes"]["NORMAL"];
			unsigned int texAccInd = -1; // Default value if TEXCOORD_0 doesn't exist
			if (primitive["attributes"].contains("TEXCOORD_0"))
			{
				texAccInd = primitive["attributes"]["TEXCOORD_0"];
			}

			unsigned int indAccInd = primitive["indices"];
			unsigned int matInd = primitive["material"];

			// Use accessor indices to get all vertices components
			std::vector<float> posVec = getFloats(JSON["accessors"][posAccInd]);
			std::vector<glm::vec3> positions = groupFloatsVec3(posVec);
			std::vector<float> normalVec = getFloats(JSON["accessors"][normalAccInd]);
			std::vector<glm::vec3> normals = groupFloatsVec3(normalVec);
			
			std::vector<glm::vec2> texUVs;
			if (texAccInd == -1) {
				texUVs = std::vector<glm::vec2>(positions.size(), glm::vec2(0.0f, 0.0f));
			}
			else {
				std::vector<float> texVec = getFloats(JSON["accessors"][texAccInd]);
				texUVs = groupFloatsVec2(texVec);
			}
			

			// Combine all the vertex components and also get the indices and textures
			std::vector<Vertex> vertices = assembleVertices(positions, normals, texUVs);
			std::vector<unsigned int> indices = getIndices(JSON["accessors"][indAccInd]);
			
			// [TODO]: Currently just get all textures from tex folder and use the 
			// default PhongMaterial, This will be changed in the future.
			//std::vector<std::shared_ptr<Texture>> textures = getTextures();
			//std::shared_ptr<PhongMaterial> material = std::make_shared<PhongMaterial>(textures);
			std::shared_ptr<Material> material = getMaterial(matInd);

			// Combine the vertices, indices, and textures into a mesh
			std::shared_ptr<SubMesh> subMesh = std::make_shared<SubMesh>(vertices, indices, material);
			myMesh->m_SubMeshes.push_back(subMesh);
		}
	}

	std::vector<float> glTFLoader::getFloats(json accessor)
	{
		std::vector<float> floatVec;

		// Get properties from the accessor
		unsigned int buffViewInd = accessor.value("bufferView", 1);
		unsigned int count = accessor["count"];
		unsigned int accByteOffset = accessor.value("byteOffset", 0);
		std::string type = accessor["type"];

		// Get properties from the bufferView
		json bufferView = JSON["bufferViews"][buffViewInd];
		unsigned int byteOffset = bufferView["byteOffset"];

		// Interpret the type and store it into numPerVert
		unsigned int numPerVert;
		if (type == "SCALAR") numPerVert = 1;
		else if (type == "VEC2") numPerVert = 2;
		else if (type == "VEC3") numPerVert = 3;
		else if (type == "VEC4") numPerVert = 4;
		else throw std::invalid_argument("Type is invalid (not SCALAR, VEC2, VEC3, or VEC4)");

		// Go over all the bytes in the m_Data at the correct place using the properties from above
		unsigned int beginningOfData = byteOffset + accByteOffset;
		unsigned int lengthOfData = count * 4 * numPerVert;
		for (unsigned int i = beginningOfData; i < beginningOfData + lengthOfData; i)
		{
			unsigned char bytes[] = { m_Data[i++], m_Data[i++], m_Data[i++], m_Data[i++] };
			float value;
			std::memcpy(&value, bytes, sizeof(float));
			floatVec.push_back(value);
		}

		return floatVec;
	}

	std::vector<unsigned int> glTFLoader::getIndices(json accessor)
	{
		std::vector<unsigned int> indices;

		// Get properties from the accessor
		unsigned int buffViewInd = accessor.value("bufferView", 0);
		unsigned int count = accessor["count"];
		unsigned int accByteOffset = accessor.value("byteOffset", 0);
		unsigned int componentType = accessor["componentType"];

		// Get properties from the bufferView
		json bufferView = JSON["bufferViews"][buffViewInd];
		unsigned int byteOffset = bufferView["byteOffset"];

		// Get indices with regards to their type: unsigned int, unsigned short, or short
		unsigned int beginningOfData = byteOffset + accByteOffset;
		if (componentType == 5125)
		{
			for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 4; i)
			{
				unsigned char bytes[] = { m_Data[i++], m_Data[i++], m_Data[i++], m_Data[i++] };
				unsigned int value;
				std::memcpy(&value, bytes, sizeof(unsigned int));
				indices.push_back((unsigned int)value);
			}
		}
		else if (componentType == 5123)
		{
			for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 2; i)
			{
				unsigned char bytes[] = { m_Data[i++], m_Data[i++] };
				unsigned short value;
				std::memcpy(&value, bytes, sizeof(unsigned short));
				indices.push_back((unsigned int)value);
			}
		}
		else if (componentType == 5122)
		{
			for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 2; i)
			{
				unsigned char bytes[] = { m_Data[i++], m_Data[i++] };
				short value;
				std::memcpy(&value, bytes, sizeof(short));
				indices.push_back((unsigned int)value);
			}
		}

		return indices;
	}

	std::shared_ptr<Texture> glTFLoader::getTextures(unsigned int indTex)
	{
		if (indTex >= JSON["images"].size()) {
			std::cout << "Texture index out of range" << std::endl;
			assert(false);
		}

		std::string texPath = JSON["images"][indTex]["uri"];
		if (m_Model->m_TexturePool[texPath] != nullptr)
			return m_Model->m_TexturePool[texPath];
		
		else {
			Texture::TextureType type;
			if (texPath.find("baseColor") != std::string::npos)
				type = Texture::TextureType::DIFFUSE;
			else if (texPath.find("metallicRoughness") != std::string::npos)
				type = Texture::TextureType::SPECULAR;

			std::shared_ptr<Texture> tex_ptr = std::make_shared<Texture>(
				(m_FileDir + texPath).c_str(), type
			);
			m_Model->m_TexturePool[texPath] = tex_ptr;

			return tex_ptr;
		}
	}

	std::shared_ptr<Material> glTFLoader::getMaterial(unsigned int indMat)
	{
		// Get the material
		json material = JSON["materials"][indMat];

		// Currently only supports PBR materials
		if(material.find("pbrMetallicRoughness") == material.end())
			throw std::invalid_argument("Material is not PBR");

		if(m_Model->m_MaterialPool.find(material["name"]) != m_Model->m_MaterialPool.end())
			return m_Model->m_MaterialPool[material["name"]];

		// Load the PBR material
		bool doubleSided = material.value("doubleSided", false);
		glm::vec3 emissiveFactor(0.0f);
		if (material.contains("emissiveFactor")) {
			auto emissiveArray = material["emissiveFactor"];
			if (emissiveArray.is_array() && emissiveArray.size() == 3) {
				emissiveFactor = glm::vec3(emissiveArray[0], emissiveArray[1], emissiveArray[2]);
			}
		}
		json pbr = material["pbrMetallicRoughness"];
		// Load base color factor
		glm::vec4 baseColorFactor(1.0f);
		if (pbr.contains("baseColorFactor")) {
			auto baseColorFactorArray = pbr["baseColorFactor"];
			baseColorFactor = glm::vec4(
				baseColorFactorArray[0], 
				baseColorFactorArray[1], 
				baseColorFactorArray[2], 
				baseColorFactorArray[3]
			);
		}

		// Load base color texture index and texCoord
		std::shared_ptr<Texture> baseColorTexture = nullptr;
		if (pbr.contains("baseColorTexture")) {
			auto jbaseColorTexture = pbr["baseColorTexture"];
			int baseColorTextureIndex = jbaseColorTexture["index"];
			// not sure what this is for
			// int baseColorTexCoord = jbaseColorTexture["texCoord"];

			// Get the texture
			baseColorTexture = getTextures(baseColorTextureIndex);
		}

		// Load metallic factor
		float metallicFactor = pbr.value("metallicFactor", 0.0f);

		// Load roughness factor
		float roughnessFactor = pbr.value("roughnessFactor", 1.0f);

		// Create and return the PBR material
		std::shared_ptr<PBRMaterial> pbrMaterial = std::make_shared<PBRMaterial>(
			emissiveFactor, 
			material["name"], 
			doubleSided, 
			baseColorFactor, 
			baseColorTexture,
			metallicFactor, 
			roughnessFactor
		);
		m_Model->m_MaterialPool[material["name"]] = pbrMaterial;
		return pbrMaterial;
	}
	
	
	std::vector<Vertex> glTFLoader::assembleVertices (
		std::vector<glm::vec3> positions,
		std::vector<glm::vec3> normals,
		std::vector<glm::vec2> texUVs
	)
	{
		std::vector<Vertex> vertices;
		for (int i = 0; i < positions.size(); i++)
		{
			vertices.push_back
			(
				Vertex
				{
					positions[i],
					normals[i],
					texUVs[i]
				}
			);
		}
		return vertices;
	}


	std::vector<glm::vec2> glTFLoader::groupFloatsVec2(std::vector<float> floatVec)
	{
		std::vector<glm::vec2> vectors;
		for (int i = 0; i < floatVec.size(); i)
		{
			vectors.push_back(glm::vec2(floatVec[i++], floatVec[i++]));
		}
		return vectors;
	}
	
	std::vector<glm::vec3> glTFLoader::groupFloatsVec3(std::vector<float> floatVec)
	{
		std::vector<glm::vec3> vectors;
		for (int i = 0; i < floatVec.size(); i)
		{
			vectors.push_back(glm::vec3(floatVec[i++], floatVec[i++], floatVec[i++]));
		}
		return vectors;
	}
	
	std::vector<glm::vec4> glTFLoader::groupFloatsVec4(std::vector<float> floatVec)
	{
		std::vector<glm::vec4> vectors;
		for (int i = 0; i < floatVec.size(); i)
		{
			vectors.push_back(glm::vec4(floatVec[i++], floatVec[i++], floatVec[i++], floatVec[i++]));
		}
		return vectors;
	}
}