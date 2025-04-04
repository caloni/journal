# Write for people, computers and food

Hello. This is the source from my [blogue] published using [GitHub Pages] and my [books]. The whole thing works by using Python scripts and AWK.

## How to use

If you just cloned this repo the core text is in the `journal.md` file. The code currently parses just the minimum Markdown needed. One day I will implement it as a whole. The dream version will support Common Markdown.

The usable code, including the ones to publish this content, is in the `code` folder. To publish the blog you should call `code/PublishBlog.py`, to generate the book `code/Journal2Book.py`. You can run a local version of the blog by using the `code/Journal2Blog.py` followed by `public/book/BlogServer.py` script. To publish I run the project inside Windows, but the code should work fine from any Unix env.

The public folder, not source controlled, is the output folder that will contain publishable content (blog or book).

The parsing and transformation scripts are written in AWK. A part of the code in `code/MetadataWriter.awk` parse the `journal.txt` to metadata that are then transformed to a HTML blog in `code/BlogWriter.awk` or an EPUB book (convertible to MOBI format as well) in `code/BookWriter.awk`.

The vast majority of my text is in my native language Portuguese, from Brazil. This should not be an issue now that machines translate flawlessly every pieace of internet to every conceivable language. But be my guest to learn something from this wonderful language and ask questions by [email] if you need to.

Thank you for the interest into my texts. Hope you find something useful here. Bye and thank you for all the steak.

[blogue]: https://caloni.com.br
[GitHub Pages]: https://github.com/Caloni/caloni.github.io
[books]: https://www.amazon.com/s?rh=p_27%3AWanderley%2BCaloni
[email]: mailto:wanderley.caloni@gmail.com
