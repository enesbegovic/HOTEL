#pragma once
#include<iostream>
#include<string>
#include"sqlite3.h"
#include "StmtPtr.h"
#include <vector>
using namespace std;
struct Soba {
	int ID;
	int BrojSobe;
	int Kapacitet;
	int CijenaPoDanu;
	string Status;
};
struct Gost {
	int ID;
	string ImePrezime;
	string JMBG;
	string email;
};
struct Rezervacija {
	int ID;
	string Gost;
	int BrojSobe;
	string datumPocetka;
	string datumKraja;
	string status;
};
struct Racun {
	int ID;
	string Gost;
	int RezervacijaID;
	string DatumIzdavanjaRacuna;
	double UkupanIznosRacuna;
	string NacinPlacanja;
	string Status;
};
class Baza {
	sqlite3* db;
public:
	Baza(string PutanjaBaze="Hotel.db");
	~Baza();
	void IzvrsiUpit(string sql,string PORUKAGRESKE);
	void KreirajTabelu();
	void KreirajIndex();
	void DodajSobu(int BrojSobe, int kapacitet, int Cijena);
	void AzurirajStatusSobe(int BrojSobe, string NoviStatus);
	string StatusSobe(int BrojSobe);
	void PrikaziSveSobe();
	vector<Soba>UzmiSveSobe();
	vector<Soba>UzmiSveSlobodneSobe(string status);
	vector<Soba>UzmiSveZauzeteSobe(string status);
	vector<Soba>UzmiSveSobeNaOdrzavanju(string status);
	int BrojSoba();
	void PrikaziSveSobePremaStatusu(string Status);
	int DodajGostaIZakaziRezervaciju(string ImePrezime, string JMBG, string email, int BrojSobe, string Pocetak, string Kraj);
	void AzurirajStatusRezervacijeIStatusSobe(int RezervacijaID, int BrojSobe, string NoviStatus);
	string StatusRezervacijeStatusSobe(int RezervacijaID, int BrojSobe);
	int ZakaziRezervacijuPostojecemGostu(string JMBG, int BrojSobe, string Pocetak, string Kraj);
	void PrikaziSveRezervacijePremaStatusu(string Status);
	void PrikaziSveGoste();
	vector<Gost>UzmiSveGoste();
	int BrojGostiju();
	void PrikaziSveRezervacije();
	vector<Rezervacija>UzmiSveRezervacije();
	vector<Rezervacija>UzmiSveRezervacijeNaCekanju(string status);
	vector<Rezervacija>UzmiSveRezervacijeUToku(string status);
	vector<Rezervacija>UzmiSveRezervacijeZavrsena(string status);
	vector<Rezervacija>UzmiSveRezervacijeOtkazana(string status);
	int BrojRezervacija();
	void KreirajRacun(int RezervacijID, string Datum, string NacinPlacanja);
	void AzurirajStatusRacuna(int RezervacijaID,string NoviStatus);
	string StatusRacuna(int RezervacijaID);
	void AzurirajNacinPlacanja(int RezervacijaID, string NoviNacin);
	string NacinPlacanja(int RezervacijaID);
	void PrikaziSveRacune();
	vector<Racun>UzmiSveRacune();
	vector<Racun>UzmiSveNeplaceneRacune(string status);
	vector<Racun>UzmiSvePlaceneRacune(string status);
	int BrojRacuna();
	void PrikaziSveRacunePremaStatusu(string Status);
};