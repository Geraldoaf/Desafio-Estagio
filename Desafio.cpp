#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ql/quantlib.hpp>
#include <ql/models/shortrate/onefactormodels/vasicek.hpp>
#include <cstdlib>

using namespace QuantLib;

class Titulo {
public:
    int id;
    std::string nome;
    Real valor_nominal;
    Time prazo;
    Rate juros;
    Rate novoJuros;

    Titulo(int _id,std::string _nome, Real _valor_nominal, Time _prazo, Rate _juros) {
        this->id = _id;
        this->nome = _nome;
        this->valor_nominal = _valor_nominal;
        this->prazo = _prazo;
        this->juros = _juros;
    };
};

class VasicekModel {
public:
    Real kappa;
    Real theta;
    Real sigma;
    Real r0;

    VasicekModel(Real _kappa, Real _theta, Real _sigma, Real _r0){
        this->kappa = _kappa;
        this->theta = _theta;
        this->sigma = _sigma;
        this->r0 = _r0;
    }
};

std::vector<Titulo> lerTitulosDoCSV(const std::string& nomeArquivo) {
    std::vector<Titulo> titulos;
    std::ifstream inputFile(nomeArquivo);
    if (!inputFile) {
        std::cerr << "Erro ao abrir o arquivo." << std::endl;
        exit(1);
    }

    std::string linha;
    while (std::getline(inputFile, linha)) {
        std::istringstream iss(linha);
        std::string campo;
        std::getline(iss, campo, ','); // Indice/ID
        int id = std::stoi(campo);

        std::getline(iss, campo, ','); // Nome
        std::string nome = campo;

        std::getline(iss, campo, ','); // Valor Nominal
        Real valorNominal = std::stof(campo);

        std::getline(iss, campo, ','); // Prazo
        Time prazo = std::stoi(campo);

        std::getline(iss, campo, ','); // Juros
        Rate juros = std::stof(campo);

        Titulo novo(id,nome, valorNominal, prazo, juros);
        titulos.push_back(novo);
    }

    inputFile.close();

    return titulos;
}


Real discountBondOption(const Titulo& titulo, const VasicekModel& model) {
    Real B = (1.0 - std::exp(-model.kappa * titulo.prazo)) / model.kappa;
    Real A = std::exp((model.theta - model.sigma * model.sigma / (2.0 * model.kappa * model.kappa)) * (B - titulo.prazo) - model.sigma * model.sigma / (4.0 * model.kappa) * B * B);
    Real bondPrice = A * std::exp(-model.r0 * B);
    return bondPrice;
}

void infoTitulo(std::vector<Titulo> &titulos ,int id ){
    std::cout << "id: " << titulos[id - 1].id << std::endl;
    std::cout << "Nome: " << titulos[id - 1].nome << std::endl;
    std::cout << "Valor Nominal: " << titulos[id - 1].valor_nominal << std::endl;
    std::cout << "Prazo: " << titulos[id - 1].prazo << std::endl;
    std::cout << "Juros: " << titulos[id - 1].juros << std::endl;
    std::cout << std::endl;
}

void salvarTitulosEmCSV(const std::vector<Titulo>& titulos, const std::string& nomeArquivo) {
    std::ofstream outputFile(nomeArquivo);
    if (!outputFile) {
        std::cerr << "Erro ao criar o arquivo." << std::endl;
        exit(1);
    }

    // Escrever cabeçalho do arquivo
    outputFile << "ID,Nome,Valor Nominal,Prazo,Juros,Novo Juros" << std::endl;

    // Escrever informações dos títulos
    for (const Titulo& titulo : titulos) {
        outputFile << titulo.id << ",";
        outputFile << titulo.nome << ",";
        outputFile << titulo.valor_nominal << ",";
        outputFile << titulo.prazo << ",";
        outputFile << titulo.juros << ",";
        outputFile << titulo.novoJuros << std::endl;
    }

    outputFile.close();
}
void calcularNovosJuros(std::vector<Titulo>& titulos) {
    for (Titulo& titulo : titulos) {
        VasicekModel model(0.1, 0.05, 0.01, titulo.juros);
        Real bondPrice = discountBondOption(titulo, model);
        titulo.novoJuros = 1.0 / bondPrice - 1.0;
    }
}

void Menu(std::vector<Titulo>& titulos) {
    int menu;
    char enter;
    do {
        std::cout << "\tEscolha uma opção:\n";
        std::cout << "\t1 - Lista todos os títulos\n";
        std::cout << "\t2 - Procurar por um título\n";
        std::cout << "\t3 - Calcular novo juros para um título\n"; // Opção adicionada
        std::cout << "\t4 - Calcular novos juros para todos os títulos\n"; // Opção adicionada
        std::cout << "\t0 - Sair\n";
        std::cout << "Opção: ";
        std::cin >> menu;

        switch (menu) {
            case 1:
                std::system("clear");
                // Opção 1: Listar todos os títulos
                for (const Titulo& titulo : titulos) {
                    // Imprimir informações do título
                    std::cout << "ID: " << titulo.id << std::endl;
                    std::cout << "Nome: " << titulo.nome << std::endl;
                    std::cout << "Valor Nominal: " << titulo.valor_nominal << std::endl;
                    std::cout << "Prazo: " << titulo.prazo << std::endl;
                    std::cout << "Juros: " << titulo.juros << std::endl;
                    if(titulo.novoJuros == 0){
                        std::cout << "Novo juro ainda nao foi calculado"<<std::endl;
                    }else{
                        std::cout << "Juros: " << titulo.novoJuros << std::endl;
                    }
                    std::cout << std::endl;
                }
                std::cout<<"Aperte enter para continuar...";
                std::cin.ignore(); // Descartar o caractere de nova linha
                std::cin.get(); // Aguardar a entrada de um caractere
                std::system("clear");
                break;

            case 2:
                // Opção 2: Procurar por um título
                int id;
                std::system("clear");
                std::cout << "Digite o ID do título: ";
                std::cin >> id;

                // Verificar se o ID é válido
                if (id >= 1 && id <= titulos.size()) {
                    // Imprimir informações do título com o ID correspondente
                    infoTitulo(titulos, id);
                } else {
                    std::cout << "ID inválido!" << std::endl;
                }
                std::cout<<"Aperte enter para continuar...";
                std::cin.ignore(); // Descartar o caractere de nova linha
                std::cin.get(); // Aguardar a entrada de um caractere
                std::system("clear");
                break;

            case 3:
                // Opção 3: Calcular novo juros para um título
                std::system("clear");
                std::cout << "Digite o ID do título: ";
                std::cin >> id;

                // Verificar se o ID é válido
                if (id >= 1 && id <= titulos.size()) {
                    Titulo& titulo = titulos[id - 1];
                    VasicekModel model(0.1, 0.05, 0.01, titulo.juros);
                    Real bondPrice = discountBondOption(titulo, model);
                    titulo.novoJuros = 1.0 / bondPrice - 1.0;
                    std::cout << "Novo Juros para o título ID " << id << ": " << titulo.novoJuros << std::endl;
                } else {
                    std::cout << "ID inválido!" << std::endl;
                }
                std::cout << "Aperte enter para continuar...";
                std::cin.ignore(); // Descartar o caractere de nova linha
                std::cin.get(); // Aguardar a entrada de um caractere
                std::system("clear");
                break;
            case 4:
                // Opção 4: Calcular novos juros para todos os títulos
                std::system("clear");
                calcularNovosJuros(titulos);
                std::cout << "Novos juros calculados para todos os títulos." << std::endl;
                std::cout << "Aperte enter para continuar...";
                std::cin.ignore(); // Descartar o caractere de nova linha
                std::cin.get();    // Aguardar a entrada de um caractere
                std::system("clear");
                break;
            case 0:
                // Opção 0: Sair
                std::cout << "Saindo..." << std::endl;
                break;

            default:
                std::cout << "Opção inválida!" << std::endl;
                break;
        }
    } while (menu != 0);
}
int main() {
    std::vector<Titulo> titulos = lerTitulosDoCSV("dados_titulos.csv");
    Menu(titulos);
    salvarTitulosEmCSV(titulos, "titulos_atualizados.csv");

    return 0;
}
