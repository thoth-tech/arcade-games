#include "main.h"
#include "login.h"
 
 
bool MainApp::OnInit()
{
 
    FormLogin *formLogin = new FormLogin(wxT("Splashkit"));
    formLogin->Show(true);
 
    return true;
}
 
wxIMPLEMENT_APP(MainApp);