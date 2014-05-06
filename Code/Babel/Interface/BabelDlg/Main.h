//---------------------------------------------------------------------------

#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include "..\..\Libraries\DLLs\BabelBase\BabelBase.h"

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include "Trayicon.h"
#include <Menus.hpp>

#define WM_TRAYICON_MESSAGE WM_USER + 1

//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
	TRichEdit *Definition;
	TPanel *Panel1;
	TEdit *Word;
	TBitBtn *Go;
	TPopupMenu *TrayIconMenu;
	TMenuItem *Abrir;
	TMenuItem *Sair;
	void __fastcall FormDestroy(TObject *Sender);
   void __fastcall FormCreate(TObject *Sender);
	void __fastcall GoClick(TObject *Sender);
	void __fastcall Panel1Resize(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall WordKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall AbrirClick(TObject *Sender);
	void __fastcall SairClick(TObject *Sender);
protected:
	void __fastcall TrayIconHandler(TMessage& Message);
	void __fastcall CloseHandler(TMessage& Message);
   void __fastcall Minimize(TObject* Sender);
BEGIN_MESSAGE_MAP
	MESSAGE_HANDLER(WM_TRAYICON_MESSAGE, TMessage, TrayIconHandler)
   MESSAGE_HANDLER(WM_CLOSE, TMessage, CloseHandler)
END_MESSAGE_MAP(TForm)

private:	// User declarations
			void CreateTrayIcon();
         void DestroyTrayIcon();
        HMODULE m_dllTranslator;
        Babel::FGetTranslator* m_getTranslator;
        Babel::FFreeTranslator* m_freeTranslator;
        Babel::ITranslator* m_translator;
public:		// User declarations
        __fastcall TMainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
