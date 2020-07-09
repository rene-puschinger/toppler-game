#include <fstream>

typedef unsigned char byte;

typedef struct {
	byte r, g, b;
} PALETTEENTRY;

using namespace std;

/* convert jasc-type palette to standard 768 palette */
void jasc2pal(char* input, char* output);

int main(void) {
	jasc2pal("items.pal", "toppler.pal");

	return 0;
}

void jasc2pal(char* input, char* output) {
	ifstream fin(input);
	ofstream fout(output, ios::binary);
	char s[20];
	fin.getline(s, 20, '\n');	/* read three first irrelevant lines */
	fin.getline(s, 20, '\n');
	fin.getline(s, 20, '\n');
	int c;
	for (int i = 0; i < 768; i++) {
		fin >> c;
		fout.put((byte) c);
	}
	fin.close();
	fout.close();
}