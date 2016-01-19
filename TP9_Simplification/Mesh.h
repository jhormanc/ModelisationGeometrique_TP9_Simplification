#pragma once
#include "Vec3.h"
#include "Vec2.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <time.h>

/*
* classe representant un Mesh
*/
class Mesh
{
	/*
	* Vecteur des coordonnes des points du mesh
	*/
	std::vector<Vec3<float>> points;
	/*
	* Vecteur des faces du mesh, contenant les indices des points
	*/
	std::vector<Vec3<unsigned int>> faces;
	/*
	* Vecteur des faces du mesh, contenant les indices des coordonnees de textures
	*/
	std::vector<Vec3<unsigned int>> facesTextures;
	/*
	* Vecteur des faces du mesh, contenant les indices des coordonnees des normales
	*/
	std::vector<Vec3<unsigned int>> facesNormalesIndex;
	/*
	* Vecteur des coordonnees de textures du mesh
	*/
	std::vector<Vec3<float>> textures;
	/*
	* Vecteur des coordonnees de normales du mesh
	*/
	std::vector<Vec3<float>> normales;

	// Vecteur des coordonnees des arretes
	std::vector<Vec2<unsigned int>> arretes;
	/*
	* Centre de gravite du mesh
	*/
	Vec3<float> pivot;

	std::vector<float> vertices;
	std::vector<float> colors;
	std::vector<unsigned int> indices;
	std::vector<Vec3<float>> facesNormales;
	std::vector<Vec3<float>> verticesNormales;

public:
	Mesh();
	Mesh(std::vector<Vec3<float>> points_, std::vector<Vec3<unsigned int>> faces_, std::vector<Vec3<unsigned int>> facesTextures_, std::vector<Vec3<unsigned int>> facesNormales_, std::vector<Vec3<float>> textures_, std::vector<Vec3<float>> normales_, std::vector<Vec2<unsigned int>> arretes_);
	Mesh(const Mesh & mesh);

	void calculateNormals();
	void calculateArrays();
	void removeFace();

	std::vector<Vec3<float>> getPoints() const;
	std::vector<Vec3<unsigned int>> getFaces() const;
	std::vector<Vec3<float>> getTextures() const;
	std::vector<Vec3<float>> getNormales() const;
	std::vector<Vec3<unsigned int>> getFacesTextures() const;
	std::vector<Vec3<unsigned int>> getFacesNormales() const;
	Vec3<float> getCentreGravite() const;
	std::vector<float> getColors() const;
	std::vector<unsigned int> getIndices() const;
	std::vector<float> getVertices() const;
	std::vector<Vec2<unsigned int>> getArretes() const;

	~Mesh();
};

