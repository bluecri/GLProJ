#pragma once
#include <ManagerOfManagerObserver.h>

class ManagerOfManagerObservee {
private:
	//disable copy, assign 
	ManagerOfManagerObservee(const ManagerOfManagerObservee &copy) {}
	ManagerOfManagerObservee operator=(const ManagerOfManagerObservee &ref) {}

public:
	ManagerOfManagerObserver *m_observer;

	ManagerOfManagerObservee() {

	}

	virtual void registerObserver(ManagerOfManagerObserver *observer) {
		m_observer = observer;
	}

	virtual void notifyBulletFire(glm::vec3 pos, glm::mat4 rotMat) {
		m_observer->bulletFire(pos, rotMat);
	}
	virtual ~ManagerOfManagerObservee() {

	}
};