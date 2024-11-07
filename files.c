#include "files.h"

int files_list_levels(const char* sDir) // debug
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	char sPath[2048];

	printf("Source Directory is : %s\n\n", sDir);

	sprintf_s(sPath, sizeof(sPath), "%s\\*.lvl", sDir);

	printf("Path: %s\n\n", sPath);

	hFind = FindFirstFileA(sPath, &FindFileData);

	if (hFind == INVALID_HANDLE_VALUE)
	{
		printf("FindFirstFile failed (%d)\n", GetLastError());
		return FALSE;
	}
	else
	{
		printf("First File Found: %s\n\n", (char*)FindFileData.cFileName);
	}

	while (FindNextFileA(hFind, &FindFileData))
	{
		printf("File: %s\n\n", (char*)FindFileData.cFileName);
	}

	FindClose(hFind);

	return TRUE;
}

int files_get_levels(char** level_names)
{
	const char* sDir = ".\\";

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	char sPath[2048];

	int i = 0;

	printf("=== FILLING LEVEL NAME MATRIX ===\n\n");

	printf("Source Directory is : %s\n\n", sDir);

	sprintf_s(sPath, sizeof(sPath), "%s\\*.lvl", sDir);

	printf("Path: %s\n\n", sPath);

	hFind = FindFirstFileA(sPath, &FindFileData);

	if (hFind == INVALID_HANDLE_VALUE)
	{
		printf("FindFirstFile failed (%d)\n", GetLastError());
		return FALSE;
	}
	else
	{
		printf("First File Found: %s\n\n", (char*)FindFileData.cFileName);
		strcpy_s(level_names[i], 2048, (char*)FindFileData.cFileName);
		printf("    Level Name %d: %s\n\n", i, level_names[i]);
		printf("         i value: %d\n\n", i);
		printf("  LEVEL NAME [0]: %s\n\n\n", level_names[0]);
		i++;
	}

	while (FindNextFileA(hFind, &FindFileData) && i < MAX_LEVELS && i > 0)
	{
		printf("Next  File Found: %s\n\n", (char*)FindFileData.cFileName);
		//level_names[i] = (char*)FindFileData.cFileName;
		strcpy_s(level_names[i], 2048, (char*)FindFileData.cFileName);
		printf("    Level Name %d: %s\n\n", i, level_names[i]);
		printf("         i value: %d\n\n", i);
		printf("  LEVEL NAME [0]: %s\n\n\n", level_names[0]);
		i++;
	}

	g_num_levels = i;

	printf("First Pre Returned Level Name: %s\n\n", level_names[0]);

	FindClose(hFind);

	return TRUE;
}