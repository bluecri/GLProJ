#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>

class OBBClass {
	public:
		glm::vec3 m_centerPosVec;		// = model vector
		glm::vec3 m_axisVec[3];	//x, y, z
		float m_axisLen[3];	//x, y, z

		OBBClass() {

		}
		OBBClass(float axisLen[3]) {
			for (int i = 0; i < 3; i++) {
				m_axisLen[i] = axisLen[i];
			}
		}

		void initObbBeforeCollisionCheck(glm::vec3 modelVec, glm::mat4 rotationMatrix) {
			m_centerPosVec = modelVec;
			for (int i = 0; i < 3; i++) {
				for (int k = 0; k < 3; k++) {
					m_axisVec[i][k] = rotationMatrix[i][k];	//init rotation of axis
				}
			}
		}
		
		bool isCollision(OBBClass* ob1, OBBClass* ob2) {
			double c[3][3];	//ob1 3 axis vs ob2 3 axis consine value
			double absC[3][3];	//abs of c[][]
			double d[3];
			double r, r0, r1;	//r = 한 축을 기준으로 obBox 두 중심점의 거리
			const float cutoff = 0.9999;
			bool existParallelPair = false;	//paralell 한가?
			glm::vec3 centerDiff = (ob1->m_centerPosVec) - (ob2->m_centerPosVec);

			//ob1 1 face vs ob2 3 faces
			for (int i = 0; i < 3; i++) {
				c[0][i] = glm::dot(ob1->m_axisVec[0], ob2->m_axisVec[i]);
				absC[0][i] = glm::abs(c[0][i]);
				if (absC[0][i] > cutoff) {
					existParallelPair = true;
				}
			}
			d[0] = glm::dot(centerDiff, ob1->m_axisVec[0]);
			r = abs(d[0]);
			r0 = ob1->m_axisLen[0];
			r1 = ob2->m_axisLen[0] * absC[0][0] + ob2->m_axisLen[1] * absC[0][1] + ob2->m_axisLen[2] * absC[0][2];
			if (r > r0 + r1) {
				return false;
			}

			for (int i = 0; i < 3; i++) {
				c[1][i] = glm::dot(ob1->m_axisVec[1], ob2->m_axisVec[i]);
				absC[1][i] = glm::abs(c[0][i]);
				if (absC[1][i] > cutoff) {
					existParallelPair = true;
				}
			}
			d[1] = glm::dot(centerDiff, ob1->m_axisVec[0]);
			r = abs(d[1]);
			r0 = ob1->m_axisLen[1];
			r1 = ob2->m_axisLen[1] * absC[1][0] + ob2->m_axisLen[1] * absC[1][1] + ob2->m_axisLen[2] * absC[1][2];
			if (r > r0 + r1) {
				return false;
			}

			for (int i = 0; i < 3; i++) {
				c[2][i] = glm::dot(ob1->m_axisVec[1], ob2->m_axisVec[i]);
				absC[2][i] = glm::abs(c[2][i]);
				if (absC[2][i] > cutoff) {
					existParallelPair = true;
				}
			}
			d[2] = glm::dot(centerDiff, ob1->m_axisVec[0]);
			r = abs(d[2]);
			r0 = ob1->m_axisLen[2];
			r1 = ob2->m_axisLen[2] * absC[2][0] + ob2->m_axisLen[1] * absC[2][1] + ob2->m_axisLen[2] * absC[2][2];
			if (r > r0 + r1) {
				return false;
			}

			//ob1 3 face vs ob2 1 faces
			r = abs(glm::dot(centerDiff, ob2->m_axisVec[0]));
			r0 = ob2->m_axisLen[0];
			r1 = ob1->m_axisLen[0] * absC[0][0] + ob1->m_axisLen[1] * absC[1][0] + ob1->m_axisLen[2] * absC[2][0];
			if (r > r0 + r1) {
				return false;
			}
			
			r = abs(glm::dot(centerDiff, ob2->m_axisVec[1]));
			r0 = ob2->m_axisLen[1];
			r1 = ob1->m_axisLen[1] * absC[0][1] + ob1->m_axisLen[1] * absC[1][1] + ob1->m_axisLen[2] * absC[2][1];
			if (r > r0 + r1) {
				return false;
			}
			
			r = abs(glm::dot(centerDiff, ob2->m_axisVec[2]));
			r0 = ob2->m_axisLen[2];
			r1 = ob1->m_axisLen[2] * absC[0][2] + ob1->m_axisLen[1] * absC[1][2] + ob1->m_axisLen[2] * absC[2][2];
			if (r > r0 + r1) {
				return false;
			}

			if (existParallelPair) {
				return true;	//pararell & r0, r1의 거리차가 존재하지 않음 => collision
			}

			//ob1의 axis와 ob2의 axis에 수직인 축을 기준으로 check. (3 axis * 3 axis)
			r = abs(d[2] * c[1][0] - d[1] * c[2][0]);
			r0 = ob1->m_axisLen[1] * absC[2][0] + ob1->m_axisLen[2] * absC[1][0];
			r1 = ob2->m_axisLen[1] * absC[0][2] + ob1->m_axisLen[2] * absC[0][1];
			if (r > r0 + r1) {
				return false;
			}
			r = abs(d[2] * c[1][1] - d[1] * c[2][1]);
			r0 = ob1->m_axisLen[1] * absC[2][1] + ob1->m_axisLen[2] * absC[1][1];
			r1 = ob2->m_axisLen[0] * absC[0][2] + ob1->m_axisLen[2] * absC[0][0];
			if (r > r0 + r1) {
				return false;
			}
			r = abs(d[2] * c[1][2] - d[1] * c[2][2]);
			r0 = ob1->m_axisLen[1] * absC[2][2] + ob1->m_axisLen[2] * absC[1][2];
			r1 = ob2->m_axisLen[0] * absC[0][1] + ob1->m_axisLen[2] * absC[0][0];
			if (r > r0 + r1) {
				return false;
			}

			r = abs(d[0] * c[2][0] - d[2] * c[0][0]);
			r0 = ob1->m_axisLen[0] * absC[2][0] + ob1->m_axisLen[2] * absC[0][0];
			r1 = ob2->m_axisLen[1] * absC[1][2] + ob1->m_axisLen[2] * absC[1][1];
			if (r > r0 + r1) {
				return false;
			}
			r = abs(d[0] * c[2][1] - d[2] * c[0][1]);
			r0 = ob1->m_axisLen[0] * absC[2][1] + ob1->m_axisLen[2] * absC[0][1];
			r1 = ob2->m_axisLen[0] * absC[1][2] + ob1->m_axisLen[2] * absC[1][0];
			if (r > r0 + r1) {
				return false;
			}
			r = abs(d[0] * c[2][2] - d[2] * c[0][2]);
			r0 = ob1->m_axisLen[0] * absC[2][2] + ob1->m_axisLen[2] * absC[0][2];
			r1 = ob2->m_axisLen[0] * absC[1][1] + ob1->m_axisLen[1] * absC[1][0];
			if (r > r0 + r1) {
				return false;
			}

			r = abs(d[1] * c[0][0] - d[0] * c[1][0]);
			r0 = ob1->m_axisLen[0] * absC[1][0] + ob1->m_axisLen[1] * absC[0][0];
			r1 = ob2->m_axisLen[1] * absC[2][2] + ob1->m_axisLen[2] * absC[2][1];
			if (r > r0 + r1) {
				return false;
			}
			r = abs(d[1] * c[0][1] - d[0] * c[1][1]);
			r0 = ob1->m_axisLen[0] * absC[1][1] + ob1->m_axisLen[1] * absC[0][1];
			r1 = ob2->m_axisLen[0] * absC[2][2] + ob1->m_axisLen[2] * absC[2][0];
			if (r > r0 + r1) {
				return false;
			}
			r = abs(d[1] * c[0][2] - d[0] * c[1][2]);
			r0 = ob1->m_axisLen[0] * absC[1][2] + ob1->m_axisLen[1] * absC[0][2];
			r1 = ob2->m_axisLen[0] * absC[2][1] + ob1->m_axisLen[1] * absC[2][0];
			if (r > r0 + r1) {
				return false;
			}
			return true;
		}
};