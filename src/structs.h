#include <dynamic_arrays.h>
#include <stddef.h>

// -- Point --
typedef struct {
    double x;
    double y;
} Point;

typedef struct {
    Point* data;
    size_t count;
    size_t capacity;
} PointArray;

// -- Lanelet --
typedef struct {
    PointArray left_bound;
    PointArray right_bound;
} Lanelet;

typedef struct {
    Lanelet* data;
    size_t count;
    size_t capacity;
} LaneletArray;

// -- State --
typedef struct {
    Point position;
    double orientation;
    double velocity;
    size_t time;
} State;

// -- Shape --
typedef struct {
    double length;
    double width;
} Shape;

// -- Dynamic obstacle --
typedef struct {
    State initial_state;
    Shape shape;
} DynamicObstacle;

typedef struct {
    DynamicObstacle* data;
    size_t count;
    size_t capacity;
} DynamicObstacleArray;

//  -- CR Scenario --
typedef struct {
    LaneletArray lanelets;
    DynamicObstacleArray dynamicObstacles;
} CRScenario;

Point make_point(double x, double y);
State make_state(double x, double y, double orientation, double velocity, size_t time);
Shape make_shape(double length, double width);
DynamicObstacle make_dynamic_obstacle(State initial_state, Shape shape);

Lanelet make_lanelet_empty(size_t bound_initial_capacity);
void destroy_lanelet(Lanelet* lanelet);

CRScenario make_cr_scenario_empty(size_t lanelet_initial_capacity,
                                  size_t dynamic_obstacle_initial_capacity);

DECLARE_DYNAMIC_ARRAY_FUNCS(PointArray, Point, PointArray);
DECLARE_DYNAMIC_ARRAY_FUNCS(LaneletArray, Lanelet, LaneletArray);
DECLARE_DYNAMIC_ARRAY_FUNCS(DynamicObstacleArray, DynamicObstacle, DynamicObstacleArray);