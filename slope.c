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

#define _USE_MATH_DEFINES

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>

const float MASS = 225.0f;
const float DISTANCE = 20.0f;
const float GRAVITY = 9.18;

/** 
 * A function that is used to calculate the effort
 * needed to traverse a gradiant slope. The output
 * is in units of J/kg*m. 
 * 
 * C_w = 280.5*(i^5) - 58.7*(i^4) - 76.8*(i^3) + 51.9*(i^2)
 *       + 19.6*i + 2.5, where is the gradiant
 * 
 * Mass assumed to be in kg
 * Distance assumed to be in meters
*/
float calculate_work(float grade, float mass, float distance) {
  float cost =  280.5 * pow(grade, 5.0f) 
    + -58.7f * pow(grade, 4.0f) 
    + -76.8f * pow(grade, 3.0f)
    + 51.9f * pow(grade, 2.0f) 
    + 19.6f * grade 
    + 2.5;
  return cost * mass * distance; 
}

float distance(float x1, float y1, float x2, float y2) {
  return sqrt(pow(fabs(x1 - x2), 2) + pow(fabs(y1 - y2), 2));
}

/**
 * Mass is assumed to be in pounds
 * Distance assumed to be in meters 
 */
float calculate_calories(float distance, float mass) {
  return mass * 0.53f * distance;
}

/** 
 *  Distance assumed to be in meters
 */
float calculate_slope(float dist, float z1, float z2) {
  return atan(fabs(z1 - z2) / dist);
}

/** 
 * Distance assumed to be in meters
 * Slope assumed to be in radians
 */
float calculate_virtual_distance(float slope, float dist) {
  return acos(slope) * dist;
}

/**
 * Distance assumed to be in meters
 */
float calculate_actual_distance(float virt_dist, float calories, float estimated_calories) {
  return virt_dist * (calories / estimated_calories);
}

void simulate_walk(const char * position_file, float mass) {
  /* Open the file for reading */
  printf("Reading file: %s, with mass of %f lbs\n", position_file, mass);
  FILE *fp = fopen(position_file, "r");

  /* Start reading the file */
  float icoords[3], fcoords[3];
  float dist, calories, slope, grade, virt_dist, estimated_work, actual_dist;
  fscanf(fp, "%f %f %f\n", &fcoords[0], &fcoords[1], &fcoords[2]);  
  while(!feof(fp)) {
    /* Shift final coords to initial coords */
    icoords[0] = fcoords[0];
    icoords[1] = fcoords[1];
    icoords[2] = fcoords[2];
    
    /* Read in next pair of numbers */
    fscanf(fp, "%f %f %f\n", &fcoords[0], &fcoords[1], &fcoords[2]);
 
    /* Calculate out distance walked */
    dist = distance(icoords[0], icoords[1], fcoords[0], fcoords[1]);
    
    /* Calculate calories spent */
    calories = calculate_calories((dist / 1609.34f), mass);

    /* Calculate slope */
    slope = calculate_slope(dist, icoords[2], fcoords[2]);

    /* Determine possible virtual distance walked */
    virt_dist = calculate_virtual_distance(slope, dist);

    /* Calculate the grade */
    grade = (abs(icoords[2] - fcoords[2]) / dist);

    /* Calculate total cost needed to traverse virtual distance */
    estimated_work = calculate_work(grade, mass * 0.453492, virt_dist);

    /* Based upon calories spent, determine amount of virtual distance walked */
    actual_dist = calculate_actual_distance(virt_dist, calories, estimated_work / 4.184);
    
    /* Print out diagnostics */
    printf("(%f, %f, %f) -> (%f, %f, %f)\n", icoords[0], icoords[1], icoords[2], 
	   fcoords[0], fcoords[1], fcoords[2]);
    printf("\tDistance: %f\n", dist);
    printf("\tCalories: %f\n", calories);
    printf("\tSlope: %f\n", slope);
    printf("\tGrade: %f\n", grade);
    printf("\tVirtual distance: %f\n", virt_dist);
    printf("\tEsimated calories needed: %f\n", estimated_work);
    printf("\tActual virtual distance travelled: %f\n", actual_dist);
    printf("\n");
  }

  /* Close the file */
  fclose(fp);
}

float simulate_work(float theta, float mass, float distance) {
  return mass * 10 * distance;
}

int main(int argc, char* argv[]) {
  /* Based upon arguments, determine what to do */
  if (argc == 1) {
    /* Print out the work needed for various gradients */
    float i;
    printf("# Slope cost simulated through the cost function. Units J/kg*m\n");
    printf("# Grade Cost Simulated");
    for (i = -.50; i <= .50; i = i + .05) {
      printf("%f\t%f\t%f\n", i, calculate_work(i, MASS, DISTANCE), simulate_work(i, MASS, DISTANCE));
    }
  } else if (argc == 3) {
    /* Read in a data file */
    simulate_walk(argv[1], atof(argv[2]));
  } else if (argc == 4) {
    /* Calculte the cost for the given slope, mass, and distance */
  } else {
    /* Incorrect use of program */
    fprintf(stderr, "Incorrect use of program\n");
  }
  
  /* Exit program */
  return 0;
}
