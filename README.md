# Write for people, computers and food

Hello! This repository contains the source code for my published [blogs] and [books]. All content is in the `journal.md` file, primarily written in Brazilian Portuguese. Feel free to explore and reach out via [email] if you have any questions.

## How It Works

This project uses Python scripts to run AWK scripts located in the `src` folder. I develop on Windows, but the code should work fine on Unix systems as well.

Main scripts:

- `build.py`: Builds the blog and book.
- `test.py`: Runs the tests.

AWK scripts are organized by function, such as:

- `MarkdownParser.awk`: Parses Markdown content.
- `BookWriter.awk`: Generates book files.

The `public` folder (not under version control) contains the output ready for publication.

For more details, check the comments within the code.
 
[blogs]: https://caloni.com.br
[books]: https://www.amazon.com/s?rh=p_27%3AWanderley%2BCaloni
[GitHub Pages]: https://github.com/Caloni/caloni.github.io
[email]: mailto:wanderleycaloni@gmail.com
