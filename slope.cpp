/** 
 * Copyright 2014 Keith Batesole 
 *
 * The purpose of this file is to simulate the effect of
 * work needed to traverse a distance at a certain grade
 * of slope. The role of this code is to serve as a prototype
 * to be later extended into the HIVE (Huge Immersive Virutal
 * Virtual Environment) at Miami University to provide a sense
 * of slope to the users. 
 */

#include <stdio.h>
//#include <stdlib.h>
#include <math.h>
#include <errno.h>

/** 
 * A function that is used to calculate the effort
 * needed to traverse a gradiant slope. The output
 * is in units of J/kg*m. 
 * 
 * C_w = 280.5*(i^5) - 58.7*(i^4) - 76.8*(i^3) + 51.9*(i^2)
 *       + 19.6*i + 2.5, where is the gradiant
 */
float calculate_work(float grade) {
  return 280.5 * powf(grade, 5.0f) 
    + -58.7f * powf(grade, 4.0f) 
    + -76.8f * powf(grade, 3.0f)
    + 51.9f * powf(grade, 2.0f) 
    + 19.6f * grade 
    + 2.5;
}

int main(int argc, char** argv) {
  // Check arguments
  if (argc < 4) {
    printf("Specify <slope grade> <mass> <distance>\n");
    return 1;
  }

  // Print out the work needed for various gradients
  for (float i = -.50; i <= .50; i = i + .05) {
    printf("Grade %f: %f\n", i, calculate_work(i));
  }

  // Exit program 
  return 0;
}