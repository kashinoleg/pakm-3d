#pragma once
#include "mymath.h"
#include "TexGenApp.h"
using namespace std;

namespace TexGen
{
	class CTexGenRenderer;
}

/// Shortcut function to get an instance of CTexGenRenderer associated with a particular window (if NULL the current)
TexGen::CTexGenRenderer *GetRenderWindow(string WindowName = "");
/// Shortcut function to create a new render window
TexGen::CTexGenRenderer *CreateRenderWindow(string WindowName = "");
/// Shortcut function to delete a new render window
bool DeleteRenderWindow(string WindowName = "");
/// Function to refresh the current render (may be necessary after performing Python commands which modify the Textile)
bool Refresh(string WindowName = "");
/// Function to capture standard output and direct it to the log window
void ReceivePythonOutput(string Output, string Type);
