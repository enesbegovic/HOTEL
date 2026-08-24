#include <httplib.h>
#include <nlohmann/json.hpp>
#include "Baza.h"
using json = nlohmann::json;
int main() {
	Baza baza("C:\\Users\\enesb\\source\\repos\\HOTEL\\HOTEL\\Hotel.db");
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
	std::cout << "Server pokrenut na http://localhost:8080" << std::endl;
	server.listen("localhost", 8080);
	return 0;
}