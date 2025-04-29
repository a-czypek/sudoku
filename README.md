# Sudoku
Projekt jest implementacją popularnej gry logicznej Sudoku w języku C. Program umożliwia rozgrywkę na różnych poziomach trudności i rozmiarach planszy.

## Kluczowe funkcjonalności:

### 1. Generowanie plansz:
- **Automatyczne tworzenie pełnych plansz** (rozmiary 4x4, 9x9, 16x16) za pomocą wykorzystanego algorytmu backtracking
- **Generowanie łamigłówek** poprzez usuwanie komórek na trzech poziomach trudności (łatwy - 40%; średni - 55%; trudny - 70%)
- **Unikalność rozwiązania** poprzez wykorzystanie funkcji *count_solutions*, która określa ile różnych rozwiązań istnieje dla danej planszy

### 2. Mechanika gry:
- Walidacja ruchów w czasie rzeczywistym (sprawdzanie czy nie istnieje taka sama wartość w kolumnach, wierszach i podkwadratach)
- System zapisu i wczytywania gry, zachowujący stan planszy, rozwiązanie i parametry gry
- Zegar rejstrujący czas rozwiązania łamigłówki (liczony w sekundach)

### 3. Interfejs użytkownika:
- **Menu główne** z opcjami nowej gry, wczytania zapisu, instrukcji i wyjścia
- **Polecenia sterujące**, które pozwalają na zapis (*save*), powrót do menu (*exit*) oraz możliwość usuwania liczb (*wpisanie wartości 0*)
- **Graficzna prezentacja** planszy Sudoku, uwzględniająca podział na podkwadraty oraz oznaczenie wstępnie wypełnionych komórek (np. *[2]*)
  
---
## Organizacja kodu
| Warstwa | Funkcje | Przeznaczenie |
|---------|---------|---------------|
| Alokacja pamięci | *duplicate_board*, *free_board* | Tworzenie i zwalnianie dynamicznych tablic|
| Walidacja i backtracking | *is_valid*, *solve_recursive*, *count_solutions* | Sprawdzanie poprawności i znajdowanie rozwiązań |
| Generowanie planszy | *generate_shuffled_numbers*, *fill_remaining*, *generate_filled_board* | Losowe wypełnienie planszy |
| Tworzenie łamigłówki | *generate_puzzle* | Usuwanie komórek z jednym rozwiązaniem
| Interfejs użytkownika | *print_board*, *play_game*, *main* | Menu, wczytywanie poleceń, pętla gry, zapis/wyjście |

## Algorytmy
#### Fisher-Yates: 
Algorytm, który pozwala na tasowanie skończonej sekwencji. Generuje nieobciążoną permutację: każda permutacja jest równie prawdopodobna, co gwarantuje równomierny rozkład permutacji

#### **Backtracking**: 
Wykorzystany do wypełnienia pustych pól generowanejk planszy oraz do liczenia i znajdowania rozwiązań. Rekurencyjnie próbuje każdą dopuszczalną liczbę i cofa się przy konflikcie.

## Działanie Programu
### Nowa gra
https://github.com/user-attachments/assets/dbf344cd-0e1f-4840-a29b-b0d956a167d3

1. Użytkownik wybiera opcję "**Nowa gra**" z menu głównego.
2. Następnie określa:
    - Rozmiar planszy (4x4, 9x9, 16x16)
    - Poziom trudności (łatwy, średni, trudny)
3. Po wygenerowaniu planszy użytkownik rozwiązuje łamigłówkę, wpisując dane w formacie:
   - `wiersz kolumna wartość` - aby dodać liczbę do konkretnej komórki
   - `wiersz kolumna 0` - aby usunąć wcześniej wpisaną wartość
   - `save` - aby zapisać stan gry
   - `exit` - aby wyjść z gry
4. Po poprawnym wypełnieniu całej planszy:
    - Wyświetlany jest komunikat gratulacyjny
    - Pokazywany jest czas rozwiązania łamigłówki
   
---

### Zapis i wczytanie

https://github.com/user-attachments/assets/73b67ed0-057e-4139-8028-e3a8f463d781

1. Podczas rozwiązywania łamigłówki uzytkownik może wprowadzić komendę `save`, aby zachować aktualny stan gry.
2. Wprowadzenie komendy `exit` umożliwia wyjście z gry bez utraty postępów
3. Po ponownym uruchomieniu programu komenda **Wczytaj grę** pozwala załadować zapisany stan gry i kontynuować rozgrywkę od momentu zapisu

---

### Instrukcja

![Zrzut ekranu 2025-04-29 202734](https://github.com/user-attachments/assets/75812e4c-b062-4824-9088-8a029894acc7)


1. Użytkownik wybiera komendę **Instrukcja** w terminalu
2. Program wyświetla czytelny opis zasad gry, format wprowadzania danych oraz dostępne komendy

## Ulubiony meme

![Zrzut ekranu 2025-04-29 180641](https://github.com/user-attachments/assets/bb864879-dc76-4743-be3d-db039437fa59)
