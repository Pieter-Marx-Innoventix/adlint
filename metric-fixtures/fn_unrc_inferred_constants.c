/*
 * FN_UNRC verification fixture: proven truth in inferred loop exits.
 * A false exit cannot make its break or a post-loop return reachable.
 * Unknown parameter conditions must retain their feasible exit paths.
 *
 * Expected function metrics:
 *     for_never_breaks = 2
 *     c99_never_breaks = 2
 *     while_never_breaks = 2
 *     do_never_breaks = 2
 *     for_always_breaks = 0
 *     if_else_false_exit = 2
 *     if_else_true_exit = 1
 *     literal_negated_exit = 2
 *     constant_arithmetic_exit = 2
 *     constant_comparison_exit = 2
 *     conditional_false_exit = 2
 *     unknown_exit = 0
 *     unknown_comparison_exit = 0
 *     assignment_exit = 0
 *     short_circuit_side_effect = 2
 *     constant_false_return = 2
 */
extern void consume(int value);

static int for_never_breaks(int flag)
{
    for (;;) { if (0 && flag) break; }
    return 0;
}

static int c99_never_breaks(int flag)
{
    for (int index = 0; ;) {
        if (0 && flag) break;
        consume(index);
    }
    return 0;
}

static int while_never_breaks(int flag)
{
    while (1) {
        if (0 && flag) break;
        ++flag;
    }
    return 0;
}

static int do_never_breaks(int flag)
{
    do {
        if (0 && flag) break;
        ++flag;
    } while (1);
    return 0;
}

static int for_always_breaks(int flag)
{
    for (;;) { if (1 || flag) break; }
    return 0;
}

static int if_else_false_exit(int flag)
{
    for (;;) {
        if (0 && flag) { break; }
        else { consume(1); }
    }
    return 0;
}

static int if_else_true_exit(int flag)
{
    for (;;) {
        if (1 || flag) { break; }
        else { consume(1); }
    }
    return 0;
}

static int literal_negated_exit(void)
{
    for (;;) { if (!1) break; }
    return 0;
}

static int constant_arithmetic_exit(void)
{
    for (;;) { if (2 - 2) break; }
    return 0;
}

static int constant_comparison_exit(void)
{
    for (;;) { if (2 < 1) break; }
    return 0;
}

static int conditional_false_exit(int flag)
{
    for (;;) { if (flag ? 0 : 0) break; }
    return 0;
}

static int unknown_exit(int flag)
{
    for (;;) { if (flag) break; }
    return 0;
}

static int unknown_comparison_exit(int flag)
{
    for (;;) { if (flag == 5) break; }
    return 0;
}

static int assignment_exit(int flag)
{
    for (;;) { if ((flag = 1)) break; }
    return 0;
}

static int short_circuit_side_effect(int flag)
{
    for (;;) { if (0 && ++flag) break; }
    return 0;
}

static int constant_false_return(int flag)
{
    for (;;) { if (0 && flag) return 0; }
    return 1;
}
