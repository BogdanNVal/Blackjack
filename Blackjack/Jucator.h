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

	void primeste(Carte c);
	bool dubleazaMiza();

	void verifica_carti();

	friend std::ostream& operator<<(std::ostream& out, const Jucator&);
	friend std::istream& operator>>(std::istream& in, Jucator&);

	friend class Lista;


};
