// fora-da-lei

int Derived::GetAnotherProtected()
{
	Base& base = *this; // os genes da minha m�e.
	return base.m_protected; // eu n�o me acesso! tipo o subconsciente...
}// Codigo-fonte disponivel no GitHub (Caloni/Caloni.com.br) 
