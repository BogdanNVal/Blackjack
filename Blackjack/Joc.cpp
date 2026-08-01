#include "Joc.h"
#include "Salvare.h"
#include <sstream>
#include <string>
#include <thread>
#include <chrono>
#include <functional>

#ifdef _WIN32
#include <windows.h>
#endif

using std::cout;
using std::cin;
using std::endl;
using std::setw;
using std::string;

Jucator dealer("Dealer", 0);

static const int LATIME_MANA = 45;
static const int PAUZA_CARTE_MS = 600;   // pauza intre cartile date, pt. efect de "animatie"

// Opreste executia pentru un scurt timp, ca sa dea senzatia ca dealerul
// (sau jucatorul) trage cartile una cate una, nu toate deodata.
static void asteapta(int ms)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

// --- Culori consola (Windows) ---------------------------------------------
// Pe alte sisteme (ex. testare pe Linux), functiile nu fac nimic vizibil,
// dar codul tot compileaza si ruleaza normal.
#ifdef _WIN32
static const int CULOARE_IMPLICITA = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
static const int CULOARE_ROSU = FOREGROUND_RED | FOREGROUND_INTENSITY;
static const int CULOARE_VERDE = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
static const int CULOARE_GALBEN = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
static const int CULOARE_GRI = FOREGROUND_INTENSITY;
#else
static const int CULOARE_IMPLICITA = 0, CULOARE_ROSU = 0, CULOARE_VERDE = 0, CULOARE_GALBEN = 0, CULOARE_GRI = 0;
#endif

static void culoare(int cod)
{
#ifdef _WIN32
	static HANDLE hConsola = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsola, (WORD)cod);
#else
	(void)cod;
#endif
}

// Afiseaza un mesaj intr-o culoare, apoi revine la culoarea implicita.
static void afiseazaColorat(const string& text, int cod)
{
	culoare(cod);
	cout << text;
	culoare(CULOARE_IMPLICITA);
}

// O carte rosie (Inima/Diamant) se afiseaza cu rosu, restul cu culoarea implicita.
static void afiseazaCarteColorata(Carte c)
{
	bool rosu = (c.getSimbol() == 1 || c.getSimbol() == 3);
	culoare(rosu ? CULOARE_ROSU : CULOARE_IMPLICITA);
	cout << c;
	culoare(CULOARE_IMPLICITA);
}

// Afiseaza toate cartile dintr-o mana, fiecare in culoarea ei.
static void afiseazaManaColorata(Lista& carti)
{
	Nod* p = carti.getFirst();
	while (p)
	{
		afiseazaCarteColorata(p->getInfo());
		p = p->getNext();
	}
}
// ----------------------------------------------------------------------------

// Reda o mana (Lista de carti) ca string, ca sa poata fi aliniata pe ecran.
static string cartiToString(Lista& carti)
{
	std::ostringstream oss;
	oss << carti;
	return oss.str();
}

// Valoarea de Blackjack a unei singure carti vizibile (asul conteaza 11
// cand e singura carte cunoscuta, fara riscul de a depasi 21).
static int scorCartePartiala(Carte c)
{
	int v = c.getValoare();
	if (v == 1) return 11;
	if (v > 10) return 10;
	return v;
}

// Numara caracterele dintr-un string UTF-8 (nu octetii), ca sa ramana
// corecta alinierea pe coloane cand textul contine simboluri Unicode
// (trefla/inima/pica/romb), care ocupa mai multi octeti dar un singur
// caracter afisat pe ecran.
static int lungimeUtf8(const string& s)
{
	int n = 0;
	for (unsigned char c : s)
		if ((c & 0xC0) != 0x80) // sare peste octetii de continuare UTF-8
			n++;
	return n;
}

// Afiseaza o mana (eticheta + carti colorate) cu scorul aliniat in partea
// dreapta. 'textPtruLungime' e versiunea simpla (necolorata) a cartilor,
// folosita doar ca sa se calculeze corect padding-ul; 'printCarti' e ce
// se afiseaza efectiv (poate fi colorat).
static void afiseazaMana(const string& eticheta, const string& textPtruLungime,
	const std::function<void()>& printCarti, const string& scor)
{
	int lungime = lungimeUtf8(eticheta) + lungimeUtf8(textPtruLungime);

	cout << eticheta;
	printCarti();

	int padding = LATIME_MANA - lungime;
	cout << string(padding > 0 ? padding : 1, ' ');
	cout << "Scor: " << scor << endl;
}

// Afiseaza banii jucatorului si ambele maini (jucator + dealer).
// Daca dealerVizibilComplet e false, a doua carte a dealerului ramane
// ascunsa si se arata doar scorul partial calculat din cartea vizibila.
// Functioneaza corect si in timpul animatiei de impartire, cand dealerul
// inca nu are nicio carte sau are doar prima.
static void afiseazaStare(Jucator& jucator, bool dealerVizibilComplet)
{
	cout << "Banii: " << jucator.getBanii() << endl << endl;

	afiseazaMana(string(jucator.getNume()) + ": ", cartiToString(jucator.getCarti()),
		[&]() { afiseazaManaColorata(jucator.getCarti()); },
		jucator.getCarti().getFirst() ? std::to_string(jucator.getScor()) : string("-"));

	Nod* p = dealer.getCarti().getFirst();

	if (dealerVizibilComplet)
	{
		afiseazaMana("Dealer: ", cartiToString(dealer.getCarti()),
			[&]() { afiseazaManaColorata(dealer.getCarti()); },
			std::to_string(dealer.getScor()));
	}
	else if (!p)
	{
		// dealerul inca nu a primit nicio carte (animatie de impartire in curs)
		afiseazaMana("Dealer: ", "", []() {}, "-");
	}
	else if (!p->getNext())
	{
		// dealerul are doar prima carte (vizibila); a doua nu a fost inca data
		std::ostringstream carti;
		carti << p->getInfo();
		Carte prima = p->getInfo();
		afiseazaMana("Dealer: ", carti.str(),
			[=]() { afiseazaCarteColorata(prima); },
			std::to_string(scorCartePartiala(prima)) + "+?");
	}
	else
	{
		std::ostringstream carti;
		carti << p->getInfo() << "?? ";
		Carte prima = p->getInfo();
		afiseazaMana("Dealer: ", carti.str(),
			[=]() { afiseazaCarteColorata(prima); afiseazaColorat("?? ", CULOARE_GRI); },
			std::to_string(scorCartePartiala(prima)) + "+?");
	}

	cout << endl;
}


void meniuPrincipal()
{
	int alegere;
	do
	{
		cout << "=== BLACKJACK ===\n";
		cout << "[1] Jucator nou\n";
		cout << "[2] Continua cu un jucator existent\n";
		cout << "[3] Iesire\n";
		cout << "Alegere: ";
		cin >> alegere;
	} while (alegere != 1 && alegere != 2 && alegere != 3);

	system("CLS");

	if (alegere == 1)
	{
		char nume[50];
		cout << "Nume jucator nou: ";
		cin >> setw(50) >> nume;

		Jucator jucator(nume);
		system("CLS");
		ruleazaJoc(jucator);
	}
	else if (alegere == 2)
	{
		if (!existaJucatoriSalvati())
		{
			cout << "Nu exista niciun jucator salvat inca.\n\n";
			system("pause");
			system("CLS");
			meniuPrincipal();
			return;
		}

		afiseazaJucatoriSalvati();
		char nume[50];
		cout << "\nNume jucator: ";
		cin >> setw(50) >> nume;

		Jucator jucator;
		if (incarcaJucator(nume, jucator))
		{
			system("CLS");
			ruleazaJoc(jucator);
		}
		else
		{
			cout << "\nNu am gasit un jucator cu acest nume.\n\n";
			system("pause");
			system("CLS");
			meniuPrincipal();
		}
	}
	// alegere == 3: iesire, main() se termina
}

void ruleazaJoc(Jucator& jucator)
{
	bool continua = true;

	while (continua)
	{
		if (jucator.getBanii() <= 0)
		{
			cout << "Nu mai aveti bani. Jocul se incheie pentru " << jucator.getNume() << ".\n";
			break;
		}

		jucator.getCarti().clear();
		dealer.getCarti().clear();
		jucator.setScor(0);
		dealer.setScor(0);

		start(jucator);

		cout << "\nBanii tai: " << jucator.getBanii() << "\n";
		cout << "Mai joci o runda? [1] Da, [2] Nu: ";
		int r;
		cin >> r;
		continua = (r == 1);
		system("CLS");
	}

	salveazaJucator(jucator);
	cout << "Progresul lui " << jucator.getNume() << " a fost salvat (Banii: " << jucator.getBanii() << ").\n";
	cout << "La revedere!\n";
}

void start(Jucator& jucator)
{
	
	creare_pachet();
	amestecare_pachet();
	cout << jucator;
	cout << endl; cout << endl;
	cout << "Ce suma doriti sa pariati: ";
	int s;
	cin >> s;
	while (s <= 0 || s > jucator.getBanii())
	{
		cout << "Suma invalida. Alege alta suma: ";
		cin >> s;
	}
	jucator.Bet(s);
	system("CLS");

	// Se trag cartile pe rand (jucator, dealer, jucator, dealer), cu o mica
	// pauza dupa fiecare, ca sa para ca sunt date una cate una, nu instant.
	jucator.getCarti().addElement(trage_carte());
	jucator.verifica_carti();
	system("CLS");
	afiseazaStare(jucator, false);
	asteapta(PAUZA_CARTE_MS);

	dealer.getCarti().addElement(trage_carte());
	dealer.verifica_carti();
	system("CLS");
	afiseazaStare(jucator, false);
	asteapta(PAUZA_CARTE_MS);

	jucator.getCarti().addElement(trage_carte());
	jucator.verifica_carti();
	system("CLS");
	afiseazaStare(jucator, false);
	asteapta(PAUZA_CARTE_MS);

	dealer.getCarti().addElement(trage_carte());
	dealer.verifica_carti();
	system("CLS");
	afiseazaStare(jucator, false);

	system("pause");
	
	verificare(jucator);
}

void runda(Jucator& jucator)
{   
	system("CLS");
	afiseazaStare(jucator, false);

	cout << "[1] Hit, [2] Stand, [3] Double\n ";
	int a = jucator.Alegere();
	alegere(jucator, a);
}

void runda_dealer(Jucator& jucator)
{
	if (jucator.getScor() > 21)
	{
		system("CLS");
		afiseazaStare(jucator, true);
		afiseazaColorat("Ati pierdut (Bust)\n", CULOARE_ROSU);
		return;
	}

	// Dealerul isi arata mai intai cartea ascunsa, cu o pauza scurta
	system("CLS");
	afiseazaStare(jucator, true);
	asteapta(PAUZA_CARTE_MS);

	while(dealer.getScor()<17)
	{
		dealer.getCarti().addElement(trage_carte());
		dealer.verifica_carti();
		
		system("CLS");
		afiseazaStare(jucator, true);
		asteapta(PAUZA_CARTE_MS);
	}

	if (dealer.getScor() > 21)
	{
		afiseazaColorat("Dealer Bust! Ati castigat!\n", CULOARE_VERDE);
		win(jucator);
	}
	else if (dealer.getScor() > jucator.getScor())
	{
		afiseazaColorat("Dealer castiga.\n", CULOARE_ROSU);
	}
	else if (dealer.getScor() < jucator.getScor())
	{
		afiseazaColorat("Ati castigat!\n", CULOARE_VERDE);
		win(jucator);
	}
	else
	{
		afiseazaColorat("Push! (egalitate)\n", CULOARE_GALBEN);
		draw(jucator);
	}

}


void verificare(Jucator& jucator)
{
	
	if (jucator.getScor() > 21)
	{
		afiseazaColorat("Bust\n", CULOARE_ROSU);
		runda_dealer(jucator);
	}
	if (jucator.getScor() == 21)
	{
		afiseazaColorat("Blackjack\n", CULOARE_VERDE);
		runda_dealer(jucator);
	}
	if (jucator.getScor() < 21)
	{
		runda(jucator);
	}
	

}


void alegere(Jucator& jucator,int nr)
{
	if (nr == 1)
	{
		jucator.Hit();
		cout << "Trageti o carte...\n";
		asteapta(PAUZA_CARTE_MS);
		verificare(jucator);
	}
	else if (nr == 2)
	{
		jucator.Stand();
		runda_dealer(jucator);
	}
	else if (nr == 3)
	{
		jucator.Double(jucator.getSuma_pariata());
		cout << "Trageti o carte...\n";
		asteapta(PAUZA_CARTE_MS);
		runda_dealer(jucator);
	}

}


void win(Jucator& jucator)
{

	
	jucator.setBanii(jucator.getBanii() + 2 * jucator.getSuma_pariata());

}

void draw(Jucator& jucator)
{
	jucator.setBanii(jucator.getBanii() + jucator.getSuma_pariata());
}