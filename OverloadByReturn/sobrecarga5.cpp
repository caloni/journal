struct CreateNewGUID
{
   operator wstring () { ... } // a convers�o � a "chamada da fun��o".

   operator GUID () { ... } // E como existem duas convers�es... sobrecarga!
};