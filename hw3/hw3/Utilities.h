#pragma once
/*Utilities.h
 * Define some utility classes to be used for the raytracing operation in main
 * but not fit as members of other classes.
 *
 * Author:Ryan Cori
 * Date: 1/21/15
 */

#ifdef EXPORT_TEST_FUNCTIONS

#define MY_CPP_UNITTESTAPP_EXPORT __declspec(dllexport)
#else
#define MY_CPP_UNITTESTAPP_EXPORT
#endif

#include "HitInfo.h"

class Utilities{
public:
	MY_CPP_UNITTESTAPP_EXPORT static HitInfo RayIntersect(Ray ray);
	MY_CPP_UNITTESTAPP_EXPORT static void FindColor(HitInfo info, float* colorVals);
};