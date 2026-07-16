#include "display_header.h"
#include "help.h"
#include "safe_input.h"
#include <stdio.h>

void help_hashing_menu(void)
{
    while (1)
    {
        display_header("Help - Hashing Algorithms");

        printf("Select a sub-topic:\n\n");
        printf("1. Introduction to Hashing & Collisions\n");
        printf("2. Linear Probing (Open Addressing)\n");
        printf("3. Separate Chaining (Closed Addressing)\n");
        printf("4. Quadratic Probing (Open Addressing)\n");
        printf("5. Double Hashing (Open Addressing)\n");
        printf("6. Relationships & Real-world Use Cases\n");
        int choice;
        int status =
            safe_input_int(&choice, "\nenter choice (\'-1\' to exit, or \'help\') : ", 1, 6);

        if (status == INPUT_EXIT_SIGNAL)
        {
            break;
        }

        if (status == 0)
        {
            continue;
        }

        switch (choice)
        {
            case 1:
                display_header("Help - Hashing & Collisions");
                printf("CONCEPT:\n");
                printf("    Hashing is a technique used to uniquely identify a specific object\n");
                printf("    from a group of similar objects using a Hash Function. It maps keys\n");
                printf("    to positions (indices) in a Hash Table, enabling average O(1) time\n");
                printf("    complexity for search, insertion, and deletion operations.\n\n");
                printf("HASH FUNCTION:\n");
                printf(
                    "    A function that converts a given key into a table index. A good hash\n");
                printf("    function distributes keys uniformly across the table to minimize "
                       "collisions.\n\n");
                printf("COLLISIONS:\n");
                printf("    A collision occurs when two different keys map to the same table "
                       "index.\n");
                printf(
                    "    Since multiple keys cannot occupy the same slot, collision resolution\n");
                printf("    methods are required to handle this.\n\n");
                printf("=================================================================\n");
                printf("Press [ENTER] to return...\n");
                printf("=================================================================\n");
                press_enter_to_continue();
                break;

            case 2:
                display_header("Help - Linear Probing");
                printf("LINEAR PROBING:\n");
                printf(
                    "    A method of collision resolution in Open Addressing. When a collision\n");
                printf("    occurs at index i, the table is searched sequentially (one slot at a "
                       "time)\n");
                printf("    for the next available slot.\n\n");
                printf("PROBING FORMULA:\n");
                printf("    hash(key, i) = (hash(key) + i) %% table_size\n");
                printf("    where i = 0, 1, 2, ... is the probe attempt number.\n\n");
                printf("CHARACTERISTICS:\n");
                printf("    • Simple to implement.\n");
                printf("    • High cache performance (sequential access).\n");
                printf("    • Suffers from Primary Clustering (long contiguous runs of occupied "
                       "slots\n");
                printf("      forming, which increases search times).\n\n");
                printf("=================================================================\n");
                printf("Press [ENTER] to return...\n");
                printf("=================================================================\n");
                press_enter_to_continue();
                break;

            case 3:
                display_header("Help - Separate Chaining");
                printf("SEPARATE CHAINING:\n");
                printf(
                    "    A method of collision resolution using Closed Addressing. Instead of\n");
                printf(
                    "    finding another slot in the table, each slot contains a pointer to a\n");
                printf("    linked list of keys that map to the same index.\n\n");
                printf("CHARACTERISTICS:\n");
                printf("    • Table size never restricts the number of elements (grows "
                       "dynamically).\n");
                printf("    • Simple insertion (insert at head/tail of the linked list).\n");
                printf("    • Requires extra pointer memory for lists.\n");
                printf(
                    "    • Worst-case search time degrades to O(N) if all elements hash to the\n");
                printf("      same slot (forming a single long linked list).\n\n");
                printf("=================================================================\n");
                printf("Press [ENTER] to return...\n");
                printf("=================================================================\n");
                press_enter_to_continue();
                break;

            case 4:
                display_header("Help - Quadratic Probing");
                printf("QUADRATIC PROBING:\n");
                printf("    An Open Addressing method designed to eliminate primary clustering.\n");
                printf("    Instead of searching sequentially, it searches slots using quadratic "
                       "offsets.\n\n");
                printf("PROBING FORMULA:\n");
                printf("    hash(key, i) = (hash(key) + c1 * i + c2 * i^2) %% table_size\n");
                printf(
                    "    where c1, c2 are constants and i = 0, 1, 2, ... is the probe number.\n\n");
                printf("CHARACTERISTICS:\n");
                printf("    • Eliminates Primary Clustering.\n");
                printf(
                    "    • Suffers from Secondary Clustering (keys hashing to the same initial\n");
                printf("      index will follow the exact same probe sequence).\n");
                printf("    • Table size must be a prime number to guarantee finding a free "
                       "slot.\n\n");
                printf("=================================================================\n");
                printf("Press [ENTER] to return...\n");
                printf("=================================================================\n");
                press_enter_to_continue();
                break;

            case 5:
                display_header("Help - Double Hashing");
                printf("DOUBLE HASHING:\n");
                printf("    One of the best Open Addressing methods. It uses a second hash "
                       "function\n");
                printf("    to compute the step size for probing when a collision occurs, ensuring "
                       "the\n");
                printf("    step size is unique for each key.\n\n");
                printf("PROBING FORMULA:\n");
                printf("    hash(key, i) = (hash(key) + i * hash2(key)) %% table_size\n");
                printf("    where hash2(key) is a secondary hash function.\n\n");
                printf("CHARACTERISTICS:\n");
                printf("    • Virtually eliminates both Primary and Secondary Clustering.\n");
                printf("    • Provides excellent distribution of keys.\n");
                printf("    • Slightly higher computation overhead due to two hash functions.\n\n");
                printf("=================================================================\n");
                printf("Press [ENTER] to return...\n");
                printf("=================================================================\n");
                press_enter_to_continue();
                break;

            case 6:
                display_header("Help - Relationships & Use Cases");
                printf("OPEN ADDRESSING VS. CLOSED ADDRESSING:\n");
                printf("    • Open Addressing (Linear, Quadratic, Double Hashing):\n");
                printf("      All elements are stored directly in the hash table array.\n");
                printf(
                    "      - Pros: Better Cache Locality (elements are contiguous in memory),\n");
                printf("              no pointer overhead.\n");
                printf(
                    "      - Cons: Table can get full; performance degrades rapidly as the load\n");
                printf("              factor (alpha = N/M) approaches 1.0. Suffers from "
                       "Clustering.\n");
                printf("    • Closed Addressing (Separate Chaining):\n");
                printf("      Elements mapping to the same bucket are linked outside the table.\n");
                printf("      - Pros: Table never fills up; degrades gracefully as elements "
                       "increase.\n");
                printf("      - Cons: Poorer Cache Locality (requires pointer chasing through "
                       "memory),\n");
                printf("              extra memory overhead per node (next pointers).\n\n");

                printf("WHEN TO USE WHAT:\n");
                printf("    • Use Separate Chaining when:\n");
                printf("      - You don't know how many keys will be inserted (dynamic size).\n");
                printf("      - Deletions are very frequent (deleting from chain is trivial,\n");
                printf(
                    "        whereas open addressing requires dummy placeholders/tombstones).\n");
                printf("      - Keys are large/complex, making array slot waste expensive.\n");
                printf("    • Use Linear Probing when:\n");
                printf("      - Keys are small and memory is tight.\n");
                printf("      - High cache locality performance is critical (reads/writes are "
                       "fast).\n");
                printf("      - Load factor is guaranteed to stay low (typically < 0.7).\n");
                printf("    • Use Double Hashing when:\n");
                printf(
                    "      - You want open addressing but want to avoid clustering completely.\n");
                printf("      - Load factor is high, and a second hash computation overhead is "
                       "minor.\n\n");

                printf("REAL-WORLD APPLICATIONS:\n");
                printf(
                    "    • Java's java.util.HashMap uses Separate Chaining (which converts to a\n");
                printf("      Red-Black tree if a chain becomes too long, i.e., > 8 elements).\n");
                printf("    • Python's dict uses Open Addressing with a pseudo-random probing\n");
                printf(
                    "      sequence (similar to quadratic probing/double hashing) to maintain\n");
                printf("      extremely fast cache-friendly lookups.\n");
                printf("    • C++ std::unordered_map is typically implemented using Separate "
                       "Chaining.\n");
                printf("    • Database Indexes and Routers often use Double Hashing or Linear "
                       "Probing\n");
                printf("      for fast, fixed-size hardware-friendly caches.\n\n");
                printf("=================================================================\n");
                printf("Press [ENTER] to return...\n");
                printf("=================================================================\n");
                press_enter_to_continue();
                break;
        }
    }
}
