#include <iostream>
#include <list>
#include <map>
#include <vector>
#include <stdbool.h>
#include <iterator>
#include <fstream>
using namespace std;

class Range {
public:
	unsigned int Num;
	int L, R;
	char Sym;

	Range();
	Range(char, int);
	void PrintRange();
};

static bool compare(Range& l, Range& r) {
	return l.Num > r.Num;
}

Range::Range(char _c, int _a) {
	Sym = _c;
	Num = _a;
}

list<Range> CreateList(map<char, int>& mp) {
	list<Range> ls;
	for (auto it = mp.begin(); it != mp.end(); it++) {
		Range p(it->first, it->second);
		ls.push_back(p);
	}
	ls.sort(compare);
	int tmp = 0;
	for (auto it = ls.begin();it != ls.end();it++) {
		it->L = tmp;
		it->R = tmp + it->Num;
		tmp = it->R;
	}
	return ls;
}


unsigned long long SymFreqReader(map<char, int>* mp, ifstream& input) {
	int key, q;
	unsigned long long len = 0;
	input.read((char*)&q, sizeof(q));
	char s;
	for (int i = 0;i < q;i++) {
		input.read((char*)&s, sizeof(s)); input.read((char*)&key, sizeof(key));
		(*mp)[s] = key;
		len += key;
	}

	return len;
}

void Range::PrintRange() {
	cout << Sym << " : " << L << " - " << R << endl;
}

void PrintList(list<Range> ls) {
	for (auto it = ls.begin();it != ls.end();it++) {
		it->PrintRange();
	}
}

const unsigned short l = 0, h = -1;
const unsigned short OneQuarter = (h + 1) / 4;
const unsigned short Middle = OneQuarter * 2;
const unsigned short ThreeQuarter = OneQuarter * 3;
//декодер

unsigned short Low = l, High = h;
int main() {
	ifstream input("Encode.txt", ios::binary);
	if (!input.is_open()) { cout << "error"; return 0; }
	ifstream Text("Text.txt", ios::binary);
	if (!input.is_open()) { cout << "error"; return 0; }
	ofstream output("Decode.txt", ios::binary);
	if (!output.is_open()) { cout << "error"; return 0; }

	map<char, int>mp;
	unsigned long long len = SymFreqReader(&mp, input);


	list<Range> ls = CreateList(mp);
	PrintList(ls);
	list<Range>::iterator it;

	int delitel = ls.back().R;
	//текущее значение кода
	unsigned short value = (input.get() << 8) | input.get();

	char tmp = input.get();
	int count = 0;

	int q = 0;
	//счетчик частот
	while (len)
	{
		long d = High - Low + 1;
		unsigned int freq = (((((unsigned long long)value - Low + 1) * delitel)) - 1) / (d);
		for (it = ls.begin(); it->R <= freq; it++);

		High = Low + it->R * d / delitel - 1;
		Low = Low + it->L * d / delitel;
		for (;;)
		{
			if (High < Middle);
			//расширение нижней части
			else if (Low >= Middle)
			{
				value -= Middle;
				Low -= Middle;
				High -= Middle;
			}
			else if ((Low >= OneQuarter) && (High < ThreeQuarter))
			{
				value -= OneQuarter;
				Low -= OneQuarter;
				High -= OneQuarter;
			}
			else break;
			//увеличиваем мастштаб интервала
			Low <<= 1;
			High <<= 1;
			High++;
			value <<= 1;
			value += ((tmp & (1 << (7 - count))) >> (7 - count));
			count++;
			//cout<<it->c<<" : "<<l0<<" - "<<h0<<endl;
			if (count == 8)
			{
				tmp = input.get();
				count = 0;
			}
		}
		output << it->Sym;
		len--;
	}
	input.close();

	Text.seekg(0, ios::end);
	ifstream check("Decode.txt", ios::binary);
	if (!check.is_open()) { cout << "error"; return 0; }
	char ck1,ck2;
	while (!check.get() && !Text.get())
	{
		ck1 = check.get();
		ck2 = Text.get();
		if (ck1 != ck2)
		{
			puts("Files not equal\n");

		}

	}
	puts("Files are equal");
	input.close();	output.close();
	output.close();
}

