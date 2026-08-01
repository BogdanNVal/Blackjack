#include "Jucator.h"
#include <cstring>
#include <iomanip>

using std::cout;
using std::cin;
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

void Jucator::Stand() {
	
	
}
void Jucator::Hit(){
	
	carti.addElement(trage_carte());
	verifica_carti();

}

void Jucator::Double(int a) {

	if (a <= banii)
	{
		banii -= a;
		suma_pariata += a;
		carti.addElement(trage_carte());
		verifica_carti();
	}
	else
	{
		cout << "Nu aveti suficienti bani pentru Double.\n";
	}

}

int  Jucator::Alegere(){
	
	int nr;
	cin >> nr;
	
	if (nr != 1 && nr != 2 && nr != 3)
	{
		cout << "Nu ati introdus o varianta corecta\n ";
		cout << "[1] Hit, [2] Stand, [3] Double ";
		while (nr != 1 && nr != 2 && nr != 3)
		{
			cin >> nr;

		}
	}
	return nr;
	
}

void Jucator::verifica_carti()
{
	Lista a = getCarti();
	Nod* p = a.getFirst();
	int s = 0, s1 = 0;
	while (p)
	{
		Carte c(p->getInfo());
		if (c.getValoare() == 1)
			s1++;
		else if (c.getValoare() > 10)
			s = s + 10;
		else
			s = s + c.getValoare();
		p = p->getNext();
	}
	while (s1 != 0)
	{
		if (s + 11 <= 21)
		{
			s = s + 11;
			s1--;
		}
		else
		{
			s = s + s1;
			s1 = 0;
		}
		
	}
		scor = s;
	


		
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



