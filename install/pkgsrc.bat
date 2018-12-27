@ECHO OFF
call version_info.bat

REM **************************************
REM  Extract subversion info
REM **************************************
call scripts\svn_revision.bat

REM **************************************
REM  Generate source code ZIP package
REM **************************************

::REMEMBER SOURCE CODE DESTINATION
set INSTALL_HOME=%cd%
cd ..
set SRC_HOME=%cd%
set SRC_DEST=%cd%\tmp

del /s /q %SRC_DEST%\..\tmp\*.* 2>NUL 1>NUL
mkdir %SRC_DEST% 2>NUL 1>NUL

REM **************************************
REM  Copy files and folders
REM **************************************
copy "%SRC_HOME%\rtttl.sln" "%SRC_DEST%\"
xcopy /y /s %SRC_HOME%\arduino %SRC_DEST%\arduino\
xcopy /y /s %SRC_HOME%\beep %SRC_DEST%\beep\
xcopy /y /s %SRC_HOME%\install %SRC_DEST%\install\
xcopy /y /s %SRC_HOME%\libAnyRtttl %SRC_DEST%\libAnyRtttl\
xcopy /y /s %SRC_HOME%\libBitReader %SRC_DEST%\libBitReader\
xcopy /y /s %SRC_HOME%\libMidi %SRC_DEST%\libMidi\
xcopy /y /s %SRC_HOME%\libRTTTL %SRC_DEST%\libRtttl\
xcopy /y /s %SRC_HOME%\rtttlOptimizerGui %SRC_DEST%\rtttlOptimizerGui\
xcopy /y /s %SRC_HOME%\testLibRtttl %SRC_DEST%\testLibRtttl\

REM **************************************
REM  Clean up already build projects
REM **************************************
cd /d %SRC_DEST%\
FOR /F "tokens=*" %%G IN ('DIR /B /AD /S Debug') DO RMDIR /S /Q "%%G"
FOR /F "tokens=*" %%G IN ('DIR /B /AD /S Release') DO RMDIR /S /Q "%%G"
FOR /F "tokens=*" %%G IN ('DIR /B /AD /S x64') DO RMDIR /S /Q "%%G"
FOR /F "tokens=*" %%G IN ('DIR /B /AD /S Win32') DO RMDIR /S /Q "%%G"
FOR /F "tokens=*" %%G IN ('DIR /B /AD /S bin') DO RMDIR /S /Q "%%G"
FOR /F "tokens=*" %%G IN ('DIR /B /S *.user') DO del "%%G"
cd /d %SRC_DEST%\testRtttl
FOR /F "tokens=*" %%G IN ('DIR /B /S *.txt') DO del "%%G"

REM **************************************
REM  zip all
REM **************************************
set outfile=".\bin\rtttlOptimizer v%PRODUCT_VERSION%.%SVN_REVISION% src.zip"

set infiles=
set infiles=%infiles% %SRC_DEST%\rtttl.sln
set infiles=%infiles% %SRC_DEST%\arduino\
set infiles=%infiles% %SRC_DEST%\beep\
set infiles=%infiles% %SRC_DEST%\install\
set infiles=%infiles% %SRC_DEST%\libAnyRtttl\
set infiles=%infiles% %SRC_DEST%\libBitReader\
set infiles=%infiles% %SRC_DEST%\libMidi\
set infiles=%infiles% %SRC_DEST%\libRtttl\
set infiles=%infiles% %SRC_DEST%\rtttlOptimizerGui\
set infiles=%infiles% %SRC_DEST%\testLibRtttl\

cd /d %INSTALL_HOME%
call scripts\portable.bat
pause
