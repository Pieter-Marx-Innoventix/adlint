/*
 * FN_UNUV verification fixture: do-loop control transfers.
 * A break leaves its own loop; a continue reaches the do condition;
 * a return leaves the function. Conditions skipped by break stay unread.
 *
 * Expected function metrics:
 *     inner_do_break_outer_read = 0
 *     do_break_then_read = 0
 *     do_continue_then_read = 0
 *     do_break_dead_write = 1
 *     do_return_dead_write = 1
 *     do_continue_condition_read = 0
 *     do_break_condition_unread = 1
 *     switch_break_inside_do = 0
 *     conditional_do_break = 0
 *     do_infinite_continue_dead_write = 1
 */
extern void consume(int value);

static int inner_do_break_outer_read(int limit)
{
    int counter = 0;
    int value = 0;
    while (counter < limit) {
        consume(value);
        do {
            value = 1;
            break;
        } while (1);
        ++counter;
    }
    return counter;
}

static int do_break_then_read(void)
{
    int value = 0;
    do {
        value = 1;
        break;
    } while (1);
    return value;
}

static int do_continue_then_read(void)
{
    int value = 0;
    do {
        value = 1;
        continue;
    } while (0);
    return value;
}

static int do_break_dead_write(void)
{
    int value = 0;
    do {
        value = 1;
        break;
    } while (1);
    return 0;
}

static int do_return_dead_write(void)
{
    int value = 0;
    do {
        value = 1;
        return 0;
    } while (0);
    consume(value); /* Unreachable: a return must still leave the function. */
    return 0;
}

static int do_continue_condition_read(int flag)
{
    do {
        continue;
    } while (flag);
    return 0;
}

static int do_break_condition_unread(int flag)
{
    do {
        break;
    } while (flag); /* An unconditional break skips this read. */
    return 0;
}

static int switch_break_inside_do(int tag)
{
    int value = 0;
    do {
        switch (tag) {
        default:
            value = 1;
            break; /* Leaves the switch, not the do-loop. */
        }
        consume(value);
        value = 2;
        break;
    } while (1);
    return value;
}

static int conditional_do_break(int flag)
{
    int value = 0;
    do {
        value = 1;
        if (flag) break;
        consume(value);
    } while (0);
    return value;
}

static int do_infinite_continue_dead_write(void)
{
    int value = 0;
    do {
        value = 1;
        continue;
    } while (1);
    consume(value); /* No exit: this must not make the assignment used. */
    return 0;
}
