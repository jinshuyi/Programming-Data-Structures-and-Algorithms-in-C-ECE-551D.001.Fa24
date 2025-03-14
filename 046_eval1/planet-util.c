#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "provided.h"

//function 1 get_location_at
//new_angle = ini_angle +2pi*t/T
point_t get_location_at(const planet_t * p, double time) {
  point_t loc;

  double new_angle = p->init_pos + (2 * M_PI * time / p->year_len);
  // load to loc.y and x
  loc.y = p->orbital_radius * sin(new_angle);

  loc.x = p->orbital_radius * cos(new_angle);

  return loc;
}

//function 2, Go through the list of all the planets
planet_t * find_planet(const planet_list_t * lst, const char * name) {
  for (size_t i = 0; i < lst->num_planets; i++) {
    //strcmp: compare
    if (strcmp(lst->planets[i]->name, name) == 0) {
      return lst->planets[i];
    }
  }
  fprintf(stderr, "error, not found planet");
  exit(EXIT_FAILURE);
}
