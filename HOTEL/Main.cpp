#include<iostream>
#include<string>
#include"Baza.h"
using namespace std;
int main() {
	Baza baza;
	baza.KreirajTabelu();
	baza.KreirajIndex();
	cout << "Gotovo" << endl;
	baza.DodajSobu(1, 4, 70);
	baza.DodajSobu(2, 2, 50);
	baza.AzurirajStatusSobe(2, "na odrzavanju");
	baza.DodajGostaIZakaziRezervaciju("Ivan Ivic", "112233", "ivan.ivic@gmail.com", 1, "2026-07-30", "2026-08-05");
	baza.AzurirajStatusRezervacijeIStatusSobe(1, 1, "u toku");
	baza.PrikaziSveGoste();
	baza.PrikaziSveRezervacije();
	baza.PrikaziSveSobe();

}