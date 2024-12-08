#include "utils.h"

double Map(double value, double minInRange, double maxInRange, double minOutRange, double maxOutRange){
    double x = (value - minInRange) / (maxInRange - minInRange);
    return minOutRange + (maxOutRange - minOutRange) * x;
}