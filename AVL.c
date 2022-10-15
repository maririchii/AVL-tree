//===============================================================================================//
//AVL.h                                                                                          //
//===============================================================================================//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define SUCESSO 1
#define FALHA 0
#define VAZIA -1

typedef struct NO* PONT;

typedef struct NO{
    int chave;
    int alt; /* essencial para calcular o fator de balanceamento do nó */
    struct NO* esq;
    struct NO* dir;
}NO;

PONT inicializa();
PONT criaNo(int chave);
int maior(int x, int y);
int altura(PONT r);
int verificaElemento(PONT r, int chave);
PONT rotacaoDireita(PONT r);
PONT rotacaoEsquerda(PONT r);
PONT rotacaoEsquerdaDireita(PONT r);
PONT rotacaoDireitaEsquerda(PONT r);
PONT insereNo(PONT r, int chave);
int fatorBalanceamento(PONT no);
PONT balancear(PONT r);
PONT removeNo(PONT r, int chave);
void impressaoUm(PONT r, int nivel);
void impressaoDois(PONT r);
void imprimeOpcoes();

//===============================================================================================//
//AVL.c                                                                                          //
//===============================================================================================//
PONT inicializa(){
    return NULL;
}

PONT criaNo(int chave){
    PONT novoNo = (PONT)malloc(sizeof(NO));
    novoNo->chave = chave;
    novoNo->alt = 0;
    novoNo->esq = NULL;
    novoNo->dir = NULL;
    return(novoNo);
}

int maior(int x, int y){
    if(x > y) return x;
    else return y;
}

int altura(PONT r){
    if(r == NULL) return (-1);
    else return (r->alt);
}

int verificaElemento(PONT r, int chave){

    /* Verifica se o elemento que quero inserir ou remover existe na árvore */

    if(r == NULL) return(VAZIA);
    else if(r->chave == chave) return(SUCESSO);
    else if(r->chave > chave) return(verificaElemento(r->esq, chave));
    else if(r->chave < chave) return(verificaElemento(r->dir, chave));
    else return (FALHA);
}

PONT rotacaoDireita(PONT r){
    PONT aux;

    /*
     * A rotação à direita será feita quando inserir duas vezes seguidas seguidas à esquerda
     * PONT r é o nó desbalanceado
     * PONT aux é a subárvore da esquerda (que depois será a nova raiz da árvore)
     */

    aux = r->esq;
    r->esq = aux->dir;
    aux->dir = r;

    /*
     * Pega a maior altura das suas subárvores (filho à esquerda e filho à direita)
     * pois altura de um nó é o caminho mais longo deste até uma folha
     * deve fazer +1 pois está um nível acima
     */
     
    r->alt = maior(altura(r->dir), altura(r->esq)) + 1;
    aux->alt = maior(altura(aux->esq), r->alt) + 1;

    return(aux);

}

PONT rotacaoEsquerda(PONT r){
    PONT aux;

    /*
     * A rotação à esquerda será feita quando inserir duas vezes seguidas seguidas à direita
     */

    aux = r->dir;
    r->dir = aux->esq;
    aux->esq = r;

    r->alt = maior(altura(r->dir), altura(r->esq)) + 1;
    aux->alt = maior(altura(aux->dir), r->alt) + 1;

    return(aux);

}

PONT rotacaoEsquerdaDireita(PONT r){
    
    /*
     * Quando insero um nó à direita do filho à esquerda do nó desbalanceado
     * corresponde a duas rotações simples alternadas
     */

    r->esq = rotacaoEsquerda(r->esq); //r->esq vai receber a nova raiz dessa subárvore que foi rotacionada
    return(rotacaoDireita(r));
}

PONT rotacaoDireitaEsquerda(PONT r){
    
    /*
     * Quando insero um nó à esquerda do filho à direita do nó desbalanceado
     * corresponde a duas rotações simples alternadas
     */

    r->dir = rotacaoDireita(r->dir); //r->dir vai receber a nova raiz dessa subárvore que foi rotacionada
    return(rotacaoEsquerda(r));
}

PONT insereNo(PONT r, int chave){
    if(r == NULL){ /* Caso em que a árvore está vazia */
        return criaNo(chave);
    }

    else{

        /*
         * Irá ser inserido na subárvore da esquerda
         */

        if(chave < (r->chave)){
            r->esq = insereNo(r->esq, chave);
        }

        /*
         * Irá ser inserido na subárvore da direira (chave > (r->chave))
         */

        else{
            r->dir = insereNo(r->dir, chave);
        }
    }

    /*
     * A inserção pode ocasionar um desbalanceamento
     * Portanto, é necessário recalcular a altura de todos os nós
     * da raiz até o nó inserido e verificar se precisa balancear
     */

    r->alt = maior(altura(r->esq), altura(r->dir)) + 1;

    /* A função 'balancear' retorna a nova raiz da árvore após as rotações */
    r = balancear(r);

    return(r);
}

int fatorBalanceamento(PONT no){
    if(no != NULL){
        return altura(no->esq) - altura(no->dir);
    }
}

PONT balancear(PONT r){
    
    /* 
     * Função auxiliar para balancear a árvore após uma remoção e inserção
     * Recebe o nó desbalanceado e retorna a nova raiz após o balanceamento
     * Fb > 0 => Pende para a esquerda
     * Fb < 0 => Pende para a direita
     */

    int fb = fatorBalanceamento(r);

    /* Com certeza possui um filho à direita
     * Verifica para qual lado esse filho pende
     * Se continuar dando negativo, pende para a direita
     * Resultado: Pende para a direita duas vezes
     */
    if(fb < -1 && fatorBalanceamento(r->dir) <= 0){
        r = rotacaoEsquerda(r);
    }

    /* Com certeza possui um filho à esquerda
     * Verifica para qual lado esse filho pende
     * Se continuar dando positivo, pende para a esquerda
     * Resultado: Pende para a esquerda duas vezes
     */
    else if(fb > 1 && fatorBalanceamento(r->esq) >= 0){
        r = rotacaoDireita(r);
    }

    /* Com certeza possui um filho à esquerda
     * Verifica para qual lado esse filho pende
     * Se der negativo, pende para a direita
     * Resultado: Pende para a esquerda e depois para a direita
     */
    else if(fb > 1 && fatorBalanceamento(r->esq) < 0){
        r = rotacaoEsquerdaDireita(r);
    }

    /* Com certeza possui um filho à direita
     * Verifica para qual lado esse filho pende
     * Se der positivo, pende para a esquerda
     * Resultado: Pende para a direita e depois para a esquerda
     */
    else if(fb < -1 && fatorBalanceamento(r->dir) > 0){
        r = rotacaoDireitaEsquerda(r);
    }

    return(r);
}

PONT removeNo(PONT r, int chave){

    /*
     * Pode ser remoção de um nó folha (sem filhos), com 1 filho ou com 2 filhos
     */

    if(r == NULL){ /* A árvore não existe */
        return NULL;
    }

    else{

        /*
         * O nó a ser removido é justamente o nó em que estou (atual raiz)
         */

        if(chave == (r->chave)){

            /*
             * Verifica se é um nó folha
             * Basta removê-lo já que não possui filhos
             */

            if(r->esq == NULL && r->dir == NULL){
                free(r);
                return NULL; /* Retorna nulo para o devido campo (direita ou esquerda) do nó anterior da chamada recursiva */
            }

            else{

                /*
                 * Caso em que o nó possui apenas 1 filho
                 */

                if(r->esq == NULL || r->dir == NULL){
                    PONT aux; /* Um ponteiro que vai receber o filho que a árvore possui */
                    
                    if(r->esq != NULL){
                        aux = r->esq;
                    }

                    else{
                        aux = r->dir;
                    }

                    free(r);
                    return(aux); /* Retorna o filho que foi salvo */
                }

                else{

                   /*
                    * Caso em que o nó possui os 2 filhos
                    * Descobrir quem é o nó mais à direita da subárvore esquerda
                    */ 

                    PONT aux = r->esq;

                    /*
                     * Quando o laço parar, estou no elemento mais à direita possível
                     * Então, troca-se o elemento da raiz (nó atual) com esse aux
                     */

                    while(aux->dir != NULL){
                        aux = aux->dir;
                    }

                    r->chave = aux->chave; /* Recebe o nó que vai ficar no lugar */
                    aux->chave = chave; /* Recebe o nó a ser removido de fato */
                    r->esq = removeNo(r->esq, chave);
                    return(r); /* Retorna a raiz em que estávamos, só que atualizada */
                }
            }
        }

        else{

            /*
             * Se trata de uma remoção na subárvore da esquerda
             */

            if(chave < (r->chave)){
                r->esq = removeNo(r->esq, chave);
            }

            /*
             * Se trata de uma remoção na subárvore da direita
             */

            else{
                r->dir = removeNo(r->dir, chave);
            }
        }

        /* A remoção pode acabar alterando o balanceamento da árvore 
         * Recalcula a altura da árvore
         */

        r->alt = maior(altura(r->dir), altura(r->esq)) + 1;
        r = balancear(r);

        return(r);
    }
}

void impressaoUm(PONT r, int nivel){
    /* subárvore direita - raíz - subárvore esquerda */
    int i;
    if(r != NULL){

        /*
         * Vai para a extrema direita da árvore
         */

        impressaoUm(r->dir, nivel + 1);
        printf("\n\n");

        for(i = 0; i < nivel; i++){
            printf("\t"); //Tabulação para separar os níveis da árvore
        }

        printf("%d", r->chave); //Imprime a chave da raiz da extrema direita
        impressaoUm(r->esq, nivel + 1); //Depois verifica à esquerda
    }
}

void impressaoDois(PONT r){
    /* raiz - subarvore esquerda - subarvore direita */
    if(r != NULL){
        printf("%d", r->chave);
        printf("(");
        impressaoDois(r->esq);
        impressaoDois(r->dir);
        printf(")");
    }
}

void imprimeOpcoes(){
    printf("O que deseja fazer com a Arvore AVL? Confira as opcoes abaixo:\n");
    printf("[1] Inserir um elemento\n");
    printf("[2] Remover um elemento\n");
    printf("[3] Imprimir a Arvore AVL\n");
    printf("[0] Finalizar\n\n");
}

//===============================================================================================//
//main.c                                                                                         //
//===============================================================================================//
int main(){

    PONT r = inicializa(); //Inicializa a raiz colocando NULL
    int op, elemento, resultado;

    do{
        imprimeOpcoes();
        printf("Opcao: ");
        scanf("%d", &op);

        switch (op){     
        case 1:
            printf("Digite o elemento (numero inteiro) que deseja inserir: ");
            scanf("%d", &elemento);

            /* Verifica se o nó já existe na árvore */
            resultado = verificaElemento(r, elemento);

            if(resultado == VAZIA || resultado == FALHA){
                r = insereNo(r, elemento);
                printf("\nO numero %d foi inserido com sucesso na Arvore AVL!\n\n", elemento);
            }
            else{
                printf("\nO elemento %d nao pode ser inserido. Tente novamente.\n\n", elemento);
            }
            break;

        case 2:
            printf("Digite o elemento (numero inteiro) que deseja remover: ");
            scanf("%d", &elemento);

            /* Verifica se o elemento que quero remover existe na árvore */
            resultado = verificaElemento(r, elemento);

            if(resultado == SUCESSO){
                r = removeNo(r, elemento);
                printf("\nO numero %d foi removido com sucesso da Arvore AVL!\n\n", elemento);
            }
            else{
                printf("\nO elemento %d nao pode ser removido. Tente novamente.\n\n", elemento);
            }
            break;
        
        case 3:

            if(r == NULL){
                printf("\nARVORE AVL VAZIA!\n\n");
            }

            else{
                impressaoUm(r, 1);
                printf("\n\n");
                printf("Outra forma de impressao: ");
                impressaoDois(r);
                printf("\n\n");
            }
            break;

        default:
            if(op != 0) printf("Essa opcao nao existe. Tente novamente.\n\n");
            break;
        }

    }while(op != 0);
    
    return 0;
}
