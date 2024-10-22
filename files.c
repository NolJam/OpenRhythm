#include "files.h"

int files_list_levels(const char* sDir)
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

	FindClose(hFind);

	return TRUE;
}