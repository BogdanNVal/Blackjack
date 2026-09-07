#pragma once
#include <stdlib.h>
#include <iomanip>
#include "Jucator.h"
#include "Pachet.h"

void meniuPrincipal();
void ruleazaJoc(Jucator& jucator);

void start(Jucator& jucator, Pachet& pachet, Jucator& dealer);
void runda(Jucator& jucator, Pachet& pachet, Jucator& dealer);
void runda_dealer(Jucator& jucator, Pachet& pachet, Jucator& dealer);
void verificare(Jucator& jucator, Pachet& pachet, Jucator& dealer);

void win(Jucator& jucator);
void draw(Jucator& jucator);

void alegere(Jucator& jucator, int nr, Pachet& pachet, Jucator& dealer);
