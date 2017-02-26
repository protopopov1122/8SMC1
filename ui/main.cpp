#define __WXMSW__
#define WXUSINGDLL

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


class _8SMC1App: public wxApp
{
	public:
		virtual bool OnInit();
};

class _8SMC1Frame: public wxFrame
{
	public:
		_8SMC1Frame();
};

wxIMPLEMENT_APP(_8SMC1App);

bool _8SMC1App::OnInit()
{
    _8SMC1Frame *frame = new _8SMC1Frame();
    frame->Show(true);
    return true;
}

_8SMC1Frame::_8SMC1Frame()
        : wxFrame(NULL, wxID_ANY, "8SMC1")
{
    CreateStatusBar();
    SetStatusText("8SMC1 test ui");
}