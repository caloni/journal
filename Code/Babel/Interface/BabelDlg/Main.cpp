//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Main.h"
#include <windowsx.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Trayicon"
#pragma resource "*.dfm"

#ifndef NOTIFYICON_VERSION_4
#define NOTIFYICON_VERSION_4 4
#endif

#define BABEL_MAX_DEFINITIONS 100

struct CUSTOM_NOTIFYICONDATA
{
    DWORD cbSize;
    HWND hWnd;
    UINT uID;
    UINT uFlags;
    UINT uCallbackMessage;
    HICON hIcon;
    TCHAR szTip[128];
    DWORD dwState;
    DWORD dwStateMask;
    TCHAR szInfo[256];
    union{
        UINT  uTimeout;
        UINT  uVersion;  // Used with Shell_NotifyIcon flag NIM_SETVERSION.
    } DUMMYUNIONNAME;
    TCHAR szInfoTitle[64];
    DWORD dwInfoFlags;
    GUID guidItem;
};


TMainForm *MainForm;
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void TMainForm::CreateTrayIcon()
{
	CUSTOM_NOTIFYICONDATA iconData = { sizeof(iconData) };

   iconData.hWnd = Handle;
   iconData.uFlags = NIF_ICON | NIF_MESSAGE;
   iconData.hIcon = Application->Icon->Handle;
   iconData.uCallbackMessage = WM_TRAYICON_MESSAGE;
   BOOL result = Shell_NotifyIcon(NIM_ADD, (PNOTIFYICONDATA) &iconData);
   DWORD err = GetLastError();

   iconData.uVersion = NOTIFYICON_VERSION_4;
   result = Shell_NotifyIcon(NIM_SETVERSION, (PNOTIFYICONDATA) &iconData);
   err = GetLastError();
}
//---------------------------------------------------------------------------

void TMainForm::DestroyTrayIcon()
{
	NOTIFYICONDATA iconData = { sizeof(iconData) };
   iconData.hWnd = Handle;
   Shell_NotifyIcon(NIM_DELETE, &iconData);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormCreate(TObject *Sender)
{
        m_getTranslator = 0;
        m_freeTranslator = 0;

        CreateTrayIcon();
        Application->OnMinimize = Minimize;

        m_dllTranslator = LoadLibrary(BABELBASE_DLL);

        if( m_dllTranslator )
        {
                m_getTranslator = (Babel::FGetTranslator*)
                        GetProcAddress(m_dllTranslator,
                                BABELBASE_FUNC_GETTRANSLATOR);

                m_freeTranslator = (Babel::FFreeTranslator*)
                        GetProcAddress(m_dllTranslator,
                                BABELBASE_FUNC_FREETRANSLATOR);

                if( ! m_getTranslator || ! m_freeTranslator )
                {
                        ShowMessage("Error getting translator.");
                        Application->Terminate();
                }
                else
                {
                        m_translator = m_getTranslator();
                }
        }
        else
        {
                ShowMessage("Error getting translator library.");
                Application->Terminate();
        }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormDestroy(TObject *Sender)
{
        if( m_freeTranslator )
                m_freeTranslator(m_translator);
        DestroyTrayIcon();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::GoClick(TObject *Sender)
{
   Definition->Clear();

   if( m_translator->GetDefinition(Word->Text.c_str()) )
   {
      Definition->Lines->Add( AnsiString(m_translator->Word()) +
      	". " + AnsiString(m_translator->Definition())
      	+ ". Fonte: " + AnsiString(m_translator->Source()) + ".\r\n" );

      while( m_translator->GetNextDefinition() )
      {
         Definition->Lines->Add( AnsiString(m_translator->Word()) +
            ". " + AnsiString(m_translator->Definition())
            + ". Fonte: " + AnsiString(m_translator->Source()) + ".\r\n" );

         if( Definition->Lines->Count > BABEL_MAX_DEFINITIONS )
         {
            break;
         }
      }
   }
   else Definition->Lines->Add(Word->Text + ": não encontrado.");

   Word->SetFocus();
   Word->SelectAll();
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::Panel1Resize(TObject *Sender)
{
	Word->Width = Panel1->Width - Go->Width;
   Go->Left = Panel1->Width - Go->Width;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
	switch( Key )
   {
   case VK_ESCAPE:
   	Minimize(Sender);
      break;
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Minimize(TObject* Sender)
{
	Hide();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::WordKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
	switch( Key )
   {
   case VK_RETURN:
	   Go->Click();
      break;

   case VK_DOWN:
   	Definition->Perform(EM_SCROLL, SB_LINEDOWN, 0);
      break;

   case VK_UP:
   	Definition->Perform(EM_SCROLL, SB_LINEUP, 0);
      break;

   case VK_PRIOR:
   	Definition->Perform(EM_SCROLL, SB_PAGEUP, 0);
      break;

   case VK_NEXT:
   	Definition->Perform(EM_SCROLL, SB_PAGEDOWN, 0);
      break;
   }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::TrayIconHandler(TMessage& Message)
{
	switch( Message.LParamLo )
   {
   case WM_LBUTTONUP:
      Show();
      break;

   case WM_RBUTTONDOWN:
   	{
         int x = GET_X_LPARAM(Message.WParam);
         int y = GET_Y_LPARAM(Message.WParam);
         TrayIconMenu->Popup(x, y);
      }
      break;
   }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::CloseHandler(TMessage& Message)
{
	Hide();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::AbrirClick(TObject *Sender)
{
	Show();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::SairClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

