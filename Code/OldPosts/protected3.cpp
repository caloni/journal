// dentro da lei

int Derived::GetAnotherProtected()
{
	Derived deriv; // meu irm�o: typeid(deriv) == typeid(*this).
	return deriv.m_protected; // mesmo tipo do escopo: meus direitos s�o preservados
} 
