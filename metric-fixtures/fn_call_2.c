/*
 * FN_CALL verification fixture, translation unit 2 of 2.
 *
 * This unit supplies cross-file direct callers, higher-level transitive
 * callers, and the second half of a mutual-recursion cycle.
 *
 * Expected metrics for definitions in this file:
 *     project_entry    = 1
 *     cross_file_direct = 1
 *     top_level        = 0
 *     mutual_b         = 2
 */

extern int leaf(int value);
extern int middle(int value);
extern int repeated_direct_calls(int value);
extern int mutual_a(int value);

int project_entry(int value)
{
    return middle(value) + repeated_direct_calls(value);
}
/* Caller: top_level.  project_entry: FN_CALL = 1. */

int cross_file_direct(int value)
{
    return leaf(value);
}
/* Caller: top_level.  cross_file_direct: FN_CALL = 1. */

int top_level(int value)
{
    return project_entry(value) + cross_file_direct(value);
}
/* No caller in this project.  top_level: FN_CALL = 0. */

int mutual_b(int value)
{
    if (value <= 0) {
        return 0;
    }

    return mutual_a(value - 1);
}
/* Callers in the recursion cycle: mutual_a and mutual_b.  FN_CALL = 2. */
