void __fastcall TForm1::Button1Click(TObject *Sender)
{
	TButton* btn2 = new TButton(this); // this � o meu form

	btn2->Parent = this; // ser� o owner e o parent do novo bot�o
	btn2->SetBounds(10, 10, 150, 25); // definindo as fronteiras dentro do form
	btn2->Caption = "Prazer! Sou din�mico!";
	btn2->Visible = true;
} 
