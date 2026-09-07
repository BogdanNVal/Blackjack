#include <catch2/catch_test_macros.hpp>

#include <set>
#include <utility>

#include "Carte.h"
#include "Pachet.h"

TEST_CASE("Un pachet nou are 52 de carti unice", "[pachet]")
{
	Pachet p;
	p.creeaza();

	REQUIRE_FALSE(p.gol());

	std::set<std::pair<int, int>> vazute;
	int trase = 0;
	while (!p.gol())
	{
		Carte c = p.trage();
		vazute.insert({ c.getValoare(), c.getSimbol() });
		trase++;
	}

	REQUIRE(trase == 52);
	REQUIRE(vazute.size() == 52); // all distinct (4 suits x 13 ranks)
}

TEST_CASE("Pachetul gol arunca la tragere", "[pachet]")
{
	Pachet p;
	p.creeaza();
	for (int i = 0; i < 52; i++)
		(void)p.trage();

	REQUIRE(p.gol());
	REQUIRE_THROWS(p.trage());
}

TEST_CASE("Amestecarea pastreaza cele 52 de carti", "[pachet]")
{
	Pachet p;
	p.creeaza();
	p.amesteca();

	std::set<std::pair<int, int>> vazute;
	while (!p.gol())
	{
		Carte c = p.trage();
		vazute.insert({ c.getValoare(), c.getSimbol() });
	}

	REQUIRE(vazute.size() == 52);
}
