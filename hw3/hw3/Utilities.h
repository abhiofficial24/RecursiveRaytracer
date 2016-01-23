#pragma once
/*Utilities.h
 * Define some utility classes to be used for the raytracing operation in main
 * but not fit as members of other classes.
 *
 * Author:Ryan Cori
 * Date: 1/21/15
 */

#include "HitInfo.h"

class Utilities{
public:
	static HitInfo RayIntersect(Ray ray);
	static void FindColor(HitInfo info, float* colorVals);
};