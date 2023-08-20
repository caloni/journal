#include <iostream>
#include <vector>

using namespace std;


int sum(int node, int left, int right, int posLeft, int posRight, const vector<int>& tree) {
    if (posLeft > posRight)
        return 0;
    if (posLeft == left && posRight == right)
        return tree[node];
    int nodeLeft = 2 * node;
    int nodeRight = 2 * node + 1;
    int middle = (left + right) / 2;
    return sum(nodeLeft, left, middle, posLeft, min(posRight, middle), tree)
        + sum(nodeRight, middle + 1, right, max(posLeft, middle + 1), posRight, tree);
}

void update(int node, int left, int right, int pos, int value, vector<int>& tree) {
    if (left == right) {
        tree[node] = value;
    } else {
        int nodeLeft = 2 * node;
        int nodeRight = 2 * node + 1;
        int middle = (left + right) / 2;
        if (pos <= middle)
            update(nodeLeft, left, middle, pos, value, tree);
        else
            update(nodeRight, middle+1, right, pos, value, tree);
        tree[node] = tree[nodeLeft] + tree[nodeRight];
    }
}

int main() {
    vector<int> tree(4 * 1000);

    update(1, 1, 999, 666, 42, tree);
    update(1, 1, 999, 600, 58, tree);
    update(1, 1, 999, 700, 45, tree);
    update(1, 1, 999, 999, 55, tree);

    vector<vector<int>> intervals = { 
        {666, 666}, {600, 700}, {600, 999}, 
        {1, 999}, {1, 599} };

    for (const vector<int>& i : intervals) {
        int isum = sum(1, 1, 999, i[0], i[1], tree);
        cout << "the interval [" << i[0] << "," << i[1] 
            << "] has the value " << isum << endl;
    }
}
