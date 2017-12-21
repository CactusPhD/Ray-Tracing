#pragma once
struct Quaternion
{
	float x, y, z, w; //a quaternion is w + xi + yj + zk

	Quaternion(float _x, float _y, float _z, float _w);

	void Normalize();

	Quaternion Conjugate();

	mat4 ConvertToRotationMatrix();
};

Quaternion operator*(const Quaternion& l, const Quaternion& r);

Quaternion operator*(const Quaternion& q, const vec3& v);

Quaternion::Quaternion(float _x, float _y, float _z, float _w)
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

void Quaternion::Normalize()
{
	float Length = sqrtf(x * x + y * y + z * z + w * w);

	x /= Length;
	y /= Length;
	z /= Length;
	w /= Length;
}


Quaternion Quaternion::Conjugate()
{
	Quaternion ret(-x, -y, -z, w);
	return ret;
}

mat4 Quaternion::ConvertToRotationMatrix() {
	return mat4(
		w*w + x*x - y*y - z*z, 2 * y*x - 2 * w*z, 2 * x*z + 2 * w*y, 0,
		2 * y*x + 2 * w*z, w*w - x*x + y*y - z*z, 2 * y*z - 2 * w*x, 0,
		2 * x*y - 2 * w*y, 2 * y*z + 2 * w*x, w*w - x*x - y*y + z*z, 0,
		0, 0, 0, 1);
}

Quaternion operator*(const Quaternion& l, const Quaternion& r)
{
	const float w = (l.w * r.w) - (l.x * r.x) - (l.y * r.y) - (l.z * r.z);
	const float x = (l.x * r.w) + (l.w * r.x) + (l.y * r.z) - (l.z * r.y);
	const float y = (l.y * r.w) + (l.w * r.y) + (l.z * r.x) - (l.x * r.z);
	const float z = (l.z * r.w) + (l.w * r.z) + (l.x * r.y) - (l.y * r.x);

	Quaternion ret(x, y, z, w);

	return ret;
}

Quaternion operator*(const Quaternion& q, const vec3& v)
{
	const float w = -(q.x * v.x) - (q.y * v.y) - (q.z * v.z);
	const float x = (q.w * v.x) + (q.y * v.z) - (q.z * v.y);
	const float y = (q.w * v.y) + (q.z * v.x) - (q.x * v.z);
	const float z = (q.w * v.z) + (q.x * v.y) - (q.y * v.x);

	Quaternion ret(x, y, z, w);

	return ret;
}
