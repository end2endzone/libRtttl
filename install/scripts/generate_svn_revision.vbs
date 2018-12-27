Option Explicit

'Read input arguments
dim subversionPath
dim outputPath
subversionPath = WScript.Arguments.Item(0)
outputPath = WScript.Arguments.Item(1)

'**************************************
' Execute svn info
'**************************************
dim list
Set list = CreateObject("System.Collections.ArrayList")
dim command
command = "svn info """ & subversionPath & """"
dim objShell
Set objShell = WScript.CreateObject("WScript.Shell")
dim objExec
Set objExec = objShell.Exec(command)
Do
    dim line
	line = objExec.StdOut.ReadLine()	
	'Wscript.Echo "-->" & line
	list.Add line
Loop While Not objExec.Stdout.atEndOfStream

'**************************************
' Search for a revision number
'**************************************
dim revision
revision = 0
dim i
for i = 0 to list.Count -1
    'dim line
	line = list.Item(i)
	if (InStr(line,"Revision:")) then
		revision = mid(line, 11,9999)
	end if
next

'**************************************
' Generate output files path
'**************************************
dim outputCpp
outputCpp = outputPath & "\" & "svn_revision.h"
dim outputNSIS
outputNSIS = outputPath & "\" & "svn_revision.nsh"
dim outputBat
outputBat = outputPath & "\" & "svn_revision.bat"

'**************************************
' Generate NSIS include file
'**************************************
dim objFSO
Set objFSO = CreateObject("Scripting.FileSystemObject")
dim objFile
Set objFile = objFSO.CreateTextFile(outputNSIS,True)
objFile.Write "!define SVN_REVISION """ & revision & """" & vbCrLf
objFile.Close
Set objFile = nothing

'**************************************
' Generate C++ include file
'**************************************
Set objFile = objFSO.CreateTextFile(outputCpp,True)
objFile.Write "#define SVN_REVISION " & revision & vbCrLf
objFile.Write "#define SVN_REVISION_STR """ & revision & """" & vbCrLf
objFile.Close
Set objFile = nothing

'**************************************
' Generate BATCH include file
'**************************************
Set objFile = objFSO.CreateTextFile(outputBat,True)
objFile.Write "@set SVN_REVISION=" & revision & vbCrLf
objFile.Close
Set objFile = nothing





'**************************************
' List functions
'**************************************
'list.Sort
'list.Reverse
'wscript.echo list.Count                 ' --> 3
'wscript.echo list.Item(0)               ' --> Pear
'wscript.echo list.IndexOf("Apple", 0)   ' --> 2
'wscript.echo join(list.ToArray(), ", ") ' --> Pear, Banana, Apple

'**************************************
' File read sample
'**************************************
'Const ForReading = 1 
'Set objFSO = CreateObject("Scripting.FileSystemObject")
'Set objFile = objFSO.OpenTextFile (filePath, ForReading)
'revision = 0
'Do Until objFile.AtEndOfStream
'    strNextLine = objFile.Readline
'	if (InStr(strNextLine,"Revision:")) then
'		revision = mid(strNextLine, 11,9999)
'		'Wscript.Echo revision
'	end if
'Loop
'objFile.Close
'Set objFile = nothing

