require "spec_helper"

module AdLint
module Cc1
  describe "Logical negation of scalar domains" do
    cases = [
      ["negative values below zero", lambda { |s| ValueDomain.less_than(0, s) }, :false],
      ["positive values above zero", lambda { |s| ValueDomain.greater_than(0, s) }, :false],
      ["negative values below -3", lambda { |s| ValueDomain.less_than(-3, s) }, :false],
      ["positive values above 3", lambda { |s| ValueDomain.greater_than(3, s) }, :false],
      ["values below 1 including zero", lambda { |s| ValueDomain.less_than(1, s) }, :unknown],
      ["values above -1 including zero", lambda { |s| ValueDomain.greater_than(-1, s) }, :unknown],
      ["exactly zero", lambda { |s| ValueDomain.equal_to(0, s) }, :true],
      ["exactly 7", lambda { |s| ValueDomain.equal_to(7, s) }, :false],
      ["exactly -7", lambda { |s| ValueDomain.equal_to(-7, s) }, :false],
      ["all nonzero values", lambda { |s| ValueDomain.not_equal_to(0, s) }, :false],
      ["two nonzero alternatives", lambda { |s| ValueDomain.equal_to(-3, s).union(ValueDomain.equal_to(7, s)) }, :false],
      ["zero or a nonzero alternative", lambda { |s| ValueDomain.equal_to(0, s).union(ValueDomain.equal_to(7, s)) }, :unknown],
      ["a positive bounded intersection", lambda { |s| ValueDomain.greater_than(0, s).intersection(ValueDomain.less_than(5, s)) }, :false],
      ["an intersection spanning zero", lambda { |s| ValueDomain.greater_than(-2, s).intersection(ValueDomain.less_than(2, s)) }, :unknown],
      ["an unrestricted value", lambda { |s| ValueDomain.of_unlimited(s) }, :unknown],
      ["an empty value domain", lambda { |s| ValueDomain.of_nil(s) }, :empty],
      ["floating values below zero", lambda { |s| ValueDomain.less_than(0.0, s) }, :false],
      ["floating values above zero", lambda { |s| ValueDomain.greater_than(0.0, s) }, :false]
    ]

    [false, true].each do |logical_shr|
      context "with logical_right_shift=#{logical_shr}" do
        cases.each do |name, factory, expected|
          it "preserves the correct truth when negating #{name}" do
            value = ScalarValue.new(!factory.call(logical_shr))
            case expected
            when :false
              value.test_must_be_false.true?.should be_true
              value.test_may_be_true.true?.should be_false
            when :true
              value.test_must_be_true.true?.should be_true
              value.test_may_be_false.true?.should be_false
            when :unknown
              value.test_may_be_true.true?.should be_true
              value.test_may_be_false.true?.should be_true
            when :empty
              value.exist?.should be_false
            end
          end
        end

        it "negates ScalarValue.of_true to a known false value" do
          value = !ScalarValue.of_true(logical_shr)
          value.test_must_be_false.true?.should be_true
        end

        it "keeps double negation of a known true value known true" do
          value = !(!ScalarValue.of_true(logical_shr))
          value.test_must_be_true.true?.should be_true
        end
      end
    end
  end
end
end
