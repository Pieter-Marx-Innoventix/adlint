/*
 * FN_PATH verification fixture.
 *
 * AdLint counts statically possible execution paths reported by its abstract
 * interpreter.  Alternative branches add paths, sequential branch groups
 * multiply them, and a path ended by return does not continue into later
 * branch groups.  A loop is represented by entering or skipping its body;
 * arbitrary iteration counts are not enumerated.
 *
 * Expected function metrics:
 *     straight_line               = 1
 *     complete_if_else            = 2
 *     incomplete_if               = 2
 *     sequential_if_else          = 4
 *     three_independent_ifs       = 8
 *     nested_ifs                  = 3
 *     early_returns               = 3
 *     switch_alternatives         = 3
 *     switch_with_nested_branch   = 4
 *     while_entry                 = 2
 *     loop_with_early_returns     = 6
 *     conditional_expression      = 2
 *     constant_condition          = 1
 *     kandr_paths                 = 2
 */

static int straight_line(int value)
{
    value += 1;
    return value;
}
/* straight_line: FN_PATH = 1. */

/* The true and false arms are two alternative paths. */
static int complete_if_else(int value)
{
    if (value > 0) {
        value += 1;
    }
    else {
        value -= 1;
    }

    return value;
}
/* complete_if_else: FN_PATH = 2. */

/* An omitted else still supplies a false path which skips the body. */
static int incomplete_if(int value)
{
    if (value > 0) {
        value += 1;
    }

    return value;
}
/* incomplete_if: FN_PATH = 2. */

/* Two independent binary decisions multiply: 2 * 2 = 4. */
static int sequential_if_else(int first, int second)
{
    int result = 0;

    if (first) {
        result += 1;
    }
    else {
        result -= 1;
    }

    if (second) {
        result += 2;
    }
    else {
        result -= 2;
    }

    return result;
}
/* sequential_if_else: FN_PATH = 4. */

/* Three independent incomplete if-statements give 2 * 2 * 2 paths. */
static int three_independent_ifs(int first, int second, int third)
{
    int result = 0;

    if (first) {
        result += 1;
    }
    if (second) {
        result += 2;
    }
    if (third) {
        result += 4;
    }

    return result;
}
/* three_independent_ifs: FN_PATH = 8. */

/* Outer-false plus outer-true/inner-false and outer-true/inner-true. */
static int nested_ifs(int outer, int inner)
{
    int result = 0;

    if (outer) {
        if (inner) {
            result = 1;
        }
    }

    return result;
}
/* nested_ifs: FN_PATH = 3. */

/* Returned paths do not flow into the following if: 1 + 1 + 1, not 4. */
static int early_returns(int first, int second)
{
    if (first) {
        return 1;
    }
    if (second) {
        return 2;
    }

    return 0;
}
/* early_returns: FN_PATH = 3. */

/* Three mutually exclusive switch destinations supply three paths. */
static int switch_alternatives(int selector)
{
    int result;

    switch (selector) {
    case 0:
        result = 10;
        break;
    case 1:
        result = 20;
        break;
    default:
        result = 30;
        break;
    }

    return result;
}
/* switch_alternatives: FN_PATH = 3. */

/* The first switch arm has two subpaths; the other two have one each. */
static int switch_with_nested_branch(int selector, int condition)
{
    int result;

    switch (selector) {
    case 0:
        if (condition) {
            result = 1;
        }
        else {
            result = 2;
        }
        break;
    case 1:
        result = 3;
        break;
    default:
        result = 4;
        break;
    }

    return result;
}
/* switch_with_nested_branch: FN_PATH = 4. */

/* AdLint models a loop with an enter-body path and a skip-body path. */
static int while_entry(int value)
{
    while (value > 0) {
        --value;
    }

    return value;
}
/* while_entry: FN_PATH = 2. */

/*
 * Paths are: enter/first return; enter/second return; enter/leave followed by
 * either final result; and skip followed by either final result: six total.
 */
static int loop_with_early_returns(int value)
{
    while (value < 5) {
        if (value == 0) {
            return 0;
        }
        if (value == 1) {
            return 1;
        }
    }

    if (value == 2) {
        return 2;
    }

    return 3;
}
/* loop_with_early_returns: FN_PATH = 6. */

/* The second and third operands of ?: are alternative execution paths. */
static int conditional_expression(int condition)
{
    return condition ? 1 : 0;
}
/* conditional_expression: FN_PATH = 2. */

/* A compile-time constant condition leaves only its executable true path. */
static int constant_condition(void)
{
    int result = 0;

    if (1) {
        result = 1;
    }

    return result;
}
/* constant_condition: FN_PATH = 1. */

/* Old-style K&R definitions use the same branch-path accounting. */
static int kandr_paths(value)
    int value;
{
    if (value > 0) {
        return 1;
    }

    return 0;
}
/* kandr_paths: FN_PATH = 2. */
