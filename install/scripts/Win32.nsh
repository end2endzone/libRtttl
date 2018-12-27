!define PRODUCT_PLATFORM "win32"
!define PROGRAMFILES_PLATFORM "$PROGRAMFILES"

Function CheckOsRequirements
  ${if} ${RunningX64}
    MessageBox MB_OK|MB_ICONSTOP "You must be running a 32 bits OS to install $(^Name)."
    Abort
  ${else}
  ${endif}
FunctionEnd
