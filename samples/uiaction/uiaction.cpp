/////////////////////////////////////////////////////////////////////////////
// Name:        uiaction.cpp
// Purpose:     wxUIActionSimulator sample
// Author:      Kevin Ollivier
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id$
// Copyright:   (c) Kevin Ollivier, Steven Lamerton
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------
 
// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
 
#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#if wxUSE_UIACTIONSIMULATOR
    #include "wx/uiaction.h"
#endif

// ----------------------------------------------------------------------------
// resources
// ----------------------------------------------------------------------------

// the application icon (under Windows and OS/2 it is in resources and even
// though we could still include the XPM here it would be unused)
#if !defined(__WXMSW__) && !defined(__WXPM__)
    #include "../sample.xpm"
#endif

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// IDs for the controls and the menu commands
enum
{
    // menu items
    Button = 100,
    TextCtrl,
    RunSimulation
};

// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------

// Define a new application type, each program should derive a class from wxApp
class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

#if wxUSE_UIACTIONSIMULATOR

// Define a new frame type: this is going to be our main frame
class MyFrame : public wxFrame
{
public:
    // ctor(s)
    MyFrame(const wxString& title);

    void OnButtonPressed(wxCommandEvent&);
    void OnRunSimulation(wxCommandEvent&);
    
    bool ButtonPressed() const { return m_buttonPressed; }
    bool MenuSelected() const { return m_menuSelected; }
    
private:
    bool m_buttonPressed;
    bool m_menuSelected;

    wxButton* m_button;
    wxTextCtrl* m_text;

    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_BUTTON(Button, MyFrame::OnButtonPressed)
    EVT_MENU(RunSimulation, MyFrame::OnRunSimulation)
END_EVENT_TABLE()

#endif // wxUSE_UIACTIONSIMULATOR

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
    if ( !wxApp::OnInit() )
        return false;

#if wxUSE_UIACTIONSIMULATOR
    MyFrame *frame = new MyFrame("wxUIActionSimulator sample application");
    frame->Show(true);
    
    return true;
#else // !wxUSE_UIACTIONSIMULATOR
    wxLogError("wxUSE_UIACTIONSIMULATOR must be 1 for this sample");
    return false;
#endif // wxUSE_UIACTIONSIMULATOR/!wxUSE_UIACTIONSIMULATOR
}

// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------

#if wxUSE_UIACTIONSIMULATOR

// frame constructor
MyFrame::MyFrame(const wxString& title)
       : wxFrame(NULL, wxID_ANY, title)
{
    SetIcon(wxICON(sample));

    m_buttonPressed = false;
    m_menuSelected = false;

#if wxUSE_MENUS
    // create a menu bar
    wxMenu *fileMenu = new wxMenu;

    fileMenu->Append(wxID_NEW, "&New File...", "Open a new file");
    fileMenu->Append(RunSimulation, "&Run Simulation...", "Run the UI action simulation");

    fileMenu->Append(wxID_EXIT, "E&xit\tAlt-X", "Quit this program");

    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, "&File");

    SetMenuBar(menuBar);
#endif // wxUSE_MENUS

    wxPanel *panel = new wxPanel(this);

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    panel->SetSizer(sizer);

    m_button = new wxButton(panel, Button, "Button");
    sizer->Add(m_button, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    m_text = new wxTextCtrl(panel, TextCtrl, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
    sizer->Add(m_text, 1, wxEXPAND|wxALL, 5);
}


// event handlers

void MyFrame::OnRunSimulation(wxCommandEvent&)
{
    wxUIActionSimulator sim;
    //We add some extra distance to take account of window decorations
    wxPoint globalPoint = m_button->GetScreenPosition() + wxPoint(10, 10);
    sim.MouseMove(globalPoint.x, globalPoint.y);
    sim.MouseClick(wxMOUSE_BTN_LEFT);

    wxYield();

    if (ButtonPressed())
        wxMessageBox("Button automagically pressed!");

    wxYield();

    m_text->SetFocus();
    sim.Char(65);
    sim.Char(65, wxMOD_SHIFT);
    sim.Char(WXK_RETURN);
    sim.Char(90);
    sim.Char(90, wxMOD_SHIFT);
    sim.Char(WXK_RETURN);
    sim.Text("aAbBcC");

    wxYield();
}

void MyFrame::OnButtonPressed(wxCommandEvent&)
{
    m_buttonPressed = true;
}

#endif // wxUSE_UIACTIONSIMULATOR
