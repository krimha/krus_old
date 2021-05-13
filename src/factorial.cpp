#include "factorial.h"

unsigned int factorial(unsigned int n) 
{
    if (n > 1) {
	return  n * factorial(n-1);
    }
    else {
	return 1;
    }
}
