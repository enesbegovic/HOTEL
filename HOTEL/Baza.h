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
	void PrikaziSveGoste();
	void PrikaziSveRezervacije();
};