#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

static const int DEFAULT_HASH_TABLE_SIZE = 200;

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

void TestCase001() {
    vector<string> operations;
    vector<vector<int>> operands;

    operations.push_back("insert");
    operands.push_back({ 1, 2 });
    operations.push_back("insert");
    operands.push_back({ 2, 3 });
    operations.push_back("add_to_values");
    operands.push_back({ 2 });
    operations.push_back("get");
    operands.push_back({ 2 });

    vector<int> results = TestCase(operations, operands);
    assert(results.size() == 1);
    assert(results[0] == 5);
}

void TestCase002() {
    vector<string> operations;
    vector<vector<int>> operands;

    operations.push_back("insert");
    operands.push_back({ 1, 2 });
    operations.push_back("insert");
    operands.push_back({ 2, 3 });
    operations.push_back("add_to_values");
    operands.push_back({ 2 });
    operations.push_back("add_to_keys");
    operands.push_back({ 1 });
    operations.push_back("get");
    operands.push_back({ 3 });

    vector<int> results = TestCase(operations, operands);
    assert(results.size() == 1);
    assert(results[0] == 5);
}

void TestCase003() {
    vector<string> operations;
    vector<vector<int>> operands;

    operations.push_back("insert");
    operands.push_back({ 1, 2 });
    operations.push_back("insert");
    operands.push_back({ 2, 3 });
    operations.push_back("add_to_values");
    operands.push_back({ 2 });
    operations.push_back("add_to_keys");
    operands.push_back({ -1 });
    operations.push_back("get");
    operands.push_back({ 1 });

    vector<int> results = TestCase(operations, operands);
    assert(results.size() == 1);
    assert(results[0] == 5);
}

void TestCase004() {
    vector<string> operations;
    vector<vector<int>> operands;

    operations.push_back("insert");
    operands.push_back({ 1, 2 });
    operations.push_back("insert");
    operands.push_back({ 2, 3 });
    operations.push_back("add_to_values");
    operands.push_back({ 2 });
    operations.push_back("add_to_keys");
    operands.push_back({ 150 });
    operations.push_back("get");
    operands.push_back({ 152 });

    vector<int> results = TestCase(operations, operands);
    assert(results.size() == 1);
    assert(results[0] == 5);
}

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

int main() {
    TestCase001();
    TestCase002();
    TestCase003();
    TestCase004();
    TestCase005();
}

