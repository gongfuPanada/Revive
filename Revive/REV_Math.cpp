#include "REV_Math.h"

OVR::Matrix4f rev_HmdMatrixToOVRMatrix(vr::HmdMatrix34_t m)
{
	OVR::Matrix4f r;
	memcpy(r.M, m.m, sizeof(vr::HmdMatrix34_t));
	return r;
}

OVR::Vector3f rev_HmdVectorToOVRVector(vr::HmdVector3_t v)
{
	OVR::Vector3f r;
	r.x = v.v[0];
	r.y = v.v[1];
	r.z = v.v[2];
	return r;
}

vr::HmdMatrix34_t rev_OvrPoseToHmdMatrix(ovrPosef pose)
{
	vr::HmdMatrix34_t result;
	OVR::Matrix4f matrix(pose);
	memcpy(result.m, matrix.M, sizeof(result.m));
	return result;
}

ovrPoseStatef rev_TrackedDevicePoseToOVRPose(vr::TrackedDevicePose_t pose, double time)
{
	ovrPoseStatef result = { 0 };
	result.ThePose = OVR::Posef::Identity();

	OVR::Matrix4f matrix;
	if (pose.bPoseIsValid)
		matrix = rev_HmdMatrixToOVRMatrix(pose.mDeviceToAbsoluteTracking);
	else
		return result;

	result.ThePose.Orientation = OVR::Quatf(matrix);
	result.ThePose.Position = matrix.GetTranslation();
	result.AngularVelocity = rev_HmdVectorToOVRVector(pose.vAngularVelocity);
	result.LinearVelocity = rev_HmdVectorToOVRVector(pose.vVelocity);
	// TODO: Calculate acceleration.
	result.AngularAcceleration = ovrVector3f();
	result.LinearAcceleration = ovrVector3f();
	result.TimeInSeconds = time;

	return result;
}
