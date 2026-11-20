Feature: Values read on later loop iterations
  FN_UNUV should recognise values read on a later loop iteration while
  retaining unread assignments that are overwritten or leave the loop.

  Scenario: Count loop-carried values and genuinely unused assignments
    Given the FN_UNUV loop verification fixture
    When I successfully run `adlint fn_unuv_loops.c` on noarch
    Then the FN_UNUV results should match the fixture expectations
