/*
 * FN_CYCM verification fixture.
 *
 * AdLint starts every function at one and adds one for each if-statement,
 * case label, while/do/for statement, logical && or || expression, and
 * conditional ?: expression.  else, default, switch itself, goto, break,
 * continue, and return do not add complexity.
 *
 * Expected function metrics:
 *     straight_line              = 1
 *     if_only                    = 2
 *     if_else                    = 2
 *     else_if_chain              = 4
 *     all_loop_forms             = 5
 *     switch_cases               = 4
 *     default_only_switch        = 1
 *     logical_operators          = 4
 *     logical_value_expression   = 3
 *     conditional_operator       = 2
 *     constant_decisions         = 4
 *     nondecision_jumps          = 1
 *     combined_decisions         = 10
 *     kandr_complexity           = 3
 */

static int straight_line(int value)
{
    value += 1;
    return value;
}
/* straight_line: FN_CYCM = 1. */

/* One if adds one to the function's base value. */
static int if_only(int value)
{
    if (value > 0) {               /* +1 */
        value -= 1;
    }

    return value;
}
/* if_only: FN_CYCM = 2. */

/* else introduces no additional decision point. */
static int if_else(int value)
{
    if (value > 0) {               /* +1 */
        value = 1;
    }
    else {                          /* +0 */
        value = -1;
    }

    return value;
}
/* if_else: FN_CYCM = 2. */

/* An else-if chain contains three distinct if-statement nodes. */
static int else_if_chain(int value)
{
    if (value < 0) {               /* +1 */
        return -1;
    }
    else if (value == 0) {         /* +1 */
        return 0;
    }
    else if (value == 1) {         /* +1 */
        return 1;
    }
    else {                          /* +0 */
        return 2;
    }
}
/* else_if_chain: FN_CYCM = 4. */

/* Each loop form contributes one, including both forms of for-statement. */
static int all_loop_forms(int limit)
{
    int i = 0;

    while (i < limit) {            /* +1 */
        ++i;
    }

    do {                            /* +1 */
        --i;
    } while (i > limit);

    for (i = 0; i < limit; ++i) { /* +1: traditional for */
        limit += 0;
    }

    for (int j = 0; j < limit; ++j) { /* +1: C99 for */
        limit += 0;
    }

    return i;
}
/* all_loop_forms: FN_CYCM = 5. */

/* switch adds zero; every case adds one; default adds zero. */
static int switch_cases(int selector)
{
    switch (selector) {            /* +0 */
    case 0:                         /* +1 */
        return 0;
    case 1:                         /* +1 */
    case 2:                         /* +1, even with fallthrough */
        return 1;
    default:                        /* +0 */
        return -1;
    }
}
/* switch_cases: FN_CYCM = 4. */

/* A switch containing only default has no counted decision node. */
static int default_only_switch(int selector)
{
    switch (selector) {            /* +0 */
    default:                        /* +0 */
        return selector;
    }
}
/* default_only_switch: FN_CYCM = 1. */

/* The if, &&, and || nodes each contribute one. */
static int logical_operators(int first, int second, int third)
{
    if ((first && second) || third) { /* +1 if, +1 &&, +1 || */
        return 1;
    }

    return 0;
}
/* logical_operators: FN_CYCM = 4. */

/* Logical operators count even when they are not an if/loop condition. */
static int logical_value_expression(int first, int second, int third)
{
    int result = (first && second) || third; /* +1 &&, +1 || */

    return result;
}
/* logical_value_expression: FN_CYCM = 3. */

/* A conditional-expression node contributes one decision. */
static int conditional_operator(int condition)
{
    return condition ? 1 : 0;      /* +1 */
}
/* conditional_operator: FN_CYCM = 2. */

/* FN_CYCM is syntactic: known truth values do not remove decisions. */
static int constant_decisions(void)
{
    int result = 0;

    if (1) {                       /* +1 */
        result = 1;
    }

    while (0) {                    /* +1 */
        result = 2;
    }

    result = 1 ? result : 3;       /* +1 */
    return result;
}
/* constant_decisions: FN_CYCM = 4. */

/* Labels and jump statements do not change cyclomatic complexity here. */
static int nondecision_jumps(int value)
{
    goto finished;                 /* +0 */

unused_label:
    value += 1;
    goto unused_label;             /* +0 */

finished:
    return value;                  /* +0 */
}
/* nondecision_jumps: FN_CYCM = 1. */

/*
 * Base 1 + while + if + && + two cases + nested if + ?: + for + || = 10.
 */
static int combined_decisions(int value, int first, int second)
{
    int result = 0;

    while (value > 0) {            /* +1 */
        if (first && second) {      /* +1 if, +1 && */
            break;
        }
        --value;
    }

    switch (value) {
    case 0:                         /* +1 */
        if (first) {                /* +1 */
            result = 1;
        }
        break;
    case 1:                         /* +1 */
        result = second ? 2 : 3;    /* +1 */
        break;
    default:
        for (; first || second; --first) { /* +1 for, +1 || */
            continue;
        }
        break;
    }

    return result;
}
/* combined_decisions: FN_CYCM = 10. */

/* K&R definitions receive the same base and decision increments. */
static int kandr_complexity(value)
    int value;
{
    if (value > 0) {               /* +1 */
        while (value > 1) {        /* +1 */
            --value;
        }
    }

    return value;
}
/* kandr_complexity: FN_CYCM = 3. */
