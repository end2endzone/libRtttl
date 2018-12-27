@ECHO OFF
:: %1 is $(OutDir)
:: %2 is $(PlatformName)
:: output files generated = $(ProjectDir)bin\flv2mkv v1.2.SVN_REVISION $(PlatformName) Setup.zip

IF NOT DEFINED outfile GOTO :missingargument
IF NOT DEFINED infiles GOTO :missingargument
::IF "%outfile%" == "" GOTO :missingargument
::IF "%infiles%" == "" GOTO :missingargument

echo Generating portable install %outfile%
echo using the following input files:
echo %infiles%
echo.
call "C:\Program Files\7-Zip\7z.exe" a -bd %outfile% %infiles%
goto :eof

:missingargument
echo Missing argument for Portable install.
echo Please specify %%outfile%% and %%infiles%% (space separated file list)
echo.

:eof
