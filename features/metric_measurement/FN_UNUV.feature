Feature: Values read on later loop iterations
  FN_UNUV should recognise values read on a later loop iteration while
  retaining unread assignments that are overwritten or leave the loop.

  Scenario: Count loop-carried values and genuinely unused assignments
    Given the metric verification fixtures:
      | metric  | file            |
      | FN_UNUV | fn_unuv_loops.c |
    When I analyse the metric fixtures
    Then the metric results should match the fixture expectations
