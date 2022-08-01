#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <windows.h>
char rfc3986[256] = {0};
char html5[256] = {0};

void url_encoder_rfc_tables_init()
{

  int i;

  for (i = 0; i < 256; i++)
  {

    rfc3986[i] = isalnum(i) || i == '~' || i == '-' || i == '.' || i == '_' ? i : 0;
    html5[i] = isalnum(i) || i == '*' || i == '-' || i == '.' || i == '_' ? i : (i == ' ') ? '+'
                                                                                           : 0;
  }
}

char *url_encode(char *table, char *s, char *enc)
{

  for (; *s; s++)
  {

    if (table[*s])
      *enc = table[*s];
    else
      sprintf(enc, "%%%02X", *s);
    while (*++enc)
      ;
  }

  return (enc);
}

// main with arguments
int main(int argc, char *argv[])
{
  char cmd_buffer[1024];
  char vrchat_path[1024] = R"(C:\Program Files (x86)\Steam\steamapps\common\VRChat\)";
  char vrchat_exe[1024] = R"(VRChat.exe)";
  char vrchat_args[1024] = " \
    --fps 90 \
    --no-vr  \
    --enable-debug-gui \
    --enable-sdk-log-levels \
    --watch-worlds \
    --enable-udon-debug-logging \
  ";
  // check if there are enough arguments
  if (argc < 2)
  {
    // print error message
    printf("Error: Not enough arguments\n");
    printf("      Usage: vrcwLauncher.exe <VRCW location>\n");
    system("pause");
    // exit with error
    return 1;
  }
  // check if there are too many arguments
  if (argc > 2)
  {
    // print error message
    printf("Error: Too many arguments\n");
    system("pause");
    // exit program
    return 1;
  }
  // check if the first argument is a valid file
  if (fopen(argv[1], "r") == NULL)
  {
    // print error message
    printf("Error: File not found\n");
    system("pause");
    // exit program
    return 1;
  }
  int roomId = rand();
  char url[1024] = "";
  // url = argv[1];
  strncpy(url, argv[1], sizeof(url));
  char url_encoded[1024] = "";

  url_encoder_rfc_tables_init();
  url_encode(html5, url, url_encoded);
  // printf("url_encoded: %s\n", url_encoded);
  sprintf(vrchat_args, "%s--url=\"create?roomId=%d^&hidden=false^&name=BuildAndRun^&url=file:///%s\"", vrchat_args, roomId, url_encoded);
  // printf("%s", );
  sprintf(cmd_buffer, "cmd.exe /c \"%s%s\" %s", vrchat_path, vrchat_exe, vrchat_args);
  // printf("%s\n\n", cmd_buffer);
  fclose(stdin);
  fclose(stdout);
  fclose(stderr);
  FreeConsole();
  system(cmd_buffer);
  AllocConsole();
  return 0;
}