#include "pch.h"
#include "Baza.h"

TEST(BazaTest, DodajSobuUvecajBrojSoba) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	testBaza.DodajSobu(1, 3, 90);
	EXPECT_EQ(testBaza.BrojSoba(), 1);
}
TEST(BazaTest, DodajIstuSobuProvjeriBrojSoba) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	testBaza.DodajSobu(1, 3, 90);
	EXPECT_EQ(testBaza.BrojSoba(), 1);
	testBaza.DodajSobu(1, 4, 120);
	EXPECT_EQ(testBaza.BrojSoba(), 1);
}
TEST(BazaTest, AzurirajStatusSobeProvjeriVrijednostStatusa) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	testBaza.DodajSobu(1, 3, 90);
	testBaza.AzurirajStatusSobe(1, "na odrzavanju");
	EXPECT_EQ(testBaza.StatusSobe(1), "na odrzavanju");
}
TEST(BazaTest, DodajGostaDodajRezervacijuProvjeriBrojGostijuBrojRezervacija) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	testBaza.DodajSobu(1, 3, 90);
	testBaza.DodajGostaIZakaziRezervaciju("Gost Gostic", "123456", "E-mail", 1, "2026-08-10", "2026-08-15");
	EXPECT_EQ(testBaza.BrojGostiju(), 1);
	EXPECT_EQ(testBaza.BrojRezervacija(), 1);
}
TEST(BazaTest, DodajGostaDodajRezervacijuNepostojecaSobaProvjeriBrojGostijuBrojRezervacija) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	testBaza.DodajSobu(1, 3, 90);
	testBaza.DodajGostaIZakaziRezervaciju("Gost Gostic", "123456", "E-mail", 2, "2026-08-10", "2026-08-15");
	EXPECT_EQ(testBaza.BrojGostiju(), 0);
	EXPECT_EQ(testBaza.BrojRezervacija(), 0);
}
TEST(BazaTest, DodajGostaDodajRezervacijuDupliGostProvjeriBrojGostijuBrojRezervacija) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	testBaza.DodajSobu(1, 3, 90);
	testBaza.DodajGostaIZakaziRezervaciju("Gost Gostic", "123456", "E-mail", 1, "2026-08-10", "2026-08-15");
	EXPECT_EQ(testBaza.BrojGostiju(), 1);
	EXPECT_EQ(testBaza.BrojRezervacija(), 1);
	testBaza.DodajSobu(2, 4, 120);
	testBaza.DodajGostaIZakaziRezervaciju("Gost1", "123456", "E-mail", 2, "2026-07-21", "2026-07-29");
	EXPECT_EQ(testBaza.BrojGostiju(), 1);
	EXPECT_EQ(testBaza.BrojRezervacija(), 1);
}
TEST(BazaTest, DodajGostaDodajRezervacijuDuplaRezervacijaProvjeriBrojGostijuBrojRezervacija) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	testBaza.DodajSobu(1, 3, 90);
	testBaza.DodajGostaIZakaziRezervaciju("Gost Gostic", "123456", "E-mail", 1, "2026-08-10", "2026-08-15");
	EXPECT_EQ(testBaza.BrojGostiju(), 1);
	EXPECT_EQ(testBaza.BrojRezervacija(), 1);
	testBaza.DodajGostaIZakaziRezervaciju("Gost1", "654321", "E-mail@", 1, "2026-08-10", "2026-08-15");
	EXPECT_EQ(testBaza.BrojGostiju(), 1);
	EXPECT_EQ(testBaza.BrojRezervacija(), 1);
}
TEST(BazaTest, DodajRezervacijuPostojecemGostuProvjeriBrojRezervacija) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	testBaza.DodajSobu(1, 4, 120);
	testBaza.DodajSobu(2, 2, 80);
	testBaza.DodajGostaIZakaziRezervaciju("Gost1", "123456", "e-mail", 1, "2026-08-08", "2026-08-13");
	EXPECT_EQ(testBaza.BrojRezervacija(), 1);
	testBaza.ZakaziRezervacijuPostojecemGostu("123456", 2, "2026-09-20", "2026-09-24");
	EXPECT_EQ(testBaza.BrojRezervacija(), 2);
}
TEST(BazaTest, DodajRezervacijuPostojecemGostuNepostojeciGostProvjeriBrojRezervacija) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	testBaza.DodajSobu(1, 4, 120);
	testBaza.DodajSobu(2, 2, 80);
	testBaza.DodajGostaIZakaziRezervaciju("Gost1", "123456", "e-mail", 1, "2026-08-08", "2026-08-13");
	EXPECT_EQ(testBaza.BrojRezervacija(), 1);
	testBaza.ZakaziRezervacijuPostojecemGostu("000000", 2, "2026-09-20", "2026-09-24");
	EXPECT_EQ(testBaza.BrojRezervacija(), 1);
}
TEST(BazaTest, DodajRezervacijuPostojecemGostuNepostojecaSobaProvjeriBrojRezervacija) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	testBaza.DodajSobu(1, 4, 120);
	testBaza.DodajGostaIZakaziRezervaciju("Gost1", "123456", "e-mail", 1, "2026-08-08", "2026-08-13");
	EXPECT_EQ(testBaza.BrojRezervacija(), 1);
	testBaza.ZakaziRezervacijuPostojecemGostu("123456", 2, "2026-09-20", "2026-09-24");
	EXPECT_EQ(testBaza.BrojRezervacija(), 1);
}
TEST(BazaTest, DodajRezervacijuPostojecemGostuDuplaRezervacijaProvjeriBrojRezervacija) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	testBaza.DodajSobu(1, 4, 120);
	testBaza.DodajGostaIZakaziRezervaciju("Gost1", "123456", "e-mail", 1, "2026-08-08", "2026-08-13");
	EXPECT_EQ(testBaza.BrojRezervacija(), 1);
	testBaza.ZakaziRezervacijuPostojecemGostu("123456", 1, "2026-08-08", "2026-08-13");
	EXPECT_EQ(testBaza.BrojRezervacija(), 1);
}
TEST(BazaTest, AzurirajStatusRezervacijeStatusSobeProvjeriVrijednostStatusaRezervacijeStatusaSobe) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	testBaza.DodajSobu(1, 3, 100);
	int RezervacijaID=testBaza.DodajGostaIZakaziRezervaciju("Gost1", "123456", "e-mail", 1, "2026-08-08", "2026-08-14");
	testBaza.AzurirajStatusRezervacijeIStatusSobe(RezervacijaID, 1, "u toku");
	EXPECT_EQ(testBaza.StatusRezervacijeStatusSobe(RezervacijaID, 1), "u toku");
}
TEST(BazaTest, DodajRacunProvjeraBrojaRacuna) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	testBaza.DodajSobu(1, 3, 90);
	testBaza.DodajGostaIZakaziRezervaciju("Gost Gostic", "123456", "E-mail", 1, "2026-08-10", "2026-08-15");
	testBaza.KreirajRacun(1, "2026-08-15", "kartica");
	EXPECT_EQ(testBaza.BrojRacuna(), 1);
}
TEST(BazaTest, DodajDupliRacunProvjeraBrojaRacuna) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	testBaza.DodajSobu(1, 3, 90);
	testBaza.DodajGostaIZakaziRezervaciju("Gost Gostic", "123456", "E-mail", 1, "2026-08-10", "2026-08-15");
	testBaza.KreirajRacun(1, "2026-08-15", "kartica");
	EXPECT_EQ(testBaza.BrojRacuna(), 1);
	testBaza.DodajSobu(2, 4, 120);
	testBaza.DodajGostaIZakaziRezervaciju("Gost1", "654321", "E-mail", 2, "2026-08-30", "2026-09-05");
	testBaza.KreirajRacun(1, "2026-09-05", "kartica");
	EXPECT_EQ(testBaza.BrojRacuna(), 1);
}
TEST(BazaTest, AzurirajStatusRacunaProvjeriProvjeriVrijednostStatusRacuna) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	testBaza.DodajSobu(1, 3, 90);
	int RezervacijaID=testBaza.DodajGostaIZakaziRezervaciju("Gost Gostic", "123456", "E-mail", 1, "2026-08-10", "2026-08-15");
	testBaza.KreirajRacun(RezervacijaID, "2026-08-15", "kartica");
	testBaza.AzurirajStatusRacuna(RezervacijaID, "placen");
	EXPECT_EQ(testBaza.StatusRacuna(RezervacijaID), "placen");
}
TEST(BazaTest, AzurirajNacinPlacanjaProvjeriVrijednostNacinaPlacanja) {
	Baza testBaza(":memory:");
	testBaza.KreirajTabelu();
	testBaza.DodajSobu(1, 3, 90);
	int RezervacijaID = testBaza.DodajGostaIZakaziRezervaciju("Gost Gostic", "123456", "E-mail", 1, "2026-08-10", "2026-08-15");
	testBaza.KreirajRacun(RezervacijaID, "2026-08-15", "kartica");
	testBaza.AzurirajNacinPlacanja(RezervacijaID, "gotovina");
	EXPECT_EQ(testBaza.NacinPlacanja(RezervacijaID), "gotovina");
}