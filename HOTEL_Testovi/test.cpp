#include "pch.h"
#include "Baza.h"

TEST(BazaTest, DodajSobuUvecajBrojSoba) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	EXPECT_EQ(testBaza.BrojSoba(), 0);
	testBaza.DodajSobu(1, 3, 90);
	EXPECT_EQ(testBaza.BrojSoba(), 1);
}
TEST(BazaTest, DodajIstuSobuProvjeriBrojSoba) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	EXPECT_EQ(testBaza.BrojSoba(), 0);
	testBaza.DodajSobu(1, 3, 90);
	EXPECT_EQ(testBaza.BrojSoba(), 1);
	testBaza.DodajSobu(1, 4, 120);
	EXPECT_EQ(testBaza.BrojSoba(), 1);
}
TEST(BazaTest, DodajGostaProvjeriBrojGostiju) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	EXPECT_EQ(testBaza.BrojGostiju(), 0);
	testBaza.DodajSobu(1, 3, 90);
	testBaza.DodajGostaIZakaziRezervaciju("Gost Gostic", "123456", "E-mail", 1, "2026-08-10", "2026-08-15");
	EXPECT_EQ(testBaza.BrojGostiju(), 1);
}
TEST(BazaTest, DodajIstogGostaProvjeriBrojGostiju) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	EXPECT_EQ(testBaza.BrojGostiju(), 0);
	testBaza.DodajSobu(1, 3, 90);
	testBaza.DodajGostaIZakaziRezervaciju("Gost Gostic", "123456", "E-mail", 1, "2026-08-10", "2026-08-15");
	EXPECT_EQ(testBaza.BrojGostiju(), 1);
	testBaza.DodajSobu(2, 4, 120);
	testBaza.DodajGostaIZakaziRezervaciju("Gost1", "123456", "E-mail", 2, "2026-07-21", "2026-07-29");
	EXPECT_EQ(testBaza.BrojGostiju(), 1);
}
TEST(BazaTest, DodajRezervacijuProvjeriBrojRezervacija) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	EXPECT_EQ(testBaza.BrojRezervacija(), 0);
	testBaza.DodajSobu(1, 3, 90);
	testBaza.DodajGostaIZakaziRezervaciju("Gost Gostic", "123456", "E-mail", 1, "2026-08-10", "2026-08-15");
	EXPECT_EQ(testBaza.BrojRezervacija(), 1);
}
TEST(BazaTest, DodajIstuRezervacijuProvjeriBrojRezervacija) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	EXPECT_EQ(testBaza.BrojRezervacija(), 0);
	testBaza.DodajSobu(1, 3, 90);
	testBaza.DodajGostaIZakaziRezervaciju("Gost Gostic", "123456", "E-mail", 1, "2026-08-10", "2026-08-15");
	EXPECT_EQ(testBaza.BrojRezervacija(), 1);
	testBaza.DodajGostaIZakaziRezervaciju("Gost1", "654321", "E-mail@", 1, "2026-08-10", "2026-08-15");
	EXPECT_EQ(testBaza.BrojRezervacija(), 1);
}
TEST(BazaTest, DodajRacunProvjeraBrojaRacuna) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	EXPECT_EQ(testBaza.BrojRacuna(), 0);
	testBaza.DodajSobu(1, 3, 90);
	testBaza.DodajGostaIZakaziRezervaciju("Gost Gostic", "123456", "E-mail", 1, "2026-08-10", "2026-08-15");
	testBaza.KreirajRacun(1, "2026-08-15", "kartica");
	EXPECT_EQ(testBaza.BrojRacuna(), 1);
}
TEST(BazaTest, DodajDupliRacunProvjeraBrojaRacuna) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	EXPECT_EQ(testBaza.BrojRacuna(), 0);
	testBaza.DodajSobu(1, 3, 90);
	testBaza.DodajGostaIZakaziRezervaciju("Gost Gostic", "123456", "E-mail", 1, "2026-08-10", "2026-08-15");
	testBaza.KreirajRacun(1, "2026-08-15", "kartica");
	EXPECT_EQ(testBaza.BrojRacuna(), 1);
	testBaza.DodajSobu(2, 4, 120);
	testBaza.DodajGostaIZakaziRezervaciju("Gost1", "654321", "E-mail", 2, "2026-08-30", "2026-09-05");
	testBaza.KreirajRacun(1, "2026-09-05", "kartica");
	EXPECT_EQ(testBaza.BrojRacuna(), 1);
}