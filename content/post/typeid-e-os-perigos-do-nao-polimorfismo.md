---
categories: [ "code" ]
date: "2007-10-24"
tags: [ "draft",  ]
title: "Typeid e os perigos do não-polimorfismo"
---
Quando usamos o operador typeid geralmente desejamos conhecer informações sobre o tipo exato do objeto que temos em mãos, independente da hierarquia de herança a qual seu tipo pertença. Só que por ignorar, assim como o sizeof, que esse operador possui duas caras, às vezes damos com os burros n'água e compramos gato por lebre. Não é pra menos. Uma sutil diferença entre classes polimórficas e estáticas pode dar aquele susto que só C++ pode proporcionar.

Eis um exemplo singelo, sem dramatização (com dramatização == "500 linhas de código de produção além do código abaixo").

    #include <iostream>
    #include <typeinfo>
    
    using namespace std;
    
    class Base
    {
    public:
    	Base()
    	{
    		cout << "Base()\n";
    		m_x = 0;
    	}
    
    	~Base()
    	{
    		cout << "~Base()\n";
    	}
    
    	int m_x;
    };
    
    // como Base não é polimórfica (como vamos ver), Deriv paga o pato
    class Deriv : public Base
    {
    public:
    	Deriv()
    	{
    		cout << "Deriv()\n";
    
    		m_x = 1;
    		m_y = 0;
    	}
    
    	virtual ~Deriv() // ela é polimórfica
    	{
    		cout << "~Deriv()\n";
    	}
    
    	int m_y;
    };
    
    void func(Base* b) // eu não sei que é uma derivada
    {
    	cout << typeid(*b).name() << '\n'; // e nem o typeid número 1
    }
    
    int main()
    {
    	Base* b = new Deriv(); // o ponteiro é pra base, mas a instância é de derivada
    	func(b);
    } 
    

    
    -----
    Saída:
    Base()
    Deriv()
    class Base

O typeid usado nesse exemplo será o estático, no estilo typeid(type), porque o tipo do objeto para a função é de "ponteiro para objeto de classe não-polimórfica", ou seja, sem nenhuma função virtual. É importante lembrar que o polimorfismo em C++ só é aplicado se houver razão para tal, pois na linguagem a regra é que "não existe sobrecarga de execução sem que o programador queira".

Se o esperado pelo programador fosse um class Deriv na última linha da saída, ou seja, que o typeid utilizado fosse a versão dinâmica, então a nossa classe Base tem que ser polimórfica:

    virtual ~Base() // agora sou polimórfica também!
    {
    	cout << "~Base()\n";
    } 
    

    
    -----
    Saída:
    Base()
    Deriv()
    class Deriv ; como desejava o programador polimórfico

Esse é um erro equivalente ao chamar o operador delete usando o ponteiro recebido em func. Se isso fosse feito, seria chamado apenas o destrutor da classe Base. Por falar nisso, temos nesse exemplo um leak de memória (percebeu pela saída que os destrutores não são chamados?). Mas esse é um erro bem menos sutil que o visto pelo nosso amigo typeid amigo-da-onça ;).
