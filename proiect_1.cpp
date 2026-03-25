/*

Aplicatia gestioneaza rezervarile unui hotel.

Datele de intrare sunt generate direct in program si consta in camere (numar, pret/noapte, stare de ocupare) si clienti (nume, telefon). 

Aplicatia permite afisarea camerelor existente si realizarea unei rezervari prin alegerea unei camere libere si a unui client deja existent. 
In urma rezervarii camera devine ocupata, iar costul total este calculat si afisat. Programul se poate inchide la alegerea utilizatorului.

*/
#include <iostream>
#include <cstring>
class Camera{
    int numar;
    double pret;
    bool ocupat;
    static int nrCamere;
public:
    Camera();
    Camera(int numar, double pret, bool ocupat);
    Camera(const Camera& c);
    Camera& operator=(const Camera& c);
    ~Camera();

    int getNumar() const { return numar; }
    double getPret() const { return pret; }
    bool getOcupat() const { return ocupat; }
    static int getNrCamere(){ return nrCamere; }
    inline void ocupa(){ ocupat = true; };
    friend std::ostream& operator<<(std::ostream& out, const Camera& c);
};

class Client{
    char nume[50];
    char telefon[20];
public:
    Client();
    Client(const char* nume, const char* telefon);
    Client(const Client& c);
    Client& operator=(const Client& c);
    ~Client();

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
int Camera::nrCamere = 0;
Camera::Camera(){
    numar = 0;
    pret = 0;
    ocupat = false;
    nrCamere++;
}
Camera::Camera(int numar, double pret, bool ocupat){
    this->numar = numar;
    this->pret = pret;
    this->ocupat = ocupat;
    nrCamere++;
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

Client::Client(){
    strcpy(nume, "");
    strcpy(telefon, "");
}
Client::Client(const char* nume, const char* telefon){
        strcpy(this->nume, nume);
        strcpy(this->telefon, telefon);
}
Client::Client(const Client& c){
    strcpy(nume, c.nume);
    strcpy(telefon, c.telefon);
}
Client& Client::operator=(const Client& c){
    if(this != &c){
        strcpy(nume, c.nume);
        strcpy(telefon, c.telefon);
    }
    return *this;
}
Client::~Client(){}
std::ostream& operator<<(std::ostream& out, const Client& c){
    out << "Client -> nume: " << c.nume << "; telefon: " << c.telefon;
    return out;
}

Rezervare::Rezervare(const Client& client, const Camera& camera, int zile){
    this->client = client;
    this->camera = camera;
    this->zile = zile;
}
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
    << "Cost total: " << r.calculeazaCost(r.camera.getPret()) << " lei\n";
    return out;
}

void afiseazaCamere(Camera camere[], int n){
    for(int i = 0; i < n; i++){
        std::cout << i+1 << ". " << camere[i] << "\n";
    }
}
void faRezervare(Camera camere[], int n, Client clienti[], int m){
    int camI, clientI;
    std::cout << "Alege camera: ";
    std::cin >> camI;
    camI--;
    if(camI < 0 || camI >= n){
        std::cout << "Index invalid!\n";
        return;
    }
    if(camere[camI].getOcupat()){
        std::cout << "Camera este deja ocupata!\n";
        return;
    }
    std::cout << "Alege client:\n";
    for(int i = 0; i < m; i++){
        std::cout << i+1 << ". " << clienti[i] << "\n";
    }
    std::cin >> clientI;
    clientI--;
    if(clientI < 0 || clientI >= m){
        std::cout << "Index invalid!\n";
        return;
    }
    Rezervare r(clienti[clientI], camere[camI], 3);
    camere[camI].ocupa();
    std::cout << "\nRezervare realizata!\n";
    std::cout << r;
}
int main(){
    Camera camere[3] = {
        Camera(1, 100, false),
        Camera(2, 150, false),
        Camera(3, 200, true)
    };
    Client clienti[2] = {
        Client("Ionel", "0724592109"),
        Client("Maria", "0712345678")
    };
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
                std::cout << "Numar total camere: " << Camera::getNrCamere() << "\n";
                afiseazaCamere(camere, 3);
                break;
            case 2:
                faRezervare(camere, 3, clienti, 2);
                break;
            case 0:
                break;
            default:
                std::cout << "Optiune invalida!\n";
        }
    }while(optiune != 0);
    return 0;
}

