# Builds the portable ZIP of NerdOne Commander from a finished Release build.
#
# Usage (from a Developer PowerShell for VS2026, after building Release|x64):
#   pwsh tools\make_portable.ps1
#
# Expects OPENSAL_BUILD_DIR to point to the build root (with trailing backslash) and
# VCToolsRedistDir to be set (Developer PowerShell / ilammy/msvc-dev-cmd set it).
# The version is read from NERDONE_VERSION_* in src\plugins\shared\spl_vers.h.

param(
    [string]$Version = '',
    [string]$BuildDir = $env:OPENSAL_BUILD_DIR,
    [string]$Configuration = 'Release',
    [string]$OutputDir = ''
)

$ErrorActionPreference = 'Stop'
$repoRoot = Split-Path -Parent $PSScriptRoot

if ([string]::IsNullOrEmpty($Version)) {
    $vers = Get-Content (Join-Path $repoRoot 'src\plugins\shared\spl_vers.h') -Raw
    $get = { param($n) if ($vers -match "#define\s+NERDONE_VERSION_$n\s+(""?)([^""\r\n/]*)\1") { $Matches[2].Trim() } else { throw "NERDONE_VERSION_$n not found" } }
    $Version = "$(& $get 'MAJOR').$(& $get 'MINOR').$(& $get 'PATCH')$(& $get 'SUFFIX')"
}
Write-Host "Version: $Version"

if ([string]::IsNullOrEmpty($BuildDir)) { throw 'OPENSAL_BUILD_DIR is not set.' }
$binDir = Join-Path $BuildDir "salamander\$($Configuration)_x64"
if (-not (Test-Path (Join-Path $binDir 'nerdone.exe'))) { throw "nerdone.exe not found in $binDir" }
if ([string]::IsNullOrEmpty($OutputDir)) { $OutputDir = $BuildDir }

$name = "NerdOne-Commander-$Version-x64-portable"
$stage = Join-Path $OutputDir $name
if (Test-Path $stage) { Remove-Item $stage -Recurse -Force }
New-Item -ItemType Directory -Path $stage | Out-Null

# 1) Program files without intermediates, import libraries and debug files
$exclude = @('*.pdb', '*.ilk', '*.lib', '*.exp', '*.iobj', '*.ipdb', '*.obj', '*.res', '*.tlog', '*.log', '*.lastbuildstate')
Get-ChildItem $binDir -Recurse -File |
    Where-Object { $_.FullName -notmatch '\\Intermediate\\' } |
    Where-Object { $f = $_.Name; -not ($exclude | Where-Object { $f -like $_ }) } |
    ForEach-Object {
        $rel = $_.FullName.Substring($binDir.Length).TrimStart('\')
        $dst = Join-Path $stage $rel
        New-Item -ItemType Directory -Path (Split-Path $dst) -Force | Out-Null
        Copy-Item $_.FullName $dst
    }

# Developer tools that are not part of the program: trace server and the installer's SFX stub
foreach ($f in @('utils\tserver.exe', 'utils\sfx7zip.exe', 'utils\sfx7zip.sfx')) {
    $p = Join-Path $stage $f
    if (Test-Path $p) { Remove-Item $p -Force }
}

# The program does not work without its helpers; stop if the Utils build was skipped
foreach ($f in @('utils\salopen.exe', 'utils\salspawn.exe', 'utils\salextx64.dll', 'utils\salmon.exe')) {
    if (-not (Test-Path (Join-Path $stage $f))) { throw "$f is missing - build 'Utils (Release)|x64' first." }
}

# Plugins that cannot work in this release: PictView lacks its decoding engine
# (PVW32Cnv.dll, closed source) and UnRAR needs the proprietary unrar.dll.
foreach ($p in @('pictview', 'unrar')) {
    $dir = Join-Path $stage "plugins\$p"
    if (Test-Path $dir) { Remove-Item $dir -Recurse -Force }
}

# 2) Visual C++ runtime (app-local); Windows 10/11 already contain the UCRT
$redist = $env:VCToolsRedistDir
if ([string]::IsNullOrEmpty($redist)) { throw 'VCToolsRedistDir is not set (use a Developer PowerShell).' }
$crt = Get-ChildItem (Join-Path $redist 'x64') -Directory -Filter 'Microsoft.VC*.CRT' | Select-Object -First 1
if ($null -eq $crt) { throw "No Microsoft.VC*.CRT folder under $redist\x64" }
foreach ($dll in @('vcruntime140.dll', 'vcruntime140_1.dll', 'msvcp140.dll', 'concrt140.dll')) {
    Copy-Item (Join-Path $crt.FullName $dll) $stage
}

# 3) Data files (same as !populate_build_dir.cmd)
function Copy-Data($from, $to, $filter = '*') {
    $src = Join-Path $repoRoot $from
    $dst = Join-Path $stage $to
    New-Item -ItemType Directory -Path $dst -Force | Out-Null
    Get-ChildItem $src -File -Filter $filter | Copy-Item -Destination $dst
}
foreach ($cs in @('centeuro', 'cyrillic', 'westeuro')) {
    Copy-Data "convert\$cs" "convert\$cs" 'convert.cfg'
    Copy-Data "convert\$cs" "convert\$cs" '*.tab'
}
Copy-Data 'src\res\toolbars' 'toolbars'
Copy-Data 'src\plugins\automation\sample-scripts' 'plugins\automation\scripts'
Copy-Data 'src\plugins\ieviewer\cmark-gfm\css' 'plugins\ieviewer\css'
foreach ($f in @('readme.txt', 'sam_cz.set', 'sample.set')) {
    Copy-Data 'src\plugins\zip\zip2sfx' 'plugins\zip\zip2sfx' $f
}

# 4) Documentation
foreach ($f in @('LICENSE', 'AUTHORS', 'README.md', 'CHANGELOG.md')) {
    Copy-Item (Join-Path $repoRoot $f) $stage
}

# 5) ZIP
$zip = Join-Path $OutputDir "$name.zip"
if (Test-Path $zip) { Remove-Item $zip -Force }
Compress-Archive -Path (Join-Path $stage '*') -DestinationPath $zip -CompressionLevel Optimal
Write-Host "Created $zip"

# 6) Debug symbols as a separate ZIP (useful for crash reports)
$symZip = Join-Path $OutputDir "NerdOne-Commander-$Version-x64-symbols.zip"
if (Test-Path $symZip) { Remove-Item $symZip -Force }
$pdbs = Get-ChildItem $binDir -Recurse -File -Filter '*.pdb' | Where-Object { $_.FullName -notmatch '\\Intermediate\\' }
if ($pdbs) { Compress-Archive -Path $pdbs.FullName -DestinationPath $symZip -CompressionLevel Optimal }

if ($env:GITHUB_OUTPUT) {
    "zip=$zip" | Out-File -FilePath $env:GITHUB_OUTPUT -Append -Encoding utf8
    "symbols=$symZip" | Out-File -FilePath $env:GITHUB_OUTPUT -Append -Encoding utf8
    "name=$name" | Out-File -FilePath $env:GITHUB_OUTPUT -Append -Encoding utf8
}
