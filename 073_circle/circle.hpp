// circle.hpp

#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include <cmath>

#include "point.hpp"

class Circle {
 private:
  Point center;
  const double radius;

 public:
  // Constructor
  Circle(const Point & centerPoint, double r) : center(centerPoint), radius(r) {}

  // Move the circle's center by dx and dy
  void move(double dx, double dy);

  // Compute the area of intersection between two circles
  double intersectionArea(const Circle & otherCircle);
};

#endif  // CIRCLE_HPP
