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
  char steamInstallPath[1024];
  HKEY hKey;
  if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\WOW6432Node\\Valve\\Steam", 0, KEY_READ, &hKey) == ERROR_SUCCESS)
  {
    DWORD dwType = REG_SZ;
    DWORD dwSize = sizeof(steamInstallPath);
    RegQueryValueEx(hKey, "InstallPath", NULL, &dwType, (LPBYTE)steamInstallPath, &dwSize);
    RegCloseKey(hKey);
  }
  else
  {
    printf("Error: Steam not found\n");
    system("pause");
    return 1;
  }
  printf("Steam install path: %s\n", steamInstallPath);
  char vrchatSteamId[] = "438100";
  char steamLibraryFoldersPath[1024];
  char vrchatSteamLibraryRootPath[1024];
  char vrchatInstallPath[1024];
  sprintf(steamLibraryFoldersPath, "%s\\steamapps\\libraryfolders.vdf", steamInstallPath);
  FILE *steamLibraryFoldersFile = fopen(steamLibraryFoldersPath, "r");
  if (steamLibraryFoldersFile == NULL)
  {
    printf("Error: Steam library folders not found\n");
    system("pause");
    return 1;
  }
  char line[1024];
  while (fgets(line, sizeof(line), steamLibraryFoldersFile))
  {
    if(strstr(line, "path") != NULL) 
    {
      char *token = strtok(line, "\"");
      token = strtok(NULL, "\"");
      token = strtok(NULL, "\"");
      token = strtok(NULL, "\"");
      sprintf(vrchatSteamLibraryRootPath, "%s", token);
      //replace "\\" to "\"
      for (int i = 0; i < strlen(vrchatSteamLibraryRootPath); i++)
      {
        if (vrchatSteamLibraryRootPath[i] == '\\' && vrchatSteamLibraryRootPath[i + 1] == '\\')
        {
          for (int j = i; j < strlen(vrchatSteamLibraryRootPath); j++)
          {
            vrchatSteamLibraryRootPath[j] = vrchatSteamLibraryRootPath[j + 1];
          }
        }
      }
      break;
    }
  }
  printf("VRChat steam library path: %s\n", vrchatSteamLibraryRootPath);
  char vrchatAppManifestPath[1024]; // appmanifest_438100.acf
  sprintf(vrchatAppManifestPath, "%s\\steamapps\\appmanifest_%s.acf", vrchatSteamLibraryRootPath, vrchatSteamId);
  printf("VRChat app manifest path: %s\n", vrchatAppManifestPath);
  
  FILE *vrchatAppManifestFile = fopen(vrchatAppManifestPath, "r");
  if (vrchatAppManifestFile == NULL)
  {
    printf("Error: VRChat not found\n");
    system("pause");
    return 1;
  }
  char line2[1024];
  while (fgets(line2, sizeof(line2), vrchatAppManifestFile))
  {
    if (strstr(line2, "installdir") != NULL)
    {
      char *token = strtok(line2, "\"");
      token = strtok(NULL, "\"");
      token = strtok(NULL, "\"");
      token = strtok(NULL, "\"");
      sprintf(vrchatInstallPath, "%s\\steamapps\\common\\%s", steamInstallPath, token);
      break;
    }
  }
  printf("VRChat install path: %s\n", vrchatInstallPath);

  // char vrchat_path[1024] = R"(C:\Program Files (x86)\Steam\steamapps\common\VRChat\)";
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
  sprintf(vrchat_args, "%s--url=create?roomId=%d^&hidden=false^&name=BuildAndRun^&url=file:///%s", vrchat_args, roomId, url_encoded);
  // printf("%s", );
  sprintf(cmd_buffer, "cmd /S /C \"\"%s/%s\" %s\"", vrchatInstallPath, vrchat_exe, vrchat_args); 
  printf ("cmd buffer: %s\n", cmd_buffer);
  printf("Launching VRChat...\n");
  system(cmd_buffer);
  system("pause");
  // AllocConsole();
  return 0;
}



  // printf("%s\n\n", cmd_buffer);
  // fclose(stdin);
  // fclose(stdout);
  // fclose(stderr);
  // FreeConsole();
  