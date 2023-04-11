---
categories: null
date: '2023-04-11T11:52:00-02:00'
draft: true
tags: null
title: Hacker Rank Warm Up
---

To solve the cloud jump problem create a loop and advance current position until finished. Try the double jump at first and ordinary jump else by incrementing position by 1 or 2 and incrementing jump counter. If in the end position just increment and get out of the loop. Return the jump counter.

To solve the counting valleys problem keep a valley counter that only increments when the hiker is coming up to the sea level. Monitor the altitude and the new altitude and compare. If the altitude was negative (into a valley) and the new altitude is zero (sea level) then that's a new valley to count. This strategy avoid to count valleys inside valleys before the hiker gets up to sea level.