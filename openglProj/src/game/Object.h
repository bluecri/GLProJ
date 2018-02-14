#pragma once
#include <string>
/******************************************************
*  Document   :	Obejct.h
*  Description: �ֻ��� object
*******************************************************/


class Object {
	public:
		static int staticMaxID;		//object ���� ID(0 ~ ). �ڵ� ����.
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