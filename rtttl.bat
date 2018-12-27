@echo off

set WXWIN=%CD%\ThirdParties\wxWidgets\2.8.10

set WXWIDGET_LIB_FOLDER=%WXWIN%\lib

set WXWIDGET_INCLUDE_FOLDER_1=%WXWIN%\include
::set WXWIDGET_INCLUDE_FOLDER_2=%WXWIN%\lib\vc_lib\mswd

set WXWIDGET_DEBUG_LIBRARIES=wxbase28d_xml.lib wxmsw28d_adv.lib wxmsw28d_html.lib wxmsw28d_core.lib wxbase28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib winmm.lib comctl32.lib rpcrt4.lib wsock32.lib odbc32.lib
set WXWIDGET_RELEASE_LIBRARIES=wxbase28_xml.lib wxmsw28_adv.lib wxmsw28_html.lib wxmsw28_core.lib wxbase28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib winmm.lib comctl32.lib rpcrt4.lib wsock32.lib odbc32.lib

start "" rtttl.sln
