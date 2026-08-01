#include "Salvare.h"
#include <fstream>
#include <map>
#include <string>
#include <iostream>

using std::cout;
using std::string;
using std::map;
using std::ifstream;
using std::ofstream;

static const char* FISIER_SALVARE = "jucatori.txt";

// Citeste toti jucatorii salvati intr-un map <nume, banii>.
// Daca fisierul nu exista inca (primul joc), intoarce pur si simplu un map gol.
static map<string, int> citesteToti()
{
	map<string, int> jucatori;
	ifstream fin(FISIER_SALVARE);

	string nume;
	int banii;
	while (fin >> nume >> banii)
		jucatori[nume] = banii;

	return jucatori;
}

static void scrieToti(const map<string, int>& jucatori)
{
	ofstream fout(FISIER_SALVARE, std::ios::trunc);
	for (const auto& p : jucatori)
		fout << p.first << " " << p.second << "\n";
}

void salveazaJucator(Jucator& jucator)
{
	map<string, int> jucatori = citesteToti();
	jucatori[jucator.getNume()] = jucator.getBanii();
	scrieToti(jucatori);
}

bool incarcaJucator(const char* nume, Jucator& jucator)
{
	map<string, int> jucatori = citesteToti();

	auto it = jucatori.find(nume);
	if (it == jucatori.end())
		return false;

	jucator = Jucator(nume, it->second, 0);
	return true;
}

void afiseazaJucatoriSalvati()
{
	map<string, int> jucatori = citesteToti();

	if (jucatori.empty())
	{
		cout << "Nu exista niciun jucator salvat inca.\n";
		return;
	}

	cout << "Jucatori salvati:\n";
	for (const auto& p : jucatori)
		cout << " - " << p.first << " (Banii: " << p.second << ")\n";
}

bool existaJucatoriSalvati()
{
	return !citesteToti().empty();
}
