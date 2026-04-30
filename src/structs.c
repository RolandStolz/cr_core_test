#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <structs.h>

DEFINE_DYNAMIC_ARRAY_FUNCS(PointArray, Point, PointArray, NULL);
DEFINE_DYNAMIC_ARRAY_FUNCS(LaneletArray, Lanelet, LaneletArray, destroy_lanelet);
DEFINE_DYNAMIC_ARRAY_FUNCS(DynamicObstacleArray, DynamicObstacle, DynamicObstacleArray, NULL);

Point make_point(double x, double y) {
    return (Point){.x = x, .y = y};
}

State make_state(double x, double y, double orientation, double velocity, size_t time) {
    return (State){.position = make_point(x, y),
                   .orientation = orientation,
                   .velocity = velocity,
                   .time = time};
}

Shape make_shape(double length, double width) {
    return (Shape){.length = length, .width = width};
}

DynamicObstacle make_dynamic_obstacle(State initial_state, Shape shape) {
    return (DynamicObstacle){.initial_state = initial_state, .shape = shape};
}

Lanelet make_lanelet_empty(size_t bound_initial_capacity) {
    Lanelet lanelet = {0};
    PointArray_reserve(&lanelet.left_bound, bound_initial_capacity);
    PointArray_reserve(&lanelet.right_bound, bound_initial_capacity);
    return lanelet;
}

CRScenario make_cr_scenario_empty(size_t lanelet_initial_capacity,
                                  size_t dynamic_obstacle_initial_capacity) {
    CRScenario scenario = {0};
    LaneletArray_reserve(&scenario.lanelets, lanelet_initial_capacity);
    DynamicObstacleArray_reserve(&scenario.dynamicObstacles, dynamic_obstacle_initial_capacity);
    return scenario;
}

void destroy_lanelet(Lanelet* lanelet) {
    assert(lanelet != NULL);
    PointArray_free(&lanelet->left_bound);
    PointArray_free(&lanelet->right_bound);
}