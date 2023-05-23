#include <windows.h>
//#include <cstdio>

struct value
{
	DWORD key;
	unsigned char* chPtr;
};

void QuickSortIter(value* arr, unsigned int N)
{
	unsigned int i, j, lb, ub, lower[128], upper[128], spos = 1, cpos;
	value c, temp;
	lower[1] = 0;
	upper[1] = N - 1;
	while (spos)
	{
		lb = lower[spos];
		ub = upper[spos];
		spos--;
		while (ub > lb)
		{
			cpos = (lb + ub) / 2;
			c = arr[cpos];
			i = lb;
			j = ub;
			while (i <= j)
			{
				while (arr[i].key < c.key) i++;
				while (arr[j].key > c.key) if (j) j--;
				if (i <= j)
				{
					temp = arr[i]; arr[i] = arr[j]; arr[j] = temp;
					i++; if (j) j--;
				}
			}
			if (i < cpos)
			{
				if (i < ub)
				{
					spos++;
					lower[spos] = i;
					upper[spos] = ub;
				}
				ub = j;
			}
			else
			{
				if (j > lb)
				{
					spos++;
					lower[spos] = lb;
					upper[spos] = j;
				}
				lb = i;
			}
		}
	}
}

int main(int argc, char* argv[])
{
	//SYSTEMTIME st;

	//GetSystemTime(&st);
	//printf("Starting time is: %02d:%02d:%02d.%03d\n", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	if (argc != 3)
		return 1;
	HANDLE hInputFile, hOutputFile, hInputMapping, hOutputMapping;
	__int64 qwInputSize = 0;
	unsigned int N = 0;
	if ((hInputFile = CreateFileA(argv[1], GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr)) == INVALID_HANDLE_VALUE)
		return 2;
	if (!GetFileSizeEx(hInputFile, (PLARGE_INTEGER)(&qwInputSize)))
	{
		CloseHandle(hInputFile);
		return 2;
	}
	if ((hInputMapping = CreateFileMappingA(hInputFile, NULL, PAGE_READONLY, 0, 0, NULL)) == nullptr)
	{
		CloseHandle(hInputFile);
		return 2;
	}
	CloseHandle(hInputFile);
	unsigned char* cpInputBuf = (unsigned char*)MapViewOfFile(hInputMapping, FILE_MAP_READ, 0, 0, qwInputSize);
	if (cpInputBuf == nullptr)
	{
		CloseHandle(hInputMapping);
		return 2;
	}
	unsigned char* cpN = cpInputBuf;
	while (*cpInputBuf != '\r')
		if (*cpInputBuf < '0' || *cpInputBuf > '9')
		{
			UnmapViewOfFile(cpInputBuf);
			CloseHandle(hInputMapping);
			return 3;
		}
		else
		{
			N *= 10;
			N += *cpInputBuf - '0';
			cpInputBuf++;
		}
	while (*cpInputBuf != '\n')
		cpInputBuf++;
	cpInputBuf++;
	value* Array = new value[N];
	for (unsigned int i = 0; i < N; i++)
	{
		Array[i].key = 0;
		Array[i].chPtr = cpInputBuf;
		if (*cpInputBuf < '0' || *cpInputBuf > '9')
		{
			UnmapViewOfFile(cpInputBuf);
			CloseHandle(hInputMapping);
			delete[] Array;
			return 3;
		}
		while (*cpInputBuf != ' ')
			if (*cpInputBuf < '0' || *cpInputBuf > '9')
			{
				UnmapViewOfFile(cpInputBuf);
				CloseHandle(hInputMapping);
				delete[] Array;
				return 3;
			}
			else
			{
				Array[i].key *= 10;
				Array[i].key += *cpInputBuf - '0';
				cpInputBuf++;
			}
		cpInputBuf++;
		while (*cpInputBuf != '\n' && *cpInputBuf != '\0')
			cpInputBuf++;
		if (*cpInputBuf != '\0')
			cpInputBuf++;
		else if (i != N - 1)
		{
			UnmapViewOfFile(cpInputBuf);
			CloseHandle(hInputMapping);
			delete[] Array;
			return 3;
		}
	}
	//GetSystemTime(&st);
	//printf("File read time is: %02d:%02d:%02d.%03d\n", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	QuickSortIter(Array, N);
	//GetSystemTime(&st);
	//printf("Sort time is: %02d:%02d:%02d.%03d\n", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	if ((hOutputFile = CreateFileA(argv[2], GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr)) == INVALID_HANDLE_VALUE)
	{
		delete[] Array;
		UnmapViewOfFile(cpInputBuf);
		CloseHandle(hInputMapping);
		return 2;
	}
	if ((hOutputMapping = CreateFileMappingA(hOutputFile, NULL, PAGE_READWRITE, 0, qwInputSize, NULL)) == nullptr)
	{
		delete[] Array;
		UnmapViewOfFile(cpInputBuf);
		CloseHandle(hInputMapping);
		CloseHandle(hOutputFile);
		return 2;
	}
	unsigned char* cpOutputBuf = (unsigned char*)MapViewOfFile(hOutputMapping, FILE_MAP_WRITE, 0, 0, qwInputSize);
	if (cpOutputBuf == nullptr)
	{
		delete[] Array;
		UnmapViewOfFile(cpInputBuf);
		CloseHandle(hInputMapping);
		CloseHandle(hOutputMapping);
		CloseHandle(hOutputFile);
		return 2;
	}
	while (*cpN != '\n')
	{
		*cpOutputBuf = *cpN;
		cpOutputBuf++; cpN++;
	}
	*cpOutputBuf = *cpN;
	cpOutputBuf++;
	for (unsigned int i = 0; i < N; i++)
	{
		while (*(Array[i].chPtr) != '\n' && *(Array[i].chPtr) != '\0')
		{
			*cpOutputBuf = *(Array[i].chPtr);
			cpOutputBuf++; Array[i].chPtr++;
		}
		if (*(Array[i].chPtr) == '\0')
		{
			*cpOutputBuf = '\r';
			cpOutputBuf++;
			*cpOutputBuf = '\n';
			cpOutputBuf++;
		}
		else
		{
			*cpOutputBuf = '\n';
			cpOutputBuf++;
		}
	}
	//GetSystemTime(&st);
	//printf("File write time is: %02d:%02d:%02d.%03d\n", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	delete[] Array;
	UnmapViewOfFile(cpInputBuf);
	CloseHandle(hInputMapping);
	UnmapViewOfFile(cpOutputBuf);
	CloseHandle(hOutputMapping);
	CloseHandle(hOutputFile);
	return 0;

}