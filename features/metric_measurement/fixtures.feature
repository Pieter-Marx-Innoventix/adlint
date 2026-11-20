Feature: Metric definitions recorded in the verification fixtures
  Check the expectations in the public C fixtures directly with AdLint,
  without depending on the private wrapper or on saved analysis output.

  Scenario Outline: Match each single-file metric fixture
    Given the metric verification fixtures:
      | metric   | file   |
      | <metric> | <file> |
    When I analyse the metric fixtures
    Then the metric results should match the fixture expectations

    Examples:
      | metric  | file      |
      | FL_FUNC | fl_func.c |
      | FL_STMT | fl_stmt.c |
      | FN_CSUB | fn_csub.c |
      | FN_CYCM | fn_cycm.c |
      | FN_GOTO | fn_goto.c |
      | FN_LINE | fn_line.c |
      | FN_NEST | fn_nest.c |
      | FN_PARA | fn_para.c |
      | FN_PATH | fn_path.c |
      | FN_RETN | fn_retn.c |
      | FN_STMT | fn_stmt.c |
      | FN_UELS | fn_uels.c |
      | FN_UNRC | fn_unrc.c |
      | FN_UNUV | fn_unuv.c |

  Scenario: Count callers across both translation units
    Given the metric verification fixtures:
      | metric  | file        |
      | FN_CALL | fn_call_1.c |
      | FN_CALL | fn_call_2.c |
    When I analyse and link the metric fixtures
    Then the metric results should match the fixture expectations

  Scenario: Keep function and file statement counts consistent
    Given the metric verification fixtures:
      | metric  | file      |
      | FN_STMT | fn_stmt.c |
      | FL_STMT | fl_stmt.c |
    When I analyse the metric fixtures
    Then the metric results should match the fixture expectations
    And the sum of FN_STMT should equal FL_STMT at 29
