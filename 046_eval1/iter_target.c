#include "iter_target.h"

#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
launch_result_t solve_launch(const launch_input_t * this_launch,
                             const planet_list_t * planets) {
  //Write your code here

  // use find_planet to find the source and destination planet
  planet_t * source = find_planet(planets, this_launch->src);
  planet_t * destination = find_planet(planets, this_launch->dest);
  // one best, one current , use best to iterate with current
  launch_result_t best_result;
  launch_result_t current_result;

  point_t source_pos, dest_pos;

  //firstly initialize the duration tto be big enough
  best_result.duration = 9999999999;

  double current_time = this_launch->time;

  //begin iterate , maximum number is max_iterations
  for (uint64_t i = 0; i < this_launch->max_iterations;
       i++) {  // use get_location_at toget the Planetary coordinates
    source_pos = get_location_at(source, current_time);

    dest_pos = get_location_at(destination, current_time);
    //get the launch current result
    current_result = compute_launch_by_info(this_launch, planets);

    double arrival_time = current_time + current_result.duration;
    double planet_return_time =
        when_does_planet_return_to(destination, dest_pos, current_time);
    //intialize the wait_time
    double wait_time = 0;
    if (arrival_time < planet_return_time) {
      wait_time = planet_return_time - arrival_time;
    }
    //total time includes waittime and duration
    double total_time = current_result.duration + wait_time;
    //check the distance if it is close enough
    if (sqrt(pow(dest_pos.x - source_pos.x, 2)) + pow(dest_pos.y - source_pos.y, 2) <=
        this_launch->close_enough) {
      return current_result;
    }
    // compare current and best
    if (current_result.duration < best_result.duration) {
      return current_result;
    }
    //update best
    if (total_time < best_result.duration) {
      best_result = current_result;
      best_result.duration = total_time;
    }
    // Update current time
    current_time += current_result.duration;
  }
  return best_result;
}
//actually accodring to the result, the duration is not included in the waittime but I think it should include???
