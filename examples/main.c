#include <parser.h>
#include <stdio.h>
#include <structs.h>

int main(void) {
    CRScenario scenario = parse_cr_scenario("scenario.xml");

    printf("Lanelets: %zu\n", scenario.lanelets.count);
    for (size_t i = 0; i < scenario.lanelets.count; i++) {
        Lanelet* l = LaneletArray_at(&scenario.lanelets, i);
        printf("  id=%zu  left=%zu pts  right=%zu pts\n", l->id, l->left_bound.count,
               l->right_bound.count);
    }

    printf("Dynamic obstacles: %zu\n", scenario.dynamicObstacles.count);
    for (size_t i = 0; i < scenario.dynamicObstacles.count; i++) {
        DynamicObstacle* obs = DynamicObstacleArray_at(&scenario.dynamicObstacles, i);
        printf("  id=%zu  type=%d  pos=(%.2f, %.2f)  vel=%.2f\n", obs->id, obs->type,
               obs->initial_state.position.x, obs->initial_state.position.y,
               obs->initial_state.velocity);
    }

    free_cr_scenario(&scenario);
    return 0;
}
