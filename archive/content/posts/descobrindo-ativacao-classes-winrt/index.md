---
categories:
 - coding
date: '2023-08-13'
tags:
 - reversing
title: Descobrir ativação de classes WinRT
---

A Windows Runtime nada mais é que o COM repaginado. O COM já previa o uso de proxies para comunicação remota entre processos. A WinRT adicionou mais uma interface e uma função. Vou neste post descrever o processo pelo qual descobri como funciona o registro de um dispositivo no Azure Active Directory (AAD).

O teste feito é logar no Microsoft Teams com uma conta de empresa pertencente a um domínio federado, o que significa que para fazer o login existe um plugin feito pela empresa que será carregado e autenticar o usuário. TL;DR isto é feito por um processo chamado `Microsoft.AAD.BrokerPlugin.exe`, que usa WinRT para se comunicar com o servidor AAD e com o sistema.

Analisando o disassembly pelo IDA e realizando algumas sessões de debug eu estava caminhando pela interface `IUserDeviceRegManagerStatics` que havia encontrado anteriormente. Eu sabia que essa interface estava por trás do registro da máquina porque depois que alguns de seus métodos são chamados o comando `dsregcmd.exe /status` lista o computador como joined dentro do AAD e se você for na parte de contas do usuário naquela tela de configurações do Windows irá aparecer a mesma conta usada para logar no Teams.

Buscando no registro pelo nome dessa interface existe uma entrada em `HKEY_CLASSES_ROOT\Interface\{b5decd05-58ef-4cd3-ae25-d177bcbcea9e}` aonde existe uma subkey `ProxyStubClsid32`. Essa subkey, de acordo com o [MSDN](https://learn.microsoft.com/en-us/windows/win32/com/proxystubclsid32), é usado para traduzir um IID -- ID de Interface -- para um CLSID -- ID de classe. Nas palavras do MSDN (em inglês, por favor): "Maps an IID to a CLSID in 32-bit proxy DLLs".

>
> If you add interfaces, you must use this entry to register them (32-bit systems) so that OLE can find the appropriate remoting code to establish interprocess communication.
>

Eu acredito que a mesma lógica se aplica a ambientes de 64 bits, que é o caso do Windows 10 onde estou analisando.

Então esse ID é um CLSID para o código de um proxy que irá me ajudar a chamar esta interface `IUserDeviceRegManagerStatics`. Tudo bem, vamos lá. O valor default dentro dessa subkey é `{65704743-2513-4987-9fc9-826bbedde743}` e eu devo fazer a busca por este ID dentro de `HKEY_CLASSES_ROOT\CLSID`, já que se trata de um ID de classe. Eu encontro. O valor default dessa chave diz "UserDeviceRegistration", então eu penso ser este o cara que estou procurando.

Existe uma chave `InProcServer32` dentro desta chave. Novamente vamos à documentação, e de acordo com o [MSDN](https://learn.microsoft.com/en-us/windows/win32/com/inprocserver32):

>
> Registers a 32-bit in-process server and specifies the threading model of the apartment the server can run in.
>

Isso faz todo o sentido, porque o valor default para esta mesma chave é "C:\Windows\System32\UserDeviceRegistration.dll"  e existe uma string `ThreadingModel` também especificando "Both". Olhando para as funções exportadas por esta DLL `UserDeviceRegistration.dll` são listadas as bem conhecidas `DllCanUnloadNow`, `DllGetActivationFactory` e `DllGetClassObject`. Isso é bê-a-bá de COM. O código que eu quero usar deve carregar esta DLL e chamar `DllGetActivationFactory` ou `DllGetClassObject` para obter um ponteiro para uma classe implementando esta interface ou um proxy para chamar esta classe remotamente.

Ao analisar pelo OleView pela `IUserDeviceRegManagerStatics` não há uma biblioteca de tipos com os nomes dos métodos e argumentos. Existe uma vtable genérica com algumas procedures chamadas "Proc1", "Proc2" e assim por diante. Já que desde minha sessão de debugging anterior eu vi um monte de funções RPC sendo chamadas eu irei assumir que `UserDeviceRegistration!DllGetActivationFactory` está sendo chamada. E, mais uma vez, de acordo com o [MSDN](https://learn.microsoft.com/en-us/previous-versions/br205771(v=vs.85)), este entry point:

>
> Retrieves the activation factory from a DLL that contains activatable Windows Runtime classes.
>

That's a bingo! Windows Runtime classes é o que estou procurando mesmo. Como eu já estudei anteriormente, isso bate com o retorno desta função: um ponteiro para a interface `IActivationFactory`. A entrada deste argumento é uma `HSTRING`, que é uma nova string definida pela Windows Runtime para strings imutáveis das linguagens mais populares, como C# e JS. E, mais uma vez, isso explica porque em minhas sessões de debug observei um monte de chamadas a `WindowsDeleteString`. Quando você começa a fazer reversa começa a ouvir ecos de sessões passadas para conseguir montar o quebra-cabeças.

Com tudo isso que descobri resolvi colocar um breakpoint no evento de load desta DLL.

```
sxe ld:UserDeviceRegistration.dll
```

Repito o procedimento de registro de máquina e o breakpoint é ativado. Estou pronto para agora colocar um breakpoint na `DllGetActivationFactory`. Porém, tenho um insight neste momento. Eu me lembro que posso usar o Process Explorer para buscar no sistema inteiro quem é que está usando a mesma DLL que estou de olho. Afinal de contas, se ela é a interface para se comunicar com a implementação, faz sentido que dentro dela exista a implementação.

E com isso eu encontro a `RuntimeBroker.exe`, processo iniciado quase ao mesmo tempo que o BrokerPlugin. Coincidência? Acho que não.

Isso significa que agora eu posso depurar tanto o BrokerPlugin quanto a RuntimeBroker e usar os símbolos da DLL em comum para encontrar quais funções deverão ser chamadas de ambos os lados, já apostando que o método chave seria `CUserDeviceRegManagerStatic::WorkplaceJoin`, já que eu já encontrei este método em sessões passadas e seu nome faz todo sentido.

Na minha primeira tentativa o breakpoint não disparou. Busco, então, por nomes de funções semelhantes:

```
x UserDeviceRegistration!Windows::Internal::UserDeviceRegistration::CUserDeviceRegManagerStatic::*
```

Já que estou interessado no macro coloco breakpoint em todos os métodos da classe. Faço isso também do lado do BrokerPlugin e seus métodos dentro de `UserDeviceRegistration::IUserDeviceRegManagerStatics`. Posso acompanhar o fluxo sem parar usando um parâmetro bem esperto nos breakpoints do WinDbg: um comando qualquer. Nesse caso uso imprimir o nome da função chamada e o comando `g`, que segue a execução sem parar. Por exemplos (RuntimeBroker e BrokerPlugin, respectivamente):

```
bp UserDeviceRegistration!Windows::Internal::UserDeviceRegistration::CUserDeviceRegManagerStatic::DelegatedWorkplaceJoinWithRequestId ".echotime; .echo UserDeviceRegistration!Windows::Internal::UserDeviceRegistration::CUserDeviceRegManagerStatic::DelegatedWorkplaceJoinWithRequestId; g"
bp AAD_Core!Windows::Internal::UserDeviceRegistration::IUserDeviceRegManagerStatics::GetJoinInfo ".echotime; .echo AAD_Core!Windows::Internal::UserDeviceRegistration::IUserDeviceRegManagerStatics::GetJoinInfo; g"
```

Com isso eu descobri qual o processo e qual a classe envolvida neste processo de registro de máquina. Graças também aos símbolos disponibilizados pela Microsoft fica muito mais fácil entender o fluxo. Pretendo usar esta técnica mais vezes no futuro.
