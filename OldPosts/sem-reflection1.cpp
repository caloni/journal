class MyClass
{
public:
        void Method1();
        void Method2();
        void Method3();
};


int main()
{
        MyClass c;
        if( auto m = typeid(c).methods.getaddresof("Method1") )
                m();
}
// Codigo-fonte disponivel no GitHub (Caloni/Caloni.com.br) 
