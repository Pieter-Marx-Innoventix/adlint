/*
 * FN_UNUV verification fixture.
 *
 * AdLint tracks named parameters and local variables by declaration identity, so
 * shadowed declarations are independent. A declaration with no later use
 * contributes one; each later write whose value is not read contributes one.
 * Writes made on alternative branches remain live until a later read or write
 * determines which values can reach that point.
 *
 * Inline annotations identify each expected contribution: +1 marks one
 * unused declaration or unread write. +0 marks a non-contribution: a value is
 * consumed, may reach a read, or is a declaration initializer rather than a
 * separate write.
 *
 * Expected function metrics:
 *     all_used                 = 0
 *     unused_declarations      = 3
 *     assigned_not_read        = 1
 *     read_then_overwritten    = 1
 *     overwritten_then_read    = 0
 *     address_taken            = 0
 *     increment_only           = 1
 *     compound_then_read       = 0
 *     branch_assigned_then_read = 0
 *     switch_assigned_then_read = 0
 *     joined_value_used_in_later_branch = 0
 *     returned_branch_value_is_unused = 1
 *     sequential_dead_assignments = 2
 *     shadowed_variables         = 1
 *     unused_local_static      = 1
 *     kandr_unused_parameter   = 1
 */

/* Both the parameter and local variable are read. */
static int all_used(int input)
{
    int doubled = input * 2;       /* +0: reads `input`; `doubled` is read by return */

    return doubled;                /* +0: reads `doubled` */
}
/* all_used: FN_UNUV = 0. */

/* `unused_parameter`, `uninitialized`, and `initialized` each contribute +1. */
static int unused_declarations(int unused_parameter)
{
    int uninitialized;             /* +1: never referred */
    int initialized = 1;           /* +1: never referred */

    return 0;
}
/* unused_declarations: FN_UNUV = 3. */

/* The input is read, but the value assigned to output is never read. */
static int assigned_not_read(int input)
{
    int output;

    output = input + 1;            /* +1: assigned value is never read */
    return 0;
}
/* assigned_not_read: FN_UNUV = 1. */

/* The final assignment to value is never read. */
static int read_then_overwritten(int input)
{
    int value = input;             /* +0: initial value is read below */
    int observed = value;          /* +0: reads `value` */

    value = 2;                     /* +1: replacement value is never read */
    return observed;               /* +0: reads `observed` */
}
/* read_then_overwritten: FN_UNUV = 1 (`value`). */

/* The assignment value is read by return. */
static int overwritten_then_read(int input)
{
    int value = input;             /* +0: initializer is not a separate write */

    value = 2;                     /* +0: read by the return statement */
    return value;                  /* +0: reads `value` */
}
/* overwritten_then_read: FN_UNUV = 0. */

/* AdLint treats taking an address as a reference to the named variable. */
static int address_taken(int input)
{
    int local = input;             /* +0: referenced through `pointer` */
    int *pointer = &local;         /* +0: reads `local`'s address */

    return *pointer;               /* +0: reads the value through `pointer` */
}
/* address_taken: FN_UNUV = 0. */

/* Increment updates the variable; its resulting value is not subsequently read. */
static int increment_only(int input)
{
    int value = input;             /* +0: the initial value is read by ++ */

    value++;                       /* +1: updated value is never read */
    return 0;
}
/* increment_only: FN_UNUV = 1 (`value`). */

/* A read following a compound assignment makes the latest value useful. */
static int compound_then_read(int input)
{
    int value = input;             /* +0: read and replaced by += */

    value += 1;                    /* +0: updated value is returned */
    return value;                  /* +0: reads `value` */
}
/* compound_then_read: FN_UNUV = 0. */

/* Values assigned on either feasible branch are consumed after the join. */
static int branch_assigned_then_read(int condition)
{
    int value;

    if (condition) {
        value = 1;                 /* +0: read after the branch join */
    }
    else {
        value = 2;                 /* +0: read after the branch join */
    }

    return value;                  /* +0: consumes either branch value */
}
/* branch_assigned_then_read: FN_UNUV = 0. */

/* Values assigned by either switch arm are consumed after the join. */
static int switch_assigned_then_read(int selector)
{
    int value;

    switch (selector) {
    case 0:
        value = 1;                 /* +0: read after the switch */
        break;
    default:
        value = 2;                 /* +0: read after the switch */
        break;
    }

    return value;                  /* +0: consumes either switch value */
}
/* switch_assigned_then_read: FN_UNUV = 0. */

/* A later branch can consume either value produced by an earlier branch. */
static int joined_value_used_in_later_branch(int first_condition,
                                              int second_condition)
{
    int value;

    if (first_condition) {
        value = 1;                 /* +0: may be read in the next branch */
    }
    else {
        value = 2;                 /* +0: may be read in the next branch */
    }

    if (second_condition) {
        return value;              /* +0: consumes either joined value */
    }

    return 0;
}
/* joined_value_used_in_later_branch: FN_UNUV = 0. */

/* A value on a returning branch cannot be consumed after the branch joins. */
static int returned_branch_value_is_unused(int condition)
{
    int value = 0;                 /* +0: initializer is not a separate write */

    if (condition) {
        value = 1;                 /* +1: return below does not read it */
        return 0;
    }

    value = 2;                     /* +0: read by the return statement */
    return value;                  /* +0: reads `value` */
}
/* returned_branch_value_is_unused: FN_UNUV = 1. */

/* Each overwritten value is counted when no later read can consume it. */
static int sequential_dead_assignments(int input)
{
    int value = input;             /* +0: initializer is not a separate write */

    value = 1;                     /* +1: overwritten before any read */
    value = 2;                     /* +1: never read */
    return 0;
}
/* sequential_dead_assignments: FN_UNUV = 2. */

/* An inner declaration does not replace the outer variable's metric state. */
static int shadowed_variables(int input)
{
    int value = input;             /* +0: read by the final return */

    {
        int value = 1;             /* +1: inner declaration is never referred */
    }

    return value;                  /* +0: reads the outer `value` */
}
/* shadowed_variables: FN_UNUV = 1 (the inner `value`). */

/* A block-scope static variable is still tracked as a local definition. */
static int unused_local_static(void)
{
    static int cached = 1;         /* +1: no reference to this local static */

    return 0;
}
/* unused_local_static: FN_UNUV = 1 (`cached`). */

/* Parameters in K&R definitions are tracked in the same way. */
static int kandr_unused_parameter(used, unused)
    int used;
    int unused;                     /* +1: never referred */
{
    return used;                   /* +0: reads `used` */
}
/* kandr_unused_parameter: FN_UNUV = 1 (`unused`). */
