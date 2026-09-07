#pragma once
#include "Lista.h"
#pragma warning(disable:4996)


class Jucator
{
private:
	char* nume;
	int banii;
	int suma_pariata;
	int scor;

protected:
	Lista carti;

public:
	Jucator(const char* nume = "",int banii=10000,int suma_pariata=0);
	Jucator(const Jucator&);
	Jucator& operator=(const Jucator&);
	~Jucator();
	
	void setBanii(int banii);
	int getBanii();
	const char* getNume();
	int getSuma_pariata();
	void Bet(int a);
	Lista& getCarti();
	void setCarti(Lista& a);
	int getScor();
	void setScor(int a);

	// Primeste o carte (trasa din pachet de bucla de joc) si recalculeaza scorul.
	void primeste(Carte c);

	// Dubleaza miza curenta daca sunt bani suficienti. Intoarce true la succes.
	bool dubleazaMiza();

	void verifica_carti();

	friend std::ostream& operator<<(std::ostream& out, const Jucator&);
	friend std::istream& operator>>(std::istream& in, Jucator&);

	friend class Lista;


};
