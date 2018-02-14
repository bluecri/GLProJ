#pragma once
#include <string>
/******************************************************
*  Document   :	Obejct.h
*  Description: 최상위 object
*******************************************************/


class Object {
	public:
		static int staticMaxID;		//object 고유 ID(0 ~ ). 자동 생성.
		int iObjID;

		std::string name;

		Object() {
			iObjID = staticMaxID;
			++staticMaxID;
			name = staticMaxID;
		}
		Object(std::string str) {
			iObjID = staticMaxID;
			++staticMaxID;
			name = str;
		}
		virtual ~Object() {}
};