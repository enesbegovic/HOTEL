#include <httplib.h>
#include <nlohmann/json.hpp>
#include "Baza.h"
using json = nlohmann::json;
int main() {
	Baza baza("Hotel.db");
	baza.KreirajTabelu();
	baza.KreirajIndex();
	httplib::Server server;
	server.Get("/", [](const httplib::Request& req, httplib::Response& res) {
		res.set_content("Server radi!!!", "text/plain");
		});
	server.Get("/sobe", [&baza](const httplib::Request& req, httplib::Response& res) {
		std::vector<Soba>sobe = baza.UzmiSveSobe();
		json rezultat = json::array();
		for (const auto& s : sobe) {
			json js;
			js["ID"] = s.ID;
			js["Broj Sobe"] = s.BrojSobe;
			js["Smjestajni Kapacitet"] = s.Kapacitet;
			js["Cijena Po Danu"] = s.CijenaPoDanu;
			js["Status Sobe"] = s.Status;
			rezultat.push_back(js);
		}
		res.set_content(rezultat.dump(4), "application/json");
		});
	server.Post("/sobe", [&baza](const httplib::Request& req, httplib::Response& res) {
		try {
			json tijelo = json::parse(req.body);
			int BrojSobe = tijelo.at("brojsobe");
			int Kapacitet = tijelo.at("kapacitet");
			int Cijena = tijelo.at("cijena");
			baza.DodajSobu(BrojSobe, Kapacitet, Cijena);
			json Poruka;
			Poruka["Poruka"] = "Soba uspijesno dodana";
			res.status = 201;
			res.set_content(Poruka.dump(4), "application/json");
		}
		catch (const std::exception& e) {
			json Greska;
			Greska["Greska"]=e.what();
			res.status = 400;
			res.set_content(Greska.dump(4), "application/json");
		}
		});
	server.Put("/sobe/:brojsobe/status", [&baza](const httplib::Request& req, httplib::Response& res) {
		try {
			int BrojSobe = std::stoi(req.path_params.at("brojsobe"));
			json tijelo = json::parse(req.body);
			string Status = tijelo.at("status");
			baza.AzurirajStatusSobe(BrojSobe, Status);
			json Poruka;
			Poruka["Poruka"] = "Status sobe uspijesno azuriran";
			res.status = 200;
			res.set_content(Poruka.dump(4), "application/json");
		}
		catch(const std::exception&e){
			json Greska;
			Greska["Greska"] = e.what();
			res.status = 400;
			res.set_content(Greska.dump(4), "application/json");
		}
		});
	server.Get("/sobe/slobodne", [&baza](const httplib::Request& req, httplib::Response& res) {
		std::vector<Soba>sobe = baza.UzmiSveSlobodneSobe("slobodna");
		json rezultat = json::array();
		for (const auto& s : sobe) {
			json js;
			js["ID"] = s.ID;
			js["Broj Sobe"] = s.BrojSobe;
			js["Smjestajni Kapacitet"] = s.Kapacitet;
			js["Cijena Po Danu"] = s.CijenaPoDanu;
			js["Status Sobe"] = s.Status;
			rezultat.push_back(js);
		}
		res.set_content(rezultat.dump(4), "application/json");
		});
	server.Get("/sobe/zauzete", [&baza](const httplib::Request& req, httplib::Response& res) {
		std::vector<Soba>sobe = baza.UzmiSveZauzeteSobe("zauzeta");
		json rezultat = json::array();
		for (const auto& s : sobe) {
			json js;
			js["ID"] = s.ID;
			js["Broj Sobe"] = s.BrojSobe;
			js["Smjestajni Kapacitet"] = s.Kapacitet;
			js["Cijena Po Danu"] = s.CijenaPoDanu;
			js["Status Sobe"] = s.Status;
			rezultat.push_back(js);
		}
		res.set_content(rezultat.dump(4), "application/json");
		});
	server.Get("/sobe/naodrzavanju", [&baza](const httplib::Request& req, httplib::Response& res) {
		std::vector<Soba>sobe = baza.UzmiSveSobeNaOdrzavanju("na odrzavanju");
		json rezultat = json::array();
		for (const auto& s : sobe) {
			json js;
			js["ID"] = s.ID;
			js["Broj Sobe"] = s.BrojSobe;
			js["Smjestajni Kapacitet"] = s.Kapacitet;
			js["Cijena Po Danu"] = s.CijenaPoDanu;
			js["Status Sobe"] = s.Status;
			rezultat.push_back(js);
		}
		res.set_content(rezultat.dump(4), "application/json");
		});
	server.Get("/gosti", [&baza](const httplib::Request& req, httplib::Response& res) {
		std::vector<Gost>gosti = baza.UzmiSveGoste();
		json rezultat = json::array();
		for (const auto& g : gosti) {
			json jg;
			jg["ID"] = g.ID;
			jg["Ime i Prezime"] = g.ImePrezime;
			jg["JMBG"] = g.JMBG;
			jg["E-mail Adresa"] = g.email;
			rezultat.push_back(jg);
		}
		res.set_content(rezultat.dump(4), "application/json");
		});
	server.Get("/rezervacije", [&baza](const httplib::Request& req, httplib::Response& res) {
		std::vector<Rezervacija>rezervacije = baza.UzmiSveRezervacije();
		json rezultat = json::array();
		for (const auto& r : rezervacije) {
			json jr;
			jr["ID"] = r.ID;
			jr["Ime i Prezime Gosta"] = r.Gost;
			jr["Broj Sobe"] = r.BrojSobe;
			jr["Datum Pocetka Rezervacije"] = r.datumPocetka;
			jr["Datum Kraja Rezervacije"] = r.datumKraja;
			jr["Status Rezervacije"] = r.status;
			rezultat.push_back(jr);
		}
		res.set_content(rezultat.dump(4), "application/json");
		});
	server.Get("/rezervacije/nacekanju", [&baza](const httplib::Request& req, httplib::Response& res) {
		std::vector<Rezervacija>rezervacije = baza.UzmiSveRezervacijeNaCekanju("na cekanju");
		json rezultat = json::array();
		for (const auto& r : rezervacije) {
			json jr;
			jr["ID"] = r.ID;
			jr["Ime i Prezime Gosta"] = r.Gost;
			jr["Broj Sobe"] = r.BrojSobe;
			jr["Datum Pocetka Rezervacije"] = r.datumPocetka;
			jr["Datum Kraja Rezervacije"] = r.datumKraja;
			jr["Status Rezervacije"] = r.status;
			rezultat.push_back(jr);
		}
		res.set_content(rezultat.dump(4), "application/json");
		});
	server.Get("/rezervacije/utoku", [&baza](const httplib::Request& req, httplib::Response& res) {
		std::vector<Rezervacija>rezervacije = baza.UzmiSveRezervacijeUToku("u toku");
		json rezultat = json::array();
		for (const auto& r : rezervacije) {
			json jr;
			jr["ID"] = r.ID;
			jr["Ime i Prezime Gosta"] = r.Gost;
			jr["Broj Sobe"] = r.BrojSobe;
			jr["Datum Pocetka Rezervacije"] = r.datumPocetka;
			jr["Datum Kraja Rezervacije"] = r.datumKraja;
			jr["Status Rezervacije"] = r.status;
			rezultat.push_back(jr);
		}
		res.set_content(rezultat.dump(4), "application/json");
		});
	server.Get("/rezervacije/zavrsene", [&baza](const httplib::Request& req, httplib::Response& res) {
		std::vector<Rezervacija>rezervacije = baza.UzmiSveRezervacijeZavrsena("zavrsena");
		json rezultat = json::array();
		for (const auto& r : rezervacije) {
			json jr;
			jr["ID"] = r.ID;
			jr["Ime i Prezime Gosta"] = r.Gost;
			jr["Broj Sobe"] = r.BrojSobe;
			jr["Datum Pocetka Rezervacije"] = r.datumPocetka;
			jr["Datum Kraja Rezervacije"] = r.datumKraja;
			jr["Status Rezervacije"] = r.status;
			rezultat.push_back(jr);
		}
		res.set_content(rezultat.dump(4), "application/json");
		});
	server.Get("/rezervacije/otkazane", [&baza](const httplib::Request& req, httplib::Response& res) {
		std::vector<Rezervacija>rezervacije = baza.UzmiSveRezervacijeOtkazana("otkazana");
		json rezultat = json::array();
		for (const auto& r : rezervacije) {
			json jr;
			jr["ID"] = r.ID;
			jr["Ime i Prezime Gosta"] = r.Gost;
			jr["Broj Sobe"] = r.BrojSobe;
			jr["Datum Pocetka Rezervacije"] = r.datumPocetka;
			jr["Datum Kraja Rezervacije"] = r.datumKraja;
			jr["Status Rezervacije"] = r.status;
			rezultat.push_back(jr);
		}
		res.set_content(rezultat.dump(4), "application/json");
		});
	server.Post("/novigost/novarezervacija", [&baza](const httplib::Request& req, httplib::Response& res) {
		try {
			json tijelo = json::parse(req.body);
			string ImePrezime = tijelo.at("imeprezime");
			string JMBG = tijelo.at("jmbg");
			string Email = tijelo.at("email");
			int BrojSobe = tijelo.at("brojsobe");
			string Pocetak = tijelo.at("pocetak");
			string Kraj = tijelo.at("kraj");
			int novaRezervacijaID=baza.DodajGostaIZakaziRezervaciju(ImePrezime, JMBG, Email, BrojSobe, Pocetak, Kraj);
			json Poruka;
			Poruka["Poruka"] = "Uspijeno dodani gost i rezervacija";
			Poruka["RezervacijaID"] = novaRezervacijaID;
			res.status = 201;
			res.set_content(Poruka.dump(4), "application/json");
		}
		catch (const std::exception& e) {
			json Greska;
			Greska["Greska"] = e.what();
			res.status = 400;
			res.set_content(Greska.dump(4), "application/json");
		}
		});
	server.Put("/rezervacije/:rezervacijaID/:brojsobe/status", [&baza](const httplib::Request& req, httplib::Response& res) {
		try {
			int RezervacijaID = std::stoi(req.path_params.at("rezervacijaID"));
			int BrojSobe = std::stoi(req.path_params.at("brojsobe"));
			json tijelo = json::parse(req.body);
			string Status = tijelo.at("status");
			baza.AzurirajStatusRezervacijeIStatusSobe(RezervacijaID, BrojSobe, Status);
			json Poruka;
			Poruka["Poruka"] = "Uspijesno azurirani status rezrvacije i status sobe";
			res.status = 200;
			res.set_content(Poruka.dump(4), "application/json");
		}
		catch (const std::exception& e) {
			json Greska;
			Greska["Greska"] = e.what();
			res.status = 400;
			res.set_content(Greska.dump(4), "application/json");
		}
		});
	server.Post("/postojecigost/novarezervacija", [&baza](const httplib::Request& req, httplib::Response& res) {
		try {
			json tijelo = json::parse(req.body);
			string JMBG = tijelo.at("jmbg");
			int BrojSobe = tijelo.at("brojsobe");
			string Pocetak = tijelo.at("pocetak");
			string Kraj = tijelo.at("kraj");
		    int novaRezervacijaID=baza.ZakaziRezervacijuPostojecemGostu(JMBG, BrojSobe, Pocetak, Kraj);
			json Poruka;
			Poruka["Poruka"] = "Uspijesno zakazana rezervacije";
			Poruka["RezervacijaID"] = novaRezervacijaID;
			res.status = 201;
			res.set_content(Poruka.dump(4), "application/json");
		}
		catch (const std::exception& e) {
			json Greska;
			Greska["Greska"] = e.what();
			res.status = 400;
			res.set_content(Greska.dump(4), "application/json");
		}
		});
	server.Get("/racuni", [&baza](const httplib::Request& req, httplib::Response& res) {
		std::vector<Racun>racuni = baza.UzmiSveRacune();
		json rezultat = json::array();
		for (const auto& r : racuni) {
			json jr;
			jr["ID"] = r.ID;
			jr["Ime i Prezime Gosta"] = r.Gost;
			jr["Rezervacija ID"] = r.RezervacijaID;
			jr["Datum Izdavanja Racuna"] = r.DatumIzdavanjaRacuna;
			jr["Ukupan Iznos Racuna"] = r.UkupanIznosRacuna;
			jr["Nacin Placanja"] = r.NacinPlacanja;
			jr["Status Racuna"] = r.Status;
			rezultat.push_back(jr);
		}
		res.set_content(rezultat.dump(4), "application/json");
		});
	server.Post("/racuni", [&baza](const httplib::Request& req, httplib::Response& res) {
		try {
			json tijelo = json::parse(req.body);
			int RezervacijaID = tijelo.at("rezervacijaid");
			string Datum = tijelo.at("datum");
			string NacinPlacanja = tijelo.at("placanje");
			baza.KreirajRacun(RezervacijaID, Datum, NacinPlacanja);
			json Poruka;
			Poruka["Poruka"] = "Racun uspijesno kreiran";
			res.status = 201;
			res.set_content(Poruka.dump(4), "application/json");
		}
		catch (const std::exception& e) {
			json Greska;
			Greska["Greska"] = e.what();
			res.status = 400;
			res.set_content(Greska.dump(4), "application/json");
		}
		});
	server.Get("/racuni/neplaceni", [&baza](const httplib::Request& req, httplib::Response& res) {
		std::vector<Racun>racuni = baza.UzmiSveNeplaceneRacune("neplacen");
		json rezultat = json::array();
		for (const auto& r : racuni) {
			json jr;
			jr["ID"] = r.ID;
			jr["Ime i Prezime Gosta"] = r.Gost;
			jr["Rezervacija ID"] = r.RezervacijaID;
			jr["Datum Izdavanja Racuna"] = r.DatumIzdavanjaRacuna;
			jr["Ukupan Iznos Racuna"] = r.UkupanIznosRacuna;
			jr["Nacin Placanja"] = r.NacinPlacanja;
			jr["Status Racuna"] = r.Status;
			rezultat.push_back(jr);
		}
		res.set_content(rezultat.dump(4), "application/json");
		});
	server.Get("/racuni/placeni", [&baza](const httplib::Request& req, httplib::Response& res) {
		std::vector<Racun>racuni = baza.UzmiSvePlaceneRacune("placen");
		json rezultat = json::array();
		for (const auto& r : racuni) {
			json jr;
			jr["ID"] = r.ID;
			jr["Ime i Prezime Gosta"] = r.Gost;
			jr["Rezervacija ID"] = r.RezervacijaID;
			jr["Datum Izdavanja Racuna"] = r.DatumIzdavanjaRacuna;
			jr["Ukupan Iznos Racuna"] = r.UkupanIznosRacuna;
			jr["Nacin Placanja"] = r.NacinPlacanja;
			jr["Status Racuna"] = r.Status;
			rezultat.push_back(jr);
		}
		res.set_content(rezultat.dump(4), "application/json");
		});
	server.Put("/racuni/:rezervacijaid/nacinplacanja", [&baza](const httplib::Request& req, httplib::Response& res) {
		try {
			int RezervacijaID = std::stoi(req.path_params.at("rezervacijaid"));
			json tijelo = json::parse(req.body);
			string NacinPlacanja = tijelo.at("nacinplacanja");
			baza.AzurirajNacinPlacanja(RezervacijaID, NacinPlacanja);
			json Poruka;
			Poruka["Poruka"] = "Uspijesno azuriran nacin placanja";
			res.status = 200;
			res.set_content(Poruka.dump(4), "application/json");
		}
		catch (const std::exception& e) {
			json Greska;
			Greska["Greska"] = e.what();
			res.status = 400;
			res.set_content(Greska.dump(4), "application/json");
		}
		});
	server.Put("/racuni/:rezervacijaid/statusracuna", [&baza](const httplib::Request& req, httplib::Response& res) {
		try {
			int RezervacijaID = std::stoi(req.path_params.at("rezervacijaid"));
			json tijelo = json::parse(req.body);
			string StatusRacuna = tijelo.at("statusracuna");
			baza.AzurirajStatusRacuna(RezervacijaID, StatusRacuna);
			json Poruka;
			Poruka["Poruka"] = "Uspijesno azuriran status racuna";
			res.status = 200;
			res.set_content(Poruka.dump(4), "application/json");
		}
		catch (const std::exception& e) {
			json Greska;
			Greska["Greska"] = e.what();
			res.status = 400;
			res.set_content(Greska.dump(4), "application/json");
		}
		});
	std::cout << "Server pokrenut na http://localhost:8080" << std::endl;
	server.listen("0.0.0.0", 8080);
	return 0;
}