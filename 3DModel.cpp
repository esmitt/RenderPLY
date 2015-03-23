#include "GL/glew.h"
#include "3DModel.h"
#include <iostream>
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "OGLBasic.h"

//#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#define BUFFER_OFFSET(i) (reinterpret_cast<void*>(i))

///
/// default constructor
///
C3DModel::C3DModel()
{
	m_uVAO = -1;
	m_uVBO = -1;
	m_iNPoints = -1;
	m_vMesh.clear();
	m_vVertex.clear();
}

///
/// default destructor
///
C3DModel::~C3DModel()
{
	m_vMesh.clear();
	m_vVertex.clear();
	deleteBuffers();
	TRACE("model unloaded\n");
}

void C3DModel::deleteBuffers()
{
	if (m_uVAO != 0)	glDeleteBuffers(1, &m_uVAO);
	if (m_uVBO != 0)	glDeleteBuffers(1, &m_uVBO);
}

///
/// Function to load a 3D object file
///
/// @param sFilename the filename of the 3d object
///
/// @return true if it is load correctly, false otherwise
///
bool C3DModel::load(const std::string & sFilename, const CGLSLProgram & program, glm::vec3 pCenterOn)
{

+	TRACE("loading the file %s\n", sFilename.c_str());
	
	//creating the VAO for the model
	glGenVertexArrays(1, &m_uVAO);
	glBindVertexArray(m_uVAO);

		//creating the VBO
		glGenBuffers(1, &m_uVBO);

		glBindBuffer(GL_ARRAY_BUFFER, m_uVBO);
		glBufferData(GL_ARRAY_BUFFER, m_vVertex.size() * sizeof(Vertex), &m_vVertex[0], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(0)); //Vertex
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(GL_FLOAT) * 4)); //Normals
		glEnableVertexAttribArray(1);

	glBindVertexArray(0);	//VAO

	m_iNPoints = m_vVertex.size();

	m_vVertex.clear();
	return true;
}

///
/// Method to draw the object
///
void C3DModel::drawObject()
{
	glBindVertexArray(m_uVAO);
	//glDrawRangeElements(GL_TRIANGLES,0,	m_iIndexSize, m_vMesh[k].iCount, GL_UNSIGNED_INT,BUFFER_OFFSET(sizeof(GL_UNSIGNED_INT)*m_vMesh[k].iFirst));
	//glDrawElementsBaseVertex(GL_TRIANGLES, m_vMesh[k].iCount, GL_UNSIGNED_INT, NULL, m_vMesh[k].iFirst);
	glBindVertexArray(0);
}