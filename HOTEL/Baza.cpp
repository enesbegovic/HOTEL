#include<iostream>
#include<string>
#include"Baza.h"
using namespace std;
Baza::Baza() {
	sqlite3_open("HOTEL.db", &db);
	sqlite3_exec(db, "PRAGMA foreign_keys=ON;", nullptr, nullptr, nullptr);
}
Baza::~Baza() {
	sqlite3_close(db);
}
void Baza::IzvrsiUpit(string sql,string PORUKAGRESKE) {
	char* errMsg = nullptr;
	int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
	if (rc != SQLITE_OK) {
		cout << PORUKAGRESKE << ":" << errMsg << endl;
		sqlite3_free(errMsg);
	}
}
void Baza::KreirajTabelu() {
	string sqlSoba =
		"CREATE TABLE IF NOT EXISTS Sobe("
		"ID INTEGER PRIMARY KEY AUTOINCREMENT,"
		"BrojSobe INTEGER NOT NULL UNIQUE,"
		"SmjestajniKapacitet INTEGER NOT NULL,"
		"CijenaPoDanu INTEGER NOT NULL,"
		"StatusSobe TEXT NOT NULL DEFAULT 'slobodna' CHECK(StatusSobe IN('slobodna','zauzeta')));";
	IzvrsiUpit(sqlSoba, "GRESKA PRI KREIRANJU TABELE SOBE");

	string sqlGost =
		"CREATE TABLE IF NOT EXISTS Gosti("
		"ID INTEGER PRIMARY KEY AUTOINCREMENT,"
		"ImePrezime TEXT NOT NULL,"
		"JMBG TEXT NOT NULL UNIQUE,"
		"Email TEXT NOT NULL);";
	IzvrsiUpit(sqlGost, "GRESKA PRI KREIRANJU TABELE GOSTI");

	string sqlRezervacije =
		"CREATE TABLE IF NOT EXISTS Rezervacije("
		"ID INTEGER PRIMARY KEY AUTOINCREMENT,"
		"GostID INTEGER NOT NULL,"
		"SobaID INTEGER NOT NULL,"
		"Pocetak TEXT NOT NULL,"
		"Kraj TEXT NOT NULL,"
		"StatusRezervacije TEXT NOT NULL DEFAULT 'na cekanju' CHECK(StatusRezervacije IN('na cekanju','u toku','zavrsena','otkazana')),"
		"FOREIGN KEY (GostID) REFERENCES Gosti(ID) ON DELETE RESTRICT,"
		"FOREIGN KEY (SobaID) REFERENCES Sobe(ID) ON DELETE RESTRICT);";
	IzvrsiUpit(sqlRezervacije, "GRESKA PRI KREIRANJU TABELE REZERVACIJE");
	
	string sqlRacun =
		"CREATE TABLE IF NOT EXISTS Racuni("
		"ID INTEGER PRIMARY KEY AUTOINCREMENT,"
		"RezervacijaID INTEGER NOT NULL UNIQUE,"
		"DatumIzdavanjaRacuna TEXT NOT NULL,"
		"UkupanIznosRacuna REAL NOT NULL,"
		"NacinPlacanja TEXT NOT NULL DEFAULT 'gotovina' CHECK(NacinPlacanja IN('gotovina','kartica')),"
		"StatusRacuna TEXT NOT NULL DEFAULT 'placen' CHECK(StatusRacuna IN('placen','neplacen')),"
		"FOREIGN KEY (RezervacijaID) REFERENCES Racuni(ID) ON DELETE RESTRICT);";
	IzvrsiUpit(sqlRacun, "GRESKA PRI KREIRANJU TABELE RACUNI");
}
void Baza::KreirajIndex() {
	IzvrsiUpit("CREATE INDEX IF NOT EXISTS idx_rezervacije_gostid ON Rezervacije(GostID);", "GRESKA PRI KREIRANJU INDEXA ZA GOSTA");
	IzvrsiUpit("CREATE INDEX IF NOT EXISTS idx_rezervacije_sobaid ON Rezervacije(SobaID);", "GRESKA PRI KREIRANJU INDEXA ZA SOBE");
}