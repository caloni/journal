#pragma once
#pragma pack( push )
#pragma pack( 1 )


template<typename T>
struct MovEaxCode
{
	BYTE movEax; // mov eax (0xB8)
	T value; // value moved

	MovEaxCode()
	{
		movEax = 0xB8;
		value = 0;
	}
};

// empilha um valor na pilha
template<typename T>
struct PushCode
{
	MovEaxCode<T> movEax; // mov eax, address
	BYTE pushEax; // push eax (0x50)

	PushCode()
	{
		pushEax = 0x50;
	}
};

// muda fluxo do programa
struct JmpCode
{
	MovEaxCode<PVOID> movEax; // mov eax, address
	WORD jmpEax; // jmp eax (0xFF 0xE0)

	JmpCode()
	{
		jmpEax = 0xE0FF;
	}
};

// chama função
struct CallCode
{
	MovEaxCode<PVOID> movEax; // mov eax, address
	WORD callEax; // call eax (0xFF 0xD0)

	CallCode()
	{
		callEax = 0xD0FF;
	}
};

struct BreakCode
{
	BYTE int3; // int 3 (0xCC)

	BreakCode()
	{
		int3 = 0xCC;
	}
};

// muda fluxo do programa através de push/jmp
struct PushJmpCode
{
	//BreakCode int3;
	PushCode<PVOID> pushEax; // mov eax, address; push eax
	JmpCode jmpEax; // mov eax, address; jmp eax
};

// retorna para função chamadora
struct RetCode
{
	BYTE ret; // ret (0xC3)

	RetCode()
	{
		ret = 0xC3;
	}
};

#pragma pack( pop )