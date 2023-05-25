#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

class Titulo {
public:
    std::string nome;
    float valor_nominal;
    int prazo;
    float juros;

    Titulo(std::string _nome, float _valor_nominal, int _prazo, float _juros){
        this->nome = _nome;
        this->valor_nominal = _valor_nominal;
        this->prazo = _prazo;
        this->juros = _juros;
    };
};




std::vector<Titulo> lerTitulosDoCSV(const std::string& nomeArquivo){
    //
    std::vector<Titulo> titulos;

    std::ifstream inputFile(nomeArquivo);
    if (!inputFile) {
        std::cerr << "Erro ao abrir o arquivo." << "\n";
        exit(1);
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line); 
        std::string field;

        std::getline(iss, field, ','); // Indice/ID
        std::getline(iss, field, ','); // Nome
        std::string nome = field;

        std::getline(iss, field, ','); // Valor Nominal
        float valorNominal = std::stof(field);

        std::getline(iss, field, ','); // Prazo
        int prazo = std::stoi(field);

        std::getline(iss, field, ','); // Juros
        float juros = std::stof(field);

        Titulo novo(nome,valorNominal,prazo,juros);
        titulos.push_back(novo);
        
    }

    inputFile.close();

    return titulos;
}



int main() {
    std::vector<Titulo> titulos = lerTitulosDoCSV("dados_titulos.csv");
    int id;
    std::cin >> id;

    std::cout << "Nome: " << titulos[id - 1].nome << std::endl;
    std::cout << "Valor Nominal: " << titulos[id - 1].valor_nominal << std::endl;
    std::cout << "Prazo: " << titulos[id - 1].prazo << std::endl;
    std::cout << "Juros: " << titulos[id - 1].juros << std::endl;
    std::cout << std::endl;
    

    return 0;
}
