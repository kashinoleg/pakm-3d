#pragma once
#include "vtkCommand.h"

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/timer.h>
#include <wx/dcclient.h>

// vtk includes
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderWindow.h"
#include "vtkSetGet.h"

// wx forward declarations
class wxPaintEvent;
class wxMouseEvent;
class wxTimerEvent;
class wxKeyEvent;
class wxSizeEvent;


class VTK_EXPORT wxVTKRenderWindowInteractor : public wxWindow, public vtkRenderWindowInteractor
{
  DECLARE_DYNAMIC_CLASS(wxVTKRenderWindowInteractor)
  
  public:
    //constructors
    wxVTKRenderWindowInteractor();

    wxVTKRenderWindowInteractor(wxWindow *parent,
                                wxWindowID id,
                                const wxPoint &pos = wxDefaultPosition,
                                const wxSize &size = wxDefaultSize,
                                long style = wxWANTS_CHARS | wxNO_FULL_REPAINT_ON_RESIZE,
                                const wxString &name = wxPanelNameStr);
	//vtk ::New()
    static wxVTKRenderWindowInteractor * New();
    void PrintSelf(ostream& os, vtkIndent indent);

	  //destructor
    ~wxVTKRenderWindowInteractor();

    // vtkRenderWindowInteractor overrides
    void Initialize();
    void Enable();
    bool Enable(bool enable);
    void Disable();
    void Start();
    void UpdateSize(int x, int y);
    int CreateTimer(int timertype);
    int DestroyTimer();
    void TerminateApp() {};

    // event handlers
    void OnPaint(wxPaintEvent &event);
    void OnEraseBackground (wxEraseEvent& event);
    void OnMotion(wxMouseEvent &event);

    void OnButtonDown(wxMouseEvent &event);
    void OnButtonUp(wxMouseEvent &event);

    void OnEnter(wxMouseEvent &event);
    void OnLeave(wxMouseEvent &event);
    void OnKeyDown(wxKeyEvent &event);
    void OnKeyUp(wxKeyEvent &event);

    void OnTimer(wxTimerEvent &event);
    void OnSize(wxSizeEvent &event);
    void OnMouseWheel(wxMouseEvent& event);

    void Render();
    void SetRenderWhenDisabled(int newValue);

    // Description:
    // Prescribe that the window be created in a stereo-capable mode. This
    // method must be called before the window is realized. Default if off.
    vtkGetMacro(Stereo,int);
    vtkBooleanMacro(Stereo,int);
    virtual void SetStereo(int capable);

    // Description:
    // As CaptureMouse could be a problem sometimes on a window box
    // This method allow to set or not the CaptureMouse.
    vtkSetMacro(UseCaptureMouse,int);
    vtkBooleanMacro(UseCaptureMouse,int);

  protected:
    wxTimer timer;
    int ActiveButton;
    int RenderAllowed;
    long GetHandleHack();
    int Stereo;
    
  private:
    long Handle;
    bool Created;
    int RenderWhenDisabled;
    int UseCaptureMouse;

    DECLARE_EVENT_TABLE()
};
