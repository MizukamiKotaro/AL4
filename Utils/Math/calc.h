#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "../Shape/Line.h"
#include "../Shape/Plane.h"
#include "../Shape/Sphere.h"

class Calc
{
public:
	enum class LineType {
		Line, // 直線
		Ray, // 半直線
		Segment //線分 
	};
public:
	//適当な垂直なベクトルを求める
	static Vector3 Perpendicular(const Vector3& vector);

	//長さを出す
	static float MakeLength(const Vector2& v);
	static float MakeLength(const Vector3& v);
	static float MakeLength(const Vector2& v1, const Vector2& v2);
	static float MakeLength(const Vector3& v1, const Vector3& v2);

	//点v1と線分v1v2の距離
	static float MakeLength(const Vector2& v1, const Vector2& v2, const Vector2& v3);
	static float MakeLength(const Vector3& v1, const Vector3& v2, const Vector3& v3);

	//点v1と線v1v2の距離
	static float MakeLength(const Vector2& v1, const Vector2& v2, const Vector2& v3, LineType linetype);
	static float MakeLength(const Vector3& v1, const Vector3& v2, const Vector3& v3, LineType linetype);

	static Vector3 Project(const Vector3& v1, const Vector3& v2);
	static Vector3 ClosestPoint(const Vector3& point, const Segment& segment);
	static Vector3 ClosestPoint(const Vector3& point, const Ray& ray);
	static Vector3 ClosestPoint(const Vector3& point, const Line& line);

	//今更ながらtを求める
	static float MakeT(const Vector3& point, const Segment& segment);
	static float MakeT(const Vector3& point, const Ray& ray);
	static float MakeT(const Vector3& point, const Line& line);

	//平面との距離
	static float MakeLength(const Sphere& sphere, const Plane& plane);
	static float MakeLength(const Vector3& position, const Plane& plane);

	//正規化
	static Vector2 Normalize(const Vector2& v);
	static Vector3 Normalize(const Vector3& v);

	//内積
	static float Dot(const Vector2& v1, const Vector2& v2);
	static float Dot(const Vector3& v1, const Vector3& v2);

	//外積
	static float Outer(const Vector2& v1, const Vector2& v2);

	//クロス積
	static Vector3 Cross(const Vector3& v1, const Vector3& v2);

	//線形補間
	static Vector3 Lerp(const Vector3& p0, const Vector3& p1, float t);
	//ベジエ
	static Vector3 Bezier(const Vector3& p0, const Vector3& p1, const Vector3& p2, float t);
	//キャトムルロム
	static Vector3 Catmull_rom(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t);

};

