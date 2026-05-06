#include <functions.h>

Point Point_add(Point a, Point b) {
    return (Point){.x = a.x + b.x, .y = a.y + b.y};
}