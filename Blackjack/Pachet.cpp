#include "Pachet.h"
#include <stdexcept>
#include <random>

Pachet::Pachet()
{
	creeaza();
}

void Pachet::creeaza()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 1; j <= 13; j++)
		{
			carti[13 * i + j - 1].setValoare(j);
			carti[13 * i + j - 1].setSimbol(i);
		}
	}
}

void Pachet::amesteca()
{
	// Un singur generator, semanat o data din random_device: doua rulari
	// pornite in aceeasi secunda nu vor produce acelasi amestec (spre
	// deosebire de rand()/srand(time(0))).
	static std::random_device rd;
	static std::mt19937 gen(rd());

	for (int i = 0; i < 52; i++)
	{
		std::uniform_int_distribution<int> dist(i, 51);
		int r = dist(gen);

		Carte temp = carti[i];
		carti[i] = carti[r];
		carti[r] = temp;
	}
}

Carte Pachet::trage()
{
	for (int i = 0; i < 52; i++)
	{
		if (carti[i].getValoare() != 0)
		{
			Carte temp = carti[i];
			carti[i].setValoare(0);
			return temp;
		}
	}

	throw std::runtime_error("Pachetul este gol, nu mai sunt carti de tras.");
}

bool Pachet::gol() const
{
	for (int i = 0; i < 52; i++)
		if (carti[i].getValoare() != 0)
			return false;
	return true;
}
