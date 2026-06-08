#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

/* Test harness that invokes dijkstra via subprocess to control stdin */
static int run_dijkstra_with_input(const char *graph_input)
{
    int pipefd[2];
    if (pipe(pipefd) == -1)
        return -1;

    pid_t pid = fork();
    if (pid == -1) {
        close(pipefd[0]);
        close(pipefd[1]);
        return -1;
    }

    if (pid == 0) {
        /* Child process */
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);

        /* Create temp file with graph data */
        FILE *f = fopen("/tmp/test_graph.txt", "w");
        if (!f) exit(1);
        fprintf(f, "%s", graph_input);
        fclose(f);

        /* Execute dijkstra binary (assumes it reads from stdin for path) */
        execlp("./dijkstra_test", "dijkstra_test", "/tmp/test_graph.txt", NULL);
        exit(1);
    }

    /* Parent process */
    close(pipefd[0]);
    write(pipefd[1], "/tmp/test_graph.txt\n", 20);
    close(pipefd[1]);

    int status;
    waitpid(pid, &status, 0);
    unlink("/tmp/test_graph.txt");

    return WIFEXITED(status) ? WEXITSTATUS(status) : -1;
}

START_TEST(test_dijkstra_bounds_validation)
{
    /* Invariant: Dijkstra must not crash or access out-of-bounds memory
       when given adversarial vertex indices exceeding declared graph size */

    const char *payloads[] = {
        /* Valid input: 3 vertices, edges within bounds */
        "3\n0 1 5\n1 2 3\n",
        /* Exploit: vertex index 10 exceeds size 3 */
        "3\n0 10 5\n1 2 3\n",
        /* Boundary: vertex index equals size (off-by-one) */
        "3\n0 3 5\n1 2 3\n",
        /* Boundary: negative index */
        "3\n-1 1 5\n0 2 3\n",
        /* Large index on small graph */
        "2\n0 1000000 5\n"
    };

    int num_payloads = sizeof(payloads) / sizeof(payloads[0]);

    for (int i = 0; i < num_payloads; i++) {
        /* The test passes if the process does not segfault or abort.
           A robust implementation must validate indices before array access. */
        int result = run_dijkstra_with_input(payloads[i]);
        
        /* Result should be 0 (success) or 1 (graceful error), not -1 (crash) */
        ck_assert_msg(result != -1,
            "Payload %d caused process crash or abnormal termination", i);
    }
}
END_TEST

Suite *security_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Dijkstra Security");
    tc_core = tcase_create("Bounds Validation");

    tcase_add_test(tc_core, test_dijkstra_bounds_validation);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = security_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}