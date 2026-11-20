Given /^the FN_UNUV loop verification fixture$/ do
  path = File.expand_path("../../metric-fixtures/fn_unuv_loops.c",
                          File.dirname(__FILE__))
  @loop_fixture = File.read(path)
  create_src_file("fn_unuv_loops.c", @loop_fixture)
end

Then /^the FN_UNUV results should match the fixture expectations$/ do
  header = @loop_fixture[/\A\/\*.*?\*\//m]
  expected = {}
  header.scan(/^\s*\*\s+(\w+)\s*=\s*(\d+)\s*$/) do |name, value|
    expected[name] = value.to_i
  end
  raise "No FN_UNUV expectations found in fixture header" if expected.empty?

  records = cd_to_tmpdir do
    CSV.read("fn_unuv_loops.c.met.csv").select { |row|
      row[0, 2] == ["MET", "FN_UNUV"]
    }
  end
  records.length.should == expected.length
  actual = {}
  records.each { |row| actual[row[2]] = row.last.to_i }
  actual.should == expected
end
