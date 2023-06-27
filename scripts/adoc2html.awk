/^= / {
    title = substr($0, 3);
    posts[title]["title"] = "<h1>" title "</h1>";
    ++postCount;
}

/^[^=]+/ {
    posts[title]["content"] = posts[title]["content"] "<p>" $0 "</p>\n";
}

END {
    for( post in posts ) {
        print posts[post]["title"];
        print "";
        print posts[post]["content"];
    }
}
