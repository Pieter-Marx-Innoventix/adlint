# Fixture expectation checking for the metric feature suite.
# Maintained by Innoventix; distributed under AdLint's GPLv3-or-later licence.

class MetricFixture
  attr_reader :metric, :filename, :source, :expected

  def initialize(metric, filename, source)
    @metric, @filename, @source = metric, filename, source
    header = source[/\A\s*\/\*.*?\*\//m]
    raise "#{filename}: missing #{metric} expectation header" unless
      header && header =~ /\b#{Regexp.escape(metric)}\b/

    @expected = {}
    header.scan(/^\s*\*\s+([A-Za-z_]\w*)\s*=\s*(.*?)\s*$/) do |name, value|
      raise "#{filename}: invalid expectation for #{name}: #{value}" unless
        value =~ /\A\d+\z/
      raise "#{filename}: duplicate expectation for #{name}" if
        @expected.has_key?(name)
      @expected[name] = Integer(value, 10)
    end
    raise "#{filename}: no #{metric} expectations" if @expected.empty?
    if metric.start_with?("FL_") && @expected.keys != [metric]
      raise "#{filename}: file expectation must be #{metric}"
    end
  end

  # Compare the complete result set, including source identity. Different
  # functions may share a name across translation units. Never coerce invalid
  # text with to_i: that could make a missing/invalid zero result pass.
  def self.verify(fixtures, rows)
    expected = {}
    fixtures.each do |fixture|
      fixture.expected.each do |name, value|
        key = [fixture.metric, fixture.filename, name]
        raise "Duplicate fixture expectation: #{key.join('/')}" if
          expected.has_key?(key)
        expected[key] = value
      end
    end

    metrics = fixtures.map { |fixture| fixture.metric }.uniq
    actual = {}
    rows.each do |row|
      next unless row[0] == "MET" && metrics.include?(row[1])
      metric = row[1]
      file_metric = metric.start_with?("FL_")
      unless row.length == (file_metric ? 4 : 8) && row.last =~ /\A\d+\z/
        raise "Malformed #{metric} result: #{row.inspect}"
      end
      filename = File.basename((file_metric ? row[2] : row[4]).tr("\\", "/"))
      key = [metric, filename, file_metric ? metric : row[2]]
      raise "Duplicate metric result: #{key.join('/')}" if actual.has_key?(key)
      actual[key] = Integer(row.last, 10)
    end

    missing = expected.keys - actual.keys
    extra = actual.keys - expected.keys
    raise "Missing metric results: #{missing.inspect}" unless missing.empty?
    raise "Unexpected metric results: #{extra.inspect}" unless extra.empty?
    expected.each do |key, value|
      unless actual[key] == value
        raise "#{key.join('/')}: expected #{value}, got #{actual[key]}"
      end
    end
    actual
  end
end
