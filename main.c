#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct Author {
    char Meno[50];
    char Priezvisko[50];
    char Rola[3];
} Author;

typedef struct Record {
    char ID[20];
    char Tema[100];
    Author *autori;
    int pocet_autorov;
    char Datum[20];
    struct Record *next;
} Record;

void uvol(Record **head) {
    Record *tmp;

    while (*head != NULL) {
        tmp = *head;
        *head = (*head)->next;
        free(tmp->autori);
        free(tmp);
    }
}

int n(const char *filename, Record **head) {
    FILE *file;
    char line[256];
    int n = 0;
    file = fopen(filename, "r");
    if (file == NULL) {
        printf("Zaznamy neboli nacitane!\n");
        return 0;
    }

    // Uvoľnenie existujúceho zoznamu
    uvol(head);


    while (fgets(line, sizeof(line), file)) {
        // Preskočenie oddelovača "$$$"
        if (strcmp(line, "$$$\n") == 0) {
            continue;
        }

        // Načítanie záznamu
        Record *new_record = (Record *)calloc(1,sizeof(Record));
        sscanf(line, "%s\n", new_record->ID);
        fgets(new_record->Tema, sizeof(new_record->Tema), file);
        new_record->Tema[strcspn(new_record->Tema, "\n")] = 0;

        // Načítanie autorov
        fgets(line, sizeof(line), file);
        new_record->pocet_autorov = 0;
        for (int i = 0; i < strlen(line); i++) {
            if (line[i] == '#') {
                new_record->pocet_autorov++;
            }
        }
        new_record->pocet_autorov /= 2;

        new_record->autori = (Author *)calloc(1, new_record->pocet_autorov * sizeof(Author));
        char *token = strtok(line, "#");//токен сохраняется в переменной token, которая объявлена как указатель на символ char *.
        for (int i = 0; i < new_record->pocet_autorov && token != NULL; i++) {
            sscanf(token, "%s %s", new_record->autori[i].Meno, new_record->autori[i].Priezvisko);
            token = strtok(NULL, "#");
            if (token != NULL) {
                strncpy(new_record->autori[i].Rola, token, 2);
                new_record->autori[i].Rola[2] = '\0';
                token = strtok(NULL, "#");
            }
        }
        fgets(new_record->Datum, sizeof(new_record->Datum), file);

        // Pridanie záznamu do zoznamu
        new_record->next = *head;
        *head = new_record;
        n++;
    }

    fclose(file);
    printf("Nacitalo sa %d zaznamov\n", n);
    return n;
}

void v(Record *head) {
    if (head == NULL) {
        printf("Prázdny zoznam záznamov.\n");
        return;
    }

    int record_count = 0;
    Record *current = head;

    while (current != NULL) {
        record_count++;
        printf("%d.\n", record_count);
        printf("ID prispevku: %s\n", current->ID);
        printf("Nazov prispevku: %s\n", current->Tema);
        printf("Mena autorov:\n");

        for (int i = 0; i < current->pocet_autorov; i++) {
            printf("%d: %s %s (%s)\n", i + 1, current->autori[i].Meno, current->autori[i].Priezvisko,
                   current->autori[i].Rola);
        }

        printf("Datum a Cas:  %.4s.%.2s.%.2s %.2s:%.2s\n", current->Datum, current->Datum + 4, current->Datum + 6,current->Datum + 8, current->Datum + 10);
        //Datum представлено как строка, для получения каждого элемента даты и времени используются смещения относительно начала строки:
        //Год: с 0-го по 3-й символы (смещение: 0, длина: 4 символа)
        //Месяц: с 4-го по 5-й символы (смещение: 4, длина: 2 символа)
        //День: с 6-го по 7-й символы (смещение: 6, длина: 2 символа)
        //Час: с 8-го по 9-й символы (смещение: 8, длина: 2 символа)
        //Минуты: с 10-го по 11-й символы (смещение: 10, длина: 2 символа)


        current = current->next;

    }
}

bool je_valid_id(const char* type) {
    return strcmp(type, "PD") == 0 || strcmp(type, "UD") == 0 || strcmp(type, "PP") == 0 || strcmp(type, "UP") == 0;
}



void update(Record * list,  const char* id, const char* new_type) {

    Record* current = list;
    //printf("1");

    while (current != NULL) {
        //printf("1");

        if (strcmp(current->ID + 2, id) == 0) {

            //printf("1");
            if (!je_valid_id(new_type)) {
                return;
            }

            printf("Prispevok s nazvom %s sa bude prezentovat %s [%.2s].\n", current->Tema, new_type, current->ID);

            strncpy(current->ID, new_type, 2);

        }

        current = current->next;
    }
}

void a (Record *head)
{
    Record *current = head;
    char  id [20];
    char new_type[3];

    scanf("%s %s", &id, new_type);
    getchar();
    //приведения строки в числовой вид
    //na prevod reťazca do číselného tvaru

    int nummer = atoi(id);

    while (nummer % 15 != 0 || !je_valid_id(new_type)) {
        printf("Zadane udaje nie su korektne, zadaj novy retazec: ");
        scanf("%d %s", &id, new_type);
        getchar();
    }

    update(current, id, new_type);
}

void p(Record **head) {
    char ID[20];
    char Tema[100];
    char autori_line[256];
    char Datum[20];
    int c1;

    printf("Zadajte cislo: ");
    scanf("%d", &c1);
    getchar();
    printf("\nZadajte id:");
    fgets(ID, sizeof(ID), stdin);
    //Reťazec načítaný zo vstupného prúdu sa potom spracuje pomocou funkcie strcspn(), ktorá vráti dĺžku počiatočného segmentu reťazca, ktorý neobsahuje zadanú znakovú sadu - v tomto prípade znak nového riadku "\n".
    //Týmto spôsobom sa reťazec, ktorý zadal používateľ, uloží do premennej ID a znak nového riadku, ktorý bol pridaný pri stlačení klávesu Enter, sa z reťazca odstráni, aby sa predišlo problémom pri neskoršom spracovaní zadaných údajov.
    ID[strcspn(ID, "\n")] = 0;
    printf("\nZadajte Nazov: ");
    fgets(Tema, sizeof(Tema), stdin);
    Tema[strcspn(Tema, "\n")] = 0;
    printf("\nZadajte (meno,prezvisko,rol): ");
    fgets(autori_line, sizeof(autori_line), stdin);
    autori_line[strcspn(autori_line, "\n")] = 0;
    printf("\nZadajte datum: ");
    fgets(Datum, sizeof(Datum), stdin);
    Datum[strcspn(Datum, "\n")] = 0;

    Record *new_record = (Record *)calloc(1, sizeof(Record));
    strcpy(new_record->ID, ID);
    strcpy(new_record->Tema, Tema);

    // Parse authors
    new_record->pocet_autorov = 0;
    for (int i = 0; i < strlen(autori_line); i++) {
        if (autori_line[i] == '#') {
            new_record->pocet_autorov++;
        }
    }
    new_record->pocet_autorov /= 2;

    new_record->autori = (Author *)calloc(1, new_record->pocet_autorov * sizeof(Author));
    char *token = strtok(autori_line, "#");
    for (int i = 0; i < new_record->pocet_autorov && token != NULL; i++) {
        sscanf(token, "%s %s", new_record->autori[i].Meno, new_record->autori[i].Priezvisko);
        token = strtok(NULL, "#");
        if (token != NULL) {
            strncpy(new_record->autori[i].Rola, token, 2);
            new_record->autori[i].Rola[2] = '\0';
            token = strtok(NULL, "#");
        }
    }

    strcpy(new_record->Datum, Datum);

    // Vloženie nového záznamu na zadanú pozíciu
    if (*head == NULL || c1 == 1) {
        new_record->next = *head;
        *head = new_record;
    } else {
        Record *current = *head;
        for (int i = 1; i < c1 - 1 && current->next != NULL; i++) {
            current = current->next;
        }
        new_record->next = current->next;
        current->next = new_record;
    }
}


// Funkcia z nefunguje ako v príklade, ak chcete odstrániť hodnoty, nastavte ju ako (Jozko Mrekvicka, nie ako v príklade Jozko MRKVICKA)

void z(Record **head) {
    char author_name[50];
    getchar(); //Čítanie a odstraňovanie znaku nového riadku
    fgets(author_name, sizeof(author_name), stdin);
    author_name[strcspn(author_name, "\n")] = 0;
    //Здесь strcspn(author_name, "\n") возвращает индекс символа новой строки в author_name, а затем программа заменяет его нулевым символом (терминатором строки), обозначенным 0. Это делает строку корректной строкой в стиле C, так как все строки в C должны заканчиваться нулевым символом.
    //Tu strcspn(meno_autora, "\n") vráti index znaku nového riadku v mene_autora a potom ho program nahradí nulovým znakom (terminátor reťazca) označeným 0. Tým sa reťazec stane platným reťazcom v štýle C, pretože všetky reťazce v jazyku C musia byť ukončené nulovým znakom.

    Record *current = *head;
    Record *prev = NULL;

    while (current != NULL) {
        bool found_author = false;

        for (int i = 0; i < current->pocet_autorov; i++) {
            char full_name[101];
            snprintf(full_name, sizeof(full_name), "%s %s", current->autori[i].Meno, current->autori[i].Priezvisko);
            if (strcmp(full_name, author_name) == 0 && strcmp(current->autori[i].Rola, "A") == 0) {
                found_author = true;
                break;
            }
        }

        if (found_author) {
            printf("Prispevok s nazvom %s bol vymazany.\n", current->Tema);

            if (prev == NULL) {
                *head = current->next;
            } else {
                prev->next = current->next;
            }

            Record *temp = current;
            current = current->next;
            free(temp->autori);
            free(temp);
        } else {
            prev = current;
            current = current->next;
        }
    }
}



// Funkcia prehodí dva záznamy v spájanom zozname na zadaných pozíciách c1 a c2.
void r(Record **head) {
    // Načítanie pozícií c1 a c2
    int c1, c2;
    scanf("%d %d", &c1, &c2);

    // Ak sú pozície neplatné alebo rovnaké, nemá zmysel ich prehodiť.
    if (c1 < 1 || c2 < 1 || c1 == c2) {
        return;
    }

    // Nájdenie záznamu a jeho predchodcu na pozícii c1
    Record *current1 = *head;
    Record *prev1 = NULL;
    for (int i = 1; i < c1 && current1 != NULL; i++) {
        prev1 = current1;
        current1 = current1->next;
    }

    // Ak sa nenájde záznam na pozícii c1, ukončíme funkciu
    if (current1 == NULL) {
        return;
    }

    // Nájdenie záznamu a jeho predchodcu na pozícii c2
    Record *current2 = *head;
    Record *prev2 = NULL;
    for (int i = 1; i < c2 && current2 != NULL; i++) {
        prev2 = current2;
        current2 = current2->next;
    }

    // Ak sa nenájde záznam na pozícii c2, ukončíme funkciu
    if (current2 == NULL) {
        return;
    }

    // Prepojenie predchodcov s opačnými záznamami
    if (prev1 != NULL) {
        prev1->next = current2;
    } else {
        // Ak je current1 prvý záznam v zozname, aktualizujeme hlavu zoznamu
        *head = current2;
    }

    if (prev2 != NULL) {
        prev2->next = current1;
    } else {
        // Ak je current2 prvý záznam v zozname, aktualizujeme hlavu zoznamu
        *head = current1;
    }

    // Výmena ukazateľov na nasledujúci záznam pre current1 a current2
    Record *temp = current1->next;
    current1->next = current2->next;
    current2->next = temp;
}


void k(Record **head) {
    // Pomocný ukazateľ na prechádzanie zoznamom a uvoľňovanie pamäti
    Record *current = *head;
    Record *next;

    // Prechádzanie spájaným zoznamom a uvoľňovanie pamäti pre jednotlivé záznamy
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    // Nastavenie hlavy zoznamu na NULL, aby sme vedeli, že zoznam bol uvoľnený
    *head = NULL;
}

int main() {
    char command;
    Record *head = NULL;

    while (1) {
        printf("\nZadajte prikaz:\n");
        printf("n - nacitanie zoznamu\n");
        printf("v - vypis zoznamu\n");
        printf("p - pridanie zoznamu\n");
        printf("z - zmazanie zaznamov\n");
        printf("a -  aktualizacia (zmena) typu prezentovania prispevku\n");
        printf( "r -  prehodenie dvoch zaznamov v spajanom zozname\n");
        printf("k -  ukoncenie programu\n");
        scanf(" %c", &command);

        if (command == 'n') {
            const char *filename = "KonferencnyZoznam2.txt";
            n(filename, &head);
        } else if (command == 'v') {
            v(head);
        } else if (command == 'k') {
            k(&head);
            break;
        }
        else if (command == 'p')
        {
            p(&head);
        }
        else if( command=='z'){
            // Funkcia z nefunguje ako v príklade, ak chcete odstrániť hodnoty, nastavte ju ako (Jozko Mrekvicka, nie ako v príklade Jozko MRKVICKA)
            z(&head);
        }
        else if (command == 'a')
        {
            a(head);
        }
        else if (command == 'r')
        {
            r(&head);
        }
        else {
            printf("Neznamy prikaz\n");
        }
    }

    return 0;
}

