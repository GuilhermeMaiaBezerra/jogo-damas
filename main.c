#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*Crie um algoritmo que simule um jogo de damas entre as duas pessoas até que um dos
jogadores elimine todas as peças do outro.

Matriz damas
*/

void Tabuleiro(char damas[8][8]);
void TransformarDama(char damas[8][8]);
int SalvarPosicoes(char damas[8][8], char simbolo);
int verificarCapturaMultipla(char damas[8][8], int linha, int coluna, char peca);

void IniciarJogo();
void Regras();
void Creditos();

int countTeamRed, countTeamGreen;

void Tabuleiro(char damas[8][8]){

    TransformarDama(damas);
    printf("\n      A   B   C   D   E   F   G   H");

    printf("\n    +---+---+---+---+---+---+---+---+");
    
    for(int i = 0; i < 8; i++){
        printf("\n");
        printf("%d   |",i+1);
        for(int j = 0; j < 8; j++){

           if(damas[i][j] == 'P' || damas[i][j] == 'D'){
            printf(" \033[1;31m%c\033[0m |", damas[i][j]);
           }
           else if(damas[i][j] == 'p' || damas[i][j] == 'd'){
            printf(" \033[1;32m%c\033[0m |", damas[i][j]);
           }
           else{
            printf("   |");
           }
           if(j == 7) printf("   %d",i+1);
        }
        printf("\n    +---+---+---+---+---+---+---+---+");
    }

    printf("\n      A   B   C   D   E   F   G   H\n");

    printf("\n+-----------------------------------------+\n");
}

void TransformarDama(char damas[8][8]){

    for(int i = 0 ; i < 8; i++){
        if(damas[0][i] == 'p'){
            damas[0][i] = 'd';
            printf("\n\033[1;32mA pedra virou dama!\033[0m\n");
        }
        else if(damas[7][i] == 'P'){
            damas[7][i] = 'D';
            printf("\n\033[1;31mA pedra virou dama!\033[0m\n");
        }
    }

}

int SalvarPosicoes(char damas[8][8], char simbolo) {
    int podeCapturarNovamente = 0;
    int origem_lin, origem_col;

    while (1) {
        char origem[3], destino[3];

        if (!podeCapturarNovamente) {
            printf("\nDigite o movimento (ex: A6 B5): ");
            scanf("%s %s", origem, destino);

            origem_col = toupper(origem[0]) - 'A';
            origem_lin = origem[1] - '1';
        } else {
            printf("\nVoce pode capturar novamente! Digite o proximo movimento (ex: C3): ");
            scanf("%s", destino);
        }

        int destino_col = toupper(destino[0]) - 'A';
        int destino_lin = destino[1] - '1';

        // Verifica se as coordenadas estão dentro do tabuleiro
        if (origem_col < 0 || origem_col > 7 || origem_lin < 0 || origem_lin > 7 ||
            destino_col < 0 || destino_col > 7 || destino_lin < 0 || destino_lin > 7) {
            printf("\n\033[1;33mMovimento invalido: Coordenadas fora do tabuleiro.\033[0m\n");
            continue;
        }

        char peca = damas[origem_lin][origem_col];

        // Verifica se a peça existe e se é ela está na vez
        if (peca != 'P' && peca != 'p' && peca != 'D' && peca != 'd') {
            printf("\n\033[1;33mMovimento invalido: Escolha uma coordenada que possua uma peca.\033[0m\n");
            continue;
        } else if ((simbolo == 'p' && (peca != 'p' && peca != 'd')) || 
                   (simbolo == 'P' && (peca != 'P' && peca != 'D'))) {
            if (simbolo == 'p') {
                printf("\n\033[1;33mMovimento invalido: Vez das pedras\033[0m \033[1;31mVermelhas\033[0m\n");
            } else {
                printf("\n\033[1;33mMovimento invalido: Vez das pedras\033[0m \033[1;32mVerdes\033[0m\n");
            }
            continue;
        }

        // Calcula diferenças
        int diff_lin = destino_lin - origem_lin;
        int diff_col = destino_col - origem_col;
        int abs_diff_lin = abs(diff_lin);
        int abs_diff_col = abs(diff_col);

        // Verifica se é movimento diagonal
        if (abs_diff_lin != abs_diff_col) {
            printf("\n\033[1;33mMovimento invalido: Apenas movimentos diagonais sao permitidos.\033[0m\n");
            continue;
        }

        // Verifica se o destino não tem nenhuma peça
        if (damas[destino_lin][destino_col] != ' ') {
            printf("\n\033[1;33mMovimento invalido: Casa de destino ocupada.\033[0m\n");
            continue;
        }

        // Verifica captura para damas e pedras
        if (abs_diff_lin >= 2 && abs_diff_col >= 2) {

            // Para ser captura, deve pular exatamente uma peça adversária
            int passo_lin = diff_lin / abs_diff_lin;
            int passo_col = diff_col / abs_diff_col;
            
            int pecas_adversarias = 0;
            int ultima_lin = origem_lin;
            int ultima_col = origem_col;
            
            for (int i = 1; i < abs_diff_lin; i++) {
                int linha_atual = origem_lin + i * passo_lin;
                int coluna_atual = origem_col + i * passo_col;
                
                char peca_atual = damas[linha_atual][coluna_atual];
                
                // Verifica peças adversárias
                if (peca_atual != ' ') {
                    if ((peca == 'd' || peca == 'p') && (peca_atual == 'P' || peca_atual == 'D')) {
                        pecas_adversarias++;
                        ultima_lin = linha_atual;
                        ultima_col = coluna_atual;
                    } else if ((peca == 'D' || peca == 'P') && (peca_atual == 'p' || peca_atual == 'd')) {
                        pecas_adversarias++;
                        ultima_lin = linha_atual;
                        ultima_col = coluna_atual;
                    } else {
                        // Peça aliada no caminho - movimento inválido
                        pecas_adversarias = 2;
                        break;
                    }
                }
            }
            
            // Se tiver apenas uma peça adversária no caminho, é captura
            if (pecas_adversarias == 1) {
                damas[destino_lin][destino_col] = peca;
                damas[origem_lin][origem_col] = ' ';
                damas[ultima_lin][ultima_col] = ' ';
                
                // Atualiza contadores
                if (peca == 'p' || peca == 'd') countTeamRed++; 
                else countTeamGreen++;
                
                // Verifica captura múltipla
                podeCapturarNovamente = verificarCapturaMultipla(damas, destino_lin, destino_col, peca);
                if (podeCapturarNovamente) {
                    origem_lin = destino_lin;
                    origem_col = destino_col;
                    continue;
                }
                return 0;
            }
        }
        
        // Movimento livre para damas (sem captura)
        if (peca == 'd' || peca == 'D') {
            int passo_lin = (diff_lin > 0) ? 1 : -1;
            int passo_col = (diff_col > 0) ? 1 : -1;
            int caminhoLivre = 1;

            // Verifica se o caminho está livre
            for (int i = 1; i < abs_diff_lin; i++) {
                int linha_atual = origem_lin + i * passo_lin;
                int coluna_atual = origem_col + i * passo_col;
                
                if (damas[linha_atual][coluna_atual] != ' ') {
                    caminhoLivre = 0;
                    break;
                }
            }

            if (caminhoLivre) {
                damas[destino_lin][destino_col] = peca;
                damas[origem_lin][origem_col] = ' ';
                return 0;
            } else {
                printf("\n\033[1;33mMovimento invalido: Caminho bloqueado para a dama.\033[0m\n");
                continue;
            }
        }
        // Movimento simples para pedras
        else if (abs_diff_lin == 1) {
            // Verifica direção correta
            if ((peca == 'p' && destino_lin < origem_lin) || 
                (peca == 'P' && destino_lin > origem_lin)) {
                damas[destino_lin][destino_col] = peca;
                damas[origem_lin][origem_col] = ' ';
                
                // Promoção a dama
                if ((peca == 'p' && destino_lin == 0) || (peca == 'P' && destino_lin == 7)) {
                    if (peca == 'p') {
                        damas[destino_lin][destino_col] = 'd';
                    } else {
                        damas[destino_lin][destino_col] = 'D';
                    }
                }
                return 0;
            } else {
                printf("\n\033[1;33mMovimento invalido: Direcao incorreta para pedra.\033[0m\n");
                continue;
            }
        } else {
            printf("\n\033[1;33mMovimento invalido.\033[0m\n");
            continue;
        }
    }
}

int verificarCapturaMultipla(char damas[8][8], int linha, int coluna, char peca) {
    int direcoes[4][2] = {{-1,-1}, {-1,1}, {1,-1}, {1,1}};

    for (int i = 0; i < 4; i++) {
        int dir_lin = direcoes[i][0];
        int dir_col = direcoes[i][1];
        
        // Para damas, verifica toda a diagonal
        if (peca == 'd' || peca == 'D') {
            int encontrouAdversaria = 0;
            
            for (int dist = 1; dist < 8; dist++) {
                int meio_lin = linha + dist * dir_lin;
                int meio_col = coluna + dist * dir_col;
                
                if (meio_lin < 0 || meio_lin > 7 || meio_col < 0 || meio_col > 7) break;
                
                char peca_atual = damas[meio_lin][meio_col];
                
                // Se é peça aliada, não pode capturar nesta direção
                if ((peca == 'd' && (peca_atual == 'p' || peca_atual == 'd')) ||
                    (peca == 'D' && (peca_atual == 'P' || peca_atual == 'D'))) {
                    break;
                }
                
                // Se é peça adversária
                char adv = (peca == 'd') ? 'P' : 'p';
                char adv_dama = (peca == 'd') ? 'D' : 'd';
                
                if (peca_atual == adv || peca_atual == adv_dama) {
                    // Verifica se tem espaço após a peça adversária
                    int prox_lin = meio_lin + dir_lin;
                    int prox_col = meio_col + dir_col;
                    
                    if (prox_lin >= 0 && prox_lin < 8 && prox_col >= 0 && prox_col < 8 &&
                        damas[prox_lin][prox_col] == ' ') {
                        return 1;
                    }
                    break;
                }
            }
        } 
        // Para pedras
        else {
            int meio_lin = linha + dir_lin;
            int meio_col = coluna + dir_col;
            int destino_lin = linha + 2 * dir_lin;
            int destino_col = coluna + 2 * dir_col;

            if (destino_lin >= 0 && destino_lin < 8 && destino_col >= 0 && destino_col < 8) {
                char adv = (peca == 'p') ? 'P' : 'p';
                char adv_dama = (peca == 'p') ? 'D' : 'd';
                
                if ((damas[meio_lin][meio_col] == adv || damas[meio_lin][meio_col] == adv_dama) && 
                    damas[destino_lin][destino_col] == ' ') {
                    return 1;
                }
            }
        }
    }
    return 0;
}

void IniciarJogo(){

    char damas[8][8] = {
        {' ', 'P', ' ', 'P', ' ', 'P', ' ', 'P'},
        {'P', ' ', 'P', ' ', 'P', ' ', 'P', ' '},
        {' ', 'P', ' ', 'P', ' ', 'P', ' ', 'P'},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {'p', ' ', 'p', ' ', 'p', ' ', 'p', ' '},
        {' ', 'p', ' ', 'p', ' ', 'p', ' ', 'p'},
        {'p', ' ', 'p', ' ', 'p', ' ', 'p', ' '}

    };

    char simbolo = 'p'; //Jogador que começa
    
    countTeamGreen = 0, countTeamRed = 0;
    int ContinuarJogo = 1;

    while(ContinuarJogo){

        Tabuleiro(damas);
        SalvarPosicoes(damas,simbolo);
        

        if(countTeamRed == 12){
            printf("\n\033[1;32mTime Verde venceu!\033[0m\n ");
            ContinuarJogo = 0;
        }
        else if(countTeamGreen == 12){
            printf("\n\033[1;31mTime Vermelho venceu!\033[0m\n ");
            ContinuarJogo = 0;
        }

        if(simbolo == 'p')simbolo = 'P';
        else simbolo = 'p';
    }

    printf("\n+------------------------------------+\n");
    printf("|\t      FIM DO JOGO\t     |\n");
    printf("+------------------------------------+\n");   

    printf("\n\nPressione Enter para voltar ao menu...");
    getchar();
    getchar();

}

void Regras(){

    system("cls");

    printf("\n+------------------------------------+\n");
    printf("|\t        Regras\t\t     |\n");
    printf("+------------------------------------+\n");

    printf("\n\n1- O jogo e jogado em um tabuleiro de 64 casas.\n");
    printf("2- Cada jogador tem 12 pecas.\n");
    printf("3- As pecas movem-se diagonalmente, para os lados (Apenas para frente).\n");
    printf("4- A captura e obrigatoria.\n");
    printf("5- Quando uma peça chega a ultima fileira torna-se Dama.\n");
    printf("6- A Dama pode se mover para frente e para tras, quantas casas quiser.\n");
    printf("7- A Dama nao pode saltar uma peca da mesma cor.\n");
    printf("8- O objetivo do jogo e imobilizar ou capturar todas as pecas do adversario\n");

    printf("\n\nPressione Enter para voltar ao menu...");
    getchar();
    getchar();

    system("cls");

}

void Creditos(){

    system("cls");

    printf("\n+------------------------------------+\n");
    printf("|\t       Creditos\t\t     |\n");
    printf("+------------------------------------+\n");

    printf("\nDesenvolvido by: Guilherme Maia");

    printf("\n\nPressione Enter para voltar ao menu...");
    getchar();
    getchar(); 

    system("cls");


}

int main(){

    while(1){    

        system("cls");

        printf("\n+------------------------------------+\n");
        printf("|\t        DAMAS\t\t     |\n");
        printf("+------------------------------------+\n");

        int codigo;

        printf("\n\n1- Jogar");
        printf("\n2- Regras");
        printf("\n3- Creditos");
        printf("\n4- Sair");
        printf("\nCodigo: ");
        scanf("%d", &codigo);

        switch (codigo)
        {
        case 1:
            IniciarJogo();
            break;
        case 2:
            Regras();
            break;
        case 3:
            Creditos();
            break;
        case 4:
            printf("\nObrigado por jogar...");
            return 0;
            break;
        default:
            printf("\nInforme um codigo valido!");
            printf("\n\nPressione Enter para continuar...");
            getchar();
            getchar();
            continue;
        }

    }

    return 0;
}