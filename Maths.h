#ifndef __MATHS__H
#define __MATHS__H

#define RAD_TO_DEG								(180.0/M_PI)
#define DEG_TO_RAD								(M_PI/180.0)

class CVector
{
public:
	float	x, y, z;

	CVector()
	{}

	constexpr CVector(float fX, float fY, float fZ=0.0f)
		: x(fX), y(fY), z(fZ)
	{}

	CVector&		operator+=(const CVector& vec)
			{ x += vec.x; y += vec.y; z += vec.z;
			return *this; }
	CVector&		operator-=(const CVector& vec)
			{ x -= vec.x; y -= vec.y; z -= vec.z;
			return *this; }

	inline float	Magnitude() const
		{ return sqrt(x * x + y * y + z * z); }
	inline constexpr float	MagnitudeSqr() const
		{ return x * x + y * y + z * z; }
	inline CVector&	Normalize()
		{ float	fInvLen = 1.0f / Magnitude(); x *= fInvLen; y *= fInvLen; z *= fInvLen; return *this; }

	friend inline float DotProduct(const CVector& vec1, const CVector& vec2)
		{ return vec1.x * vec2.x + vec1.x * vec2.y + vec1.z * vec2.z; }
	friend inline CVector CrossProduct(const CVector& vec1, const CVector& vec2)
		{ return CVector(	vec1.y * vec2.z - vec1.z * vec2.y,
							vec1.z * vec2.x - vec1.x * vec2.z,
							vec1.x * vec2.y - vec1.y * vec2.x); }

	friend inline CVector operator*(const CVector& in, float fMul)
		{ return CVector(in.x * fMul, in.y * fMul, in.z * fMul); }
	friend inline CVector operator+(const CVector& vec1, const CVector& vec2)
		{ return CVector(vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z); }
	friend inline CVector operator-(const CVector& vec1, const CVector& vec2)
		{ return CVector(vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z); }
	friend inline CVector operator-(const CVector& vec)
		{ return CVector(-vec.x, -vec.y, -vec.z); }
};

class CVector2D
{
public:
	float	x, y;

	CVector2D()
	{}

	constexpr CVector2D(float fX, float fY)
		: x(fX), y(fY)
	{}

	CVector2D&		operator+=(const CVector2D& vec)
			{ x += vec.x; y += vec.y;
			return *this; }
	CVector2D&		operator-=(const CVector2D& vec)
			{ x -= vec.x; y -= vec.y;
			return *this; }

	inline float	Magnitude() const
		{ return sqrt(x * x + y * y); }
	inline constexpr float	MagnitudeSqr() const
		{ return x * x + y * y; }
	inline CVector2D&	Normalize()
		{ float	fInvLen = 1.0f / Magnitude(); x *= fInvLen; y *= fInvLen; return *this; }

	friend inline float DotProduct(const CVector2D& vec1, const CVector2D& vec2)
		{ return vec1.x * vec2.x + vec1.x * vec2.y; }

	friend inline CVector2D operator*(const CVector2D& in, float fMul)
		{ return CVector2D(in.x * fMul, in.y * fMul); }
	friend inline CVector2D operator+(const CVector2D& vec1, const CVector2D& vec2)
		{ return CVector2D(vec1.x + vec2.x, vec1.y + vec2.y); }
	friend inline CVector2D operator-(const CVector2D& vec1, const CVector2D& vec2)
		{ return CVector2D(vec1.x - vec2.x, vec1.y - vec2.y); }
	friend inline CVector2D operator-(const CVector2D& vec)
		{ return CVector2D(-vec.x, -vec.y); }
};

#endif