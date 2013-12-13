GUID guid;
wstring guidS;

CreateNewGUID(guidS); // chama void CreateNewGUID(wstring&)
CreateNewGUID(guid); // chama void CreateNewGUID(GUID&) (o compilador sabe disso)
// Codigo-fonte disponivel no GitHub (<a href="https://github.com/Caloni/Caloni.com.br">Caloni/Caloni.com.br)</a>.
