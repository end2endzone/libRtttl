#include "stdwx.h"
#include "MainApp.h"
#include "MainFrame.h"
#include "Model.h"
#include "Controller.h"

int MainApp::OnRun()
{
  //Run application main loop
  return wxApp::OnRun();
}

bool MainApp::OnInit()
{
  // call default behaviour (mandatory)
  if (!wxApp::OnInit())
    return false;

  wxInitAllImageHandlers();

  MainFrame * frame = new MainFrame(NULL);
  SetTopWindow(frame);
  frame->Show();

  return true;
}

void MainApp::OnInitCmdLine(wxCmdLineParser& parser)
{
}

bool MainApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
  return true;
}
