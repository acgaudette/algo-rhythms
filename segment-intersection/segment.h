#ifndef SEGMENT_H
#define SEGMENT_H

#include "alg.h"

struct Segment {
	Vec2 start;
	Vec2 end;

	// Returns true if the two segments intersect in the plane.
	// Ignores collinear edge cases, including the case where both segments
	// are on the same line.
	bool Intersects(const Segment other) const;
};

#endif
