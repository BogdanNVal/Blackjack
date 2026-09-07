#pragma once
#include "Carte.h"

// Pachetul de 52 de carti. Inlocuieste vechiul array global `Carte Pachet[52]`
// cu o instanta detinuta de bucla de joc, ca sa nu mai existe stare globala
// mutabila (mai usor de testat si de rationat).
class Pachet
{
	Carte carti[52];

public:
	Pachet();

	// (Re)creeaza cele 52 de carti (4 simboluri x 13 valori), nefolosite.
	void creeaza();

	// Amesteca pachetul folosind un generator Mersenne Twister.
	void amesteca();

	// Trage urmatoarea carte nefolosita. Arunca std::runtime_error daca
	// pachetul e gol.
	Carte trage();

	// true daca nu mai exista carti de tras.
	bool gol() const;
};
