#include "alg.h"
#include "segment.h"

bool Segment::Intersects(const Segment other) const
{
	// The line segment at origin
	Vec2 compare = end - start;

	// Check relative orientation of the other segment's endpoints
	Vec2 vs = other.start - start;
	float o1 = vs.Cross(compare);
	vs = other.end - start;
	float o2 = vs.Cross(compare);

	// Compare against the other segment
	compare = other.end - other.start;

	vs = start - other.start;
	float t1 = vs.Cross(compare);
	vs = end - other.start;
	float t2 = vs.Cross(compare);

	// Test signs of each comparison
	return o1*o2 < 0 && t1*t2 < 0;
}
