/*
 * FN_RETN verification fixture.
 *
 * AdLint counts explicit return-statement AST nodes and adds one implicit
 * return when a void function has a feasible path that falls through its end.
 * It does not add an implicit return for a non-void function.  Explicit return
 * statements are counted syntactically, including unreachable statements.
 *
 * Expected function metrics:
 *     void_empty                    = 1
 *     void_fallthrough              = 1
 *     void_explicit_final           = 1
 *     void_conditional_then_fallthrough = 2
 *     void_conditional_final_return     = 2
 *     void_infinite_loop                = 0
 *     nonvoid_single_return         = 1
 *     nonvoid_multiple_returns      = 3
 *     unreachable_return            = 2
 *     nonvoid_fallthrough           = 0
 *     kandr_returns                 = 2
 */

/* An empty void function has one implicit return point. */
static void void_empty(void)
{
}
/* void_empty: FN_RETN = 1 (implicit). */

/* A reachable final expression is followed by an implicit return. */
static void void_fallthrough(int *value)
{
    *value += 1;
}
/* void_fallthrough: FN_RETN = 1 (implicit). */

/* A final explicit return leaves no path to an implicit return. */
static void void_explicit_final(void)
{
    return;                         /* +1 explicit */
}
/* void_explicit_final: FN_RETN = 1. */

/* One branch returns; the remaining path reaches the implicit return. */
static void void_conditional_then_fallthrough(int value, int *output)
{
    if (value < 0) {
        return;                     /* +1 explicit */
    }

    *output = value;
    /* +1 implicit return */
}
/* void_conditional_then_fallthrough: FN_RETN = 2. */

/*
 * One branch returns while the other can reach the end of the void function.
 * Both the explicit and implicit return points therefore contribute.
 */
static void void_conditional_final_return(int value)
{
    if (value < 0) {
        return;                     /* +1 explicit */
    }
}
/* void_conditional_final_return: FN_RETN = 2. */

/* A loop whose condition is always true has no path to the function end. */
static void void_infinite_loop(void)
{
    while (1) {
    }
}
/* void_infinite_loop: FN_RETN = 0. */

static int nonvoid_single_return(int value)
{
    return value;                   /* +1 */
}
/* nonvoid_single_return: FN_RETN = 1. */

/* Every explicit return site in the selection contributes once. */
static int nonvoid_multiple_returns(int value)
{
    if (value < 0) {
        return -1;                  /* +1 */
    }
    if (value > 0) {
        return 1;                   /* +1 */
    }

    return 0;                       /* +1 */
}
/* nonvoid_multiple_returns: FN_RETN = 3. */

/* Reachability is not checked by count_return, so both nodes contribute. */
static int unreachable_return(int value)
{
    return value;                   /* +1 reachable */
    return 0;                       /* +1 unreachable */
}
/* unreachable_return: FN_RETN = 2. */

/* AdLint adds no implicit return for a non-void function. */
static int nonvoid_fallthrough(int *output)
{
    *output = 1;
}
/* nonvoid_fallthrough: FN_RETN = 0. */

/* Explicit returns in a K&R definition are measured normally. */
static int kandr_returns(value)
    int value;
{
    if (value < 0) {
        return -1;                  /* +1 */
    }

    return value;                   /* +1 */
}
/* kandr_returns: FN_RETN = 2. */
