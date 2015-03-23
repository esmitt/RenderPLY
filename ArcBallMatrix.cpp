#include "stdafx.h"
#include "ArcBallMatrix.h"


CArcBallMatrix::CArcBallMatrix()
{
	m_Matrix = glm::mat4(1);
	m_fScale = 1.f;
	m_vPan = glm::vec3(0);
}

CArcBallMatrix::~CArcBallMatrix()
{
	Reset();
}

void CArcBallMatrix::SetScale(float fScale)
{
	m_fScale = fScale;
}

void CArcBallMatrix::SetPan(glm::vec3 vPan)
{
	m_vPan = vPan;
}

void CArcBallMatrix::SetRotation(const glm::vec4 & vquat)
{
	float n, s;
	float xs, ys, zs;
	float wx, wy, wz;
	float xx, xy, xz;
	float yy, yz, zz;

	//n = (vquat.x * vquat.x) + (vquat.y * vquat.y) + (vquat.z * vquat.z) + (vquat.w * vquat.w);
	n = glm::dot(vquat, vquat);
	s = (n > 0.0f) ? 2.0f / n : 0.0f;

	xs = vquat.x * s;
	ys = vquat.y * s;
	zs = vquat.z * s;
	wx = vquat.w * xs;
	wy = vquat.w * ys;
	wz = vquat.w * zs;
	xx = vquat.x * xs;
	xy = vquat.x * ys;
	xz = vquat.x * zs;
	yy = vquat.y * ys;
	yz = vquat.y * zs;
	zz = vquat.z * zs;

	// rotation
	m_Matrix[0][0] = 1.0f - (yy + zz);
	m_Matrix[0][1] = xy - wz;
	m_Matrix[0][2] = xz + wy;

	m_Matrix[1][0] = xy + wz;
	m_Matrix[1][1] = 1.0f - (xx + zz);
	m_Matrix[1][2] = yz - wx;

	m_Matrix[2][0] = xz - wy;
	m_Matrix[2][1] = yz + wx;
	m_Matrix[2][2] = 1.0f - (xx + yy);

	m_Matrix[3][3] = 1.0f;

	// translation (pan)
	m_Matrix[0][3] = m_vPan.x;
	m_Matrix[1][3] = m_vPan.y;
	
	// scale (zoom)
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			m_Matrix[i, j] *= m_fScale;
}

void CArcBallMatrix::Reset()
{
	m_Matrix = glm::mat4(1);
	m_fScale = 1.f;
	m_vPan = glm::vec3(0);
}