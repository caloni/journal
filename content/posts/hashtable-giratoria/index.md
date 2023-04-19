---
categories:
- code
date: '2023-04-19'
tags:
- interview
title: Hash Table Giratória
---

Ainda estudando e praticando testes de entrevista me veio essa em que seja possível realizar somas para todas as chaves de uma hash table. Curioso, nunca tinha pensado nesta feature. Imagine que temos uma tabela de hash entre inteiros em que `{ 1: 8, 2: 9 }`. A chave corresponde ao hash.

```
| -5: | -4: | -3: | -2: | -1: | 0: | 1:8 | 2:9 | 3: | 4: | 5: |...
```

Então eu aplico um comando na tabela inteira adicionando o valor 2 às chaves, fazendo seus elementos irem parar duas posições à frente de onde estavam. A posição 1 vira 3 e a posição 2 vira 5, mantendo os mesmos valores.

```
| -5: | -4: | -3: | -2: | -1: | 0: | 1: | 2: | 3: | 4:8 | 5:9 |...
```

Em primeiro momento eu pensei em mover posições em um vetor para resolver esta questão, mas em seguida descobri que a mesma lógica pode ser aplicada a números negativos, o que deixou tudo muito confuso na minha cabeça.

Depois de pensar em uma caminhada cheguei à conclusão que não é necessário ficar movendo memória uma vez que as posições relativas se mantém. Com base nisso eu desenvolvi a lógica de apenas manter um referencial do início "real" da tabela, ou seja, qual valor deve ser adicionado para se chegar à posição real após os deslocamentos. Dessa forma a posição na memória dos elementos permanece a mesma, mas do ponto de vista de indexação eles estariam, no exemplo acima, duas posições à frente. Para isso eu colocaria meu indexador duas posições atrás.

```
| -5: | -4: | -3: | -2: | -1: | 0: | 1:8 | 2:9 | 3: | 4: | 5: |...
                              |
                              beg    1     2     3    4    5   ...

add_to_key 2

| -5: | -4: | -3: | -2: | -1: | 0: | 1:8 | 2:9 | 3: | 4: | 5: |...
                  |
                  beg      1    2    3     4     5   ...
```

Agora sempre que alguém referenciar a posição 0 ela estará em -2 e assim por diante. Como a posição dentro de um array não precisa ser alterada não me preocupei em atualizar as chaves, apenas os campos internos de uma hash table: sua chave e valor.

A implementação não ficou exatamente assim, pois arrays não possuem índices negativos em C++. Para implementar eu já iniciei meu contador de início na metade do array, permitindo índices positivos e negativos na faixa de 100 elementos para cada lado.

```
struct HashTable {
    typedef vector<vector<int>> BufferType;

    HashTable(int tableSize) {
        this->tableSize = tableSize;
        bufferRaw = new BufferType[tableSize];
        buffer = shared_ptr<BufferType>(bufferRaw, default_delete<BufferType[]>());
        currentPosition = tableSize / 2; // for positive and negative room
    }

    BufferType& operator [] (int position) {
        int actualPosition = (currentPosition + position) % tableSize;
        return buffer.get()[actualPosition];
    }

    int tableSize;
    shared_ptr<BufferType> buffer;
    vector<vector<int>>* bufferRaw;
    int currentPosition;
};

int HashFunction(int key, int tableSize) {
    return key % tableSize;
}

vector<int>* FindItem(HashTable& hashTable, int key) {
    for (vector<int>& items : hashTable[HashFunction(key, hashTable.tableSize)]) {
        if (items[0] == key) {
            return &items;
        }
    }
    return nullptr;
}
```

Uma parte importante no código acima é entender que a hash function apenas pega o resto da divisão do tamanho da tabela. Porém, o método de subscrito da tabela considera onde ela começa, o `currentPosition`, que pode ser em qualquer lugar, dependendo das movimentações do usuário.

```
int actualPosition = (currentPosition + position) % tableSize;
```

O código que realiza as operações de fato apenas busca ou modifica valores:

```
vector<int> TestCase(vector<string> operations, vector<vector<int>> operands) {
    HashTable hashTable(DEFAULT_HASH_TABLE_SIZE);
    vector<int> results;

    for( int i = 0; i < operations.size(); ++i ) {
        if (operations[i] == "get") {
            if (auto* item = FindItem(hashTable, operands[i][0]))
                results.push_back((*item)[1]);
        }
        else if (operations[i] == "insert") {
            if (auto* item = FindItem(hashTable, operands[i][0]))
                (*item)[1] = operands[i][1];
            else
                hashTable[HashFunction(operands[i][0], hashTable.tableSize)].push_back(operands[i]);
        }
        else if (operations[i] == "add_to_values") {
            for (int j = 0; j < hashTable.tableSize; ++j)
                for (auto& item : hashTable[j])
                    item[1] += operands[i][0];
        }
        else if (operations[i] == "add_to_keys") {
            for (int j = 0; j < hashTable.tableSize; ++j)
                for (auto& item : hashTable[j])
                    item[0] += operands[i][0];
            hashTable.currentPosition -= operands[i][0];
        }
    }

    return results;
}
```

Um teste simples de rotacionamento de índices na hash table:

```
void TestCase005() {
    vector<string> operations;
    vector<vector<int>> operands;

    operations.push_back("insert");
    operands.push_back({ 1, 2 });
    operations.push_back("insert");
    operands.push_back({ 2, 3 });
    operations.push_back("add_to_values");
    operands.push_back({ 2 });
    operations.push_back("add_to_keys");
    operands.push_back({ -150 });
    operations.push_back("get");
    operands.push_back({ -148 });

    vector<int> results = TestCase(operations, operands);
    assert(results.size() == 1);
    assert(results[0] == 5);
}
```

E dessa forma é possível gastar apenas O(N) para atualizar chaves ou valores dentro de uma hash table, sem movimentação de memória alguma.
