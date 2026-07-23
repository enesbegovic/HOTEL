#pragma once
#include<iostream>
#include<string>
#include"sqlite3.h"
using namespace std;
class Baza {
	sqlite3* db;
public:
	Baza();
	~Baza();
	void IzvrsiUpit(string sql,string PORUKAGRESKE);
	void KreirajTabelu();
	void KreirajIndex();
	void DodajSobu(int BrojSobe, int kapacitet, int Cijena);
	void AzurirajStatusSobe(int BrojSobe, string NoviStatus);
	void PrikaziSveSobe();
	void DodajGostaIZakaziRezervaciju(string ImePrezime, string JMBG, string email, int BrojSobe, string Pocetak, string Kraj);
	void AzurirajStatusRezervacijeIStatusSobe(int RezervacijaID, int BrojSobe, string NoviStatus);
	void ZakaziRezervacijuPostojecemGostu(string JMBG, int BrojSobe, string Pocetak, string Kraj);
	void PrikaziSveRezervacijePremaStatusu(string Status);
	void PrikaziSveGoste();
	void PrikaziSveRezervacije();
	void KreirajRacun(int RezervacijID, string Datum, string NacinPlacanja);
	void AzurirajStatusRacuna(int RezervacijaID,string NoviStatus);
	void AzurirajNacinPlacanja(int RezervacijaID, string NoviNacin);
	void PrikaziSveRacune();
	void PrikaziSveRacunePremaStatusu(string Status);
};