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

	// Simbolurile grafice ale cartilor (Trefla, Inima, Pica, Romb) sunt
	// caractere Unicode reale, scrise ca octeti UTF-8 (\xE2\x99\x..) ca sa
	// nu depinda de encoding-ul fisierului sursa. Merg corect in orice
	// consola (cmd clasic, Windows Terminal), indiferent de fontul folosit,
	// atata timp cat codepagina de iesire e setata pe UTF-8 (vezi Source.cpp).
	if (carte.simbol == 0) out << "\xE2\x99\xA3";   // Trefla  (U+2663)
	if (carte.simbol == 1) out << "\xE2\x99\xA5";   // Inima   (U+2665)
	if (carte.simbol == 2) out << "\xE2\x99\xA0";   // Pica    (U+2660)
	if (carte.simbol == 3) out << "\xE2\x99\xA6";   // Romb    (U+2666)
	out << "  ";

	return out;


}