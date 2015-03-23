#pragma once
#include "glm\glm.hpp"
class CArcBallMatrix
{
private:
	glm::mat4 m_Matrix;
	float m_fScale;
	glm::vec3 m_vPan;
public:
	CArcBallMatrix();
	~CArcBallMatrix();
	const glm::mat4 & GetMatrix(){ return m_Matrix; }	//a reference
	void SetMatrix(const glm::mat4 & matrix){ m_Matrix = matrix; }
	void SetScale(float fScale);
	void SetPan(glm::vec3 vPan);
	void SetRotation(const glm::vec4 & vquat);
	void Reset();
};

