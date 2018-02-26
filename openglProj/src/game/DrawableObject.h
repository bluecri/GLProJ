#pragma once

#include <string>
#include <src/game/Object.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>


class DrawableObject : public Object{
	public:
		//glm::mat4 modelMatrix;
		glm::vec3 modelVec;
		bool isDrawableObjDelete = false;	//will be deleted
		bool isDrawableObjDraw = true;	//draw or not
		DrawableObject() : Object(), modelVec(0.0f, 0.0f, 0.0f) {
			
		}
		DrawableObject(std::string name) : Object(name), modelVec(0.0f, 0.0f, 0.0f) {

		}


		DrawableObject(float x, float y, float z) : Object(), modelVec(x, y, z) {
			//modelMatrix = glm::translate(glm::mat4(), modelVec);
		}
		DrawableObject(glm::vec3 vec) : Object(), modelVec(vec) {
			//modelMatrix = glm::translate(glm::mat4(), modelVec);
		}

		DrawableObject(std::string name, float x, float y, float z) : Object(name), modelVec(x, y, z) {
			//modelMatrix = glm::translate(glm::mat4(), modelVec);
		}
		DrawableObject(std::string name, glm::vec3 vec) : Object(name), modelVec(vec) {
			//modelMatrix = glm::translate(glm::mat4(), modelVec);
		}

		virtual ~DrawableObject(){}
};