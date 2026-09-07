#pragma once
#include "Carte.h"

enum class Actiune
{
	Hit,
	Stand,
	Double
};

Actiune sfatBaza(int scorJucator, bool esteSoft, Carte cartaDealer, bool poateDubla);
const char* numeActiune(Actiune a);
