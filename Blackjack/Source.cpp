#include "Joc.h"
#include "Lista.h"
#include "Carte.h"

#ifdef _WIN32
#include <windows.h>
#endif

int main()
{
#ifdef _WIN32
	// Codepagina UTF-8 (65001) permite afisarea simbolurilor Unicode ale
	// cartilor de joc (trefla, inima, pica, romb) folosite in Carte.cpp,
	// indiferent de fontul consolei (merge si in Windows Terminal).
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
#endif

	meniuPrincipal();
	return 0;
}
