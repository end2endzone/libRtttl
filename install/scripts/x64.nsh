!define PRODUCT_PLATFORM "x64"
!define PROGRAMFILES_PLATFORM "$PROGRAMFILES64"

Function CheckOsRequirements
  ${if} ${RunningX64}
  ${else}
    MessageBox MB_OK|MB_ICONSTOP "You must be running a 64 bits OS to install $(^Name)."
    Abort
  ${endif}
FunctionEnd
