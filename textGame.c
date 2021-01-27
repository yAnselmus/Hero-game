
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stddef.h"
#include <time.h>
#include <math.h>
#include <ctype.h>


typedef struct  {
    char name[20];
    int hp;
    int exp;
    char weapon[20];
    unsigned int weapondmg;
} Hero;

// Global values
unsigned int numberOfHeroes = 0;
int quit_flag = 1;
Hero *heroes;


void createHero(char *name, int hp, char *weapon, int weapondmg ) {

    numberOfHeroes++;
    heroes = realloc(heroes, numberOfHeroes*sizeof(Hero));

    unsigned int i = numberOfHeroes-1;

    strcpy(heroes[i].name, name);
    strcpy(heroes[i].weapon, weapon);
    heroes[i].hp = hp;
    heroes[i].weapondmg = weapondmg;
    heroes[i].exp = 0;
    
    printf("A hero bearing name %s was created with dmg %d and hp %d \n", name, weapondmg, hp);

}


void attack( char *attackerName, char *victimName) {
 
    unsigned int dmg;
    int i = 0;
    int j = 0;

    while(i<numberOfHeroes) {
        if (  strcmp(heroes[i].name, attackerName) == 0  )
            break;
        i++;
    }
    dmg = heroes[i].weapondmg;
    heroes[i].exp = heroes[i].exp + 5;


    while(j<numberOfHeroes) {
        if (  strcmp(heroes[j].name, victimName) == 0  )
            break;
        j++;
    }
    heroes[j].hp = heroes[j].hp - dmg;

    printf("%s attacked %s with %s by %d damage. %s has %d hitpoints remaining. %s gained 5 experience points.\n", attackerName, victimName,heroes[i].weapon, dmg, victimName, heroes[j].hp, attackerName);
}


void printHeroes(){
    
    int n = 0;
    int size = numberOfHeroes;

    while (n<size) {


        int biggest;
        if (heroes[n].hp > 0) {
            biggest = heroes[n].exp;
        }
        else biggest = -1;
        Hero placeholderValue = heroes[n];
        Hero *placeholder = &heroes[n];

        for (int i = n; i < size; i++)
        {
            if ( heroes[i].hp > 0) {
                if (heroes[i].exp > biggest) {
                    biggest = heroes[i].exp;
                    placeholder = &heroes[i];
                }
            }
        }
            heroes[n] = *placeholder;
            *placeholder = placeholderValue;

        n++;

    }

    int a = 0;
    while(a<numberOfHeroes) {
        printf("Hero %d: %s, Weapon: %s, Weapondamage: %d, HP: %d, EXP: %d\n", a+1, heroes[a].name, heroes[a].weapon, heroes[a].weapondmg, heroes[a].hp, heroes[a].exp);
        a++;
    }
}


void saveGame(char* filename) {

    FILE *f = fopen(filename, "w");
    int i = 0;

    fprintf(f, "%d ", numberOfHeroes);

    while(i < numberOfHeroes) {
        fprintf(f, "%s %d %d %s %d " , heroes[i].name , heroes[i].hp , heroes[i].exp , heroes[i].weapon , heroes[i].weapondmg );
        i++;
    }

    fclose(f);

    printf("Game saved in file %s\n", filename);
}


void loadGame(char* filename) {

    FILE *f = fopen(filename, "r");

    if(f!=NULL) {
        int i = 0;
        int newCount;

        fscanf(f, "%d ", &newCount);
        free(heroes);
        heroes = malloc(0);

        printf("Session was demolished. Loading from the file...\n");

        while(i < newCount) {
            char name[20];
            int hp;
            int exp;
            char weapon[20];
            unsigned int weapondmg;
            fscanf(f, "%s %d %d %s %d " , name , &hp , &exp , weapon , &weapondmg );
            createHero(name,hp,weapon,weapondmg);
            i++;
        }

        fclose(f);

        printf("Game succesfully loaded from file %s\n", filename);
    }
    else printf("The file doesn't exist.\n");
}

void quit() {
    quit_flag = 0;
    free(heroes);
}


int main() {
    
    while(quit_flag) {
    
    char buffer[80];
    fgets(buffer, sizeof(buffer), stdin);
    char first;
    char second[20];
    char third[20];
    char fourth[20];
    int fifth;
    int argNum = sscanf(buffer, "%c %s %s %s %d\n", &first, second, third, fourth, &fifth);

    switch(first) {

	    case 'A':
            if (argNum != 5) {
                printf("Incorrect arguments for this command! Please check parameters for HP and dmg.\n");
                break;
            }
            if (atoi(third) == 0) {
                printf("A hero can't be created with 0 HP.\n");
                break;
            }
		    createHero(second, atoi(third), fourth, fifth);
		    break;

	    case 'H':
            if (argNum != 3) {
                printf("Incorrect amount of arguments for this command!\n");
                break;
            }
            attack(second, third);
		    break;

	    case 'L':
            if (argNum > 1) {
                printf("Too many arguments for this command!\n");
                break;
            }
		    printHeroes();
		    break;

	    case 'W':
            if (argNum != 2) {
                printf("Incorrect amount of arguments for this command!\n");
                break;
            }
		    saveGame(second);
		    break;

        case 'O':
            if (argNum != 2) {
                printf("Incorrect amount of arguments for this command!\n");
                break;
            }
            loadGame(second);
            break;

        case 'Q':
            if (argNum != 1) {
                printf("Are you sure you want to quit? If so, please type Q\n");
                break;
            }
            quit();
            printf("Quitted.");
            break;

        default:
            printf("Please give a proper command.\n");
            break;
    }
    }
    
}