#pragma once
#include <stdlib.h>
#include <iomanip>
#include "Jucator.h"

void meniuPrincipal();
void ruleazaJoc(Jucator& jucator);

void start(Jucator& jucator);
void runda(Jucator& jucator);
void runda_dealer(Jucator& jucator);
void verificare(Jucator& jucator);

void win(Jucator& jucator);
void draw(Jucator& jucator);

void alegere(Jucator& jucator,int nr);
