#include "radians.h"

float degToRad(float *degrees)
{
	//fixAngle
	if(*degrees>360.0f)
		*degrees-=360.0f;
	if(*degrees< -360.0f)
		*degrees+=360.0f;

	return (((*degrees)*M_PI)/180.0f);
}
