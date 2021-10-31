#include "amfunctions.h"

float Q_rsqrt(float number)
{
	long i;
	const float x2 = number * 0.5F;
	const float threehalfs = 1.5F;
	i = *(long*)&number;
	i = 0x5f3759df - (i >> 1); // approximating through bitshifting
	number = *(float*)&i;
	number *= threehalfs - (x2 * number * number); // Newton method iteration
	return number;
}
