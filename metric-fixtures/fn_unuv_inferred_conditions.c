/*
 * FN_UNUV verification fixture: inferred loop-breaking conditions.
 * Inferring a condition must not skip real reads, assignments or calls.
 * Unevaluated sizeof operands and short-circuited operands stay unread.
 *
 * Expected function metrics:
 *     inferred_control_reference = 0
 *     inferred_if_else_reference = 0
 *     inferred_negated_reference = 0
 *     inferred_logical_reference = 0
 *     inferred_assignment_read = 0
 *     inferred_call_reference = 0
 *     inferred_sizeof_unread = 1
 *     inferred_short_circuit_unread = 1
 *     while_inferred_logical_reference = 0
 *     c99_inferred_control_reference = 0
 */
extern void consume(int value);
extern int should_stop(int value);

static int inferred_control_reference(int flag)
{
    for (;;) {
        if (flag) break;
    }
    return 0;
}

static int inferred_if_else_reference(int flag)
{
    for (;;) {
        if (flag) {
            break;
        } else {
            consume(1);
        }
    }
    return 0;
}

static int inferred_negated_reference(int flag)
{
    for (;;) {
        if (!flag) break;
    }
    return 0;
}

static int inferred_logical_reference(int first, int second)
{
    for (;;) {
        if (first && second) break;
    }
    return 0;
}

static int inferred_assignment_read(int flag)
{
    int value = 0;
    for (;;) {
        if ((value = flag)) break;
    }
    return value;
}

static int inferred_call_reference(int flag)
{
    for (;;) {
        if (should_stop(flag)) break;
    }
    return 0;
}

static int inferred_sizeof_unread(int flag)
{
    for (;;) {
        if (sizeof(flag)) break;
    }
    return 0;
}

static int inferred_short_circuit_unread(int flag)
{
    for (;;) {
        if (0 && flag) break;
    }
    return 0;
}

static int while_inferred_logical_reference(int first, int second)
{
    while (1) {
        if (first && second) break;
        first = 1;
    }
    return 0;
}

static int c99_inferred_control_reference(int flag)
{
    for (int counter = 0; ;) {
        if (flag) break;
        consume(counter);
    }
    return 0;
}
