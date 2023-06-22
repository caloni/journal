/^= / { title[++idx] = substr($0, 3) }
/^[^=]+/ { content[idx] = content[idx] "<p>" $0 "</p>\n" }

END {
    for( i = 1; i <= idx; ++i ) {
        print title[i];
        print "\n";
        print content[i];
    }
}
