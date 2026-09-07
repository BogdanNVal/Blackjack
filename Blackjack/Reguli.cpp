#include "Reguli.h"

int valoareBlackjack(const Carte& c)
{
	int v = c.getValoare();
	if (v == 1) return 1;      // as: promovat la 11 in calculeazaScor
	if (v > 10) return 10;     // J, Q, K
	return v;
}

int calculeazaScor(Lista& mana)
{
	int s = 0;   // suma cartilor non-as (plus asii numarati ca 1 la final)
	int asi = 0;

	for (Nod* p = mana.getFirst(); p; p = p->getNext())
	{
		Carte c = p->getInfo();
		if (c.getValoare() == 1)
			asi++;
		else
			s += valoareBlackjack(c);
	}

	// Fiecare as vrea sa conteze 11 daca incape, altfel 1.
	while (asi != 0)
	{
		if (s + 11 <= 21)
		{
			s += 11;
			asi--;
		}
		else
		{
			s += asi; // restul asilor conteaza 1 fiecare
			asi = 0;
		}
	}

	return s;
}

bool esteManaSoft(Lista& mana)
{
	int s = 0;
	int asi = 0;

	for (Nod* p = mana.getFirst(); p; p = p->getNext())
	{
		Carte c = p->getInfo();
		if (c.getValoare() == 1)
			asi++;
		else
			s += valoareBlackjack(c);
	}

	// Mana e soft daca exista cel putin un as care poate conta 11 fara bust.
	return asi > 0 && (s + asi - 1 + 11 <= 21);
}

bool dealerTrebuieSaTraga(int scorDealer)
{
	return scorDealer < 17;
}

bool esteBlackjackNatural(Lista& mana)
{
	int n = 0;
	for (Nod* p = mana.getFirst(); p; p = p->getNext())
		n++;
	return n == 2 && calculeazaScor(mana) == 21;
}

Rezultat determinaRezultat(int scorJucator, int scorDealer)
{
	if (scorJucator > 21) return Rezultat::JucatorBust;
	if (scorDealer > 21)  return Rezultat::DealerBust;
	if (scorDealer > scorJucator) return Rezultat::DealerCastiga;
	if (scorDealer < scorJucator) return Rezultat::JucatorCastiga;
	return Rezultat::Egalitate;
}
