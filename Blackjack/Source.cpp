#include "Joc.h"
#include "Lista.h"
#include "Carte.h"

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

int main()
{
#ifdef _WIN32
	// UTF-8 code page so card suit symbols (♣♥♠♦) display correctly.
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
#endif

	meniuPrincipal();
	return 0;
}
