# Write for people, computers and food

Hello. This is the source from my [blogue] published using [GitHub Pages] and my [books]. All the text is in the `journal.md` file.

The vast majority of my text is in my native language Portuguese, from Brazil. This should not be an issue now that machines translate flawlessly every pieace of internet to every conceivable language. But be my guest to learn something from this wonderful language and ask questions by [email] if you need to.

Thank you for the interest into my texts. Hope you find something useful here. Bye and thank you for all the steak.

## How to use (for programmers)

The whole thing works by using Python scripts and AWK. To publish I run the project inside Windows, but the code should work fine from any Unix env.

If you just cloned this repo the core text is in the `journal.md` file. The usable code, including the ones to publish this content, is in the `code` folder. The code currently parses just the minimum Markdown needed in `code/MarkdownParser.awk`. One day I will implement it as a whole. The dream version will support Common Markdown.

To publish the blog you should call `code/PublishBlog.py`, to generate the book `code/Journal2Book.py`. You can run a local version of the blog by using the `code/Journal2Blog.py` followed by `public/book/BlogServer.py` script. You can generate a MOBI version of the book by running `code/PublishBlog.py`.

A part of the code in `code/MetadataWriter.awk` writes the previously parsed text to metadata that are then transformed to a HTML blog in `code/BlogWriter.awk` or an EPUB book in `code/BookWriter.awk`.

The public folder, not source controlled, is the output folder that will contain publishable content (blog or book).

[blogue]: https://caloni.com.br
[GitHub Pages]: https://github.com/Caloni/caloni.github.io
[books]: https://www.amazon.com/s?rh=p_27%3AWanderley%2BCaloni
[email]: mailto:wanderley.caloni@gmail.com
