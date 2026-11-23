# Building the AdLint 3.4.0 gem

This procedure describes the tested Windows toolchain. Ruby's package setting
`>= 1.9.3` is a minimum-version guard, not a claim that all later Rubies work.
Other Ruby versions, other platforms and Bundler remain unverified for this
fork. No Bundler workflow or Gemfile is supplied.

## Tools

| Tool | Version used for verification | Purpose |
|---|---|---|
| Ruby / RubyGems | 32-bit Ruby 1.9.3p125 / 1.8.16 | Runtime and packaging |
| RubyInstaller DevKit | tdm-32-4.5.2-20111229-1559 | Windows build-tool environment |
| GNU Make / GNU Texinfo makeinfo | 3.81 / 4.13 | Documentation build |
| Graphviz dot | 16.1.2, Windows 64-bit ZIP | Generate c99gram.pdf |
| Rake / RDoc | 0.9.2.2 / 3.9.4 | Task definitions and packaging |
| Racc | 1.4.7 | Generate both Ruby parsers |
| RSpec / rspec-core | 2.14.1 / 2.14.8 | Unit suite |
| Cucumber / Gherkin | 1.3.15 / 2.12.2 | Feature suite |
| multi_json / multi_test | 1.10.1 / 0.1.1 | Cucumber dependencies |
| builder / diff-lcs | See the captured gem list for a build | Transitive test dependencies |

Use the [RubyInstaller archive](https://rubyinstaller.org/downloads/archives/)
for the Ruby 1.9.3-compatible DevKit named
`DevKit-tdm-32-4.5.2-20111229-1559-sfx.exe`. Extract to a chosen directory
(for example `C:DevKit`), then run `ruby dk.rb init` and `ruby dk.rb install`
there to register it with the intended Ruby installation.

Get Graphviz from its [official download page](https://graphviz.org/download/).
For the ZIP distribution, compare its SHA-256 with the publisher's matching
checksum before extraction. Add its `bin` directory to this PowerShell
session's PATH; no global installation is needed. Graphviz runs as a separate
process, so a 64-bit dot can accompany the 32-bit Ruby used here.

The Rakefile loads RDoc, RSpec and Cucumber task libraries even for a build
that does not run tests; all must be available. The listed versions describe
the tested environment, not a dependency lock. Save `gem list --local` with
each verification run. The old RubyGems client may require obtaining legacy
gem files separately and installing them with `gem install --local`.

From the intended shell, check:

```powershell
ruby --version
gem --version
gem list --local
ruby -rdevkit -e "abort 'GNU Make missing' unless system('make --version'); abort 'makeinfo missing' unless system('makeinfo --version'); abort 'dot missing' unless system('dot -V')"
ruby -rrdoc/task -rrspec/core/rake_task -rcucumber/rake/task -e "puts 'Task libraries loaded'"
```

Confirm that `make --version` reports **GNU Make**. Loading `devkit` is
necessary on the tested machine because its ordinary PATH selects Borland
Make. Do not proceed with the wrong program.

## Build in a separate folder

Keep qualified gems and installed copies unchanged while preparing a new
candidate. These commands create a new source copy without local settings,
build output or generated files. Choose a new destination if it already exists.
Run in PowerShell:

```powershell
$sourcePath = 'C:AdLintadlint-fork'
$buildPath = 'C:AdLintadlint-build'
if (Test-Path -LiteralPath $buildPath) { throw 'Choose a new build folder' }
robocopy $sourcePath $buildPath /E /R:0 /W:0 /NFL /NDL `
  /XD .git .claude pkg cucumber-tmp rdoc `
  /XF features.html spec.html parser.rb constexpr.rb c99gram.pdf `
      users_guide_en.html users_guide_ja.html developers_guide_ja.html
if ($LASTEXITCODE -ge 8) { throw 'Source copy failed' }
Set-Location $buildPath
rake --tasks
```

Check each command's exit code before continuing:

```powershell
rake parser
$LASTEXITCODE
rake spec
$LASTEXITCODE
rake features
$LASTEXITCODE
ruby -rdevkit -S rake doc
$LASTEXITCODE
ruby -rdevkit -S rake gem
$LASTEXITCODE
Get-FileHash -Algorithm SHA256 -LiteralPath '.pkgadlint-3.4.0.gem'
```

Every build/test command must exit 0. The feature suite can take more than
ten minutes. Preserve its newly generated reports and the unit report.
For the source through the 23 November fixes, the expected totals are 416
unit examples, 2,164 feature scenarios and 6,511 steps; the public metric
fixtures contain 251 expectations. These are checkpoints, not a substitute
for inspecting failures or keeping the actual exit codes.

The fresh copy forces generation of both parsers, all three HTML guides and
the grammar PDF. Reusing an existing PDF or HTML file is not a clean
documentation build. `rake clobber` deletes package output; do not run it
in a working folder containing qualification artifacts.

Inspect the gem's files against MANIFEST, retain the source revision and
package SHA-256, then test installation into a separate gem directory before
choosing to replace any qualified installation. Avoid the default global
install for this check. Publication and changing an installed tool are
separate steps, performed manually by the maintainer.

The gem declares no external runtime-gem dependencies. Ruby and its required
standard libraries are still needed. Racc parser generation, Rake, RDoc,
RSpec, Cucumber, GNU Make, makeinfo, Graphviz and DevKit are build/test tools;
they are not needed to run the packaged command-line analyzer. GNU Make is
optional if users choose its generated analysis Makefiles.
