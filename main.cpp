#include <iostream>
#include <conio.h>
#include <string>
#include <vector>
#include <limits>
#include <algorithm>
#include <ctime>
#include <thread>
#include <math.h>
#include <fstream>
#include <numeric>
#include <sstream>
#include <dos.h>
#include <thread>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <system_error>
#include <exception>
#include <filesystem>
using namespace std;
void mainMenu();
void authAdmin();
void authUser();
void meniuAdmin();

void deleteTour(int option, const char* fisier)
{
	fstream toursFile;
	fstream temp;
	toursFile.open(fisier, ios::in);
	temp.open("temp.csv", ios::out);
	try {
		if (!toursFile.is_open())
			throw runtime_error("\nFisierul nu poate fi decshis. Cod " + to_string(errno));
		if (!temp.is_open())
			throw runtime_error("\nFisierul nu poate fi decshis. Cod " + to_string(errno));
	}	//verificare daca fisierul e deschis
	catch (exception& x) {
		cerr << x.what() << "Reincercati. \n\n";
		this_thread::sleep_for(chrono::seconds(1));
		meniuAdmin();
	} //afisare mesaj de eroare si reincercare
	toursFile.clear();
	toursFile.seekg(0, ios::beg);
	int i = 0;
	string line;
	while (getline(toursFile, line))
	{
		i++;
		if (i != option)
		{
			temp << line << endl;
		}
	}//scriere in fisierul temporar a tuturor liniilor din fisierul original, mai putin a celei care trebuie stearsa
	cout << "Cursa a fost stearsa cu succes. \n";
	toursFile.close();
	temp.close();
	exception e;
	try {
		if (remove(fisier) != 0)
			throw e;
		if (rename("temp.csv", fisier) != 0)
			throw e;
	}//stergerea fisierului original si redenumirea fisierului temporar

	catch (exception& e) {
		cerr << e.what() << "\n";
		this_thread::sleep_for(chrono::seconds(1));
		meniuAdmin();
	}

} //functie de stergere a unei curse
void deleteTourUser(int option, const char* fisier, string email)
{
	fstream toursFile;
	fstream temp;
	toursFile.open(fisier, ios::in);
	temp.open("temp.csv", ios::out);
	try {
		if (!toursFile.is_open())
			throw runtime_error("\nFisierul nu poate fi decshis. Cod " + to_string(errno));
		if (!temp.is_open())
			throw runtime_error("\nFisierul nu poate fi decshis. Cod " + to_string(errno));
	} //verificare daca fisierul e deschis
	catch (exception& x) {
		cerr << x.what() << " Reincercati. \n\n";
		this_thread::sleep_for(chrono::seconds(1));
		return;
	}//afisare mesaj de eroare si reincercare
	toursFile.clear();
	toursFile.seekg(0, ios::beg); //mutarea cursorului la inceputul fisierului
	int i = 0;
	string line;
	while (getline(toursFile, line)) //citirea fiecarei linii din fisier
	{
		istringstream iss(line);
		string mail;
		getline(iss, mail, ',');
		if (mail == email)//verificare daca linia curenta contine adresa de email a utilizatorului curent
		{
			i++;
			if (i != option)
			{
				temp << line << endl;
			}
		}
		else temp << line << endl;
	}//scriere in fisierul temporar a tuturor liniilor din fisierul original, mai putin a celei care trebuie stearsa

	toursFile.close();
	temp.close();
	if (toursFile.is_open())
		toursFile.close(); //inchiderea fisierelor
	if (temp.is_open())
		temp.close(); //inchiderea fisierelor
	exception e;
	try {
		if (remove(fisier) != 0)
			throw runtime_error("Fisierul nu se poate sterge. Cod " + to_string(errno));
		if (rename("temp.csv", fisier))
			throw runtime_error("Fisierul nu se poate redenumi. Cod " + to_string(errno));
	}//stergerea fisierului original si redenumirea fisierului temporar

	catch (exception& e) {
		cerr << e.what() << "\n";
		this_thread::sleep_for(chrono::seconds(3));
		return;
	}//afisare mesaj de eroare si reincercare
	cout << "Cursa a fost stearsa cu succes. \n";
}//functie de stergere a unei rezervari a unui utilizator
int checkPassword(string password)
{
	int ok = 0;
	int ok1 = 0;
	int ok2 = 0;
	int ok3 = 0;
	int ok4 = 0;
	if (password.length() >= 6) //verificare daca parola are cel putin 6 caractere
		ok4 = 1;

	for (int i = 0; i < password.length(); i++)
	{
		if (password[i] >= 'a' && password[i] <= 'z')//verificare daca parola contine cel putin o litera mica
			ok = 1;
		if (password[i] >= 'A' && password[i] <= 'Z')//verificare daca parola contine cel putin o litera mare
			ok1 = 1;
		if (password[i] >= '0' && password[i] <= '9')//verificare daca parola contine cel putin un numar
			ok2 = 1;
		if (password[i] == '@' || password[i] == '#' || password[i] == '$' || password[i] == '%' || password[i] == '&' || password[i] == '*' || password[i] == '!' || password[i] == '?')
			ok3 = 1;
	}//verificare daca parola este conforma cu cerintele
	try {
		if (ok == 0 || ok1 == 0 || ok2 == 0 || ok3 == 0 || ok4 == 0)
			throw runtime_error("\nParola trebuie sa contina cel putin 6 caractere, dintre care: o litera mica, o litera mare, un numar si unul din caracterele \"!@#$%&*?\". Reincercati. \n");
	}
	catch (exception& x) {
		return 1;
	}
	return 0;
}//functie de verificare a parolei
int checkDestination(string destination) {
	int ok = 0;
	try {

		for (int i = 0; i < destination.length(); i++)
		{
			if (destination[i] >= 'a' && destination[i] <= 'z')
				ok = 1;
			else if (destination[i] >= 'A' && destination[i] <= 'Z')
				ok = 1;
			else if (destination[i] == ' ' || destination[i] == '-' || destination[i] == '.')
				ok = 1;
			else if (destination[i] > ' ' && destination[i] < '-')
				throw runtime_error("Destinatia nu este valida. Reincercati. " + to_string(errno));
			else if (destination[i] > '.' && destination[i] < 'A')
				throw runtime_error("Destinatia nu este valida. Reincercati. " + to_string(errno));
			else if (destination[i] > 'Z' && destination[i] < 'a')
				throw runtime_error("Destinatia nu este valida. Reincercati. " + to_string(errno));
			else if (destination[i] > 'z')
				throw runtime_error("Destinatia nu este valida. Reincercati. " + to_string(errno));
		}//verificare daca destinatia este conforma cu cerintele

		if (ok == 0)
			throw runtime_error("Destinatia nu este valida. Reincercati. " + to_string(errno));
	}
	catch (exception& x) {
		return 2;
	}
	return 0;
}//functie de verificare a destinatiei. aceasta functie poate fi accesata doar de administrator, intrucat doar acesta poate adauga curse
int checkDate(string date)
{
	int ok = 0;
	try {
		for (int i = 0; i < date.length(); i++)
		{
			if ((date[i] >= '0' && date[i] <= '9') || date[i] == '.')
				ok = 1;
			else
				throw 1;
		}
	}

	catch (int x) {
		return 1;
	}
}//verificare daca data este de forma DD.MM.YYYY si returneaza eroare in caz contrar
class User
{
protected:
	string name;
	string email;
	string password;
public:
	friend void meniuUser(string name, string email);
	friend void authAdmin();
	friend void authUser();
	void setName()
	{
		string verify;
		cin.clear();
		cin.ignore();
		cout << "Introdu numele: ";
		getline(cin, name);
	} //functie de introducere a numelui cu spatii
	void setEmail() {
		do {
			cout << "Introdu adresa de e-mail: ";
			cin >> email;
			/*if (stoi(email) == 0)
				break;*/
			if (email.find('@') == string::npos)
				cout << "Adresa de e-mail trebuie sa contina @. Reincercati. \n";
		} while (email.find('@') == string::npos);
	}//functie de introducere a adresei de email, cu verificare daca adresa contine @
	void setPassword() { //functie de introducere a parolei, cu verificare si cu afisare * in loc de caractere
		char ch;
		int w = 1;
		string pwd = "";
		do
		{
			int p = 1;
			do
			{
				cout << "Introdu parola: ";
				pwd = "";
				while ((ch = _getch()) != '\r') {
					if (ch == '\b') {
						if (pwd.length() >= 1)
						{
							pwd.pop_back();
							cout << "\b \b";
						}
					}
					else
					{
						cout << '*';
						pwd += ch;
					}
				}//afisare * in loc de caractere

				if (checkPassword(pwd) == 1)
					cout << "\nParola trebuie sa contina cel putin 6 caractere, dintre care: o litera mica, o litera mare, un numar si unul din caracterele \"!@#$%&*?\". Reincercati. \n";
			} while (checkPassword(pwd) == 1); //repeta pana cand parola e valida

			cout << "\nReintrodu parola: ";
			string verify = "";
			while ((ch = _getch()) != '\r') {
				if (ch == '\b') {
					if (verify.length() >= 1)
					{
						verify.pop_back();
						cout << "\b \b";
					}
				}
				else
				{
					cout << '*';
					verify += ch;
				}
			}//reintroducere parola
			try {
				if (verify != pwd)
					throw 1;
			}
			catch (int x) {

				cout << "\nParolele nu coincid. Reincercati. \n";

			}
			if (verify == pwd)
			{
				w = 0;
				password = pwd;
			}
		} while (w == 1); //repeta pana cand parolele coincid

	}
	void authPassword() { //functie de introducere a parolei, cu verificare si cu afisare * in loc de caractere. 
		//Diferenta fata de functia de introducere a parolei este ca aceasta functie nu verifica parola de 2 ori
		char ch;
		int w = 1;
		string pwd = "";
		int p = 1;
		do
		{
			cout << "Introdu parola: ";
			pwd = "";
			while ((ch = _getch()) != '\r') {
				if (ch == '\b') {
					if (pwd.length() >= 1)
					{
						pwd.pop_back();
						cout << "\b \b";
					}
				}
				else
				{
					cout << '*';
					pwd += ch;
				}
			}

			if (checkPassword(pwd) == 1)
				cout << "\nParola trebuie sa contina cel putin 6 caractere, dintre care: o litera mica, o litera mare, un numar si unul din caracterele \"!@#$%&*?\". Reincercati. \n";
		} while (checkPassword(pwd) == 1); //repeta pana cand parola e valida
		password = pwd;
	}
	void print() { //functie de afisare a datelor utilizatorului, folosita pentru testare
		cout << "Name: " << this->name << endl;
		cout << "Email: " << this->email << endl;
		cout << "Password: " << this->password << endl;
	}
	~User() { //destructor
		cout << "Destructor called" << endl;
	}

};
class date {
private:
	time_t currentTime;
	struct tm today;
	int Y;
	int M;
	int D;
public:
	friend void meniuAdmin();
	friend void meniuUser(string name, string email);
	date() {
		time(&currentTime);
		localtime_s(&today, &currentTime);
		Y = today.tm_year + 1900;
		M = today.tm_mon + 1;
		D = today.tm_mday;
	}

	~date()
	{
		cout << "destructor folosit cu succes. rest in peace \n";
	}
};//clasa pentru data curenta
class rsa {
private:
	unsigned long long int private_key = 1892699;
	unsigned long long int n = 4329229561;
	unsigned long long int public_key = 2287;
public:

	//algoritmul de gasire a cheilor. nu e nevoie sa fie apelat, am pus aici doar pentru a arata cum am gasit cheile
	/*void setkeys() {
		unsigned long long int prime1 = 6983;
			unsigned long long int prime2 = 619967;

		n = prime1 * prime2;
		unsigned long long int fi = (prime1 - 1) * (prime2 - 1);
		unsigned long long int e, d, k;
		unsigned long long int min_d = 0, min_e = 0;
		unsigned long long int min_sum = n; // Set an initial high value for comparison
		e = 2;
		min_d = (fi + 1) / e;
		for (e = 2; e < fi/1000; e++) {
			if (gcd(e, fi) == 1) {
				k = 1;
				while (1)
				{
					if (((k * fi + 1) /e) < min_d)
						if ((k * fi + 1) % e == 0)
						{
							d = (k * fi + 1) / e;
							// Check if the sum of d and e is smaller than the current minimum
							if (d + e < min_sum)
							{
								min_d = d;
								min_e = e;
								min_sum = d + e;
							}
							break;
						}
					else break;
					k++;
				}
			}
		}

		public_key = min_e;
		private_key = min_d;
		cout << "public key: " << public_key << endl;
		cout << "private key: " << private_key << endl;
		cout<< "n: " << n << endl;
	}*/
	// to encrypt the given number
	unsigned long long int encrypt(unsigned long long int message)
	{
		unsigned long long int e = public_key;
		unsigned long long int encrpyted_text = 1;
		while (e--) {
			encrpyted_text *= message;
			encrpyted_text = encrpyted_text % n;
		}
		return encrpyted_text;
	}
	// to decrypt the given number
	unsigned long long int decrypt(unsigned long long int encrpyted_text)
	{
		unsigned long long int d = private_key;
		unsigned long long int decrypted = 1;
		while (d--) {
			decrypted *= encrpyted_text;
			decrypted = decrypted % n;
		}
		return decrypted;
	}


	// first converting each character to its ASCII value and
// then encoding it then decoding the number to get the
// ASCII and converting it to character
	vector<unsigned long long int> encoder(string message)
	{
		vector<unsigned long long int> form;
		// calling the encrypting function in encoding function
		for (auto& letter : message)
		{
			form.push_back(encrypt((int)letter));
		}


		return form;
	}
	string decoder(vector<unsigned long long int> encoded)
	{
		string s;
		// calling the decrypting function decoding function
		for (auto& num : encoded)
		{
			cout << decrypt(num) << " ";
			s += decrypt(num);
		}
		return s;
	}
	~rsa() {
		cout << "Destructor called" << endl;
	}
};//clasa pentru criptarea parolei
void meniuUser(string name, string email) { //meniul utilizatorului
	system("cls"); //stergere ecran
	cout << "Bine ai revenit, " << name << "  \n Selecteaza optiunea dorita : \n 1. Rezervare bilet \n 2. Anulare rezervare \n 3. Afisare rezervari \n 4. Log out \n 5. Stergere cont\n 9. Revenire la meniul principal \n 0. Iesire \n";
	int option;
	User currentUser;
	cin >> option;
	if (cin.fail())
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cerr << "Optiunea nu este valida. Reincercati. \n";
		this_thread::sleep_for(chrono::seconds(1));
		meniuUser(name, email);
	}//verificare daca optiunea introdusa este integer. daca e caracter reincearca
	switch (option)
	{
	case 0: {
		cout << "Multumim ca ati folosit aplicatia noastra. La revedere! \n";
		break;
	}//iesire din program
	case 1: //cazul in care utilizatorul doreste sa isi rezerve un bilet
	{
		string destination;
		string date;
		int check = 0;
		int k = 0;
		int number_of_tickets;
		fstream toursFile;
		toursFile.open("curse.csv", ios::in);
		try {
			if (!toursFile.is_open())
				throw runtime_error("\nFisierul nu poate fi decshis. Cod " + to_string(errno));
		}//verificare daca fisierul e deschis
		catch (exception& x) {
			cerr << x.what() << " Reincercati. \n\n";
			this_thread::sleep_for(chrono::seconds(1));
			//afisare mesaj de eroare timp de 1 secunda si reincercare
			meniuUser(name, email);
		}
		if (toursFile.peek() == EOF)
		{
			cout << "Nu exista curse disponibile. \n";
			this_thread::sleep_for(chrono::seconds(1));
			toursFile.close();
			meniuUser(name, email);
		}//cazul in care nu exista curse disponibile
		else
		{
			cout << "Cursele disponibile sunt: \n";
			while (toursFile.good())
			{
				k++;
				string line;
				if (getline(toursFile, line))
					cout << k << ". " << line << endl;
			}
		}//afisare pe ecran toate cursele disponibile
		toursFile.clear();//stergerea flagurilor de eroare
		toursFile.seekg(0, ios::beg);//mutarea cursorului la inceputul fisierului
		bool control = true;
		while (control == true)
			//aceasta functie primeste de la utilizator numarul cursei pe care acesta o doreste. 
			//daca numarul introdu este mai mare decat numarul de curse, se afiseaza un mesaj de eroare si se cere reintroducerea numarului
		{
			cout << "Introdu numarul cursei pe care vrei sa o rezervi sau 0 pentru anulare: ";
			int option;
			cin >> option;
			if (cin.fail())
			{
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cerr << "Optiunea nu este valida. Reincercati. \n";
				this_thread::sleep_for(chrono::seconds(1));
				meniuUser(name, email);
			}//verificare daca optiunea introdusa este integer. daca e caracter reincearca
			int i = 0;
			if (option == 0)
				meniuUser(name, email);
			string line;
			if (option != 0)
				if (option < k)
				{
					while (getline(toursFile, line))
					{
						i++;
						if (i == option)
						{
							istringstream iss(line);
							getline(iss, destination, ',');
							getline(iss, date, ',');
							cout << destination << " " << date << endl;
							control = false;
							break;
						}
					}//afisare pe ecran destinatia si data cursei selectate
				}
				else {
					cerr << "Optiunea selectata este mai mare decat numarul de curse. Reincercati. \n";
				}
		}
		fstream rezervariFile;
		rezervariFile.open("rezervari.csv", ios::app);
		try {
			if (!rezervariFile.is_open())
				throw runtime_error("\nFisierul nu poate fi decshis. Cod " + to_string(errno));
		}//verificare daca fisierul e deschis
		catch (exception& x) {
			cerr << x.what() << " Reincercati. \n\n";
			this_thread::sleep_for(chrono::seconds(1));
			meniuUser(name, email);
		}//verificare daca fisierul e deschis
		toursFile.close();
		do {
			try {
				cout << "Introdu numarul de bilete: ";
				cin >> number_of_tickets;
				if (cin.fail()) {
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					throw invalid_argument("Numarul de bilete nu este valid. Reincercati. \n");
				}
				else check = 1;
			}
			catch (exception& e) {
				cerr << e.what() << endl;
			}
		} while (check == 0);//verificare daca numarul de bilete este integer. daca e caracter reincearca pana cand se introduce un integer
		rezervariFile << email << "," << destination << "," << date << "," << number_of_tickets << "\n";//scriere in fisierul de rezervari a datelor introduse de utilizator
		try {
			if (rezervariFile.fail())
				throw runtime_error("\nRezervarea nu a putut fi scrisa in fisier. Cod " + to_string(errno));
			else
				if (number_of_tickets == 1)
					cout << "Rezervarea a fost efectuata cu succes. \n";
				else cout << "Rezervarile au fost efectuate cu succes. \n";
		}
		catch (exception& x) {
			cerr << x.what();

		}

		this_thread::sleep_for(chrono::seconds(1));
		rezervariFile.close();
		cout << endl;
		meniuUser(name, email);
		break;
	}
	case 2: { //functie prin care utilizatorul poate anula o rezervare
		string destination;
		string data;
		string line;
		date currentTime;
		date inputTime;
		string currentEmail;
		int check = 0;
		string number_of_tickets;
		cout << "Rezervarile tale sunt: \n";
		fstream rezervariFile;
		rezervariFile.open("rezervari.csv", ios::in);
		try {
			if (!rezervariFile.is_open())
				throw filesystem::filesystem_error("Fisierul nu poate fi decshis. Cod ", error_code());;
		}//verificare daca fisierul e deschis
		catch (exception& e) {
			cerr << e.what() << " Reincercati. \n\n";
			this_thread::sleep_for(chrono::seconds(1));
			meniuUser(name, email);
		}
		int k = 0;
		bool control = true;
		while (control == true) //aceasta functie afiseaza toate rezervarile utilizatorului curent si apoi primeste de la acesta numarul rezervarii pe care acesta o doreste sa o anuleze
			//daca numarul introdus este mai mare decat numarul de rezervari efectuate anterior de acesta, se afiseaza un mesaj de eroare si se cere reintroducerea numarului
			//functia se executa pana cand se executa cu succes stergerea unei rezervari, sau pana cand utilizatorul doreste sa anuleze
			//dupa executarea cu succes a functiei, se revine la meniul userului
		{

			while (getline(rezervariFile, line))
			{
				istringstream iss(line);
				getline(iss, currentEmail, ',');
				getline(iss, destination, ',');
				getline(iss, data, ',');
				getline(iss, number_of_tickets, ',');
				if (email == currentEmail)
				{
					k++;
					cout << k << "." << destination << ", data: " << data << ", numar de bilete: " << stoi(number_of_tickets) << endl;
				}
			}//funtie de afisare pe ecran a tuturor rezervarilor utilizatorului curent. aceasta compara adresa de email cu adresele de email existente in fisierul de rezervari
			//daca adresele coincid, se afiseaza rezervarea. de asemenea, se numara cate rezervari are utilizatorul curent
			if (k == 0)
			{
				cout << "Nu exista rezervari. \n";
				this_thread::sleep_for(chrono::seconds(1));
				break;
			}//cazul in care nu exista rezervari
			cout << "Introdu numarul rezervarii pe care vrei sa o anulezi sau 0 pentru anulare: ";
			rezervariFile.clear();
			rezervariFile.seekg(0, ios::beg);
			int option;
			cin >> option;
			if (cin.fail())
			{
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cerr << "Optiunea nu este valida. Reincercati. \n";
				this_thread::sleep_for(chrono::seconds(1));
				meniuUser(name, email);
			}//verificare daca optiunea introdusa este integer. daca e caracter reincearca
			if (option == 0)
				break;//iesire din functie daca utilizatorul doreste sa anuleze
			k = 0;
			while (getline(rezervariFile, line))
			{
				istringstream iss(line);
				getline(iss, currentEmail, ',');
				getline(iss, destination, ',');
				getline(iss, data, ',');
				getline(iss, number_of_tickets, ',');
				if (email == currentEmail)
				{
					k++;
					if (k == option)
						break;
				}
			}//functie de cautare in fisier a rezervarii selectate, cu scopul prelucrarilor ulterioare
			rezervariFile.close();
			istringstream ss(data);
			string token;
			vector<string> tokens;
			while (getline(ss, token, '.'))
			{
				tokens.push_back(token);
			}//functie de impartire a datei in zi, luna si an
			inputTime.D = stoi(tokens[0]);
			inputTime.M = stoi(tokens[1]);
			inputTime.Y = stoi(tokens[2]);
			try {
				if (inputTime.Y < currentTime.Y)
					throw 1;
				else if (inputTime.Y == currentTime.Y && inputTime.M < currentTime.M)
					throw 1;
				else if (inputTime.Y == currentTime.Y && inputTime.M == currentTime.M && inputTime.D == currentTime.D)
					throw 1;
				else //daca data rezervarii este mai mica decat data curenta, se afiseaza faptul ca rezervarea nu poate fi anulata
					if (option <= k)
					{
						deleteTourUser(option, "rezervari.csv", email);
						control = false;
					}
					else cout << "Optiunea selectata este mai mare decat numarul de curse. Reincercati. \n";
			}
			catch (int x) {
				cerr << "Nu poti anula o rezervare pentru o cursa care a trecut. \n";
			}

		}
		rezervariFile.close();

		this_thread::sleep_for(chrono::seconds(1));
		cout << endl;
		meniuUser(name, email);
		break;
	}
	case 3: { //functie de afisare a rezervarilor efectuate de utilizatorul curent
		string destination;
		string data;
		string line;
		int check = 0;
		string currentEmail;
		string number_of_tickets;
		fstream rezervariFile;
		rezervariFile.open("rezervari.csv", ios::in);
		try {
			if (!rezervariFile.is_open())
				throw filesystem::filesystem_error("Fisierul nu poate fi decshis. Cod ", error_code());
		}//verificare daca fisierul e deschis
		catch (exception& e) {
			cerr << e.what() << " Reincercati. \n\n";
			this_thread::sleep_for(chrono::seconds(1));
			meniuUser(name, email);
		}//afiseaza mesaj de eroare si revenire la meniul principal
		int k = 0;
		cout << "Rezervarile tale sunt: \n";
		while (getline(rezervariFile, line)) //functie de afisare a rezervarilor utilizatorului curent. aceasta compara adresa de email 
			//a utilizatorului curent cu adresa de email din fisierul de rezervari
			//daca adresele coincid, se afiseaza rezervarea
		{
			istringstream iss(line);
			getline(iss, currentEmail, ',');
			getline(iss, destination, ',');
			getline(iss, data, ',');
			getline(iss, number_of_tickets, ',');
			if (email == currentEmail)
			{
				k++;
				cout << k << ". Destinatia " << destination << " in data de " << data << ", numar de bilete: " << stoi(number_of_tickets) << endl;
			}
		}
		rezervariFile.close();
		if (k == 0)
		{
			cout << "Nu exista rezervari. \n";
			this_thread::sleep_for(chrono::seconds(1));
			meniuUser(name, email);
		}//cazul in care nu exista rezervari
		cout << "apasa enter pentru a reveni la meniul principal \n";
		char c;
		cin.clear();
		cin.ignore();
		c = cin.get();
		cout << endl;
		meniuUser(name, email);
		break; }
	case 4: { //functie de log out. aceasta fuctie trimite la meniul de autentificare
		authUser();
		break;
	}
	case 5: { //functie de stergere a contului, cu toate datele asociate
		cout << "Esti sigur ca vrei sa iti stergi contul si toate datele asociate? Actiunea este ireversibila\n 1. Da \n 2. Nu \n";
		int option;
		cin >> option;
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cerr << "Optiunea nu este valida. Reincercati. \n";
			this_thread::sleep_for(chrono::seconds(1));
			meniuUser(name, email);
		}//verificare daca optiunea introdusa este integer. daca e caracter reincearca
		switch (option)
		{
		case 1: {//cazul in care utilizatorul doreste sa isi stearga contul, acestuia i se va cere parola pentru confirmare. actiunea este ireversibila

			currentUser.authPassword(); //introducere a parolei pentru confirmare
			fstream userFile;
			fstream rezervariFile;
			string line;
			string nume;
			string mail;
			rsa rsa;
			fstream temp;
			string destination;
			string data;
			string number_of_tickets;
			string encryptedpwd;
			userFile.open("user.csv", ios::in);
			try {
				if (!userFile.is_open())
					throw  filesystem::filesystem_error("Fisierul nu poate fi decshis. Cod ", error_code());
			}//verificare  daca fisierul cu conturi este deschis
			catch (exception& e) {
				cerr << e.what() << " Reincercati. \n\n";
				this_thread::sleep_for(chrono::seconds(1));
				meniuUser(name, email);
			}
			userFile.clear();
			userFile.seekg(0, ios::beg);
			while (getline(userFile, line)) //functie de cautare a parolei utilizatorului curent in fisierul de conturi. daca userul este gasit, 
				//se verifica daca parola introdusa este corecta. daca parola este corecta, se sterge contul. daca nu, se afiseaza un mesaj de eroare
			{
				istringstream iss(line);
				vector<unsigned long long int> storedPwd;
				getline(iss, nume, ',');
				getline(iss, mail, ',');
				unsigned long long int number;
				string numbersStr;
				while (iss >> number)
				{
					storedPwd.push_back(number);
					if (iss.peek() == ' ')
						iss.ignore();
				}//functie de citire a parolei din fisier
				if (email == mail)
				{

					vector<unsigned long long int> currentPwd = rsa.encoder(currentUser.password);
					try {
						if (currentPwd == storedPwd)
						{//verificare daca parolele coincid
							cout << "\nAutentificare reusita. Contul se sterge \n\n";
							break;
						}
						else throw 1;
					}
					catch (int x) {
						if (x == 1)
							cerr << "\nparola incorecta. reincercati. \n\n";
						this_thread::sleep_for(chrono::seconds(1));
						meniuUser(name, email);
					}
				}
			}
			userFile.clear();
			userFile.seekg(0, ios::beg);
			temp.open("temp.csv", ios::out);
			try {
				if (!temp.is_open())
					throw runtime_error("\nFisierul nu este deschis. Cod " + to_string(errno));
			}
			catch (exception& e) {
				cerr << e.what() << "\n";
				this_thread::sleep_for(chrono::seconds(3));
				meniuUser(name, email);
			}
			rezervariFile.open("rezervari.csv", ios::in);
			try {
				if (!rezervariFile.is_open())
					throw runtime_error("\nFisierul nu este deschis. Cod " + to_string(errno));
			}
			catch (exception& e) {
				cerr << e.what() << "\n";
				this_thread::sleep_for(chrono::seconds(3));
				meniuUser(name, email);
			}
			while (getline(rezervariFile, line))
			{
				istringstream iss(line);
				getline(iss, mail, ',');
				if (email != mail)
					temp << line << "\n";
			}//functie de stergere a rezervarilor utilizatorului curent din fisierul de rezervari
			rezervariFile.close();
			if (rezervariFile.is_open())
				rezervariFile.close();
			temp.close();
			if (temp.is_open())
				temp.close();
			try {
				if (remove("rezervari.csv") != 0)
					throw runtime_error("Fisierul nu se poate sterge. Cod " + to_string(errno));
				if (rename("temp.csv", "rezervari.csv") != 0)
					throw runtime_error("Fisierul nu se poate redenumi. Cod " + to_string(errno));
			}
			catch (exception& e) {
				cerr << e.what() << "\n";
				this_thread::sleep_for(chrono::seconds(3));
				return;
			}
			temp.open("temp.csv", ios::out);
			while (getline(userFile, line))
			{
				istringstream iss(line);
				getline(iss, nume, ',');
				getline(iss, mail, ',');

				if (mail != email)
					temp << line << "\n";
			}//functie de stergere a contului utilizatorului curent din fisierul de conturi
			userFile.close();
			temp.close();
			if (temp.is_open())
				temp.close();
			if (userFile.is_open())
				userFile.close();
			try {
				if (remove("user.csv") != 0)
					throw runtime_error("Fisierul nu se poate sterge. Cod " + to_string(errno));
				if (rename("temp.csv", "user.csv") != 0)
					throw runtime_error("Fisierul nu se poate redenumi. Cod " + to_string(errno));
			}
			catch (exception& e) {
				cerr << e.what() << "\n";
				this_thread::sleep_for(chrono::seconds(3));
				return;
			}
			cout << "Contul a fost sters cu succes. \n";
			this_thread::sleep_for(chrono::seconds(1));
			mainMenu();
			break;
		}
		default: { //cazul in care utilizatorul nu doreste sa isi stearga contul
			meniuUser(name, email);
			break;
		}
		}}
	case 9: { //revenire la meniul de autentificare
		return;
		break;
	}

	default: { //cazul in care optiunea este integer, dar nu se afla in lista de optiuni
		cout << "Optiunea nu este valida. Reincercati. \n";
		meniuUser(name, email);
		break;
	}
	}
}
void meniuAdmin() {
	system("cls");
	cout << "introdu cifra corespunzatoare actiunii dorite: \n 1. adaugare cursa \n 2. stergere cursa \n 3. afisare rezervari existente \n 4. log out \n";
	int option;
	rsa rsa;
	fstream adminFile;
	User currentUser;
	cin >> option;
	if (cin.fail())
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cerr << "Optiunea nu este valida. Reincercati. \n";
		this_thread::sleep_for(chrono::seconds(1));
		meniuAdmin();
	}//verificare daca optiunea introdusa este integer. daca e caracter reincearca
	switch (option)
	{
	case 1: { //functie de adaugare a  unei curse
		string destination;
		string data;
		date inputTime;
		fstream toursFile;
		int ok;
		int check = 0;
		do {
			ok = 1;
			cout << "Introdu destinatia: ";
			cin.clear();
			cin.ignore();
			getline(cin, destination);
			try {
				if (checkDestination(destination) == 2)
					throw invalid_argument("Destinatia nu este valida. Reincercati. \n");
			}
			catch (exception& x) {
				cerr << x.what();
				ok = 0;
			}
		} while (ok == 0); //introdu destinatia. repeta pana cand destinatia e valida
		ok = 0;
		while (ok == 0) //verificare daca data introdusa este valida. daca nu, se afiseaza un mesaj de eroare si se cere reintroducerea datei
		{
			ok = 1;
			cout << "Introdu data (DD.MM.YYYY): ";
			cin.clear();
			cin >> data;
			try {
				if (data.length() != 10)
					throw invalid_argument("Data nu este valida. Reincercati. Formatul acceptat este DD.MM.YYYY \n");
				else if (data[2] != '.' || data[5] != '.')
					throw invalid_argument("Data nu este valida. Reincercati. Formatul acceptat este DD.MM.YYYY \n");
				else if (checkDate(data) == 1)
					throw invalid_argument("Data nu este valida. Reincercati. Formatul acceptat este DD.MM.YYYY \n");
			}
			catch (exception& x) {
				ok = 0;
				cerr << x.what();
			}
			if (ok == 1)//verificare daca data introdusa este in trecut fata de data curenta, sau nu are formatul calendarului gregorian. 
				//daca nu, se afiseaza un mesaj de eroare si se cere reintroducerea datei
			{
				date currentTime;
				istringstream ss(data);
				string token;
				vector<string> tokens;
				while (getline(ss, token, '.'))
				{
					tokens.push_back(token);
				}
				inputTime.D = stoi(tokens[0]);
				inputTime.M = stoi(tokens[1]);
				inputTime.Y = stoi(tokens[2]);
				try {
					if (inputTime.M < 1 || inputTime.M>12)
						throw 2;
					else
						if (inputTime.M == 1 || inputTime.M == 3 || inputTime.M == 5 || inputTime.M == 7 || inputTime.M == 8 || inputTime.M == 10 || inputTime.M == 12)
							if (inputTime.D > 31 || inputTime.D < 1)
								throw 2;
							else if (inputTime.M == 4 || inputTime.M == 6 || inputTime.M == 9 || inputTime.M == 11)
								if (inputTime.D > 30 || inputTime.D < 1)
									throw 2;
					if (inputTime.Y % 4 == 0)
					{
						if (inputTime.M == 2)
						{
							if (inputTime.D > 29 || inputTime.D < 1)
								throw 2;
						}
						else
						{
							if (inputTime.D > 28 || inputTime.D < 1)
								throw 2;
						}
					}
					if (inputTime.Y < currentTime.Y)
						throw 1;
					else if (inputTime.Y == currentTime.Y && inputTime.M < currentTime.M)
						throw 1;
					else if (inputTime.Y == currentTime.Y && inputTime.M == currentTime.M && inputTime.D < currentTime.D)
						throw 1;
				}
				catch (int x) {
					if (x == 1)cerr << "Data se afla in trecut. Agentia noastra nu dispune de transport cu DMC DeLorean\n\n";
					if (x == 2)cerr << "Data introdusa " << inputTime.D << "." << inputTime.M << "." << inputTime.Y << " nu este din calendarul gregorian. Reincercati. \n";
					ok = 0;
				}
			}
		}
		toursFile.open("curse.csv", ios::app);
		try {
			if (!toursFile.is_open())
				throw runtime_error("\nFisierul nu este decshis. Cod " + to_string(errno));
		}
		catch (exception& x) {
			cerr << x.what() << "  Reincercati. \n\n";
			this_thread::sleep_for(chrono::seconds(1));
			meniuAdmin();
		}//verificare daca fisierul e deschis
		toursFile << destination << "," << data << "\n";
		cout << "Cursa a fost adaugata cu succes. \n";
		this_thread::sleep_for(chrono::seconds(2));
		toursFile.close();
		cout << endl;
		meniuAdmin();
		break;

	}
	case 2: { //functie de stergere a unei curse
		while (1) {
			int k = 0;
			fstream toursFile;
			toursFile.open("curse.csv", ios::in | ios::out);
			try {
				if (!toursFile.is_open())
					throw runtime_error("\nFisierul nu este decshis. Cod " + to_string(errno));
			}
			catch (exception& x) {
				cerr << x.what() << " Reincercati. \n\n";
				this_thread::sleep_for(chrono::seconds(1));
				meniuAdmin();
			}
			if (toursFile.peek() == EOF)
			{
				cout << "Nu exista curse disponibile. \n";
				this_thread::sleep_for(chrono::seconds(1));
				meniuAdmin();
			}
			else
			{
			}
			cout << "Cursele disponibile sunt: \n";
			while (toursFile.good())
			{
				k++;
				string line;
				if (getline(toursFile, line))
					cout << k << ". " << line << endl;
			}
			//afisare pe ecran a tuturor curselor disponibile, pana la sfarsitul fisierului. 
			//creste un contor, care va fi folosit ulterior pentru a verifica daca optiunea introdusa este valida
			toursFile.close();
			int option1;
			cout << "Introdu numarul cursei pe care vrei sa o stergi: ";
			cin >> option1;
			if (cin.fail())
			{
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cerr << "Optiunea nu este valida. Reincercati. \n";
				this_thread::sleep_for(chrono::seconds(1));
				meniuAdmin();
			}
			if (option1 == 0)
				break;
			else
				if (option1 <= k)
					deleteTour(option1, "curse.csv");
				else cout << "Optiunea selectata este mai mare decat numarul de curse. Reincercati. \n";
		}//functie de stergere a cursei selectate. se verifica daca optiunea introdusa este valida. daca nu, se afiseaza un mesaj de eroare si se cere reintroducerea optiunii
		meniuAdmin();
		break;
	}
	case 3: { //functie de afisare a rezervarilor existente
		fstream rezervariFile;
		rezervariFile.open("rezervari.csv", ios::in);
		try {
			if (!rezervariFile.is_open())
				throw runtime_error("\nFisierul nu este decshis. Cod " + to_string(errno));
		}
		catch (exception& x) {
			cerr << x.what() << " Reincercati. \n\n";
			this_thread::sleep_for(chrono::seconds(1));
			meniuAdmin();
		}
		int k = 0;
		rezervariFile.peek();
		if (rezervariFile.eof()) //verificare daca fisierul este gol
		{
			cout << "Nu exista rezervari. \n";
			this_thread::sleep_for(chrono::seconds(1));
		}
		else //afisare pe ecran a tuturor rezervarilor existente, pana la sfarsitul fisierului
		{
			cout << "Rezervarile existente sunt: \n";
			while (rezervariFile.good())
			{
				string line;
				k++;
				if (getline(rezervariFile, line))
					cout << k << ". " << line << endl;
			}
			char c;
			cout << "apasa enter pentru a reveni la meniul principal \n";
			cin.clear();
			cin.ignore();
			c = cin.get();
		}
		meniuAdmin();
		break;
	}
	case 4: { //functie de log out. aceasta fuctie trimite la meniul de autentificare
		authAdmin();
		break;
	}
	default: { //cazul in care optiunea este valida, dar nu se afla in lista de optiuni
		cerr << "Optiunea nu este valida. Reincercati. \n";
		this_thread::sleep_for(chrono::seconds(1));
		meniuAdmin();
		break;
	}
	}

}
void authAdmin() {//meniul de autentificare al administratorului
	system("cls");
	User currentUser;
	fstream adminFile;
	cout << "Bine ai revenit, administrator! introdu cifra corespunzatoare actiunii dorite: \n 1. autentificare\n 9. revenire la meniul principal\n 0. Iesire\n";
	int option;
	cin >> option;
	if (cin.fail())
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cerr << "Optiunea nu este valida. Reincercati. \n";
		this_thread::sleep_for(chrono::seconds(1));
		authAdmin();
	}//verificare daca optiunea introdusa este integer. daca e caracter reincearca
	switch (option)
	{
	case 0: {
		cout << "Multumim ca ati folosit aplicatia noastra. La revedere! \n";
		break;
	}//iesire din aplicatie
	case 1: {//functie de autentificare a administratorului
		bool exista = false;
		bool eof = false;
		rsa rsa;
		string email;
		string line;
		currentUser.setEmail();
		currentUser.authPassword();
		adminFile.open("admin.csv", ios::in);
		try {
			if (!adminFile.is_open())
				throw runtime_error("\nEroare la deschiderea fisierului. Cod " + to_string(errno));
		}
		catch (exception& x) {
			cerr << x.what() << " Reincercati. \n\n";
			this_thread::sleep_for(chrono::seconds(1));
			authAdmin();
		}
		adminFile.clear();
		adminFile.seekg(0, ios::beg);
		while (getline(adminFile, line))
		{//functie de cautare a adresei de email a administratorului curent in fisierul de conturi
			istringstream iss(line);

			vector<unsigned long long int> storedPwd;
			getline(iss, email, ',');
			unsigned long long int number;
			string numbersStr;
			while (iss >> number)
			{
				storedPwd.push_back(number);
				if (iss.peek() == ' ')
					iss.ignore();
			}//functie de citire a parolei criptate din fisier
			if (email == currentUser.email)
			{
				exista = true;
				vector<unsigned long long int> currentPwd = rsa.encoder(currentUser.password);
				try {
					if (currentPwd == storedPwd)
					{
						cout << "\nAutentificare reusita. \n\n";
						this_thread::sleep_for(chrono::seconds(1));

						meniuAdmin();
						break;
					}//functie de comparare a parolei introduse cu cea din fisier. daca parolele coincid, se afiseaza un mesaj de autentificare reusita
					else throw 1;
				}
				catch (int x) {
					if (x == 1)
						cerr << "\nparola incorecta. reincercati. \n\n";
					this_thread::sleep_for(chrono::seconds(1));
					authAdmin();
				}
			}
		}
		if (exista == false)
		{
			cerr << "\n!!emailul nu exista in baza de date. reincercati. \n\n";
			this_thread::sleep_for(chrono::seconds(1));
			authAdmin();
		}//cazul in care adresa de email nu exista in fisierul de conturi
		cout << endl;
		break;

	}
	case 9: { //revenire la meniul principal
		mainMenu();
		break;
	}
	default: { //cazul in care optiunea este valida, dar nu se afla in lista de optiuni
		cerr << "Optiunea nu este valida. Reincercati. \n";
		this_thread::sleep_for(chrono::seconds(1));
		break;
	}
	}
}
void authUser() {//meniul de autentificare al utilizatorului
	system("cls");
	User currentUser;
	fstream userFile;
	cout << "Meniu login utilizator\n Selecteaza optiunea dorita \n 1. autentificare \n 2. creare cont\n 9. revenire la meniul principal\n 0. Iesire\n";
	int option;
	cin >> option;
	if (cin.fail())
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cerr << "Optiunea nu este valida. Reincercati. \n";
		this_thread::sleep_for(chrono::seconds(1));
		authUser();
	}
	switch (option)
	{
	case 0: {//iesire din aplicatie
		cout << "Multumim ca ati folosit aplicatia noastra. La revedere! \n";
		break;
	}
	case 1: { //functie de autentificare a utilizatorului
		bool exista = false;
		bool eof = false;
		rsa rsa;
		string email;
		string line;
		string nume;
		currentUser.setEmail();
		/*if (stoi(currentUser.email) == 0)
			break;*/
		currentUser.authPassword();
		userFile.open("user.csv", ios::in);
		try {
			if (!userFile.is_open())
				throw 1;

		}
		catch (int x) {
			cerr << "\nEroare la deschiderea fisierului. Reincercati. \n\n";
			this_thread::sleep_for(chrono::seconds(1));
			authAdmin();
		}
		userFile.clear();
		userFile.seekg(0, ios::beg);
		while (getline(userFile, line))
		{//functie de cautare a adresei de email a utilizatorului curent in fisierul de conturi
			istringstream iss(line);
			vector<unsigned long long int> storedPwd;
			getline(iss, nume, ',');
			getline(iss, email, ',');
			unsigned long long int number;
			string numbersStr;
			while (iss >> number)
			{//functie de citire a parolei criptate din fisier
				storedPwd.push_back(number);
				if (iss.peek() == ' ')
					iss.ignore();
			}
			if (email == currentUser.email)
			{//functie de comparare a parolei introduse cu cea din fisier. daca parolele coincid, se afiseaza un mesaj de autentificare reusita
				exista = true;
				vector<unsigned long long int> currentPwd = rsa.encoder(currentUser.password);
				try {
					if (currentPwd == storedPwd)
					{
						userFile.close();
						cout << "\nAutentificare reusita. \n\n";
						this_thread::sleep_for(chrono::seconds(1));
						meniuUser(nume, email);
						break;
					}
					else throw 1;
				}
				catch (int x) {
					if (x == 1)
						//se afiseaza un mesaj de eroare si se cere reintroducerea parolei
						cerr << "\nparola incorecta. reincercati. \n\n";
					this_thread::sleep_for(chrono::seconds(1));
					authUser();
				}
			}
		}
		if (exista == false)
		{//cazul in care adresa de email nu exista in fisierul de conturi
			cerr << "\n!!emailul nu exista in baza de date. reincercati sau creati cont nou \n\n";
			this_thread::sleep_for(chrono::seconds(1));
			authUser();
		}
		break;
	}
	case 2: { //functie de creare a unui cont nou
		rsa rsa;
		string email;
		string line;
		string nume;
		userFile.open("user.csv", ios::in | ios::app);
		try {
			if (!userFile.is_open())
				throw filesystem::filesystem_error("\nFisierul nu poate fi decshis. Cod ", error_code());
		}//verificare daca fisierul e deschis
		catch (exception& e)
		{

			cerr << e.what() << "Reincercati. \n\n";
			this_thread::sleep_for(chrono::seconds(1));
			authUser();
		}

		currentUser.setName();
		currentUser.setEmail();
		currentUser.setPassword();
		while (getline(userFile, line))
		{//functie de cautare a adresei de email a utilizatorului curent in fisierul de conturi
			istringstream iss(line);
			getline(iss, nume, ',');
			getline(iss, email, ',');
			try {
				if (email == currentUser.email)
					throw 1;
			}
			catch (int x) {
				//se afiseaza un mesaj de eroare deoarece contul este deja existent in sistem si se redirectioneaza utilizatorul catre meniul de autentificare
				cerr << "\n!!emailul exista deja in baza de date. reincercati sau autentificati-va. \n\n";
				this_thread::sleep_for(chrono::seconds(1));
				userFile.close();
				authUser();
			}
		}
		userFile.clear();
		userFile.seekg(0, ios::beg);
		userFile << currentUser.name << "," << currentUser.email << ",";
		vector<unsigned long long int> currentPwd = rsa.encoder(currentUser.password);
		for (auto& p : currentPwd)
			userFile << p << " ";
		userFile << "\n";
		if (userFile.fail())
		{//verificare daca fisierul poate fi scris
			cerr << "\nEroare la scrierea in fisier. Reincercati. \n\n";
			this_thread::sleep_for(chrono::seconds(1));
		}
		else if (!userFile.fail())
		{//afisare mesaj de succes
			cout << "\nCont creat cu succes. \n\n";
			this_thread::sleep_for(chrono::seconds(1));
		}
		userFile.close();
		meniuUser(currentUser.name, currentUser.email);
		break;
	}
	case 9: {//revenire la meniul principal
		mainMenu();
		break;
	}
	default: {//cazul in care optiunea este valida, dar nu se afla in lista de optiuni
		cerr << "Optiunea nu este valida. Reincercati. \n";
		this_thread::sleep_for(chrono::seconds(1));
		break;
	}
	}
}
void mainMenu()
{
	system("cls");
	cout << "Bine ai venit pe aplicatia de rezervare a biletelor de vacanta! \n Selecteaza tipul de utilizator: \n 1. Administrator \n 2. Utilizator \n 0. Iesire \n";
	int option;
	cin >> option;
	if (cin.fail())
	{//verificare daca optiunea introdusa este integer. daca e caracter reincearca
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cerr << "Optiunea nu este valida. Reincercati. \n";
		this_thread::sleep_for(chrono::seconds(1));
		mainMenu();
	}
	switch (option)
	{
	case 0: {//iesire din aplicatie
		cout << "Multumim ca ati folosit aplicatia noastra. La revedere! \n";
		break; }
	case 1://autentificare administrator
	{authAdmin();
	break; }
	case 2: {authUser();//autentificare utilizator
		break;
	}

	default: {cerr << "Optiunea nu este valida. Reincercati. \n";
		this_thread::sleep_for(chrono::seconds(1));
		mainMenu(); }
	}//cazul in care optiunea este valida, dar nu se afla in lista de optiuni
}
int main()
{
	mainMenu();
}
