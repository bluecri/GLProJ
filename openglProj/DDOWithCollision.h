#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <OBBStruct.h>

#include <string>


#include "src/game/DynamicDrawableObjectWithTexture.h"

class DDOWithCollision : public DynamicDrawableObjectWithTexture {
	public:
		OBBClass pro_obbClass;
		DDOWithCollision(std::string name, int arrIdx, int textureIdx, int vertexIdx, glm::vec3 modlevec, glm::vec3 angleVec, glm::vec3 scaleVec, glm::vec3 compenVec, float obbLen[3], bool isStatic)
			: DynamicDrawableObjectWithTexture(name, arrIdx, textureIdx, vertexIdx, modlevec, angleVec, scaleVec, isStatic), pro_obbClass(compenVec, obbLen){
			
		}

		virtual OBBClass* getObbClassPtr() override {
			return &pro_obbClass;
		}

		
		virtual bool collisionCheck(DynamicDrawableObjectWithTexture* ddoWithTexturePtr) override {
			OBBClass* anotherObbClassPtr = ddoWithTexturePtr->getObbClassPtr();
			if (anotherObbClassPtr == NULL) {
				return false;
			}
			pro_obbClass.initObbBeforeCollisionCheck(modelVec, getRotationMatrix());
			anotherObbClassPtr->initObbBeforeCollisionCheck(ddoWithTexturePtr->modelVec, ddoWithTexturePtr->getRotationMatrix());

			return pro_obbClass.isCollision(&pro_obbClass, anotherObbClassPtr);
		}

};

//reference : https://m.blog.naver.com/PostView.nhn?blogId=njuhb&logNo=140138684368&proxyReferer=https%3A%2F%2Fwww.google.co.kr%2F
//game physics book 