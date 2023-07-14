#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
  char buffer[512];

  printf ("Enter name:\n");

  gets (buffer);
  
  printf ("Hello %s\n", buffer);
  
  return EXIT_SUCCESS;
}

