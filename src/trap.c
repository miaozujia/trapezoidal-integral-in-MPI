#ifndef __TRAP_H__
#define __TRAP_H__
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
//#include "get_input.c"
double fun(double x)
{
    double result=1.0/(1.0+x*x);
    return result;
}
//extern voidTrap(
extern double Trap(
    double left_endpt,double right_endpt,
    int trap_count,
    double base_len)
{
    //double result=0;
    double estimate, x;
    int i;

    estimate = (fun(left_endpt) + fun(right_endpt)) / 2.0; 
    for (i = 1; i <= trap_count - 1; i++)
    {
        x = left_endpt + i * base_len;
        estimate += fun(x);
    }
    estimate = estimate * base_len;
    //*result+=estimate;
    return estimate;
}
#endif
