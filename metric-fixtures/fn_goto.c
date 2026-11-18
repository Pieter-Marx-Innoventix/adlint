/*
 * FN_GOTO verification fixture.
 *
 * AdLint counts goto-statement AST nodes inside each ANSI or K&R function.
 * Labels do not contribute, and reachability or target identity does not alter
 * the result.
 *
 * Expected function metrics:
 *     no_goto              = 0
 *     forward_goto         = 1
 *     backward_goto        = 1
 *     repeated_target      = 3
 *     multiple_targets     = 3
 *     nested_gotos         = 2
 *     unreachable_goto     = 1
 *     kandr_goto           = 1
 */

static int no_goto(int value)
{
    return value;
}
/* no_goto: FN_GOTO = 0. */

/* A forward jump counts once; its destination label counts zero. */
static int forward_goto(int value)
{
    goto finished;                /* +1 */
    value = 0;

finished:
    return value;
}
/* forward_goto: FN_GOTO = 1. */

/* A backward jump is counted exactly like a forward jump. */
static int backward_goto(int value)
{
again:
    value -= 1;
    if (value > 0) {
        goto again;               /* +1 */
    }

    return value;
}
/* backward_goto: FN_GOTO = 1. */

/* Separate goto statements count even when they share one destination. */
static int repeated_target(int value)
{
    if (value < 0) {
        goto finished;            /* +1 */
    }
    if (value == 0) {
        goto finished;            /* +1 */
    }

    goto finished;                /* +1 */

finished:
    return value;
}
/* repeated_target: FN_GOTO = 3. */

/* Targeting different labels has no special effect on the count. */
static int multiple_targets(int value)
{
    if (value < 0) {
        goto negative;            /* +1 */
    }
    if (value > 0) {
        goto positive;            /* +1 */
    }
    goto zero;                    /* +1 */

negative:
    return -1;
positive:
    return 1;
zero:
    return 0;
}
/* multiple_targets: FN_GOTO = 3. */

/* Gotos nested in loop and switch control structures still count once each. */
static int nested_gotos(int value)
{
    while (value > 0) {
        if (value == 1) {
            goto one;             /* +1 */
        }

        switch (value) {
        case 2:
            goto two;             /* +1 */
        default:
            value -= 1;
            break;
        }
    }

    return 0;
one:
    return 1;
two:
    return 2;
}
/* nested_gotos: FN_GOTO = 2. */

/* FN_GOTO is syntactic, so even an unreachable goto node contributes. */
static int unreachable_goto(int value)
{
    return value;
    goto unreachable_label;       /* +1 */

unreachable_label:
    return 0;
}
/* unreachable_goto: FN_GOTO = 1. */

/* Gotos inside old-style K&R definitions are measured normally. */
static int kandr_goto(value)
    int value;
{
    goto finished;                /* +1 */

finished:
    return value;
}
/* kandr_goto: FN_GOTO = 1. */
