---
categories:
- code
- essays
date: '2023-06-05'
tags: null
title: Asciidoc is the best
---

After reading about the purpose and [current implementation of asciidoctor] I realize that asciidoc is the best text standard for the future of the blog. I say that based on the structure care of the format, the embebbed metadata into its own format and the clear objective of being a unique and formalized standard, what could simplify a lot the necessity of writers and publishers of keeping a text  base as the original and yet keep the structure inside with almost no effort in the side of the writers.

Based also on the article [Why I Built My Own Shitty Static Site Generator] I also realized hugo is not the best tool to convert my texts to publish, despite being the best for so many years. I need to implement this tool, for my single oriented purpose. Being that, the use of Hugo will be temporarily.

So the TL;DR of all this is:

- Convert the blog content to asciidoc (keeping hugo);
- Convert the ebook publishing to epub instead of mobi without hugo if possible;
- If not possible starting to work to a solution with asciidoc and its metadata
  instead of using the yaml header (not sure how to keep the blog running in
  this phase yet);
- Solved the epub issue, with or without hugo, the [epub PR] is finished.

The next step after this PR is to develop a tool to replace hugo and automatize blog, ebooks and whatever the blog can be turned on (with any set of content).

The final solution for the blog is a single folder with a bunch of asciidoc files and its metadata. The publishing tool must be able to filter and generate related content based on metadata and parameters.

I expect to keep or reconstruct the text history based in this single folder, but I am not sure yet if it is worth it.

[Why I Built My Own Shitty Static Site Generator]: https://ewintr.nl/shitty-ssg/why-i-built-my-own-shitty-static-site-generator/
[current implementation of asciidoctor]: https://docs.asciidoctor.org/asciidoc/latest/
[epub PR]: https://github.com/Caloni/blog/pull/7