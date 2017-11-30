//
//  main.c
//  Second project
//
//  Created by Martin Gábor on 22.11.17.
//  Copyright © 2017 Martin Gábor. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct cars {
    int num;
    char category[50];
    char brand[50];
    char seller[100];
    int price;
    int yearOfProd;
    char status[200];
    struct cars *next;
} CARS;

void getstring(char input[], int n, FILE *infile){  //moja pomocna funkcia na nacitanie vhodneho formatu vstupu
    if (fgets(input,n,infile) == NULL){
        exit(1);
    }
    input[strlen(input) - 1] = '\0';
}

void create(CARS **head, FILE **fr){
    int i = 0;
    
    CARS *current = *head, *tmp = NULL;
    
    if (*head != NULL){
        current = current->next;
        while(current != NULL){
            tmp = current->next;
            free(current);
            current = tmp;
        }
    }

    current = NULL;
    *head = current;
    if ((*fr = fopen("/Users/MocinGejbr/Documents/auta.txt", "r")) == NULL){
        printf("Zaznamy neboli nacitane\n");
    }
    
    while(fgetc(*fr) == '$'){
        if(i == 0){
            current = (CARS*) malloc(sizeof(CARS));
            i++;
            fscanf(*fr,"\n");
            current->num = i;
            getstring(current->category, 50, *fr);
            getstring(current->brand, 50, *fr);
            getstring(current->seller, 100, *fr);
            fscanf(*fr, "%d\n", &current->price);
            fscanf(*fr, "%d\n", &current->yearOfProd);
            getstring(current->status, 200, *fr);
            
            *head = current;
        } else {
            i++;
            current->next = (CARS*) malloc(sizeof(CARS));
            current = current->next;
            current->next = NULL;
            
            fscanf(*fr,"\n");
            current->num = i;
            getstring(current->category, 50, *fr);
            getstring(current->brand, 50, *fr);
            getstring(current->seller, 100, *fr);
            fscanf(*fr, "%d\n", &current->price);
            fscanf(*fr, "%d\n", &current->yearOfProd);
            getstring(current->status, 200, *fr);

        }
    }
    
    printf("Nacitalo sa %d zaznamov\n",i);
}

void prnt(CARS* head){
    CARS *current = head;
    while(current != NULL){
        printf("%d.\nkategoria: %s\nznacka: %s\npredajca: %s\ncena: %d\nrok_vyroby: %d\nstav_vozidla: %s\n",current->num,current->category, current->brand, current->seller,current->price, current->yearOfProd, current->status);
        current = current->next;
    }
}

void add(CARS **head){
    CARS *current = *head, *new = (CARS*)malloc(sizeof(CARS));
    int n;
    
    scanf("%d\n",&n);
    
    while(current->next != NULL){
        current = current->next;
    }
    
    if(current->num < n) {
        n = current->num + 1;
    }
    
    current = *head;
    
    new->num = n;
    getstring(new->category, 50, stdin);
    getstring(new->brand, 50, stdin);
    getstring(new->seller, 100, stdin);
    
    scanf("%d\n", &new->price);
    scanf("%d\n", &new->yearOfProd);
    
    getstring(new->status, 200, stdin);
    new->next = NULL;
    
    if(n == 1){
        new->next = *head;
        *head = new;
    }
    
    while(current != NULL){
       if(current->num == n - 1){
           new->next = current->next;
           current->next = new;
           current = current->next;
       } else if (current->num >= n){
            current->num++;
        }
        current = current->next;
    }
}

void delete(CARS **head){
    CARS *current = *head, *temp = NULL;
    int deleted = 0, i = 0;
    
    char delBrand[50];
    
    scanf("%s",delBrand); //ktora znacka bude vymazana
    
    
    
    while(current != NULL){//prechadzam spajany zoznam a hladam ci je zadana znacka ako substring dakej znacky zo zoznamu
        if(strcasestr(current->brand, delBrand) != NULL){
            if(temp == NULL){
                temp = current;
                current = current->next;
                free(temp);
                temp = NULL;
                deleted++;
                *head = current;;
            } else {
                temp->next = current->next;
                temp = current;
                free(temp);
                deleted++;
                current = current->next;
            }
        }
        else {
            temp = current;
            current = current->next;
        }
    }
    
    current = *head;
    
    while(current != NULL){
        i++;
        current->num = i;
        current = current->next;
    }
    
    printf("Vymazalo sa %d zaznamov\n",deleted);
}

void search(CARS *head){
    int c, i = 0;
    CARS *current = head;
    
    scanf("%d",&c);
    
    while (current != NULL){
        if(current->price <= c){
            i++;
            printf("%d.\nkategoria: %s\nznacka: %s\npredajca: %s\ncena: %d\nrok_vyroby: %d\nstav_vozidla: %s\n",i,current->category, current->brand, current->seller,current->price, current->yearOfProd, current->status);
        }
        current = current->next;
    }
    
    if(i == 0){
        printf("V ponuke su len auta s vyssou cenou\n");
    }
    
}

void change(CARS **head){
    CARS *current = *head;
    char srchbrand[50], category[50], brand[50], seller[100], status[50];
    int srchprice, price, yearOfProd, i = 0;
    
    scanf("%s\n",srchbrand);
    scanf("%d\n",&srchprice);
    
    getstring(category, 50, stdin);
    getstring(brand, 50, stdin);
    getstring(seller, 100, stdin);
    scanf("%d\n", &price);
    scanf("%d\n", &yearOfProd);
    getstring(status, 200, stdin);
    
    while(current != NULL){
        if(strcasestr(current->brand,srchbrand) != NULL && srchprice == current->price){
            strcpy(current->category, category);
            strcpy(current->brand, brand);
            strcpy(current->seller, seller);
            current->price = price;
            current->yearOfProd = yearOfProd;
            strcpy(current->status, status);
            i++;
        }
        current = current->next;
    }
    
    printf("Aktualizovalo sa %d zaznamov\n", i);
}

int main() {
    char control;
    CARS *head = NULL, *current = NULL, *tmp = NULL;
    FILE *fr = NULL;
    
    
    while ((control = getchar()) != 'k'){
        switch (control){
            case 'n': create(&head, &fr); break;
            case 'v': prnt(head); break;
            case 'p': add(&head); break;
            case 'z': delete(&head); break;
            case 'h': search(head); break;
            case 'a': change(&head); break;
        }
    }
    
    current = head;
    if (head != NULL){
        current = current->next;
        while(current != NULL){
            tmp = current->next;
            free(current);
            current = tmp;
        }
    }
    
    return 0;
}
