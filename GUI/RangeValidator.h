#pragma once
#include "wx/wx.h"
#include "Convert.h"

/// Class to validate a wxTextCtrl input.  Checks input is within upper and lower bounds
/// Taken from wxWidgets user forum: http://forums.wxwidgets.org/viewtopic.php?t=12582

class RangeValidator : public wxValidator {
	DECLARE_DYNAMIC_CLASS(RangeValidator)

	DECLARE_EVENT_TABLE()
protected:
	double m_min;
	double m_max;
	wxString* m_value;
public:
	/// Constructor
	RangeValidator(wxString* value, double min, double max);
	RangeValidator() {};


	/// Copy-Constructor
	RangeValidator(const RangeValidator& val);

	/// Returns a copy of this validator
	virtual RangeValidator* Clone(void) const { return (new RangeValidator(*this)); };

	/// validate. true when is correct
	virtual bool Validate(wxWindow *parent);

	/// transfer the data to the window
	virtual bool TransferToWindow(void);

	/// transfer the data from the window
	virtual bool TransferFromWindow(void);
protected:
	/// char-event callback
	void OnChar(wxKeyEvent& event);

	/// check for valid pointers
	virtual bool CheckValidator(void);

	/// check, if the given value is a number in the defined range. The second
	/// parameter specifies, from where the method is called. It will return "0"
	/// if the given value is in the range, a number greater than zero if it is
	/// too big and a number less than zero if it is to small.
	virtual int CheckValue(const wxString& value, bool validate);
};
