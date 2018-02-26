#pragma once
#include <CollisionProcessInfo.h>

class HasCollisionObj {
public:
	DDOWithCollision * m_ddoWithCollision;
	CollisionProcessInfo * m_cpi;
	bool isCollisionObjDelete = false;	//if false, dead object and will be deleted. Cannot access ddo.
	bool isCollisionCheck = true;	//check collision or not

	HasCollisionObj(DDOWithCollision * ddoWithCollision, CollisionProcessInfo * cpi)
		: m_ddoWithCollision(ddoWithCollision), m_cpi(cpi){

	}

	virtual void collisionOccur(CollisionProcessInfo * anotherCpi) {
	}
	virtual bool collisionCheck(DDOWithCollision * anotherDdoWithCollision) {
		return m_ddoWithCollision->collisionCheck(anotherDdoWithCollision);
	}
	virtual bool collisionCheck(HasCollisionObj * anotherHasCollisionObj) {
		if (anotherHasCollisionObj->isCollisionCheck == false) {
			return false;
		}
		return m_ddoWithCollision->collisionCheck(anotherHasCollisionObj->m_ddoWithCollision);
	}

	virtual void update(float deltaTime) {

	}
};