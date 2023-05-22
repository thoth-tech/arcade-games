//Extended application on https://imron02.wordpress.com/2014/09/26/c-simple-form-login-using-wxwidgets/
//Changes made was in the function  Onlogin(), and addition of getPassword(), and pasword hashing checks.
#include "login.h"
#include <wx/wx.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <functional>
#include <algorithm>
#include <iterator>

using std::cout; using std::endl;
using std::string; using std::reverse;


FormLogin::FormLogin(const wxString& title)
: wxFrame((wxFrame *) NULL, wxID_ANY, title, wxPoint(wxID_ANY, wxID_ANY), wxSize(340, 150), 
wxSYSTEM_MENU | wxMINIMIZE_BOX | wxCLOSE_BOX | wxCAPTION | wxCLIP_CHILDREN)
{
    SetIcon(wxIcon(wxT("images/splashkit.png")));
    Centre();
    wxPanel *panel = new wxPanel(this, wxID_ANY);
 
    wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
 
    wxBoxSizer *hbox1 = new wxBoxSizer(wxHORIZONTAL);
    m_usernameLabel = new wxStaticText(panel, wxID_ANY, wxT("Username: "), wxDefaultPosition, wxSize(70, -1));
    hbox1->Add(m_usernameLabel, 0);
 
    m_usernameEntry = new wxTextCtrl(panel, wxID_ANY);
    hbox1->Add(m_usernameEntry, 1);
    vbox->Add(hbox1, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);
 
    wxBoxSizer *hbox2 = new wxBoxSizer(wxHORIZONTAL);
    m_passwordLabel = new wxStaticText(panel, wxID_ANY, wxT("Password: "), wxDefaultPosition, wxSize(70, -1));
    hbox2->Add(m_passwordLabel, 0);
 
    m_passwordEntry = new wxTextCtrl(panel, wxID_ANY, wxString(""),
        wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
    hbox2->Add(m_passwordEntry, 1);
    vbox->Add(hbox2, 0, wxEXPAND | wxLEFT | wxTOP | wxRIGHT, 10);
 
    wxBoxSizer *hbox3 = new wxBoxSizer(wxHORIZONTAL);
    m_buttonLogin = new wxButton(panel, BUTTON_Login, wxT("Login"));
    hbox3->Add(m_buttonLogin);
 
    m_buttonQuit = new wxButton(panel, wxID_EXIT, ("Quit"));
    hbox3->Add(m_buttonQuit);
    vbox->Add(hbox3, 0, wxALIGN_RIGHT | wxTOP | wxRIGHT | wxBOTTOM, 10);
 
    panel->SetSizer(vbox);
    Centre();
}
 
void FormLogin::OnQuit(wxCommandEvent& event)
{
     Close(true);
}

string getPassword(){
        
    std::ifstream inFile;
    std::string password;

    inFile.open("users.txt");
    if (!inFile) {
        std::cout << "Unable to open file";
        exit(1); // terminate with error
    }
    else { //checking whether the file is open
      while(getline(inFile, password)){ //read data from file object and put it into string.
         //std::cout << tp; //print the data of the string
      }
      inFile.close();//close the file object.
   }
   return password;
}

 
void FormLogin::OnLogin(wxCommandEvent& event)
{

    wxString username = m_usernameEntry->GetValue(); //get user inputs
    wxString password = m_passwordEntry->GetValue();
    
    string pass = getPassword();
    std::string str = m_usernameEntry->GetValue().ToStdString()+m_passwordEntry->GetValue().ToStdString(); //combine username and password
    std::hash<std::string> hash_fn;
    std::size_t str_hash = hash_fn(str);//hash the concat strinf

    std::string combine = std::to_string(str_hash); //coonvert to string


    if (! username.empty() || ! password.empty()) {
        if(combine.compare(pass) == 0){
            wxMessageBox(wxT("Hello Admin"), wxT("Warning!"), wxICON_INFORMATION);
            Close(true);
        }else{
            wxMessageBox(wxT("Username or password is incorrect"), wxT("Warning!"), wxICON_WARNING);
        }
    }
    else {
        wxMessageBox(wxT("Username or password must not empty"), wxT("Warning!"), wxICON_WARNING);
    }
}
 
FormLogin::~FormLogin() {}
 
BEGIN_EVENT_TABLE(FormLogin, wxFrame)
EVT_BUTTON(wxID_EXIT, FormLogin::OnQuit)
EVT_BUTTON(BUTTON_Login, FormLogin::OnLogin)
END_EVENT_TABLE()