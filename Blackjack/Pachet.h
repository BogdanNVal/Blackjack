#pragma once
#include "Carte.h"

class Pachet
{
	Carte carti[52];

public:
	Pachet();

	void creeaza();
	void amesteca();
	Carte trage();
	bool gol() const;
};
