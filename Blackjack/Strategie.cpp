#include "Strategie.h"
#include "Reguli.h"

const char* numeActiune(Actiune a)
{
	switch (a)
	{
	case Actiune::Hit:    return "Hit";
	case Actiune::Stand:  return "Stand";
	case Actiune::Double: return "Double";
	}
	return "?";
}

static int upcardDealer(Carte cartaDealer)
{
	int v = valoareBlackjack(cartaDealer);
	return (v == 1) ? 11 : v;
}

static Actiune strategieSoft(int scor, int up, bool poateDubla)
{
	if (scor >= 19) return Actiune::Stand;

	if (scor == 18)
	{
		if (up >= 9) return Actiune::Hit;
		if (poateDubla && up >= 3 && up <= 6) return Actiune::Double;
		return Actiune::Stand;
	}

	if (scor == 17)
		return (poateDubla && up >= 3 && up <= 6) ? Actiune::Double : Actiune::Hit;

	if (scor == 15 || scor == 16)
		return (poateDubla && up >= 4 && up <= 6) ? Actiune::Double : Actiune::Hit;

	if (scor == 13 || scor == 14)
		return (poateDubla && up >= 5 && up <= 6) ? Actiune::Double : Actiune::Hit;

	return Actiune::Hit;
}

static Actiune strategieHard(int scor, int up, bool poateDubla)
{
	if (scor >= 17) return Actiune::Stand;

	if (scor >= 13 && scor <= 16)
		return (up >= 2 && up <= 6) ? Actiune::Stand : Actiune::Hit;

	if (scor == 12)
		return (up >= 4 && up <= 6) ? Actiune::Stand : Actiune::Hit;

	if (scor == 11)
		return (poateDubla && up <= 10) ? Actiune::Double : Actiune::Hit;

	if (scor == 10)
		return (poateDubla && up >= 2 && up <= 9) ? Actiune::Double : Actiune::Hit;

	if (scor == 9)
		return (poateDubla && up >= 3 && up <= 6) ? Actiune::Double : Actiune::Hit;

	return Actiune::Hit;
}

Actiune sfatBaza(int scorJucator, bool esteSoft, Carte cartaDealer, bool poateDubla)
{
	int up = upcardDealer(cartaDealer);

	if (esteSoft)
		return strategieSoft(scorJucator, up, poateDubla);

	return strategieHard(scorJucator, up, poateDubla);
}
