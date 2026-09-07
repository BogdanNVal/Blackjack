#pragma once
#include "Carte.h"

// Sfat de "basic strategy" pentru Blackjack (dealerul sta pe soft 17, fara
// split si fara surrender, in acord cu regulile implementate in joc).

enum class Actiune
{
	Hit,
	Stand,
	Double
};

// Actiunea recomandata pentru mana curenta.
//  - scorJucator: scorul optim curent al jucatorului
//  - esteSoft:    daca mana contine un as numarat ca 11
//  - cartaDealer: cartea vizibila a dealerului
//  - poateDubla:  daca dublarea e permisa (2 carti si bani suficienti)
Actiune sfatBaza(int scorJucator, bool esteSoft, Carte cartaDealer, bool poateDubla);

// Eticheta lizibila pentru afisare ("Hit" / "Stand" / "Double").
const char* numeActiune(Actiune a);
