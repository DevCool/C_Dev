'VBScript created by Philip "5n4k3" Simonson
'Used for writing key value to registry run
'--------------------------------------------

Public Sub Main()
	Dim Input
	If WScript.Arguments.Count > 1 And WScript.Arguments.Count < 4 Then
		If WScript.Arguments(0) = "w" Then
			WriteValue ".", WScript.Arguments(1), WScript.Arguments(2)
		ElseIf WScript.Arguments.Count = 2 And WScript.Arguments(0) = "d" Then
			DeleteValue ".", WScript.Arguments(1)
		Else
			WScript.Echo "Usage: crv.vbs [w][d] <KeyName> [FilePath]"
		End If
	Else
		WScript.Echo "Usage: crv.vbs [w][d] <KeyName> [FilePath]"
	End If
End Sub

Const HKEY_CURRENT_USER = &H8000001

Private Sub WriteValue(ByVal strComputer, ByVal strInput, ByVal strInputTwo)
	'Write key value to registry
	Set objRegistry = GetObject("winmgmts:\\" & strComputer & "\root\default:StdRegProv")
	strKeyPath = "SOFTWARE\Windows\CurrentVersion\Run"
	strValueName = strInput
	objRegistry.SetStringValue HKEY_CURRENT_USER, strKeyPath, strValueName, strInputTwo
End Sub

Private Sub DeleteValue(ByVal strComputer, ByVal strInput)
	'Delete key value in registry
	Set objRegistry = GetObject("winmgmts:\\" & strComputer & "\root\default:StdRegProv")
	strKeyPath = "SOFTWARE\Windows\CurrentVersion\Run"
	strValueName = strInput
	objRegistry.DeleteValue HKEY_CURRENT_USER, strKeyPath, strValueName
End Sub

Call Main