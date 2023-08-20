---
categories:
- coding
date: '2022-05-20'
tags: null
title: Programando bash script
---

Programar scripts em bash é basicamente rodar os comandos já disponíveis no modo shell a partir de um arquivo texto. Além das construções de qualquer linguagem procedural como if e laços de repetição o que é mais usado é o controle de entrada e saída dos comandos executados (e manipulação de file descriptors), além do controle sobre os signals durante as execuções.

## Referências

 - [Bash Reference Manual](https://www.gnu.org/savannah-checkouts/gnu/bash/manual/bash.html)
 - [Advanced Bash-Scripting Guide](https://tldp.org/LDP/abs/html/index.html)

```sh
#
# teste da entrevista de goiaba em bash
#
ordenar_numeros() {

        maior=0
        menor=$1
        media=$(echo "scale=2; ( $1 + $2 + $3 ) / 3" | bc -l)
        #media=$(( ($1 + $2 + $3) / 3 ))
        numeros=($1 $2 $3)

        for i in "${numeros[@]}"; do

                if [ $i -le $menor ]
                then
                        menor=$i
                fi

                if [ $i -ge $maior ]
                then
                        maior=$i
                fi
        done

        echo maior=$maior, menor=$menor, media=$media
}

ordenar_numeros $1 $2 $3
```

Como você deve ter percebido lendo o primeiro script da sua vida variáveis em bash script começam com `$`. As variáveis de `$0` a `$9` são os argumentos do programa, mas de resto pode ser um nome qualquer e usado em qualquer lugar, mas caso você precise tornar o nome não-ambíguo é importante colocar entre chaves:

```sh
[ ${var}text -ne $vartext ]
```

Como você já deve ter percebido lendo o primeiro script da sua vida comandos são terminados por nova-linha ou por ponto-e-vírgula, não tendo briga para ninguém. Dois comandos não podem conviver um do lado do outro e dar bom. É por isso que caso você queira fazer um `if-then` na mesma linha precisa colocar um ponto-e-vírgula antes de usar o `then`:

```sh
for i in "${numeros[@]}"; do
if [ $i -le $menor ]; then
```

## Preparando o terreno

Há algumas dicas úteis para explicar para todos que mexem com bash script:

 - Sempre setar antes de rodar um script a flag de executável (diferente do Windows, nem todos os arquivos possuem esse direito no Linux);
 - Scripts podem/devem conter comentários; o padrão de comentários para tudo no Linux é a cerquilha (#) no início das linhas.


No começo dos scripts em geral você irá notar a seguinte linha:

```sh
set -e
```

O uso do parâmetro `-e` é para sair imediatamente caso qualquer comando dê ruim. Além disso, há a possibilidade de setar funções de saída para limpar coisas deixadas pelo caminho. É o garbage collector do script. Há [outras opções](https://www.gnu.org/software/bash/manual/html_node/The-Set-Builtin.html) para modificar o comportamento do shell.

A opção de limpeza pode ser setada através do [comando trap](https://linuxhint.com/bash_trap_command/). O comando trap permite associar chamadas de funções aos diferentes tipos de sinais que o programa pode receber. Por exemplo, abaixo é setada a função de limpeza para quando o script sair.

```sh
trap cleanup EXIT

cl_stack=()
cleanup() {
    for ((i=${#cl_stack[@]}-1;i>=0;i--)); do
        eval ${cl_stack[$i]}
    done
}
```

Note esta técnica utilizada para incluir operações de limpeza dentro de um array, o `cl_stack`. No decorrer da execução do script se houver a necessidade de adicionar mais uma operação de limpeza o usuário deve incluir esta operação neste array.

```sh
cl_stack+=( "kill pid" )
```


### Lendo parâmetros de entrada

Para ler parâmetros de entrada há um comando muito bom chamado getopt, mas por algum motivo ele não é muito popular nos scripts deste projeto. No lugar é usada a leitura toscona dos parâmetros pelas variáveis reservadas $1 a $9.

```sh
#
# Read the params.
#
if [ "$1" = "" ] || [ ! -d ${LIB_ROOT}/libs/$2 ]; then
    echo "usage: ./copy-lib <lib-path>"
    exit 1
fi
```

Você irá encontrar também o shift de argumentos. Conforme você vai comendo os primeiros argumentos sendo passados para seu script o $2 poderia ser o $1, o $3 viraria o $2 e assim por diante, para facilitar o reaproveitamento de script e simplificar a contagem. É particularmente útil para argumentos sem ordem específica.

```sh
[ -z $1 ] && usage

if [ $1 = "-b" ]; then
  do_boot=y
  shift
fi
if [ $1 = "-d" ]; then
  do_delete=y
  shift
fi
```

## Estrutura do código

Abaixo as palavras reservadas para montar scripts. E é só. Em geral elas são usadas para definir condições e rodar código em loop. O resto são comandos disponíveis no sistema.

```
if      then   elif     else     fi        time
for     in     until    while    do        done
case    esac   coproc   select   function
{   }   [[     ]]       !
```

## Evaluation

Avaliações de expressões e comandos é um dos pedaços mais úteis em scripts bash. Você pode capturar a saída de um comando e guardar em uma variável ou usar uma variável como entrada de um comando, além de criar novas variáveis através da montagem de um ou mais comandos com outras strings. Em geral tudo que está dentro de `$()` pode ser usado como um minishell que irá executar e entregar o output.

```sh
tmpdir=$(mktemp -d)
mountdir=$(pwd)/subdir
echo "Today is $(date). What a lovely, lovely day!"
```

Evaluation precisa de redirects às vezes, e às vezes não precisa de nenhum (`/dev/null`). No exemplo abaixo nós queremos o id do usuário de um arquivo sem nos preocupar com a saída de erro.

```sh
uid=$(stat -c %u ${mountdir}/data/file 2> /dev/null)
```

A saída de erro é o file descriptor 2, e por isso foi usada a construção `2>` em vez do simples `>`, que redirecionaria o file descriptor 1, que é a saída padrão. O file descriptor 0, caso não tenha adivinhado ainda, é a entrada padrão, que pode ser obtida com a flechinha invertida `<`. Resumo:

| sinal | nome    | file descriptor |
| ----- | ------- | --------------- |
| <     | stdin   | 0               |
| >     | stdout  | 1               |
| 2>    | stderr  | 2               |

Você deve estar se perguntando se é possível usar outros file descriptors além do 2. Sim, você pode. Abaixo de 10 é mais ou menos seguro utilizar para file descriptors temporários. Acima de 10 é possível que entre em conflito com file descriptors dos próprios comandos sendo executados ou fds do sistema.

## Condições

Uma condição em bash pode ser feita entre colchetes simples, duplos e parênteses, cada uma dessas formas [sendo inventada](https://acloudguru.com/blog/engineering/conditions-in-bash-scripting-if-statements) para adicionar mais formas de expressar condições. É importante ter sempre em mãos também as [diferentes checagens](https://www.gnu.org/savannah-checkouts/gnu/bash/manual/bash.html#Bash-Conditional-Expressions) que podem ser feitas dentro de um if.

### ifs

```sh
if [ $foo -ge 3 ]; then
if [ -f regularfile ]; then
if [ -r readablefile]; then
if <condition>; then<commands>fi
```

### if-elif-else

```sh
if [ -r somefile ]; then
  content=$(cat somefile)
elif [ -f somefile ]; then
  echo "The file 'somefile' exists but is not readable to the script."
else
  echo "The file 'somefile' does not exist."
fi
```

### Dicas sobre uso do if

 - Sempre mantenha espaços entre os colchetes e a checagem/comparação; e.g. erro: “missing `]'“;

 - Palavras-chave não podem compartilhar a mesma linha (mesmo comando); solução: ou coloque um ponto-e-vírgula entre as palavras-chave ou quebra uma nova linha; e.g. erro: “syntax error near unexpected token `fi”;

 - Variáveis do tipo strings podem conter espaços e isso é um problema para o comparador; solução: coloque sempre aspas entre variáveis do tipo string.

```sh
if [ "$1" = "" -o ! -f "$1" ]; then
  echo "usage: cpfiles <compactedfile.tar>"
  exit 1
fi
```

### Brinde

 - Você pode inverter uma condição colocando um “!” na frente; `if [ ! -f regularfile ]; then`

 - Você pode combinar condições usando certos operadores como -a (AND) e -o (OR); `if [ $foo -ge 3 -a $foo -lt 10 ]; then`

 - As mesmas condições e regras podem ser usadas em outros comandos como while e until.

### Condições com colchetes-duplos

```sh
if [[ "$stringvar" == *string* ]]; then
if [[ "$stringvar" == *[sS]tring* ]]; then
if [[ $stringvarwithspaces != foo ]]; then
if [[ -a *.sh ]]; then
if [[ $num -eq 3 && "$stringvar" == foo ]]; then
if [[ "$title" =~ [^a-zA-Z0-9\ ] ]]; then
```

 - Duplos-colchetes expandem variáveis como shell globbing (exemplo acima com asteriscos retorna verdade se houver string em qualquer parte de $stringvar;

 - Por outro lado não há file globbing (e por isso a condição -a acima funciona para um arquivo chamado *.sh);

 - Não é preciso usar aspas para strings que contém espaços (não há divisão da variável);

 - Syntax sugar: você pode usar operadores && e ||.

### Considerações importantíssimas sobre ifs

 - Operadores AND e OR são short-circuit;

 - 0 = true, >0 = false (C-like); por isso há comandos como `if ! tar etc; then "Error"; fi`.

### Brinde: switch-case

Sim, ele existe. E é até bonitinho:

```sh
case $1 in
    'start')
        systemd-run --unit mydaemon $(realpath $0) run
        exit
        ;;
    'stop')
        systemctl stop mydaemon
        exit
        ;;
    'run')
        echo -n
        ;;
    *)
        echo "usage: $(basename $0) [start|stop|run]" >&2
        exit 1
        ;;
```

### Alguns comandos úteis do Linux para se divertir

| comando     | descrição                                                                |
| ----------- | ------------------------------------------------------------------------ |
| [           | para construir testes direto do shell                                    |
| antiword    | para ler arquivos do Word                                                |
| arch        | imprime a arquitetura (e.g. x86_64)                                      |
| awk         | linguagem para interpretar texto                                         |
| b2sum       | checksum                                                                 |
| base32      | parser para base32                                                       |
| base64      | parser para base64                                                       |
| basename    | nome de arquivo sem path                                                 |
| basenc      | coda/decoda de várias bases                                              |
| bash        | abre sessão bash                                                         |
| brotli      | compressor de arquivos texto                                             |
| bunzip2     | descompressor de arquivos                                                |
| bz-sth      | compressão de arquivos                                                   |
| cat         | exibe conteúdo de arquivo                                                |
| chattr      | modifica atributos de arquivos                                           |
| chgrp       | modifica grupo associado a arquivos                                      |
| chmod       | modifica flags de arquivos                                               |
| chown       | modifica proprietário de arquivos                                        |
| chroot      | modifica diretório root                                                  |
| cksum       | checksum                                                                 |
| clear       | limpa tela                                                               |
| cmp         | compara arquivos                                                         |
| column      | formatação de tabelas                                                    |
| comm        | compara arquivos ordenados                                               |
| connect     | commando de relay via proxy                                              |
| cp          | copia arquivos                                                           |
| curl        | acessa recursos web                                                      |
| cut         | recorta texto                                                            |
| d2u         | dos2unix                                                                 |
| dash        | outro bash                                                               |
| date        | imprime data/hora atual                                                  |
| dd          | copia arquivo com conversão                                              |
| df          | mostra espaço usado em disco                                             |
| diff[3]     | comparador de arquivos                                                   |
| dir         | listagem de arquivos                                                     |
| dirname     | nome do diretório de um arquivo                                          |
| dos2unix    | converte final de linha                                                  |
| du          | uso de espaço por arquivos em diretório                                  |
| dumpsexp    | dump hexa de arquivos binários                                           |
| echo        | imprime mensagem na saída                                                |
| env         | variáveis de ambiente                                                    |
| envsubst    | substituição de variáveis de ambiente                                    |
| ex          | editor vim no modo ex                                                    |
| expand      | expande tabs para espaços                                                |
| expr        | avalia expressões                                                        |
| factor      | fatora números                                                           |
| false       | retorna false (duh), aka 1 (vide bash true/false)                        |
| fido2-sth   | coisas de autenticação/criptografia                                      |
| file        | identifica tipo de arquivo                                               |
| find        | encontra arquivos                                                        |
| fmt         | formata arquivos (código-fonte, etc)                                     |
| fold        | formata texto para limitar máximo de colunas                             |
| funzip      | descompacta gzip                                                         |
| gawk        | versão GNU do awk                                                        |
| getconf     | retorna configurações do sistema via variáveis POSIX                     |
| getopt      | parser de parâmetros de linha de comando (para scripts)                  |
| gettext     | tradução de mensagens do sistema                                         |
| gpg-sth     | comandos relacionados a PGP da GNU                                       |
| grep        | busca padrões em texto                                                   |
| groups      | lista grupos de usuário                                                  |
| gzip        | compacta dados usando algoritmo gzip                                     |
| head        | equivalente ao tail, mas do começo                                       |
| hmac256     | gerador de hash                                                          |
| hostname    | mostra nome da máquina                                                   |
| iconv       | converte entre diferentes encodings                                      |
| id          | mostra ids do usuário e grupo                                            |
| infocmp     | compara terminfo (conf de terminais) diferentes                          |
| infotocap   | conversão de terminfo para termcap                                       |
| join        | junta linhas de dois arquivos em campos comuns (e.g. ID Name)            |
| kbxutil     | gerenciador de chaves criptográficas                                     |
| kill        | mata processo                                                            |
| ldd         | exibe dependências de módulo (executável ou dll)                         |
| less        | paginador de saída com navegação                                         |
| link        | cria links simbólicos                                                    |
| ln          | cria hard links                                                          |
| locale      | imprime informação de localização                                        |
| logname     | nome de login usado                                                      |
| ls          | lista diretórios e arquivos                                              |
| lsattr      | lista atributos junto dos arquivos e pastas                              |
| lzmadec     | descomprime arquivos no formato .lzma                                    |
| mac2unix    | formata final de linha                                                   |
| md5sum      | gera hash md5                                                            |
| minidumper  | gera dumps de memória (win32)                                            |
| mintty      | abre terminal TTY                                                        |
| mkdir       | cria diretório                                                           |
| mkgroup     | lista grupos do sistema                                                  |
| mkpasswd    | gera arquivo de senhas                                                   |
| mktemp      | cria nome de arquivo temporário único                                    |
| mount       | monta unidades                                                           |
| mv          | move arquivos e pastas                                                   |
| nano        | editor simples                                                           |
| nettle-hash | gerador de hashs (várias)                                                |
| nice        | ajusta afinidade do processo                                             |
| nl          | cria hard links                                                          |
| nohup       | ignora sinais de hangup para processo                                    |
| nproc       | fala quantos processadores temos                                         |
| numfmt      | formatador numérico                                                      |
| od          | escreve saída de arquivo em octal                                        |
| odt2txt     | converte openoffice para texto                                           |
| openssl     | shell com funções criptográficas                                         |
| passwd      | troca senha                                                              |
| paste       | junta diferentes arquivos na horizontal                                  |
| patch       | aplica patchs em arquivos texto                                          |
| pdftotext   | converte PDF para texto                                                  |
| perl        | linguagem de script para tratamento de texto                             |
| pldd        | lista objetos compartilhados na memória do processo                      |
| pr          | pagina texto para impressão                                              |
| printenv    | imprime variáveis de ambiente                                            |
| printf      | função printf do C para scripts                                          |
| ps          | lista processos                                                          |
| pwd         | diretório atual                                                          |
| readlink    | imprime o valor de um link simbólico                                     |
| realpath    | resolve nome de arquivo para absoluto                                    |
| regtool     | gerencia registro do Windows                                             |
| reset       | reinicia sessão                                                          |
| rm          | apaga arquivos                                                           |
| rmdir       | apaga pastas                                                             |
| scp         | copia arquivos remotos                                                   |
| sdiff       | diff horizontal                                                          |
| sed         | manipulador de streams de texto                                          |
| seq         | imprime sequências numéricas                                             |
| setfacl     | modifica ACLs de arquivos                                                |
| sftp        | client FTP                                                               |
| sh          | abre novo shell                                                          |
| sha-sth     | vários hashs SHA                                                         |
| shred       | sobrescreve arquivo várias vezes para torná-lo irrecuperável             |
| shuf        | embaralha linhas de arquivo aleatoriamente                               |
| sleep       | dorme por um tempo                                                       |
| sort        | ordena linhas                                                            |
| split       | divide arquivo em pedaços                                                |
| ssh-sht     | funções de SSH                                                           |
| ssh         | client SSH                                                               |
| sshd        | serviço SSH                                                              |
| ssp         | gerador de dados de profiling a nível de instrução assembly              |
| stat        | imprime estado de arquivos                                               |
| strace      | rastreia chamadas de sistema e signals de processo                       |
| stty        | muda ou mostra características do terminal                               |
| sum         | imprime checksum de arquivos                                             |
| tabs        | verificador de tabs em código-fonte                                      |
| tac         | cat invertido                                                            |
| tail        | head invertido                                                           |
| tar         | junta arquivos em um só (para ser compactado)                            |
| tee         | copia entrada para arquivos ou saída padrão                              |
| tic         | traduz informações do terminal de um formato fonte para o compilado      |
| tig         | git com navegação                                                        |
| timeout     | executa programa com tempo limite para terminar (ou é morto)             |
| touch       | toca em arquivos (bad boy)                                               |
| tr          | traduz, comprime ou apaga caracteres                                     |
| true        | programa que sempre sai com sucesso =)                                   |
| truncate    | aumenta ou diminui arquivos para tamanho especificado                    |
| trust       | gerenciador de certificados                                              |
| tty         | imprime nome do terminal conectado                                       |
| tzset       | mostra fuso horário atual                                                |
| u2d         | unix 2 dos                                                               |
| umount      | desmonta unidades                                                        |
| uname       | imprime informações como processador, máquina, so                        |
| unexpand    | converte espaços para tabs                                               |
| uniq        | elimina linhas duplicadas (desde que ordenadas)                          |
| unix2dos    | altera finais de linha                                                   |
| unix2mac    | altera finais de linha                                                   |
| unlink      | remove link simbólico                                                    |
| unxz        | comprime e descomprime dados no formato .xz                              |
| unzip       | descompacta arquivos no formato zip                                      |
| unzipsfx    | descompactador embutido                                                  |
| vdir        | ls com listagem de flags, tamanho, etc                                   |
| vim         | editor simples                                                           |
| vimdiff     | mesmo editor simples, mas com comparação de arquivos                     |
| wc          | contador de palavras                                                     |
| which       | identifica onde está o comando usado                                     |
| who         | mostra quais os usuários logados                                         |
| whoami      | mostra quem eu sou (filosófico)                                          |
| whouses     | mostra quem está usando                                                  |
| xargs       | inicia programa com argumentos lidos da entrada padrão                   |
| xmlwf       | valida XML                                                               |
| xxd         | dump hexadecimal                                                         |
| xz          | comprime arquivos no formato... er... xz?                                |
| xzcat       | mostra arquivos comprimidos no formato... er... xz?                      |
| xzdec       | descompacta arquivos do formato xz                                       |
| yat2m       | extrai man pages de dentro de um Texinfo                                 |
| yes         | imprime para sempre yes! yes! yes! (útil para aceitar sempre perguntas)  |
| zipcmp      | comparador de zips                                                       |
| zipinfo     | informações sobre um zip                                                 |
| zipmerge    | merge de arquivos zip                                                    |
| ziptool     | mais alguma coisa que dá para fazer com zips                             |


## Debugging

Para depurar scripts bash use o parâmetro `-x` para conseguir ver a execução de cada linha.

```sh
bash -x sort-number.sh 1 2 3
+ ordenar_numeros 1 2 3
+ maior=0
+ menor=1
++ echo 'scale=2; ( 1 + 2 + 3 ) / 3'
++ bc -l
+ media=2.00
+ numeros=($1 $2 $3)
+ for i in "${numeros[@]}"
+ '[' 1 -le 1 ']'
+ menor=1
+ '[' 1 -ge 0 ']'
+ maior=1
+ for i in "${numeros[@]}"
+ '[' 2 -le 1 ']'
+ '[' 2 -ge 1 ']'
+ maior=2
+ for i in "${numeros[@]}"
+ '[' 3 -le 1 ']'
+ '[' 3 -ge 2 ']'
+ maior=3
+ echo maior=3, menor=1, media=2.00
maior=3, menor=1, media=2.00
```

Ou se você precisar depurar apenas uma parte do script pode ligar e desligar o modo `-x` dentro do próprio script usando a mudança de modos como já aprendemos no começo deste artigo.

```sh
for i in "${numeros[@]}"; do

  set -x # activate debugging from here
  if [ $i -le $menor ]
  then
          menor=$i
  fi
  set +x # stop debugging from here

  if [ $i -ge $maior ]
```

```sh
bash sort-number.sh 1 2 3
+ '[' 1 -le 1 ']'
+ menor=1
+ set +x
+ '[' 2 -le 1 ']'
+ set +x
+ '[' 3 -le 1 ']'
+ set +x
maior=3, menor=1, media=2.00
```