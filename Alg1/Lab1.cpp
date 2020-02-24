#include <fstream>
#include <iostream>

using namespace std;

typedef struct data
{
	int num;
	string str = NULL;
};

int main(int argc, char* argv[])
{
	if (argc != 2)
		return 1;
	int N;
	ifstream fin;
	ofstream fout;
	fin.open(argv[0]);
	fout.open(argv[1]);
	if (!fin || !fout)
		return 2;
	fin >> N;

	return 0;
}