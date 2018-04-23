/*  by Alun Evans 2016 LaSalle (alunthomasevasevans@gmail.com)
	based on framework by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	The Mesh contains the info about how to render a mesh and also how to parse it from a file.
*/

#ifndef MESH_H
#define MESH_H

#include <vector>
#include "framework.h"
#include "coldet\coldet.h"

struct AABB{
	Vector3 center;
	Vector3 halfsize;
	unsigned int vao;
};

class Shader;

class Mesh
{
public:
	std::vector< Vector3 > vertices; //here we store the vertices
	std::vector< Vector3 > normals;	 //here we store the normals
	std::vector< Vector2 > uvs;	 //here we store the texture coordinates
	std::vector< Vector4 > colors;	 //here we store colors by vertex
    std::vector< unsigned int > indices; //here we store the indices of the mesh

    unsigned int vao;
	unsigned int vao2;

	unsigned int vertices_vbo_id;
	unsigned int normals_vbo_id;
	unsigned int uvs_vbo_id;
	unsigned int colors_vbo_id;
    unsigned int indices_vbo_id;

	Mesh();
	Mesh( const Mesh& m );
	~Mesh();

	void clear();
	void render(int primitive, Shader* sh);

	void genBuffers(); //create the VAO and VBOs for this mesh

	AABB aabb;
	void createAABBGeometry();
	void setAABB();
	void renderAABB(Shader* sh);
	bool draw = true; 

	void renderLine(Shader* sh);
	void setLine(Vector3 direction); 


	void createPlane( float size );
	void createQuad(float center_x, float center_y, float w, float h, bool flip_uvs = false);
	//bool createBulletsMesh();
	
	CollisionModel3D* collision_model;
	void setCollisionModel();
	
	//Añadido en clase (mesh.h)
	bool loadASE (const char*filename);
};

#endif
