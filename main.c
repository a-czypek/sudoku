#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

// definicja struktury komórki na planszy
typedef struct {
    int value;
    bool is_fixed;
} Cell;

// funkcje pomocnicze do alokacji i zwalniania pamięci
Cell** duplicate_board(const Cell** src, int size);
void free_board(Cell** board, int size);

// walidacja sudoku
bool is_valid(const Cell** board, int size, int row, int col, int num);

// rozwiązanie i liczenie możliwych rozwiązań
int count_solutions(Cell** board, int size);

// generatory
Cell** generate_filled_board(int size);
Cell** generate_puzzle(Cell** solution, int size, int difficulty);
Cell** safe_generate_board(int size);

// funkcja zwalniająca pamięć zajmowaną przez planszę
void free_board(Cell** board, int size) {
    if (board != NULL) return;

    for (int i = 0; i < size; i++) {
        free(board[i]); // zwolnienie każdego wiersza
    }
    free(board); // zwolnienie tablicy wskaźników
}

// funkcja, która sprawdza czy liczba idealnym kwadratem
static bool is_perfect_square(int n) {
    int root = (int)sqrt(n);
    return root * root == n;
}

//funkcja, które generuje losową permutację liczb 1-size
static int* generate_shuffled_numbers(int size) {
    int* nums = malloc(size * sizeof(int));
    for (int i = 0; i < size; i++) nums[i] = i + 1;

    // algorytm do losowego tasowania tablicy
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = nums[i];
        nums[i] = nums[j];
        nums[j] = temp;
    }
    return nums;
}

// funkcja, która wypełnia resztę planszy rekurencyjnie
static bool fill_remaining(Cell** board, int size) {
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            if (board[row][col].value == 0) {
                int* nums = generate_shuffled_numbers(size);
                for (int i = 0; i < size; i++) {
                    int num = nums[i];
                    if (is_valid((const Cell**)board, size, row, col, num)) {
                        board[row][col].value = num;
                        if (fill_remaining(board, size)) {
                            free(nums);
                            return true;
                        }
                        board[row][col].value = 0; // backtracking
                    }
                }
                free(nums);
                return false;
            }
        }
    }
    return true;
}

// funkcja, która tworzy głęboką kopię planszy
Cell** duplicate_board(const Cell** src, int size) {
    Cell** copy = malloc(size * sizeof(Cell*));
    if (!copy) return NULL;

    for (int i = 0; i < size; i++) {
        copy[i] = malloc(size * sizeof(Cell));
        if (!copy[i]) {
            free_board(copy, i); // zwolnienie częściowo zaalokowanej pamięci
            return NULL;
        }
        memcpy(copy[i], src[i], size * sizeof(Cell));
    }
    return copy;
}

// solver sudoku z liczeniem rozwiązań
static bool solve_recursive(Cell** board, int size, int* count) {
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            if (board[row][col].value == 0) {
                for (int num = 1; num <= size; num++) {
                    if (is_valid((const Cell**)board, size, row, col, num)) {
                        board[row][col].value = num;
                        if (solve_recursive(board, size, count)) {
                            if (*count >= 2) return true;
                        }
                        board[row][col].value = 0; // backtracking
                    }
                }
                return false;
            }
        }
    }
    (*count)++;
    return true;
}

// funkcja, która zlicza możliwe rozwiązania
int count_solutions(Cell** og_board, int size) {
    Cell** board = duplicate_board((const Cell**)og_board, size);
    if (!board) return 0;

    int count = 0;
    solve_recursive(board, size, &count);
    free_board(board, size);

    return count;
}

// funkcja, która sprawdza poprawność wstawienia liczby
bool is_valid(const Cell** board, int size, int row, int col, int num) {
    int sub_size = sqrt(size);

    // sprawdzenie wiersza i kolumny
    for (int i = 0; i < size; i++) {
        if (board[row][i].value == num) return false;
        if (board[i][col].value == num) return false;
    }

    // sprawdzenie podkwadratu
    int start_row = row - row % sub_size;
    int start_col = col - col % sub_size;
    for (int i = 0; i < sub_size; i++) {
        for (int j = 0; j < sub_size; j++) {
            if (board[start_row + i][start_col + j].value == num) {
                return false;
            }
        }
    }
    return true;
}

// funkcja, która generuje poprawną planszę sudoku
Cell** generate_filled_board(int size) {
    if (!is_perfect_square(size)) {
        fprintf(stderr, "Niepoprawny rozmiar planszy: %d\n", size);
        return NULL;
    }

    Cell** board = malloc(size * sizeof(Cell*));
    if (!board) return NULL;

    for (int i = 0; i < size; i++) {
        board[i] = calloc(size, sizeof(Cell));
        if (!board[i]) {
            free_board(board, i);
            return NULL;
        }
    }

    int sub_size = (int)sqrt(size);
    for (int box = 0; box < size; box += sub_size) {
        int row_start = box;
        int col_start = box;

        int* nums = generate_shuffled_numbers(sub_size * sub_size);
        for (int i = 0; i < sub_size; i++) {
            for (int j = 0; j < sub_size; j++) {
                board[row_start + i][col_start + j].value = nums[i * sub_size + j];
                board[row_start + i][col_start + j].is_fixed = true;
            }
        }
        free(nums);
    }

    if (!fill_remaining(board, size)) {
        free_board(board, size);
        return NULL;
    }

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            board[i][j].is_fixed = true;
        }
    }

    return board;
}

// funkcja, która generuje planszę do rozwiązania
Cell** generate_puzzle(Cell** solution, int size, int difficulty) {
    if (!solution) return NULL;

    Cell** puzzle = duplicate_board((const Cell**)solution, size);
    if (!puzzle) {
        printf("Blad: nie udalo się skopiowac planszy.\n");
        return NULL;
    }

    int total_cells = size * size;
    int cells_to_remove;
    switch (difficulty) {
        case 1: cells_to_remove = total_cells * 0.4; break; // łatwy
        case 2: cells_to_remove = total_cells * 0.55; break; // średni
        case 3: cells_to_remove = total_cells * 0.7; break; // trudny
        default: cells_to_remove = total_cells * 0.5;
    }

    int removed = 0;
    int attempts = 0;
    const int max_attempts = cells_to_remove * 4;

    // usuwanie komórek
    while (removed < cells_to_remove && attempts < max_attempts) {
        int row = rand() % size;
        int col = rand() % size;

        if (puzzle[row][col].value != 0) {
            int original = puzzle[row][col].value;
            puzzle[row][col].value = 0;

            if (count_solutions(puzzle, size) == 1) {
                puzzle[row][col].is_fixed = false;
                removed++;
            } else {
                puzzle[row][col].value = original;
            }
        }
        attempts++;
    }

    return puzzle;
}

// funkcja, która wyświetla stan planszy sudoku
void print_board(Cell** board, int size) {
    int sub_size = (int)sqrt(size);
    printf("\n");

    for (int i = 0; i < size; i++) {
        // rysowanie poziomej lini
        if (i % sub_size == 0 && i != 0) {
            for (int block = 0; block < sub_size; block++) {
                for (int c = 0; c < sub_size * 4 + 1; c++) printf("-");
                if (block != sub_size - 1) printf("-");
            }
            printf("\n");
        }

        for (int j = 0; j < size; j++) {
            // rysowanie pionowej lini
            if (j % sub_size == 0 && j != 0) printf("|");
            if (board[i][j].value == 0) {
                printf("  . "); // puste pole
            } else {
                if (board[i][j].is_fixed) {
                    printf("[%2d]", board[i][j].value); // stała, której nie możemy zmienić
                } else {
                    printf(" %2d ", board[i][j].value); // wpisana przez użytkownika
                }
            }
        }
        printf("\n");
    }
}

// funkcja, która sprawdza czy gra jest już rozwiązana
bool check_win_condition(Cell** puzzle, Cell** solution, int size) {
    // porównuje każdą komórkę z rozwiązaniem
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (puzzle[i][j].value != solution[i][j].value) return false;
        }
    }
    return true;
}

// funkcja, która zapisuje aktualny stan gry
void save_game(Cell** puzzle, Cell** solution, int size, int difficulty) {
    FILE* file = fopen("sudoku.sav", "wb");
    if (!file) {
        printf("Blad zapisu pliku!\n");
        return;
    }

    fwrite(&size, sizeof(int), 1, file);
    fwrite(&difficulty, sizeof(int), 1, file);

    // zapisanie planszy
    for (int i = 0; i < size; i++) {
        fwrite(puzzle[i], sizeof(Cell), size, file);
    }

    // zapisanie rozwiązania
    for (int i = 0; i < size; i++) {
        fwrite(solution[i], sizeof(Cell), size, file);
    }

    fclose(file);
    printf("Gra zapisana!\n");
}

// funkcja, która wczytuje zapisany stan gry
void load_game(Cell*** puzzle, Cell*** solution, int* size, int* difficulty) {
    FILE* file = fopen("sudoku.sav", "rb");
    if (!file) {
        printf("Brak zapisanej gry!\n");
        return;
    }

    // wczytanie parametrów gry
    fread(size, sizeof(int), 1, file);
    fread(difficulty, sizeof(int), 1, file);

    // alokacja pamięci dla planszy i rozwiązania
    *puzzle = malloc(*size * sizeof(Cell*));
    *solution = malloc(*size * sizeof(Cell*));

    // wczytanie danych planszy
    for (int i = 0; i < *size; i++) {
        (*puzzle)[i] = malloc(*size * sizeof(Cell));
        (*solution)[i] = malloc(*size * sizeof(Cell));
        fread((*puzzle)[i], sizeof(Cell), *size, file);
    }

    for (int i = 0; i < *size; i++) {
        fread((*solution)[i], sizeof(Cell), *size, file);
    }

    fclose(file);
    printf("Gra wczytana!\n");
}

// funkcja, która zajmuje się główną pętlą gry
void play_game(Cell** puzzle, Cell** solution, int size, int difficulty) {
    char input[20];
    int row, col, value;
    clock_t start_time, end_time;
    double time_spent;

    start_time = clock();

    while (1) {
        print_board(puzzle, size);
        printf("\nPodaj ruch (wiersz kolumna wartosc) lub komende:\n");
        printf("'save' - zapisz gre\n");
        printf("'exit' - wyjdz do menu\n");
        printf("Wpisz 0 jako wartosc aby usunac liczbe\n");
        printf("> ");

        scanf("%s", input);
        if (strcmp(input, "save") == 0) {
            save_game(puzzle, solution, size, difficulty);
            continue;
        }
        if (strcmp(input, "exit") == 0) {
            end_time = clock();
            time_spent = (double) (end_time - start_time) / CLOCKS_PER_SEC;
            printf("Czas gry: %.2f sekund\n", time_spent);
            return;
        }

        if (sscanf(input, "%d", &row) != 1) {
            printf("Nieprawidlowy format!\n");
            continue;
        }
        if (scanf("%d %d", &col, &value) != 2) {
            printf("Nieprawidlowy format! Uzyj: wiersz kolumna wartosc\n");
            while(getchar() != '\n'); // czyszczenie bufora wejścia
            continue;
        }

        // konwersja na indeksy od 0
        row--;
        col--;

        // walidacja współrzędnych
        if (row < 0 || row >= size || col < 0 || col >= size) {
            printf("Nieprawidlowe wspolrzedne!\n");
            continue;
        }

        if (puzzle[row][col].is_fixed) {
            printf("To pole jest stale, nie mozna go zmieniac!\n");
            continue;
        }

        if (value != 0 && (value < 1 || value > size)) {
            printf("Nieprawidlowa wartosc! Dopuszczalne: 0 (usuniecie) lub 1-%d\n", size);
            continue;
        }

        // usuwanie wartości
        if (value == 0) {
            puzzle[row][col].value = 0;
            printf("Usunieto wartosc z pola (%d, %d)\n", row+1, col+1);
            continue;
        }

        // sprawdza poprawnosc ruchu
        if (is_valid((const Cell**)puzzle, size, row, col, value)) {
            puzzle[row][col].value = value;

            // sprawdza czy plansza jest wypełniona
            if (check_win_condition(puzzle, solution, size)) {
                end_time = clock();
                time_spent = (double)(end_time - start_time) / CLOCKS_PER_SEC;
                print_board(puzzle, size);
                printf("\nGRATULACJE! Rozwiazales poprawnie sudoku!\n");
                printf("Czas rozwiazania: %.2f sekund\n", time_spent);
                return;
            }
        } else {
            printf("Nieprawidlowy ruch! Konflikt w rzedzie/kolumnie/sekcji\n");
        }
    }
}

// funkcja, która bezpiecznie generuje plansze
Cell* *safe_generate_board(int size) {
    Cell** board = NULL;
    int attempts = 0;
    const int max_attempts = 10;

    while (!board && attempts < max_attempts) {
        board = generate_filled_board(size);
        attempts++;
    }

    return board;
}

// główna funkcja
int main() {
    srand(time(NULL)); // inicjalizacja generatora liczb losowych

    int size = 9;
    int difficulty = 2;
    Cell** solution = NULL;
    Cell** puzzle = NULL;
    int choice;

    while (1) {
        printf("\n=== MENU GLOWNE ===\n");
        printf("1. Nowa gra\n");
        printf("2. Wczytaj gre\n");
        printf("3. Instrukcja\n");
        printf("4. Wyjscie\n");
        printf("Wybierz opcje: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                // zwalnianie poprzedniej gry
                if (solution) {
                    free_board(solution, size);
                    solution = NULL;
                }
                if (puzzle) {
                    free_board(puzzle, size);
                    puzzle = NULL;
                }

                // wybieranie parametrow
                printf("\nWybierz rozmiar planszy (4, 9, 16): ");
                scanf("%d", &size);
                printf("Wybierz poziom trudnosci (1-3): ");
                scanf("%d", &difficulty);

                // generowanie gry
                solution = safe_generate_board(size);
                puzzle = generate_puzzle(solution, size, difficulty);

                if (solution && puzzle) {
                    play_game(puzzle, solution, size, difficulty);
                }
                break;
            }

            case 2:
                load_game(&puzzle, &solution, &size, &difficulty);
                if (puzzle) play_game(puzzle, solution, size, difficulty);
                break;

            case 3:
                printf("\n=== INSTRUKCJA ===\n");
                printf("1. Wprowadz wspolrzedne w formacie: wiersz kolumna wartosc\n");
                printf("2. Stale liczby sa w [nawiasach]\n");
                printf("3. Puste pola oznaczone sa kropkami\n");
                printf("4. Uzyj 'save' aby zapisac, 'exit' aby wyjsc\n");
                break;

            case 4:
                // zwalnianie pamięci
                if (solution) {
                    for (int i = 0; i < size; i++) free(solution[i]);
                    free(solution);
                }
                if (puzzle) {
                    for (int i = 0; i < size; i++) free(puzzle[i]);
                    free(puzzle);
                }
                return 0;

            default:
                printf("Nieprawidlowa opcja!\n");
        }
    }
}