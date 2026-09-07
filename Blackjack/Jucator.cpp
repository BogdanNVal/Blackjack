#include "Jucator.h"
#include "Reguli.h"
#include <cstring>
#include <iomanip>

using std::cout;
using std::endl;

Jucator::Jucator(const char* nume,int banii,int suma_pariata) {
	this->nume = new char[strlen(nume) + 1];
	strcpy(this->nume, nume);
	this->banii = banii;
	this->suma_pariata = suma_pariata;
	this->scor = 0;
}

Jucator::Jucator(const Jucator& a)
	: carti(a.carti)
{
	nume = new char[strlen(a.nume) + 1];
	strcpy(nume, a.nume);
	banii = a.banii;
	suma_pariata = a.suma_pariata;
	scor = a.scor;
}

Jucator& Jucator::operator=(const Jucator& a)
{
	if (this == &a) return *this;

	char* nou = new char[strlen(a.nume) + 1];
	strcpy(nou, a.nume);
	delete[] nume;
	nume = nou;

	banii = a.banii;
	suma_pariata = a.suma_pariata;
	scor = a.scor;
	carti = a.carti;

	return *this;
}

Jucator::~Jucator()
{
	if (nume) delete[] nume;
}

void Jucator::setBanii(int banii)
{
	this->banii = banii;

}
int Jucator::getBanii()
{
	return banii;
}

const char* Jucator::getNume()
{
	return nume;
}

int Jucator::getSuma_pariata()
{
	return suma_pariata;
}

 Lista& Jucator::getCarti() {
	
	 
	 return carti;

}

 void Jucator::setScor(int a)
 {
	 scor = a;


 }

 int Jucator::getScor()
 {
	 return scor;
 }


 

 void Jucator::setCarti(Lista& a) {
	 
	 
	 carti = a;

	 
 }

void Jucator::Bet(int a)
{
	if (a <= banii)
	{
		suma_pariata = a;
		banii = banii - a;
	}
	else
	{
		cout << "Nu aveti suficenti banii\nAlege alta suma";
	}

}

void Jucator::primeste(Carte c)
{
	carti.addElement(c);
	verifica_carti();
}

bool Jucator::dubleazaMiza()
{
	if (suma_pariata <= banii)
	{
		banii -= suma_pariata;
		suma_pariata += suma_pariata;
		return true;
	}
	return false;
}

void Jucator::verifica_carti()
{
	// Logica de scor traieste acum in Reguli (o singura sursa de adevar,
	// testata unitar).
	scor = calculeazaScor(carti);
}

std::ostream& operator<<(std::ostream& out, const Jucator& jucator) {

	out << "Banii: " << jucator.banii << endl << endl << jucator.nume;
	return out;
}

std::istream& operator>>(std::istream& in, Jucator& jucator) {
	char buffer[50];
	in >> std::setw(50) >> buffer;

	delete[] jucator.nume;
	jucator.nume = new char[strlen(buffer) + 1];
	strcpy(jucator.nume, buffer);

	return in;
}



