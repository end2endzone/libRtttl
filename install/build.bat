@echo off

REM **************************************
REM  Generate NSIS include file
REM **************************************
cscript //nologo "scripts\generate_svn_revision.vbs" "%~dp0." "%~dp0scripts"

REM **************************************
REM  Build Installer
REM **************************************
"C:\Program Files (x86)\NSIS\makensis.exe" /DPLATFORM_INCLUDE=scripts\%PRODUCT_PLATFORM%.nsh main.nsi

pause
