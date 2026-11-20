# Building the AdLint 3.4.0 Gem

Run the following commands in PowerShell.

1. Download the Ruby 1.9.3-compatible DevKit:

   [DevKit-tdm-32-4.5.2-20111229-1559-sfx.exe](https://github.com/oneclick/rubyinstaller/releases/download/DevKit-tdm-32-4.5.2/DevKit-tdm-32-4.5.2-20111229-1559-sfx.exe)

2. Verify the download:

   ```powershell
   $installer = "C:\CheckMetricsWork\00_ToolInstallers\DevKit-tdm-32-4.5.2-20111229-1559-sfx.exe"

   Get-Item -LiteralPath $installer |
       Select-Object Name, Length, LastWriteTime

   Get-FileHash -Algorithm SHA256 -LiteralPath $installer

   Get-AuthenticodeSignature -LiteralPath $installer |
       Select-Object Status, StatusMessage, SignerCertificate
   ```

   Expected results:

   ```text
   Size:      22,975,511 bytes
   SHA-256:   6C3AF5487DAFDA56808BAF76EDD262B2020B1B25AB86AABF972629F4A6A54491
   Signature: NotSigned
   ```

3. Run the downloaded file and extract it to:

   ```text
   C:\DevKit
   ```

4. Initialize and install the DevKit integration:

   ```powershell
   Set-Location C:\DevKit
   ruby dk.rb init
   ruby dk.rb install
   ```

5. Verify the compiler tools:

   ```powershell
   ruby -rdevkit -e "system('gcc --version'); system('make --version')"
   ```

6. Install and verify Racc:

   ```powershell
   gem install racc --version 1.4.7 --no-ri --no-rdoc
   racc --version
   ```

7. Install and verify RSpec:

   ```powershell
   gem install rspec --version 2.14.1 --no-ri --no-rdoc
   rspec --version
   ruby -rrspec/core/rake_task -e "puts 'RSpec Rake task loaded successfully'"
   ```

8. Install the period-compatible Cucumber dependencies:

   ```powershell
   gem install multi_json --version 1.10.1 --no-ri --no-rdoc
   gem install builder --version 3.2.2 --no-ri --no-rdoc
   gem install multi_test --version 0.1.1 --no-ri --no-rdoc
   gem install gherkin --version 2.12.2 --no-ri --no-rdoc
   ```

9. Install and verify Cucumber:

   ```powershell
   gem install cucumber --version 1.3.15 --no-ri --no-rdoc
   cucumber --version
   ruby -rcucumber/rake/task -e "puts 'Cucumber Rake task loaded successfully'"
   ```

10. Open the AdLint project and verify its Rake tasks:

    ```powershell
    Set-Location "C:\AdLint\adlint-fork"
    rake --tasks
    ```

11. Run the unit and feature tests:

    ```powershell
    rake spec
    $LASTEXITCODE

    rake features
    $LASTEXITCODE
    ```

    Both exit codes should be `0`. Feature tests may take 5–10 minutes.

12. Regenerate the parsers and retest:

    ```powershell
    rake parser
    rake spec
    $LASTEXITCODE
    ```

13. Generate the packaged HTML documentation:

    ```powershell
    ruby -rdevkit -S rake doc
    ```

14. Build the gem:

    ```powershell
    ruby -rdevkit -S rake gem
    ```

    Expected output:

    ```text
    pkg\adlint-3.4.0.gem
    ```

Racc, RSpec, Cucumber, Make, and the DevKit are development-only tools. The resulting AdLint gem declares no runtime dependencies, so end users do not need to install them.
