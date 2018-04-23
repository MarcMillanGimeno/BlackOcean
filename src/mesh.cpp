#include "mesh.h"
#include <cassert>
#include "includes.h"
#include "shader.h"
#include "extra/textparser.h"

using namespace std;

Mesh::Mesh()
{
	vertices_vbo_id = 0;
	normals_vbo_id = 0;
	uvs_vbo_id = 0;
	colors_vbo_id = 0;
    indices_vbo_id = 0;
    vao = 0;
}

Mesh::Mesh( const Mesh& m )
{
	vertices = m.vertices;
	normals = m.normals;
	uvs = m.uvs;
	colors = m.colors;
    indices = m.indices;
    vao = m.vao;
}

Mesh::~Mesh()
{
	if(vertices_vbo_id) glDeleteBuffers(1, &vertices_vbo_id);
	if(normals_vbo_id) glDeleteBuffers(1, &normals_vbo_id);
	if(uvs_vbo_id) glDeleteBuffers(1, &uvs_vbo_id);
	if(colors_vbo_id) glDeleteBuffers(1, &colors_vbo_id);
    if(colors_vbo_id) glDeleteBuffers(1, &indices_vbo_id);
    if(vao)glDeleteVertexArrays(1, &vao);
}

void Mesh::clear()
{
	vertices.clear();
	normals.clear();
	uvs.clear();
	colors.clear();
    indices.clear();
}

void Mesh::genBuffers() {
    assert(vertices.size() && "No vertices in this mesh");
    

    //delete old
    if(vertices_vbo_id) glDeleteBuffers(1, &vertices_vbo_id);
    if(normals_vbo_id) glDeleteBuffers(1, &normals_vbo_id);
    if(uvs_vbo_id) glDeleteBuffers(1, &uvs_vbo_id);
    if(colors_vbo_id) glDeleteBuffers(1, &colors_vbo_id);
    
    //bind vao to store state
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    glGenBuffers( 1, &vertices_vbo_id ); //generate one handler (id)
    glBindBuffer( GL_ARRAY_BUFFER, vertices_vbo_id ); //bind the handler
    glBufferData( GL_ARRAY_BUFFER, vertices.size() * 3 * sizeof(float), &vertices[0], GL_STATIC_DRAW ); //upload data
    
    //shader must a_vertex attribute, error if not
    glEnableVertexAttribArray(VERTEX_ATTRIBUTE_LOCATION);
    glVertexAttribPointer(VERTEX_ATTRIBUTE_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, NULL );

    
    if(normals.size())
    {
        glGenBuffers( 1, &normals_vbo_id); //generate one handler (id)
        glBindBuffer( GL_ARRAY_BUFFER, normals_vbo_id ); //bind the handler
        glBufferData( GL_ARRAY_BUFFER, normals.size() * 3 * sizeof(float), &normals[0], GL_STATIC_DRAW ); //upload data
        
        //shader must have a_normal attribute
        glEnableVertexAttribArray(NORMAL_ATTRIBUTE_LOCATION);
        glVertexAttribPointer(NORMAL_ATTRIBUTE_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, NULL );
        assert( glGetError() == GL_NO_ERROR );

    }
    
    if(uvs.size())
    {
        glGenBuffers( 1, &uvs_vbo_id); //generate one handler (id)
        glBindBuffer( GL_ARRAY_BUFFER, uvs_vbo_id ); //bind the handler
        glBufferData( GL_ARRAY_BUFFER, uvs.size() * 2 * sizeof(float), &uvs[0], GL_STATIC_DRAW ); //upload data
        
        //shader must have a_uv attribute
        glEnableVertexAttribArray(UV_ATTRIBUTE_LOCATION);
        glVertexAttribPointer(UV_ATTRIBUTE_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, NULL );
        assert( glGetError() == GL_NO_ERROR );
        
    }
    
    if(colors.size())
    {
        glGenBuffers( 1, &colors_vbo_id); //generate one handler (id)
        glBindBuffer( GL_ARRAY_BUFFER, colors_vbo_id ); //bind the handler
        glBufferData( GL_ARRAY_BUFFER, uvs.size() * 4 * sizeof(float), &colors[0], GL_STATIC_DRAW ); //upload data

        //shader must have a_color attribute
        glEnableVertexAttribArray(COLOR_ATTRIBUTE_LOCATION);
        glVertexAttribPointer(COLOR_ATTRIBUTE_LOCATION, 4, GL_FLOAT, GL_FALSE, 0, NULL );
        assert( glGetError() == GL_NO_ERROR );
        
    }
    
    if (indices.size() == 0){
        //if there is no index buffer, make an 'emergency' serial one here
        for (size_t i = 0; i < vertices.size(); i++) {
            indices.push_back((unsigned int)i);
        }
    }
    
    //create index array
    glGenBuffers(1, &indices_vbo_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_vbo_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    
    glBindVertexArray(0); //unbind VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind buffer
    
}

void Mesh::render( int primitive, Shader* sh )
{
    
    glBindVertexArray(vao);
    
    glDrawElements(primitive, (unsigned int)indices.size(), GL_UNSIGNED_INT, 0);
    
    glBindVertexArray(0);
    
}



void Mesh::createPlane(float size)
{
	vertices.clear();
	normals.clear();
	uvs.clear();
	colors.clear();
	indices.clear();

	//create four vertices for corner of quad

	vertices.push_back( Vector3(size,0,size) );
	vertices.push_back( Vector3(-size, 0, size));
	vertices.push_back( Vector3(-size,0,-size) );
	vertices.push_back( Vector3(size,0,-size) );
	

	//all of them have the same normal
	normals.push_back( Vector3(0,1,0) );
	normals.push_back( Vector3(0,1,0) );
	normals.push_back( Vector3(0,1,0) );
	normals.push_back( Vector3(0,1,0) );


	//texture coordinates
	uvs.push_back( Vector2(1,1) );
	uvs.push_back( Vector2(0,1) );
	uvs.push_back( Vector2(0,0) );
	uvs.push_back( Vector2(1,0) );
    
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(1);
	indices.push_back(0);
	indices.push_back(3);
	indices.push_back(2);


    
    this->genBuffers();
}
/*
bool Mesh::createBulletsMesh() {


}
*/
//Añadido en clase (mesh.cpp)
bool Mesh::loadASE(const char* filename){
	vertices.clear();
	normals.clear();
	uvs.clear();
	colors.clear();
	indices.clear();


	TextParser t;

	if(t.create (filename) == false){
		cout << "Couldn't open file: " << filename << endl;
		return false;
	}

	t.seek("*MESH_NUMVERTEX");
	int num_vertices = t.getint();
	
	//cout << "Num vertices: " <<num_vertices << endl;
	
	t.seek("*MESH_NUMFACES");
	int num_faces = t.getint();

	//cout << "Num faces: " << num_faces << endl;

	vertices.resize(num_faces * 3);
	normals.resize(num_faces * 3);
	uvs.resize(num_faces * 3);

	//Vertices
	vector<Vector3> unique_verts;

	unique_verts.resize(num_vertices);


	for (int i = 0; i<num_vertices; i++){
		t.seek("*MESH_VERTEX");
		int new_index = t.getint();
		float new_x = (float)t.getfloat();
		float new_y = (float)t.getfloat();
		float new_z = (float)t.getfloat();

		Vector3 new_vec3 (-new_x, new_z, new_y);

		unique_verts[new_index] = new_vec3;
	}


	//cout << "Vertices cargados correctamente"<< endl;

	//CARAS 
	for (int i = 0; i < num_faces; i++){

		t.seek("*MESH_FACE");

		t.seek("A:");
		int vert1 = t.getint();

		t.seek("B:");
		int vert2 = t.getint();

		t.seek("C:");
		int vert3 = t.getint();

		vertices[i * 3 + 0] = unique_verts[vert1];
		vertices[i * 3 + 1] = unique_verts[vert2];
		vertices[i * 3 + 2] = unique_verts[vert3];
	}


	vector<Vector2> unique_uvs;
	t.seek("*MESH_NUMTVERTEX");
	int num_tvertex = t.getint();
	unique_uvs.resize(num_tvertex);
	for (int i = 0; i < num_tvertex; i++) {
		t.seek("*MESH_TVERT");
		int aux = t.getint();
		float new_x = (float)t.getfloat();
		float new_y = (float)t.getfloat();
		unique_uvs[aux].set(new_x, new_y);
	}
	
	for (int i = 0; i < num_faces; i++) {
		t.seek("*MESH_TFACE");
		int index = t.getint();
		int new_1 = t.getint();
		int new_2 = t.getint();
		int new_3 = t.getint();
		uvs[i * 3 + 0] = unique_uvs[new_1];
		uvs[i * 3 + 1] = unique_uvs[new_2];
		uvs[i * 3 + 2] = unique_uvs[new_3];
	}

	//NORMALES
	for (int i = 0; i < num_faces; i++) {
		for (int j = 0; j < 3; j++) {
			t.seek("*MESH_VERTEXNORMAL");
			int aux = t.getint();
			float new_x = (float)t.getfloat();
			float new_y = (float)t.getfloat();
			float new_z = (float)t.getfloat();
			Vector3 new_vec3(-new_x, new_z, new_y);
			normals[i * 3 + j] = new_vec3;
		}
		
	}

	this->genBuffers();

	return true;
}

void Mesh::setAABB() {
	Vector3 min = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 max = Vector3(0.0f, 0.0f, 0.0f);
	aabb.halfsize = Vector3(0.0f, 0.0f, 0.0f);
	
	for (int i = 0; i < vertices.size(); i++) {
		if (max.x < vertices[i].x) {
			max.x = vertices[i].x;
		} 

		if (max.y < vertices[i].y) {
			max.y = vertices[i].y;
		}

		if (max.z < vertices[i].z) {
			max.z = vertices[i].z;
		}

		if (min.x > vertices[i].x) {
			min.x = vertices[i].x;
		}

		if (min.y > vertices[i].y) {
			min.y = vertices[i].y;
		}

		if (min.z > vertices[i].z) {
			min.z = vertices[i].z;
		}
	}

	aabb.center = Vector3((max.x + min.x) / 2, (max.y + min.y) / 2, (max.z + min.z) / 2);
	
	if (max.x < 0) max.x = -max.x;
	if (max.y < 0) max.x = -max.y;
	if (max.z < 0) max.x = -max.z;
	if (min.x < 0) max.x = -min.x;
	if (min.y < 0) max.x = -min.y;
	if (min.z < 0) max.x = -min.z;

	float distancia; 

	distancia = max.x - aabb.center.x;
	if (distancia > aabb.halfsize.x) {
		aabb.halfsize.x = distancia;
	}

	distancia = min.x - aabb.center.x;
	if (distancia > aabb.halfsize.x) {
		aabb.halfsize.x = distancia;
	}

	distancia = max.y - aabb.center.y;
	if (distancia > aabb.halfsize.y) {
		aabb.halfsize.y = distancia;
	}

	distancia = min.y - aabb.center.y;
	if (distancia > aabb.halfsize.y) {
		aabb.halfsize.y = distancia;
	}

	distancia = max.z - aabb.center.z;
	if (distancia > aabb.halfsize.z) {
		aabb.halfsize.z = distancia;
	}

	distancia = min.z - aabb.center.z;
	if (distancia > aabb.halfsize.z) {
		aabb.halfsize.z = distancia;
	}
	
	//std::cout << "HALFSIZE: " << aabb.halfsize.x <<aabb.halfsize.y << aabb.halfsize.z << std::endl; 
}

void Mesh::createAABBGeometry() {
	std::vector<float> line_verts;
	line_verts.resize(24);
	//0
	line_verts[0] = aabb.center.x - aabb.halfsize.x;
	line_verts[1] = aabb.center.y + aabb.halfsize.y;
	line_verts[2] = aabb.center.z + aabb.halfsize.z;
	//1
	line_verts[3] = aabb.center.x - aabb.halfsize.x;
	line_verts[4] = aabb.center.y - aabb.halfsize.y;
	line_verts[5] = aabb.center.z + aabb.halfsize.z;
	//2
	line_verts[6] = aabb.center.x + aabb.halfsize.x;
	line_verts[7] = aabb.center.y - aabb.halfsize.y;
	line_verts[8] = aabb.center.z + aabb.halfsize.z;
	//3
	line_verts[9] = aabb.center.x + aabb.halfsize.x;
	line_verts[10] = aabb.center.y + aabb.halfsize.y;
	line_verts[11] = aabb.center.z + aabb.halfsize.z;
	//4
	line_verts[12] = aabb.center.x - aabb.halfsize.x;
	line_verts[13] = aabb.center.y + aabb.halfsize.y;
	line_verts[14] = aabb.center.z - aabb.halfsize.z;
	//5
	line_verts[15] = aabb.center.x - aabb.halfsize.x;
	line_verts[16] = aabb.center.y - aabb.halfsize.y;
	line_verts[17] = aabb.center.z - aabb.halfsize.z;
	//6
	line_verts[18] = aabb.center.x + aabb.halfsize.x;
	line_verts[19] = aabb.center.y - aabb.halfsize.y;
	line_verts[20] = aabb.center.z - aabb.halfsize.z;
	//7
	line_verts[21] = aabb.center.x + aabb.halfsize.x;
	line_verts[22] = aabb.center.y + aabb.halfsize.y;
	line_verts[23] = aabb.center.z - aabb.halfsize.z;


	//bind vao to store state
	glGenVertexArrays(1, &aabb.vao);
	glBindVertexArray(aabb.vao);

	GLuint aabb_vertices_vbo_id;
	glGenBuffers(1, &aabb_vertices_vbo_id); //generate one handler (id)
	glBindBuffer(GL_ARRAY_BUFFER, aabb_vertices_vbo_id); //bind the handler
	glBufferData(GL_ARRAY_BUFFER, line_verts.size()  * sizeof(float), &line_verts[0], GL_STATIC_DRAW); //upload data

																									   //shader must a_vertex attribute, error if not
	glEnableVertexAttribArray(VERTEX_ATTRIBUTE_LOCATION);
	glVertexAttribPointer(VERTEX_ATTRIBUTE_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	std::vector<unsigned int> aabb_line_indices;
	aabb_line_indices.resize(24);
	aabb_line_indices[0] = 0;
	aabb_line_indices[1] = 1;
	aabb_line_indices[2] = 1;
	aabb_line_indices[3] = 2;
	aabb_line_indices[4] = 2;
	aabb_line_indices[5] = 3;
	aabb_line_indices[6] = 3;
	aabb_line_indices[7] = 0;
	aabb_line_indices[8] = 4;
	aabb_line_indices[9] = 5;
	aabb_line_indices[10] = 5;
	aabb_line_indices[11] = 6;
	aabb_line_indices[12] = 6;
	aabb_line_indices[13] = 7;
	aabb_line_indices[14] = 7;
	aabb_line_indices[15] = 4;
	aabb_line_indices[16] = 4;
	aabb_line_indices[17] = 0;
	aabb_line_indices[18] = 7;
	aabb_line_indices[19] = 3;
	aabb_line_indices[20] = 5;
	aabb_line_indices[21] = 1;
	aabb_line_indices[22] = 6;
	aabb_line_indices[23] = 2;

	//create index array
	GLuint aabb_indices_vbo_id;
	glGenBuffers(1, &aabb_indices_vbo_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, aabb_indices_vbo_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, aabb_line_indices.size() * sizeof(unsigned int), &aabb_line_indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0); //unbind VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind buffer
}

void Mesh::setLine(Vector3 direction) {
	std::vector<float> line;
	line.resize(3);
	float size = 250;

	line[0] = aabb.center.x - direction.x * size;
	std::cout << direction.x << std::endl;
	line[1] = aabb.center.y - direction.y * size;
	std::cout << direction.y << std::endl;
	line[2] = aabb.center.z - direction.z * size;
	std::cout << direction.z << std::endl;

	glGenVertexArrays(1, &vao2);
	glBindVertexArray(vao2);

	GLuint aabb_vertices_vbo_id;
	glGenBuffers(1, &aabb_vertices_vbo_id); //generate one handler (id)
	glBindBuffer(GL_ARRAY_BUFFER, aabb_vertices_vbo_id); //bind the handler
	glBufferData(GL_ARRAY_BUFFER, line.size()  * sizeof(float), &line[0], GL_STATIC_DRAW); //upload data

																									   //shader must a_vertex attribute, error if not
	glEnableVertexAttribArray(VERTEX_ATTRIBUTE_LOCATION);
	glVertexAttribPointer(VERTEX_ATTRIBUTE_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	std::vector<unsigned int> line_indices;
	line_indices.resize(2);
	line_indices[0] = 0; 
	line_indices[1] = 1;

	//create index array
	GLuint aabb_indices_vbo_id;
	glGenBuffers(1, &aabb_indices_vbo_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, aabb_indices_vbo_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, line_indices.size() * sizeof(unsigned int), &line_indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0); //unbind VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind buffer
}

void Mesh::renderAABB(Shader* sh) {
	glBindVertexArray(aabb.vao);
	glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::renderLine(Shader* sh) {
	glBindVertexArray(vao2);
	glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::setCollisionModel() {
	collision_model = newCollisionModel3D();

	collision_model->setTriangleNumber(indices.size() / 3);
	
	for (size_t i = 0; i < indices.size() / 3; i++){
		Vector3 v1 = vertices[indices[i * 3]];
		Vector3 v2 = vertices[indices[i * 3 + 1]];
		Vector3 v3 = vertices[indices[i * 3 + 2]];

		collision_model->addTriangle(
			v1.x, v1.y, v1.z,
			v2.x, v2.y, v2.z,
			v3.x, v3.y, v3.z);
	}

	collision_model->finalize();
}

