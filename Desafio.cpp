#include <iostream>
#include <vector>
#include <string>
#include <fstream>

class Titulo{
public:
    std::string nome;
    float valor_nominal;
    int prazo;
    float juros;

    Titulo(const std::string _nome,float _valor_nominal,int _prazo, float _juros){
        this->nome = _nome;
        this->valor_nominal = _valor_nominal;
        this->prazo = _prazo;
        this->juros = _juros;
    }
};

int main() {
    std::ifstream inputFile("dados_titulos.csv");
    if (!inputFile) {
        std::cerr << "Erro ao abrir o arquivo." << std::endl;
        return 1;
    }
    std::string line;
    std::vector<std::string> fields;
    std::vector<Titulo> titulos;

    while (std::getline(inputFile, line)) {
        size_t pos = 0;
        std::string value;

        
        while ((pos = line.find(',')) != std::string::npos) {
            value = line.substr(0, pos);
            fields.push_back(value);
            line.erase(0, pos + 1);
        }
        fields.push_back(line);

    }


    
    inputFile.close();
    
    return 0;
}
