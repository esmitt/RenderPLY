#include "glm/glm.hpp"
#pragma once
namespace
{
	const int X = 0;
	const int Y = 0;
	const int Z = 0;
};

class CBoundingBox
{
private:
	glm::vec3 m_pMin, m_pMax;

public:
	///Default constructor
	CBoundingBox();

	///Default destructor
	~CBoundingBox();

	///Get the distance on the i-axis
	float getLength(int iAxis);

	///Add a point to create the bounding box
	void addPoint(glm::vec3 p);

	///Get the center of the bounding box
	glm::vec3 getCenter(){return (m_pMax + m_pMin)/2.0f;}

	float getDiagonal();

	inline void resetPoints(){m_pMin = glm::vec3(std::numeric_limits<float>::max());
	m_pMax = glm::vec3(-999999.99f);}
};