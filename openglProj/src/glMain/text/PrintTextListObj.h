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

	PrintTextListObj(const PrintTextListObj& copy) {
		m_printDurationDeltaTime = copy.m_printDurationDeltaTime;
		m_textManagerIndex = copy.m_textManagerIndex;
		m_x = copy.m_x;
		m_y = copy.m_y;
		m_size = copy.m_size;
		m_text = new char[strlen(copy.m_text) + 1];
		strcpy(m_text, copy.m_text);
	}

	PrintTextListObj operator=(const PrintTextListObj& eq) {
		m_printDurationDeltaTime = eq.m_printDurationDeltaTime;
		m_textManagerIndex = eq.m_textManagerIndex;
		m_x = eq.m_x;
		m_y = eq.m_y;
		m_size = eq.m_size;
		m_text = new char[strlen(eq.m_text) + 1];
		strcpy(m_text, eq.m_text);
	}

	~PrintTextListObj() {
		delete[] m_text;
	}
};