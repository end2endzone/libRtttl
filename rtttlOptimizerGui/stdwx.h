//
//  libRTTTL Library - v1.2 - 05/27/2016
//  Copyright (C) 2016 Antoine Beauchamp
//  The code & updates for the library can be found on http://end2endzone.com
//
// AUTHOR/LICENSE:
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3.0 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License (LGPL-3.0) for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//
// DISCLAIMER:
//  This software is furnished "as is", without technical support, and with no 
//  warranty, express or implied, as to its usefulness for any purpose.
//
// PURPOSE:
//  The library allows one to convert an RTTTL melody to other formats like:
//  - Smaller RTTTL code
//  - Arduino code (tone() and delay() function calls)
//  - Binary RTTTL. See AnyRtttl library at http://end2endzone.com
//
//  The library also support converting arduino code (tones) to RTTTL.
//  
// HISTORY:
// 05/27/2016 v1.2 - First public release.
//

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