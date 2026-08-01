#include "Pachet.h"
#include <stdexcept>
#include <random>

using std::cout;

Carte Pachet[52];


void afisare_pachet() {
	for (int i = 0; i < 52; i++)
		cout << "Cartea " << i + 1 << "= " << Pachet[i];
}

void creare_pachet() {



	for (int i = 0; i < 4; i++)
	{
		for (int j = 1; j <= 13; j++)
		{
			Pachet[13 * i + j - 1].setValoare(j);
			Pachet[13 * i + j - 1].setSimbol(i);


		}

	}
	//afisare_pachet();
	//cout << endl;




}



void amestecare_pachet()
{
	static std::random_device rd;
	static std::mt19937 gen(rd());

	for (int i = 0; i < 52; i++)
	{
		std::uniform_int_distribution<int> dist(i, 51);
		int r = dist(gen);

		Carte temp = Pachet[i];
		Pachet[i] = Pachet[r];
		Pachet[r] = temp;
	}

	//afisare_pachet();

}


Carte trage_carte() {
	for (int i = 0; i < 52; i++)
	{
		if (Pachet[i].getValoare() != 0)
		{
			Carte temp = Pachet[i];
			Pachet[i].setValoare(0);

			return temp;
		}
	}

	throw std::runtime_error("Pachetul este gol, nu mai sunt carti de tras.");
}

bool pachet_gol() {
	for (int i = 0; i < 52; i++)
		if (Pachet[i].getValoare() != 0)
			return false;
	return true;
}