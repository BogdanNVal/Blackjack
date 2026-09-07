#pragma once
#include <iostream>

class Carte
{
	int valoare;
	int simbol;


public:
	Carte(int valoare=0,int simbol=0);
	Carte(const Carte&);
	int getValoare() const;
	int getSimbol() const;
	void setValoare(int valoare);
	void setSimbol(int valoare);

	Carte& operator=(const Carte&);
	friend std::ostream& operator<<(std::ostream& out, const Carte&);
};






