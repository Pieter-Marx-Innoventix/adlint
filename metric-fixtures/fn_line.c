/*
 * FN_LINE verification fixture.
 *
 * AdLint calculates the inclusive physical-line span from the line containing
 * the function identifier through the line containing the body's closing
 * brace.  Text before the identifier line is excluded; blank lines, comments,
 * and inactive preprocessing lines inside that span are included.
 *
 * Expected function metrics:
 *     one_line             = 1
 *     standard_layout      = 4
 *     split_declarator     = 7
 *     internal_noncode     = 7
 *     closing_brace_shared = 3
 *     preprocessor_gap     = 8
 *     kandr_lines          = 5
 */

static int one_line(void) { return 0; }
/* one_line: FN_LINE = 1. */

static int standard_layout(void)
{
    return 0;
}
/* standard_layout: FN_LINE = 4. */

/* The return-type line is before the identifier and is therefore excluded. */
static int
split_declarator
(
    void
)
{
    return 0;
}
/* split_declarator: FN_LINE = 7. */

/* Blank and comment-only lines inside the physical span still contribute. */
static int internal_noncode(void)
{

    /* This comment occupies one physical line. */

    return 0;
}
/* internal_noncode: FN_LINE = 7. */

/* The closing brace shares the return-statement's physical line. */
static int closing_brace_shared(void)
{
    return 0; }
/* closing_brace_shared: FN_LINE = 3. */

/* Inactive source lines remain between the identifier and closing brace. */
static int preprocessor_gap(void)
{
#if 0
    int excluded_declaration = 0;
    excluded_declaration += 1;
#endif
    return 0;
}
/* preprocessor_gap: FN_LINE = 8. */

/* K&R parameter declarations fall within the measured physical span. */
static int kandr_lines(value)
    int value;
{
    return value;
}
/* kandr_lines: FN_LINE = 5. */
