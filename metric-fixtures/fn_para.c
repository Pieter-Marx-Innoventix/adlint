/*
 * FN_PARA verification fixture.
 *
 * AdLint reports the number of formal parameters in each ANSI or K&R function
 * definition.  The sole `void` marker and an unspecified empty parameter list
 * count as zero; an ellipsis is not an additional named parameter.
 *
 * Expected function metrics:
 *     explicit_void              = 0
 *     unspecified_parameters     = 0
 *     one_scalar                 = 1
 *     mixed_parameters           = 3
 *     array_parameters           = 2
 *     function_parameters        = 3
 *     aggregate_parameter        = 1
 *     variadic_parameters        = 1
 *     kandr_parameters           = 3
 */

struct pair {
    int first;
    int second;
};

/* Prototypes are not measured because FN_PARA is emitted for definitions. */
int prototype_only(int first, int second, int third);

/* `void` explicitly states that the function has no parameters. */
static int explicit_void(void)
{
    return 0;
}
/* explicit_void: FN_PARA = 0. */

/* In C, an empty list leaves the parameter types unspecified. */
static int unspecified_parameters()
{
    return 0;
}
/* unspecified_parameters: FN_PARA = 0. */

static int one_scalar(int value)
{
    return value;
}
/* one_scalar: FN_PARA = 1. */

/* Arithmetic, qualified, and pointer parameters each count once. */
static int mixed_parameters(int count, const unsigned long flags, char *text)
{
    return count + (flags != 0) + (text != 0);
}
/* mixed_parameters: FN_PARA = 3. */

/* Array bounds and dimensions do not create additional parameters. */
static int array_parameters(int values[10], int matrix[][4])
{
    return values[0] + matrix[0][0];
}
/* array_parameters: FN_PARA = 2. */

/* Function and function-pointer parameters are each single parameters. */
static int function_parameters(int callback(int),
                               int (*callback_pointer)(int), int value)
{
    return callback(value) + callback_pointer(value);
}
/* function_parameters: FN_PARA = 3. */

/* Passing a structure by value still introduces only one parameter. */
static int aggregate_parameter(struct pair value)
{
    return value.first + value.second;
}
/* aggregate_parameter: FN_PARA = 1. */

/* The ellipsis is not a named formal parameter. */
static int variadic_parameters(int required, ...)
{
    return required;
}
/* variadic_parameters: FN_PARA = 1. */

/*
 * K&R identifier-list entries count even when a declaration—and therefore
 * the implicit `int` type—is omitted for one of them.
 */
static int kandr_parameters(first, second, third)
    long second;
    char *third;
{
    return first + (int)second + (third != 0);
}
/* kandr_parameters: FN_PARA = 3. */
