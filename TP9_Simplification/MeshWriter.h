#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include "Mesh.h"
#include "Vec3.h"

/*
* namespace contenant les methodes d'exports de Mesh (singleton)
*/
namespace MeshWriter
{
	std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) 
	{
		std::stringstream ss(s);
		std::string item;

		while (std::getline(ss, item, delim)) 
		{
			elems.push_back(item);
		}

		return elems;
	}


	std::vector<std::string> split(const std::string &s, char delim) 
	{
		std::vector<std::string> elems;

		split(s, delim, elems);

		return elems;
	}

	/*
	* export d'un objet Mesh en format .off
	* obj : objet Mesh a ecrire
	* filename : nom du fichier dans lequel va etre exporte le mesh
	*/
	void exportObj(const Mesh& obj, std::string filename)
	{
		std::vector<Vec3<float>> points = obj.getPoints();
		std::vector<Vec3<unsigned int>> faces = obj.getFaces();
		std::vector<Vec3<float>> textures = obj.getTextures();
		std::vector<Vec3<float>> normales = obj.getNormales();
		std::vector<Vec3<unsigned int>> facesTextures = obj.getFacesTextures();
		std::vector<Vec3<unsigned int>> facesNormales = obj.getFacesNormales();
		std::vector<Vec2<unsigned int>> arretes = obj.getArretes();

		std::ofstream file(filename);

		file << "OFF" << std::endl;
		file << points.size() << " " << faces.size() << " " << arretes.size() << std::endl;

		for (unsigned int i = 0; i < points.size(); i++)
			file << points[i].x << " " << points[i].y << " " << points[i].z << std::endl;
		for (unsigned int i = 0; i < faces.size(); i++)
			file << "3 " << faces[i].x << " " << faces[i].y << " " << faces[i].z << std::endl;

		file.close();
	}

	void addArrete(const Vec2<unsigned int> &a, std::vector<Vec2<unsigned int>> &v)
	{
		if (std::find(v.begin(), v.end(), a) == v.end())
			v.push_back(a);
	}

	Mesh importObj(std::string filename)
	{
		std::vector<Vec3<float>> points = std::vector<Vec3<float>>();
		std::vector<Vec3<unsigned int>> faces = std::vector<Vec3<unsigned int>>();
		std::vector<Vec3<float>> textures = std::vector<Vec3<float>>();
		std::vector<Vec3<float>> normales = std::vector<Vec3<float>>();
		std::vector<Vec3<unsigned int>> facesTextures = std::vector<Vec3<unsigned int>>();
		std::vector<Vec3<unsigned int>> facesNormales = std::vector<Vec3<unsigned int>>();
		std::vector<Vec2<unsigned int>> arretes = std::vector<Vec2<unsigned int>>();

		const int debut = 2;
		int line_nb = 1;
		int nb_sommets, nb_faces, nb_arretes;
		unsigned int x, y, z;
		int n; // Nombre de sommets pour une facette
		std::string line;
		std::ifstream file(filename);

		if (file.is_open())
		{
			textures.push_back(Vec3<float>(0., 51., 204.));

			while (getline(file, line))
			{
				if (line_nb == debut)
				{
					std::vector<std::string> s = split(line, ' ');
					nb_sommets = std::stoi(s[0]);
					nb_faces = std::stoi(s[1]);
					nb_arretes = std::stoi(s[2]);
				}
				else if (line_nb > debut)
				{
					if (line_nb <= debut + nb_sommets) // Points
					{
						std::vector<std::string> s = split(line, ' ');
						points.push_back(Vec3<float>(std::stof(s[0]), std::stof(s[1]), std::stof(s[2])));

					}
					else if (line_nb <= debut + nb_sommets + nb_faces) // Faces
					{
						std::vector<std::string> s = split(line, ' ');
						n = std::stoi(s[0]);
						if (n == 3) // Facette triangulaire
						{
							x = std::stoi(s[1]);
							y = std::stoi(s[2]);
							z = std::stoi(s[3]);

							faces.push_back(Vec3<unsigned int>(x, y, z));

							facesTextures.push_back(Vec3<unsigned int>(0, 0, 0));

							addArrete(Vec2<unsigned int>(x, y), arretes);
							addArrete(Vec2<unsigned int>(y, z), arretes);
							addArrete(Vec2<unsigned int>(z, x), arretes);
						}
					}
				}
				line_nb++;
			}
			file.close();
		}

		return Mesh(points, faces, facesTextures, facesNormales, textures, normales, arretes);
	}
};

