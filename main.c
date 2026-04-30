#include <stdio.h>
#include <structs.h>

int main(void) {
    Point point = make_point(1.0, 0.0);
    Lanelet empty_lanelet = (Lanelet){0};
    Lanelet lanelet = make_lanelet_empty(2);
    PointArray_push(&lanelet.left_bound, make_point(1.0, 1.0));
    PointArray_push(&lanelet.left_bound, make_point(2.0, 2.0));

    CRScenario scenario = make_cr_scenario_empty(2, 2);
    LaneletArray_push(&scenario.lanelets, lanelet);

    printf("Point coordinates: (%.2f, %.2f)\n", point.x, point.y);
    printf("Lanelet left bound size: %zu\n", lanelet.left_bound.count);

    Point laneletPoint = *PointArray_at(&lanelet.left_bound, 0);
    printf("Lanelet left bound point 1: (%.2f, %.2f)\n", laneletPoint.x, laneletPoint.y);

    printf("Number of dynamic obstacles in scenario: %zu\n", scenario.dynamicObstacles.count);
    printf("Number of lanelets in scenario: %zu\n", scenario.lanelets.count);
}