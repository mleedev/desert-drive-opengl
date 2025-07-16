#pragma once
#include "Mesh3D.h"
#include "Object3D.h"
#include <unordered_map>
#include <assimp/scene.h>

struct PathHash {
    std::size_t operator()(const std::filesystem::path& p) const {
        return std::hash<std::string>{}(p.string());
    }
};

Mesh3D fromAssimpMesh(const aiMesh* mesh, const aiScene* scene, const std::filesystem::path& modelPath,
	std::unordered_map<std::filesystem::path, Texture>& loadedTextures);
Object3D assimpLoad(const std::string& path, bool flipTextureCoords);
Object3D processAssimpNode(aiNode* node, const aiScene* scene,
	const std::filesystem::path& modelPath,
	std::unordered_map<std::filesystem::path, Texture, PathHash>& textures);
std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName,
	const std::filesystem::path& modelPath,
	std::unordered_map<std::filesystem::path, Texture, PathHash>& loadedTextures);