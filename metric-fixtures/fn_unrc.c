/*
 * FN_UNRC verification fixture.
 *
 * AdLint counts C statements that cannot be reached on any feasible
 * control-flow path.  Declarations and the internal expression-statement
 * nodes used for a for-loop header are excluded.  A dead control statement
 * and each dead statement nested beneath it are counted independently.
 *
 * Expected function metrics:
 *     all_reachable              = 0
 *     after_return               = 2
 *     unreachable_subtree        = 2
 *     constant_branches          = 2
 *     contradictory_condition    = 1
 *     after_break                = 1
 *     after_continue             = 1
 *     constrained_switch         = 3
 *     forward_goto               = 1
 *     backward_goto              = 0
 *     conditional_goto           = 0
 *     unreachable_for            = 2
 *     after_infinite_loop        = 1
 *     after_all_continue         = 1
 *     before_first_case          = 1
 *     unbraced_switch_case       = 0
 *     unbraced_switch_default    = 0
 *     unbraced_switch_label_chain = 0
 *     unbraced_switch_unlabelled = 1
 *     kandr_after_return         = 1
 */

/* Both branches are feasible for an unconstrained parameter. */
static int all_reachable(int value)
{
    if (value > 0) {
        value += 1;
    }
    else {
        value -= 1;
    }

    return value;
}
/* all_reachable: FN_UNRC = 0. */

/* Neither statement following the unconditional return can execute. */
static int after_return(int value)
{
    return value;
    value += 1;       /* +1: unreachable expression-statement */
    return value;     /* +1: unreachable return-statement */
}
/* after_return: FN_UNRC = 2. */

/* The dead if-statement and its dead body statement are counted separately. */
static int unreachable_subtree(int value)
{
    return value;

    if (value != 0) { /* +1: unreachable if-statement */
        value = 0;    /* +1: unreachable expression-statement */
    }
}
/* unreachable_subtree: FN_UNRC = 2. */

/* Exercise compile-time false and true branch elimination. */
static int constant_branches(int value)
{
    if (0) {
        value = 1;    /* +1: unreachable expression-statement */
    }

    if (1) {
        value = 2;
    }
    else {
        value = 3;    /* +1: unreachable expression-statement */
    }

    return value;
}
/* constant_branches: FN_UNRC = 2. */

/* The outer condition constrains value for the nested condition. */
static int contradictory_condition(int value)
{
    if (value > 0) {
        if (value <= 0) {
            value = 1; /* +1: unreachable expression-statement */
        }
    }

    return value;
}
/* contradictory_condition: FN_UNRC = 1. */

/* A break terminates the current loop-body path. */
static int after_break(int value)
{
    while (value > 0) {
        break;
        value -= 1;   /* +1: unreachable expression-statement */
    }

    return value;
}
/* after_break: FN_UNRC = 1. */

/* A continue likewise terminates the current loop-body path. */
static int after_continue(int value)
{
    while (value > 0) {
        continue;
        value -= 1;   /* +1: unreachable expression-statement */
    }

    return value;
}
/* after_continue: FN_UNRC = 1. */

/* value is known to be negative upon entering the switch. */
static int constrained_switch(int value)
{
    if (value < 0) {
        switch (value) {
        case -1:
            value = 0;
            break;
        case 1:       /* +1: unreachable case-labeled-statement */
            value = 1;/* +1: unreachable expression-statement */
            break;    /* +1: unreachable break-statement */
        default:
            value = 0;
            break;
        }
    }

    return value;
}
/* constrained_switch: FN_UNRC = 3. */

/* An unconditional forward goto prevents lexical fall-through. */
static int forward_goto(int value)
{
    goto finished;
    value = 1;       /* +1: unreachable expression-statement */

finished:
    return value;
}
/* forward_goto: FN_UNRC = 1. */

/* A backward goto forms a reachable cycle without confusing the worklist. */
static int backward_goto(int value)
{
again:
    value -= 1;
    if (value > 0) {
        goto again;
    }
    return value;
}
/* backward_goto: FN_UNRC = 0. */

/* Both the jump and non-jump paths are feasible. */
static int conditional_goto(int value)
{
    if (value > 0) {
        goto finished;
    }
    value = 1;

finished:
    return value;
}
/* conditional_goto: FN_UNRC = 0. */

/* For-header expressions are not separate statements in FN_STMT/FN_UNRC. */
static int unreachable_for(int value)
{
    return value;
    for (value = 0; value < 3; ++value) { /* +1: unreachable for-statement */
        value += 1;                       /* +1: unreachable expression */
    }
}
/* unreachable_for: FN_STMT = 3 and FN_UNRC = 2. */

/* A definitely infinite loop has no fall-through edge. */
static int after_infinite_loop(void)
{
    while (1) {
    }
    return 1;          /* +1: unreachable return-statement */
}
/* after_infinite_loop: FN_UNRC = 1. */

/* Every branch transfers control to the next loop iteration. */
static int after_all_continue(int value)
{
    while (value != 0) {
        if (value > 0) {
            continue;
        }
        else {
            continue;
        }
        value = 0;     /* +1: unreachable expression-statement */
    }
    return value;
}
/* after_all_continue: FN_UNRC = 1. */

/* Switch entry jumps to a case/default label, not the first block item. */
static int before_first_case(int value)
{
    switch (value) {
        value = 99;    /* +1: unreachable expression-statement */
    case 0:
        value = 1;
        break;
    default:
        break;
    }
    return value;
}
/* before_first_case: FN_UNRC = 1. */

/* A switch body may be a directly labelled statement without braces. */
static int unbraced_switch_case(int value)
{
    switch (value)
    case 0:
        return 0;

    return 1;
}
/* unbraced_switch_case: FN_UNRC = 0. */

/* A directly labelled default clause is valid without a compound body. */
static int unbraced_switch_default(int value)
{
    switch (value)
    default:
        return value;
}
/* unbraced_switch_default: FN_UNRC = 0. */

/* Consecutive unbraced labels are nested labelled statements. */
static int unbraced_switch_label_chain(int value)
{
    switch (value)
    case 0:
    default:
        return value;
}
/* unbraced_switch_label_chain: FN_UNRC = 0. */

/* Without a clause label, an unbraced switch body cannot be entered. */
static int unbraced_switch_unlabelled(int value)
{
    switch (value)
        return 0;

    return value;
}
/* unbraced_switch_unlabelled: FN_UNRC = 1. */

/* Verify that reachability is measured inside a K&R definition as well. */
static int kandr_after_return(value)
    int value;
{
    return value;
    value = 0;        /* +1: unreachable expression-statement */
}
/* kandr_after_return: FN_UNRC = 1. */
