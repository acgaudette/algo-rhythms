#ifndef ALG_H
#define ALG_H

struct Vec2 {
	float x;
	float y;

	// "Scalar cross product" of two 2D vectors
	// (returns magnitude of the orthogonal 3D vector).
	// Equivalent to the determinant of a 2x2 matrix
	// with both points as column vectors.
	inline float Cross(const Vec2 other) const
	{
		return x * other.y - y * other.x;
	}

	inline Vec2 operator+(const Vec2 other) const
	{
		return Vec2 {
			x + other.x,
			y + other.y,
		};
	}

	inline Vec2 operator-(const Vec2 other) const
	{
		return Vec2 {
			x - other.x,
			y - other.y,
		};
	}
};

#endif
