#include "BoundingBox.h"
#include <limits>

CBoundingBox::CBoundingBox()
{
	m_pMin = glm::vec3(std::numeric_limits<float>::max());
	m_pMax = glm::vec3(-999999.99);
	
}

CBoundingBox::~CBoundingBox()
{

}

///
/// Get the lenght/distance on each axis
///
/// @param iAxis indicates the axis: X, Y or Z
///
float CBoundingBox::getLength(int iAxis)
{
	return std::abs(m_pMax[iAxis] - m_pMin[iAxis]); 
}

///
/// Get the lenght/distance on each axis
///
float CBoundingBox::getDiagonal()
{
	return sqrtf(getLength(0)*getLength(0) + getLength(1)*getLength(1) + getLength(2)*getLength(2));
}

///
/// Add a point to create the bounding box
///
/// @param p point to be added
///
void CBoundingBox::addPoint(glm::vec3 p)
{
	if(p.x < m_pMin.x) m_pMin.x = p.x;
	if(p.x > m_pMax.x) m_pMax.x = p.x;
	if(p.y < m_pMin.y) m_pMin.y = p.y;
	if(p.y > m_pMax.y) m_pMax.y = p.y;
	if(p.z < m_pMin.z) m_pMin.z = p.z;
	if(p.z > m_pMax.z) m_pMax.z = p.z;
}