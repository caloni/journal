// Papai, o que que � isso?
// Ora, filho, apenas uma defini��o de estrutura!
//
struct MinhaStruct {
   int x;
   int y;
};

// muitas linhas abaixo...

void func(MinhaStruct* ms)
{
   // asterisco significa ponteiro para MinhaStruct!
}

int main()
{
   MinhaStruct ms;
   func(&ms);
}
 
