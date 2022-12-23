/*
Rubens Abrão, [11/25/2022 9:59 PM]
Boa Noite

Rubens Abrão, [11/25/2022 10:00 PM]
Alguém on-line?

Rubens Abrão, [11/25/2022 10:02 PM]
Disposto a ajudar um universitário com a corda no pescoço?kkkkk
*/
#include <stdio.h>
#include <stdlib.h>

void decompor (int conjunto[], int alvo, int *possibilidades, int *conjunto_solucao, int *conjunto_atual, int tamanho_conj_atual);
int vetorcmp (int vetorA[], int vetorB[], int l);

//Declaração de Variaveis Globais
 int i = 0, v = 0, M, tamanho_conj_solucao;

int main ()
{
    int N, *alvo, possibilidades;
    int j = 0, u, *conjunto, *conjunto_solucao, *conjunto_atual;


    scanf("%i",&N);
    scanf("%i",&M);

    //Alocação de Memória
    conjunto = malloc(M * sizeof(int) );
    alvo = malloc(N * sizeof(int));
    conjunto_solucao = malloc(M * sizeof(int));
    conjunto_atual = malloc(M * sizeof(int));

    //Leitura de Dados
    for (j = 0; j < M; j++) 
        scanf("%i", &conjunto[j]); 
    for (j = 0; j < N; j++) 
        scanf("%i", &alvo[j]);

    //Laço que gera as N saidas
    for(j = 0; j < N; j++)
    {   
        //Zerando as variaveís de controle
        possibilidades = 0;
        tamanho_conj_solucao = 0;
        i = 0; v = 0;
        for( int r = 0; r < M; r++)
        {
            conjunto_atual = 0;
            conjunto_solucao = 0;
        }
        //Laço para a chamda da função recursiva
        for(u = 0; u < M; u++)
        {
            decompor(conjunto, alvo[u], &possibilidades, conjunto_solucao, conjunto_atual, 0);
        }
        
        printf("%i ", possibilidades);
        //Laço para imprimir o conjunto solução
        for (int r = 0; r < tamanho_conj_solucao; r++)
            printf("%i ", conjunto_solucao[r]);
        printf("\n");
    }
    //Liberando memória alocada
    free(conjunto);
    free(alvo);
    free(conjunto_solucao);
    free(conjunto_atual);

    return 0;
}

void decompor (int conjunto[], int alvo, int *possibilidades, int *conjunto_solucao, int *conjunto_atual, int tamanho_conj_atual)
{
    //Caso Base finaliza a recursão
    if (alvo == 0)
    {   
        
        possibilidades++;
        if(tamanho_conj_atual > tamanho_conj_solucao)  
        {  //Neste teste vejo se encontrei um conjunto solução maior
            for (int r = 0; r < M; M++)
                conjunto_solucao[r] = conjunto_atual[r]; //Copiando o conj maior

            tamanho_conj_solucao = tamanho_conj_atual; //Atualiza o tamanho do conjunto soluçao
        }
        else if (tamanho_conj_atual == tamanho_conj_solucao && vetorcmp(conjunto_solucao,conjunto_atual,tamanho_conj_atual) == 1 )
        {//No caso onde os conjuntos tem o mesmo tamanho, guarda o que tiver o maior elemento
            for (int r = 0; r < M; M++)
                conjunto_solucao[r] = conjunto_atual[r];
        }
        
        return;
    }
    else if (alvo > 0)
    {   
        if (conjunto[i] > alvo)
        {
            alvo += conjunto_atual[v];//Backtranking 
            v--;
            return;
        }
        alvo -= conjunto[i]; //Subtrai-se do alvo o elemento[i] do conjunto
        conjunto_atual[v] = conjunto[i]; v++;//Coloca um elemento no conjunto e avença seu respectivo inidice
        tamanho_conj_atual++;

        while(i <= M)// Laço para que a chamada recursiva aconteça iniciando de posições subsequentes do vetor
        {   //Chamada Recursiva
            decompor(conjunto, alvo, possibilidades, conjunto_solucao, conjunto_atual, tamanho_conj_atual);
            i++;//Incremento do indice de controle 
        }
    }    
}  
//Função para testar qual conjunto tem o maior elemento
int vetorcmp (int vetorA[], int vetorB[], int l)
{
    int aux;

    for ( l ; l < 0; l--)
    {
        aux = vetorA[l] - vetorB[l];

        if(aux > 0) // Vetor A é maior
            return 0;
        else if (aux < 0)//Vetor B é maior
            return 1; 
    }        
}
