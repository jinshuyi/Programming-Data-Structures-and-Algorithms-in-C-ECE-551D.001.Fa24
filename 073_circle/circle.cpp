// circle.cpp

#include "circle.hpp"

#include <algorithm>  // Added this line

// Move the circle's center by dx and dy
void Circle::move(double dx, double dy) {
  center.move(dx, dy);
}

// Compute the area of intersection between two circles
double Circle::intersectionArea(const Circle & otherCircle) {
  double r = radius;
  double R = otherCircle.radius;
  double d = center.distanceFrom(otherCircle.center);

  if (d >= r + R)
    return 0;  // circles are separate
  if (d <= fabs(R - r))
    return M_PI * std::min(r, R) *
           std::min(r, R);  // one circle is contained within the other

  double angle1 = 2 * acos((r * r + d * d - R * R) / (2 * r * d));
  double angle2 = 2 * acos((R * R + d * d - r * r) / (2 * R * d));

  double area1 = 0.5 * angle1 * r * r - 0.5 * r * r * sin(angle1);
  double area2 = 0.5 * angle2 * R * R - 0.5 * R * R * sin(angle2);

  return area1 + area2;
}
