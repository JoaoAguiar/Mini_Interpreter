#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "HashTable.h"
#include "Functions.h"

int main(int argc, char* argv[]) {
  if(argc != 2) {
    printf("ERROR: your input is wrong!!\n");
    exit(1);
  }

  FILE* file;
  FILE* copyFile;
  file = fopen(argv[1], "r");
  jumpFile = fopen(argv[1], "r");

  if(file == NULL) {
     printf("ERROR: your file is empty!!\n");
     exit(1);
  }

  List list = readData(file);
  List jumpList = readData(jumpFile);

  runData(list, jumpList);

  return 0;
}
