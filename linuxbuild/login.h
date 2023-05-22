#ifndef FORMLOGIN_H
#define FORMLOGIN_H
 
#include "wx/wx.h"
 
class FormLogin : public wxFrame
{
public:
    FormLogin(const wxString& title);
 
    // Destructor
    virtual ~FormLogin();
 
private:
    wxStaticText* m_usernameLabel;
    wxStaticText* m_passwordLabel;
    wxTextCtrl* m_usernameEntry;
    wxTextCtrl* m_passwordEntry;
    wxButton* m_buttonLogin;
    wxButton* m_buttonQuit;
    wxMessageDialog *dlg;
 
private:
    void OnQuit(wxCommandEvent& event);
    void OnLogin(wxCommandEvent& event);
 
private:
    DECLARE_EVENT_TABLE()
 
    enum
    {
        BUTTON_Login = wxID_HIGHEST + 1
    };
};
#endif // FORMLOGIN_H