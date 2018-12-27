#include "gtest.h"
#include <string>
#include <Windows.h>

void showAssertionErrorMessage(const char * iAssertionType, const char * iExpr, const char * iSubError, const char * iFile, int iLine)
{
  char buffer[10240];
  sprintf(buffer, "The expression\n%s(%s)\nfailed at line %d of file\n%s\n%s\nPress OK to continue.",
    iAssertionType,
    iExpr,
    iLine,
    iFile,
    iSubError);
  
  printf("%s\n", buffer);
  ::MessageBox(NULL, buffer, "ASSERTION FAILED", MB_OK | MB_ICONERROR);

  exit(1);
}
