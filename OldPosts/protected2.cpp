// another chance

int Derived::GetAnotherProtected()
{
	Base base; // somos derivados dessa base, n�o somos?
	return base.m_protected; // mas n�o � por isso que vamos acessar um membro protegido de boa.
}// Codigo-fonte disponivel no GitHub (Caloni/Caloni.com.br) 
