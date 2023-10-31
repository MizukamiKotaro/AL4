#include "Collision.h"
#include "../Math/calc.h"
#include "../Math/Vector3.h"
#include <cmath>
#include <algorithm>

bool Collision::IsCollision(const Sphere& s1, const Sphere& s2) {
	float length = Calc::MakeLength(s1.center_, s2.center_);

	//スケールはいじるな！お願いします。
	if (length <= s1.radius_ + s2.radius_) {
		return true;
	}
	return false;
}

bool Collision::IsCollision(const Sphere& sphere, const Plane& plane) {
	float length = Calc::MakeLength(sphere, plane);

	if (length <= sphere.radius_) {
		if (plane.isLimit) {
			Vector3 center = plane.normal;
			center *= plane.distance;

			Vector3 perpendiculars[4];

			perpendiculars[0] = Calc::Normalize(Calc::Perpendicular(plane.normal));
			perpendiculars[1] = { -perpendiculars[0].x,-perpendiculars[0].y, -perpendiculars[0].z };
			perpendiculars[2] = Calc::Cross(plane.normal, perpendiculars[0]);
			perpendiculars[3] = { -perpendiculars[2].x,-perpendiculars[2].y,-perpendiculars[2].z };

			for (int32_t index = 0; index < 4; index++) {
				perpendiculars[index] = center + perpendiculars[index] * 2.0f;
			}

			Vector3 pos = Calc::Project(center - sphere.center_, plane.normal) + sphere.center_;
			float radius = sqrtf(powf(sphere.radius_, 2.0f) - powf(length, 2.0f));

			float len;
			len = Calc::MakeLength(pos, perpendiculars[0], perpendiculars[2]);
			if (len <= radius) {
				return true;
			}
			len = Calc::MakeLength(pos, perpendiculars[0], perpendiculars[3]);
			if (len <= radius) {
				return true;
			}
			len = Calc::MakeLength(pos, perpendiculars[1], perpendiculars[2]);
			if (len <= radius) {
				return true;
			}
			len = Calc::MakeLength(pos, perpendiculars[1], perpendiculars[2]);
			if (len <= radius) {
				return true;
			}

			float dot[4] = {};

			dot[0] = Calc::Dot(plane.normal, Calc::Cross(pos - perpendiculars[2], perpendiculars[0] - pos));
			dot[1] = Calc::Dot(plane.normal, Calc::Cross(pos - perpendiculars[1], perpendiculars[2] - pos));
			dot[2] = Calc::Dot(plane.normal, Calc::Cross(pos - perpendiculars[3], perpendiculars[1] - pos));
			dot[3] = Calc::Dot(plane.normal, Calc::Cross(pos - perpendiculars[0], perpendiculars[3] - pos));

			if ((dot[0] >= 0 && dot[1] >= 0 && dot[2] >= 0 && dot[3] >= 0) || (dot[0] <= 0 && dot[1] <= 0 && dot[2] <= 0 && dot[3] <= 0)) {
				return true;
			}
		}
		else {
			return true;
		}
	}
	return false;
}

bool Collision::IsCollision(const Segment& segment, const Plane& plane) {

	if (Calc::Dot(segment.diff, plane.normal) == 0) {

		Vector3 pos = plane.normal;
		pos *= plane.distance;

		Line tmpLine = { segment.origin,plane.normal };

		pos = Calc::ClosestPoint(pos, tmpLine);

		if (Calc::MakeLength(pos, segment.origin) == 0) {

			return true;
		}
		return false;
	}

	float t = (plane.distance - Calc::Dot(segment.origin, plane.normal)) / Calc::Dot(segment.diff, plane.normal);

	if (t >= 0 && t <= 1) {
		if (plane.isLimit) {
			return IsInPlane(segment, plane);
		}

		return true;
	}
	return false;

}

bool Collision::IsCollision(const Ray& ray, const Plane& plane) {
	if (Calc::Dot(ray.diff, plane.normal) == 0) {
		Vector3 pos = plane.normal;
		pos *= plane.distance;

		Line tmpLine = { ray.origin,plane.normal };

		pos = Calc::ClosestPoint(pos, tmpLine);

		if (Calc::MakeLength(pos, ray.origin) == 0) {

			return true;
		}
		return false;
	}

	float t = (plane.distance - Calc::Dot(ray.origin, plane.normal)) / Calc::Dot(ray.diff, plane.normal);

	if (t >= 0) {
		if (plane.isLimit) {
			return IsInPlane(ray, plane);
		}

		return true;
	}
	return false;

}

bool Collision::IsCollision(const Line& line, const Plane& plane) {

	if (Calc::Dot(line.diff, plane.normal) == 0) {
		Vector3 pos = plane.normal;
		pos *= plane.distance;

		Line tmpLine = { line.origin,plane.normal };

		pos = Calc::ClosestPoint(pos, tmpLine);

		if (Calc::MakeLength(pos, line.origin) == 0) {

			return true;
		}
		return false;
	}

	if (plane.isLimit) {
		return IsInPlane(line, plane);
	}

	return true;
}

bool Collision::IsInPlane(const Segment& segment, const Plane& plane) {

	Vector3 pos = plane.normal;
	pos *= plane.distance;

	Vector3 perpendiculars[4];

	perpendiculars[0] = Calc::Normalize(Calc::Perpendicular(plane.normal));
	perpendiculars[1] = { -perpendiculars[0].x,-perpendiculars[0].y, -perpendiculars[0].z };
	perpendiculars[2] = Calc::Cross(plane.normal, perpendiculars[0]);
	perpendiculars[3] = { -perpendiculars[2].x,-perpendiculars[2].y,-perpendiculars[2].z };

	for (int32_t index = 0; index < 4; index++) {
		perpendiculars[index] = pos + perpendiculars[index] * 2.0f;
	}

	float t = (plane.distance - Calc::Dot(segment.origin, plane.normal)) / Calc::Dot(segment.diff, plane.normal);

	pos = Vector3(segment.origin) + Vector3(segment.diff) * t;

	float dot[4] = {};

	dot[0] = Calc::Dot(plane.normal, Calc::Cross(pos - perpendiculars[2], perpendiculars[0] - pos));
	dot[1] = Calc::Dot(plane.normal, Calc::Cross(pos - perpendiculars[1], perpendiculars[2] - pos));
	dot[2] = Calc::Dot(plane.normal, Calc::Cross(pos - perpendiculars[3], perpendiculars[1] - pos));
	dot[3] = Calc::Dot(plane.normal, Calc::Cross(pos - perpendiculars[0], perpendiculars[3] - pos));

	if ((dot[0] >= 0 && dot[1] >= 0 && dot[2] >= 0 && dot[3] >= 0) || (dot[0] <= 0 && dot[1] <= 0 && dot[2] <= 0 && dot[3] <= 0)) {
		return true;
	}

	return false;
}

bool Collision::IsInPlane(const Ray& ray, const Plane& plane) {

	Vector3 pos = plane.normal;
	pos *= plane.distance;

	Vector3 perpendiculars[4]{};

	perpendiculars[0] = Calc::Normalize(Calc::Perpendicular(plane.normal));
	perpendiculars[1] = { -perpendiculars[0].x,-perpendiculars[0].y, -perpendiculars[0].z };
	perpendiculars[2] = Calc::Cross(plane.normal, perpendiculars[0]);
	perpendiculars[3] = { -perpendiculars[2].x,-perpendiculars[2].y,-perpendiculars[2].z };

	for (int32_t index = 0; index < 4; index++) {
		perpendiculars[index] = pos + perpendiculars[index] * 2.0f;
	}

	float t = (plane.distance - Calc::Dot(ray.origin, plane.normal)) / Calc::Dot(ray.diff, plane.normal);

	pos = Vector3(ray.origin) + Vector3(ray.diff) * t;

	float dot[4] = {};

	dot[0] = Calc::Dot(plane.normal, Calc::Cross(pos - perpendiculars[2], perpendiculars[0] - pos));
	dot[1] = Calc::Dot(plane.normal, Calc::Cross(pos - perpendiculars[1], perpendiculars[2] - pos));
	dot[2] = Calc::Dot(plane.normal, Calc::Cross(pos - perpendiculars[3], perpendiculars[1] - pos));
	dot[3] = Calc::Dot(plane.normal, Calc::Cross(pos - perpendiculars[0], perpendiculars[3] - pos));

	if ((dot[0] >= 0 && dot[1] >= 0 && dot[2] >= 0 && dot[3] >= 0) || (dot[0] <= 0 && dot[1] <= 0 && dot[2] <= 0 && dot[3] <= 0)) {
		return true;
	}

	return false;
}

bool Collision::IsInPlane(const Line& line, const Plane& plane) {

	Vector3 pos = plane.normal;
	pos *= plane.distance;

	Vector3 perpendiculars[4]{};

	perpendiculars[0] = Calc::Normalize(Calc::Perpendicular(plane.normal));
	perpendiculars[1] = { -perpendiculars[0].x,-perpendiculars[0].y, -perpendiculars[0].z };
	perpendiculars[2] = Calc::Cross(plane.normal, perpendiculars[0]);
	perpendiculars[3] = { -perpendiculars[2].x,-perpendiculars[2].y,-perpendiculars[2].z };

	for (int32_t index = 0; index < 4; index++) {
		perpendiculars[index] = pos + perpendiculars[index] * 2.0f;
	}

	float t = (plane.distance - Calc::Dot(line.origin, plane.normal)) / Calc::Dot(line.diff, plane.normal);

	pos = Vector3(line.origin) + Vector3(line.diff) * t;

	float dot[4] = {};

	dot[0] = Calc::Dot(plane.normal, Calc::Cross(pos - perpendiculars[2], perpendiculars[0] - pos));
	dot[1] = Calc::Dot(plane.normal, Calc::Cross(pos - perpendiculars[1], perpendiculars[2] - pos));
	dot[2] = Calc::Dot(plane.normal, Calc::Cross(pos - perpendiculars[3], perpendiculars[1] - pos));
	dot[3] = Calc::Dot(plane.normal, Calc::Cross(pos - perpendiculars[0], perpendiculars[3] - pos));

	if ((dot[0] >= 0 && dot[1] >= 0 && dot[2] >= 0 && dot[3] >= 0) || (dot[0] <= 0 && dot[1] <= 0 && dot[2] <= 0 && dot[3] <= 0)) {
		return true;
	}

	return false;
}

bool Collision::IsCollision(const Triangle& triangle, const Segment& segment) {

	Vector3 normal = Calc::Normalize(Calc::Cross(Vector3(triangle.vertices[1]) - Vector3(triangle.vertices[0]),
		Vector3(triangle.vertices[2]) - Vector3(triangle.vertices[1])));

	if (Calc::Dot(segment.diff, normal) == 0) {

		/*Line tmpLine = { segment.origin,normal };

		Vector3 pos = Calc::ClosestPoint(triangle.vertices[0], tmpLine);


		if (Calc::MakeLength(pos, segment.origin) == 0) {

			return true;
		}*/
		return false;
	}

	Line tmpLine = { {},normal};

	Vector3 closePoint = Calc::ClosestPoint(triangle.vertices[0], tmpLine);

	float distance = Calc::MakeLength(closePoint);

	if (distance != 0) {
		normal = Calc::Normalize(closePoint);
	}

	float t = (distance - Calc::Dot(segment.origin, normal)) / Calc::Dot(segment.diff, normal);

	if (t >= 0 && t <= 1) {
		Vector3 pos = Vector3(segment.origin) + Vector3(segment.diff) * t;

		float dot[3] = {};

		dot[0] = Calc::Dot(normal, Calc::Cross(pos - triangle.vertices[2], Vector3(triangle.vertices[0]) - pos));
		dot[1] = Calc::Dot(normal, Calc::Cross(pos - triangle.vertices[1], Vector3(triangle.vertices[2]) - pos));
		dot[2] = Calc::Dot(normal, Calc::Cross(pos - triangle.vertices[0], Vector3(triangle.vertices[1]) - pos));

		if ((dot[0] >= 0 && dot[1] >= 0 && dot[2] >= 0) || (dot[0] <= 0 && dot[1] <= 0 && dot[2] <= 0)) {
			return true;
		}
	}

	return false;
}

bool Collision::IsCollision(const Triangle& triangle, const Ray& ray) {

	Vector3 normal = Calc::Normalize(Calc::Cross(Vector3(triangle.vertices[1]) - Vector3(triangle.vertices[0]),
		Vector3(triangle.vertices[2]) - Vector3(triangle.vertices[1])));

	if (Calc::Dot(ray.diff, normal) == 0) {

		/*Line tmpLine = { ray.origin,normal };

		Vector3 pos = Calc::ClosestPoint(triangle.vertices[0], tmpLine);


		if (Calc::MakeLength(pos, ray.origin) == 0) {

			return true;
		}*/
		return false;
	}

	Line tmpLine = { {},normal };

	Vector3 closePoint = Calc::ClosestPoint(triangle.vertices[0], tmpLine);

	float distance = Calc::MakeLength(closePoint);

	if (distance != 0) {
		normal = Calc::Normalize(closePoint);
	}

	float t = (distance - Calc::Dot(ray.origin, normal)) / Calc::Dot(ray.diff, normal);

	if (t >= 0) {
		Vector3 pos = Vector3(ray.origin) + Vector3(ray.diff) * t;

		float dot[3] = {};

		dot[0] = Calc::Dot(normal, Calc::Cross(pos - triangle.vertices[2], Vector3(triangle.vertices[0]) - pos));
		dot[1] = Calc::Dot(normal, Calc::Cross(pos - triangle.vertices[1], Vector3(triangle.vertices[2]) - pos));
		dot[2] = Calc::Dot(normal, Calc::Cross(pos - triangle.vertices[0], Vector3(triangle.vertices[1]) - pos));

		if ((dot[0] >= 0 && dot[1] >= 0 && dot[2] >= 0) || (dot[0] <= 0 && dot[1] <= 0 && dot[2] <= 0)) {
			return true;
		}
	}

	return false;
}

bool Collision::IsCollision(const Triangle& triangle, const Line& line) {

	Vector3 normal = Calc::Normalize(Calc::Cross(Vector3(triangle.vertices[1]) - Vector3(triangle.vertices[0]),
		Vector3(triangle.vertices[2]) - Vector3(triangle.vertices[1])));

	if (Calc::Dot(line.diff, normal) == 0) {

		/*Line tmpLine = { line.origin,normal };

		Vector3 pos = Calc::ClosestPoint(triangle.vertices[0], tmpLine);


		if (Calc::MakeLength(pos, line.origin) == 0) {

			return true;
		}*/
		return false;
	}

	Line tmpLine = { {},normal };

	Vector3 closePoint = Calc::ClosestPoint(triangle.vertices[0], tmpLine);

	float distance = Calc::MakeLength(closePoint);

	if (distance != 0) {
		normal = Calc::Normalize(closePoint);
	}

	float t = (distance - Calc::Dot(line.origin, normal)) / Calc::Dot(line.diff, normal);

	Vector3 pos = Vector3(line.origin) + Vector3(line.diff) * t;

	float dot[3] = {};

	dot[0] = Calc::Dot(normal, Calc::Cross(pos - triangle.vertices[2], Vector3(triangle.vertices[0]) - pos));
	dot[1] = Calc::Dot(normal, Calc::Cross(pos - triangle.vertices[1], Vector3(triangle.vertices[2]) - pos));
	dot[2] = Calc::Dot(normal, Calc::Cross(pos - triangle.vertices[0], Vector3(triangle.vertices[1]) - pos));

	if ((dot[0] >= 0 && dot[1] >= 0 && dot[2] >= 0) || (dot[0] <= 0 && dot[1] <= 0 && dot[2] <= 0)) {
		return true;
	}

	return false;
}

bool Collision::IsCollision(const Quadrangle& quadrangle, const Segment& segment) {

	Vector3 normal = Calc::Normalize(Calc::Cross(Vector3(quadrangle.vertices[1]) - Vector3(quadrangle.vertices[0]),
		Vector3(quadrangle.vertices[2]) - Vector3(quadrangle.vertices[1])));

	if (Calc::Dot(segment.diff, normal) == 0) {

		/*Line tmpLine = { segment.origin,normal };

		Vector3 pos = Calc::ClosestPoint(triangle.vertices[0], tmpLine);


		if (Calc::MakeLength(pos, segment.origin) == 0) {

			return true;
		}*/
		return false;
	}


	Line tmpLine = { {},normal };

	Vector3 closePoint = Calc::ClosestPoint(quadrangle.vertices[0], tmpLine);

	float distance = Calc::MakeLength(closePoint);

	if (distance != 0) {
		normal = Calc::Normalize(closePoint);
	}

	float t = (distance - Calc::Dot(segment.origin, normal)) / Calc::Dot(segment.diff, normal);

	if (t >= 0 && t <= 1) {
		Vector3 pos = Vector3(segment.origin) + Vector3(segment.diff) * t;

		float dot[4] = {};

		dot[0] = Calc::Dot(normal, Calc::Cross(pos - quadrangle.vertices[1], Vector3(quadrangle.vertices[0]) - pos));
		dot[1] = Calc::Dot(normal, Calc::Cross(pos - quadrangle.vertices[2], Vector3(quadrangle.vertices[1]) - pos));
		dot[2] = Calc::Dot(normal, Calc::Cross(pos - quadrangle.vertices[3], Vector3(quadrangle.vertices[2]) - pos));
		dot[3] = Calc::Dot(normal, Calc::Cross(pos - quadrangle.vertices[0], Vector3(quadrangle.vertices[3]) - pos));

		if ((dot[0] >= 0 && dot[1] >= 0 && dot[2] >= 0 && dot[3] >= 0) || (dot[0] <= 0 && dot[1] <= 0 && dot[2] <= 0 && dot[3] <= 0)) {
			return true;
		}
	}

	return false;
}

bool Collision::IsCollision(const Quadrangle& quadrangle, const Ray& ray) {

	Vector3 normal = Calc::Normalize(Calc::Cross(Vector3(quadrangle.vertices[1]) - Vector3(quadrangle.vertices[0]),
		Vector3(quadrangle.vertices[2]) - Vector3(quadrangle.vertices[1])));

	if (Calc::Dot(ray.diff, normal) == 0) {

		/*Line tmpLine = { ray.origin,normal };

		Vector3 pos = Calc::ClosestPoint(triangle.vertices[0], tmpLine);


		if (Calc::MakeLength(pos, ray.origin) == 0) {

			return true;
		}*/
		return false;
	}

	Line tmpLine = { {},normal };

	Vector3 closePoint = Calc::ClosestPoint(quadrangle.vertices[0], tmpLine);

	float distance = Calc::MakeLength(closePoint);

	if (distance != 0) {
		normal = Calc::Normalize(closePoint);
	}

	float t = (distance - Calc::Dot(ray.origin, normal)) / Calc::Dot(ray.diff, normal);

	if (t >= 0) {
		Vector3 pos = Vector3(ray.origin) + Vector3(ray.diff) * t;

		float dot[4] = {};

		dot[0] = Calc::Dot(normal, Calc::Cross(pos - quadrangle.vertices[1], Vector3(quadrangle.vertices[0]) - pos));
		dot[1] = Calc::Dot(normal, Calc::Cross(pos - quadrangle.vertices[2], Vector3(quadrangle.vertices[1]) - pos));
		dot[2] = Calc::Dot(normal, Calc::Cross(pos - quadrangle.vertices[3], Vector3(quadrangle.vertices[2]) - pos));
		dot[3] = Calc::Dot(normal, Calc::Cross(pos - quadrangle.vertices[0], Vector3(quadrangle.vertices[3]) - pos));

		if ((dot[0] >= 0 && dot[1] >= 0 && dot[2] >= 0 && dot[3] >= 0) || (dot[0] <= 0 && dot[1] <= 0 && dot[2] <= 0 && dot[3] <= 0)) {
			return true;
		}
	}

	return false;
}

bool Collision::IsCollision(const Quadrangle& quadrangle, const Line& line) {

	Vector3 normal = Calc::Normalize(Calc::Cross(Vector3(quadrangle.vertices[1]) - Vector3(quadrangle.vertices[0]),
		Vector3(quadrangle.vertices[2]) - Vector3(quadrangle.vertices[1])));

	if (Calc::Dot(line.diff, normal) == 0) {

		/*Line tmpLine = { line.origin,normal };

		Vector3 pos = Calc::ClosestPoint(triangle.vertices[0], tmpLine);


		if (Calc::MakeLength(pos, line.origin) == 0) {

			return true;
		}*/
		return false;
	}

	Line tmpLine = { {},normal };

	Vector3 closePoint = Calc::ClosestPoint(quadrangle.vertices[0], tmpLine);

	float distance = Calc::MakeLength(closePoint);

	if (distance != 0) {
		normal = Calc::Normalize(closePoint);
	}

	float t = (distance - Calc::Dot(line.origin, normal)) / Calc::Dot(line.diff, normal);

	Vector3 pos = Vector3(line.origin) + Vector3(line.diff) * t;

	float dot[4] = {};

	dot[0] = Calc::Dot(normal, Calc::Cross(pos - quadrangle.vertices[1], Vector3(quadrangle.vertices[0]) - pos));
	dot[1] = Calc::Dot(normal, Calc::Cross(pos - quadrangle.vertices[2], Vector3(quadrangle.vertices[1]) - pos));
	dot[2] = Calc::Dot(normal, Calc::Cross(pos - quadrangle.vertices[3], Vector3(quadrangle.vertices[2]) - pos));
	dot[3] = Calc::Dot(normal, Calc::Cross(pos - quadrangle.vertices[0], Vector3(quadrangle.vertices[3]) - pos));

	if ((dot[0] >= 0 && dot[1] >= 0 && dot[2] >= 0 && dot[3] >= 0) || (dot[0] <= 0 && dot[1] <= 0 && dot[2] <= 0 && dot[3] <= 0)) {
		return true;
	}

	return false;
}

bool Collision::IsCollision(const AABB& a, const AABB& b) {

	if ((a.min.x <= b.max.x && a.max.x >= b.min.x) &&
		(a.min.y <= b.max.y && a.max.y >= b.min.y) &&
		(a.min.z <= b.max.z && a.max.z >= b.min.z)) {
		return true;
	}
	return false;
}

bool Collision::IsCollision(const AABB& a, const Sphere& sphere) {
	Vector3 closestPoint = { std::clamp(sphere.center_.x,a.min.x,a.max.x),
		std::clamp(sphere.center_.y,a.min.y,a.max.y),
		std::clamp(sphere.center_.z,a.min.z,a.max.z) };

	float distance = Calc::MakeLength(closestPoint, sphere.center_);

	if (distance <= sphere.radius_) {
		return true;
	}

	return false;
}

bool Collision::IsCollision(const AABB& a, const Segment& segment) {

	AABB b = a;

	if (a.max.x < a.min.x) {
		b.min.x = a.max.x;
		b.max.x = a.min.x;
	}
	if (a.max.y < a.min.y) {
		b.min.y = a.max.y;
		b.max.y = a.min.y;
	}
	if (a.max.z < a.min.z) {
		b.min.z = a.max.z;
		b.max.z = a.min.z;
	}

	if (((b.min.x <= segment.origin.x && b.max.x >= segment.origin.x) &&
		(b.min.y <= segment.origin.y && b.max.y >= segment.origin.y) &&
		(b.min.z <= segment.origin.z && b.max.z >= segment.origin.z)) ||
		((b.min.x <= segment.origin.x + segment.diff.x && b.max.x >= segment.origin.x + segment.diff.x) &&
		(b.min.y <= segment.origin.y + segment.diff.y && b.max.y >= segment.origin.y + segment.diff.y) &&
		(b.min.z <= segment.origin.z + segment.diff.z && b.max.z >= segment.origin.z + segment.diff.z))) {
		return true;
	}

	Vector3 radius = (Vector3(b.max) - b.min) / 2.0f;

	Vector3 center = radius + b.min;

	Vector3 verteces[8] = {
		{-radius.x,radius.y,-radius.z},
		{radius.x,radius.y,-radius.z},
		{radius.x,radius.y,radius.z},
		{-radius.x,radius.y,radius.z},
		{-radius.x,-radius.y,-radius.z},
		{radius.x,-radius.y,-radius.z},
		{radius.x,-radius.y,radius.z},
		{-radius.x,-radius.y,radius.z},
	};

	for (uint32_t index = 0; index < 8; index++) {
		verteces[index] += center;
	}

	Quadrangle quadrangle = { verteces[0],verteces[1] ,verteces[2] ,verteces[3] };
	if (IsCollision(quadrangle, segment)) {
		return true;
	}
	quadrangle = { verteces[4],verteces[5] ,verteces[6] ,verteces[7] };
	if (IsCollision(quadrangle, segment)) {
		return true;
	}

	quadrangle = { verteces[0],verteces[1] ,verteces[5] ,verteces[4] };
	if (IsCollision(quadrangle, segment)) {
		return true;
	}

	quadrangle = { verteces[7],verteces[6] ,verteces[2] ,verteces[3] };
	if (IsCollision(quadrangle, segment)) {
		return true;
	}

	quadrangle = { verteces[5],verteces[1] ,verteces[2] ,verteces[6] };
	if (IsCollision(quadrangle, segment)) {
		return true;
	}

	quadrangle = { verteces[0],verteces[4] ,verteces[7] ,verteces[3] };
	if (IsCollision(quadrangle, segment)) {
		return true;
	}

	return false;

}

bool Collision::IsCollision(const AABB& a, const Ray& ray) {

	AABB b = a;

	if (a.max.x < a.min.x) {
		b.min.x = a.max.x;
		b.max.x = a.min.x;
	}
	if (a.max.y < a.min.y) {
		b.min.y = a.max.y;
		b.max.y = a.min.y;
	}
	if (a.max.z < a.min.z) {
		b.min.z = a.max.z;
		b.max.z = a.min.z;
	}

	if ((b.min.x <= ray.origin.x && b.max.x >= ray.origin.x) &&
		(b.min.y <= ray.origin.y && b.max.y >= ray.origin.y) &&
		(b.min.z <= ray.origin.z && b.max.z >= ray.origin.z)) {
		return true;
	}

	Vector3 radius = (Vector3(b.max) - b.min) / 2.0f;

	Vector3 center = radius + b.min;

	Vector3 verteces[8] = {
		{-radius.x,radius.y,-radius.z},
		{radius.x,radius.y,-radius.z},
		{radius.x,radius.y,radius.z},
		{-radius.x,radius.y,radius.z},
		{-radius.x,-radius.y,-radius.z},
		{radius.x,-radius.y,-radius.z},
		{radius.x,-radius.y,radius.z},
		{-radius.x,-radius.y,radius.z},
	};

	for (uint32_t index = 0; index < 8; index++) {
		verteces[index] += center;
	}

	Quadrangle quadrangle = { verteces[0],verteces[1] ,verteces[2] ,verteces[3] };
	if (IsCollision(quadrangle, ray)) {
		return true;
	}
	quadrangle = { verteces[4],verteces[5] ,verteces[6] ,verteces[7] };
	if (IsCollision(quadrangle, ray)) {
		return true;
	}

	quadrangle = { verteces[0],verteces[1] ,verteces[5] ,verteces[4] };
	if (IsCollision(quadrangle, ray)) {
		return true;
	}

	quadrangle = { verteces[7],verteces[6] ,verteces[2] ,verteces[3] };
	if (IsCollision(quadrangle, ray)) {
		return true;
	}

	quadrangle = { verteces[5],verteces[1] ,verteces[2] ,verteces[6] };
	if (IsCollision(quadrangle, ray)) {
		return true;
	}

	quadrangle = { verteces[0],verteces[4] ,verteces[7] ,verteces[3] };
	if (IsCollision(quadrangle, ray)) {
		return true;
	}

	return false;
}

bool Collision::IsCollision(const AABB& a, const Line& line) {

	Vector3 radius = (Vector3(a.max) - a.min) / 2.0f;

	Vector3 center = radius + a.min;

	Vector3 verteces[8] = {
		{-radius.x,radius.y,-radius.z},
		{radius.x,radius.y,-radius.z},
		{radius.x,radius.y,radius.z},
		{-radius.x,radius.y,radius.z},
		{-radius.x,-radius.y,-radius.z},
		{radius.x,-radius.y,-radius.z},
		{radius.x,-radius.y,radius.z},
		{-radius.x,-radius.y,radius.z},
	};

	for (uint32_t index = 0; index < 8; index++) {
		verteces[index] += center;
	}

	Quadrangle quadrangle = { verteces[0],verteces[1] ,verteces[2] ,verteces[3] };
	if (IsCollision(quadrangle, line)) {
		return true;
	}
	quadrangle = { verteces[4],verteces[5] ,verteces[6] ,verteces[7] };
	if (IsCollision(quadrangle, line)) {
		return true;
	}

	quadrangle = { verteces[0],verteces[1] ,verteces[5] ,verteces[4] };
	if (IsCollision(quadrangle, line)) {
		return true;
	}

	quadrangle = { verteces[7],verteces[6] ,verteces[2] ,verteces[3] };
	if (IsCollision(quadrangle, line)) {
		return true;
	}

	quadrangle = { verteces[5],verteces[1] ,verteces[2] ,verteces[6] };
	if (IsCollision(quadrangle, line)) {
		return true;
	}

	quadrangle = { verteces[0],verteces[4] ,verteces[7] ,verteces[3] };
	if (IsCollision(quadrangle, line)) {
		return true;
	}

	return false;
}

bool Collision::IsCollision(const OBB& a, const Sphere& sphere) {

	Matrix4x4 worldMat = {
		a.orientations[0].x,a.orientations[0].y,a.orientations[0].z,0,
		a.orientations[1].x,a.orientations[1].y,a.orientations[1].z,0,
		a.orientations[2].x,a.orientations[2].y,a.orientations[2].z,0,
		a.center.x,a.center.y,a.center.z,1
	};

	Matrix4x4 worldMatInverse = Matrix4x4::Inverse(worldMat);

	Vector3 centerInOBBLocalSpase = Matrix4x4::Multiply(sphere.center_, worldMatInverse);

	AABB aabb = { Vector3(a.size) * (-1), a.size };
	
	Sphere b = { centerInOBBLocalSpase,sphere.scale_,sphere.rotate_,sphere.radius_ };
	
	return IsCollision(aabb, b);
}

bool Collision::IsCollision(const OBB& a, const Segment& segment) {

	Matrix4x4 worldMat = {
		a.orientations[0].x,a.orientations[0].y,a.orientations[0].z,0,
		a.orientations[1].x,a.orientations[1].y,a.orientations[1].z,0,
		a.orientations[2].x,a.orientations[2].y,a.orientations[2].z,0,
		a.center.x,a.center.y,a.center.z,1
	};

	Matrix4x4 rotateMat = {
		a.orientations[0].x,a.orientations[0].y,a.orientations[0].z,0,
		a.orientations[1].x,a.orientations[1].y,a.orientations[1].z,0,
		a.orientations[2].x,a.orientations[2].y,a.orientations[2].z,0,
		0,0,0,1
	};

	Matrix4x4 worldMatInverse = Matrix4x4::Inverse(worldMat);

	Matrix4x4 rotateMatInverse = Matrix4x4::Inverse(rotateMat);

	AABB aabb = { Vector3(a.size) * (-1), a.size };

	Segment s = { Matrix4x4::Multiply(segment.origin, worldMatInverse),Matrix4x4::Multiply(segment.diff, rotateMatInverse) };

	return IsCollision(aabb, s);
}

bool Collision::IsCollision(const OBB& a, const Ray& ray) {

	Matrix4x4 worldMat = {
		a.orientations[0].x,a.orientations[0].y,a.orientations[0].z,0,
		a.orientations[1].x,a.orientations[1].y,a.orientations[1].z,0,
		a.orientations[2].x,a.orientations[2].y,a.orientations[2].z,0,
		a.center.x,a.center.y,a.center.z,1
	};

	Matrix4x4 rotateMat = {
		a.orientations[0].x,a.orientations[0].y,a.orientations[0].z,0,
		a.orientations[1].x,a.orientations[1].y,a.orientations[1].z,0,
		a.orientations[2].x,a.orientations[2].y,a.orientations[2].z,0,
		0,0,0,1
	};

	Matrix4x4 worldMatInverse = Matrix4x4::Inverse(worldMat);

	Matrix4x4 rotateMatInverse = Matrix4x4::Inverse(rotateMat);

	AABB aabb = { Vector3(a.size) * (-1), a.size };

	Ray r = { Matrix4x4::Multiply(ray.origin, worldMatInverse),Matrix4x4::Multiply(ray.diff, rotateMatInverse) };

	return IsCollision(aabb, r);
}

bool Collision::IsCollision(const OBB& a, const Line& line) {

	Matrix4x4 worldMat = {
		a.orientations[0].x,a.orientations[0].y,a.orientations[0].z,0,
		a.orientations[1].x,a.orientations[1].y,a.orientations[1].z,0,
		a.orientations[2].x,a.orientations[2].y,a.orientations[2].z,0,
		a.center.x,a.center.y,a.center.z,1
	};

	Matrix4x4 rotateMat = {
		a.orientations[0].x,a.orientations[0].y,a.orientations[0].z,0,
		a.orientations[1].x,a.orientations[1].y,a.orientations[1].z,0,
		a.orientations[2].x,a.orientations[2].y,a.orientations[2].z,0,
		0,0,0,1
	};

	Matrix4x4 worldMatInverse = Matrix4x4::Inverse(worldMat);

	Matrix4x4 rotateMatInverse = Matrix4x4::Inverse(rotateMat);

	AABB aabb = { Vector3(a.size) * (-1), a.size };

	Line l = { Matrix4x4::Multiply(line.origin, worldMatInverse),Matrix4x4::Multiply(line.diff, rotateMatInverse) };

	return IsCollision(aabb, l);
}

bool Collision::IsCollision(const AABB& a, const OBB& b) {

	Matrix4x4 worldMat = {
		b.orientations[0].x,b.orientations[0].y,b.orientations[0].z,0,
		b.orientations[1].x,b.orientations[1].y,b.orientations[1].z,0,
		b.orientations[2].x,b.orientations[2].y,b.orientations[2].z,0,
		b.center.x,b.center.y,b.center.z,1
	};

	AABB aabb = { Vector3(b.size) * (-1), b.size };

	Vector3 radius = (Vector3(aabb.max) - aabb.min) / 2.0f;

	Vector3 verteces[8] = {
		{-radius.x,radius.y,-radius.z},
		{radius.x,radius.y,-radius.z},
		{radius.x,radius.y,radius.z},
		{-radius.x,radius.y,radius.z},
		{-radius.x,-radius.y,-radius.z},
		{radius.x,-radius.y,-radius.z},
		{radius.x,-radius.y,radius.z},
		{-radius.x,-radius.y,radius.z},
	};

	for (int i = 0; i < 8; i++) {
		verteces[i] = Matrix4x4::Multiply(verteces[i], worldMat);
	}

	Segment s = { verteces[3],verteces[0] - verteces[3] };

	if (IsCollision(a, s)) {
		return true;
	}

	for (int i = 0; i < 3; i++) {
		s = { verteces[i],verteces[i + 1] - verteces[i] };

		if (IsCollision(a, s)) {
			return true;
		}
	}

	s = { verteces[7],verteces[4] - verteces[7] };

	if (IsCollision(a, s)) {
		return true;
	}

	for (int i = 0; i < 3; i++) {
		s = { verteces[i + 4],verteces[i + 1 + 4] - verteces[i + 4] };

		if (IsCollision(a, s)) {
			return true;
		}
	}

	for (int i = 0; i < 4; i++) {
		s = { verteces[i],verteces[i + 4] - verteces[i] };

		if (IsCollision(a, s)) {
			return true;
		}
	}

	radius = (Vector3(a.max) -a.min) / 2.0f;

	Vector3 verteces2[8] = {
		{-radius.x,radius.y,-radius.z},
		{radius.x,radius.y,-radius.z},
		{radius.x,radius.y,radius.z},
		{-radius.x,radius.y,radius.z},
		{-radius.x,-radius.y,-radius.z},
		{radius.x,-radius.y,-radius.z},
		{radius.x,-radius.y,radius.z},
		{-radius.x,-radius.y,radius.z},
	};

	for (int i = 0; i < 8; i++) {
		verteces2[i] += Vector3(a.min) + radius;
	}

	s = { verteces2[3],verteces2[0] - verteces2[3] };

	if (IsCollision(b, s)) {
		return true;
	}

	for (int i = 0; i < 3; i++) {
		s = { verteces2[i],verteces2[i + 1] - verteces2[i] };

		if (IsCollision(b, s)) {
			return true;
		}
	}

	s = { verteces2[7],verteces2[4] - verteces2[7] };

	if (IsCollision(b, s)) {
		return true;
	}

	for (int i = 0; i < 3; i++) {
		s = { verteces2[i + 4],verteces2[i + 1 + 4] - verteces2[i + 4] };

		if (IsCollision(b, s)) {
			return true;
		}
	}

	for (int i = 0; i < 4; i++) {
		s = { verteces2[i],verteces2[i + 4] - verteces2[i] };

		if (IsCollision(b, s)) {
			return true;
		}
	}

	return false;
}

bool Collision::IsCollision(const OBB& a, const OBB& b) {

	Matrix4x4 worldMat = {
		b.orientations[0].x,b.orientations[0].y,b.orientations[0].z,0,
		b.orientations[1].x,b.orientations[1].y,b.orientations[1].z,0,
		b.orientations[2].x,b.orientations[2].y,b.orientations[2].z,0,
		b.center.x,b.center.y,b.center.z,1
	};
	
	Vector3 verteces[8] = {
		{-b.size.x,b.size.y,-b.size.z},
		{b.size.x,b.size.y,-b.size.z},
		{b.size.x,b.size.y,b.size.z},
		{-b.size.x,b.size.y,b.size.z},
		{-b.size.x,-b.size.y,-b.size.z},
		{b.size.x,-b.size.y,-b.size.z},
		{b.size.x,-b.size.y,b.size.z},
		{-b.size.x,-b.size.y,b.size.z},
	};

	for (int i = 0; i < 8; i++) {
		verteces[i] = Matrix4x4::Multiply(verteces[i], worldMat);
	}

	Segment s = { verteces[3],verteces[0] - verteces[3] };

	if (IsCollision(a, s)) {
		return true;
	}

	for (int i = 0; i < 3; i++) {
		s = { verteces[i],verteces[i + 1] - verteces[i] };

		if (IsCollision(a, s)) {
			return true;
		}
	}

	s = { verteces[7],verteces[4] - verteces[7] };

	if (IsCollision(a, s)) {
		return true;
	}

	for (int i = 0; i < 3; i++) {
		s = { verteces[i + 4],verteces[i + 1 + 4] - verteces[i + 4] };

		if (IsCollision(a, s)) {
			return true;
		}
	}

	for (int i = 0; i < 4; i++) {
		s = { verteces[i],verteces[i + 4] - verteces[i] };

		if (IsCollision(a, s)) {
			return true;
		}
	}

	Matrix4x4 worldMat2 = {
		a.orientations[0].x,a.orientations[0].y,a.orientations[0].z,0,
		a.orientations[1].x,a.orientations[1].y,a.orientations[1].z,0,
		a.orientations[2].x,a.orientations[2].y,a.orientations[2].z,0,
		a.center.x,a.center.y,a.center.z,1
	};

	Vector3 verteces2[8] = {
		{-a.size.x,a.size.y,-a.size.z},
		{a.size.x,a.size.y,-a.size.z},
		{a.size.x,a.size.y,a.size.z},
		{-a.size.x,a.size.y,a.size.z},
		{-a.size.x,-a.size.y,-a.size.z},
		{a.size.x,-a.size.y,-a.size.z},
		{a.size.x,-a.size.y,a.size.z},
		{-a.size.x,-a.size.y,a.size.z},
	};

	for (int i = 0; i < 8; i++) {
		verteces2[i] = Matrix4x4::Multiply(verteces2[i], worldMat2);
	}

	s = { verteces2[3],verteces2[0] - verteces2[3] };

	if (IsCollision(b, s)) {
		return true;
	}

	for (int i = 0; i < 3; i++) {
		s = { verteces2[i],verteces2[i + 1] - verteces2[i] };

		if (IsCollision(b, s)) {
			return true;
		}
	}

	s = { verteces2[7],verteces2[4] - verteces2[7] };

	if (IsCollision(b, s)) {
		return true;
	}

	for (int i = 0; i < 3; i++) {
		s = { verteces2[i + 4],verteces2[i + 1 + 4] - verteces2[i + 4] };

		if (IsCollision(b, s)) {
			return true;
		}
	}

	for (int i = 0; i < 4; i++) {
		s = { verteces2[i],verteces2[i + 4] - verteces2[i] };

		if (IsCollision(b, s)) {
			return true;
		}
	}

	return false;
}