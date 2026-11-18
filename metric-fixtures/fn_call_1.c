/*
 * FN_CALL verification fixture, translation unit 1 of 2.
 *
 * Analyze and link this file together with fn_call_2.c.  AdLint's FN_CALL
 * counts distinct direct and transitive calling functions.  Multiple call
 * sites from the same caller collapse to one caller.
 *
 * Expected metrics for definitions in this file:
 *     leaf                 = 5
 *     middle               = 2
 *     repeated_direct_calls = 2
 *     self_recursive       = 1
 *     mutual_a             = 2
 */

extern int mutual_b(int value);

int leaf(int value)
{
    return value + 1;
}
/*
 * Callers: middle, repeated_direct_calls, cross_file_direct,
 *          project_entry (transitive), top_level (transitive)
 * leaf: FN_CALL = 5.
 */

int middle(int value)
{
    return leaf(value);
}
/* Callers: project_entry, top_level (transitive).  FN_CALL = 2. */

int repeated_direct_calls(int value)
{
    /* Two call sites, but both originate in this one calling function. */
    return leaf(value) + leaf(value + 1);
}
/* Callers: project_entry, top_level (transitive).  FN_CALL = 2. */

int self_recursive(int value)
{
    if (value <= 0) {
        return 0;
    }

    return self_recursive(value - 1);
}
/* Its recursive self-reference makes self_recursive: FN_CALL = 1. */

int mutual_a(int value)
{
    if (value <= 0) {
        return 0;
    }

    return mutual_b(value - 1);
}
/* Callers in the recursion cycle: mutual_b and mutual_a.  FN_CALL = 2. */
