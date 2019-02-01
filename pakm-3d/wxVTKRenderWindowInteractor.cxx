#include "wxVTKRenderWindowInteractor.h"

#define ID_wxVTKRenderWindowInteractor_TIMER 1001

IMPLEMENT_DYNAMIC_CLASS(wxVTKRenderWindowInteractor, wxWindow)

BEGIN_EVENT_TABLE(wxVTKRenderWindowInteractor, wxWindow)
	//refresh window by doing a Render
	EVT_PAINT(wxVTKRenderWindowInteractor::OnPaint)
	EVT_ERASE_BACKGROUND(wxVTKRenderWindowInteractor::OnEraseBackground)
	EVT_MOTION(wxVTKRenderWindowInteractor::OnMotion)

	//Bind the events to the event converters
	EVT_LEFT_DOWN(wxVTKRenderWindowInteractor::OnButtonDown)
	EVT_MIDDLE_DOWN(wxVTKRenderWindowInteractor::OnButtonDown)
	EVT_RIGHT_DOWN(wxVTKRenderWindowInteractor::OnButtonDown)
	EVT_LEFT_UP(wxVTKRenderWindowInteractor::OnButtonUp)
	EVT_MIDDLE_UP(wxVTKRenderWindowInteractor::OnButtonUp)
	EVT_RIGHT_UP(wxVTKRenderWindowInteractor::OnButtonUp)

	EVT_ENTER_WINDOW(wxVTKRenderWindowInteractor::OnEnter)
	EVT_LEAVE_WINDOW(wxVTKRenderWindowInteractor::OnLeave)
	EVT_MOUSEWHEEL(wxVTKRenderWindowInteractor::OnMouseWheel)

	EVT_CHAR(wxVTKRenderWindowInteractor::OnKeyDown)
	EVT_KEY_UP(wxVTKRenderWindowInteractor::OnKeyUp)

	EVT_TIMER(ID_wxVTKRenderWindowInteractor_TIMER, wxVTKRenderWindowInteractor::OnTimer)
	EVT_SIZE(wxVTKRenderWindowInteractor::OnSize)
END_EVENT_TABLE()

wxVTKRenderWindowInteractor::wxVTKRenderWindowInteractor() : vtkRenderWindowInteractor(), wxWindow()
	, timer(this, ID_wxVTKRenderWindowInteractor_TIMER)
	, ActiveButton(wxEVT_NULL)
	, RenderAllowed(0)
	, Stereo(0)
	, Handle(0)
	, Created(true)
	, RenderWhenDisabled(1)
	, UseCaptureMouse(0)
{
	this->RenderWindow = NULL;
	this->SetRenderWindow(vtkRenderWindow::New());
	this->RenderWindow->Delete();
}

wxVTKRenderWindowInteractor::wxVTKRenderWindowInteractor(wxWindow *parent,
	wxWindowID id, const wxPoint &pos, const wxSize &size, long style, const wxString &name)
	: vtkRenderWindowInteractor(), wxWindow(parent, id, pos, size, style, name)
	, timer(this, ID_wxVTKRenderWindowInteractor_TIMER)
	, ActiveButton(wxEVT_NULL)
	, RenderAllowed(0)
	, Stereo(0)
	, Handle(0)
	, Created(true)
	, RenderWhenDisabled(1)
	, UseCaptureMouse(0)
{
	this->RenderWindow = NULL;
	this->SetRenderWindow(vtkRenderWindow::New());
	this->RenderWindow->Delete();
}

wxVTKRenderWindowInteractor::~wxVTKRenderWindowInteractor() {}

wxVTKRenderWindowInteractor * wxVTKRenderWindowInteractor::New() {
	// we don't make use of the objectfactory, because we're not registered
	return new wxVTKRenderWindowInteractor;
}

void wxVTKRenderWindowInteractor::Initialize() {
	int *size = RenderWindow->GetSize();
	// enable everything and start rendering
	Enable();
	//RenderWindow->Start();

	// set the size in the render window interactor
	Size[0] = size[0];
	Size[1] = size[1];

	// this is initialized
	Initialized = 1;
}

void wxVTKRenderWindowInteractor::Enable() {
	// if already enabled then done
	if (Enabled) {
		return;
	}
	// that's it
	Enabled = 1;
	Modified();
}

bool wxVTKRenderWindowInteractor::Enable(bool enable) {
	return wxWindow::Enable(enable);
}

void wxVTKRenderWindowInteractor::Disable() {
	// if already disabled then done
	if (!Enabled) {
		return;
	}
	// that's it (we can't remove the event handler like it should be...)
	Enabled = 0;
	Modified();
}

void wxVTKRenderWindowInteractor::Start() {
	// the interactor cannot control the event loop
	vtkErrorMacro(<< "wxVTKRenderWindowInteractor::Start() interactor cannot control event loop.");
}

void wxVTKRenderWindowInteractor::UpdateSize(int x, int y) {
	if (RenderWindow) {
		// if the size changed tell render window
		if (x != Size[0] || y != Size[1]) {
			// adjust our (vtkRenderWindowInteractor size)
			Size[0] = x;
			Size[1] = y;
			// and our RenderWindow's size
			RenderWindow->SetSize(x, y);
		}
	}
}

int wxVTKRenderWindowInteractor::CreateTimer(int WXUNUSED(timertype)) {
	// it's a one shot timer
	if (!timer.Start(10, TRUE)) {
		assert(false);
	}
	return 1;
}

int wxVTKRenderWindowInteractor::DestroyTimer() {
	// do nothing
	return 1;
}

void wxVTKRenderWindowInteractor::OnTimer(wxTimerEvent& WXUNUSED(event)) {
	if (!Enabled) {
		return;
	}
	InvokeEvent(vtkCommand::TimerEvent, NULL);
}

//---------------------------------------------------------------------------
// NOTE on implementation:
// Bad luck you ended up in the only tricky place of this code.
// A few note, wxWidgets still refuse to provide such convenient method
// so I have to maintain it myself, eventhough this is completely integrated
// in wxPython...
// Anyway if this happen to break for you then compare to a recent version of wxPython
// and look for the function long wxPyGetWinHandle(wxWindow* win)
// in wxPython/src/helpers.cpp
long wxVTKRenderWindowInteractor::GetHandleHack() {
	//helper function to hide the MSW vs GTK stuff
	long handle_tmp = 0;
	handle_tmp = (long)this->GetHandle();
	return handle_tmp;
}

void wxVTKRenderWindowInteractor::OnPaint(wxPaintEvent& WXUNUSED(event)) {
	//must always be here
	wxPaintDC pDC(this);

	//do it here rather than in the cstor: this is safer.
	if (!Handle) {
		Handle = GetHandleHack();
		RenderWindow->SetWindowId(reinterpret_cast<void *>(Handle));
		RenderWindow->SetParentId(reinterpret_cast<void *>(this->GetParent()->GetHWND()));
	}
	// get vtk to render to the wxWindows
	Render();
}

void wxVTKRenderWindowInteractor::OnEraseBackground(wxEraseEvent &event) {
	//turn off background erase to reduce flickering on MSW
	event.Skip(false);
}

void wxVTKRenderWindowInteractor::OnSize(wxSizeEvent& WXUNUSED(event)) {
	int w, h;
	GetClientSize(&w, &h);
	UpdateSize(w, h);
	if (!Enabled) {
		return;
	}
	InvokeEvent(vtkCommand::ConfigureEvent, NULL);
}

void wxVTKRenderWindowInteractor::OnMotion(wxMouseEvent &event)
{
	if (!Enabled)
	{
		return;
	}
	SetEventInformationFlipY(event.GetX(), event.GetY(), event.ControlDown(), event.ShiftDown(), '\0', 0, NULL);
	InvokeEvent(vtkCommand::MouseMoveEvent, NULL);
}

void wxVTKRenderWindowInteractor::OnEnter(wxMouseEvent &event) {
	if (!Enabled) {
		return;
	}
	SetEventInformationFlipY(event.GetX(), event.GetY(), event.ControlDown(), event.ShiftDown(), '\0', 0, NULL);
	InvokeEvent(vtkCommand::EnterEvent, NULL);
}

void wxVTKRenderWindowInteractor::OnLeave(wxMouseEvent &event) {
	if (!Enabled) {
		return;
	}
	SetEventInformationFlipY(event.GetX(), event.GetY(), event.ControlDown(), event.ShiftDown(), '\0', 0, NULL);
	InvokeEvent(vtkCommand::LeaveEvent, NULL);
}

void wxVTKRenderWindowInteractor::OnKeyDown(wxKeyEvent &event) {
	if (!Enabled) {
		return;
	}
	int keycode = event.GetKeyCode();
	char key = '\0';
	if (keycode < 256)
	{
		// TODO: Unicode in non-Unicode mode ??
		key = (char)keycode;
	}

	SetEventInformationFlipY(event.GetX(), event.GetY(), event.ControlDown(), event.ShiftDown(), key, 0, NULL);

	InvokeEvent(vtkCommand::KeyPressEvent, NULL);
	InvokeEvent(vtkCommand::CharEvent, NULL);
	event.Skip();
}

void wxVTKRenderWindowInteractor::OnKeyUp(wxKeyEvent &event) {
	if (!Enabled) {
		return;
	}
	int keycode = event.GetKeyCode();
	char key = '\0';
	if (keycode < 256) {
		// TODO: Unicode in non-Unicode mode ??
		key = (char)keycode;
	}

	SetEventInformationFlipY(event.GetX(), event.GetY(), event.ControlDown(), event.ShiftDown(), key, 0, NULL);
	InvokeEvent(vtkCommand::KeyReleaseEvent, NULL);
	event.Skip();
}

void wxVTKRenderWindowInteractor::OnButtonDown(wxMouseEvent &event)
{
	if (!Enabled || (ActiveButton != wxEVT_NULL)) {
		return;
	}
	ActiveButton = event.GetEventType();
	SetEventInformationFlipY(event.GetX(), event.GetY(), event.ControlDown(), event.ShiftDown(), '\0', 0, NULL);

	if (event.RightDown()) {
		InvokeEvent(vtkCommand::RightButtonPressEvent, NULL);
	}
	else if (event.LeftDown()) {
		InvokeEvent(vtkCommand::LeftButtonPressEvent, NULL);
	}
	else if (event.MiddleDown()) {
		InvokeEvent(vtkCommand::MiddleButtonPressEvent, NULL);
	}
	//save the button and capture mouse until the button is released
	//Only if :
	//1. it is possible (WX_USE_X_CAPTURE)
	//2. user decided to.
	if ((ActiveButton != wxEVT_NULL) && UseCaptureMouse) {
		CaptureMouse();
	}
}

void wxVTKRenderWindowInteractor::OnButtonUp(wxMouseEvent &event)
{
	//EVT_xxx_DOWN == EVT_xxx_UP - 1
	//This is only needed if two mouse buttons are pressed at the same time.
	//In wxWindows 2.4 and later: better use of wxMOUSE_BTN_RIGHT or 
	//wxEVT_COMMAND_RIGHT_CLICK
	if (!Enabled || (ActiveButton != (event.GetEventType() - 1))) {
		return;
	}
	SetEventInformationFlipY(event.GetX(), event.GetY(), event.ControlDown(), event.ShiftDown(), '\0', 0, NULL);

	if (ActiveButton == wxEVT_RIGHT_DOWN)
	{
		InvokeEvent(vtkCommand::RightButtonReleaseEvent, NULL);
	}
	else if (ActiveButton == wxEVT_LEFT_DOWN)
	{
		InvokeEvent(vtkCommand::LeftButtonReleaseEvent, NULL);
	}
	else if (ActiveButton == wxEVT_MIDDLE_DOWN)
	{
		InvokeEvent(vtkCommand::MiddleButtonReleaseEvent, NULL);
	}
	//if the ActiveButton is realeased, then release mouse capture
	if ((ActiveButton != wxEVT_NULL) && UseCaptureMouse)
	{
		ReleaseMouse();
	}
	ActiveButton = wxEVT_NULL;
}

void wxVTKRenderWindowInteractor::OnMouseWheel(wxMouseEvent& event)
{
	// Mouse wheel was only added after VTK 4.4 (I think...)
	  //Set vtk event information ... The numebr of wheel rotations is stored in
	  //the x varible.  y varible is zero
	SetEventInformationFlipY(event.GetWheelRotation() / event.GetWheelDelta(), 0, event.ControlDown(), event.ShiftDown(), '\0', 0, NULL);
	if (event.GetWheelRotation() > 0)
	{
		//Send event to VTK
		InvokeEvent(vtkCommand::MouseWheelForwardEvent, NULL);
	}
	else
	{
		//Send event to VTK
		InvokeEvent(vtkCommand::MouseWheelBackwardEvent, NULL);
	}
}

void wxVTKRenderWindowInteractor::Render() {
	RenderAllowed = 1;
	if (!RenderWhenDisabled) {
		//the user doesn't want us to render when the toplevel frame
		//is disabled - first find the top level parent
		wxWindow *topParent = wxGetTopLevelParent(this);
		if (topParent) {
			//if it exists, check whether it's enabled
			//if it's not enabeld, RenderAllowed will be false
			RenderAllowed = topParent->IsEnabled();
		}
	}
	if (RenderAllowed) {
		if (Handle && (Handle == GetHandleHack())) {
			RenderWindow->Render();
		}
		else if (GetHandleHack()) {
			//this means the user has reparented us; let's adapt to the
			//new situation by doing the WindowRemap dance
			//store the new situation
			Handle = GetHandleHack();
			RenderWindow->SetNextWindowId(reinterpret_cast<void *>(Handle));
			RenderWindow->WindowRemap();
			RenderWindow->Render();
		}
	}
}

void wxVTKRenderWindowInteractor::SetRenderWhenDisabled(int newValue) {
	//Change value of __RenderWhenDisabled ivar.
	//If __RenderWhenDisabled is false (the default), this widget will not
	//call Render() on the RenderWindow if the top level frame (i.e. the
	//containing frame) has been disabled.

	//This prevents recursive rendering during wxSafeYield() calls.
	//wxSafeYield() can be called during the ProgressMethod() callback of
	//a VTK object to have progress bars and other GUI elements updated -
	//it does this by disabling all windows (disallowing user-input to
	//prevent re-entrancy of code) and then handling all outstanding
	//GUI events.

	//However, this often triggers an OnPaint() method for wxVTKRWIs,
	//resulting in a Render(), resulting in Update() being called whilst
	//still in progress.

	RenderWhenDisabled = newValue ? true : false;
}

// Set the variable that indicates that we want a stereo capable window
// be created. This method can only be called before a window is realized.
void wxVTKRenderWindowInteractor::SetStereo(int capable) {
	if (Stereo != capable) {
		Stereo = capable;
		RenderWindow->StereoCapableWindowOn();
		RenderWindow->SetStereoTypeToCrystalEyes();
		Modified();
	}
}

void wxVTKRenderWindowInteractor::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os, indent);
}
