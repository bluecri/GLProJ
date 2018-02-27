#pragma once
#include <cstring>
#include <glm/glm.hpp>

class PrintTextListWithModelVecObj {
public:
	float m_printDurationDeltaTime;
	int m_textManagerIndex;
	char * m_text;
	int m_size;
	glm::vec3 modelVec;


	PrintTextListWithModelVecObj(int index, const char * text, glm::vec3 modelVec, int size, float durationTime) : m_printDurationDeltaTime(durationTime), m_textManagerIndex(index), modelVec(modelVec), m_size(size) {
		m_text = new char[strlen(text) + 1];
		strcpy(m_text, text);
	}

	PrintTextListWithModelVecObj(const PrintTextListWithModelVecObj& copy) {
		m_printDurationDeltaTime = copy.m_printDurationDeltaTime;
		m_textManagerIndex = copy.m_textManagerIndex;
		modelVec = copy.modelVec;
		m_size = copy.m_size;
		m_text = new char[strlen(copy.m_text) + 1];
		strcpy(m_text, copy.m_text);
	}

	PrintTextListWithModelVecObj operator=(const PrintTextListWithModelVecObj& eq) {
		m_printDurationDeltaTime = eq.m_printDurationDeltaTime;
		m_textManagerIndex = eq.m_textManagerIndex;
		modelVec = eq.modelVec;
		m_size = eq.m_size;
		m_text = new char[strlen(eq.m_text) + 1];
		strcpy(m_text, eq.m_text);
	}

	~PrintTextListWithModelVecObj() {
		delete[] m_text;
	}
};