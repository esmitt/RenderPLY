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
	std::vector<Vertex> m_vVertex;
	std::vector<Mesh> m_vMesh;
	unsigned int m_uVBO;
	int m_iNPoints;
	GLuint m_uVAO;		//for the model
public:
	C3DModel();
	~C3DModel();

	///Method to load an 3Dmodel
	bool load(const std::string & sFilename, const CGLSLProgram & program, glm::vec3 pCenterOn = glm::vec3(0));

	///Draw the object using the VAO
	void drawObject();

	///Get the center of the object
	inline glm::vec3 getCenter(){return m_bbox.getCenter();}

	///Get the lenght of the diagonal of bounding box
	inline float getDiagonal(){return m_bbox.getDiagonal();}

	void deleteBuffers();
};
#endif
