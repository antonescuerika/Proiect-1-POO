/*

Aplicatia gestioneaza rezervarile unui hotel.

Camerele sunt citite din fisierul "camere.txt", care cuprinde: numarul camerei, pretul/noapte si daca e ocupata sau nu. 
Utilizatorul introduce de la tastatura datele clientului si numarul de zile pentru a realiza o rezervare.

Aplicatia are trei optiuni. Prima optiune este afisarea tuturor camerelor si informatii legate de acestea. 
A doua optiune permite realizarea unei rezervari, unde trebuie introduse date despre client si perioada rezervarii, care vor fi introduse in fisierul "rezervari.txt". 
De asemnea, se calculeaza si costul total al rezervarii si se actualizeaza informatiile despre camere. A treia optiune este cea de iesire din aplicatie.

*/
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
class Camera{
    int numar;
    double pret;
    bool ocupat;
public:
    Camera();
    Camera(int numar, double pret, bool ocupat);
    Camera(const Camera& c);
    Camera& operator=(const Camera& c);
    ~Camera();

    int getNumar() const { return numar; }
    double getPret() const { return pret; }
    bool getOcupat() const { return ocupat; }

    void ocupa(){ ocupat = true; };

    friend std::ostream& operator<<(std::ostream& out, const Camera& c);
    friend std::istream& operator>>(std::istream& in, Camera& c);
};

class Client{
    std::string nume;
    std::string telefon;
public:
    Client();
    Client(const std::string& nume, const std::string& telefon);
    Client(const Client& c);
    Client& operator=(const Client& c);
    ~Client();

    std::string getNume() const { return nume; }
    std::string getTelefon() const { return telefon; }

    friend std::ostream& operator<<(std::ostream& out, const Client& c);
};

class Rezervare{
    Client client;
    Camera camera;
    int zile;
    double calculeazaCostIntern(double pret) const{
        return zile * pret;
    }
public:
    Rezervare(const Client& client, const Camera& camera, int zile);
    Rezervare(const Rezervare& r);
    Rezervare& operator=(const Rezervare& r);
    ~Rezervare();
    double calculeazaCost(double pret) const{
        return calculeazaCostIntern(pret);
    };
    friend std::ostream& operator<<(std::ostream& out, const Rezervare& r);
};

Camera::Camera(){
    numar = 0;
    pret = 0;
    ocupat = false;
}
Camera::Camera(int numar, double pret, bool ocupat){
    this->numar = numar;
    this->pret = pret;
    this->ocupat = ocupat;
}
Camera::Camera(const Camera& c){
    numar = c.numar;
    pret = c.pret;
    ocupat = c.ocupat;
}
Camera& Camera::operator=(const Camera& c){
    if(this != &c){
        numar = c.numar;
        pret = c.pret;
        ocupat = c.ocupat;
    }
    return *this;
}
Camera::~Camera(){}
std::ostream& operator<<(std::ostream& out, const Camera& c){
    out << "Camera -> numar: " << c.numar << "; pret/noapte: " << c.pret << " lei; ocupata: " << (c.ocupat ? "da" : "nu");
    return out;
}
std::istream& operator>>(std::istream& in, Camera& c){
    in >> c.numar >> c.pret >> c.ocupat;
    return in;
}

Client::Client(){
    nume = "";
    telefon = "";
}
Client::Client(const std::string& nume, const std::string& telefon){
    this->nume = nume;
    this->telefon = telefon;
}
Client::Client(const Client& c){
    nume = c.nume;
    telefon = c.telefon;
}
Client& Client::operator=(const Client& c){
    if(this != &c){
        nume = c.nume;
        telefon = c.telefon;
    }
    return *this;
}
Client::~Client(){}
std::ostream& operator<<(std::ostream& out, const Client& c){
    out << "Client -> nume: " << c.nume << "; telefon: " << c.telefon;
    return out;
}

Rezervare::Rezervare(const Client& client, const Camera& camera, int zile): client(client), camera(camera), zile(zile){}
Rezervare::Rezervare(const Rezervare& r): client(r.client), camera(r.camera), zile(r.zile){}
Rezervare& Rezervare::operator=(const Rezervare& r){
    if(this != &r){
        client = r.client;
        camera = r.camera;
        zile = r.zile;
    }
    return *this;
}
Rezervare::~Rezervare(){}
std::ostream& operator<<(std::ostream& out, const Rezervare& r){
    out << "Rezervare:\n" 
    << r.client << "\n" 
    << "Camera -> numar: " << r.camera.getNumar() << "; pret/noapte: " << r.camera.getPret() << " lei; zile: " << r.zile << "\n"
    << "Cost total: " << r.calculeazaCost(r.camera.getPret()) << " lei";
    return out;
}

void incarcaCamere(std::vector<Camera>& camere){
    std::ifstream fin("camere.txt");
    if(!fin){
        std::cout << "Eroare la deschiderea fisierului camere.txt\n";
        return;
    }
    Camera c;
    while(fin >> c){
        camere.push_back(c);
    }
    fin.close();
}
void salveazaCamere(const std::vector<Camera>& camere){
    std::ofstream fout("camere.txt");
    if(!fout){
        std::cout << "Eroare la scrierea fisierului camere.txt\n";
        return;
    }
    for(int i = 0; i < camere.size(); i++){
        fout << camere[i].getNumar() << " " << camere[i].getPret() << " " << camere[i].getOcupat() << "\n";
    }
    fout.close();
}
void afiseazaCamere(const std::vector<Camera>& camere){
    for(int i = 0; i < camere.size(); i++){
        std::cout << camere[i] << "\n";
    }
}
void faRezervare(std::vector<Camera>& camere){
    int i;
    std::cout << "Alege camera: ";
    std::cin >> i;
    i--;
    if(i < 0 || i >= camere.size()){
        std::cout << "Index invalid!\n";
        return;
    }
    if(camere[i].getOcupat()){
        std::cout << "Camera este deja ocupata!\n";
        return;
    }
    std::string nume, telefon;
    int zile;
    std::cout << "Nume client: ";
    std::cin >> nume;
    std::cout << "Telefon: ";
    std::cin >> telefon;
    std::cout << "Numar zile: ";
    std::cin >> zile;
    Client client(nume, telefon);
    Rezervare rezervare(client, camere[i], zile);
    camere[i].ocupa();
    std::ofstream fout("rezervari.txt", std::ios::app);
    if(fout){
        fout << rezervare << "\n";
        fout.close();
    }
    salveazaCamere(camere);
    std::cout << "Rezervare realizata!\n";
    std::cout << "Cost total: " << rezervare.calculeazaCost(camere[i].getPret()) << " lei\n";
}

int main(){
    std::vector<Camera> camere;
    incarcaCamere(camere);
    int optiune;
    do{
        std::cout << "\n------- MENIU -------\n";
        std::cout << "-> Afisare camere (1)\n";
        std::cout << "-> Fa rezervare (2)\n";
        std::cout << "-> Iesire (0)\n";
        std::cout << "Optiune: ";
        std::cin >> optiune;
        switch(optiune){
            case 1:
                afiseazaCamere(camere);
                break;
            case 2:
                faRezervare(camere);
                break;
            case 0:
                break;
            default:
                std::cout << "Optiune invalida!\n";
        }
    }while(optiune != 0);
    return 0;
}
