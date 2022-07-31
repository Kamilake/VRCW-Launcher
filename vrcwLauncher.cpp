#include <bits/stdc++.h>
//main with arguments
int main(int argc, char *argv[])
{
  //check if there are enough arguments
  if (argc < 2)
  {
    //print error message
    printf("Error: Not enough arguments\n");
    //exit program
    return 1;
  }
  //check if there are too many arguments
  if (argc > 2)
  {
    //print error message
    printf("Error: Too many arguments\n");
    //exit program
    return 1;
  }
  //check if the first argument is a valid file
  if (fopen(argv[1], "r") == NULL)
  {
    //print error message
    printf("Error: File not found\n");
    //exit program
    return 1;
  }
}