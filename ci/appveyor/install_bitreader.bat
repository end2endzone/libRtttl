@echo off

:: Validate appveyor's environment
if "%APPVEYOR_BUILD_FOLDER%"=="" (
  echo Please define 'APPVEYOR_BUILD_FOLDER' environment variable.
  exit /B 1
)

set GTEST_ROOT=%APPVEYOR_BUILD_FOLDER%\third_parties\googletest\install
set rapidassist_DIR=%APPVEYOR_BUILD_FOLDER%\third_parties\RapidAssist\install
set INSTALL_LOCATION=%APPVEYOR_BUILD_FOLDER%\install

echo ============================================================================
echo Cloning BitReader into %APPVEYOR_BUILD_FOLDER%\third_parties\BitReader
echo ============================================================================
mkdir %APPVEYOR_BUILD_FOLDER%\third_parties >NUL 2>NUL
cd %APPVEYOR_BUILD_FOLDER%\third_parties
git clone "https://github.com/end2endzone/BitReader.git"
cd BitReader
echo.

echo Checking out version v1.3.0...
git checkout 1.3.0
echo.

REM echo ============================================================================
REM echo Compiling...
REM echo ============================================================================
REM mkdir build >NUL 2>NUL
REM cd build
REM cmake -DCMAKE_INSTALL_PREFIX=%INSTALL_LOCATION% ..
REM if %errorlevel% neq 0 exit /b %errorlevel%
REM cmake --build . --config %Configuration%
REM if %errorlevel% neq 0 exit /b %errorlevel%
REM echo.

REM echo ============================================================================
REM echo Installing into %INSTALL_LOCATION%
REM echo ============================================================================
REM cmake --build . --config %Configuration% --target INSTALL
REM if %errorlevel% neq 0 exit /b %errorlevel%
REM echo.
