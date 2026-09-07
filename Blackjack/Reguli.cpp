#include "Reguli.h"

int valoareBlackjack(const Carte& c)
{
	int v = c.getValoare();
	if (v == 1) return 1;
	if (v > 10) return 10;
	return v;
}

int calculeazaScor(Lista& mana)
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

	// Count aces as 1 first, then promote to 11 while the hand stays <= 21.
	// (The old loop could promote one ace and then bust by adding the rest as 1s,
	// e.g. A,A,10 -> 22 instead of 12.)
	s += asi;
	while (asi > 0 && s + 10 <= 21)
	{
		s += 10;
		asi--;
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

	// Soft = at least one ace can be counted as 11 in the optimal total.
	return asi > 0 && (s + asi + 10 <= 21);
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
