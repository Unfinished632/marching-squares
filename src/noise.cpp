#include <algorithm>
#include <iostream>

#include "noise.h"

double SimplexNoise::GetNoise2D(Vec2F coords){
    double n0, n1, n2; // Noise contributions from the three corners
    // Skew the input space to determine which simplex cell we're in
    double F2 = 0.5*(sqrt(3.0)-1.0);
    double s = (coords.x+coords.y)*F2; // Hairy factor for 2D
    int i = floor(coords.x+s);
    int j = floor(coords.y+s);
    double G2 = (3.0-sqrt(3.0))/6.0;
    
    double t = (i+j)*G2;
    double X0 = i-t; // Unskew the cell origin back to (x,y) space
    double Y0 = j-t;
    double x0 = coords.x-X0; // The x,y distances from the cell origin
    double y0 = coords.y-Y0;
    
    // For the 2D case, the simplex shape is an equilateral triangle.
    // Determine which simplex we are in.
    int i1, j1; // Offsets for second (middle) corner of simplex in (i,j) coords
    if(x0>y0){
        i1=1; 
        j1=0;  // lower triangle, XY order: (0,0)->(1,0)->(1,1)
    }
    else {
        i1=0;
        j1=1; // upper triangle, YX order: (0,0)->(0,1)->(1,1)
    }
    
    // A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and
    // a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where
    // c = (3-sqrt(3))/6

    double x1 = x0 - i1 + G2; // Offsets for middle corner in (x,y) unskewed coords
    double y1 = y0 - j1 + G2;
    double x2 = x0 - 1.0 + 2.0 * G2; // Offsets for last corner in (x,y) unskewed coords
    double y2 = y0 - 1.0 + 2.0 * G2;

    // Work out the hashed gradient indices of the three simplex corners
    int ii = i & 255;
    int jj = j & 255;
    int gi0 = m_perm[ii+m_perm[jj]] % 12;
    int gi1 = m_perm[ii+i1+m_perm[jj+j1]] % 12;
    int gi2 = m_perm[ii+1+m_perm[jj+1]] % 12;

    // Calculate the contribution from the three corners
    double t0 = 0.5 - x0*x0-y0*y0;
    if (t0<0){
        n0 = 0.0;
    }
    else{
        t0 = t0 * t0;
        n0 = t0 * t0 * GetDot2D(m_gradients3D[gi0], x0, y0); // (x,y) of Gradients3D used for 2D gradient
    }
    
    double t1 = 0.5 - x1*x1-y1*y1;
    if (t1<0){
        n1 = 0.0;
    }
    else{
        t1 = t1*t1;
        n1 = t1 * t1 * GetDot2D(m_gradients3D[gi1], x1, y1);
    }
    
    double t2 = 0.5 - x2*x2-y2*y2;
    if (t2<0){
        n2 = 0.0;
    }
    else{
        t2 = t2*t2;
        n2 = t2 * t2 * GetDot2D(m_gradients3D[gi2], x2, y2);
    }
    
    // Add contributions from each corner to get the final noise value.
    // The result is scaled to return values in the localerval [-1,1].
    double ret = (70.0 * (n0 + n1 + n2));

    return ret;
}

double SimplexNoise::GetDot2D(int tbl[], double x, double y){
    return tbl[0]*x + tbl[1]*y; 
}

SimplexNoise::SimplexNoise(){
    int i = 0;
    for (i = 0 ; i < 255 ; i++) {
        m_perm[i] = m_p[i];
        m_perm[i+256] = m_p[i];
    }
}

SimplexNoise::~SimplexNoise(){
    delete[] m_perm;
    delete[] m_p;
}