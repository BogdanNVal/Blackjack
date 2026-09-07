#pragma once
#include "Carte.h"
#include "Lista.h"

// Reguli pure de Blackjack, fara I/O de consola, ca sa poata fi testate unitar
// si reutilizate (ex. de sfatul de strategie).

// Rezultatul unei runde, din perspectiva jucatorului.
enum class Rezultat
{
	JucatorBust,
	DealerBust,
	JucatorCastiga,
	DealerCastiga,
	Egalitate
};

// Valoarea de Blackjack a unei carti: asul = 1 (promovarea la 11 se face in
// calculeazaScor), J/Q/K = 10, restul valoarea nominala.
int valoareBlackjack(const Carte& c);

// Scorul optim al unei maini: asii conteaza 11 cat timp nu se depaseste 21,
// altfel 1.
int calculeazaScor(Lista& mana);

// true daca mana e "soft" (contine un as numarat ca 11).
bool esteManaSoft(Lista& mana);

// Dealerul mai trage cat timp scorul lui e sub 17.
bool dealerTrebuieSaTraga(int scorDealer);

// true daca mana e un "blackjack natural": exact doua carti care fac 21
// (as + carte de 10). Se plateste 3:2, spre deosebire de un 21 obtinut din
// trei sau mai multe carti.
bool esteBlackjackNatural(Lista& mana);

// Determina rezultatul comparand scorurile finale.
Rezultat determinaRezultat(int scorJucator, int scorDealer);
