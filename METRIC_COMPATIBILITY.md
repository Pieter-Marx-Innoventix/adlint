# Metric meanings and compatibility

AdLint 3.4.0 is the Innoventix fork based on 3.2.14. The user guides and
public C fixtures describe the intended conventions. The table below covers
the 15 metrics exercised by those fixtures; it is not a list of every metric
available in AdLint.

| Metric | Counting convention |
|---|---|
| FL_FUNC | Function definitions belonging to the input file, including ANSI and K&R forms. |
| FL_STMT / FN_STMT | File/function statement totals. Count expression statements (including empty ones), labels (including case/default), selections, loops and transfers. A label and its labelled statement count separately. Exclude declarations and compound braces. Count a for-loop once; exclude its initializer/condition helper statements. |
| FN_UNRC | Statements in that same population which the reachability pass marks unreachable; nested unreachable statements count separately. |
| FN_LINE | Inclusive span from the function identifier's line to the closing brace. A return type on a preceding line is outside that span. |
| FN_PARA | Named formal parameters. A void marker is zero; an ellipsis adds no named parameter. |
| FN_UNUV | Unreferenced local variables/parameters and unread explicit assignments. Use declaration identity, including shadowed names. A declaration initializer is not an additional write. Loop access replay accounts for values read on later iterations within the analyzer's model. |
| FN_CSUB | Syntactically present call expressions, rather than distinct callees. |
| FN_GOTO | Syntactically present goto statements. |
| FN_RETN | Explicit returns, including unreachable ones, plus one implicit return for a void function only if its body exit is reachable. No implicit return is added for a non-void function. |
| FN_UELS | An else-if chain without a final else counts once, at its terminal else-if. A standalone if is zero. An else containing a braced nested if is not a bare else-if continuation. |
| FN_NEST | Maximum control-structure depth, starting at zero in the function body. Plain braces add nothing. A bare if/else-if/else ladder uses one level; a separately nested if adds a level. |
| FN_PATH | Paths explored by the abstract interpreter, including loop entry/skip choices. This is not enumeration of all concrete loop iterations or runtime paths. |
| FN_CYCM | One plus each if, case label, loop, logical &&, logical || and conditional ?: expression. Count syntax even when constant or unreachable. Else, default and switch itself add nothing. |
| FN_CALL | Distinct direct and transitive caller functions, requiring cross-module analysis. Repeated call sites do not multiply a caller; recursion can include the function itself. |

FN_CYCM uses the documented **expanded McCabe-style syntactic convention**.
For example, each of `if (a && b)` and `if (a || b)` gives 3 in an otherwise
empty function: base 1 + if 1 + logical operator 1. `if (a && b || c)` gives
4. It is not a promise to reproduce every control-flow-graph definition of
strict McCabe complexity; syntactically present loops and constant conditions
are still counted.

## Comparing with older results

FL_STMT, FN_STMT, FN_UNRC, FN_UNUV, FN_RETN, FN_NEST and FN_CYCM have revised
calculations compared with 3.2.14. FN_UELS and FN_PATH documentation was
clarified. Interpreter corrections can also change metrics and warnings even
when an individual metric class is unchanged. FN_LINE's inclusive span is
inherited from 3.2.14, not a new fork change.

Recalculate a representative baseline before adopting thresholds or comparing
trends across versions. Investigate differences against the definitions;
do not silently replace expected values or loosen limits to make checks pass.
Record the exact source revision or gem SHA-256, Ruby/platform, traits,
initial headers, input bytes and SMA/CMA commands. The displayed 3.4.0 alone
does not identify a build. Preserve old qualification artifacts separately.

## Scope of the evidence

The public suite contains 251 metric expectations in 22 C files, plus a
statement-total cross-check and cross-module caller analysis. Fresh output
directories prevent old CSV files from satisfying new runs. Unit tests check
the expectation reader as well as analyzer internals. See
[metric-fixtures/README.md](metric-fixtures/README.md).

Passing these examples establishes agreement within their tested scope, not
complete correctness for every C program. Interpretation is approximate;
loops, unknown values, aliasing and external side effects can limit precision.
The statement graph follows goto targets, but the value interpreter still
visits source statements lexically after a goto. Reachability depends partly
on its observations and is not an independent proof of program behavior.
Review unusual inputs and surprising results against source and diagnostics.
