#ifndef ALG_H
#define ALG_H

struct Vec2 {
	float x;
	float y;

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
