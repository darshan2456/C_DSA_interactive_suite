#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include headers first so their declarations are parsed correctly
#include "../src/utils/clear_screen.h"
#include "../src/utils/cross_platform_timer.h"
#include "../src/utils/config.h"

// Define overrides for non-interactive test execution
#define TEST_MOCK_SYNC
#define is_instant() 1
#define clear_screen() (void)0
#define sleep_seconds(x) (void)0

// Now include the source directly
#include "../src/process_synchronization/dining_philosophers.c"

void test_dining_philosophers_transitions()
{
    int chopsticks[5] = {-1, -1, -1, -1, -1};
    PhilosopherState phil_states[5] = {THINKING, THINKING, THINKING, THINKING, THINKING};
    int strategy = 0; // Naive

    // P0 transitions THINKING -> HUNGRY -> EATING (since both chopsticks 0 & 1 are free)
    trigger_philosopher(0, strategy, chopsticks, phil_states);
    assert(phil_states[0] == EATING);
    assert(chopsticks[0] == 0);
    assert(chopsticks[1] == 0);

    // P0 transitions EATING -> THINKING (releases chopsticks)
    trigger_philosopher(0, strategy, chopsticks, phil_states);
    assert(phil_states[0] == THINKING);
    assert(chopsticks[0] == -1);
    assert(chopsticks[1] == -1);

    printf("test_dining_philosophers_transitions passed\n");
}

void test_dining_philosophers_naive_deadlock()
{
    int chopsticks[5] = {-1, -1, -1, -1, -1};
    PhilosopherState phil_states[5] = {THINKING, THINKING, THINKING, THINKING, THINKING};
    int strategy = 0; // Naive

    // Simulating the deadlock situation by making each philosopher acquire their left chopstick first.
    // In Naive: picking up left first.
    for (int i = 0; i < 5; i++)
    {
        phil_states[i] = HUNGRY;
        // Acquire left chopstick
        chopsticks[i] = i;
    }

    // Now, all 5 philosophers are HUNGRY and hold exactly 1 chopstick (circular wait)
    int hungry_count = 0;
    int eating_count = 0;
    int chopsticks_held_count = 0;
    for (int p = 0; p < 5; p++)
    {
        if (phil_states[p] == HUNGRY) hungry_count++;
        if (phil_states[p] == EATING) eating_count++;
    }
    for (int c = 0; c < 5; c++)
    {
        if (chopsticks[c] != -1) chopsticks_held_count++;
    }

    assert(hungry_count == 5);
    assert(eating_count == 0);
    assert(chopsticks_held_count == 5); // Deadlocked!

    // Verify no philosopher can transition to EATING
    for (int i = 0; i < 5; i++)
    {
        trigger_philosopher(i, strategy, chopsticks, phil_states);
        assert(phil_states[i] != EATING);
    }

    printf("test_dining_philosophers_naive_deadlock passed\n");
}

void test_dining_philosophers_asymmetric()
{
    int chopsticks[5] = {-1, -1, -1, -1, -1};
    PhilosopherState phil_states[5] = {THINKING, THINKING, THINKING, THINKING, THINKING};
    int strategy = 1; // Asymmetric

    // In Asymmetric strategy:
    // Even IDs (0, 2, 4) pick Left first then Right.
    // Odd IDs (1, 3) pick Right first then Left.

    // Force P0 (Even) to pick Left (C0)
    phil_states[0] = HUNGRY;
    chopsticks[0] = 0;

    // Force P1 (Odd) to pick Right (C2)
    phil_states[1] = HUNGRY;
    chopsticks[2] = 1;

    // If P0 wants right (C1), it is free
    trigger_philosopher(0, strategy, chopsticks, phil_states);
    assert(phil_states[0] == EATING);

    printf("test_dining_philosophers_asymmetric passed\n");
}

void test_dining_philosophers_arbitrator()
{
    int chopsticks[5] = {-1, -1, -1, -1, -1};
    PhilosopherState phil_states[5] = {THINKING, THINKING, THINKING, THINKING, THINKING};
    int strategy = 2; // Arbitrator

    // Under Arbitrator: P0 wants to eat. Neighbors (4, 1) are THINKING.
    phil_states[0] = HUNGRY;
    trigger_philosopher(0, strategy, chopsticks, phil_states);
    assert(phil_states[0] == EATING);

    // Adjacent philosopher P1 wants to eat, but P0 is EATING. Should remain HUNGRY.
    phil_states[1] = HUNGRY;
    trigger_philosopher(1, strategy, chopsticks, phil_states);
    assert(phil_states[1] == HUNGRY);

    printf("test_dining_philosophers_arbitrator passed\n");
}

int main()
{
    test_dining_philosophers_transitions();
    test_dining_philosophers_naive_deadlock();
    test_dining_philosophers_asymmetric();
    test_dining_philosophers_arbitrator();

    printf("All Dining Philosophers tests passed\n");
    return 0;
}
