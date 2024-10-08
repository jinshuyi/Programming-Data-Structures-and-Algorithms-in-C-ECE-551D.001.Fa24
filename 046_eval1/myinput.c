#include "myinput.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void parse_planet_info(planet_t * planet, char * line) {
  // Write the code here
  //define the variables in the function
  char stp1_name[MAX_PLANET_NAME_LEN];
  double stp1_orbital_radius;
  double stp1_year_len;
  double stp1_init_pos_degrees;

  // split the string use, use strtok to spilt when :, to divide into 4 parts
  char * part = strtok(line, ":");
  if (part == NULL) {
    fprintf(stderr, "error exists, no name");
    exit(EXIT_FAILURE);
  }

  // check it if exceeds 32
  //strncpy(stp1_name, part, MAX_PLANET_NAME_LEN)char *strncpy(char *dest, const char *src, size_t n);
  //this is because when the number is 32-1=31, it excludes \0 and add one to it
  strncpy(stp1_name, part, MAX_PLANET_NAME_LEN - 1);
  stp1_name[MAX_PLANET_NAME_LEN - 1] = '\0';

  // Ensure the name is null-terminated
  if (strlen(part) >= MAX_PLANET_NAME_LEN) {
    fprintf(stderr, "errors, name characters exceed");
    exit(EXIT_FAILURE);
  }

  //load to the struct planet, finish
  strncpy(planet->name, stp1_name, MAX_PLANET_NAME_LEN);

  //check orbital_radius
  part = strtok(NULL, ":");
  if (part == NULL) {
    fprintf(stderr, "Errors");
    exit(EXIT_FAILURE);
  }

  //atof function:Returns a floating-point number(double type）
  //double atof(const char *str);

  //ensure it positive
  stp1_orbital_radius = atof(part);
  if (stp1_orbital_radius <= 0) {
    fprintf(stderr, "error, unvalid radius");
    exit(EXIT_FAILURE);
  }

  //load to struct
  planet->orbital_radius = stp1_orbital_radius;

  //check year length
  part = strtok(NULL, ":");
  if (part == NULL) {
    fprintf(stderr, "error: no year length");
    exit(EXIT_FAILURE);
  }

  //ensure it positive
  stp1_year_len = atof(part);
  if (stp1_year_len <= 0) {
    fprintf(stderr, "error: unvalid year length");
    exit(EXIT_FAILURE);
  }
  planet->year_len = stp1_year_len;

  // check initial position
  part = strtok(NULL, ":");
  if (part == NULL) {
    fprintf(stderr, "errors, no ins-position.");
    exit(EXIT_FAILURE);
  }

  //ensure it non-negative(for degree, it is from 0->360)
  stp1_init_pos_degrees = atof(part);
  if (stp1_init_pos_degrees < 0 || stp1_init_pos_degrees >= 360) {
    fprintf(stderr, "erorr, exceed the initial degree from0 -360");
    exit(EXIT_FAILURE);
  }

  //convert radians  to degree
  double stp1_init_pos_radians = (M_PI / 180.0) * stp1_init_pos_degrees;

  planet->init_pos = stp1_init_pos_radians;

  //or there is another way,  we can jsut use sscanf
  //int number = sscanf(line, "%31[^:]:%lf:%lf:%lf", stp1_name,& stp1_orbital_radius, &stp1_year_length, &stp1_init_pos_radians);
  //and compare number with4 ? but when I try there is something wrong about this , so I change

  //check if there are more fields
  if (strtok(NULL, ":") != NULL) {
    fprintf(stderr, "Error，more fields");
    exit(EXIT_FAILURE);
  }
}
