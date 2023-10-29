#include "calc.h"
#include <cmath>

Vector3 Calc::Perpendicular(const Vector3& vector) {
	if (vector.x != 0.0f || vector.y != 0.0f) {
		return { -vector.y,vector.x,0.0f };
	}
	return { 0.0f,-vector.z,vector.y };
}

float Calc::MakeLength(const Vector2& v) {
	float result = static_cast<float>(sqrt(powf(v.x, 2) + powf(v.y, 2)));
	return result;
}
float Calc::MakeLength(const Vector3& v) {
	float result = static_cast<float>(sqrt(powf(v.x, 2) + powf(v.y, 2) + powf(v.z, 2)));
	return result;
}
float Calc::MakeLength(const Vector2& v1, const Vector2& v2) {
	float result = static_cast<float>(sqrt(powf(v1.x - v2.x, 2) + powf(v1.y - v2.y, 2)));
	return result;
}
float Calc::MakeLength(const Vector3& v1, const Vector3& v2) {
	float result = static_cast<float>(sqrt(powf(v1.x - v2.x, 2) + powf(v1.y - v2.y, 2) + powf(v1.z - v2.z, 2)));
	return result;
}

float Calc::MakeLength(const Vector2& v1, const Vector2& v2, const Vector2& v3) {
	Vector2 lineV[2] = { v3,v1 };
	lineV[0] -= v2;
	lineV[1] -= v2;

	float dot = Dot(lineV[0], lineV[1]);

	float lineLength = MakeLength(v2, v3);

	float t = dot / lineLength / lineLength;

	Vector2 pos = {};
	if (t < 0) {
		pos = v2;
	}
	else if (t > 1) {
		pos = v3;
	}
	else {
		pos = lineV[0] * t + v2;
	}

	float length = MakeLength(v1, pos);
	return length;
}

float Calc::MakeLength(const Vector3& v1, const Vector3& v2, const Vector3& v3) {
	Vector3 lineV[2] = { v3,v1 };
	lineV[0] -= v2;
	lineV[1] -= v2;

	float dot = Dot(lineV[0], lineV[1]);

	float lineLength = MakeLength(v2, v3);

	float t = dot / lineLength / lineLength;

	Vector3 pos = {};
	if (t < 0) {
		pos = v2;
	}
	else if (t > 1) {
		pos = v3;
	}
	else {
		pos = lineV[0] * t + v2;
	}

	float length = MakeLength(v1, pos);
	return length;
}

float Calc::MakeLength(const Vector2& v1, const Vector2& v2, const Vector2& v3, LineType linetype) {
	Vector2 lineV[2] = { v3,v1 };
	lineV[0] -= v2;
	lineV[1] -= v2;

	float dot = Dot(lineV[0], lineV[1]);

	float lineLength = MakeLength(v2, v3);

	float t = dot / lineLength / lineLength;

	Vector2 pos = {};

	switch (linetype)
	{
	case Calc::LineType::Line:
		pos = lineV[0] * t + v2;
		break;
	case Calc::LineType::Ray:
		if (t < 0) {
			pos = v2;
		}
		else {
			pos = lineV[0] * t + v2;
		}
		break;
	case Calc::LineType::Segment:
		if (t < 0) {
			pos = v2;
		}
		else if (t > 1) {
			pos = v3;
		}
		else {
			pos = lineV[0] * t + v2;
		}
		break;
	default:
		break;
	}

	float length = MakeLength(v1, pos);
	return length;
}

float Calc::MakeLength(const Vector3& v1, const Vector3& v2, const Vector3& v3, LineType linetype) {
	Vector3 lineV[2] = { v3,v1 };
	lineV[0] -= v2;
	lineV[1] -= v2;

	float dot = Dot(lineV[0], lineV[1]);

	float lineLength = MakeLength(v2, v3);

	float t = dot / lineLength / lineLength;

	Vector3 pos = {};

	switch (linetype)
	{
	case Calc::LineType::Line:
		pos = lineV[0] * t + v2;
		break;
	case Calc::LineType::Ray:
		if (t < 0) {
			pos = v2;
		}
		else {
			pos = lineV[0] * t + v2;
		}
		break;
	case Calc::LineType::Segment:
		if (t < 0) {
			pos = v2;
		}
		else if (t > 1) {
			pos = v3;
		}
		else {
			pos = lineV[0] * t + v2;
		}
		break;
	default:
		break;
	}

	float length = MakeLength(v1, pos);
	return length;
}

Vector3 Calc::Project(const Vector3& v1, const Vector3& v2) {
	float dot = Dot(v1, v2);
	float length = MakeLength(v2);

	float t = dot / length / length;

	Vector3 result = v2;
	result *= t;
	return result;
}

Vector3 Calc::ClosestPoint(const Vector3& point, const Segment& segment) {
	Vector3 lineV = point;
	lineV -= segment.origin;

	float dot = Dot(lineV, segment.diff);

	float lineLength = MakeLength(segment.diff);

	float t = dot / lineLength / lineLength;

	Vector3 pos = segment.origin;

	if (t > 1) {
		pos += segment.diff;
	}
	else if (t > 0) {
		Vector3 project = segment.diff;
		pos += project * t;
	}

	return pos;
}

Vector3 Calc::ClosestPoint(const Vector3& point, const Ray& ray) {
	Vector3 lineV = point;
	lineV -= ray.origin;

	float dot = Dot(lineV, ray.diff);

	float lineLength = MakeLength(ray.diff);

	float t = dot / lineLength / lineLength;

	Vector3 pos = ray.origin;

	if (t > 0) {
		Vector3 project = ray.diff;
		pos += project * t;
	}

	return pos;
}

Vector3 Calc::ClosestPoint(const Vector3& point, const Line& line) {
	Vector3 lineV = point;
	lineV -= line.origin;

	float dot = Dot(lineV, line.diff);

	float lineLength = MakeLength(line.diff);

	float t = dot / lineLength / lineLength;

	Vector3 pos = line.origin;

	Vector3 project = line.diff;
	pos += project * t;

	return pos;
}

float Calc::MakeT(const Vector3& point, const Segment& segment) {
	Vector3 lineV = point;
	lineV -= segment.origin;

	float dot = Dot(lineV, segment.diff);

	float lineLength = MakeLength(segment.diff);

	float t = dot / lineLength / lineLength;

	return t;
}

float Calc::MakeT(const Vector3& point, const Ray& ray) {
	Vector3 lineV = point;
	lineV -= ray.origin;

	float dot = Dot(lineV, ray.diff);

	float lineLength = MakeLength(ray.diff);

	float t = dot / lineLength / lineLength;

	return t;
}

float Calc::MakeT(const Vector3& point, const Line& line) {
	Vector3 lineV = point;
	lineV -= line.origin;

	float dot = Dot(lineV, line.diff);

	float lineLength = MakeLength(line.diff);

	float t = dot / lineLength / lineLength;

	return t;
}

float Calc::MakeLength(const Sphere& sphere, const Plane& plane) {
	Vector3 pos = plane.normal;
	pos *= plane.distance;
	return MakeLength(Project(pos - sphere.center_, plane.normal));
}

float Calc::MakeLength(const Vector3& position, const Plane& plane) {
	Vector3 pos = plane.normal;
	pos *= plane.distance;
	return MakeLength(Project(pos - position, plane.normal));
}


Vector2 Calc::Normalize(const Vector2& v) {
	float len = MakeLength(v);

	Vector2 result = {};

	if (len != 0) {
		result = { v.x / len,v.y / len };
	}

	return result;
}

Vector3 Calc::Normalize(const Vector3& v) {
	float len = MakeLength(v);

	Vector3 result = {};

	if (len != 0) {
		result = { v.x / len, v.y / len, v.z / len };
	}

	return result;
}

float Calc::Dot(const Vector2& v1, const Vector2& v2) {
	float dot = v1.x * v2.x + v1.y * v2.y;
	return dot;
}

float Calc::Dot(const Vector3& v1, const Vector3& v2) {
	float dot = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	return dot;
}

float Calc::Outer(const Vector2& v1, const Vector2& v2) {
	float outer = v1.x * v2.y - v1.y * v2.x;
	return outer;
}

Vector3 Calc::Cross(const Vector3& v1, const Vector3& v2) {
	Vector3 result = { v1.y * v2.z - v1.z * v2.y,v1.z * v2.x - v1.x * v2.z ,v1.x * v2.y - v1.y * v2.x };
	return result;
}


Vector3 Calc::Lerp(const Vector3& p0, const Vector3& p1, float t) {
	Vector3 result = {};
	result.x = (1.0f - t) * p0.x + t * p1.x;
	result.y = (1.0f - t) * p0.y + t * p1.y;
	result.z = (1.0f - t) * p0.z + t * p1.z;
	return result;
}

Vector3 Calc::Bezier(const Vector3& p0, const Vector3& p1, const Vector3& p2, float t) {
	Vector3 result = Lerp(Lerp(p0, p1, t), Lerp(p1, p2, t), t);
	return result;
}

Vector3 Calc::Catmull_rom(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t) {
	Vector3 result = {};
	result.x = ((-p0.x + 3 * p1.x - 3 * p2.x + p3.x) * t * t * t + (2 * p0.x - 5 * p1.x + 4 * p2.x - p3.x) * t * t + (-p0.x + p2.x) * t + 2 * p1.x) / 2;
	result.y = ((-p0.y + 3 * p1.y - 3 * p2.y + p3.y) * t * t * t + (2 * p0.y - 5 * p1.y + 4 * p2.y - p3.y) * t * t + (-p0.y + p2.y) * t + 2 * p1.y) / 2;
	result.z = ((-p0.z + 3 * p1.z - 3 * p2.z + p3.z) * t * t * t + (2 * p0.z - 5 * p1.z + 4 * p2.z - p3.z) * t * t + (-p0.z + p2.z) * t + 2 * p1.z) / 2;
	return result;
}