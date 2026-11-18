/*
 * FN_NEST verification fixture.
 *
 * AdLint defines the function body's top level as zero.  Each if statement or
 * if/else-if/else chain, switch, while, do, and both forms of for increases
 * the level.  Braces, case/default labels, and plain compound blocks do not
 * contribute; braced and unbraced control bodies are equivalent.
 *
 * Expected function metrics:
 *     top_level_only             = 0
 *     single_braced_control      = 1
 *     nested_braced_controls     = 4
 *     sibling_control_forms      = 1
 *     nested_plain_blocks        = 0
 *     switch_body                = 1
 *     unbraced_single            = 1
 *     unbraced_chain             = 3
 *     sequential_unbraced        = 1
 *     mixed_if_else              = 1
 *     else_if_chain              = 1
 *     kandr_nesting              = 2
 */

static int top_level_only(int value)
{
    value += 1;
    return value;
}
/* top_level_only: FN_NEST = 0. */

static int single_braced_control(int value)
{
    if (value > 0) {
        value -= 1;                /* nesting level 1 */
    }

    return value;
}
/* single_braced_control: FN_NEST = 1. */

/* if -> for -> while -> do produces four nested control structures. */
static int nested_braced_controls(int value)
{
    int i;

    if (value > 0) {               /* level 1 */
        for (i = 0; i < value; ++i) { /* level 2 */
            while (i < value) {    /* level 3 */
                do {               /* level 4 */
                    ++i;
                } while (i < 0);
                break;
            }
        }
    }

    return value;
}
/* nested_braced_controls: FN_NEST = 4. */

/* Sibling constructs do not add their levels together when all use braces. */
static int sibling_control_forms(int value)
{
    if (value < 0) {
        value = -value;
    }
    else {
        value += 1;
    }

    for (int i = 0; i < value; ++i) {
        value += i;
    }

    while (value > 10) {
        --value;
    }

    return value;
}
/* sibling_control_forms: FN_NEST = 1. */

/* Plain compound blocks do not create control-structure nesting. */
static int nested_plain_blocks(int value)
{
    {
        {
            value += 1;
        }
    }

    return value;
}
/* nested_plain_blocks: FN_NEST = 0. */

/* A switch statement contributes one level; its labels do not. */
static int switch_body(int value)
{
    switch (value) {               /* body nesting level 1 */
    case 0:
        return 0;
    default:
        return 1;
    }
}
/* switch_body: FN_NEST = 1. */

/* A single unbraced control structure has nesting level one. */
static int unbraced_single(int value)
{
    if (value > 0)
        value -= 1;                /* level 1 from the `if`, with no extra body level */

    return value;
}
/* unbraced_single: FN_NEST = 1. */

/*
 * Nested unbraced controls have the same nesting as their braced equivalent.
 */
static int unbraced_chain(int first, int second, int third)
{
    if (first)                      /* level 1 */
        if (second)                 /* level 2 */
            if (third)              /* level 3 */
                return 1;

    return 0;
}
/* unbraced_chain: FN_NEST = 3. */

/* Sequential unbraced controls are siblings, so the maximum remains one. */
static int sequential_unbraced(int value)
{
    if (value < 0)
        value = 0;                 /* level 1 */
    while (value < 1)
        ++value;                   /* level 1 */
    for (; value < 2; ++value)
        ;                          /* level 1 */

    return value;
}
/* sequential_unbraced: FN_NEST = 1. */

/* Bracing either branch of an if-else does not change its nesting depth. */
static int mixed_if_else(int value)
{
    if (value < 0) {
        value = -value;
    }
    else
        value += 1;

    return value;
}
/* mixed_if_else: FN_NEST = 1. */

/* An if/else-if/else ladder is one control nesting level. */
static int else_if_chain(int value)
{
    if (value < 0) {               /* level 1 */
        return -1;
    }
    else if (value == 0) {         /* level 1: another arm of the chain */
        return 0;
    }
    else {
        return 1;
    }
}
/* else_if_chain: FN_NEST = 1. */

/* Compound depth in K&R definitions is measured normally. */
static int kandr_nesting(value)
    int value;
{
    if (value > 0) {               /* level 1 */
        while (value > 1) {        /* level 2 */
            --value;
        }
    }

    return value;
}
/* kandr_nesting: FN_NEST = 2. */
