/*
 * FN_UNUV verification fixture: while/for loop-local breaks.
 * Reads after an exited loop use its assignments. Break skips a for-increment;
 * return and continue must retain their existing control-transfer meaning.
 *
 * Expected function metrics:
 *     while_break_then_read = 0
 *     for_break_then_read = 0
 *     c99_for_break_then_read = 0
 *     while_break_dead_write = 1
 *     for_break_dead_write = 1
 *     for_break_skips_increment = 1
 *     c99_break_skips_increment = 1
 *     nested_while_break_outer_read = 0
 *     nested_for_break_outer_read = 0
 *     while_return_dead_write = 1
 *     for_return_dead_write = 1
 *     while_continue_dead_write = 1
 *     for_continue_dead_write = 1
 *     switch_break_then_loop_break = 0
 *     conditional_while_break = 0
 *     conditional_for_break = 0
 */
extern void consume(int value);

static int while_break_then_read(void)
{
    int value = 0;
    while (1) { value = 1; break; }
    return value;
}

static int for_break_then_read(void)
{
    int value = 0;
    for (;;) { value = 1; break; }
    return value;
}

static int c99_for_break_then_read(void)
{
    int value = 0;
    for (int index = 0; ;) { value = index; break; }
    return value;
}

static int while_break_dead_write(void)
{
    int value = 0;
    while (1) { value = 1; break; }
    return 0;
}

static int for_break_dead_write(void)
{
    int value = 0;
    for (;;) { value = 1; break; }
    return 0;
}

static int for_break_skips_increment(int flag)
{
    for (;; consume(flag)) { break; }
    return 0;
}

static int c99_break_skips_increment(int flag)
{
    for (int index = 0; ; consume(flag)) { consume(index); break; }
    return 0;
}

static int nested_while_break_outer_read(int limit)
{
    int counter = 0;
    int value = 0;
    while (counter < limit) {
        consume(value);
        while (1) { value = 1; break; }
        ++counter;
    }
    return counter;
}

static int nested_for_break_outer_read(int limit)
{
    int counter = 0;
    int value = 0;
    while (counter < limit) {
        consume(value);
        for (;;) { value = 1; break; }
        ++counter;
    }
    return counter;
}

static int while_return_dead_write(void)
{
    int value = 0;
    value = 1;
    while (1) { return 0; }
    consume(value); /* A return must still leave the function. */
    return 0;
}

static int for_return_dead_write(void)
{
    int value = 0;
    value = 1;
    for (;;) { return 0; }
    consume(value);
    return 0;
}

static int while_continue_dead_write(void)
{
    int value = 0;
    value = 1;
    while (1) { continue; }
    consume(value); /* The endless loop has no exit. */
    return 0;
}

static int for_continue_dead_write(void)
{
    int value = 0;
    value = 1;
    for (;;) { continue; }
    consume(value);
    return 0;
}

static int switch_break_then_loop_break(int tag)
{
    int value = 0;
    while (1) {
        switch (tag) {
        default: value = 1; break;
        }
        consume(value);
        value = 2;
        break;
    }
    return value;
}

static int conditional_while_break(int flag)
{
    int value = 0;
    while (1) {
        if (flag) { value = 1; break; }
        else { value = 2; break; }
    }
    return value;
}

static int conditional_for_break(int flag)
{
    int value = 0;
    for (;;) {
        if (flag) { value = 1; break; }
        else { value = 2; break; }
    }
    return value;
}
