//******TESTAR EM LINUX******
//*******CONFERIR SE A IDENTAÇÃO ESTÁ HOMOGENEA OU ESPACOS OU TAB********
//*****CRIAR CABEÇALHO******

//TODO colocar exemplo de CSV, já que ele começa lendo um.


#include <iostream> 
#include <fstream>
#include <iomanip>
#include <string>
#include <cstring>



using namespace std;

struct baseDeDados{
    char fabricante[50];
    long long codBarras;
    char comp[94];
    float generico;
    float original;
    bool situacao;
};
baseDeDados *aumentarVetor(baseDeDados *dados, int &tam){
    int n = tam+5;
    baseDeDados *aux = new baseDeDados[n];
    copy(dados, dados+tam, aux);
    tam += 5;
    delete [] dados;
    return aux;
}

void insercaoDeDado(){//Iuri

    /*copia todos os dados maiores que o elemento a ser inserido
    cria um vetor de n+1 e insere o elemento na primeica casa, depois escreve novamente no arquivo*/
    //perguntar se tem como inserir uma quebra de linha
   
    /*Como inserir dados em dados que estão logicamente excluídos?*/
    // criando um vetor de dados excluidos?
}

void forBin(baseDeDados *dados, int cad){ //Joao pedro
    ofstream fout;
    string nomeArq;
    cin >> nomeArq;
    fout.open(nomeArq, ios_base::out|ios_base::binary);
    for(int i = 0; i < cad;i++){
        //TODO: coloca quebras de linha nesta bodega pelamordedeus.
        cout << dados[i].fabricante << " " << dados[i].codBarras << " "<< dados[i].comp << " " << dados[i].generico << " " << dados[i].original << endl;
        
    }
    if(fout.is_open()){
            fout.write((char*)dados, sizeof(baseDeDados) * cad);
            fout.close();
    }

}

string subVirgula(char base[]){
    int tam = strlen(base);
    char *ptr = new char[tam];
    for(int i = 0; i < tam;i++){
        if(base[i] == ',')
            ptr[i] = '.';
        else
            ptr[i] = base[i];
    }
    copy(ptr,ptr+tam, base);
    delete[] ptr;
    return base;
}


void preencherVetor(char one[], char control, int cad, baseDeDados *dados){
    switch(control){
        case 'f':
            for(int i = 0;one[i];i++){
                dados[cad].fabricante[i] = one[i];
            }
        break;
        case 'c':
            for(int i = 0;one[i];i++){
                    dados[cad].comp[i] = one[i];
            }
        break;
    }
}
void inCsv(char menu, baseDeDados *dados, int &tam, int &cad){//Joao pedro
    string nomeBase;
    if(menu == '1'){
        nomeBase = "padrao.csv";
    }
    else{
        //TODO: coloca quebras de linha nesta bodega pelamordedeus.
        cout << "Voce optou por escolher uma base diferente, sendo assim veja os requisitos para ela ser aceita sem erros:" << endl;
        cout << "Ter exatamente 5 colunas" << endl << "Estar no formato .CSV " << endl << "As colunas devem ser do tipo string, int, string, float, float exatamente nessa ordem" << endl << "A base deve estar na mesma pasta do programa" << endl;
        cout << "Agora que tem conhecimento dos requisitos insira o nome da base desejada adicionando .csv ao final do nome da base" << endl; 
        cin >> nomeBase;
    }
    ifstream base (nomeBase);
    if(base){
        string linha;
        while(getline(base, linha)){   
            //TODO corrigir indentação.
            //TODO esses arrays de tamanho fixo são muito fáceis de serem atacados por buffer overrun; isso é C++, usa std::string.
    char fab[50] = {}, comp[94] = {}, gen[15] = {}, ori[15] = {}, cod[15] = {};
            int i, j;
            j = 0;
            i = 0; 
            //TODO isso não é um comma separated value, pois ; não é um comma.
            //TOOD lógica está muito ruim, repetindo blocos while; tem como apenas parsear campos e depois define o que é cada um?
            while(linha[i] != ';'){
                fab[j] = linha[i];
                i++;
                j++;
            }
            i++;
            j = 0;
            while(linha[i] != ';'){
                cod[j] = linha[i];
                i++;
                j++;
            }
            i++;
            j = 0;
            while(linha[i] != ';'){
                comp[j] = linha[i];
                i++;
                j++;
                
            }
            
            i++;
            j = 0;
            while(linha[i] != ';'){
                gen[j] = linha[i];
                i++;
                j++;
            }
            j = 0;
            i++;
            while(i < linha.size()){
                    ori[j] = linha[i];
                    j++;
                    i++;
            }
            if(cad < tam){
                //TODO preencherVetor é um péssimo nome; diga o que tem nesse vetor no lugar.
                preencherVetor(fab,'f', cad, dados);
                dados[cad].codBarras = atoll(cod);
                preencherVetor(comp, 'c', cad, dados);
                subVirgula(gen);
                dados[cad].generico = atof(gen);
                subVirgula(ori);

                dados[cad].original = atof(ori);
                dados[cad].situacao = true;
                //cout << dados[cad].fabricante<< " " << dados[cad].codBarras << " " << dados[cad].comp << " " << dados[cad].generico << " " << dados[cad].original << endl;
                cad++;
            }
            else{
                dados = aumentarVetor(dados, tam);
                preencherVetor(fab,'f', cad, dados);
                dados[cad].codBarras = atoll(cod);
                preencherVetor(comp, 'c', cad, dados);
                subVirgula(gen);
                dados[cad].generico = atof(gen);
                subVirgula(ori);
 
                dados[cad].original = atof(ori);

                dados[cad].situacao = true;
                //cout << dados[cad].fabricante << " " << dados[cad].codBarras << " " << dados[cad].comp << " " << dados[cad].generico << " " << dados[cad].original << endl;
                cad++;
            }
           //cout << ' ' <<  cad << endl;
        }
    }
    else{
        //TODO: coloca quebras de linha nesta bodega pelamordedeus.
        cout << "O nome digitado não foi encontrado, deseja digitar novamente:" << endl << "(1) Nao, usar base padrao" << endl << "(2) Sim , digitar a propria base" << endl;
        cin >> menu;
        //TODO: isso vai quebrar se o usuário ficar digitando 1 eternamente.
        inCsv(menu, dados, tam, cad); 
    }
    base.close();
    forBin(dados, cad);
}




void leituraDeDados(){//Joao pedro
    
}

void ordenacao(){//Joao pedro

    //eliminar dados que já haviam sido excluidos logicamente
}

void buscaDeDados(){//Giovanna

}

void exclusaoDeDados(){//Giovanna
   
}

void imprimirArquivo(){//Iuri
    
}


int main(){
    char menu0;
    int tam = 5;
    int cad = 0;
    baseDeDados *dados = new baseDeDados[tam];
    //TODO: coloca quebras de linha nesta bodega pelamordedeus.
    cout << "Bem vindos ao programa do grupo 18" << endl << "Para comecarmos defina a base a ser usada no programa:" << endl << "(1) Base padrao" << endl << "(2) Outra base" << endl;
    cin >> menu0;
    inCsv(menu0, dados, tam, cad);

    delete [] dados;
    return 0;
}