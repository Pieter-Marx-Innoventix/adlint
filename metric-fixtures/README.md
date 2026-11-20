# Metric verification fixtures

These public C examples are the authoritative fixture inputs for the metric
feature tests. Each file starts with a comment containing its metric name and
expected integer values. Function metrics use `function_name = value`; file
metrics use `METRIC_NAME = value`. Keep these expectations under review when
changing metric definitions; do not change them simply to make a test pass.

Run all checks from the repository root with `rake spec` and `rake features`.
For only the metric features, run:

```powershell
ruby -Ilib -S cucumber --strict --require features features/metric_measurement
```

The 17 C files contain 187 expectations across 15 metrics, including 35
FN_UNUV loop cases. `fn_call_1.c` and `fn_call_2.c` are analysed together and
passed through cross-module analysis to check all nine caller counts. A
separate scenario checks that sum(FN_STMT) = FL_STMT = 29 for the equivalent
statement fixtures. The files are copied byte for byte, preserving FN_LINE's
line-sensitive examples.

The tests run this checkout's analyzer with the current Ruby interpreter and
the feature suite's noarch traits and empty initial headers. Every scenario
uses a fresh directory under `cucumber-tmp/`; inputs, CSV files and subprocess
logs remain there for diagnosis. Nonzero analyzer exit codes, missing or extra
results, duplicates, malformed values and mismatches all fail the tests. Unit
tests also check that the expectation checker rejects invalid evidence.

These checks are independent of the private wrapper. Passing them establishes
agreement with these examples, not complete analyzer correctness. The inherited
interpreter findings tracked under A04 remain separate audit work.
