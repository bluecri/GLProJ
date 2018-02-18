#pragma once
#include <cstring>

class PrintTextListObj{
public:
	float m_printDurationDeltaTime;
	int m_textManagerIndex;
	char * m_text;
	int m_x, m_y, m_size;

	
	PrintTextListObj(int index, const char * text, int x, int y, int size, float durationTime) : m_printDurationDeltaTime(durationTime), m_textManagerIndex(index), m_x(x), m_y(y), m_size(size) {
		m_text = new char[strlen(text) + 1];
		strcpy(m_text, text);
	}

	PrintTextListObj() {
		delete[] m_text;
	}
};