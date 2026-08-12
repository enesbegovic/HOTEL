#pragma once
#include<iostream>
#include<string>
#include"sqlite3.h"
#include "StmtPtr.h"
using namespace std;
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
	int BrojSoba();
	void PrikaziSveSobePremaStatusu(string Status);
	int DodajGostaIZakaziRezervaciju(string ImePrezime, string JMBG, string email, int BrojSobe, string Pocetak, string Kraj);
	void AzurirajStatusRezervacijeIStatusSobe(int RezervacijaID, int BrojSobe, string NoviStatus);
	string StatusRezervacijeStatusSobe(int RezervacijaID, int BrojSobe);
	void ZakaziRezervacijuPostojecemGostu(string JMBG, int BrojSobe, string Pocetak, string Kraj);
	void PrikaziSveRezervacijePremaStatusu(string Status);
	void PrikaziSveGoste();
	int BrojGostiju();
	void PrikaziSveRezervacije();
	int BrojRezervacija();
	void KreirajRacun(int RezervacijID, string Datum, string NacinPlacanja);
	void AzurirajStatusRacuna(int RezervacijaID,string NoviStatus);
	string StatusRacuna(int RezervacijaID);
	void AzurirajNacinPlacanja(int RezervacijaID, string NoviNacin);
	string NacinPlacanja(int RezervacijaID);
	void PrikaziSveRacune();
	int BrojRacuna();
	void PrikaziSveRacunePremaStatusu(string Status);
};