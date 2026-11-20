/*
 * FN_UNUV loop verification fixture: values read on later loop iterations.
 * Expected values follow the intended unread-assignment definition.
 * An assignment that can reach a later read should not count as unused.
 * Declaration initializers are not separate writes in this metric.
 *
 * Expected function metrics:
 *     while_counter                = 0
 *     for_counter                  = 0
 *     do_counter                   = 0
 *     while_counter_returned       = 0
 *     next_iteration_body_read     = 0
 *     genuine_dead_write           = 1
 *     overwritten_before_next_read = 1
 *     break_before_next_read       = 1
 *     continue_counter             = 0
 *     for_body_read                  = 0
 *     for_continue_body_read         = 0
 *     for_increment_overwrites       = 1
 *     automatic_local_recreated      = 1
 *     static_local_carried           = 0
 *     shadowed_read_is_separate      = 1
 *     conditional_next_read          = 0
 *     both_branches_overwrite        = 1
 *     returning_branch_write         = 1
 *     conditional_break              = 0
 *     nested_loop_read               = 0
 *     inner_break_outer_read         = 0
 *     switch_break_stays_in_loop     = 0
 *     do_single_iteration            = 1
 *     do_later_body_read             = 0
 *     compound_update_read           = 0
 *     condition_overwrites_value     = 1
 *     goto_exits_loop                = 1
 *     for_continue_increment_reads     = 0
 *     for_continue_increment_update    = 0
 *     for_increment_write_then_read    = 1
 *     c99_for_body_read                = 0
 *     switch_continue_stays_in_loop    = 0
 *     do_break_dead_write              = 1
 *     for_without_condition            = 0
 *     for_initializer_runs_once        = 0
 */

extern void consume(int value);

static int while_counter(int limit)
{
    int counter = 0;
    while (counter < limit) {
        ++counter;
    }
    return 0;
}

static int for_counter(int limit)
{
    int counter;
    for (counter = 0; counter < limit; ++counter) {
    }
    return 0;
}

static int do_counter(int limit)
{
    int counter = 0;
    do {
        ++counter;
    } while (counter < limit);
    return 0;
}

static int while_counter_returned(int limit)
{
    int counter = 0;
    while (counter < limit) {
        ++counter;
    }
    return counter;
}

static int next_iteration_body_read(int limit)
{
    int counter = 0;
    int previous = 0;
    while (counter < limit) {
        consume(previous);
        previous = counter;
        ++counter;
    }
    return counter;
}

static int genuine_dead_write(int limit)
{
    int counter = 0;
    int unused = 0;
    while (counter < limit) {
        unused = 1;
        ++counter;
    }
    return counter;
}

static int overwritten_before_next_read(int limit)
{
    int counter = 0;
    int value = 0;
    while (counter < limit) {
        value = 0;
        consume(value);
        value = 1;
        ++counter;
    }
    return counter;
}

static int break_before_next_read(void)
{
    int value = 0;
    while (1) {
        value = 1;
        break;
    }
    return 0;
}

static int continue_counter(int limit)
{
    int counter = 0;
    while (counter < limit) {
        ++counter;
        continue;
    }
    return 0;
}

static int for_body_read(int limit)
{
    int counter;
    int value = 0;
    for (counter = 0; counter < limit; ++counter) {
        consume(value);
        value = counter;
    }
    return 0;
}
/* for_body_read: FN_UNUV = 0. */

static int for_continue_body_read(int limit)
{
    int counter;
    int value = 0;
    for (counter = 0; counter < limit; ++counter) {
        consume(value);
        value = counter;
        continue;
    }
    return 0;
}
/* for_continue_body_read: FN_UNUV = 0. */

static int for_increment_overwrites(int limit)
{
    int counter = 0;
    int value = 0;
    for (; counter < limit; value = 0) {
        consume(value);
        value = 1;
        ++counter;
        continue;
    }
    return counter;
}
/* for_increment_overwrites: FN_UNUV = 1. */

static int automatic_local_recreated(int limit)
{
    int counter = 0;
    while (counter < limit) {
        int value = 0;
        consume(value);
        value = 1;
        ++counter;
    }
    return counter;
}
/* automatic_local_recreated: FN_UNUV = 1. */

static int static_local_carried(int limit)
{
    int counter = 0;
    while (counter < limit) {
        static int value = 0;
        consume(value);
        value = 1;
        ++counter;
    }
    return counter;
}
/* static_local_carried: FN_UNUV = 0. */

static int shadowed_read_is_separate(int limit)
{
    int counter = 0;
    int value = 0;
    while (counter < limit) {
        {
            int value = 0;
            consume(value);
        }
        value = 1;
        ++counter;
    }
    return counter;
}
/* shadowed_read_is_separate: FN_UNUV = 1. */

static int conditional_next_read(int limit, int flag)
{
    int counter = 0;
    int value = 0;
    while (counter < limit) {
        if (flag) consume(value);
        value = 1;
        ++counter;
    }
    return counter;
}
/* conditional_next_read: FN_UNUV = 0. */

static int both_branches_overwrite(int limit, int flag)
{
    int counter = 0;
    int value = 0;
    while (counter < limit) {
        if (flag) {
            value = 0;
            consume(value);
        } else {
            value = 0;
            consume(value);
        }
        value = 1;
        ++counter;
    }
    return counter;
}
/* both_branches_overwrite: FN_UNUV = 1. */

static int returning_branch_write(int limit, int flag)
{
    int counter = 0;
    int value = 0;
    while (counter < limit) {
        consume(value);
        if (flag) {
            value = 1;
            return 0;
        }
        value = 2;
        ++counter;
    }
    return counter;
}
/* returning_branch_write: FN_UNUV = 1. */

static int conditional_break(int limit, int flag)
{
    int counter = 0;
    int value = 0;
    while (counter < limit) {
        consume(value);
        value = 1;
        if (flag) break;
        ++counter;
    }
    return counter;
}
/* conditional_break: FN_UNUV = 0. */

static int nested_loop_read(int limit)
{
    int outer = 0;
    int inner;
    int value = 0;
    while (outer < limit) {
        inner = 0;
        while (inner < limit) {
            consume(value);
            ++inner;
        }
        value = 1;
        ++outer;
    }
    return outer;
}
/* nested_loop_read: FN_UNUV = 0. */

static int inner_break_outer_read(int limit, int flag)
{
    int counter = 0;
    int value = 0;
    while (counter < limit) {
        consume(value);
        while (flag) {
            value = 1;
            break;
        }
        ++counter;
    }
    return counter;
}
/* inner_break_outer_read: FN_UNUV = 0. */

static int switch_break_stays_in_loop(int limit, int selector)
{
    int counter = 0;
    int value = 0;
    while (counter < limit) {
        consume(value);
        switch (selector) {
        case 0: value = 1; break;
        default: value = 2; break;
        }
        ++counter;
    }
    return counter;
}
/* switch_break_stays_in_loop: FN_UNUV = 0. */

static int do_single_iteration(void)
{
    int value = 0;
    do {
        consume(value);
        value = 1;
    } while (0);
    return 0;
}
/* do_single_iteration: FN_UNUV = 1. */

static int do_later_body_read(int limit)
{
    int counter = 0;
    int value = 0;
    do {
        consume(value);
        value = 1;
        ++counter;
    } while (counter < limit);
    return 0;
}
/* do_later_body_read: FN_UNUV = 0. */

static int compound_update_read(int limit)
{
    int counter = 0;
    int value = 0;
    while (counter < limit) {
        value += 1;
        ++counter;
    }
    return counter;
}
/* compound_update_read: FN_UNUV = 0. */

static int condition_overwrites_value(int limit)
{
    int counter = 0;
    int value = 0;
    while ((value = 0, counter < limit)) {
        consume(value);
        value = 1;
        ++counter;
    }
    return counter;
}
/* condition_overwrites_value: FN_UNUV = 1. */

static int goto_exits_loop(int limit)
{
    int counter = 0;
    int value = 0;
    while (counter < limit) {
        consume(value);
        value = 1;
        goto finished;
    }
finished:
    return counter;
}
/* goto_exits_loop: FN_UNUV = 1. */

static int for_continue_increment_reads(int limit)
{
    int counter;
    int value = 0;
    for (counter = 0; counter < limit; consume(value), ++counter) {
        value = counter;
        continue;
    }
    return 0;
}
/* for_continue_increment_reads: FN_UNUV = 0. */

static int for_continue_increment_update(int limit)
{
    int counter;
    for (counter = 0; counter < limit; ++counter) {
        ++counter;
        continue;
    }
    return 0;
}
/* for_continue_increment_update: FN_UNUV = 0. */

static int for_increment_write_then_read(int limit)
{
    int counter;
    int value = 0;
    for (counter = 0; counter < limit; value = 0, consume(value), ++counter) {
        consume(value);
        value = 1;
        continue;
    }
    return 0;
}
/* for_increment_write_then_read: FN_UNUV = 1. */

static int c99_for_body_read(int limit)
{
    int value = 0;
    for (int counter = 0; counter < limit; ++counter) {
        consume(value);
        value = counter;
    }
    return 0;
}
/* c99_for_body_read: FN_UNUV = 0. */

static int switch_continue_stays_in_loop(int limit, int selector)
{
    int counter = 0;
    int value = 0;
    while (counter < limit) {
        consume(value);
        switch (selector) {
        case 0:
            value = 1;
            ++counter;
            continue;
        default:
            value = 2;
            break;
        }
        ++counter;
    }
    return counter;
}
/* switch_continue_stays_in_loop: FN_UNUV = 0. */


static int do_break_dead_write(void)
{
    int value = 0;
    do {
        consume(value);
        value = 1;
        break;
    } while (1);
    return 0;
}
/* do_break_dead_write: FN_UNUV = 1. */

static int for_without_condition(int flag)
{
    int value = 0;
    consume(flag); /* Isolate loop-carried values from inferred-control references. */
    for (;;) {
        consume(value);
        value = 1;
        if (flag) break;
    }
    return 0;
}
/* for_without_condition: FN_UNUV = 0. */

static int for_initializer_runs_once(int flag)
{
    int value = 0;
    for (value = 1; 1;) {
        consume(value);
        value = 2;
        if (flag) break;
    }
    return 0;
}
/* for_initializer_runs_once: FN_UNUV = 0. */
