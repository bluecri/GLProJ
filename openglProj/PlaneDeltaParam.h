#pragma once
class PlaneDeltaParam {
private:
	float cretia = 0.0001;
public:
	float yaw, pitch, roll;
	float yawDelta, pitchDelta, rollDelta;
	float yawMinMax, pitchMinMax, rollMinMax;
	float m_yawReback, m_pitchReback, m_rollReback;
	
	
	PlaneDeltaParam(float yaw, float pitch, float roll, float yawDelta, float pitchDelta, float rollDelta, float yawMinMax, float pitchMinMax, float rollMinMax, float yawReback, float pitchReback, float rollReback )
	 : yaw(yaw), pitch(pitch), roll(roll), yawDelta(yawDelta), pitchDelta(pitchDelta), rollDelta(rollDelta), yawMinMax(yawMinMax), pitchMinMax(pitchMinMax), rollMinMax(rollMinMax){
		m_yawReback = yawReback;
		m_pitchReback = pitchReback;
		m_rollReback = rollReback;
	}

	void addYaw(float add) {
		yaw += add;
		if (yaw > yawMinMax) {
			yaw = yawMinMax;
		}
		else if(yaw < -yawMinMax) {
			yaw = -yawMinMax;
		}
	}
	void addPitch(float add) {
		pitch += add;
		if (pitch > pitchMinMax) {
			pitch = pitchMinMax;
		}
		else if (pitch < -pitchMinMax) {
			pitch = -pitchMinMax;
		}
	}
	void addRoll(float add) {
		roll += add;
		if (roll > rollMinMax) {
			roll = rollMinMax;
		}
		else if (roll < -rollMinMax) {
			roll = -rollMinMax;
		}
	}
	void reBackAllParam(float deltaTime) {
		if (yaw > cretia) {
			yaw -= m_yawReback * deltaTime;
			if (yaw < -cretia) {
				yaw = 0;
			}
		}
		else {
			yaw += m_yawReback * deltaTime;
			if (yaw > cretia) {
				yaw = 0;
			}
		}

		if (roll > cretia) {
			roll -= m_rollReback * deltaTime;
			if (roll < -cretia) {
				roll = 0;
			}
		}
		else {
			roll += m_rollReback * deltaTime;
			if (roll > cretia) {
				roll = 0;
			}
		}

		if (pitch > cretia) {
			pitch -= m_pitchReback * deltaTime;
			if (pitch < -cretia) {
				pitch = 0;
			}
		}
		else {
			pitch += m_pitchReback * deltaTime;
			if (pitch > cretia) {
				pitch = 0;
			}
		}
	}
};
