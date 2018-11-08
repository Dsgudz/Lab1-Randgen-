#include <iostream>
#include <string>

#define DEBUGCOUTMAYBE true

using namespace std;


class reallylong
{
public:
	reallylong() { theword = new long[Npack]; for (int i = 0; i < Npack; i++) theword[i] = 0; /*reallylong::ptmult = *reallylong::facemult;*/ };
	~reallylong() { if (!(theword == nullptr)) delete[] theword; };

	reallylong(const reallylong &init)
	{
		theword = new long[Npack]; for (int i = 0; i < Npack; i++) theword[i] = init.theword[i];
	}

	reallylong(reallylong &&init)
	{
		theword = init.theword; init.theword = nullptr; Npack = init.Npack;
	}
	
	const void operator *= (reallylong b) { *this = /*(reallylong->*ptmult)*/karatsuba(*this, b); };

	operator string()
	{
		string st = "";
		string st1;
		long siz = size();
		for (long i = siz - 1; i >= 0; i--)
		{
			st1 = "";
			if (theword[i] == 0 && st != "")
				st += '0';
			else
			{
				while (theword[i] > 0)
				{
					st1 += (theword[i] % 10 + 48);
					theword[i] /= 10;
				}
				for (int j = st1.length() - 1; j >= 0; j--)
					st += st1[j];
			}
		}
		return st;
	};

	const reallylong operator = (string st)
	{
		long c = 1;
		long long i = 0;
		long long j = st.length()-1;
		while (j >= 0)
		{
			if (c >= mdl)
			{
				c = 1;
				i++;
			}
			theword[i] += c*(st[j] - 48);


			j--;
			c *= 10;
		}
		for (int k = i + 1; k < Npack; k++)
			theword[k] = 0;
		return *this;
	};

	const reallylong operator = (reallylong other)
	{
		for (int i = 0; i < Npack; i++)
			this->theword[i] = other.theword[i];

		return *this;
	}

	const void operator+=(reallylong other)
	{
		for (int i = 0; i < Npack - 1; i++)
		{
			theword[i] += other.theword[i];
		}
		checkcollisions();
	}

	const void operator-=(reallylong other)
	{
		for (int i = 0; i < Npack - 1; i++)
		{
			theword[i] -= other.theword[i];
		}
		checkcollisions();
	}

	static reallylong facemult(reallylong a1, reallylong a2)
	{
		reallylong res;
		long siz0 = a1.siz0();
		long siz1 = a1.size();
		long siz2 = a2.siz0();
		long siz3 = a2.size();
		res = "0";
		for (int i = siz0 - 1; i < siz1; i++)
		{
			for (int j = siz2 - 1; j < siz3; j++)
			{

				res.theword[i + j] += a1.theword[i] * a2.theword[j];

			}
			res.checkcollisions();			
		}


		return res;
	}

	static reallylong karatsuba(reallylong a1, reallylong a2)
	{ 
		if (((a1.size() - a1.siz0()) == 0) || ((a2.size() - a2.siz0()) == 0))
		{
			return reallylong::facemult(a1, a2);
		}
		long x = 0;
		reallylong a, b, c, d, ac, abcd, bd, res;
		divkara(a1, a, b, a2, c, d, x);

		ac = karatsuba(a, c);

		bd = karatsuba(b, d);

		a += b;
		c += d;

		abcd = karatsuba(a, c);

		res = bd;

		res.modmult(x);

		res += abcd;
		
		res -= ac;

		res -= bd;

		res.modmult(x);

		res += ac;

		return res;
	};


	void modmult(long x)
	{
		for (int j = 0; j<x; j++)
		for (int i = Npack - 1; i > 0; i--)
			theword[i] = theword[i - 1];
		for (int i = 0; i<x; i++)
			theword[i] = 0;
	}

	static reallylong(reallylong::*ptmult)(reallylong, reallylong);
/*	static void setmult(int i)
	{
		if (i == 0)
			reallylong::ptmult = &reallylong::facemult;
		if (i == 1)
			reallylong::ptmult = &reallylong::karatsuba;
	}*/

	// a*b <==> a->operator*(b)

private:
	
	void checkcollisions()
	{
		for (int i = 0; i < Npack - 1; i++)
		{
			if (theword[i] < 0)
			{
				theword[i + 1] += theword[i] / mdl - 1;
				theword[i] = mdl + theword[i] % mdl;
			}
			else
			{
				theword[i + 1] += theword[i] / mdl;
				theword[i] = theword[i] % mdl;
			}
		}
	}

	long size()
	{
		long res = 0;
		for (int i = 0; i < Npack; i++)
		{
			if (theword[i] != 0)
				res = i;
		}
		return res + 1;
	}

	long siz0()
	{
		long res = 0;
		while (theword[res] == 0)
		{
			res++;
		}
		return res + 1;
	}

	static void divkara(reallylong a1, reallylong &a, reallylong &b, reallylong a2, reallylong &c, reallylong &d, long& x)
	{
		a = "0";
		b = "0";
		c = "0";
		d = "0";
		reallylong* a11;
		reallylong* a22;
		if (a2.size() >= a1.size())
		{
			a11 = &a1;
			a22 = &a2;
		}
		else
		{
			a11 = &a2;
			a22 = &a1;
		}

		long s = a22->size();
		for (int i = 0; i < s / 2; i++)
		{
			a.theword[i] = a11->theword[i];
			c.theword[i] = a22->theword[i];
		}

		for (int i = s / 2; i < s; i++)
		{
			b.theword[i - s / 2] = a11->theword[i];
			d.theword[i - s / 2] = a22->theword[i];
		}

		x = a.size();
	}

	const static long mdl = 10;
	long long Npack = 30;
	long* theword;


	void debug_cout()
	{
		cout << endl;
		long size1 = size();
		long size0 = siz0();
//		cout << "Mdl     = " << mdl << endl;
	//	cout << "Npack   = " << Npack << endl;
	//	cout << "Size    = " << size1 << endl;
	//	cout << "Siz0    = " << size1 << endl;
		cout << "theword = ";


		for (int i = Npack -1; i>=0; i--)
		{
			cout << theword[i] << ' ';
		}
		cout << endl << endl;
		
	}

};






int main()
{

	reallylong A, B, C;
	//reallylong::setmult(0);
	A = "872654351";
	B = "1606546879";

	C = A;
	C *= B;
	cout << string(A) << "*" << string(B) << "=" << string(C) << endl;
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