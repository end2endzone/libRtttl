/**********************************************************************************
 * MIT License
 * 
 * Copyright (c) 2018 Antoine Beauchamp
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *********************************************************************************/

// wxWidgets precompiled / standard headers
//#include <wx/wxprec.h>
#include <wx/wx.h>

// When debugging changes all calls to "new" to be calls to "DEBUG_NEW" allowing for memory leaks to
// give you the file name and line number where it occurred.
#ifdef _WIN32
#ifdef _DEBUG
//Note:
//  http://rhyous.com/2009/10/13/how-to-find-the-file-and-line-number-of-memory-leaks-using-c-with-wxwidgets-in-visual-studio-2008/
//  http://msdn.microsoft.com/en-us/library/e5ewb1h3.aspx
//  _CrtDumpMemoryLeaks() can also be used to display memory leaks.
  #define _CRTDBG_MAP_ALLOC
  #define _INC_MALLOC
  #include <stdlib.h>
  #include <crtdbg.h>
	#define DEBUG_NEW new(_NORMAL_BLOCK ,__FILE__, __LINE__)
	#define new DEBUG_NEW
#else
	#define DEBUG_NEW new
#endif //_DEBUG
#endif //_WIN32