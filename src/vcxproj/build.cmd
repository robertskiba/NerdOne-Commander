@echo off

:: Check for command line arguments
if "%~1"=="help" (
  echo Usage: build.cmd [build_config] [build_arch]
  echo.
  echo build_config: Build configuration, default is 'Debug'
  echo build_arch: Build architecture, default is 'x64'
  echo.
  goto :eof
)

:: Check for MSBuild in the PATH
where msbuild >nul 2>&1
if %errorlevel% neq 0 set NO_MSBUILD=1

:: Locate MSBuild of Visual Studio 2026 (version 18) or newer
set "MSB="
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if exist "%VSWHERE%" for /f "usebackq delims=" %%i in (`"%VSWHERE%" -latest -version 18.0 -requires Microsoft.Component.MSBuild -find MSBuild\**\Bin\MSBuild.exe`) do set "MSB=%%i"

if not exist "%MSB%" (
  if defined NO_MSBUILD (
    echo MSBuild.exe not found. Please open a Developer Command Prompt for Visual Studio.
    echo.
    goto :eof
  ) else (
    set MSB=msbuild
  )
)

if "%OPENSAL_BUILD_DIR%"=="" (
  echo Please set OPENSAL_BUILD_DIR environment variable.
  echo.
  goto :eof
)

:: Default values for build_config and build_arch
set build_config=Debug
set build_arch=x64

:: Override default values with command line arguments if provided
if not "%~1"=="" set build_config=%~1
if not "%~2"=="" set build_arch=%~2

call :build %build_config% %build_arch%

goto :eof

:build
  echo Building %~1/%2
  if exist %3 del /q %3
  "%msb%" salamand.sln /t:build "/p:Configuration=%~1" /p:Platform=%2 /m:%NUMBER_OF_PROCESSORS%
  exit /b