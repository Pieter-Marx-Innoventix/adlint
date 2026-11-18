/*
 * FN_UELS verification fixture.
 *
 * FN_UELS counts terminal `else if` statements that have no explicit final
 * `else` clause.  A standalone `if` without `else` does not contribute, and
 * each incomplete `else if` chain contributes once.
 *
 * Expected function metrics:
 *     no_selection                = 0
 *     standalone_if              = 0
 *     simple_if_else             = 0
 *     incomplete_else_if         = 1
 *     complete_else_if           = 0
 *     long_incomplete_chain      = 1
 *     two_incomplete_chains      = 2
 *     braced_else_with_if        = 0
 *     dangling_else_chain        = 1
 *     unreachable_incomplete     = 1
 *     kandr_incomplete_chain     = 1
 */

static int no_selection(int value)
{
    return value;
}
/* no_selection: FN_UELS = 0. */

/* A standalone if-statement is not counted by this implementation. */
static int standalone_if(int value)
{
    if (value < 0) {
        value = 0;
    }

    return value;
}
/* standalone_if: FN_UELS = 0. */

/* A conventional if-else has an explicit terminal else branch. */
static int simple_if_else(int value)
{
    if (value < 0) {
        return -1;
    }
    else {
        return 1;
    }
}
/* simple_if_else: FN_UELS = 0. */

/* The final `else if` has no terminating `else`. */
static int incomplete_else_if(int value)
{
    if (value < 0) {
        return -1;
    }
    else if (value > 0) {          /* +1: incomplete chain */
        return 1;
    }

    return 0;
}
/* incomplete_else_if: FN_UELS = 1. */

/* A terminal else completes the chain. */
static int complete_else_if(int value)
{
    if (value < 0) {
        return -1;
    }
    else if (value > 0) {
        return 1;
    }
    else {
        return 0;
    }
}
/* complete_else_if: FN_UELS = 0. */

/* Chain length does not matter: only its missing terminal else contributes. */
static int long_incomplete_chain(int value)
{
    if (value < -1) {
        return -2;
    }
    else if (value < 0) {
        return -1;
    }
    else if (value == 0) {         /* +1: incomplete chain */
        return 0;
    }

    return 1;
}
/* long_incomplete_chain: FN_UELS = 1. */

/* Two independent incomplete chains contribute independently. */
static int two_incomplete_chains(int first, int second)
{
    if (first < 0) {
        first = 0;
    }
    else if (first > 0) {          /* +1 */
        first = 1;
    }

    if (second < 0) {
        second = 0;
    }
    else if (second > 0) {         /* +1 */
        second = 1;
    }

    return first + second;
}
/* two_incomplete_chains: FN_UELS = 2. */

/* Braces make the else branch a compound statement, not an `else if` chain. */
static int braced_else_with_if(int value)
{
    if (value < 0) {
        value = -value;
    }
    else {
        if (value > 0) {
            value -= 1;
        }
    }

    return value;
}
/* braced_else_with_if: FN_UELS = 0. */

/* The else binds to the nearest unmatched if; that inner chain is incomplete. */
static int dangling_else_chain(int first, int second)
{
    if (first)
        if (second) {
            return 1;
        }
        else if (first > second) { /* +1: incomplete inner chain */
            return 2;
        }

    return 0;
}
/* dangling_else_chain: FN_UELS = 1. */

/* FN_UELS is structural, so unreachable syntax still contributes. */
static int unreachable_incomplete(int value)
{
    return value;

    if (value < 0) {
        return -1;
    }
    else if (value > 0) {          /* +1 */
        return 1;
    }
}
/* unreachable_incomplete: FN_UELS = 1. */

/* Incomplete chains in K&R definitions are measured normally. */
static int kandr_incomplete_chain(value)
    int value;
{
    if (value < 0) {
        return -1;
    }
    else if (value > 0) {          /* +1 */
        return 1;
    }

    return 0;
}
/* kandr_incomplete_chain: FN_UELS = 1. */
