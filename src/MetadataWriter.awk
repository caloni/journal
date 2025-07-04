# Write parsed metadata to metadata file.

function MetadataWriter_Flush(    l_slug, l_tag, l_tags)
{
  for( l_slug in G_METADATA["chaptersBySlug"] )
  {
    print "metadata_chapter", l_slug, G_METADATA["chaptersBySlug"][l_slug] > G_METADATA["output"]
  }
  if( "tags" in G_METADATA )
  {
    for( l_tag in G_METADATA["tags"] )
    {
      l_tags = l_tags " " l_tag
    }
    print "tags:", l_tags
      print "metadata_tags", l_tags > G_METADATA["output"]
  }
}

# Flushes all captured metadata from posts.
# Delete global variables to not mixed up with further scripts.
# Maintain global variables that should be used in further scripts.
END {
  MetadataWriter_Flush()
  # cleanup
  split("", G_NEW_POST)
  split("", G_CONTENT_STATE)
  split("", G_METADATA)
  # not clean G_INDEX
}
