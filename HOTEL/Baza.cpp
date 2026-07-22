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
		"StatusRacuna TEXT NOT NULL DEFAULT 'neplacen' CHECK(StatusRacuna IN('placen','neplacen')),"
		"FOREIGN KEY (RezervacijaID) REFERENCES Racuni(ID) ON DELETE RESTRICT);";
	IzvrsiUpit(sqlRacun, "GRESKA PRI KREIRANJU TABELE RACUNI");
}
void Baza::KreirajIndex() {
	IzvrsiUpit("CREATE INDEX IF NOT EXISTS idx_rezervacije_gostid ON Rezervacije(GostID);", "GRESKA PRI KREIRANJU INDEXA ZA GOSTA");
	IzvrsiUpit("CREATE INDEX IF NOT EXISTS idx_rezervacije_sobaid ON Rezervacije(SobaID);", "GRESKA PRI KREIRANJU INDEXA ZA SOBE");
}
void Baza::DodajSobu(int BrojSobe, int Kapacitet, int Cijena) {
	sqlite3_stmt* stmtProvjera;
	int count = 0;
	string sqlProvjera =
		"SELECT COUNT (*) FROM Sobe WHERE BrojSobe=?;";
	int rc = sqlite3_prepare_v2(db, sqlProvjera.c_str(), -1, &stmtProvjera, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PROVJERE SOBE:" << sqlite3_errmsg(db) << endl;
		return;
	}
	sqlite3_bind_text(stmtProvjera, 1, to_string(BrojSobe).c_str(), -1, SQLITE_TRANSIENT);
	if (sqlite3_step(stmtProvjera) == SQLITE_ROW) {
		count = sqlite3_column_int(stmtProvjera, 0);
	}
	sqlite3_finalize(stmtProvjera);
	if (count > 0) {
		cout << "GRESKA PRI DODAVANJU SOBE-SOBA VEC POSTOJI" << endl;
		return;
	}
	sqlite3_stmt* stmt;
	string sql =
		"INSERT INTO Sobe(BrojSobe, SmjestajniKapacitet, CijenaPoDanu) VALUES(?, ?, ?);";
	int rc1 = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
	if (rc1 != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI DODAVANJA SOBE:" << sqlite3_errmsg(db) << endl;
		return;
	}
	sqlite3_bind_int(stmt, 1, BrojSobe);
	sqlite3_bind_int(stmt, 2, Kapacitet);
	sqlite3_bind_int(stmt, 3, Cijena);
	rc1 = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	if (rc1 != SQLITE_DONE) {
		cout << "GRESKA PRI DODAVANJU SOBE:" << sqlite3_errmsg(db) << endl;
		return;
	}
}
void Baza::AzurirajStatusSobe(int BrojSobe, string StatusSobe) {
	sqlite3_stmt* stmt;
	string sql =
		"UPDATE Sobe SET StatusSobe=? WHERE BrojSobe=?;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI AZURIRANJA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	sqlite3_bind_text(stmt, 1, StatusSobe.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_int(stmt, 2, BrojSobe);
	rc = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	if (rc != SQLITE_DONE) {
		cout << "GRESKA PRI AZURIRANJU STATUSA:" << sqlite3_errmsg(db) << endl;
		return;
	}
}
void Baza::PrikaziSveSobe() {
	sqlite3_stmt* stmt;
	string sql =
		"SELECT * FROM Sobe;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PRIKAZA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		int ID = sqlite3_column_int(stmt, 0);
		int BrojSobe = sqlite3_column_int(stmt, 1);
		int Kapacitet = sqlite3_column_int(stmt, 2);
		int Cijena = sqlite3_column_int(stmt, 3);
		string Status = (const char*)sqlite3_column_text(stmt, 4);

		cout << "ID:" << ID << endl;
		cout << "Broj Sobe:" << BrojSobe << endl;
		cout << "Kapacitet Sobe/Broj Osoba Koje Soba Moze Primiti:" << Kapacitet << endl;
		cout << "Cijena Po Danu:" << Cijena << " KM" << endl;
		cout << "Status:" << Status << endl;
		cout << "---" << endl;
	}
	sqlite3_finalize(stmt);
}