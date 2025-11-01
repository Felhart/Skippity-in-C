// https://youtu.be/gKkRTHCrcHo
// https://youtu.be/gKkRTHCrcHo
// https://youtu.be/gKkRTHCrcHo
// https://youtu.be/gKkRTHCrcHo
// https://youtu.be/gKkRTHCrcHo
// https://youtu.be/gKkRTHCrcHo
// https://youtu.be/gKkRTHCrcHo
// https://youtu.be/gKkRTHCrcHo
// https://youtu.be/gKkRTHCrcHo
// https://youtu.be/gKkRTHCrcHo



#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

typedef struct player {
    int A;
    int B;
    int C;
    int D;
    int seriSayisi;
} PLAYER;

typedef struct {
    char** board;
    int boardSize;
    PLAYER player1;
    PLAYER player2;
    int isSinglePlayer;
    PLAYER* currentPlayer;
} GameState;

GameState savedState;



GameState savedState;

typedef struct {
    int x;
    int y;
    int nx;
    int ny;
    int kazanilanTas;
} Hamle;


int enAzTasSayisi(PLAYER player) {
    int min = player.A;
    if (player.B < min) min = player.B;
    if (player.C < min) min = player.C;
    if (player.D < min) min = player.D;
    return min;
}

char enAzTasTuruBul(PLAYER player) {
    int minTas = enAzTasSayisi(player);
    if (player.A == minTas) return 'A';
    if (player.B == minTas) return 'B';
    if (player.C == minTas) return 'C';
    if (player.D == minTas) return 'D';
    return ' '; // Bu durum gerçekleşmemeli
}




void saveGameToFile(GameState savedState, PLAYER* currentPlayer) {
    FILE *file = fopen("game_save.txt", "w");
    if (file == NULL) {
        printf("Oyun kaydedilemedi.\n");
        return;
    }

    // Tahta boyutu
    fprintf(file, "%d\n", savedState.boardSize);

    // Tahta durumu
    for (int i = 0; i < savedState.boardSize; i++) {
        for (int j = 0; j < savedState.boardSize; j++) {
            fprintf(file, "%c ", savedState.board[i][j]);
        }
        fprintf(file, "\n");
    }

    // Oyuncu durumları
    fprintf(file, "%d %d %d %d %d\n", savedState.player1.A, savedState.player1.B, savedState.player1.C, savedState.player1.D, savedState.player1.seriSayisi);
    fprintf(file, "%d %d %d %d %d\n", savedState.player2.A, savedState.player2.B, savedState.player2.C, savedState.player2.D, savedState.player2.seriSayisi);

    // Oyun modu
    fprintf(file, "%d\n", savedState.isSinglePlayer);

    // Sıra kimde
    fprintf(file, "%d\n", (currentPlayer == &savedState.player1) ? 1 : 2);

    fclose(file);
    printf("Oyun kaydedildi ve cikiliyor.\n");
    exit(0);
}


void loadGameFromFile(char*** board, int* N, PLAYER* player1, PLAYER* player2, int* isSinglePlayer, PLAYER** currentPlayer) {
    FILE *file = fopen("game_save.txt", "r");
    if (file == NULL) {
        printf("Kayitli oyun bulunamadi.\n");
        return;
    }

    // Tahta boyutu
    fscanf(file, "%d\n", N);

    // Tahta oluşturma
    *board = (char**)malloc((*N) * sizeof(char*));
    for (int i = 0; i < *N; i++) {
        (*board)[i] = (char*)malloc((*N) * sizeof(char));
        for (int j = 0; j < *N; j++) {
            fscanf(file, " %c", &(*board)[i][j]);
        }
    }

    // Oyuncu durumları
    fscanf(file, "%d %d %d %d %d\n", &player1->A, &player1->B, &player1->C, &player1->D, &player1->seriSayisi);
    fscanf(file, "%d %d %d %d %d\n", &player2->A, &player2->B, &player2->C, &player2->D, &player2->seriSayisi);

    // Oyun modu
    fscanf(file, "%d\n", isSinglePlayer);

    // Sıra kimde
    int currentPlayerNum;
    fscanf(file, "%d\n", &currentPlayerNum);
    *currentPlayer = (currentPlayerNum == 1) ? player1 : player2;

    fclose(file);
    printf("Kayitli oyun yüklendi.\n");
}




void saveGameState(char** board, int N, PLAYER player1, PLAYER player2, int isSinglePlayer, PLAYER* currentPlayer) {
    savedState.boardSize = N;
    savedState.board = (char**)malloc(N * sizeof(char*));
    for (int i = 0; i < N; i++) {
        savedState.board[i] = (char*)malloc(N * sizeof(char));
        memcpy(savedState.board[i], board[i], N * sizeof(char));
    }
    savedState.player1 = player1;
    savedState.player2 = player2;
    savedState.isSinglePlayer = isSinglePlayer;
    savedState.currentPlayer = currentPlayer;
}

void loadGameState(char** board, int N, PLAYER* player1, PLAYER* player2, int* isSinglePlayer, PLAYER** currentPlayer) {
    for (int i = 0; i < N; i++) {
        memcpy(board[i], savedState.board[i], N * sizeof(char));
    }
    *player1 = savedState.player1;
    *player2 = savedState.player2;
    *isSinglePlayer = savedState.isSinglePlayer;
    *currentPlayer = savedState.currentPlayer;
}



int menuGoster() {
    int choice;
    printf("\n1-Multiplayer\n");
    printf("2-Singleplayer\n");
    printf("3-Kayitli Oyun Yukle\n");
    printf("4-Exit");
    scanf("%d", &choice);
    return choice;
}


void skorGoster(PLAYER player1, PLAYER player2) {
    printf("\nPlayer 1 Skor: A=%d, B=%d, C=%d, D=%d, Seri Sayisi=%d\n", 
           player1.A, player1.B, player1.C, player1.D, player1.seriSayisi);
    printf("Player 2 Skor: A=%d, B=%d, C=%d, D=%d, Seri Sayisi=%d\n", 
           player2.A, player2.B, player2.C, player2.D, player2.seriSayisi);
}


char** tahtaOlustur(int *boyut) {
    int N, r, i, j, tasSayisi;
    char *taslar, **tahta;
    printf("NxN'lik tahta boyutunu girin (6-20 arasi ve cift sayi): ");
    scanf("%d", &N);
    while (N < 4 || N % 2 != 0 || N > 20) {
        printf("Girilecek deger 6-20 arasinda ve cift olmali: ");
        scanf("%d", &N);
    }
    *boyut = N;
    tahta = (char**)(malloc(sizeof(char*) * N));
    for (i = 0; i < N; i++) {
        tahta[i] = (char*)(malloc(sizeof(char) * N));
    }
    taslar = (char*)(malloc(sizeof(char) * (N * N - 4)));
    tasSayisi = N * N - 4;

    srand(time(NULL));
    for (i = 0; i < tasSayisi; i++) {
        if (i < tasSayisi / 4) {
            taslar[i] = 'A';
        } else if (i < tasSayisi / 2) {
            taslar[i] = 'B';
        } else if (i < 3 * tasSayisi / 4) {
            taslar[i] = 'C';
        } else {
            taslar[i] = 'D';
        }
    }

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if ((i == N / 2 && j == N / 2) || (i == N / 2 - 1 && j == N / 2) ||
                (i == N / 2 && j == N / 2 - 1) || (i == N / 2 - 1 && j == N / 2 - 1)) {
                tahta[i][j] = '-';
            } else {
                r = rand() % (tasSayisi);
                tahta[i][j] = taslar[r];
                taslar[r] = taslar[tasSayisi - 1];
                tasSayisi--;
            }
        }
    }
    free(taslar);
    return tahta;
}

void tahtaGoster(char** tahta, int N) {
    int i, j;
    printf("     ");
    for ( i = 0; i < N; i++){
        printf(" %d ", i);
    }
    printf("\n");
    for (i = 0; i < N; i++) {
        printf("\n");
        printf(" %d   ", i);
        for (j = 0; j < N; j++) {
            printf(" %c ", tahta[i][j]);
        }
    }
    printf("\n");
}

int tasKontrol(int x, int y, char **board, int N) {
    if (board[x][y] != 'A' && board[x][y] != 'B' && board[x][y] != 'C' && board[x][y] != 'D') {
        printf("Konumda tas yok\n");
        return 0;
    }
    int oynanabilecekHamleSayisi = 0;
    if (x + 2 < N && board[x + 1][y] != '-' && board[x + 2][y] == '-') oynanabilecekHamleSayisi++;
    if (x - 2 >= 0 && board[x - 1][y] != '-' && board[x - 2][y] == '-') oynanabilecekHamleSayisi++;
    if (y + 2 < N && board[x][y + 1] != '-' && board[x][y + 2] == '-') oynanabilecekHamleSayisi++;
    if (y - 2 >= 0 && board[x][y - 1] != '-' && board[x][y - 2] == '-') oynanabilecekHamleSayisi++;

    if (oynanabilecekHamleSayisi == 0) {
        printf("Tasin oynayabilecegi gecerli hamle yok\n");
        return 0;
    }
    return 1;
}

int konumKontrol(int x, int ox, int y, int oy, char **board, int N) {
    if (x - ox == 2 && y == oy && board[x][y] == '-' && board[ox + 1][y] != '-') return 1;
    if (x - ox == -2 && y == oy && board[x][y] == '-' && board[ox - 1][y] != '-') return 1;
    if (y - oy == 2 && x == ox && board[x][y] == '-' && board[x][oy + 1] != '-') return 1;
    if (y - oy == -2 && x == ox && board[x][y] == '-' && board[x][oy - 1] != '-') return 1;

    printf("Gecersiz konum\n");
    return 0;
}

void tasTopla(int x, int y, int ox, int oy, PLAYER* player, char **board) {
    if (x - ox == 2) {
        if (board[ox + 1][y] == 'A') player->A++;
        if (board[ox + 1][y] == 'B') player->B++;
        if (board[ox + 1][y] == 'C') player->C++;
        if (board[ox + 1][y] == 'D') player->D++;
        board[ox + 1][y] = '-';
    }
    if (x - ox == -2) {
        if (board[ox - 1][y] == 'A') player->A++;
        if (board[ox - 1][y] == 'B') player->B++;
        if (board[ox - 1][y] == 'C') player->C++;
        if (board[ox - 1][y] == 'D') player->D++;
        board[ox - 1][y] = '-';
    }
    if (y - oy == 2) {
        if (board[x][oy + 1] == 'A') player->A++;
        if (board[x][oy + 1] == 'B') player->B++;
        if (board[x][oy + 1] == 'C') player->C++;
        if (board[x][oy + 1] == 'D') player->D++;
        board[x][oy + 1] = '-';
    }
    if (y - oy == -2) {
        if (board[x][oy - 1] == 'A') player->A++;
        if (board[x][oy - 1] == 'B') player->B++;
        if (board[x][oy - 1] == 'C') player->C++;
        if (board[x][oy - 1] == 'D') player->D++;
        board[x][oy - 1] = '-';
    }
}



void move(char **tahta, int N, PLAYER *currentPlayer, PLAYER *player1, PLAYER *player2, int isSinglePlayer) {
    int str, stn, oldStr, oldStn, kontrol1, kontrol2;
    int undoUsed = 0;
    saveGameState(tahta, N, *player1, *player2, isSinglePlayer, currentPlayer);
    
    // İlk taşın koordinatını al
    while (1) {
        kontrol1 = 0;
        while (kontrol1 == 0) {
            printf("\nOynayacaginiz tasin koordinatini girin (örn: 2 3): ");
            scanf("%d %d", &oldStr, &oldStn);
            if (oldStr == -2 && oldStn == -2 && undoUsed) {
                printf("Hamle zaten geri alindi, baska hamle secin.\n");
                continue;
            }
            if (oldStr == -3 && oldStn == -3) {
                saveGameToFile(savedState, currentPlayer);
                return;
            }
            kontrol1 = tasKontrol(oldStr, oldStn, tahta, N);
        }

        // Hamle öncesi durumu kaydet
        

        while (1) {
            printf("\nTasi nereye oynayacaksiniz (Turu bitirmek icin -1 -1, hamleyi geri almak için -2 -2, kaydetmek icin -3 -3 girin): ");
            scanf("%d %d", &str, &stn);

            if (str == -2 && stn == -2 && !undoUsed) {
                // Hamleyi geri al
                loadGameState(tahta, N, player1, player2, &isSinglePlayer, &currentPlayer);
                printf("Hamle geri alindi.\n");
                tahtaGoster(tahta, N);
                skorGoster(*player1, *player2);
                undoUsed = 1;
                break;
            }
            else if(str == -2 && stn == -2 && undoUsed){
                printf("Hakkinizi zaten kullandiniz.");
            }

            else if (str == -1 && stn == -1) {
                return; // Kullanıcı turu bitirmek istiyor
            }

            else if (str == -3 && stn == -3) {
                saveGameToFile(savedState, currentPlayer);
                return;
            }

            kontrol2 = konumKontrol(str, oldStr, stn, oldStn, tahta, N);
            if (kontrol2 == 1) {
                tasTopla(str, stn, oldStr, oldStn, currentPlayer, tahta);
                tahta[str][stn] = tahta[oldStr][oldStn];
                tahta[oldStr][oldStn] = '-';

                tahtaGoster(tahta, N); // Hamle sonrası tahtayı göster
                skorGoster(*player1, *player2); // Hamle sonrası skoru göster

                oldStr = str;
                oldStn = stn;

            } else {
                printf("Gecersiz hamle. Lutfen tekrar deneyin.\n");
            }
        }
    }
}




void hamleEkle(Hamle* hamleler, int* hamleSayisi, int x, int y, int nx, int ny, int kazanilanTas) {
    hamleler[*hamleSayisi].x = x;
    hamleler[*hamleSayisi].y = y;
    hamleler[*hamleSayisi].nx = nx;
    hamleler[*hamleSayisi].ny = ny;
    hamleler[*hamleSayisi].kazanilanTas = kazanilanTas;
    (*hamleSayisi)++;
}




int hamleTasSayisiniHesapla(char **board, int N, int x, int y, int dx, int dy) {
    if (x + 2 * dx >= 0 && x + 2 * dx < N && y + 2 * dy >= 0 && y + 2 * dy < N &&
        board[x + dx][y + dy] != '-' && board[x + 2 * dx][y + 2 * dy] == '-') {
        return 1;
    }
    return 0;
}

int min(int a, int b) {
    return (a < b) ? a : b;
}

int min3(int a, int b, int c) {
    return min(min(a, b), c);
}

int min4(int a, int b, int c, int d) {
    return min(min(a, b), min(c, d));
}


void bilgisayarHamlesiYap(char **board, int N, PLAYER *computer, PLAYER *opponent) {
    int dx[4] = {-1, 1, 0, 0}; // Yukarı, Aşağı, Sol, Sağ
    int dy[4] = {0, 0, -1, 1};

    int maxKazanilanTas = 0;
    Hamle secilenHamle;
    secilenHamle.kazanilanTas = 0;

    int minTasBilgisayar = fmin(fmin(computer->A, computer->B), fmin(computer->C, computer->D));
    int minTasRakip = fmin(fmin(opponent->A, opponent->B), fmin(opponent->C, opponent->D));

    // Öncelik sırasına göre hamleleri kontrol et: 1. Kendi en az taşını almak, 2. Rakibin en az taşını almak, 3. En uzun yolu seçmek

    // Adım 1: Kendi en az taşını almak
    for (int x = 0; x < N; x++) {
        for (int y = 0; y < N; y++) {
            if (board[x][y] == 'A' || board[x][y] == 'B' || board[x][y] == 'C' || board[x][y] == 'D') {
                for (int i = 0; i < 4; i++) {
                    int curX = x, curY = y;
                    int kazanilanTas = 0;
                    while (1) {
                        int nextX = curX + 2 * dx[i];
                        int nextY = curY + 2 * dy[i];
                        if (hamleTasSayisiniHesapla(board, N, curX, curY, dx[i], dy[i])) {
                            curX = nextX;
                            curY = nextY;
                            if ((board[curX][curY] == 'A' && computer->A == minTasBilgisayar) ||
                                (board[curX][curY] == 'B' && computer->B == minTasBilgisayar) ||
                                (board[curX][curY] == 'C' && computer->C == minTasBilgisayar) ||
                                (board[curX][curY] == 'D' && computer->D == minTasBilgisayar)) {
                                kazanilanTas++;
                            }
                        } else {
                            break;
                        }
                    }
                    if (kazanilanTas > maxKazanilanTas) {
                        maxKazanilanTas = kazanilanTas;
                        secilenHamle.x = x;
                        secilenHamle.y = y;
                        secilenHamle.nx = curX;
                        secilenHamle.ny = curY;
                        secilenHamle.kazanilanTas = kazanilanTas;
                    }
                }
            }
        }
    }

    // Adım 2: Rakibin en az taşını almak
    if (maxKazanilanTas == 0) {
        for (int x = 0; x < N; x++) {
            for (int y = 0; y < N; y++) {
                if (board[x][y] == 'A' || board[x][y] == 'B' || board[x][y] == 'C' || board[x][y] == 'D') {
                    for (int i = 0; i < 4; i++) {
                        int curX = x, curY = y;
                        int kazanilanTas = 0;
                        while (1) {
                            int nextX = curX + 2 * dx[i];
                            int nextY = curY + 2 * dy[i];
                            if (hamleTasSayisiniHesapla(board, N, curX, curY, dx[i], dy[i])) {
                                curX = nextX;
                                curY = nextY;
                                if ((board[curX][curY] == 'A' && opponent->A == minTasRakip) ||
                                    (board[curX][curY] == 'B' && opponent->B == minTasRakip) ||
                                    (board[curX][curY] == 'C' && opponent->C == minTasRakip) ||
                                    (board[curX][curY] == 'D' && opponent->D == minTasRakip)) {
                                    kazanilanTas++;
                                }
                            } else {
                                break;
                            }
                        }
                        if (kazanilanTas > maxKazanilanTas) {
                            maxKazanilanTas = kazanilanTas;
                            secilenHamle.x = x;
                            secilenHamle.y = y;
                            secilenHamle.nx = curX;
                            secilenHamle.ny = curY;
                            secilenHamle.kazanilanTas = kazanilanTas;
                        }
                    }
                }
            }
        }
    }

    // Adım 3: En uzun yolu seçmek
    if (maxKazanilanTas == 0) {
        for (int x = 0; x < N; x++) {
            for (int y = 0; y < N; y++) {
                if (board[x][y] == 'A' || board[x][y] == 'B' || board[x][y] == 'C' || board[x][y] == 'D') {
                    for (int i = 0; i < 4; i++) {
                        int curX = x, curY = y;
                        int kazanilanTas = 0;
                        while (1) {
                            int nextX = curX + 2 * dx[i];
                            int nextY = curY + 2 * dy[i];
                            if (hamleTasSayisiniHesapla(board, N, curX, curY, dx[i], dy[i])) {
                                curX = nextX;
                                curY = nextY;
                                kazanilanTas++;
                            } else {
                                break;
                            }
                        }
                        if (kazanilanTas > maxKazanilanTas) {
                            maxKazanilanTas = kazanilanTas;
                            secilenHamle.x = x;
                            secilenHamle.y = y;
                            secilenHamle.nx = curX;
                            secilenHamle.ny = curY;
                            secilenHamle.kazanilanTas = kazanilanTas;
                        }
                    }
                }
            }
        }
    }

    // Seçilen hamleyi uygula
    if (secilenHamle.kazanilanTas > 0) {
        int curX = secilenHamle.x;
        int curY = secilenHamle.y;

        while (secilenHamle.kazanilanTas > 0) {
            int dirX = (secilenHamle.nx - curX) / (2 * secilenHamle.kazanilanTas);
            int dirY = (secilenHamle.ny - curY) / (2 * secilenHamle.kazanilanTas);

            int midX = curX + dirX;
            int midY = curY + dirY;
            int nextX = curX + 2 * dirX;
            int nextY = curY + 2 * dirY;

            // Skoru güncelle ve taşı boşalt
            tasTopla(nextX, nextY, curX, curY, computer, board);
            board[midX][midY] = '-';

            curX = nextX;
            curY = nextY;
            secilenHamle.kazanilanTas--;

            // Yeni hamle yönünü güncelle
            int yeniKazanilanTas = 0;
            for (int j = 0; j < 4; j++) {
                if (hamleTasSayisiniHesapla(board, N, curX, curY, dx[j], dy[j])) {
                    secilenHamle.nx = curX + 2 * dx[j];
                    secilenHamle.ny = curY + 2 * dy[j];
                    secilenHamle.kazanilanTas = 1; // L hamlesi için 1 taşı hesaba katar
                    yeniKazanilanTas = 1;
                    break;
                }
            }

            if (yeniKazanilanTas == 0) break; // Oynayacak başka taş kalmadıysa döngüden çık
        }

        board[curX][curY] = board[secilenHamle.x][secilenHamle.y];
        board[secilenHamle.x][secilenHamle.y] = '-';
    }
}








int oynanabilecekTasKaldiMi(char **board, int N) {
    for (int x = 0; x < N; x++) {
        for (int y = 0; y < N; y++) {
            if (board[x][y] == 'A' || board[x][y] == 'B' || board[x][y] == 'C' || board[x][y] == 'D') {
                if ((x + 2 < N && board[x + 1][y] != '-' && board[x + 2][y] == '-') ||
                    (x - 2 >= 0 && board[x - 1][y] != '-' && board[x - 2][y] == '-') ||
                    (y + 2 < N && board[x][y + 1] != '-' && board[x][y + 2] == '-') ||
                    (y - 2 >= 0 && board[x][y - 1] != '-' && board[x][y - 2] == '-')) {
                    return 1; // Oynanabilecek hamle var
                }
            }
        }
    }
    return 0; // Oynanabilecek hamle yok
}



void kazananOyuncu(PLAYER player1, PLAYER player2) {
    int player1_seri = enAzTasSayisi(player1);
    int player2_seri = enAzTasSayisi(player2);

    if (player1_seri > player2_seri) {
        printf("Kazanan: Player 1\n");
    } else if (player2_seri > player1_seri) {
        printf("Kazanan: Player 2\n");
    } else {
        // Eğer seriler eşitse, toplam taş sayılarını karşılaştır
        int player1_toplamTas = player1.A + player1.B + player1.C + player1.D;
        int player2_toplamTas = player2.A + player2.B + player2.C + player2.D;

        if (player1_toplamTas > player2_toplamTas) {
            printf("Kazanan: Player 1\n");
        } else if (player2_toplamTas > player1_toplamTas) {
            printf("Kazanan: Player 2\n");
        } else {
            printf("Oyun berabere!\n");
        }
    }
}




int main() {
    int choice, oyun = 1, boyut;
    char **tahta = NULL;
    PLAYER player1 = {0, 0, 0, 0, 0}; // İnsan oyuncu
    PLAYER player2 = {0, 0, 0, 0, 0}; // Bilgisayar veya ikinci oyuncu
    PLAYER *currentPlayer;
    int isSinglePlayer = 0;

    choice = menuGoster();
    while (choice != 4) {
        switch (choice) {
            case 1: // Multiplayer
                tahta = tahtaOlustur(&boyut);
                currentPlayer = &player1;
                isSinglePlayer = 0;
                while (oyun == 1) {
                    printf("\n*****************************************\n");
                    printf("********** SIRA %s'DE **********\n", (currentPlayer == &player1) ? "PLAYER 1" : "PLAYER 2");
                    printf("*****************************************\n");
                    tahtaGoster(tahta, boyut);
                    move(tahta, boyut, currentPlayer, &player1, &player2, isSinglePlayer);
                    skorGoster(player1, player2);
                    currentPlayer = (currentPlayer == &player1) ? &player2 : &player1;

                    // Oyun bitiş kontrolü
                    if (!oynanabilecekTasKaldiMi(tahta, boyut)) {
                        printf("Oyun bitti! Oynanabilecek taş kalmadı.\n");
                        oyun = 0;
                        kazananOyuncu(player1, player2); // Kazananı belirle
                    }
                }
                break;
            case 2: // Singleplayer
                tahta = tahtaOlustur(&boyut);
                currentPlayer = &player1;
                isSinglePlayer = 1;
                while (oyun == 1) {
                    printf("\n*****************************************\n");
                    printf("********** SIRA PLAYER 1'DE **********\n");
                    printf("*****************************************\n");
                    tahtaGoster(tahta, boyut);
                    if(currentPlayer == &player1){
                    move(tahta, boyut, currentPlayer, &player1, &player2, isSinglePlayer);
                    skorGoster(player1, player2);
                    currentPlayer = (currentPlayer == &player1) ? &player2 : &player1;
                    }
                    else if(currentPlayer == &player2){
                        // Bilgisayarın hamlesi
                        printf("\n*****************************************\n");
                        printf("********** SIRA BILGISAYARDA **********\n");
                        printf("*****************************************\n");
                        bilgisayarHamlesiYap(tahta, boyut, &player2, &player1);
                        tahtaGoster(tahta, boyut);
                        skorGoster(player1, player2);
                        currentPlayer = (currentPlayer == &player1) ? &player2 : &player1;
                    }
                    

                    // Oyun bitiş kontrolü
                    if (!oynanabilecekTasKaldiMi(tahta, boyut)) {
                        printf("Oyun bitti! Oynanabilecek tas kalmadi.\n");
                        oyun = 0;
                        kazananOyuncu(player1, player2); // Kazananı belirle
                    } 
                }
                break;
            case 3: // Kayıtlı oyun yükle
                loadGameFromFile(&tahta, &boyut, &player1, &player2, &isSinglePlayer, &currentPlayer);
                oyun = 1;
                while (oyun == 1) {
                    printf("\n*****************************************\n");
                    printf("********** SIRA %s'DE **********\n", (currentPlayer == &player1) ? "PLAYER 1" : (isSinglePlayer ? "BILGISAYAR" : "PLAYER 2"));
                    printf("*****************************************\n");
                    tahtaGoster(tahta, boyut);
                    if (isSinglePlayer && currentPlayer == &player2) {
                        bilgisayarHamlesiYap(tahta, boyut, &player2, &player1);
                        tahtaGoster(tahta, boyut);
                        skorGoster(player1, player2);
                        currentPlayer = &player1;
                    } else {
                        move(tahta, boyut, currentPlayer, &player1, &player2, isSinglePlayer);
                        tahtaGoster(tahta, boyut);
                        skorGoster(player1, player2);
                        currentPlayer = (currentPlayer == &player1) ? &player2 : &player1;
                    }

                    // Oyun bitiş kontrolü
                    if (!oynanabilecekTasKaldiMi(tahta, boyut)) {
                        printf("Oyun bitti! Oynanabilecek tas kalmadi.\n");
                        oyun = 0;
                        kazananOyuncu(player1, player2); // Kazananı belirle
                    }
                }
                break;

            default:
                printf("Gecerli bir deger girin: ");
                scanf("%d", &choice);
        }
        choice = menuGoster();  // Menüye dön
    }
    if (tahta != NULL) {
        for (int i = 0; i < boyut; i++) {
            free(tahta[i]);
        }
        free(tahta);
    }
    return 0;
}








