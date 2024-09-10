#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <stack>

using namespace std;
class No
{
public:
    string nome;
    int custoTotal;
    No *pai;
    vector<pair<No *, int>> filhos;

    No(const string &nomezao) : nome(nomezao), custoTotal(0), pai(NULL) {}

    void adicionarFilhos(No *filho, int custo)
    {

        filhos.push_back(make_pair(filho, custo));
    }

    void setCusto(int n)
    {
        custoTotal = n;
    }

    string getNome() const
    {
        return nome;
    }
};

unordered_map<string, No *> nos; // Usando unordered_map para armazenar nós

void adicionarAcao(const string &nomepai, const string &nomefilho, int custo)
{
    if (nos.find(nomefilho) == nos.end())
    {
        nos[nomefilho] = new No(nomefilho); // Cria e adiciona o nó filho ao mapa de nós
    }

    if (nos.find(nomepai) == nos.end())
    {
        nos[nomepai] = new No(nomepai); // Cria e adiciona o nó pai ao mapa de nós
    }

    No *pai = nos[nomepai];
    No *filho = nos[nomefilho];

    if (pai && filho)
    {
        pai->adicionarFilhos(filho, custo);
    }
}

No *encontrarNo(const string &nome)
{
    auto it = nos.find(nome);
    if (it != nos.end())
    {
        return it->second;
    }
    return nullptr;
}

void configurarEstados()
{
    adicionarAcao("Arad", "Zerind", 75);
    adicionarAcao("Arad", "Timisoara", 118);
    adicionarAcao("Arad", "Sibiu", 140);

    adicionarAcao("Zerind", "Arad", 75);
    adicionarAcao("Zerind", "Oradea", 71);

    adicionarAcao("Timisoara", "Arad", 118);
    adicionarAcao("Timisoara", "Lugoj", 111);


    adicionarAcao("Sibiu", "Arad", 140);
    adicionarAcao("Sibiu", "Oradea", 151);
    adicionarAcao("Sibiu", "Fagaras", 99);
    adicionarAcao("Sibiu", "Rimnicu Vilcea", 80);


    adicionarAcao("Oradea", "Zerind", 71);
    adicionarAcao("Oradea", "Sibiu", 151);

    adicionarAcao("Lugoj", "Timisoara", 111);
    adicionarAcao("Lugoj", "Mehadia", 70);

    adicionarAcao("Mehadia", "Lugoj", 70);
    adicionarAcao("Mehadia", "Drobeta", 75);

    adicionarAcao("Drobeta", "Mehadia", 75);
    adicionarAcao("Drobeta", "Craiova", 120);


    adicionarAcao("Craiova", "Drobeta", 120);
    adicionarAcao("Craiova", "Rimnicu Vilcea", 146);
    adicionarAcao("Craiova", "Pitesti", 138);

    adicionarAcao("Rimnicu Vilcea", "Sibiu", 80);
    adicionarAcao("Rimnicu Vilcea", "Craiova", 146);
    adicionarAcao("Rimnicu Vilcea", "Pitesti", 97);

    adicionarAcao("Fagaras", "Sibiu", 99);
    adicionarAcao("Fagaras", "Bucharest", 211);


    adicionarAcao("Pitesti", "Rimnicu Vilcea", 97);
    adicionarAcao("Pitesti", "Craiova", 138);
    adicionarAcao("Pitesti", "Bucharest", 101);

    
    adicionarAcao("Giurgiu", "Bucharest", 90);
    adicionarAcao("Bucharest", "Fagaras", 211);
    adicionarAcao("Bucharest", "Pitesti", 101);
    adicionarAcao("Bucharest", "Giurgiu", 90);
    adicionarAcao("Bucharest", "Urziceni", 85);
    adicionarAcao("Urziceni", "Bucharest", 85);
    adicionarAcao("Urziceni", "Vaslui", 142);
    adicionarAcao("Urziceni", "Hirsova", 98);
    adicionarAcao("Hirsova", "Urziceni", 98);
    adicionarAcao("Hirsova", "Eforie", 86);
    adicionarAcao("Eforie", "Hirsova", 86);
    adicionarAcao("Vaslui", "Urziceni", 142);
    adicionarAcao("Vaslui", "Iasi", 92);
    adicionarAcao("Iasi", "Vaslui", 92);
    adicionarAcao("Iasi", "Neamt", 87);
    adicionarAcao("Neamt", "Iasi", 87);
}

vector<string> reconstruirCaminho(No *objetivo)
{
    vector<string> resultado;

    No *n = objetivo;
    while (n != NULL)
    {
        resultado.push_back(n->nome);
        n = n->pai;
    }

    reverse(resultado.begin(), resultado.end());

    return resultado;
}

vector<vector<string>> buscaEmLarguraPassoAPasso(const string &inicio, const string &objetivo, int &custoTotal)
{
    vector<vector<string>> passos; // Vetor de vetores para armazenar o estado em cada passo
    unordered_set<string> visitados;
    queue<No *> fila;

    if (!encontrarNo(inicio) || !encontrarNo(objetivo))
    {
        cout << "Estado inicial ou final não encontrado! " << endl;
        return passos;
    }

    No *inicial = encontrarNo(inicio);
    fila.push(inicial);
    visitados.insert(inicial->nome);
    inicial->setCusto(0);

    while (!fila.empty())
    {
        vector<string> estadoAtual; // Vetor para armazenar o estado atual da fila
        int tamanhoFila = fila.size();
       
        for (int i = 0; i < tamanhoFila; ++i)
        {
            No *atual = fila.front();
            fila.pop();
            estadoAtual.push_back(atual->nome);

            if (atual->nome == objetivo)
            {
                custoTotal = atual->custoTotal;
                passos.push_back(reconstruirCaminho(atual));
                return passos;
            }

            for (const auto &filho : atual->filhos)
            {
                No *proximoNo = filho.first;
                int custo = filho.second;

                if (visitados.find(proximoNo->nome) == visitados.end())
                {
                    fila.push(proximoNo);
                    visitados.insert(proximoNo->nome);
                    proximoNo->pai = atual;
                    proximoNo->custoTotal = custo + atual->custoTotal;
                }
            }
        }

        passos.push_back(estadoAtual); // Adiciona o estado atual da fila aos passos
    }

    custoTotal = -1;
    return passos;
}

vector<string> buscaEmLargura(const string &inicio, const string &objetivo, int &custoTotal)
{
    vector<string> resultado;
    unordered_set<string> visitados;
    queue<No *> fila;

    if (!encontrarNo(inicio) || !encontrarNo(objetivo))
    {
        cout << "Estado inicial ou final não encontrado! " << endl;
        return resultado;
    }

    No *inicial = encontrarNo(inicio);

    fila.push(inicial);
    visitados.insert((inicial)->nome);
    inicial->setCusto(0);

    while (!fila.empty())
    {

        No *atual = fila.front();
        fila.pop();
        resultado.push_back(atual->nome);

        if (atual->nome == objetivo)
        {
            custoTotal = atual->custoTotal;
            return reconstruirCaminho(atual);
        }

        for (const auto &filho : atual->filhos)
        {

            No *proximoNo = filho.first;

            int custo = filho.second;

            if (visitados.find(proximoNo->nome) == visitados.end())
            {

                fila.push(proximoNo);

                visitados.insert(proximoNo->nome);

                proximoNo->pai = atual;

                proximoNo->custoTotal = custo + atual->custoTotal;
            }
        }
    }

    custoTotal = -1;
    return resultado;
}


vector<string> buscaEmProfundidade(const string &inicio, const string &objetivo, int &custoTotal){

    vector<string> resultado;
    unordered_set<string> visitados;
    stack <No*> pilha;

   if(!encontrarNo(inicio)||!encontrarNo(objetivo)){
    cout<<"Cidade inicial ou final não encontrada"<<endl;
    return resultado;
   }

No* inicial = encontrarNo(inicio);
pilha.push(inicial);
visitados.insert(inicio);
inicial->custoTotal = 0;

while(!pilha.empty()){

No* atual = pilha.top();
pilha.pop();
resultado.push_back(atual->nome);

if(atual->nome == objetivo){
    custoTotal = atual->custoTotal;
    return reconstruirCaminho(atual);
}

for(const auto &filho : atual->filhos){
    No* proximoNo = filho.first;
    int custo = filho.second;

    if(visitados.find(proximoNo->nome) == visitados.end()){
        pilha.push(proximoNo);
        visitados.insert(proximoNo->nome);
        proximoNo->pai = atual;
        proximoNo->custoTotal = atual->custoTotal + custo;

    }




}






}

custoTotal = -1;
return resultado;

}

vector<vector<string>> buscaEmProfundidadepassoapasso(const string &inicio, const string &objetivo, int &custoTotal){
    vector<vector<string>> passos; // Vetor de vetores para armazenar o estado em cada passo
    unordered_set<string> visitados;
    stack<No *> pilha;

    if (!encontrarNo(inicio) || !encontrarNo(objetivo))
    {
        cout << "Estado inicial ou final não encontrado! " << endl;
        return passos;
    }

    No *inicial = encontrarNo(inicio);
    pilha.push(inicial);
    visitados.insert(inicial->nome);
    inicial->setCusto(0);

    while (!pilha.empty())
    {
        vector<string> estadoAtual; // Vetor para armazenar o estado atual da pilha
        int tamanhopilha = pilha.size();

        for (int i = 0; i < tamanhopilha; ++i)
        {
            No *atual = pilha.top();
            pilha.pop();
            estadoAtual.push_back(atual->nome);

            if (atual->nome == objetivo)
            {
                custoTotal = atual->custoTotal;
                passos.push_back(reconstruirCaminho(atual));
                return passos;
            }

            for (const auto &filho : atual->filhos)
            {
                No *proximoNo = filho.first;
                int custo = filho.second;
                
                if (visitados.find(proximoNo->nome) == visitados.end())
                {   
                    pilha.push(proximoNo);
                    visitados.insert(proximoNo->nome);
                    
                    proximoNo->pai = atual;
                    proximoNo->custoTotal = custo + atual->custoTotal;
                }
            }
        }

        passos.push_back(estadoAtual); // Adiciona o estado atual da fila aos passos
    }

    custoTotal = -1;
    return passos;
}

void Profundidade_geral(const string& cidadeI, const string& cidadeF, int &custoTotal){

     vector<string> resultado = buscaEmProfundidade(cidadeI, cidadeF, custoTotal);

    cout << "Caminho: ";
    for (const string &estado : resultado)
    {
        cout << estado << " ";
    }
    cout << endl;

    // Imprime o custo total do percurso
    if (custoTotal != -1)
    {
        cout << "Custo total do percurso: " << custoTotal << endl;
    }
    else
    {
        cout << "Objetivo não encontrado." << endl;
    }
}

void Profundidade_passo(const string& cidadeI, const string& cidadeF, int &custoTotal){
vector<vector<string>> passos = buscaEmProfundidadepassoapasso(cidadeI, cidadeF, custoTotal);

    for (size_t i = 0; i < passos.size(); ++i)
    {
        cout << "Passo " << i + 1 << ": ";
        for (const string &nome : passos[i])
        {
            cout << nome << " ";
        }
        cout << endl;
    }

    if (custoTotal != -1)
    {
        cout << "Custo total do percurso: " << custoTotal << endl;
    }
    else
    {
        cout << "Objetivo não encontrado." << endl;
    }
}



void Largura_geral(const string& cidadeI, const string& cidadeF, int &custoTotal)
{

    vector<string> resultado = buscaEmLargura(cidadeI, cidadeF, custoTotal);

    cout << "Caminho: ";
    for (const string &estado : resultado)
    {
        cout << estado << " ";
    }
    cout << endl;

    // Imprime o custo total do percurso
    if (custoTotal != -1)
    {
        cout << "Custo total do percurso: " << custoTotal << endl;
    }
    else
    {
        cout << "Objetivo não encontrado." << endl;
    }
}

void Largura_passo(const string& cidadeI, const string& cidadeF,int &custoTotal)
{

    vector<vector<string>> passos = buscaEmLarguraPassoAPasso(cidadeI, cidadeF, custoTotal);

    for (size_t i = 0; i < passos.size(); ++i)
    {
        cout << "Passo " << i + 1 << ": ";
        for (const string &nome : passos[i])
        {
            cout << nome << " ";
        }
        cout << endl;
    }

    if (custoTotal != -1)
    {
        cout << "Custo total do percurso: " << custoTotal << endl;
    }
    else
    {
        cout << "Objetivo não encontrado." << endl;
    }
}
int main()
{

    configurarEstados();
    int custoTotal = 0;
    int t;
    string cidadeI;
    string cidadeF;

    cout << "------------------- Bem Vindo ----------------------" << endl
         << endl;
    cout<<"Escreva as cidades respectivas que deseja comecar: "<<endl;
    cout<<"Digite a cidade inicial: ";
    cin>>cidadeI;
    cout<<endl;
    cout<<"Digite a cidade final: ";
    cin>>cidadeF;
    cout<<endl;
    cout << "Digite o numero correspondente: "<<endl<<"(1) Largura passo a passo"<<endl<<"(2) Largura geral"<<endl<<"(3) Profundiade geral"<<endl<<"(4) Profundidade passo a passo" << endl
         << endl;
    
    cin >> t;
    cout << "------------------- Bem Vindo ----------------------" << endl
         << endl;


    switch (t)
    {
    case 1:
        cout << "--------------------- Largura_Passo ---------------------------" << endl
             << endl;
        Largura_passo(cidadeI,cidadeF,custoTotal);
        cout << endl
             << endl;
        cout << "--------------------- Largura_Passo  ---------------------------" << endl
             << endl;

        break;
    case 2:

        cout << "--------------------- Largura_geral ---------------------------" << endl
             << endl;
        Largura_geral(cidadeI,cidadeF,custoTotal);

        cout << endl
             << endl;
        cout << "---------------------  Largura_geral ---------------------------" << endl
             << endl;
        break;

    case 3: 
        cout << "--------------------- Profundiade_geral ---------------------------" << endl
             << endl;
            Profundidade_geral(cidadeI,cidadeF,custoTotal);

        cout << endl
             << endl;
        cout << "---------------------  Profundidade_geral ---------------------------" << endl
             << endl;
    break;
    case 4:

    cout << "--------------------- Profundiade passo a passo ---------------------------" << endl
             << endl;
                Profundidade_passo(cidadeI,cidadeF,custoTotal);

        cout << endl
             << endl;
        cout << "---------------------  Profundidade passo a passo  ---------------------------" << endl
             << endl;
    break;


    default:
        cout << "Numero diferente" << endl;
        break;
    }
}
