//http://rhyous.com/2009/10/13/how-to-find-the-file-and-line-number-of-memory-leaks-using-c-with-wxwidgets-in-visual-studio-2008/
//http://msdn.microsoft.com/en-us/library/e5ewb1h3.aspx

//Note:
//  _CrtDumpMemoryLeaks() can also be used to display memory leaks.

// wxWidgets precompiled / standard headers
//#include <wx/wxprec.h>
#include <wx/wx.h>

// When debugging changes all calls to "new" to be calls to "DEBUG_NEW" allowing for memory leaks to
// give you the file name and line number where it occurred.
#ifdef _DEBUG
  #define _CRTDBG_MAP_ALLOC
  #define _INC_MALLOC
  #include <stdlib.h>
  #include <crtdbg.h>
	#define DEBUG_NEW new(_NORMAL_BLOCK ,__FILE__, __LINE__)
	#define new DEBUG_NEW
#else
	#define DEBUG_NEW new
#endif
