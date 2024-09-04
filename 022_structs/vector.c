#include <stdio.h>
#include <stdlib.h>
#include "vector.h"

double dot_product(vector_t v1, vector_t v2) {
  //YOUR CODE GOES HERE
}
vector_t cross_product(vector_t v1, vector_t v2) {
  //YOUR CODE GOES HERE
}
//You should not modify anything below this line
void print_vector(vector_t v) {
  printf("<%.1f, %.1f, %.1f>", v.x, v.y, v.z);
}

void print_results(vector_t v1, vector_t v2){
  printf("===================================\n");
  printf("v1 = ");
  print_vector(v1);
  printf("\n");
  printf("v2 = ");
  print_vector(v2);
  printf("\n");
  printf("  v1  dot  v2 = %.1f\n", dot_product(v1, v2));
  printf("  v1 cross v2 = ");
  print_vector(cross_product(v1, v2));
  printf("\n");
}
int main(void) {
  vector_t va;
  va.x=1;
  va.y=2;
  va.z=3;
  vector_t vb;
  vb.x=3;
  vb.y=4;
  vb.z=5;
  vector_t vc;
  vc.x=0;
  vc.y=0;
  vc.z=2;
  vector_t vd;
  vd.x=5;
  vd.y=-3;
  vd.z=0;
  print_results(va, vb);
  print_results(vb, va);
  print_results(va, vc);
  print_results(vb, vc);
  print_results(va, vd);
  print_results(vb, vd);
  print_results(vc, vd);
  return EXIT_SUCCESS;
}
