; NSIS installer script for DnD Text Adventure Game
Name "D&D Text Adventure Game"
OutFile "dnd-game-installer.exe"
InstallDir "$PROGRAMFILES\\DnD Game"
SetCompress lzma

Page directory
Page instfiles

Section "Install"
  SetOutPath "$INSTDIR"
  ; Include all files from the release/windows folder
  File /r "release\\windows\\*.*"

  ; Create a desktop shortcut to the start script if present
  CreateDirectory "$SMPROGRAMS\\DnD Game"
  CreateShortCut "$DESKTOP\\D&D Text Adventure.lnk" "$INSTDIR\\start_windows.bat"
SectionEnd

Section "Uninstall"
  Delete "$INSTDIR\\start_windows.bat"
  Delete "$INSTDIR\\dnd_game.exe"
  Delete "$DESKTOP\\D&D Text Adventure.lnk"
  RMDir /r "$INSTDIR"
SectionEnd
