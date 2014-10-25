//...
private:
	bool mouseDown; // essa vari�vel guarda o estado do mouse...
//...

__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	mouseDown = false; // ... e � importante inici�-la
}

void __fastcall TForm1::FormMouseUp(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y)
{
	mouseDown = false;
}

void __fastcall TForm1::FormMouseDown(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y)
{
	Canvas->PenPos = TPoint(X, Y); // mais tarde veremos o porqu� disso
	mouseDown = true;
} 
