#include<iostream>
#include<string>
#include"Baza.h"
using namespace std;
int main() {
	Baza baza("C:\\Users\\enesb\\source\\repos\\HOTEL\\HOTEL\\Hotel.db");
	baza.KreirajTabelu();
	baza.KreirajIndex();
	int izbor;
	do {
		cout << "MENI:" << endl;
		cout << "1.Dodaj Sobu" << endl;
		cout << "2.Azuriraj Status Sobe(slobodna,zauzeta,na odrzavanju)" << endl;
		cout << "3.Prikazi Sve Sobe" << endl;
		cout << "4.Prikazi Sve Sobe Prema Statusu(slobodna,zauzeta,na odrzavanju)" << endl;
		cout << "5.Dodaj Gosta I Zakazi Rezervaciju" << endl;
		cout << "6.Zakazi Rezervaciju Postojecem Gostu" << endl;
		cout << "7.Azuriraj Status Rezervacije/Status Sobe(na cekanju,u toku,zavrsena,otkazana)" << endl;
		cout << "8.Prikazi Sve Goste" << endl;
		cout << "9.Prikazi Sve Rezervacije" << endl;
		cout << "10.Prikazi Sve Rezervacije Prema Statusu(na cekanju,u toku,zavrsena,otkazana)" << endl;
		cout << "11.Napravi Racun" << endl;
		cout << "12.Azuriraj Status Racuna(neplacen,placen)" << endl;
		cout << "13.Azuriraj Nacin Placanja(gotovina,kartica)" << endl;
		cout << "14.Prikazi Sve Racune" << endl;
		cout << "15.Prikazi Sve Racune Prema Statusu(placen,neplacen)" << endl;
		cout << "16.Izlaz" << endl;
		cin >> izbor;
		cin.ignore();
		switch (izbor) {
		case 1:
		{
			int BrojSobe;
			int Kapacitet;
			int Cijena;
			cout << "Unesite Broj Sobe: ";
			cin >> BrojSobe;
			cin.ignore();
			cout << "Unesite Kapacitet Sobe/Broj Osoba Koje Soba Moze Primiti: ";
			cin >> Kapacitet;
			cin.ignore();
			cout << "Unesite Cijenu Sobe Po Danu: ";
			cin >> Cijena;
			cin.ignore();
			baza.DodajSobu(BrojSobe, Kapacitet, Cijena);
			break;
		}
		case 2:
		{
			int BrojSobe;
			string Status;
			cout << "Unesite Broj Sobe Kojoj Zelite Azurirati Status: ";
			cin >> BrojSobe;
			cin.ignore();
			cout << "Unesite Novi Status Sobe(slobodna,zauzeta,na odrzavanju)" << endl;
			getline(cin, Status);
			baza.AzurirajStatusSobe(BrojSobe, Status);
			break;
		}
		case 3:
		{
			baza.PrikaziSveSobe();
			break;
		}
		case 4:
		{
			string Status;
			cout << "Unesite Staus Sobe(slobodna,zauzeta,na odrzavanju: ";
			getline(cin, Status);
			baza.PrikaziSveSobePremaStatusu(Status);
			break;
		}
		case 5:
		{
			string ImePrezime;
			string JMBG;
			string Email;
			int BrojSobe;
			int dp, mp, gp;
			int dk, mk, gk;
			cout << "Uneite Ime I Prezime Gosta: ";
			getline(cin, ImePrezime);
			cout << "Unesite JMBG/OIB Gosta: ";
			getline(cin, JMBG);
			cout << "Unesite E-mail Adresu Gosta: ";
			getline(cin, Email);
			cout << "Unesite Broj Sobe Za Koju Vrsite Rezervaciju: ";
			cin >> BrojSobe;
			cin.ignore();
			cout << "Unesite Dan Pocetka Rezervacije: ";
			cin >> dp;
			cout << "Unesite Mjesec Pocetka Rezervacije: ";
			cin >> mp;
			cout << "Unesite Godinu Pocetka Rezervacije: ";
			cin >> gp;
			cin.ignore();
			cout << "Unesite Dan Kraja Rezervacije: ";
			cin >> dk;
			cout << "Unesite Mjesec Kraja Rezervacije: ";
			cin >> mk;
			cout << "Unesite Godinu Kraja Rezervacije: ";
			cin >> gk;
			cin.ignore();
			string DatumPocetkaISO = to_string(gp) + "-" + (mp < 10 ? "0" : "") + to_string(mp) + "-" + (dp < 10 ? "0" : "") + to_string(dp);
			string DatumKrajaISO = to_string(gk) + "-" + (mk < 10 ? "0" : "") + to_string(mk) + "-" + (dk < 10 ? "0" : "") + to_string(dk);
			baza.DodajGostaIZakaziRezervaciju(ImePrezime, JMBG, Email, BrojSobe, DatumPocetkaISO, DatumKrajaISO);
			break;
		}
		case 6:
		{
			string JMBG;
			int BrojSobe;
			int dp, mp, gp;
			int dk, mk, gk;
			cout << "Unesite JMBG/OIB Gosta Kojem Zakazujete Rezervaciju: ";
			getline(cin, JMBG);
			cout << "Unesite Broj Sobe Za Koju Vrsite Rezervaciju: ";
			cin >> BrojSobe;
			cin.ignore();
			cout << "Unesite Dan Pocetka Rezervacije: ";
			cin >> dp;
			cout << "Unesite Mjesec Pocetka Rezervacije: ";
			cin >> mp;
			cout << "Unesite Godinu Pocetka Rezervacije: ";
			cin >> gp;
			cin.ignore();
			cout << "Unesite Dan Kraja Rezervacije: ";
			cin >> dk;
			cout << "Unesite Mjesec Kraja Rezervacije: ";
			cin >> mk;
			cout << "Unesite Godinu Kraja Rezervacije: ";
			cin >> gk;
			cin.ignore();
			string DatumPocetkaISO = to_string(gp) + "-" + (mp < 10 ? "0" : "") + to_string(mp) + "-" + (dp < 10 ? "0" : "") + to_string(dp);
			string DatumKrajaISO = to_string(gk) + "-" + (mk < 10 ? "0" : "") + to_string(mk) + "-" + (dk < 10 ? "0" : "") + to_string(dk);
			baza.ZakaziRezervacijuPostojecemGostu(JMBG, BrojSobe, DatumPocetkaISO, DatumKrajaISO);
			break;
		}
		case 7:
		{
			int RezervacijaID;
			int BrojSobe;
			string Status;
			cout << "Unesite ID Rezervacije Za Koju Azurirate Status: ";
			cin >> RezervacijaID;
			cin.ignore();
			cout << "Unesite Broj Sobe Za Koju Je Izvrsena Rezervacija: ";
			cin >> BrojSobe;
			cin.ignore();
			cout << "Unesite Novi Status Rezervacije(na cekanju,u toku,zavrsena,otkazana): ";
			getline(cin, Status);
			baza.AzurirajStatusRezervacijeIStatusSobe(RezervacijaID, BrojSobe, Status);
			break;
		}
		case 8:
		{
			baza.PrikaziSveGoste();
			break;
		}
		case 9:
		{
			baza.PrikaziSveRezervacije();
			break;
		}
		case 10:
		{
			string Status;
			cout << "Unesite Status Rezervacije: ";
			getline(cin, Status);
			baza.PrikaziSveRezervacijePremaStatusu(Status);
			break;
		}
		case 11:
		{
			int RezervacijaID;
			int d, m, g;
			string NacinPlacanja;
			cout << "Unesite ID Rezervacije Za Koju Pravite Racun: ";
			cin >> RezervacijaID;
			cin.ignore();
			cout << "Unesite Dan Kreiranja Racuna: ";
			cin >> d;
			cout << "Unesite Mjesec Kreiranja Racuna: ";
			cin >> m;
			cout << "Unesite Godinu Kreiranja Racuna: ";
			cin >> g;
			cin.ignore();
			string DatumPlacanjaISO = to_string(g) + "-" + (m < 10 ? "0" : "") + to_string(m) + "-" + (d < 10 ? "0" : "") + to_string(d);
			cout << "Unesite Nacin Placanja Racuna(gotovina,kartica): ";
			getline(cin, NacinPlacanja);
			baza.KreirajRacun(RezervacijaID, DatumPlacanjaISO, NacinPlacanja);
			break;
		}
		case 12:
		{
			int RezervacijaID;
			string StatusRacuna;
			cout << "Unesite ID Rezervacije Za Koju Azurirate Status Racuna: ";
			cin >> RezervacijaID;
			cin.ignore();
			cout << "Unesite Novi Status Racuna(neplacen,placen): ";
			getline(cin, StatusRacuna);
			baza.AzurirajStatusRacuna(RezervacijaID, StatusRacuna);
			break;
		}
		case 13:
		{
			int RezervacijaID;
			string NoviNacin;
			cout << "Unesite ID Rezervacije Za Koju Mijenjate Nacin Placanja: ";
			cin >> RezervacijaID;
			cin.ignore();
			cout << "Unesite Novi Nacin Placanja(gotovina,kartica): ";
			getline(cin, NoviNacin);
			baza.AzurirajNacinPlacanja(RezervacijaID, NoviNacin);
			break;
		}
		case 14:
		{
			baza.PrikaziSveRacune();
			break;
		}
		case 15:
		{
			string StatusRacuna;
			cout << "Unesite Status Racuna(neplacen,placen): ";
			getline(cin, StatusRacuna);
			baza.PrikaziSveRacunePremaStatusu(StatusRacuna);
			break;
		}
		case 16:
		{
			cout << "DOVIDJENJA" << endl;
			break;
		}
		default:
		{
			cout << "POGRESAN IZBOR" << endl;
			break;
		}
		}
	}while (izbor != 16);

}