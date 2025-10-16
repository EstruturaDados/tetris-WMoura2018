#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define TAM_FILA 5
#define TAM_PILHA 3

// Estrutura da peça
typedef struct {
    int id;
    char nome;
} Peca;

// Estrutura da fila circular
typedef struct {
    Peca pecas[TAM_FILA];
    int frente;
    int tras;
    int qtd;
} Fila;

// Estrutura da pilha
typedef struct {
    Peca pecas[TAM_PILHA];
    int topo;
} Pilha;

// Estrutura para controle de desfazer
typedef struct {
    int ativo; // 1 = há jogada para desfazer
    Peca ultimaJogada;
} Historico;

// ==== Protótipos ====
void inicializarFila(Fila *f);
void inicializarPilha(Pilha *p);
int filaVazia(Fila *f);
int filaCheia(Fila *f);
int pilhaVazia(Pilha *p);
int pilhaCheia(Pilha *p);
void enfileirar(Fila *f, Peca p);
Peca desenfileirar(Fila *f);
void empilhar(Pilha *p, Peca p);
Peca desempilhar(Pilha *p);
Peca gerarPeca();
void exibirFila(Fila *f);
void exibirPilha(Pilha *p);
void trocarTopoFila(Fila *f, Pilha *p);
void desfazerJogada(Fila *f, Historico *h);
void inverterEstruturas(Fila *f, Pilha *p);

int main() {
    Fila fila;
    Pilha pilha;
    Historico historico = {0};
    int opcao;
    Peca nova, removida;

    srand(time(NULL));
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Inicializa a fila com 5 peças
    for (int i = 0; i < TAM_FILA; i++) {
        enfileirar(&fila, gerarPeca());
    }

    do {
        printf("\n===== TETRIS STACK - NIVEL MESTRE =====\n");
        printf("1 - Jogar peca\n");
        printf("2 - Reservar peca\n");
        printf("3 - Usar peca reservada\n");
        printf("4 - Trocar topo da pilha com frente da fila\n");
        printf("5 - Desfazer ultima jogada\n");
        printf("6 - Inverter fila e pilha\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: // Jogar peça da frente
                if (!filaVazia(&fila)) {
                    removida = desenfileirar(&fila);
                    printf("\nPeca jogada: ID=%d | Tipo=%c\n", removida.id, removida.nome);

                    // Guarda no histórico
                    historico.ativo = 1;
                    historico.ultimaJogada = removida;

                    // Insere nova peça automaticamente
                    nova = gerarPeca();
                    enfileirar(&fila, nova);
                    printf("Nova peca inserida automaticamente: ID=%d | Tipo=%c\n", nova.id, nova.nome);
                } else {
                    printf("\nA fila esta vazia!\n");
                }
                break;

            case 2: // Reservar peça
                if (pilhaCheia(&pilha)) {
                    printf("\nA pilha de reserva esta cheia!\n");
                } else if (filaVazia(&fila)) {
                    printf("\nA fila esta vazia!\n");
                } else {
                    removida = desenfileirar(&fila);
                    empilhar(&pilha, removida);
                    printf("\nPeca reservada: ID=%d | Tipo=%c\n", removida.id, removida.nome);

                    nova = gerarPeca();
                    enfileirar(&fila, nova);
                    printf("Nova peca inserida automaticamente: ID=%d | Tipo=%c\n", nova.id, nova.nome);
                }
                break;

            case 3: // Usar peça reservada
                if (pilhaVazia(&pilha)) {
                    printf("\nNao ha pecas reservadas!\n");
                } else {
                    removida = desempilhar(&pilha);
                    printf("\nPeca usada da reserva: ID=%d | Tipo=%c\n", removida.id, removida.nome);
                    historico.ativo = 1;
                    historico.ultimaJogada = removida;
                }
                break;

            case 4: // Trocar topo da pilha com frente da fila
                trocarTopoFila(&fila, &pilha);
                break;

            case 5: // Desfazer última jogada
                desfazerJogada(&fila, &historico);
                break;

            case 6: // Inverter fila com pilha
                inverterEstruturas(&fila, &pilha);
                break;

            case 0:
                printf("\nEncerrando o programa...\n");
                break;

            default:
                printf("\nOpcao invalida!\n");
        }

        // Exibe o estado atual das estruturas
        exibirFila(&fila);
        exibirPilha(&pilha);

    } while (opcao != 0);

    return 0;
}

// ==== Implementações ====

void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = -1;
    f->qtd = 0;
}

void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int filaVazia(Fila *f) {
    return (f->qtd == 0);
}

int filaCheia(Fila *f) {
    return (f->qtd == TAM_FILA);
}

int pilhaVazia(Pilha *p) {
    return (p->topo == -1);
}

int pilhaCheia(Pilha *p) {
    return (p->topo == TAM_PILHA - 1);
}

void enfileirar(Fila *f, Peca p) {
    if (filaCheia(f)) return;
    f->tras = (f->tras + 1) % TAM_FILA;
    f->pecas[f->tras] = p;
    f->qtd++;
}

Peca desenfileirar(Fila *f) {
    Peca p = f->pecas[f->frente];
    f->frente = (f->frente + 1) % TAM_FILA;
    f->qtd--;
    return p;
}

void empilhar(Pilha *p, Peca peca) {
    if (pilhaCheia(p)) return;
    p->topo++;
    p->pecas[p->topo] = peca;
}

Peca desempilhar(Pilha *p) {
    Peca peca = p->pecas[p->topo];
    p->topo--;
    return peca;
}

Peca gerarPeca() {
    Peca p;
    char tipos[] = {'I', 'O', 'T', 'L', 'J', 'S', 'Z'};
    p.id = rand() % 1000;
    p.nome = tipos[rand() % 7];
    return p;
}

void exibirFila(Fila *f) {
    printf("\n===== FILA DE PECAS FUTURAS =====\n");
    if (filaVazia(f)) {
        printf("Fila vazia!\n");
        return;
    }
    int i = f->frente;
    for (int c = 0; c < f->qtd; c++) {
        Peca p = f->pecas[i];
        printf("[%d] ID=%d | Tipo=%c\n", c + 1, p.id, p.nome);
        i = (i + 1) % TAM_FILA;
    }
}

void exibirPilha(Pilha *p) {
    printf("\n===== PILHA DE RESERVA =====\n");
    if (pilhaVazia(p)) {
        printf("Pilha vazia!\n");
        return;
    }
    for (int i = p->topo; i >= 0; i--) {
        printf("[%d] ID=%d | Tipo=%c\n", i + 1, p->pecas[i].id, p->pecas[i].nome);
    }
}

// Troca o topo da pilha com a frente da fila
void trocarTopoFila(Fila *f, Pilha *p) {
    if (filaVazia(f) || pilhaVazia(p)) {
        printf("\nNao e possivel trocar! Uma das estruturas esta vazia.\n");
        return;
    }
    int idxFrente = f->frente;
    Peca temp = f->pecas[idxFrente];
    f->pecas[idxFrente] = p->pecas[p->topo];
    p->pecas[p->topo] = temp;
    printf("\nTroca realizada com sucesso!\n");
}

// Desfaz a última jogada (retorna a peça ao início da fila)
void desfazerJogada(Fila *f, Historico *h) {
    if (!h->ativo) {
        printf("\nNao ha jogada para desfazer!\n");
        return;
    }
    if (filaCheia(f)) {
        printf("\nFila cheia! Nao e possivel desfazer agora.\n");
        return;
    }

    // Move a frente da fila para trás para abrir espaço
    f->frente = (f->frente - 1 + TAM_FILA) % TAM_FILA;
    f->pecas[f->frente] = h->ultimaJogada;
    f->qtd++;

    printf("\nUltima jogada desfeita! Peca ID=%d | Tipo=%c retornou a frente da fila.\n",
           h->ultimaJogada.id, h->ultimaJogada.nome);
    h->ativo = 0; // limpa histórico
}

// Inverte fila e pilha
void inverterEstruturas(Fila *f, Pilha *p) {
    if (filaVazia(f) && pilhaVazia(p)) {
        printf("\nNada para inverter!\n");
        return;
    }

    // Cria estruturas temporárias
    Fila novaFila;
    Pilha novaPilha;
    inicializarFila(&novaFila);
    inicializarPilha(&novaPilha);

    // Move da pilha para nova fila
    for (int i = p->topo; i >= 0; i--) {
        enfileirar(&novaFila, p->pecas[i]);
    }

    // Move da fila para nova pilha
    int i = f->frente;
    for (int c = 0; c < f->qtd; c++) {
        empilhar(&novaPilha, f->pecas[i]);
        i = (i + 1) % TAM_FILA;
    }

    // Substitui originais
    *f = novaFila;
    *p = novaPilha;

    printf("\nFila e pilha foram invertidas com sucesso!\n");
}
