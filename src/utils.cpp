#include "utils.h"

double Map(double value, double minInRange, double maxInRange, double minOutRange, double maxOutRange){
    if(minOutRange > maxOutRange){
        double temp = minOutRange;
        minOutRange = maxOutRange;
        maxOutRange = temp;
    }

    double x = (value - minInRange) / (maxInRange - minInRange);
    return minOutRange + (maxOutRange - minOutRange) * x;
}