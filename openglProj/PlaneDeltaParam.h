#pragma once
class PlaneDeltaParam {
	float yaw, pitch, roll;
	float yawDelta, pitchDelta, rollDelta;
	float yawMinMax, pitchMinMax, rollMinMax;
	
	PlaneDeltaParam(float yaw, float pitch, float roll, float yawDelta, float pitchDelta, float rollDelta, float yawMinMax, float pitchMinMax, float rollMinMax)
	 : yaw(yaw), pitch(pitch), roll(roll), yawDelta(yawDelta), pitchDelta(pitchDelta), rollDelta(rollDelta), yawMinMax(yawMinMax), pitchMinMax(pitchMinMax), rollMinMax(rollMinMax){

	}
};
