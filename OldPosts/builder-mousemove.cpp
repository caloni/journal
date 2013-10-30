void __fastcall TForm1::FormMouseMove(TObject *Sender, TShiftState Shift,
	int X, int Y)
{
	if( mouseDown )
	{
		Canvas->LineTo(X, Y);
	}
}// Codigo-fonte disponivel no GitHub (Caloni/Caloni.com.br) 
