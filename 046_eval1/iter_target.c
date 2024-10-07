#include "iter_target.h"

#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

launch_result_t solve_launch(const launch_input_t * this_launch,
                             const planet_list_t * planets) {
  //STEP 4: write this function
  //search
  planet_t * source = find_planet(planets, this_launch->src);
  planet_t * destination = find_planet(planets, this_launch->dest);

  if (source == NULL) {
    fprintf(stderr, "error");
    EXIT_FAILURE;
  }
  if (destination == NULL) {
    fprintf(stderr, "error");
    EXIT_FAILURE;
  }

  double best_total_time = 99999999;
  double best_wait_time = 0.0;

  launch_result_t best_result;
  best_result = compute_launch_by_info(this_launch, planets);

  point_t src_pos = get_location_at(source, this_launch->time);

  //copy the current information
  launch_input_t current_launch = *this_launch;
  uint64_t i = 0;
  while (i < current_launch.max_iterations) {
    point_t dest_pos =
        get_location_at(destination, current_launch.time + best_result.duration);

    double distance =
        sqrt(pow(dest_pos.x - src_pos.x, 2) + pow(dest_pos.y - src_pos.y, 2));
    //meet the final result to finish the iteration
    if (distance <= current_launch.close_enough) {
      break;
    }
    point_t projected_target =
        get_location_at(destination, current_launch.time + best_result.duration);

    launch_result_t current_result = compute_launch_by_info(&current_launch, planets);
    // I think actually the wait time should be included in the duration, however it seems that the result show that it is now included???

    double arrival_time = current_launch.time + current_result.duration;

    double return_time = when_does_planet_return_to(
        destination, projected_target, current_launch.time + arrival_time);
    double wait_time = 0.0;
    if (arrival_time < return_time) {
      wait_time = return_time - arrival_time;
    }

    double total_time = current_result.duration + wait_time;
    if (total_time < best_total_time) {
      best_total_time = total_time;
      best_result = current_result;
      //how to apply the best wait time because ? actually i am confused

      best_wait_time = wait_time;
    }

    current_launch.time += best_result.duration;

    i++;
  }

  return best_result;
}
