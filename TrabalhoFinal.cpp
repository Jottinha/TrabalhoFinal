#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <stdbool.h>
#include <map>
#include <string.h>
#include <locale.h>
#include <time.h>

#define DIESEL 1
#define GASOLINA 2
#define ETANOL 3

int priceDisel = 6.04;
int priceGasolina = 6.93;
int priceEtanol = 5.56;
int idClient = 0;
int idGasoline = 0;

typedef struct data{
	int dia;
	int mes;
	int ano;
    int hora;
	int minu;
	int sec;
}Data;

typedef struct gasoline{
	char nameResponsavel[50];
	int idComprador;
	int codigoProduto;
	int litrosAbastecidos;
	Data diaAbastecimento;
}Gasoline;

typedef struct client{
	char name[50];
	char cpf[12];
	bool activate;
	float bill;
}Client;

void newClient(std::map < int, Client> &mapsClient); //criando novo cliente
void newExtrato(std::map <int, Gasoline> &mapsGasoline); // criando um extrato novo quando é feito um novo abastecimento
void sales(std::map <int, Client> &mapsClient, std::map <int, Gasoline> &mapsGasoline); // gerencia as vendas
void gasolina(std::map <int, Client> &mapsClient, int id, std::map <int, Gasoline> &mapsGasoline); //venda de gasolina
void diesel(std::map <int, Client> &mapsClient, int id, std::map <int, Gasoline> &mapsGasoline); //venda diesel
void etanol(std::map <int, Client> &mapsClient, int id, std::map <int, Gasoline> &mapsGasoline); //venda etanol
void dataHoraAutomatica(std::map <int, Gasoline> &mapsGasoline);//cria data e hora de forma automatica
void printExtratos(std::map <int, Gasoline> &mapsGasoline);//printa todos os extratos das compras que foram feitas
void printClients(std::map <int, Client> &mapsClient, bool situationAccount);//lista todos os clientes e suas dividas
void removerCliente(std::map <int, Client> &mapsClient);
void desativarConta(std::map <int, Client> &mapsClient);
int menu();
void alterarPreco();
void apagarTela();
void pagarConta(std::map <int, Client> &mapsClient);
void criandoPessoasTeste(std::map <int, Client> &mapsClient);

int main(){
    setlocale(LC_ALL, "Portuguese");

    Gasoline *extrato = NULL;
	std::map <int, Client> mapsClient;
	std::map <int, Gasoline>mapsGasoline;
    int resposta;

    criandoPessoasTeste(mapsClient);

    alterarPreco();
    getch();
    apagarTela();

    do
    {
        resposta = menu();

        switch(resposta)
        {
            case 1: //Cadastrar novo cliente
                newClient(mapsClient);
                getch();
                apagarTela();
                break;
            case 2: //Listar clientes conta ativa
                printClients(mapsClient, true);
                getch();
                apagarTela();
                break;
            case 3: //Remover Clientes
                removerCliente(mapsClient);
                getch();
                apagarTela();
                break;
            case 4: //Extrato de vendas
                printExtratos(mapsGasoline);
                getch();
                apagarTela();
                break;
            case 5: //Abastecer
                sales(mapsClient, mapsGasoline);
                getch();
                apagarTela();
                break;
            case 6: //Clientes com conta inativas
                printClients(mapsClient, false);
                getch();
                apagarTela();
                break;
            case 7: //Desativar conta cliente
                desativarConta(mapsClient);
                getch();
                apagarTela();
                break;
            case 8: //pagar conta
                pagarConta(mapsClient);
                getch();
                apagarTela();
                break;
            case 9: //sair programa
                break;
            default: //resposta invalida
                std::cout << "Opção invalida" << std::endl;
                std::cout <<"\n\nAperte qualquer tecla para continuar\n";
                getch();
                apagarTela();
                break;
        }
    }while(resposta != 9);
	return 0;
}

void newClient(std::map < int, Client> &mapsClient){

	Client addClient;
	idClient++;
    int id = idClient;

	if(mapsClient.find(id) == mapsClient.end()){

		fflush(stdin);
		std::cout <<"Nome do cliente: ";
		gets(addClient.name);

		std::cout <<"Cpf do cliente: ";
		gets(addClient.cpf);

		addClient.activate = true;
		addClient.bill = 0;
		mapsClient[id] = addClient;

		std::cout <<"Cliente cadastrado com sucesso\n\n\nAperte qualquer tecla para voltar\n";
	}else
	{
		std::cout <<"Erro ao cadastrar novo id!";
	}
}

void newExtrato(std::map <int, Gasoline> &mapsGasoline){

	Gasoline addExtrato;
	idGasoline++;
    int novoId = idGasoline;

    addExtrato.codigoProduto = 0;
    addExtrato.idComprador = novoId;
    addExtrato.litrosAbastecidos = 0;

    mapsGasoline[novoId] = addExtrato;
}

void sales(std::map <int, Client> &mapsClient, std::map <int, Gasoline> &mapsGasoline){

	long int id, resposta;

    fflush(stdin);
	std::cout <<"Qual o id do cliente: ";
	std::cin >> id;

	if(mapsClient.find(id) == mapsClient.end()){
		std::cout <<"Cliente não cadastrado!";
	}else{
        newExtrato(mapsGasoline);
		std::cout<<"(1)Diesel\n(2)Gasolina\n(3)Etanol: \n";
		std::cin >> resposta;

		switch(resposta)
		{
			case DIESEL:
			    diesel(mapsClient, id, mapsGasoline);
				break;
			case GASOLINA :
			    gasolina(mapsClient, id, mapsGasoline);
				break;
			case ETANOL:
			    etanol(mapsClient, id, mapsGasoline);
				break;
			default:
				break;
		}
	}
	std::cout <<"\n\nAperte qualquer tecla para continuar\n";
}

void gasolina(std::map <int, Client> &mapsClient, int id, std::map <int, Gasoline> &mapsGasoline){

	int totalLitro;
	char nomeFrentista[50];
	float totalDevido;

    //Declarando variaveis para a data ser feita de forma automatica
    struct tm *p;
    time_t seconds;

    time(&seconds);
    p = localtime(&seconds);
    //Fim da declarção

    fflush(stdin);
	std::cout <<"Responsavel pelo abastecimento: ";
	gets(mapsGasoline[idGasoline].nameResponsavel);

    fflush(stdin);
	std::cout <<"Abastecer quantos litros: ";
	std::cin >> totalLitro;

    totalDevido = priceGasolina * totalLitro;

    //data correta
    dataHoraAutomatica(mapsGasoline);

    mapsGasoline[idGasoline].codigoProduto = 2;
    mapsGasoline[idGasoline].idComprador = id;
    mapsGasoline[idGasoline].litrosAbastecidos = totalLitro;
    mapsClient[id].bill += totalDevido;

	std::cout << totalDevido <<" R$ adicionado a conta do cliente"<<std::endl;
}

void diesel(std::map <int, Client> &mapsClient, int id, std::map <int, Gasoline> &mapsGasoline){

	int totalLitro;
	char nomeFrentista[50];
	float totalDevido;

    //Declarando variaveis para a data ser feita de forma automatica
    struct tm *p;
    time_t seconds;

    time(&seconds);
    p = localtime(&seconds);
    //Fim da declarção

    fflush(stdin);
	std::cout <<"Responsavel pelo abastecimento: ";
	gets(mapsGasoline[idGasoline].nameResponsavel);

    fflush(stdin);
	std::cout <<"Abastecer quantos litros: ";
	std::cin >> totalLitro;

    totalDevido = priceDisel * totalLitro;

    //data correta
    dataHoraAutomatica(mapsGasoline);

    mapsGasoline[idGasoline].codigoProduto = 1;
    mapsGasoline[idGasoline].idComprador = id;
    mapsGasoline[idGasoline].litrosAbastecidos = totalLitro;
    mapsClient[id].bill += totalDevido;

	std::cout << totalDevido <<" R$ adicionado a conta do cliente"<<std::endl;
}

void etanol(std::map <int, Client> &mapsClient, int id, std::map <int, Gasoline> &mapsGasoline){

	int totalLitro;
	char nomeFrentista[50];
	float totalDevido;

	//Declarando variaveis para a data ser feita de forma automatica
    struct tm *p;
    time_t seconds;

    time(&seconds);
    p = localtime(&seconds);
    //Fim da declarção

    fflush(stdin);
	std::cout <<"Responsavel pelo abastecimento: ";
	gets(mapsGasoline[idGasoline].nameResponsavel);

    fflush(stdin);
	std::cout <<"Abastecer quantos litros: ";
	std::cin >> totalLitro;

    totalDevido = priceEtanol * totalLitro;

    //data correta
    dataHoraAutomatica(mapsGasoline);

    mapsGasoline[idGasoline].codigoProduto = 3;
    mapsGasoline[idGasoline].idComprador = id;
    mapsGasoline[idGasoline].litrosAbastecidos = totalLitro;
    mapsClient[id].bill += totalDevido;

	std::cout << totalDevido <<" R$ adicionado a conta do cliente"<<std::endl;
}

void dataHoraAutomatica(std::map <int, Gasoline> &mapsGasoline)
{
    struct tm *p;
    time_t seconds;

    time(&seconds);
    p = localtime(&seconds);

    //data correta
    mapsGasoline[idGasoline].diaAbastecimento.ano = p->tm_year + 1900;
    mapsGasoline[idGasoline].diaAbastecimento.mes = p->tm_mon + 1;
    mapsGasoline[idGasoline].diaAbastecimento.dia = p->tm_mday;
    //hora correta
    mapsGasoline[idGasoline].diaAbastecimento.hora = p->tm_hour;
	mapsGasoline[idGasoline].diaAbastecimento.minu = p->tm_min;
	mapsGasoline[idGasoline].diaAbastecimento.sec = p ->tm_sec;
}

void printExtratos(std::map <int, Gasoline> &mapsGasoline)
{
    std::map<int, Gasoline>::iterator it = mapsGasoline.begin();
    int i = 0;
    while (it != mapsGasoline.end()) {
        int codigo = it->first;
        Gasoline ponto = it->second;
        std::cout << "\n\nCompras no mês\n\nExtrato "<< i+1<<"\nCodigo do produto: " << ponto.codigoProduto <<"\n"<<
            "ID do comprador: " << ponto.idComprador <<"\n"<< "Litros abastecidos: " << ponto.litrosAbastecidos <<"\n"<<
                    "Responsavel pelo abastecimento: " <<ponto.nameResponsavel <<"\nData: " <<
                        ponto.diaAbastecimento.dia <<"/" << ponto.diaAbastecimento.mes << "/" << ponto.diaAbastecimento.ano <<
                            "\nHoras: " << ponto.diaAbastecimento.hora <<":"<<ponto.diaAbastecimento.minu<<":"<<
                                ponto.diaAbastecimento.sec<<std::endl;
        i++;
        it++;
    }
    std::cout <<"\n\nAperte qualquer tecla para continuar\n";
}

void printClients(std::map <int, Client> &mapsClient, bool situationAccount)
{
    std::map<int, Client>::iterator it = mapsClient.begin();
    int i = 0;
    while (it != mapsClient.end()) {
        int codigo = it->first;
        Client ponto = it->second;

        if (ponto.activate == situationAccount){
            std::cout << "\n\nClientes\n\n" << "Nome: " << ponto.name <<"\nId: " <<codigo <<
                "\nCpf: "<<ponto.cpf<<"\nDivida: "<<ponto.bill <<" R$"<< std::endl;
        }
        i++;
        it++;
    }
    std::cout <<"\n\nAperte qualquer tecla para continuar\n";
}

void removerCliente(std::map <int, Client> &mapsClient)
{
    int resposta;

    std::cout <<"Qual o id do cliente: ";
    std::cin >> resposta;

    if (mapsClient.find(resposta) == mapsClient.end()){
        std::cout <<"Matricula não encontrada";
    }
    else{
            if (mapsClient[resposta].bill == 0)
            {
                mapsClient.erase(resposta);
            }
            else{
                std::cout << "O cliente deve pagar sua divida antes!" <<std::endl;
            }
    }
    std::cout <<"\n\nAperte qualquer tecla para continuar\n";
}

void desativarConta(std::map <int, Client> &mapsClient)
{
    int resposta;

    std::cout <<"Digite o id do cliente: ";
    std::cin >> resposta;

    if (mapsClient.find(resposta) == mapsClient.end()){
        std::cout <<"Matricula não encontrada";
    }
    else{
        mapsClient[resposta].activate = false;
    }
    std::cout <<"\n\nAperte qualquer tecla para continuar\n";
}

int menu()
{
	int resposta;

	std::cout <<"\n\n   **********************************\n"<<
	   "   * Cadastrar novo cliente(1)      *\n" <<
		"   * Listar clientes conta ativa(2) *\n"<<
			"   * Remover Clientes(3)            *\n" <<
				"   * Extrato de vendas(4)           *\n" <<
					"   * Abastecer(5)                   *\n" <<
						"   * Clientes com conta inativas(6) *\n" <<
							"   * Desativar conta cliente(7)     *\n" <<
                                "   * Pagar conta(8)                 *\n"<<
                                    "   * Sair do programa(9)            *\n" <<
                                        "   **********************************\n";
	std::cin >> resposta;
	return resposta;
}

void apagarTela()
{
	#ifdef _WIN32
		system("cls");
	#elif __ANDROID__
		clrscr();
	#else
		system("clear");
	#endif
}

void alterarPreco()
{
    int gasolinaResposta;

    std::cout <<"\n   ********************************\n   " <<
        "* Alterar preço combustivel(1) *\n   * Não alterar(2)" <<
            "               *\n" <<
                "   ********************************";
    std::cin >>gasolinaResposta;

    switch(gasolinaResposta){
        case 1:
            std::cout <<"Preço gasolina: ";
            std::cin >> priceGasolina;

            std::cout <<"\nPreço etanol: ";
            std::cin >> priceEtanol;

            std::cout <<"\nPreço diesel: ";
            std::cin >> priceDisel;

            std::cout <<"Preço alterado com sucesso!";
            break;

        case 2:
            std::cout <<"\nPreço da gasolina permanece o mesmo!";
            break;
        default:
            std::cout <<"\nPreço da gasolina permanece o mesmo!";
            break;
    }
    std::cout <<"\n\nAperte qualquer tecla para continuar\n";
}

void pagarConta(std::map <int, Client> &mapsClient)
{
    int resposta, valorPago;
    int verificador = 1;

    std::cout <<"Digite o id do cliente: ";
    std::cin >> resposta;

    if (mapsClient.find(resposta) == mapsClient.end()){
        std::cout <<"Matricula não encontrada!";
    }
    else{
        do
        {
            std::cout <<"Qual o valor pago: ";
            std::cin >> valorPago;

            if(valorPago <= mapsClient[resposta].bill){
                mapsClient[resposta].bill = mapsClient[resposta].bill - valorPago;
                verificador = 0;
            }
            else{
                std::cout <<"Valor maior que a divida de "<<mapsClient[resposta].bill<<"R$!\n";
                verificador = 1;
            }
        }while(verificador != 0);

        if(mapsClient[resposta].bill > 0)
        {
            std::cout <<"Ainda falta pagar "<<mapsClient[resposta].bill<<"R$";
        }
        else
        {
            std::cout <<"Conta quitada!";
        }
    }
    std::cout <<"\n\nAperte qualquer tecla para continuar\n";
}

void criandoPessoasTeste(std::map <int, Client> &mapsClient)
{
	idClient++;
    int id = idClient;

    if(mapsClient.find(id) == mapsClient.end()){

		fflush(stdin);
		char nome[50] = "Pablo";
		strcpy(mapsClient[id].name, nome);

        char cpf[12] = "12345678910";
        strcpy(mapsClient[id].cpf, cpf);

		mapsClient[id].activate = true;
		mapsClient[id].bill = 0;
	}

    idClient++;
    id = idClient;

    if(mapsClient.find(id) == mapsClient.end()){

		fflush(stdin);
		char nome[50] = "Carlos";
		strcpy(mapsClient[id].name, nome);

        char cpf[12] = "11345678910";
        strcpy(mapsClient[id].cpf, cpf);

		mapsClient[id].activate = true;
		mapsClient[id].bill = 0;
	}

	idClient++;
    id = idClient;

    if(mapsClient.find(id) == mapsClient.end()){

		fflush(stdin);
		char nome[50] = "Luan";
		strcpy(mapsClient[id].name, nome);

        char cpf[12] = "11345668910";
        strcpy(mapsClient[id].cpf, cpf);

		mapsClient[id].activate = true;
		mapsClient[id].bill = 350;
	}

	idClient++;
    id = idClient;

    if(mapsClient.find(id) == mapsClient.end()){

		fflush(stdin);
		char nome[50] = "Escobar";
		strcpy(mapsClient[id].name, nome);

        char cpf[12] = "11345668900";
        strcpy(mapsClient[id].cpf, cpf);

		mapsClient[id].activate = false;
		mapsClient[id].bill = 10;
	}
}
