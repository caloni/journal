// espertinho, hein?

int Derived::GetAnotherProtected()
{
	AnotherDerived anotherDeriv; // n�o acesso os protegidos desse aqui...
	Base& base = anotherDeriv; // mas quem sabe se eu acessar os genes da minha m�e?
	return base.m_protected; // nada feito =(. Tamb�m, se nem eu mesmo consegui me acessar...
}