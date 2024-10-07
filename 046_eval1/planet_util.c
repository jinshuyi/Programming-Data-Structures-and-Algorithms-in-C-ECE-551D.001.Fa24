#include "planet_util.h"

#include <stdio.h>
#include <string.h>

#include "provided.h"

point_t get_location_at(const planet_t * p, double time) {
  // Step 2: WRITE THIS
  point_t loc;

  double new_angle = p->init_pos + (2 * M_PI * time / p->year_len);

  loc.y = p->orbital_radius * sin(new_angle);

  loc.x = p->orbital_radius * cos(new_angle);
  return loc;
}
planet_t * find_planet(const planet_list_t * lst, const char * name) {
  //Step 2: WRITE THIS
  for (size_t i = 0; i < lst->num_planets; i++) {
    //strcmp: compare
    if (strcmp(lst->planets[i]->name, name) == 0) {
      return lst->planets[i];
    }
  }
  fprintf(stderr, "error, not found planet");
  exit(EXIT_FAILURE);
}
