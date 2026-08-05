#include<iostream>
#include<string>
#include"Baza.h"
using namespace std;
Baza::Baza(string PutanjaBaze) {
	sqlite3_open(PutanjaBaze.c_str(), &db);
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
		"StatusSobe TEXT NOT NULL DEFAULT 'slobodna' CHECK(StatusSobe IN('slobodna','zauzeta','na odrzavanju')));";
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
		"FOREIGN KEY (RezervacijaID) REFERENCES Rezervacije(ID) ON DELETE RESTRICT);";
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
	cout << "SOBA USPJESNO DODANA" << endl;
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
	cout << "STATUS SOBE USPJESNO AZURIRAN" << endl;
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
int Baza::BrojSoba() {
	sqlite3_stmt* stmt;
	string sql =
		"SELECT COUNT (*) FROM Sobe;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI BROJANJA SOBA:" << sqlite3_errmsg(db) << endl;
		return -1;
	}
	int count = 0;
	if (sqlite3_step(stmt) == SQLITE_ROW) {
		count = sqlite3_column_int(stmt, 0);
	}
	sqlite3_finalize(stmt);
	return count;
}
void Baza::PrikaziSveSobePremaStatusu(string Status) {
	sqlite3_stmt* stmt;
	string sql =
		"SELECT * FROM Sobe WHERE StatusSobe=?;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PRIKAZA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	sqlite3_bind_text(stmt, 1, Status.c_str(), -1, SQLITE_TRANSIENT);
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
void Baza::DodajGostaIZakaziRezervaciju(string ImePrezime, string JMBG, string email, int BrojSobe, string Pocetak, string Kraj) {
	IzvrsiUpit("BEGIN TRANSACTION;", "GRESKA PRI POKRETANJU TRANSAKCIJE");
	sqlite3_stmt* stmtGost;
	string sqlGost =
		"INSERT INTO Gosti(ImePrezime, JMBG, Email) VALUES(?, ?, ?);";
	int rc = sqlite3_prepare_v2(db, sqlGost.c_str(), -1, &stmtGost, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI DODAVANJA GOSTA:" << sqlite3_errmsg(db) << endl;
		IzvrsiUpit("ROLLBACK;", "GRESKA PRI ROLLBAC-u");
		return;
	}
	sqlite3_bind_text(stmtGost, 1, ImePrezime.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmtGost, 2, JMBG.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmtGost, 3, email.c_str(), -1, SQLITE_TRANSIENT);
	rc = sqlite3_step(stmtGost);
	if (rc != SQLITE_DONE) {
		cout << "GRESKA PRI DODAVANJU GOSTA:" << sqlite3_errmsg(db) << endl;
		IzvrsiUpit("ROLLBACK;", "GRESKA PRI ROLLBACK-u");
		return;
	}
	int GostID = (int)sqlite3_last_insert_rowid(db);
	sqlite3_stmt* stmtProvjeraSobe;
	int SobaID = 0;
	string sqlProvjera =
		"SELECT ID FROM Sobe WHERE BrojSobe=?;";
	int rc1 = sqlite3_prepare_v2(db, sqlProvjera.c_str(), -1, &stmtProvjeraSobe, nullptr);
	if (rc1 != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PROVJERE SOBE:" << sqlite3_errmsg(db) << endl;
		IzvrsiUpit("ROLLBACK;", "GRESKA PRI ROLLBACK-u");
		return;
	}
	sqlite3_bind_int(stmtProvjeraSobe, 1, BrojSobe);
	if (sqlite3_step(stmtProvjeraSobe) == SQLITE_ROW) {
		SobaID = sqlite3_column_int(stmtProvjeraSobe, 0);
	}
	sqlite3_finalize(stmtProvjeraSobe);
	if (SobaID == 0) {
		cout << "GRESKA PRI DODAVANJU REZERVACIJE-NE POSTOJI TRAZENA SOBA" << endl;
		IzvrsiUpit("ROLLBACK;", "GRESKA PRI ROLLBACK-u");
		return;
	}
	sqlite3_stmt* stmtProvjeraRezervacije;
	int count = 0;
	string sqlProvjeraRezervacije =
		"SELECT COUNT (*) FROM Rezervacije WHERE SobaID=? AND Pocetak=? AND Kraj=?;";
	int rc3 = sqlite3_prepare_v2(db, sqlProvjeraRezervacije.c_str(), -1, &stmtProvjeraRezervacije, nullptr);
	if (rc3 != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PROVJERE REZERVACIJE:" << sqlite3_errmsg(db) << endl;
		IzvrsiUpit("ROLLBACK;", "GRESKA PRI ROLLBACK-u");
		return;
	}
	sqlite3_bind_int(stmtProvjeraRezervacije, 1, SobaID);
	sqlite3_bind_text(stmtProvjeraRezervacije, 2, Pocetak.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmtProvjeraRezervacije, 3, Kraj.c_str(), -1, SQLITE_TRANSIENT);
	if (sqlite3_step(stmtProvjeraRezervacije) == SQLITE_ROW) {
		count = sqlite3_column_int(stmtProvjeraRezervacije, 0);
	}
	sqlite3_finalize(stmtProvjeraRezervacije);
	if (count > 0) {
		cout << "GRESKA PRI ZAKAZIVANJU REZERVACIJE-REZERVACIJA ZA TRAZENI VREMENSKI PERIOD VEC POSTOJI" << endl;
		IzvrsiUpit("ROLLBACK;", "GRESKA PRI ROLLBACK-u");
		return;
	}
	sqlite3_stmt* stmtRezervacija;
	string sqlRezervacija =
		"INSERT INTO Rezervacije(GostID, SobaID, Pocetak, Kraj) VALUES(?, ?, ?, ?);";
	int rc2 = sqlite3_prepare_v2(db, sqlRezervacija.c_str(), -1, &stmtRezervacija, nullptr);
	if (rc2 != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI DODAVANJA REZERVACIJE" << sqlite3_errmsg(db) << endl;
		IzvrsiUpit("ROLLBACK;", "GRESKA PRI ROLLBACK-u");
		return;
	}
	sqlite3_bind_int(stmtRezervacija, 1, GostID);
	sqlite3_bind_int(stmtRezervacija, 2, SobaID);
	sqlite3_bind_text(stmtRezervacija, 3, Pocetak.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmtRezervacija, 4, Kraj.c_str(), -1, SQLITE_TRANSIENT);
	rc2 = sqlite3_step(stmtRezervacija);
	sqlite3_finalize(stmtRezervacija);
	if (rc2 != SQLITE_DONE) {
		cout << "GRESKA PRI DODAVANJU REZERVACIJE:" << sqlite3_errmsg(db) << endl;
		IzvrsiUpit("ROLLBACK;", "GRESKA PRI ROLLBACK-u");
		return;
	}
	IzvrsiUpit("COMMIT;", "GRESKA PRI COMMIT-u");
	cout << "TRANSAKCIJA USPJESNA-DODANI GOST I REZERVACIJA" << endl;
}
void Baza::AzurirajStatusRezervacijeIStatusSobe(int RezervacijaID, int BrojSobe, string NoviStatus) {
	IzvrsiUpit("BEGIN TRANSACTION;", "GRESKA PRI POKRETANJU TRANSAKCIJE");
	sqlite3_stmt* stmtStatusRerervacije;
	string sqlRezervacija =
		"UPDATE Rezervacije SET StatusRezervacije=? WHERE Rezervacije.ID=?;";
	int rc = sqlite3_prepare_v2(db, sqlRezervacija.c_str(), -1, &stmtStatusRerervacije, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI AZURIRANJA STATUSA REZERVACIJE" << sqlite3_errmsg(db) << endl;
		IzvrsiUpit("ROLLBACK;", "GRESKA PRI ROLLBACK-u");
		return;
	}
	sqlite3_bind_text(stmtStatusRerervacije, 1, NoviStatus.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_int(stmtStatusRerervacije, 2, RezervacijaID);
	rc = sqlite3_step(stmtStatusRerervacije);
	sqlite3_finalize(stmtStatusRerervacije);
	if (rc != SQLITE_DONE) {
		cout << "GRESKA PRI AZURIRANJU STATUSA REZERVACIJE:" << sqlite3_errmsg(db) << endl;
		IzvrsiUpit("ROLLBACK;", "GRESKA PRI ROLLBACK-u");
		return;
	}
	string trenutniStatusSobe = "";
	if (NoviStatus == "u toku") {
		trenutniStatusSobe = "zauzeta";
	}
	else if (NoviStatus == "otkazana" || NoviStatus == "zavrsena") {
		trenutniStatusSobe = "slobodna";
	}
	if (trenutniStatusSobe != "") {
		sqlite3_stmt* stmtStatusSobe;
		string sqlSoba =
			"UPDATE Sobe SET StatusSobe=? WHERE BrojSobe=?;";
		int rc1 = sqlite3_prepare_v2(db, sqlSoba.c_str(), -1, &stmtStatusSobe, nullptr);
		if (rc1 != SQLITE_OK) {
			cout << "GRESKA PRI PRIPREMI AZURIRANJA STATUSA SOBE:" << sqlite3_errmsg(db) << endl;
			IzvrsiUpit("ROLLBACK;", "GRESKA PRI ROLLBACK-u");
			return;
		}
		sqlite3_bind_text(stmtStatusSobe, 1, trenutniStatusSobe.c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_int(stmtStatusSobe, 2, BrojSobe);
		rc1 = sqlite3_step(stmtStatusSobe);
		sqlite3_finalize(stmtStatusSobe);
		if (rc1 != SQLITE_DONE) {
			cout << "GRESKA PRI AZURIRANJU STATUSA SOBE:" << sqlite3_errmsg(db) << endl;
			IzvrsiUpit("ROLLBACK;", "GRESKA PRI ROLLBACK-u");
			return;
		}
	}
	IzvrsiUpit("COMMIT;", "GRESKA PRI COMMIT-u");
	cout << "TRANSAKCIJA USPJESNA-AZURIRANI STATUS REZERVACIJE I STATUS SOBE" << endl;
}
void Baza::PrikaziSveGoste() {
	sqlite3_stmt* stmt;
	string sql =
		"SELECT * FROM Gosti;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PRIKAZA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		int ID = sqlite3_column_int(stmt, 0);
		string ImePrezime = (const char*)sqlite3_column_text(stmt, 1);
		string JMBG = (const char*)sqlite3_column_text(stmt, 2);
		string email = (const char*)sqlite3_column_text(stmt, 3);
		
        cout << "ID:" << ID << endl;
		cout << "Ime I Prezime Gosta:" << ImePrezime << endl;
		cout << "JMBG:" << JMBG << endl;
		cout << "E-mail Adresa:" << email << endl;
		cout << "---" << endl;
	}
	sqlite3_finalize(stmt);
}
int Baza::BrojGostiju() {
	sqlite3_stmt* stmt;
	string sql =
		"SELECT COUNT (*) FROM Gosti;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI BROJANJA GOSTIJU:" << sqlite3_errmsg(db) << endl;
		return -1;
	}
	int count = 0;
	if (sqlite3_step(stmt) == SQLITE_ROW) {
		count = sqlite3_column_int(stmt,0);
	}
	sqlite3_finalize(stmt);
	return count;
}
void Baza::PrikaziSveRezervacije() {
	sqlite3_stmt* stmt;
	string sql =
		"SELECT Rezervacije.ID, Gosti.ImePrezime, Sobe.BrojSobe, Rezervacije.Pocetak, Rezervacije.Kraj, Rezervacije.StatusRezervacije "
		"FROM Rezervacije "
		"JOIN Gosti ON Rezervacije.GostID=Gosti.ID "
		"JOIN Sobe ON Rezervacije.SobaID=Sobe.ID;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PRIKAZA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		int ID = sqlite3_column_int(stmt, 0);
		string ImePrezime = (const char*)sqlite3_column_text(stmt, 1);
		int BrojSobe = sqlite3_column_int(stmt, 2);
		string Pocetak = (const char*)sqlite3_column_text(stmt, 3);
		string Kraj = (const char*)sqlite3_column_text(stmt, 4);
		string Status = (const char*)sqlite3_column_text(stmt, 5);

		cout << "ID:" << ID << endl;
		cout << "Ime I Prezime Gosta:" << ImePrezime << endl;
		cout << "Broj Sobe:" << BrojSobe << endl;
		cout << "Datum Pocetka Rezervacije:" << Pocetak << endl;
		cout << "Datum Kraja Rezervacije:" << Kraj << endl;
		cout << "Status:" << Status << endl;
		cout << "---" << endl;
	}
	sqlite3_finalize(stmt);
}
int Baza::BrojRezervacija() {
	sqlite3_stmt* stmt;
	string sql =
		"SELECT COUNT (*) FROM Rezervacije;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI BROJANJA REZERVACIJA:" << sqlite3_errmsg(db) << endl;
		return -1;
	}
	int count = 0;
	if (sqlite3_step(stmt) == SQLITE_ROW) {
		count = sqlite3_column_int(stmt, 0);
	}
	sqlite3_finalize(stmt);
	return count;
}
void Baza::ZakaziRezervacijuPostojecemGostu(string JMBG, int BrojSobe, string Pocetak, string Kraj) {
	sqlite3_stmt* stmtProvjeraSobe;
	int SobaID = 0;
	string sqlProvjeraSobe =
		"SELECT ID FROM Sobe WHERE BrojSobe=?;";
	int rc = sqlite3_prepare_v2(db, sqlProvjeraSobe.c_str(), -1, &stmtProvjeraSobe, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PROVJERE SOBE:" << sqlite3_errmsg(db) << endl;
		return;
	}
	sqlite3_bind_int(stmtProvjeraSobe, 1, BrojSobe);
	if (sqlite3_step(stmtProvjeraSobe) == SQLITE_ROW) {
		SobaID = sqlite3_column_int(stmtProvjeraSobe, 0);
	}
	sqlite3_finalize(stmtProvjeraSobe);
	if (SobaID == 0) {
		cout << "GRESKA PRI ZAKAZIVANJU REZERVACIJE-NE POSTOJI TRAZENA SOBA" << endl;
		return;
	}
	sqlite3_stmt* stmtProvjeraRezervacije;
	int count = 0;
	string sqlProvjeraRezervacije =
		"SELECT COUNT (*) FROM Rezervacije WHERE SobaID=? AND Pocetak=? AND Kraj=?;";
	int rc1 = sqlite3_prepare_v2(db, sqlProvjeraRezervacije.c_str(), -1, &stmtProvjeraRezervacije, nullptr);
	if (rc1 != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PROVJERE REZERVACIJE:" << sqlite3_errmsg(db) << endl;
		return;
	}
	sqlite3_bind_int(stmtProvjeraRezervacije, 1, SobaID);
	sqlite3_bind_text(stmtProvjeraRezervacije, 2, Pocetak.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmtProvjeraRezervacije, 3, Kraj.c_str(), -1, SQLITE_TRANSIENT);
	if (sqlite3_step(stmtProvjeraRezervacije) == SQLITE_ROW) {
		count = sqlite3_column_int(stmtProvjeraRezervacije, 0);
	}
	sqlite3_finalize(stmtProvjeraRezervacije);
	if (count > 0) {
		cout << "GRESKA PRI ZAKAZIVANJU REZERVACIJE-REZERVACIJA VEC POSTOJI" << endl;
		return;
	}
	sqlite3_stmt* stmtProvjera;
	int GostID = 0;
	string sqlProvjera =
		"SELECT ID FROM Gosti WHERE JMBG=?;";
	int rc2 = sqlite3_prepare_v2(db, sqlProvjera.c_str(), -1, &stmtProvjera, nullptr);
	if (rc2 != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PROVJERE GOSTA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	sqlite3_bind_text(stmtProvjera, 1, JMBG.c_str(), -1, SQLITE_TRANSIENT);
	if (sqlite3_step(stmtProvjera) == SQLITE_ROW) {
		GostID = sqlite3_column_int(stmtProvjera, 0);
	}
	sqlite3_finalize(stmtProvjera);
	if (GostID == 0) {
		cout << "GRESKA PRI ZAKAZIVANJU REZERVACIJE-NE POSTOJI TRAZENI GOST" << endl;
		return;
	}
	sqlite3_stmt* stmt;
	string sql =
		"INSERT INTO Rezervacije(GostID, SobaID, Pocetak, Kraj) VALUES(?, ?, ?, ?);";
	int rc3 = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
	if (rc3 != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI ZAKAZIVANJA REZERVACIJE:" << sqlite3_errmsg(db) << endl;
		return;
	}
	sqlite3_bind_int(stmt, 1, GostID);
	sqlite3_bind_int(stmt, 2, SobaID);
	sqlite3_bind_text(stmt, 3, Pocetak.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 4, Kraj.c_str(), -1, SQLITE_TRANSIENT);
	rc3 = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	if (rc3 != SQLITE_DONE) {
		cout << "GRESKA PRI ZAKAZIVANJU REZERVACIJE:" << sqlite3_errmsg(db) << endl;
		return;
	}
	cout << "USPJESNO ZAKAZANA REZERVACIJA POSTOJECEM GOSTU" << endl;
}
void Baza::PrikaziSveRezervacijePremaStatusu(string Status) {
	sqlite3_stmt* stmt;
	string sql =
		"SELECT Gosti.ImePrezime, Sobe.BrojSobe, Rezervacije.Pocetak, Rezervacije.Kraj, Rezervacije.StatusRezervacije "
		"FROM Rezervacije "
		"JOIN Gosti ON Rezervacije.GostID=Gosti.ID "
		"JOIN Sobe ON Rezervacije.SobaID=Sobe.ID "
		"WHERE Rezervacije.StatusRezervacije=?;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PRIKAZA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	sqlite3_bind_text(stmt, 1, Status.c_str(), -1, SQLITE_TRANSIENT);
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		string Ime = (const char*)sqlite3_column_text(stmt, 0);
		int BrojSobe = sqlite3_column_int(stmt, 1);
		string Pocetak = (const char*)sqlite3_column_text(stmt, 2);
		string Kraj = (const char*)sqlite3_column_text(stmt, 3);
		string Status = (const char*)sqlite3_column_text(stmt, 4);

		cout << "Ime I Prezime Gosta:" << Ime << endl;
		cout << "Broj Sobe:" << BrojSobe << endl;
		cout << "Datum Pocetka Rezervacije:" << Pocetak << endl;
		cout << "Datum Kraja Rezervacije:" << Kraj << endl;
		cout << "Status Rezervacije:" << Status << endl;
		cout << "---" << endl;
	}
	sqlite3_finalize(stmt);
}
void Baza::KreirajRacun(int RezervacijID, string Datum, string NacinPlacanja) {
	sqlite3_stmt* stmtProvjera;
	int CijenaPoDanu = 0;
	string Pocetak="", Kraj="";
	string sqlProvjera =
		"SELECT Rezervacije.Pocetak, Rezervacije.Kraj, Sobe.CijenaPoDanu "
		"FROM Rezervacije "
		"JOIN Sobe ON Rezervacije.SobaID=Sobe.ID "
		"WHERE Rezervacije.ID=?;";
	int rc = sqlite3_prepare_v2(db, sqlProvjera.c_str(), -1, &stmtProvjera, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PROVJERE:" << sqlite3_errmsg(db) << endl;
		return;
	}
	sqlite3_bind_int(stmtProvjera, 1, RezervacijID);
	if (sqlite3_step(stmtProvjera) == SQLITE_ROW) {
		Pocetak =(const char*) sqlite3_column_text(stmtProvjera, 0);
		Kraj = (const char*)sqlite3_column_text(stmtProvjera, 1);
		CijenaPoDanu = sqlite3_column_int(stmtProvjera, 2);
	}
	sqlite3_finalize(stmtProvjera);
	if (Pocetak == "" || Kraj == "") {
		cout << "GRESKA PRI KREIRANJU RACUNA-NEISPRAVAN UNOS DATUMA" << endl;
		return;
	}
	sqlite3_stmt* stmtDani;
	double BrojNocenja = 0;
	string sqlDani =
		"SELECT julianday(?) - julianday(?);";
	int rc1 = sqlite3_prepare_v2(db, sqlDani.c_str(), -1, &stmtDani, nullptr);
	if (rc1 != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PROVJERE DANA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	sqlite3_bind_text(stmtDani, 1, Kraj.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmtDani, 2, Pocetak.c_str(), -1, SQLITE_TRANSIENT);
	if (sqlite3_step(stmtDani) == SQLITE_ROW) {
		BrojNocenja = sqlite3_column_double(stmtDani, 0);
	}
	sqlite3_finalize(stmtDani);
	double UkupanIznos = CijenaPoDanu * BrojNocenja;
	sqlite3_stmt* stmt;
	string sql =
		"INSERT INTO Racuni(RezervacijaID, DatumIzdavanjaRacuna, UkupanIznosRacuna, NacinPlacanja) VALUES(?, ?, ?, ?);";
	int rc2 = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
	if (rc2 != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI KREIRANJA RACUNA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	sqlite3_bind_int(stmt, 1, RezervacijID);
	sqlite3_bind_text(stmt, 2, Datum.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_double(stmt, 3, UkupanIznos);
	sqlite3_bind_text(stmt, 4, NacinPlacanja.c_str(), -1, SQLITE_TRANSIENT);
	rc2 = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	if (rc2 != SQLITE_DONE) {
		cout << "GRESKA PRI KREIRANJU RACUNA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	cout << "RACUN USPJESNO KREIRAN,UKUPAN IZNOS RACUNA-" << UkupanIznos <<" KM"<< endl;
}
void Baza::AzurirajStatusRacuna(int RezervacijaID,string NoviStatus) {
	sqlite3_stmt* stmt;
	string sql =
		"UPDATE Racuni SET StatusRacuna=?;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI AZURIRANJA RACUNA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	sqlite3_bind_text(stmt, 1, NoviStatus.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_int(stmt, 2, RezervacijaID);
	rc = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	if (rc != SQLITE_DONE) {
		cout << "GRESKA PRI AZURIRANJU RACUNA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	cout << "USPJESNO AZURIRAN STATUS RACUNA" << endl;
}
void Baza::AzurirajNacinPlacanja(int RezervacijaID, string NoviNacin) {
	sqlite3_stmt* stmt;
	string sql =
		"UPDATE Racuni SET NacinPlacanja=?;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI AZURIRANJA NACINA PLACANJA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	sqlite3_bind_text(stmt, 1, NoviNacin.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_int(stmt, 2, RezervacijaID);
	rc = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	if (rc != SQLITE_DONE) {
		cout << "GRESKA PRI AZURIRANJU NACINA PLACANJA:" << sqlite3_errmsg(db) << endl;
		return;
	}
	cout << "USPJESNO AZURIRAN NACIN PLACANJA" << endl;
}
void Baza::PrikaziSveRacune() {
	sqlite3_stmt* stmt;
	string sql =
		"SELECT Racuni.ID, Gosti.ImePrezime, Rezervacije.ID, Racuni.DatumIzdavanjaRacuna, Racuni.UkupanIznosRacuna, Racuni.NacinPlacanja, Racuni.StatusRacuna "
		"FROM Racuni "
		"JOIN Rezervacije ON Racuni.RezervacijaID=Rezervacije.ID "
		"JOIN Gosti ON Rezervacije.GostID=Gosti.ID "
		"JOIN Sobe ON Rezervacije.SobaID=Sobe.ID;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PRIKAZA RACUNA:" << sqlite3_errmsg(db) << endl;
	}
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		int ID = sqlite3_column_int(stmt, 0);
		string Ime = (const char*)sqlite3_column_text(stmt, 1);
		int IDRezervacije = sqlite3_column_int(stmt, 2);
		string Datum = (const char*)sqlite3_column_text(stmt, 3);
		double Ukupno = sqlite3_column_double(stmt, 4);
		string Nacin = (const char*)sqlite3_column_text(stmt, 5);
		string Status = (const char*)sqlite3_column_text(stmt, 6);

		cout << "Broj Racuna:" << ID << endl;
		cout << "Ime I Prezime Gosta:" << Ime << endl;
		cout << "Broj Rezervacije:" << IDRezervacije << endl;
		cout << "Datum Izdavanja Racuna:" << Datum << endl;
		cout << "Ukupan Iznos Racuna:" << Ukupno <<" KM"<< endl;
		cout << "Nacin Placanja Racuna:" << Nacin << endl;
		cout << "Status Racuna:" << Status << endl;
		cout << "---" << endl;
	}
	sqlite3_finalize(stmt);
}
void Baza::PrikaziSveRacunePremaStatusu(string Status) {
	sqlite3_stmt* stmt;
	string sql =
		"SELECT Racuni.ID, Gosti.ImePrezime, Rezervacije.ID, Racuni.DatumIzdavanjaRacuna, Racuni.UkupanIznosRacuna, Racuni.NacinPlacanja, Racuni.StatusRacuna "
		"FROM Racuni "
		"JOIN Rezervacije ON Racuni.RezervacijaID=Rezervacije.ID "
		"JOIN Gosti ON Rezervacije.GostID=Gosti.ID "
		"JOIN Sobe ON Rezervacije.SobaID=Sobe.ID "
		"WHERE Racuni.StatusRacuna=?;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI PRIKAZA RACUNA:" << sqlite3_errmsg(db) << endl;
	}
	sqlite3_bind_text(stmt, 1, Status.c_str(), -1, SQLITE_TRANSIENT);
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		int ID = sqlite3_column_int(stmt, 0);
		string Ime = (const char*)sqlite3_column_text(stmt, 1);
		int IDRezervacije = sqlite3_column_int(stmt, 2);
		string Datum = (const char*)sqlite3_column_text(stmt, 3);
		double Ukupno = sqlite3_column_double(stmt, 4);
		string Nacin = (const char*)sqlite3_column_text(stmt, 5);
		string Status = (const char*)sqlite3_column_text(stmt, 6);

		cout << "Broj Racuna:" << ID << endl;
		cout << "Ime I Prezime Gosta:" << Ime << endl;
		cout << "Broj Rezervacije:" << IDRezervacije << endl;
		cout << "Datum Izdavanja Racuna:" << Datum << endl;
		cout << "Ukupan Iznos Racuna:" << Ukupno << " KM" << endl;
		cout << "Nacin Placanja Racuna:" << Nacin << endl;
		cout << "Status Racuna:" << Status << endl;
		cout << "---" << endl;
	}
	sqlite3_finalize(stmt);
}
int Baza::BrojRacuna() {
	sqlite3_stmt* stmt;
	string sql =
		"SELECT COUNT (*) FROM Racuni;";
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK) {
		cout << "GRESKA PRI PRIPREMI BROJANJA RACUNA:" << sqlite3_errmsg(db) << endl;
		return -1;
	}
	int count = 0;
	if (sqlite3_step(stmt) == SQLITE_ROW) {
		count = sqlite3_column_int(stmt, 0);
	}
	sqlite3_finalize(stmt);
	return count;
}