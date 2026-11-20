require "spec_helper"
require File.expand_path("../features/support/metric_fixture", File.dirname(__FILE__))

describe MetricFixture do
  def fixture(metric = "FN_UNUV", filename = "sample.c", expectation = "example = 0")
    MetricFixture.new(metric, filename, "/* #{metric}\n * #{expectation}\n */\n")
  end

  def result(value = "0", name = "example", filename = "sample.c")
    ["MET", "FN_UNUV", name, "void #{name}(void)", filename, "4", "1", value]
  end

  it "accepts a genuine zero result and ignores other metric types" do
    rows = [result, ["MET", "FN_LINE", "example", "ignored"]]
    MetricFixture.verify([fixture], rows).values.should == [0]
  end

  it "accepts file-level metrics with Windows paths" do
    f = fixture("FL_STMT", "sample.c", "FL_STMT = 29")
    MetricFixture.verify([f], [["MET", "FL_STMT", "C:\\work\\sample.c", "29"]]).values.should == [29]
  end

  it "keeps identically named functions in different translation units separate" do
    fixtures = [fixture, fixture("FN_UNUV", "second.c", "example = 1")]
    MetricFixture.verify(fixtures, [result, result("1", "example", "second.c")]).values.sort.should == [0, 1]
  end

  it "rejects a missing zero-valued result" do
    expect { MetricFixture.verify([fixture], []) }.to raise_error(/Missing metric results/)
  end

  it "rejects duplicate results even when they have the expected value" do
    expect { MetricFixture.verify([fixture], [result, result]) }.to raise_error(/Duplicate metric result/)
  end

  it "rejects unexpected functions" do
    expect { MetricFixture.verify([fixture], [result, result("0", "extra")]) }.to raise_error(/Unexpected metric results/)
  end

  it "rejects results attributed to the wrong source file" do
    expect { MetricFixture.verify([fixture], [result("0", "example", "wrong.c")]) }.to raise_error(/Missing metric results/)
  end

  it "rejects the wrong numeric value" do
    expect { MetricFixture.verify([fixture], [result("1")]) }.to raise_error(/expected 0, got 1/)
  end

  it "rejects malformed values instead of converting them to zero" do
    ["", "invalid", "0extra", "-1", nil].each do |value|
      expect { MetricFixture.verify([fixture], [result(value)]) }.to raise_error(/Malformed FN_UNUV result/)
    end
  end

  it "rejects truncated records" do
    expect { MetricFixture.verify([fixture], [["MET", "FN_UNUV", "0"]]) }.to raise_error(/Malformed FN_UNUV result/)
  end

  it "rejects absent or empty expectation headers" do
    ["int example(void);", "/* FN_UNUV */"].each do |source|
      expect { MetricFixture.new("FN_UNUV", "sample.c", source) }.to raise_error(/expectation/)
    end
  end

  it "rejects a header for a different metric" do
    expect { MetricFixture.new("FN_LINE", "sample.c", "/* FN_UNUV\n * example = 0\n */") }.to raise_error(/missing FN_LINE expectation header/)
  end

  it "rejects duplicate expectations" do
    expect { fixture("FN_UNUV", "sample.c", "example = 0\n * example = 0") }.to raise_error(/duplicate expectation/)
  end

  it "rejects invalid expected values" do
    expect { fixture("FN_UNUV", "sample.c", "example = invalid") }.to raise_error(/invalid expectation/)
  end

  it "rejects a file-level expectation with the wrong key" do
    expect { fixture("FL_STMT") }.to raise_error(/file expectation must be FL_STMT/)
  end
end
