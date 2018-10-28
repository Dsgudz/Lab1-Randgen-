#include <iostream>
#include <string>

using namespace std;


class reallylong
{
public:
	reallylong() { theword = new long[Npack]; theword = {}; ptmult = &reallylong::karatsuba; };
	~reallylong()noexcept {	delete[] theword;};

	reallylong(const reallylong &init)
	{
		theword = new long[Npack]; for (int i = 0; i < Npack; i++) theword[i] = init.theword[i]; ptmult = &reallylong::karatsuba;
	}

	reallylong(const reallylong &&init)
	{
		theword = init.theword; Npack = init.Npack; ptmult = init.ptmult;
	}
	
	reallylong operator* (reallylong b) { (*ptmult)(*this, b); };

	operator string() {
							string st = "";	
							string st1;
							for (int i = Npack; i >= 0; i--)
							{
								st1 = "";
								if (theword[i] == 0 && st != "")
									st += '0';
								while (theword[i] >= 0)
								{
									st1 += theword[i] % 10;
									theword[i] /= 10;
								}
								for (int j = st1.length(); j >= 0; j++)
									st += st1[j];
							}
							return st;
						};

	const reallylong operator = (string st)		{
													long c = 1;
													long long i = 0;
													long long j = 0;
													while (j<st.length())
													{
														if (c >= mdl)
														{
															c = 1;
															i++;
														}
														theword[i] += c*st[j];


														j++;
														c *= 10;
													}
													return *this;
												};


	static reallylong karatsuba(reallylong a1, reallylong a2) { return a1; };

	// a*b <==> a->operator*(b)

private:
	
	

	reallylong (*ptmult)(reallylong, reallylong);

	const static long mdl = 1000;
	long long Npack = 20;
	long* theword;
};






int main()
{

	reallylong A;
	A = "123";
	cout << string(A);
    return 0;
}

/*
https://ru.stackoverflow.com/questions/128511/%D0%9A%D0%B0%D0%BA-%D0%B2-%D0%A1-%D0%BF%D0%B5%D1%80%D0%B5%D0%B4%D0%B0%D1%82%D1%8C-%D1%84%D1%83%D0%BD%D0%BA%D1%86%D0%B8%D1%8E-%D0%B2-%D0%BA%D0%B0%D1%87%D0%B5%D1%81%D1%82%D0%B2%D0%B5-%D0%BF%D0%B0%D1%80%D0%B0%D0%BC%D0%B5%D1%82%D1%80%D0%B0


Функция:

int MyFunction(int a){ printf("Hi!\n");   return 2*a; }
Объявляем:

int (*pt2Func)(int ) = NULL;
Инициализируем указатель:

pt2Func = &MyFunction;
Передаем в функцию и используем:

void PassPointer(int (*pt2Func)(int )) {
int result = (*pt2Func)(12);
cout << result << endl;
}


*/