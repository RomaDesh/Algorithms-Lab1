#include <windows.h>

typedef struct value
{
	DWORD key;
	char* chp;
};

int main(int argc, char* argv[])
{
	if (argc != 3)
		return 1;
	HANDLE hFile, hInputMapping, hOutputMapping;
	__int64 qwInputSize = 0;
	if ((hFile = CreateFileA(argv[1], GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr)) == INVALID_HANDLE_VALUE)
		return 2;
	if (!GetFileSizeEx(hFile, (PLARGE_INTEGER)(&qwInputSize)))
	{
		CloseHandle(hFile);
		return 2;
	}
	if ((hInputMapping = CreateFileMappingA(hFile, NULL, PAGE_READONLY, 0, 0, NULL)) == nullptr)
	{
		CloseHandle(hFile);
		return 2;
	}
	CloseHandle(hFile);
	unsigned char* cpInputMap = (unsigned char*)MapViewOfFile(hInputMapping, FILE_MAP_READ, 0, 0, qwInputSize);
	if (cpInputMap == nullptr)
	{
		CloseHandle(hInputMapping);
		return 2;
	}

}