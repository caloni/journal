#include <stdio.h>
#include <string.h>

void parse_url(char *url, char **hostname, char **port, char **path)
{
  char *p = strstr(url, "://");
  char *protocol = NULL;

  printf("URL: %s\n", url);

  if( p ) 
  {
    protocol = url;
    printf("teste1:%s\n%s\n", url, protocol);
    *p = '\0';
    printf("teste:%s\n\n%s\n\n", protocol, url);
    p += 3;
  }
  else 
  {
    p = url;
  }
}

int main(int argc, char* argv[])
{
  char* hostname = NULL, port = NULL, path = NULL;

  while( --argc )
  {
    parse_url(argv[argc], &hostname, &port, &path);
  }
}

