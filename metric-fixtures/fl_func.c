/*
 * FL_FUNC verification fixture.
 *
 * AdLint counts ANSI and K&R function-definition AST nodes whose definitions
 * originate in this source file.  Declarations, function types, function
 * pointers, and aggregate members do not contribute.
 *
 * Expected file metric:
 *     FL_FUNC = 10
 */

/* Function declarations and prototypes are not function definitions. */
void declared_without_parameters();
extern int declared_with_prototype(int value);
static void declared_with_internal_linkage(void);

/* Function-related types and objects are also not function definitions. */
typedef int callback_type(int value);
typedef int (*callback_pointer_type)(int value);

static callback_pointer_type callback_pointer;

struct callback_holder {
    callback_pointer_type callback;
};

/* +1: external ANSI definition with an explicit void parameter list. */
void external_no_parameters(void)
{
}

/* +1: static ANSI definition. */
static int internal_definition(void)
{
    return 0;
}

/* +1: an extern declaration carrying a body is still a definition. */
extern int explicit_external_definition(void)
{
    return 1;
}

/* +1: abbreviated empty parameter list; not a prototype in C. */
int unspecified_parameters()
{
    return 2;
}

/* A preceding prototype does not add to FL_FUNC. */
int typed_parameters(int left, int right);

/* +1: ordinary ANSI definition with typed parameters. */
int typed_parameters(int left, int right)
{
    return left + right;
}

/* +1: variadic ANSI definition. */
int variadic_definition(int first, ...)
{
    return first;
}

/* +1: an array parameter is adjusted to a pointer parameter by C. */
int array_parameter(int values[], int count)
{
    return count > 0 ? values[0] : 0;
}

/* +1: the parameter is a pointer to a function, not another definition. */
int function_pointer_parameter(callback_pointer_type callback, int value)
{
    return callback(value);
}

/* +1: a pointer return type does not change definition counting. */
int *pointer_return(int *value)
{
    return value;
}

/* +1: old-style K&R function definition. */
int kandr_definition(left, right)
    int left;
    int right;
{
    return left + right;
}

/* Final FL_FUNC = 10. */
