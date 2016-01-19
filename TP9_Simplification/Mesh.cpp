#include "Mesh.h"


Mesh::Mesh() : points(std::vector<Vec3<float>>()), faces(std::vector<Vec3<unsigned int>>()){};
Mesh::Mesh(std::vector<Vec3<float>> points_, std::vector<Vec3<unsigned int>> faces_, std::vector<Vec3<unsigned int>> facesTextures_, std::vector<Vec3<unsigned int>> facesNormales_, std::vector<Vec3<float>> textures_, std::vector<Vec3<float>> normales_, std::vector<Vec2<unsigned int>> arretes_)
	: points(points_), faces(faces_), textures(textures_), facesTextures(facesTextures_), facesNormalesIndex(facesNormales_), normales(normales_), arretes(arretes_)
{
	srand(time(NULL));

	calculateArrays();

	calculateNormals();
}

Mesh::Mesh(const Mesh & mesh)
	: points(mesh.points), faces(mesh.faces), textures(mesh.textures), facesTextures(mesh.facesTextures), facesNormales(mesh.facesNormales), normales(mesh.normales), 
	arretes(mesh.arretes), pivot(mesh.pivot), indices(mesh.indices), colors(mesh.colors), vertices(mesh.vertices) { };

void Mesh::calculateNormals() {
	// Calcule des normales par face
	std::cout << "\nCalculate face normals...";
	unsigned int facesCount = faces.size();
	facesNormalesIndex = std::vector<Vec3<unsigned int>>(facesCount);
	facesNormales = std::vector<Vec3<float>>(facesCount);

	for (unsigned int i = 0; i < facesCount; ++i) {
		Vec3<unsigned int> face = faces[i];
		Vec3<float> normal = Vec3<float>::crossProduct(points[face.y] - points[face.x], points[face.z] - points[face.x]).normalized();
		facesNormalesIndex[i] = face;
		facesNormales[i] = normal;
	}

	// Calcule des normales par vertex
	std::cout << "\nCalculate vertex normals...";
	unsigned int verticesCount = points.size();
	normales = std::vector<Vec3<float>>(verticesCount);

	for (unsigned int i = 0; i < verticesCount; ++i) {
		Vec3<float> normal = Vec3<float>(.0f, .0f, .0f);

		for (unsigned int j = 0; j < facesCount; ++j) {
			Vec3<unsigned int> face = faces[j];

			for (unsigned int k = 0; k < verticesCount; ++k) { // Obligé d'avoir des faces triangulaires ...
				if ((k == 0 && face.x == i) ||
					(k == 1 && face.y == i) ||
					(k == 2 && face.z == i)) {
					normal += facesNormales[j];
				}
			}
			normales[i] = normal.normalized();
		}
	}
}

void Mesh::calculateArrays()
{
	float max = 1.;
	colors = std::vector<float>();
	vertices = std::vector<float>();
	indices = std::vector<unsigned int>();

	for (unsigned int i = 0; i < points.size(); ++i)
	{
		max = std::fmaxf(max, std::fmaxf(std::fmaxf(std::abs(points[i].x), std::abs(points[i].y)), std::abs(points[i].z)));
	}

	pivot = Vec3<float>(0.f);
	for (unsigned int i = 0; i < points.size(); ++i)
	{
		points[i] = points[i] / max;
		pivot += points[i];

		colors.push_back(textures[0].x);
		colors.push_back(textures[0].y);
		colors.push_back(textures[0].z);

		vertices.push_back(points[i].x);
		vertices.push_back(points[i].y);
		vertices.push_back(points[i].z);
	}

	pivot = pivot / (float)points.size();

	for (int i = 0; i < faces.size(); i++)
	{
		indices.push_back(faces[i].x);
		indices.push_back(faces[i].y);
		indices.push_back(faces[i].z);
	}
}

std::vector<Vec3<float>> Mesh::getPoints() const
{
	return points;
}
std::vector<Vec3<unsigned int>> Mesh::getFaces() const
{
	return faces;
}

std::vector<Vec3<float>> Mesh::getTextures() const
{
	return textures;
}

std::vector<Vec3<float>> Mesh::getNormales() const
{
	return normales;
}

std::vector<Vec3<unsigned int>> Mesh::getFacesTextures() const
{
	return facesTextures;
}

std::vector<Vec3<unsigned int>> Mesh::getFacesNormales() const
{
	return facesNormalesIndex;
}

Vec3<float> Mesh::getCentreGravite() const
{
	return pivot;
}

std::vector<float> Mesh::getColors() const
{
	return colors;
}

std::vector<float> Mesh::getVertices() const
{
	return vertices;
}


std::vector<unsigned int> Mesh::getIndices() const
{
	return indices;
}

std::vector<Vec2<unsigned int>> Mesh::getArretes() const
{
	return arretes;
}

void Mesh::removeFace()
{
	int nb = (int)((double)(rand() / (double)RAND_MAX) * (int)faces.size() - 1);
	faces.erase(faces.begin() + nb);
	calculateArrays();
	printf("Suppression face %d\n", nb);
}



Mesh::~Mesh()
{
}
