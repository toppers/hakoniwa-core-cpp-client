@echo off
setlocal

set "BUILD_REL_PATH=out\build\x64-Debug\core\sample\base-procs\hako-cmd"
set "BIN_DIR=%CD%\bin"
set "MY_CONFIG_PATH=%CD%\core\cpp_core_config.json"

if not exist "%BIN_DIR%" mkdir "%BIN_DIR%"
xcopy "%CD%\%BUILD_REL_PATH%\*.exe" "%BIN_DIR%" /D /Y

echo Please add PATH:  %BIN_DIR%
::setx PATH "%PATH%;%BIN_DIR%"
setx HAKO_CONFIG_PATH "%MY_CONFIG_PATH%"

echo Installation completed successfully. Please restart your terminal or log off and log back in for the changes to take effect.
endlocal
