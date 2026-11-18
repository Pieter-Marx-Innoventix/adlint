/*
 * FN_STMT verification fixture.
 *
 * AdLint counts the same statement AST nodes as FL_STMT, but reports an
 * independent value for every ANSI or K&R function definition.
 *
 * Expected function metrics:
 *     declarations_only       = 0
 *     expression_statements   = 3
 *     selections              = 5
 *     iterations              = 10
 *     switch_and_labels       = 6
 *     generic_label_and_jumps = 3
 *     kandr_statements        = 2
 *
 * Cross-check:
 *     sum(FN_STMT) = 29 = FL_STMT for the equivalent fl_stmt.c fixture
 */

/* Declarations, initializers, and a compound function body count as zero. */
static void declarations_only(void)
{
    int value = 0;
    struct point {
        int x;
        int y;
    } origin = { 0, 0 };
}
/* declarations_only: FN_STMT = 0. */

/* Ordinary and empty expression-statements each count once. */
static void expression_statements(int *value)
{
    *value = 1;       /* +1: expression-statement */
    ++*value;         /* +1: expression-statement */
    ;                 /* +1: empty expression-statement */
}
/* expression_statements: FN_STMT = 3. */

/* Selection nodes and their controlled statements are separate nodes. */
static void selections(int value)
{
    if (value > 0)    /* +1: if-statement */
        --value;      /* +1: expression-statement */

    if (value == 0) { /* +1: if-else-statement */
        ;             /* +1: empty expression-statement */
    }
    else {
        value = 0;    /* +1: expression-statement */
    }
}
/* selections: FN_STMT = 5. */

/* Exercise every valid iteration-statement hook. */
static void iterations(int limit)
{
    int i = 0;

    while (i < limit) {                   /* +1: while-statement */
        continue;                         /* +1: continue-statement */
    }

    do {                                  /* +1: do-statement */
        ;                                 /* +1: empty expression-statement */
    } while (0);

    for (i = 0; i < limit; ++i) {         /* +1: complete for-statement */
        break;                            /* +1: break-statement */
    }

    for (int j = 0; j < limit; ++j) {     /* +1: complete C99 for-statement */
        ;                                 /* +1: empty expression-statement */
    }

    for (;;) {                            /* +1: complete for-statement */
        break;                            /* +1: break-statement */
    }
}
/* iterations: FN_STMT = 10. */

/* switch, case/default labels, and their substatements all count separately. */
static void switch_and_labels(int value)
{
    switch (value) {          /* +1: switch-statement */
    case 0:                   /* +1: case-labeled-statement */
        value = 1;            /* +1: expression-statement */
        break;                /* +1: break-statement */
    default:                  /* +1: default-labeled-statement */
        break;                /* +1: break-statement */
    }
}
/* switch_and_labels: FN_STMT = 6. */

/* A generic label and its contained jump-statement are distinct nodes. */
static int generic_label_and_jumps(int value)
{
    goto finished;            /* +1: goto-statement */

finished:                     /* +1: generic labeled-statement */
    return value;             /* +1: return-statement */
}
/* generic_label_and_jumps: FN_STMT = 3. */

/* Statements in an old-style K&R definition contribute normally. */
static int kandr_statements(value)
    int value;
{
    value += 1;               /* +1: expression-statement */
    return value;             /* +1: return-statement */
}
/* kandr_statements: FN_STMT = 2. */
