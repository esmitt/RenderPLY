#ifndef __3DMODEL_H__
#define __3DMODEL_H__

#include "BoundingBox.h"
#include <string>
#include <vector>
#include "GLSLProgram.h"
#include "rply.h"

struct Vertex
{
	float x;
	float y;
	float z;
};
struct Mesh
{
	unsigned int id0;
	unsigned int id1;
	unsigned int id2;
};

///
/// Class C3DModel.
///
/// A class for loading and displaying and object using Open Asset Import Librery
///
///
class C3DModel
{
private:
	CBoundingBox m_bbox;
	static std::vector<Vertex> m_vVertex;
	static std::vector<Mesh> m_vMesh;
	unsigned int m_uVBO;
	unsigned int m_uVBOIndex;
	int m_iNPoints;
	int m_iNTriangles;
	GLuint m_uVAO;		//for the model
	static Vertex m_tempVertex;
	static Mesh m_tempMesh;
	static int m_iTempIndex;
protected:
	static int vertex_cb(p_ply_argument argument);
	static int face_cb(p_ply_argument argument);
	static std::vector<Vertex> Helper(){ std::vector<Vertex> a; a.reserve(1); return a; }
public:
	C3DModel();
	~C3DModel();

	///Method to load an 3Dmodel
	bool load(const std::string & sFilename);

	//delete all buffers
	void deleteBuffers();

	///Draw the object using the VAO
	void drawObject();

	///Get the center of the object
	inline glm::vec3 getCenter(){return m_bbox.getCenter();}

	///Get the lenght of the diagonal of bounding box
	inline float getDiagonal(){return m_bbox.getDiagonal();}
};
#endif
