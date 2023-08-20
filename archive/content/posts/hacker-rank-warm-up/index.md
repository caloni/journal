---
categories:
- coding
date: '2023-04-16'
link: https://www.hackerrank.com/interview/interview-preparation-kit
tags:
- interview
title: Hacker Rank Warm Up
---

Here I am doing interview exercise tests at Hacker Rank. I am trying to recap what I've been doing the last two months before going on. Let's see what I learned, starting with the Warm Up exercises.

# Counting Valleys

To solve the [counting valleys problem] keep a valley counter that only increments when the hiker is coming up to the sea level. Monitor the altitude and the new altitude and compare. If the altitude was negative (into a valley) and the new altitude is zero (sea level) then that's a new valley to count. This strategy avoid to count valleys inside valleys before the hiker gets up to sea level.

This solution has a complexity of O(n).

```
int countingValleys(int steps, string path) {
    int valleys = 0;
    int altitude = 0;

    for (int s = 0; s < steps; ++s)
    {
        int step = path[s] == 'D' ? -1 : 1;
        int newAltitude = altitude + step;

        if (altitude < 0 && newAltitude == 0)
        {
            valleys++;
        }
        altitude = newAltitude;
    }

    return valleys;
}
```

# Cloud Jump

To solve the [cloud jump problem] create a loop and advance current position until finished. Try the double jump at first and ordinary jump else by incrementing position by 1 or 2 and incrementing jump counter. If in the end position just increment and get out of the loop. Return the jump counter.

This solution has a complexity of O(n).

```
int jumpingOnClouds(vector<int> c) {
    int jumps = 0;
    size_t i = 0;

    while (i < c.size()) {
        if (i < c.size() - 2 && c[i + 2] == 0) {
            i += 2;
            ++jumps;
        }
        else if (i < c.size() - 1) {
            i += 1;
            ++jumps;
        }
        else {
            i += 1;
        }
    }

    return jumps;
}
```

# Repeated Strings

To solve the [repeated string problem] we count the 'a' occurrences for the full unique string and divide n by the size of the unique string size, getting the number of times we need to multiply the full occurrences.

For the partial string after the number of full unique strings we format this string and count independently this last part.

The total of occurrences is calculated multiplying the times there will be full unique strings and sum up the partial string 'a' occurrences.

This algorithm has a complexity of O(n) because we got to count every char.

```
long repeatedString(string s, long n) {
    long fullOccur = (long) count(s.begin(), s.end(), 'a');
    long fullMult = n / s.size();
    string partialStr = s.substr(0, n % s.size());
    long partialOccur = (long) count(partialStr.begin(), partialStr.end(), 'a');
    return fullOccur * fullMult + partialOccur;
}
```

# Sales by Match

To solve the [sales by match] problem we traverse all the array of socks and keep inserting and deleting a set of colors. If the current color is not found in the set we insert it. If the current color is found we increase a pair counter and remove the color from the set. The next time the same color appears it will be inserted again waiting for its pair.

The complexity of this solution is O(N), since we have to traverse all array of socks.

```
int sockMerchant(int n, vector<int> ar)
{
    int ret = 0;
    set<int> colors;
    for (int color : ar)
    {
        if (colors.find(color) != colors.end())
        {
            ret++;
            colors.erase(color);
        }
        else
        {
            colors.insert(color);
        }
    }
    return ret;
}
```

[cloud jump problem]: https://www.hackerrank.com/challenges/jumping-on-the-clouds
[counting valleys problem]: https://www.hackerrank.com/challenges/counting-valleys
[repeated string problem]: https://www.hackerrank.com/challenges/repeated-string
[sales by match]: https://www.hackerrank.com/challenges/sock-merchant