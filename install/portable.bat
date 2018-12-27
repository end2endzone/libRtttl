@ECHO OFF
call version_info.bat

REM **************************************
REM  Generate NSIS include file
REM **************************************
cscript //nologo "scripts\generate_svn_revision.vbs" "%~dp0." "%~dp0scripts"
call scripts\svn_revision.bat

REM **************************************
REM  Generate ZIP package
REM **************************************

set outfile=".\bin\rtttlOptimizer v%PRODUCT_VERSION%.%SVN_REVISION% %PRODUCT_PLATFORM% portable.zip"

set infiles=
set infiles=%infiles% "..\bin\%PRODUCT_PLATFORM%\Release\rtttlOptimizerGui.exe"
set infiles=%infiles% "..\bin\%PRODUCT_PLATFORM%\Release\libMidi.dll"
set infiles=%infiles% "..\bin\%PRODUCT_PLATFORM%\Release\beep.dll"
set infiles=%infiles% "..\bin\%PRODUCT_PLATFORM%\Release\libRtttl.dll"
set infiles=%infiles% "LICENSE.htm"
set infiles=%infiles% "LICENSE.pdf"

call scripts\portable.bat
pause
