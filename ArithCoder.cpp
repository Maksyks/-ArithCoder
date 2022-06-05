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

static bool Compare(Range& l, Range& r) {
	return l.Num > r.Num;
}

//����� � ������� �������
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
	ls.sort(Compare);
	int tmp = 0;//�������
	for (auto it = ls.begin();it != ls.end();it++) {
		it->L = tmp;
		it->R = tmp + it->Num;
		tmp = it->R;
	}
	return ls;
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
const unsigned short ThreeQuarters = OneQuarter * 3;
unsigned short High = h, Low = l;
void compressValue(const char* input_text = "Text.txt", const char* output_text = "Encode.txt")
{
	long long file_size = 0;
	long long compress_size = 0;

	struct stat s1 {};
	struct stat s2 {};

	if (!stat(input_text, &s1)) {
		file_size = s1.st_size;
	}
	else {
		perror("STAT ERROR ");
	}
	if (!stat(output_text, &s2)) {
		compress_size = s2.st_size;
	}
	else {
		perror("STAT ERROR ");
	}

	cout << "\nCompress value is:" << (compress_size + 0.0) / file_size << "\n";
}
int main() 
{
	ifstream input("Text.txt", ios::binary);
	if (!input.is_open()) { cout << "error"; return 0; }
	ofstream output("Encode.txt", ios::binary);
	if (!output.is_open()) { cout << "error"; return 0; }

	map<char, int> mp;
	list<Range> ls;
	char c;
	unsigned long long len = 0;
	//������� �������
	for (input.get(c);!input.eof();input.get(c)) mp[c]++;
	input.clear();
	input.seekg(0);

	ls = CreateList(mp);
	PrintList(ls);

	int k = mp.size();
	output.write((char*)&k, sizeof(k));
	//��������� ������� � ������� � �����
	for (auto it = mp.begin(); it != mp.end(); it++)
	{
		output.write((char*)&it->first, sizeof(it->first));
		output.write((char*)&it->second, sizeof(it->second));
	}

	list<Range>::iterator it;
	//������� �������
	int delitel = ls.back().R;
	int bits_to_follow = 0;
	char tmp = 0;
	int count = 0;
	char s;
	while (input.get(s)) 
	{
		for (it = ls.begin(); it->Sym != s; it++);
		//������������ �� ����� ���������� ����� � ����
		int d = High - Low + 1;
		High = Low + it->R * d / delitel - 1;
		Low = Low + it->L * d / delitel;
		for (;;) 
		{/* ���� � ������ ��������, ��������� ����p���� 
		 �� ������� 0*/
			if (High < Middle) 
			{
				count++;
				if (count == 8)
				{
					output.put(tmp);
					tmp = 0;
					count = 0;
				}
				for (; bits_to_follow > 0; bits_to_follow--)
				{
					tmp = tmp | (1 << (7 - count));
					count++;
					if (count == 8)
					{
						output.put(tmp);
						tmp = 0;
						count = 0;
					}
				}
			}
			/* ���� � ��p����, ��  ����� 1, � ����� ��p��� 
			��������� � �p���� ����� ����� */
			else if (Low >= Middle) 
			{
				tmp = tmp | (1 << (7 - count));
				count++;
				if (count == 8)
				{
					output.put(tmp);
					tmp = 0;
					count = 0;
				}
				for (; bits_to_follow > 0; bits_to_follow--)
				{
					count++;
					//��������� 8�� � ���������
					if (count == 8)
					{
						output.put(tmp);
						tmp = 0;
						count = 0;
					}
				}
				Low -= Middle; High -= Middle;

			}
			/*���� ������� �������� �������� �������� ������� 
			�� ���������� 1 � ������� ����� �����*/
			else if ((Low >= OneQuarter) && (High < ThreeQuarters)) 
			{//���������� ������� � 8�� ��� ��� ����
				bits_to_follow++;
				Low -= OneQuarter; High -= OneQuarter;
			}
			else break;
			//��������� �������
			Low <<= 1;
			High <<= 1;
			High++;
		}
	}
	output.seekp(0);
	input.seekg(0);
	compressValue();
	input.close();
	output.close();
}