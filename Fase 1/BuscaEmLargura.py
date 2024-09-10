def acao(destino, custo):
    # Cria uma ação representando um destino e o custo para chegar a ele
    return { 'destino': destino, 'custo': custo }

# Definindo o espaço de estados para o problema da Romênia
estados_romenia = [
    { 'estado': 'Arad',
      'acoes': [acao('Zerind', 75), acao('Timisoara', 118), acao('Sibiu', 140)] },

    { 'estado': 'Zerind',
      'acoes': [acao('Arad', 75), acao('Oradea', 71)] },

    { 'estado': 'Timisoara',
      'acoes': [acao('Arad', 118), acao('Lugoj', 111)] },

    { 'estado': 'Sibiu',
      'acoes': [acao('Arad', 140), acao('Oradea', 151), acao('Fagaras', 99),
                acao('Rimnicu Vilcea', 80)] },

    { 'estado': 'Oradea',
      'acoes': [acao('Zerind', 71), acao('Sibiu', 151)] },

    { 'estado': 'Lugoj',
      'acoes': [acao('Timisoara', 111), acao('Mehadia', 70)] },

    { 'estado': 'Mehadia',
      'acoes': [acao('Lugoj', 70), acao('Drobeta', 75)] },

    { 'estado': 'Drobeta',
      'acoes': [acao('Mehadia', 75), acao('Craiova', 120)] },

    { 'estado': 'Craiova',
      'acoes': [acao('Drobeta', 120), acao('Rimnicu Vilcea', 146),
                acao('Pitesti', 138)] },

    { 'estado': 'Rimnicu Vilcea',
      'acoes': [acao('Sibiu', 80), acao('Craiova', 146), acao('Pitesti', 97)] },

    { 'estado': 'Fagaras',
      'acoes': [acao('Sibiu', 99), acao('Bucharest', 211)] },

    { 'estado': 'Pitesti',
      'acoes': [acao('Rimnicu Vilcea', 97), acao('Craiova', 138), acao('Bucharest', 101)] },

    { 'estado': 'Giurgiu',
      'acoes': [acao('Bucharest', 90)] },

    { 'estado': 'Bucharest',
      'acoes': [acao('Fagaras', 211), acao('Pitesti', 101), acao('Giurgiu', 90),
                acao('Urziceni', 85)] },

    { 'estado': 'Urziceni',
      'acoes': [acao('Bucharest', 85), acao('Vaslui', 142), acao('Hirsova', 98)] },

    { 'estado': 'Hirsova',
      'acoes': [acao('Urziceni', 98), acao('Eforie', 86)] },

    { 'estado': 'Eforie',
      'acoes': [acao('Hirsova', 86)] },

    { 'estado': 'Vaslui',
      'acoes': [acao('Urziceni', 142), acao('Iasi', 92)] },

    { 'estado': 'Iasi',
      'acoes': [acao('Vaslui', 92), acao('Neamt', 87)] },

    { 'estado': 'Neamt',
      'acoes': [acao('Iasi', 87)] }
]

class No:
    def __init__(self, estado, custo, pai, acao):
        # Inicializa um nó com estado, custo para chegar até ele, nó pai e ação realizada
        self.estado = estado
        self.custo = custo
        self.pai = pai
        self.acao = acao

    def __str__(self):
        # Representação em string do nó
        return f'({self.estado}, {self.custo})'

    def __repr__(self):
        # Representação em string para debug do nó
        return self.__str__()

    def filhos(self, problema):
        # Encontra os filhos do nó baseado no problema
        espaco_acoes = next(e for e in problema.espaco_estados if e['estado'] == self.estado)

        resultado = []
        for acao in espaco_acoes['acoes']:
            filho = No(acao['destino'], self.custo + acao['custo'],
                       self, acao['destino'])
            resultado.append(filho)

        return resultado

    def constroi_solucao(self):
        # Constrói a solução (caminho) a partir do nó até o nó inicial
        no_atual = self
        solucao = [no_atual]
        while no_atual.pai != None:
            no_atual = no_atual.pai
            solucao.insert(0, no_atual)

        return solucao

class Problema:
    def __init__(self, espaco_estados, inicial, objetivo):
        # Inicializa o problema com espaço de estados, estado inicial e função objetivo
        self.espaco_estados = espaco_estados
        self.inicial = inicial
        self.objetivo = objetivo

# Constantes para representar o status da busca
BUSCA_INICIANDO = 0
BUSCA_FALHOU = 1
BUSCA_SUCESSO = 2
BUSCA_EM_CURSO = 3

class BuscaLargura:
    def __init__(self, problema):
        # Inicializa a busca em largura com o problema dado
        self.problema = problema
        self.fronteira = [problema.inicial]  # Nós a serem explorados
        self.visitados = [problema.inicial.estado]  # Estados já visitados
        self.solucao = []  # Solução encontrada
        self.situacao = BUSCA_INICIANDO  # Estado da busca

    def executar(self):
        # Executa a busca em largura até que seja encontrada a solução ou ocorra uma falha
        while self.situacao != BUSCA_FALHOU and self.situacao != BUSCA_SUCESSO:
            self.passo_busca()

        if self.situacao == BUSCA_FALHOU:
            print("Busca falhou")
        elif self.situacao == BUSCA_SUCESSO:
            print("Busca teve sucesso")
            print(f"Solucao: {self.solucao}")

        return
      
    def passo_busca(self):
        # Realiza um passo da busca em largura
        if (self.situacao == BUSCA_FALHOU):
            print("Busca falhou")
            return

        if (self.situacao == BUSCA_SUCESSO):
            print("Busca chegou ao objetivo com sucesso")
            return

        try:
            no = self.fronteira.pop(0)  # Pega o próximo nó da fronteira
        except IndexError:
            self.situacao = BUSCA_FALHOU  # Falhou se não houver mais nós na fronteira
            return

        # Faz teste do objetivo
        if self.problema.objetivo(no):
            self.situacao = BUSCA_SUCESSO
            self.solucao = no.constroi_solucao()  # Constrói a solução encontrada
            return

        # Obtém os filhos do nó atual e os adiciona à fronteira se ainda não foram visitados
        for filho in no.filhos(self.problema):
            if not (filho in self.fronteira) and not (filho.estado in self.visitados):
                self.fronteira.append(filho)
                self.visitados.append(filho.estado)

        return


#Solicita ao usuario para dizer a cidade inicial e final

nome_inicial = input("Digite o nome da cidade inicial corretamente: ")
nome_final = input("Digite o nome da cidade final corretamente: ")

# Cria o nó inicial (Arad) com custo 0 e sem pai nem ação
no_inicial = No(nome_inicial, 0, None, None)

# Define o problema com o espaço de estados da Romênia, o estado inicial e o objetivo (Bucharest)
problema_romenia = Problema(estados_romenia,
                            no_inicial,
                            lambda no: no.estado == nome_final)

# Inicializa a busca em largura com o problema da Romênia
busca = BuscaLargura(problema_romenia)




# Solicita ao usuário para escolher entre busca passo a passo ou busca completa
escolha = int(input("Digite a sua escolha \n (1) Busca passo a passo \n (2) Busca.executar\n Escolha: "))

if(escolha==1):
    # Se a escolha for 1, faz a busca passo a passo e exibe a solução quando encontrada
    while(busca.situacao !=BUSCA_SUCESSO):
      print(busca.fronteira)
      busca.passo_busca()
    print(f"Solucao: {busca.solucao}")
else:
    # Se a escolha for 2, executa a busca completa
    busca.executar()
