#pragma once
#include "../Shape/Sphere.h"
#include "../Shape/Plane.h"
#include "../Shape/Line.h"
#include "../Shape/Triangle.h"
#include "../Shape/AABB.h"
#include "../Shape/Quadrangle.h"
#include "../Shape/OBB.h"

class Collision
{
public:
	static bool IsCollision(const Sphere& s1, const Sphere& s2);
	static bool IsCollision(const Sphere& sphere, const Plane& plane);

	static bool IsCollision(const Segment& segment, const Plane& plane);
	static bool IsCollision(const Ray& ray, const Plane& plane);
	static bool IsCollision(const Line& line, const Plane& plane);

	static bool IsInPlane(const Segment& segment, const Plane& plane);
	static bool IsInPlane(const Ray& ray, const Plane& plane);
	static bool IsInPlane(const Line& line, const Plane& plane);

	static bool IsCollision(const Triangle& triangle, const Segment& segment);
	static bool IsCollision(const Triangle& triangle, const Ray& ray);
	static bool IsCollision(const Triangle& triangle, const Line& line);

	static bool IsCollision(const Quadrangle& quadrangle, const Segment& segment);
	static bool IsCollision(const Quadrangle& quadrangle, const Ray& ray);
	static bool IsCollision(const Quadrangle& quadrangle, const Line& line);
	
	static bool IsCollision(const AABB& a, const AABB& b);

	static bool IsCollision(const AABB& a, const Sphere& sphere);

	static bool IsCollision(const AABB& a, const Segment& segment);
	static bool IsCollision(const AABB& a, const Ray& ray);
	static bool IsCollision(const AABB& a, const Line& line);

	static bool IsCollision(const OBB& a, const Sphere& sphere);

	static bool IsCollision(const OBB& a, const Segment& segment);
	static bool IsCollision(const OBB& a, const Ray& ray);
	static bool IsCollision(const OBB& a, const Line& line);

	static bool IsCollision(const AABB& a, const OBB& b);
	static bool IsCollision(const OBB& a, const OBB& b);
};

