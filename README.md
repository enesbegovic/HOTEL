# HOTEL - Sistem za upravljanje hotelom

Konzolna C++ aplikacija za upravljanje hotelskim poslovanjem, povezana sa SQLite bazom podataka.

## Opis

Aplikacija omogućava upravljanje sobama, gostima, rezervacijama i računima kroz interaktivni konzolni meni.

## Funkcionalnosti

### Sobe
- Dodavanje nove sobe
- Ažuriranje statusa sobe (slobodna, zauzeta, na održavanju)
- Prikaz svih soba
- Prikaz soba prema statusu

### Gosti i Rezervacije
- Dodavanje gosta i zakazivanje rezervacije (transakcija)
- Zakazivanje rezervacije postojećem gostu
- Ažuriranje statusa rezervacije i statusa sobe (transakcija, sa automatskim mapiranjem statusa)
- Prikaz svih gostiju
- Prikaz svih rezervacija (sa JOIN-om)
- Prikaz rezervacija prema statusu (na čekanju, u toku, završena, otkazana)

### Računi
- Kreiranje računa za rezervaciju (automatski izračun iznosa preko julianday() i cijene sobe)
- Ažuriranje statusa računa (plaćen, neplaćen)
- Ažuriranje načina plaćanja (gotovina, kartica)
- Prikaz svih računa (sa JOIN-om)
- Prikaz računa prema statusu

## Tehnologije

- C++ 
- SQLite3 (prepared statements, transakcije, FOREIGN KEY, CHECK constraints, indeksi)
- Visual Studio

## Struktura baze

- **Sobe** — informacije o sobama i njihovom statusu
- **Gosti** — podaci o gostima
- **Rezervacije** — povezuje gosta i sobu, prati period i status boravka
- **Racuni** — povezan sa rezervacijom, prati iznos i status plaćanja

Sve tabele koriste FOREIGN KEY i CHECK constraint za integritet podataka.

## Pokretanje

Otvoriti `HOTEL.sln` u Visual Studiu, buildati (x64/Debug), pokrenuti preko Local Windows Debugger.
## Pokretanje

Otvoriti `HOTEL.sln` u Visual Studiu, buildati (x64/Debug), pokrenuti preko Local Windows Debugger.
