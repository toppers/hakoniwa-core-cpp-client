@echo off
setlocal enabledelayedexpansion

set "default_shm_type=mmap"
set "default_core_mmap_path=%CD%\mmap"
set "config_file=%CD%\core\cpp_core_config.json"
set "reset="
set "path_to="

:parse_args
if "%1"=="" goto validate
if "%1"=="-r" set "reset=1" & shift & goto parse_args
if "%1"=="-p" set "path_to=%~f2" & shift & shift & goto parse_args
goto usage

:validate
if defined reset goto check_file
if not defined path_to goto usage

:check_file
if not exist "%config_file%" (
    echo Error: Configuration file does not exist at %config_file%
    exit /b 1
)

powershell -command "Get-Command jq" >nul 2>&1
if %errorlevel% neq 0 (
    echo Error: jq is not installed.
    echo Please install jq to use this script. You can download jq from https://stedolan.github.io/jq/download/
    exit /b 1
)

if defined reset (
    powershell -command "(Get-Content '%config_file%' | ConvertFrom-Json | ForEach-Object { $_.shm_type = '%default_shm_type%'; $_.core_mmap_path = '%default_core_mmap_path%'; $_ } | ConvertTo-Json) | Set-Content '%config_file%'"
    echo Configuration reset to default.
    goto end
)

powershell -command "(Get-Content '%config_file%' | ConvertFrom-Json | ForEach-Object { $_.shm_type = 'mmap'; $_.core_mmap_path = '%path_to%'; $_ } | ConvertTo-Json) | Set-Content '%config_file%'"
echo Configuration updated with custom path.

:end
echo INFO: updated %config_file%
goto eof

:usage
echo Usage:
echo   %0 [-r] -p /path/to  # Set custom path or reset to default
exit /b 1

:eof
endlocal
