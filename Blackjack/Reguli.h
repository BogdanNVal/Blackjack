#pragma once
#include "Carte.h"
#include "Lista.h"

enum class Rezultat
{
	JucatorBust,
	DealerBust,
	JucatorCastiga,
	DealerCastiga,
	Egalitate
};

int valoareBlackjack(const Carte& c);
int calculeazaScor(Lista& mana);
bool esteManaSoft(Lista& mana);
bool dealerTrebuieSaTraga(int scorDealer);

// Natural blackjack: exactly two cards totaling 21 (ace + 10-value). Pays 3:2.
bool esteBlackjackNatural(Lista& mana);

Rezultat determinaRezultat(int scorJucator, int scorDealer);
