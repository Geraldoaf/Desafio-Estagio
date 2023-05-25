#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>




class Titulo { // Cria a Classe do tipo titulo
public:
    int id;
    std::string nome; 
    float valor_nominal;
    int prazo;
    float juros;

    Titulo(int _id,std::string _nome, float _valor_nominal, int _prazo, float _juros){ // Contrutor da classe
        this->id = _id;
        this->nome = _nome;
        this->valor_nominal = _valor_nominal;
        this->prazo = _prazo;
        this->juros = _juros;
    };
};


std::vector<Titulo> lerTitulosDoCSV(const std::string& nomeArquivo){ 

    std::vector<Titulo> titulos; //Criar vector de objetos do tipo titulo
    std::ifstream inputFile(nomeArquivo);
    if (!inputFile) { //verifica se o arquivo de entrada é valido
        std::cerr << "Erro ao abrir o arquivo." << "\n";
        exit(1);
    }

    std::string linha;
    while (std::getline(inputFile, linha)) {
        std::istringstream iss(linha); 
        std::string campo; // Campo do csv
        std::getline(iss, campo, ','); // Indice/ID
        int id = std::stoi(campo);
        std::getline(iss, campo, ','); // Nome
        std::string nome = campo;

        std::getline(iss, campo, ','); // Valor Nominal
        float valorNominal = std::stof(campo);

        std::getline(iss, campo, ','); // Prazo
        int prazo = std::stoi(campo);

        std::getline(iss, campo, ','); // Juros
        float juros = std::stof(campo);

        Titulo novo(id,nome,valorNominal,prazo,juros); //Cria um novo objeto titulo
        titulos.push_back(novo); // Adiciona pra dentro do vector titulo
    }

    inputFile.close();
    return titulos;
}


int main() {
    std::vector<Titulo> titulos = lerTitulosDoCSV("dados_titulos.csv");
    int id;
    std::cin >> id;
    
    std::cout << "id: " << titulos[id - 1].id << std::endl;
    std::cout << "Nome: " << titulos[id - 1].nome << std::endl;
    std::cout << "Valor Nominal: " << titulos[id - 1].valor_nominal << std::endl;
    std::cout << "Prazo: " << titulos[id - 1].prazo << std::endl;
    std::cout << "Juros: " << titulos[id - 1].juros << std::endl;
    std::cout << std::endl;

    return 0;
}
