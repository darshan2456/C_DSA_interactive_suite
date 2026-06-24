#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/utils/clear_screen.h"
#include "../src/utils/cross_platform_timer.h"
#include "../src/utils/config.h"

#define TEST_MOCK_SYNC
#define is_instant() 1
#define clear_screen() (void)0
#define sleep_seconds(x) (void)0

// Include Peterson's source to test state logic and step_process
#include "../src/process_synchronization/petersons_algorithm.c"

void test_petersons_state_transitions()
{
    int flag[2] = {0, 0};
    int turn = 0;
    int pc[2] = {0, 0};

    // Step P0: leaves Non-CS (pc[0] -> 1)
    step_process(0, flag, &turn, pc);
    assert(pc[0] == 1);

    // Step P0: requests entry (sets flag[0] = 1, pc[0] -> 2)
    step_process(0, flag, &turn, pc);
    assert(pc[0] == 2);
    assert(flag[0] == 1);

    // Step P0: sets turn = 1 (other process, pc[0] -> 3)
    step_process(0, flag, &turn, pc);
    assert(pc[0] == 3);
    assert(turn == 1);

    // Step P0: evaluates await condition. Since flag[1] (readyB) is 0, P0 enters CS (pc[0] -> 4)
    step_process(0, flag, &turn, pc);
    assert(pc[0] == 4);

    printf("test_petersons_state_transitions passed\n");
}

void test_petersons_mutual_exclusion()
{
    int flag[2] = {0, 0};
    int turn = 0;
    int pc[2] = {0, 0};

    // P0 enters CS
    step_process(0, flag, &turn, pc); // pc[0] -> 1
    step_process(0, flag, &turn, pc); // pc[0] -> 2, flag[0] = 1
    step_process(0, flag, &turn, pc); // pc[0] -> 3, turn = 1
    step_process(0, flag, &turn, pc); // pc[0] -> 4 (CS)
    assert(pc[0] == 4);

    // P1 now requests entry
    step_process(1, flag, &turn, pc); // pc[1] -> 1
    step_process(1, flag, &turn, pc); // pc[1] -> 2, flag[1] = 1
    step_process(1, flag, &turn, pc); // pc[1] -> 3, turn = 0
    assert(pc[1] == 3);

    // Since P0 is in CS (flag[0] == 1) and turn == 0 (favoring P0), P1 should busy-wait
    step_process(1, flag, &turn, pc);
    assert(pc[1] == 3); // Remains blocked

    // Let P0 exit CS
    step_process(0, flag, &turn, pc); // pc[0] -> 5
    step_process(0, flag, &turn, pc); // pc[0] -> 0, flag[0] = 0
    assert(flag[0] == 0);

    // Now P1 should be able to enter CS
    step_process(1, flag, &turn, pc); // pc[1] -> 4 (CS)
    assert(pc[1] == 4);

    printf("test_petersons_mutual_exclusion passed\n");
}

int main()
{
    test_petersons_state_transitions();
    test_petersons_mutual_exclusion();

    printf("All Peterson's Algorithm tests passed\n");
    return 0;
}
