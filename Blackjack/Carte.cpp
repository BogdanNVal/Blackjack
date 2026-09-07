#include "Carte.h"

Carte::Carte(int valoare, int simbol)
{
	this->valoare = valoare;
	this->simbol = simbol;

}
Carte::Carte(const Carte& a)
{
	this->valoare = a.valoare;
	this->simbol = a.simbol;
}
int Carte::getValoare()
{
	return valoare;
}

int Carte::getSimbol()
{
	return simbol;
}

void Carte::setValoare(int valoare)
{
	this->valoare = valoare;
}

void Carte::setSimbol(int simbol)
{
	this->simbol = simbol;
}

Carte& Carte::operator=(const Carte& a)
{
	valoare = a.valoare;
	simbol = a.simbol;
	return *this;
}


std::ostream& operator<<(std::ostream& out, const Carte& carte)
{
	if (carte.valoare == 1) out << "A" << " ";
	else if (carte.valoare == 11) out << "J" << " ";
	else if (carte.valoare == 12) out << "Q" << " ";
	else if (carte.valoare == 13) out << "K" << " ";
	else out << carte.valoare << " ";

	// Suit symbols as raw UTF-8 bytes so they do not depend on the source file encoding.
	if (carte.simbol == 0) out << "\xE2\x99\xA3";   // Clubs  (U+2663)
	if (carte.simbol == 1) out << "\xE2\x99\xA5";   // Hearts (U+2665)
	if (carte.simbol == 2) out << "\xE2\x99\xA0";   // Spades (U+2660)
	if (carte.simbol == 3) out << "\xE2\x99\xA6";   // Diamonds (U+2666)
	out << "  ";

	return out;


}
