#include <math.h>
#include <stdlib.h>

static double dotProduct(double x1, double y1, double x2, double y2, double X, double Y) {
    double dx1, dx2, dy1, dy2;
    dx1 = x1 - x2;
    dx2 = x1 - X;
    dy1 = y1 - y2;
    dy2 = y1 - Y;
    return dx1 * dx2 + dy1 * dy2;
}

static double crossProduct(double x1, double y1, double x2, double y2, double X, double Y) {
    double dx1, dx2, dy1, dy2;
    dx1 = x1 - x2;
    dx2 = x1 - X;
    dy1 = y1 - y2;
    dy2 = y1 - Y;
    return dx1 * dy2 - dx2 * dy1;
}

static double Mag(double x, double y) {
    return sqrt(x * x + y * y);
}

static double getDistPointLine(double x1, double y1, double x2, double y2, double X, double Y) {
    double Distance;
    if((X == x1) && (Y == y1)) return 0.0;
    if((abs(x1 - x2) < 5.0) && (((y1 < Y) && ( Y < y2)) || ((y1 > Y) && ( Y > y2)))) return X - x1;
    if((abs(y1 - y2) < 5.0) && (((x1 < X) && ( X < x2)) || ((x1 > X) && ( X > x2)))) return Y - y1;
    if((X < x1) && (X < x2)) return 1000;
    if((X > x1) && (X > x2)) return 1000;
    if((Y < y1) && (Y < y2)) return 1000;
    if((Y > y1) && (Y > y2)) return 1000;
    
    Distance = crossProduct(x1, y1, x2, y2, X, Y) / Mag(x2 - x1, y2 - y1);
    return Distance;
} 

