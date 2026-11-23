/*
 * FN_UNRC verification fixture: negation of known scalar truth.
 * Zero negates to true; nonzero negates to false, including open ranges
 * and unions. Unconstrained operands must keep both outcomes feasible.
 *
 * Expected function metrics:
 *     negated_known_true = 1
 *     negated_known_false = 1
 *     double_true_dead_else = 1
 *     triple_true_dead_then = 1
 *     negative_domain = 1
 *     positive_domain = 1
 *     nonzero_domain = 1
 *     zero_domain = 1
 *     unknown_operand = 0
 *     positive_range = 1
 *     negated_true_return = 1
 *     negated_loop_exit = 2
 */
extern void consume(int value);

static int negated_known_true(int flag)
{
    if (!(1 || flag)) consume(1);
    return 0;
}

static int negated_known_false(int flag)
{
    if (!(0 && flag)) consume(1);
    else consume(2);
    return 0;
}

static int double_true_dead_else(int flag)
{
    if (!!(1 || flag)) consume(1);
    else consume(2);
    return 0;
}

static int triple_true_dead_then(int flag)
{
    if (!!!(1 || flag)) consume(1);
    return 0;
}

static int negative_domain(int value)
{
    if (value < 0) {
        if (!value) consume(1);
    }
    return 0;
}

static int positive_domain(int value)
{
    if (value > 0) {
        if (!value) consume(1);
    }
    return 0;
}

static int nonzero_domain(int value)
{
    if (value != 0) {
        if (!value) consume(1);
    }
    return 0;
}

static int zero_domain(int value)
{
    if (value == 0) {
        if (!value) consume(1);
        else consume(2);
    }
    return 0;
}

static int unknown_operand(int value)
{
    if (!value) consume(1);
    else consume(2);
    return 0;
}

static int positive_range(int value)
{
    if (value > 0 && value < 5) {
        if (!value) consume(1);
    }
    return 0;
}

static int negated_true_return(int flag)
{
    if (!(1 || flag)) return 1;
    return 0;
}

static int negated_loop_exit(int flag)
{
    for (;;) { if (!(1 || flag)) break; }
    return 0;
}
