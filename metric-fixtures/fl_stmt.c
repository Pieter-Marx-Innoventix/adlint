/*
 * FL_STMT verification fixture.
 *
 * AdLint counts statement AST nodes in the source file.  Compound statements
 * (braces), declarations, and function definitions do not contribute.
 *
 * Expected file metric:
 *     FL_STMT = 29
 *
 * The subtotal beside each function is provided only to make the file-level
 * result easy to audit.  FL_STMT itself produces one value for the whole file.
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
/* Subtotal: 0.  FL_STMT = 0. */

/* Ordinary and empty expression-statements each count once. */
static void expression_statements(int *value)
{
    *value = 1;       /* +1: expression-statement */
    ++*value;         /* +1: expression-statement */
    ;                 /* +1: empty expression-statement */
}
/* Subtotal: 3.  Running FL_STMT = 3. */

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
/* Subtotal: 5.  Running FL_STMT = 8. */

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
/* Subtotal: 10.  Running FL_STMT = 18. */

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
/* Subtotal: 6.  Running FL_STMT = 24. */

/* A generic label and its contained jump-statement are distinct nodes. */
static int generic_label_and_jumps(int value)
{
    goto finished;            /* +1: goto-statement */

finished:                     /* +1: generic labeled-statement */
    return value;             /* +1: return-statement */
}
/* Subtotal: 3.  Running FL_STMT = 27. */

/* Statements in an old-style K&R definition contribute normally. */
static int kandr_statements(value)
    int value;
{
    value += 1;                 /* +1: expression-statement */
    return value;               /* +1: return-statement */
}
/* Subtotal: 2.  Final FL_STMT = 29. */
