Feature: Preserve variable uses across interpreter control transfers
  Loop-local jumps and inferred loop conditions must preserve real reads
  while leaving unread values and skipped conditions counted as unused.

  Scenario: Keep do-loop breaks and continues local and propagate returns
    Given the metric verification fixtures:
      | metric  | file                   |
      | FN_UNUV | fn_unuv_do_transfers.c |
    When I analyse the metric fixtures
    Then the metric results should match the fixture expectations

  Scenario: Keep while and for breaks local without consuming returns or continues
    Given the metric verification fixtures:
      | metric  | file                  |
      | FN_UNUV | fn_unuv_loop_breaks.c |
    When I analyse the metric fixtures
    Then the metric results should match the fixture expectations

  Scenario: Preserve known truth in inferred loop exits
    Given the metric verification fixtures:
      | metric  | file                         |
      | FN_UNRC | fn_unrc_inferred_constants.c |
    When I analyse the metric fixtures
    Then the metric results should match the fixture expectations

  Scenario: Evaluate inferred loop-breaking conditions without inventing reads
    Given the metric verification fixtures:
      | metric  | file                          |
      | FN_UNUV | fn_unuv_inferred_conditions.c |
    When I analyse the metric fixtures
    Then the metric results should match the fixture expectations

  Scenario: Preserve known truth through scalar logical negation
    Given the metric verification fixtures:
      | metric  | file                       |
      | FN_UNRC | fn_unrc_logical_negation.c |
    When I analyse the metric fixtures
    Then the metric results should match the fixture expectations
