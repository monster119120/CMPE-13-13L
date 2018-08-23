/*
 * File:   CMath.c
 * Author: Sophia Tacderas
 * CMPE 13L-04, Fri 1-3 pm
 * Created on January 13, 2016, 1:02 PM
 */

// ***Include libraries here ***
#include "CMath.h"
#include "stdio.h"
#include "stdlib.h"

// Declaration of absolute value function
float absolute(float x);

// Contains the algorithms for calculating the Euclidean distance

float enorm(float px, float py, float qx, float qy)
{
    float dx = absolute(qx - px);
    float dy = absolute(qy - py);
    float g, e, r, s, t;
    int x = 0;

    if (dx > dy) {

        g = dx;
        e = dy;
    } else {

        g = dy;
        e = dx;

    }
    while (x < 2) {

        t = e / g;
        r = t * t;
        s = (r) / (4.0 + r);
        g = (g) + (2 * s * g);
        e = e * s;

        x = x + 1;
    }
    return g; // g = result
}

// Contains the algorithms for calculating the arctangent

float arctangent2(float y, float x)
{
    float y_bar = absolute(y);
    float pi = 3.14159265;
    float o, r;

    if (x > 0) {

        r = (x - y_bar) / (x + y_bar);
        o = (0.1963 * r * r * r) - (0.9817 * r) + (pi / 4);
    } else {

        r = (x + y_bar) / (x - y_bar);
        o = (0.1963 * r * r * r) - (0.9817 * r) + ((3 * pi) / 4);
    }
    if (y_bar < 0) {

        o = -o;
    }
    return o; // o = result_2
}

// Contains the algorithms for calculating absolute value

float absolute(float x)
{
    if (x < 0) {

        x = x * (-1);
    } else {

        x = x;
    }
    return x;
}
