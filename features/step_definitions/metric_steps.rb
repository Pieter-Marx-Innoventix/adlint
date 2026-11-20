require "csv"
require "open3"
require "rbconfig"
require "tmpdir"

Given /^the metric verification fixtures:$/ do |table|
  fixture_dir = File.expand_path("../../metric-fixtures", File.dirname(__FILE__))
  @metric_fixtures = table.hashes.map do |row|
    filename = row.fetch("file")
    MetricFixture.new(row.fetch("metric"), filename,
                      File.binread(File.join(fixture_dir, filename)))
  end
  raise "No metric fixtures selected" if @metric_fixtures.empty?
  filenames = @metric_fixtures.map { |fixture| fixture.filename }
  raise "Duplicate metric fixture filename" unless filenames.uniq == filenames

  # A fresh directory prevents an old CSV from satisfying a new test. Retain
  # the inputs and subprocess output under cucumber-tmp for failure diagnosis.
  Dir.mkdir($tmpdir) unless Dir.exist?($tmpdir)
  @metric_run_dir = Dir.mktmpdir("metrics-", $tmpdir)
  @metric_fixtures.each do |fixture|
    File.open(File.join(@metric_run_dir, fixture.filename), "wb") do |io|
      io.write(fixture.source)
    end
  end
  { "noarch_traits.yml" => NOARCH_TRAITS,
    "empty_pinit.h" => "", "empty_cinit.h" => "" }.each do |name, content|
    File.open(File.join(@metric_run_dir, name), "wb") { |io| io.write(content) }
  end
end

def run_metric_command(command, *arguments)
  stdout, stderr, status = Open3.capture3(RbConfig.ruby,
    File.join($bindir, command), "-t", "noarch_traits.yml", *arguments,
    :chdir => @metric_run_dir)
  File.open(File.join(@metric_run_dir, command + ".stdout.log"), "wb") do |io|
    io.write(stdout)
  end
  File.open(File.join(@metric_run_dir, command + ".stderr.log"), "wb") do |io|
    io.write(stderr)
  end
  unless status.success?
    raise "#{command} failed (exit #{status.exitstatus}) in #{@metric_run_dir}:\n" +
      stdout + stderr
  end
end

When /^I analyse the metric fixtures$/ do
  run_metric_command("adlint_sma",
                     *@metric_fixtures.map { |fixture| fixture.filename })
end

When /^I analyse and link the metric fixtures$/ do
  step "I analyse the metric fixtures"
  inputs = @metric_fixtures.map { |fixture| fixture.filename + ".met.csv" }
  run_metric_command("adlint_cma", "-o", "cma", *inputs)
end

Then /^the metric results should match the fixture expectations$/ do
  @metric_actual = {}
  @metric_fixtures.group_by { |fixture| fixture.metric }.each do |metric, fixtures|
    files = if metric == "FN_CALL"
      Dir[File.join(@metric_run_dir, "cma", "*.met.csv")]
    else
      fixtures.map { |fixture| File.join(@metric_run_dir, fixture.filename + ".met.csv") }
    end
    raise "Expected one cross-module metric file, got #{files.length}" if
      metric == "FN_CALL" && files.length != 1
    rows = files.map { |file| CSV.read(file) }.flatten(1)
    @metric_actual.merge!(MetricFixture.verify(fixtures, rows))
  end
end

Then /^the sum of FN_STMT should equal FL_STMT at (\d+)$/ do |count|
  function_values = @metric_actual.select { |key, _| key[0] == "FN_STMT" }.values
  file_values = @metric_actual.select { |key, _| key[0] == "FL_STMT" }.values
  raise "Missing statement cross-check results" if function_values.empty?
  function_values.inject(0, :+).should == count.to_i
  file_values.should == [count.to_i]
end
