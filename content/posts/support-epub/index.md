---
categories:
- code
date: '2023-05-05T10:00:58-02:00'
draft: true
tags:
- english
title: Support EPUB
---

Amazon gave up about supporting MOBI format and the next generation readers are using ePUB instead. My blog published as a ebook is using Kindlegen to make the book structure and it is a monolithic book hard to open in ePUB readers.

I need to discontinue Kindlegen approach and use ePUB format as the main format. It is still possible to support MOBI to old readers as mine, but having the ePUB output it can be converted to MOBI format with one command.

In order to do that [this link](http://bbebooksthailand.com/bb-epub-kindlegen-tutorial.html) has the tips and tricks to maintain a bunch of pages in a book format that is publishable. Nevertheless, I already stole a hugo conversion from [this blog](https://weitblick.org). It is already in the master branch (themes/epub).

However, before the migration, the same indexes available in the current book must be available after this conversion, what means index by letters starting the posts, index by tags and categories and inner links working to browse between posts. This is not ready yet.
