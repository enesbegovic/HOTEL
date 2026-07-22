#include<iostream>
#include<string>
#include"Baza.h"
using namespace std;
int main() {
	Baza baza;
	baza.KreirajTabelu();
	baza.KreirajIndex();
	cout << "Gotovo" << endl;
}