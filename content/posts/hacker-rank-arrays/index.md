---
categories:
- code
date: '2023-04-11T16:09:00-02:00'
draft: true
link: https://www.hackerrank.com/interview/interview-preparation-kit
tags:
- interview
title: Hacker Rank Array
---

The next step after [the Warm Up challenges] are the array challenges. And so I did it. Now I am going to recap what I did and how I did. And what complexity the algorithms have.

# Array Manipulation

To solve the [array manipulation problem] ChatGPT helped me with the code.  Now in the review I realized how segment trees work and how binary trees can be implemented using arrays.

The issue about this problem is that summing up intervals costs too much processing to large intervals. In order to do that, segment trees help, since its nodes contain the sum of all its nodes bellow. This way, to get the sum of determined intervals all we need to do is to get the bigger intervals and sum it up.

```
// Segment tree implementation
void update(int v, int tl, int tr, int l, int r, int x, vector<long>& t) {
    if (l > r) return;
    if (l == tl && r == tr) {
        t[v] += x;
    }
    else {
        int tm = (tl + tr) / 2;
        update(v * 2, tl, tm, l, min(r, tm), x, t);
        update(v * 2 + 1, tm + 1, tr, max(l, tm + 1), r, x, t);
    }
}

// Segment tree query
long query(int v, int tl, int tr, int pos, vector<long>& t) {
    if (tl == tr) {
        return t[v];
    }
    else {
        int tm = (tl + tr) / 2;
        if (pos <= tm) {
            return t[v] + query(v * 2, tl, tm, pos, t);
        }
        else {
            return t[v] + query(v * 2 + 1, tm + 1, tr, pos, t);
        }
    }
}

// Solution
long arrayManipulation(int n, vector<vector<int>> queries) {
    vector<long> t(4 * n);
    for (int i = 0; i < queries.size(); i++)
        update(1, 1, n, queries[i][0], queries[i][1], queries[i][2], t);

    long max_val = 0;
    for (int i = 1; i <= n; i++)
        max_val = max(max_val, query(1, 1, n, i, t));
    return max_val;
}
```

[array manipulation problem]: https://www.hackerrank.com/challenges/crush
[the Warm Up challenges]: {{< ref hacker-rank-warm-up >}}
