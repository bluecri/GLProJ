#include "./OpenglResourceManager.h"

OpenglResourceManager::OpenglResourceManager() {

}

BeforeLoadedVertexWithTexture * OpenglResourceManager::getBLVWTWithIndex(int idx) {
	return m_beforeLoadedVertexWithTextureVec[idx];
}
BeforeLoadedTexture * OpenglResourceManager::getBLTWithIndex(int idx) {
	return m_beforeLoadedTextureVec[idx];
}
BeforeLoadedVertexWithTexture * OpenglResourceManager::getBLVWTWithName(std::string name) {
	std::map<std::string, int>::iterator it = m_bLVWTVnameToIndexMap.find(name);
	if (it != m_bLVWTVnameToIndexMap.end()) {
		return m_beforeLoadedVertexWithTextureVec[it->second];
	}
	return NULL;
}
BeforeLoadedTexture * OpenglResourceManager::getBLTWithName(std::string name) {
	std::map<std::string, int>::iterator it = m_bLTnameToIndexMap.find(name);
	if (it != m_bLTnameToIndexMap.end()) {
		return m_beforeLoadedTextureVec[it->second];
	}
	return NULL;
}
int OpenglResourceManager::getBLVWTIndexWithName(std::string name) {
	std::map<std::string, int>::iterator it = m_bLVWTVnameToIndexMap.find(name);
	if (it != m_bLVWTVnameToIndexMap.end()) {
		//return std::distance(m_bLVWTVnameToIndexMap.begin(), it);
		return it->second;
	}
	return -1;
}
int OpenglResourceManager::getBLTIndexWithName(std::string name) {
	std::map<std::string, int>::iterator it = m_bLTnameToIndexMap.find(name);
	if (it != m_bLTnameToIndexMap.end()) {
		//return std::distance(m_bLTnameToIndexMap.begin(), it);
		return it->second;
	}
	return -1;
}





int OpenglResourceManager::getBLVWTLen() {
	return m_beforeLoadedVertexWithTextureVec.size();
}
int OpenglResourceManager::getBLTLen() {
	return m_beforeLoadedTextureVec.size();
}


bool OpenglResourceManager::addVertexVec(std::string vtName, const char * objectFileName) {
	BeforeLoadedVertexWithTexture* tempVertexTexturePtr = new BeforeLoadedVertexWithTexture(vtName, objectFileName);
	if (tempVertexTexturePtr->m_bCanUse) {
		m_beforeLoadedVertexWithTextureVec.push_back(tempVertexTexturePtr);
		m_bLVWTVnameToIndexMap.insert(make_pair(vtName, m_bLVWTVnameToIndexMap.size()));
	}
	else {
		delete tempVertexTexturePtr;
	}
	return true;
}

bool OpenglResourceManager::addTextureVec(std::string vtName, const char * textureFileName) {
	BeforeLoadedTexture* tempTexturePtr = new BeforeLoadedTexture(vtName, textureFileName);
	if (tempTexturePtr->m_bCanUse) {
		m_beforeLoadedTextureVec.push_back(tempTexturePtr);
		m_bLTnameToIndexMap.insert(make_pair(vtName, m_bLTnameToIndexMap.size()));
	}
	else {
		delete tempTexturePtr;
	}
	return true;
}

int OpenglResourceManager::getTotalBLVWTVVertexNum() {
	int vertSum = 0;
	for (size_t i = 0; i < m_beforeLoadedVertexWithTextureVec.size(); i++) {
		vertSum += m_beforeLoadedVertexWithTextureVec[i]->getVertexVecNum();
	}
	return vertSum;
}

int OpenglResourceManager::getTotalBLVWTVVertexSize() {
	int vertSum = 0;
	for (size_t i = 0; i < m_beforeLoadedVertexWithTextureVec.size(); i++) {
		vertSum += m_beforeLoadedVertexWithTextureVec[i]->getVertexVecSize();
	}
	return vertSum;
}

int OpenglResourceManager::getTotalBLVWTVIndiceNum() {
	int vertSum = 0;
	for (size_t i = 0; i < m_beforeLoadedVertexWithTextureVec.size(); i++) {
		vertSum += m_beforeLoadedVertexWithTextureVec[i]->getIndiceVecNum();
	}
	return vertSum;
}

int OpenglResourceManager::getTotalBLVWTVIndiceSize() {
	int vertSum = 0;
	for (size_t i = 0; i < m_beforeLoadedVertexWithTextureVec.size(); i++) {
		vertSum += m_beforeLoadedVertexWithTextureVec[i]->getIndiceVecSize();
	}
	return vertSum;
}

OpenglResourceManager::~OpenglResourceManager() {
	for (size_t i = 0; i < m_beforeLoadedVertexWithTextureVec.size(); i++) {
		delete m_beforeLoadedVertexWithTextureVec[i];
	}
	for (size_t i = 0; i < m_beforeLoadedTextureVec.size(); i++) {
		delete m_beforeLoadedTextureVec[i];
	}
}