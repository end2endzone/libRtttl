#pragma once

#include <wx/wx.h>
#include <wx/cmdline.h>

class MainApp: public wxApp
{
public:
  virtual int OnRun();
  virtual bool OnInit();
  virtual void OnInitCmdLine(wxCmdLineParser& parser);
  virtual bool OnCmdLineParsed(wxCmdLineParser& parser);
private:
  int mExitCode;
};
