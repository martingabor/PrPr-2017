//
//  main.c
//  First project
//
//  Created by Martin Gábor on 27.10.17.
//  Copyright © 2017 Martin Gábor. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define terminalValue "terVal\0" //pomocná konštanta označujúca koniec poľa s SPZ
#define SPZlength 7  //konštanta označujúca bežnú dĺžku SPZ na slovensku


typedef enum bool { false, true} bool;

void v(FILE **fr){
    char name[50], SPZ[SPZlength + 1];
    int date;
    bool type;
    double price;
    
    if ((*fr = fopen("/Users/MocinGejbr/Documents/predaj.txt", "r")) == NULL){ //otvorenie súboru
        printf("Neotvoreny subor\n");
        return ;
    }
    
    while (!feof(*fr)){ //kým nie je koniec súboru načítava zázanamy a vypisuje ich na obrazovku
        
        fgets(name,50,*fr);
        fscanf(*fr ,"%s\n", SPZ);
        fscanf(*fr ,"%d\n", &type);
        fscanf(*fr ,"%lf\n", &price);
        fscanf(*fr ,"%d\n", &date);
        
        printf("meno priezvisko: %s", name);
        printf("SPZ: %s\n", SPZ);
        printf("typ: %d\n", type);
        printf("cena: %.2lf\n", price);
        printf("datum: %d\n\n", date);


    }
    rewind(*fr); //vráti ukazovateľ na aktuálnu polohu v súbore na začiatok
}

void o(FILE **fr){
    if (*fr == NULL) return; //ak pole *fr nie je alokované funkcia končí
    
    char name[50], SPZ[SPZlength + 1]; //dĺžka bežnej ŠPZ + 1 miesto na znak \0
    bool type;
    double price;
    int currentDate, fileDate;
    
    scanf("%d", &currentDate); //dátum od ktorého sa bude počítať jeden rok(podľa zadania)
    
    while (!feof(*fr)){  //načítava zo súboru ako vo funkcií v s tým, že až keď prečíta dátum pracuje s údajmi
        
        fgets(name ,50, *fr);
        name[strlen(name) - 1] = '\0';
        fscanf(*fr ,"%s\n", SPZ);
        fscanf(*fr ,"%d\n", &type);
        fscanf(*fr ,"%lf\n", &price);
        fscanf(*fr ,"%d\n", &fileDate); // načíta dátum
        
        if ((currentDate - fileDate) >= 10000){  //podľa formátu dátumu vieme určiť, že ak odčítame dátum zo súboru od
            if (type) printf("%s %s %.2lf\n",name,SPZ,0.015 * price); //zadaného dátumu a výsledok bude väčší ako 10000
            else printf("%s %s %.2lf\n",name,SPZ,0.022 * price); // zamestnanec tam ja aspoň jeden rok
        }
    }
    
    rewind(*fr); //vráti ukazovateľ na aktuálnu polohu v súbore na začiatok
}

void n(FILE **fr, char ***SPZarray){
    if (*fr == NULL) return; //ak pole *fr nie je alokované funkcia končí
    
    char temp, tempStr[50];
    int lineCount = 0, inputCount = 0, j = 0;
    
    while ((temp = fgetc(*fr)) != EOF){  //cyklus ktorý počíta počet odriadkovaní v súbore
        if (temp == '\n'){
            lineCount++;
            if (lineCount == 6){ //vieme, že 6 riadkov = 1 záznam
                inputCount++;
                lineCount = 0;
            }
        }
    }
    if (lineCount != 0){ //ošetrenie kvôli koncu súboru (či je tam jeden alebo dva prázdne riadky)
        inputCount++;
        lineCount = 0;
    }

    rewind(*fr); //vráti ukazovateľ na aktuálnu polohu v súbore na začiatok
    
    if(*SPZarray == NULL){                                      //ak pole ešte nebolo alokované alokuje ho
        *SPZarray = malloc((inputCount + 1) * sizeof(char*));
    }else{                                                      //ak pole alokované bolo dealokuje ho a následne alokuje nové
        while(strcmp(*(*SPZarray + j), terminalValue) != 0){
            free(*(*SPZarray + j));
            j++;
        }
        free(*SPZarray);
        *SPZarray = (char**) malloc((inputCount + 1) * sizeof(char*));
    }

    
    for (int i = 0; i < inputCount; i++){ //naplní pole SPZtiek SPZtkami zo súboru v poradí v akom sú v súbore
        *(*SPZarray + i)  = (char*) malloc(8 * sizeof(char));
        
        fgets(tempStr, 50, *fr);
        fscanf(*fr , "%s\n", *(*SPZarray + i));
        fgets(tempStr, 50, *fr);
        fgets(tempStr, 50, *fr);
        fgets(tempStr, 50, *fr);
        fgets(tempStr, 50, *fr);
    }
    
    *(*SPZarray + inputCount)  = (char*) malloc(8 * sizeof(char)); //terminalValue je konštanta ktorou si označujem koniec
    *(*SPZarray + inputCount) = terminalValue;                     //poľa SPZ - keďže ide o dynamické pole
 
    rewind(*fr); //vráti ukazovateľ na aktuálnu polohu v súbore na začiatok
}

void s(char **SPZ){
    if(SPZ == NULL){  //ak ešte nebolo vytvorené pole  (nebol zavolaný príkaz n) končí a vypíše informáciu o nevytvorenom poli
        printf("Pole nie je vytvorene\n");
        return;
    }
    
    int i = 0;
    while (SPZ[i] != terminalValue){  //prechádza pole a vypisuje všetky SPZ vo formáte XX YYY ZZ  v poradí v akom sú v poli
        for (int j = 0; j < SPZlength; j++){
            putchar(*(SPZ[i]+j));
            if(j == 1 || j == 4) putchar(' '); //medzery po druhom a piatom znaku kvôli formátu spomenutému vyššie
        }
        putchar('\n');
        i++;
    }
}

void p(char **SPZ){
    if(SPZ == NULL){ //ak ešte nebolo vytvorené pole  (nebol zavolaný príkaz n) končí a vypíše informáciu o nevytvorenom poli
        printf("Pole nie je vytvorene\n");
        return;
    }
    
    int i = 0;
    bool palindrom = true;
    
    while (SPZ[i] != terminalValue){ //prechádza pole SPZ
        for (int j = 0; j < SPZlength; j++){ //prechádza jednotlivé prvky poľa SPZ po znakoch
            if (SPZ[i][j] != SPZ[i][SPZlength - 1 - j]){ //porovnáva prvý s posledným, predposledný s druhý atď.
                palindrom = false; //ak sa aspoň jeden znak nezhoduje už to nie je palnidrom = označí premennú palindrom=false
                break;
            }
        }
        
        if (palindrom) //na záver kontroly každého slova (vo vnútornom cykle) vypíše prvé dva znaky XX označujúce mesto SPZ
            printf("%c%c\n", SPZ[i][0], SPZ[i][1]);          //ktorá je palindrom
        
        palindrom = true; //nastaví sa hodnota palindrom na default (true)
        i++;
    }
}

void z(char **SPZ){
    
    int numberOfSPZ = 0, currentNumberOfSPZ = 1, maxNumberOfSPZ = 0, k = 0;
    
    char **mostCommonSPZ = NULL;
    
    if(SPZ == NULL){ //ak pole nie je zaplnené/vytvorené končí
        return;
    }
    while (SPZ[numberOfSPZ] != terminalValue){ //spočíta počet záznamov v poli
        numberOfSPZ++;
    }
    numberOfSPZ++; //+ 1 záznam kvôli ukončovacej hodnote poľa (terminalValue)
    
    for (int i = 0; i < numberOfSPZ; i++){
        for (int j = i + 1; j < numberOfSPZ; j++){ //od prvého prvku poľa počíta koľko krát sa prvé dva znaky SPZ opakujú na
            if(!strncmp(SPZ[i], SPZ[j],2))         //ďalších miestach poľa a ukladá si počet do premennej currentNummberOfSPZ
                currentNumberOfSPZ++;
        }
        if (currentNumberOfSPZ > maxNumberOfSPZ){   //ak je aktuálny počet opakovaní väčší ako maximálny dealokuje poľe
            if (mostCommonSPZ != NULL) {            //mostCommonSPZ(ak už bolo vytvorené) v ktorom sú uložené najčastejšie SPZ
                k = 0;                              //a nahradí ho prvkom ktorý je momentálne najčastejší
                while (!strncmp(*(mostCommonSPZ + k), terminalValue, 3)) {
                    free(*(mostCommonSPZ + k));
                    k++;
                }
                free(*(mostCommonSPZ + k));
                free(mostCommonSPZ);
                mostCommonSPZ = NULL;
                k = 1;
                
            }
            
            mostCommonSPZ = (char** ) malloc(2 * sizeof(char*));
            *mostCommonSPZ = (char*) malloc(3 * sizeof(char));
            strncpy(*mostCommonSPZ, SPZ[i], 2);
            strcat(*mostCommonSPZ, "\0");
            *(mostCommonSPZ + 1) = (char*) malloc(3 * sizeof(char));
            strncpy(*(mostCommonSPZ + 1), terminalValue, 3);
            
            maxNumberOfSPZ = currentNumberOfSPZ;
        } else if (currentNumberOfSPZ == maxNumberOfSPZ) {  //ak je aktuálny počet opakovaní rovný ako maximálny rozšíri pole
            k++;                                            //mostCommonSPZ a na ďalšie voľné miesto pridá ďalší prvok s
                                                            //s rovnakým počtom opakovaní
            mostCommonSPZ = (char** ) realloc(mostCommonSPZ, (k+2) * sizeof(char*));
            *(mostCommonSPZ + k) = (char*) malloc(3 * sizeof(char));
            strncpy(*(mostCommonSPZ + k), SPZ[i], 2);
            strcat(*(mostCommonSPZ + k), "\0");
            *(mostCommonSPZ + k + 1) = (char*) malloc(3 * sizeof(char));
            strncpy(*(mostCommonSPZ + k + 1), terminalValue, 3);
        }
        currentNumberOfSPZ = 1;
    }
    k = 0;
    while (strncmp(*(mostCommonSPZ + k), terminalValue, 3)){ //výpis najčastejších prvkov
        printf("%c%c %d\n", mostCommonSPZ[k][0], mostCommonSPZ[k][1], maxNumberOfSPZ);
        k++;
    }

}

void a(char ***SPZ){
    int k, numberOfSPZ = 0, l = 0;
    
    char **pomArr = NULL;
    
    scanf("%d",&k);
    
    if(*SPZ == NULL)
        return;
    
    while (*(*SPZ + numberOfSPZ) != terminalValue){ //spočíta počet záznamov v poli
        numberOfSPZ++;
    }
    
    pomArr = (char**) malloc ((2 * numberOfSPZ + 1) * sizeof(char*));
    
    for (int i = 0; i < numberOfSPZ; i++){
        for (int j = 0; j < 2; j++){
            if (j == 0){
                *(pomArr + l) = (char*) malloc (8 * sizeof(char));
                strcpy(*(pomArr + l), *(*SPZ + i));
            } else {
                *(pomArr + l) = (char*) malloc (3 * sizeof(char));
                strncpy(*(pomArr + l), *(*SPZ + i), 2);
                if(pomArr[l][0] + k <= 90) //65 - 90
                    pomArr[l][0] += k;
                else pomArr[l][0] = pomArr[l][0] + k - 26;
                if(pomArr[l][1] + k <= 90) //65 - 90
                    pomArr[l][1] += k;
                else pomArr[l][1] = pomArr[l][1] + k - 26;

                strcat(*(pomArr + l),"\0");
            }
            l++;
        }
    }
    *(pomArr + l) = (char*) malloc (8 * sizeof(char));
    *(pomArr + l) = terminalValue;
    
    *SPZ = (char**) realloc (*SPZ, (2 * numberOfSPZ + 1) * sizeof(char*));
    *SPZ = pomArr;

}

void b(char **SPZ){
    int numberOfSPZ = 0, pomArr[10];
    
    for(int i = 0; i< 10; i++){
        pomArr[i] = 0;
    }

    while (SPZ[numberOfSPZ] != terminalValue){ //spočíta počet záznamov v poli
        for (int i = 0; i < strlen(SPZ[numberOfSPZ]); i++){
            for(int j = 30; j <= 39; j++){
                if(SPZ[numberOfSPZ][i] == j)
                    pomArr[j - 30]++;
            }
        }
        numberOfSPZ++;
    }
    for(int i = 0; i <= 9; i++){
        printf("%d:%d\n",i, pomArr[i]);
    }
    
}

int main() {
    
    char control, **SPZ = NULL;
    FILE *fr = NULL;
    int i = 0;
    
    
    while ((control = getchar()) != 'k'){
        switch (control){
            case 'v': v(&fr); break;
            case 'o': o(&fr); break;
            case 'n': n(&fr, &SPZ); break;
            case 's': s(SPZ); break;
            case 'p': p(SPZ); break;
            case 'z': z(SPZ); break;
            case 'a': a(&SPZ); break;
            case 'b': b(SPZ); break;
        }
    }
    if (fr != NULL && fclose(fr) == EOF)  //ak bol počas behu programu otvorený súbor zatvorí sa
        printf("Subor sa nepodarilo zatvorit.");
    
    if (SPZ != NULL){ //ak bolo počas behu programu alokované pole dealokuje sa
        while (*(SPZ + i) != terminalValue){
            free(*(SPZ + i));
            i++;
        }
        free(SPZ);
    }
        
    
    return 0;
}
