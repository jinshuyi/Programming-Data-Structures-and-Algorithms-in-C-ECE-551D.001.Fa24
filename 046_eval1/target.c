#include "target.h"

launch_result_t compute_launch_by_info(const launch_input_t * this_launch,
                                       const planet_list_t * planets) {
  planet_t * source = find_planet(planets, this_launch->src);
  planet_t * destination = find_planet(planets, this_launch->dest);
  point_t source_pos = get_location_at(source, this_launch->time);
  point_t dest_pos = get_location_at(destination, this_launch->time);

  //get the distance
  double dx = dest_pos.x - source_pos.x;
  double dy = dest_pos.y - source_pos.y;
  double dis = sqrt(dy * dy + dx * dx);

  double t = dis / this_launch->speed;

  //  launch angle, fomula is used, atan
  double angle = atan2(dy, dx);
  if (angle < 0) {
    angle += M_PI * 2;
  }
  launch_result_t result;
  result.theta = angle;
  result.duration = t;

  return result;
}

double when_does_planet_return_to(const planet_t * planet,
                                  point_t pos,
                                  double start_time) {
  double tar_angle = atan2(pos.y, pos.x);
  //standard the angle
  if (tar_angle < 0) {
    tar_angle += 2 * M_PI;
  }

  //fomd:returns the remainder of the division of two numbers, typically used with floating-point division

  // angle difference
  double cur_angle =
      fmod(planet->init_pos + 2 * M_PI * (start_time / planet->year_len), 2 * M_PI);
  double diffe = fmod(tar_angle - cur_angle + 2 * M_PI, 2 * M_PI);

  double angle_v = 2 * M_PI / planet->year_len;
  double return_time = diffe / angle_v;

  // the whole time
  return start_time + return_time;
}
