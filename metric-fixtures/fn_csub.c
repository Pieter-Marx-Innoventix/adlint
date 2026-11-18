/*
 * FN_CSUB verification fixture.
 *
 * AdLint counts function-call-expression AST nodes inside each ANSI or K&R
 * function definition.  It counts call sites, not distinct callees, and does
 * not require that a call be evaluated at runtime.
 *
 * Expected function metrics:
 *     no_calls                 = 0
 *     one_direct_call          = 1
 *     repeated_call_sites      = 3
 *     nested_calls             = 4
 *     indirect_call            = 1
 *     member_pointer_call      = 1
 *     returned_pointer_call    = 2
 *     conditional_calls        = 3
 *     comma_calls              = 2
 *     unevaluated_sizeof_call  = 1
 *     recursive_call           = 1
 *     kandr_call               = 1
 */

extern int unary(int value);
extern int binary(int left, int right);
extern int (*callback_provider(void))(int value);

struct operations {
    int (*apply)(int value);
};

static int no_calls(int value)
{
    return value + 1;
}
/* no_calls: FN_CSUB = 0. */

static int one_direct_call(int value)
{
    return unary(value);                       /* +1 */
}
/* one_direct_call: FN_CSUB = 1. */

/* Repeated calls to the same function count as separate call sites. */
static int repeated_call_sites(int value)
{
    value = unary(value);                      /* +1 */
    value = unary(value);                      /* +1 */
    return unary(value);                       /* +1 */
}
/* repeated_call_sites: FN_CSUB = 3. */

/* The outer call and every argument call are independent AST nodes. */
static int nested_calls(int value)
{
    return unary(binary(unary(value), unary(value)));
    /*     +1     +1     +1            +1          */
}
/* nested_calls: FN_CSUB = 4. */

/* A call through a function-pointer parameter still counts once. */
static int indirect_call(int (*callback)(int), int value)
{
    return callback(value);                    /* +1 */
}
/* indirect_call: FN_CSUB = 1. */

/* Calling a function pointer stored in an aggregate member counts once. */
static int member_pointer_call(const struct operations *ops, int value)
{
    return ops->apply(value);                  /* +1 */
}
/* member_pointer_call: FN_CSUB = 1. */

/* The provider call and invocation of its returned pointer both count. */
static int returned_pointer_call(int value)
{
    return callback_provider()(value);         /* +2 */
}
/* returned_pointer_call: FN_CSUB = 2. */

/* Calls in both conditional outcomes count, as does the short-circuit call. */
static int conditional_calls(int value)
{
    return (value && unary(value))
        ? unary(value)
        : binary(value, value);                /* +3 total */
}
/* conditional_calls: FN_CSUB = 3. */

/* Both operands of a comma expression contain distinct call sites. */
static int comma_calls(int value)
{
    return (unary(value), unary(value));        /* +2 */
}
/* comma_calls: FN_CSUB = 2. */

/* sizeof does not evaluate its expression, but the call AST node exists. */
static int unevaluated_sizeof_call(int value)
{
    return (int)sizeof(unary(value));           /* +1 */
}
/* unevaluated_sizeof_call: FN_CSUB = 1. */

/* A recursive call is counted like any other call expression. */
static int recursive_call(int value)
{
    if (value <= 0) {
        return 0;
    }

    return recursive_call(value - 1);           /* +1 */
}
/* recursive_call: FN_CSUB = 1. */

/* Calls inside K&R definitions are measured normally. */
static int kandr_call(value)
    int value;
{
    return unary(value);                        /* +1 */
}
/* kandr_call: FN_CSUB = 1. */
