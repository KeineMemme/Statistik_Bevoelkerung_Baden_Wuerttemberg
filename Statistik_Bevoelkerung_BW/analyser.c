#include "analyser.h"

//Macht erstmal die Grundeinstellungen:
//fname = Datei, die analysiert wird, jahr = in welchem Jahr..
void init(char *fname, unsigned int j) {
    strncpy(dataname, fname, sizeof(dataname)-1);
    rows = countRows(fname);
    if(rows % 18 > 0) {
        printf("Wrong data format..\n");
        exit(1);
    }
    numCities = rows / 18;
    city = malloc(sizeof(City)*numCities);
    analyseJahr = j;
    printf("CheckIn:\nFilename:\t%s\nKontrolle:\t%lf\nIm Jahr:\t%i\n", dataname, (double)rows / 18, analyseJahr);
}

//Zählt die zu verarbeiteten Zeilen(unnötige Zeilen schon abgezogen):
unsigned int countRows(char *fname) {
    FILE *f;
    unsigned int rowsCount = 0;
    char puffer[500];
    f = fopen(fname, "r");
    if(f == NULL) {
        fprintf(stderr, "Cant opened file..\n");
        exit(1);
    }
    while(fgets(puffer, 500, f)) {
        rowsCount++;
    }
    rowsCount--;
    fclose(f);
    return rowsCount;
}

//Liest Inhalt der Datei in die Struktur ein:
bool readData(void) {
    FILE *f;
    char puffer[500];
    unsigned int wayne = 0, numCity = 0, supportCity = 0, numAgeGroup = 0;
    f = fopen(dataname, "r");
    if(f == NULL) {
        fprintf(stderr, "Cant opened file..\n");
        return false;
    }
    while(fgets(puffer, 500, f)) {
        if(wayne > 0) {
            char *checkAnfuehrungszeichen = strchr(puffer, '"');
            char *checkLand = strstr(puffer,"Land Baden-Württemberg");
            char *checkRegierungsbezirk = strstr(puffer,"Regierungsbezirk");
            char *checkStadtkreis = strstr(puffer, "Stadtkreis");
            char *checkLandkreis = strstr(puffer, "Landkreis");

            unsigned int helperZahl = 0;
            char *pToken = strtok(puffer, ","); //Erste unwichtig..
            pToken = strtok(NULL, ",");         //Zweite ebenfalls unwichtig..
            if(checkLand != NULL) {
                city[numCity].typ = 1;         //Typ: Land
            }
            else if(checkRegierungsbezirk != NULL) {
                city[numCity].typ = 2;         //Typ: Regierungsbezirk
            }
            else if(checkStadtkreis != NULL) {
                city[numCity].typ = 3;          //Stadkreis
            }
            else if(checkLandkreis != NULL) {
                city[numCity].typ = 4;          //Landkreis
            }
            else {
               city[numCity].typ = 5;           //Dorf/Stadt
            }
            //Normal:
            if(checkAnfuehrungszeichen == NULL) {
                pToken = strtok(NULL, ",");         //Nun enthält es den Stadtname
                strncpy(city[numCity].name, pToken, sizeof(city[numCity].name)-1);
                city[numCity].aGroups[numAgeGroup].id = numAgeGroup;   //ID für Altersgruppen
                pToken = strtok(NULL, ",");         //Nun enthält es den Altersgruppennamen
                strncpy(city[numCity].aGroups[numAgeGroup].name, pToken, sizeof(city[numCity].aGroups[numAgeGroup].name)-1);
                pToken = strtok(NULL, ",");         //Nun enthält es den gesamt Altersgruppenbereich
                sscanf(pToken, "%u", &helperZahl);  //Wandelt String in Zahl um..
                city[numCity].aGroups[numAgeGroup].aggregateAmount = helperZahl;
                pToken = strtok(NULL, ",");         //Anzahl männlich
                sscanf(pToken, "%u", &helperZahl);  //Wandelt String in Zahl um..
                city[numCity].aGroups[numAgeGroup].maleAmount = helperZahl;
                pToken = strtok(NULL, ",");         //Anzahl weiblich
                sscanf(pToken, "%u", &helperZahl);  //Wandelt String in Zahl um..
                city[numCity].aGroups[numAgeGroup].femaleAmount = helperZahl;
            }
            //Falls dumme Anfürhungszeichen drinne sind:
            else {
                pToken = strtok(NULL, ",");         //Nun enthält es den Stadtname
                strncpy(city[numCity].name, pToken+1, sizeof(city[numCity].name)-1);
                pToken = strtok(NULL, ",");         //unwichtig..
                city[numCity].aGroups[numAgeGroup].id = numAgeGroup;   //ID für Altersgruppen
                pToken = strtok(NULL, ",");         //Nun enthält es den Altersgruppennamen
                strncpy(city[numCity].aGroups[numAgeGroup].name, pToken, sizeof(city[numCity].aGroups[numAgeGroup].name)-1);
                pToken = strtok(NULL, ",");         //Nun enthält es den gesamt Altersgruppenbereich
                sscanf(pToken, "%u", &helperZahl);  //Wandelt String in Zahl um..
                city[numCity].aGroups[numAgeGroup].aggregateAmount = helperZahl;
                pToken = strtok(NULL, ",");         //Anzahl männlich
                sscanf(pToken, "%u", &helperZahl);  //Wandelt String in Zahl um..
                city[numCity].aGroups[numAgeGroup].maleAmount = helperZahl;
                pToken = strtok(NULL, ",");         //Anzahl weiblich
                sscanf(pToken, "%u", &helperZahl);  //Wandelt String in Zahl um..
                city[numCity].aGroups[numAgeGroup].femaleAmount = helperZahl;
            }
            //SupportCity achtet wann Datensatz Stadt wechselt
            supportCity++;
            numAgeGroup++;  //Z
            if(numAgeGroup == 18) {
                supportCity = 0;
                numAgeGroup = 0;
                numCity++;  //Array City Position
            }
        }
        else {
            wayne++;    //Erste Reihe ist unwichtig
        }
        memset(puffer, 0, sizeof(puffer));

    }
    countVTs();
    fclose(f);
    return true;
}
//Ausgabe aller Daten:
bool printAllData(void) {
    unsigned int i, d;
    for(i = 0; i < numCities; i++) {
        printf("+++++++++++++++\n");
        printf("(%u)%s\n", city[i].typ, city[i].name);
        for(d = 0; d < 18; d++) {
            printf("(%u)%s:\t%u\t", city[i].aGroups[d].id, city[i].aGroups[d].name, city[i].aGroups[d].aggregateAmount);
            printf("Maenner:\t%u\t", city[i].aGroups[d].maleAmount);
            printf("Frauen:\t%u\n", city[i].aGroups[d].femaleAmount);
        }
        printf("+++++++++++++++\n");
    }
    return true;
}
//Kleinste Gemeinde:
City smallestCity(void) {
    City SmallC;
    unsigned int i;
    for(i = 0; i < numCities; i++) {
        if(i == 0) {
            SmallC = city[i];
        }
        else if(i > 0) {
            if(SmallC.aGroups[17].aggregateAmount > city[i].aGroups[17].aggregateAmount) {
                SmallC = city[i];
            }
        }
    }
    printf("Die kleinste Gemeinde ist %s und hat %u Einwohner\n", SmallC.name, SmallC.aGroups[17].aggregateAmount);
    return SmallC;
}

//ProzentBevoelkerungBerechnung:
double procentPeopleBw(unsigned int anzahl, int gender, int ageGroup) { //1 = male, 2 = female, Rest = Alle
    if(gender == 1) {
        return ((double)anzahl / (double)city[0].aGroups[ageGroup].maleAmount) * 100.00;
    }
    else if(gender == 2) {
        return ((double)anzahl / (double)city[0].aGroups[ageGroup].femaleAmount) * 100.00;
    }
    else {
        return ((double)anzahl / (double)city[0].aGroups[ageGroup].aggregateAmount) * 100.00;
    }
}

//Einfluss einer Person in dieser Gemeinde
double getInfluenceOfOnePerson(unsigned int numPeople) {
    return 1.00 / (double)numPeople * 100.00;
}
//Zaehlt Typen(Bezirke, Kreise usw. usw.)
unsigned int count(int typ) {
    unsigned int i, count = 0;
    for(i = 0; i < numCities; i++) {
        if(city[i].typ == typ) {
            count++;
        }
    }
    return count;
}

//Checkt nach Stadt oder Dorf:
bool checkTown(City frage) {
    if(frage.aGroups[17].aggregateAmount >= 10000) {
        return true;
    }
    return false;
}
//Zähle Dörfer, Städte:
void countVTs(void) {
    unsigned int i = 0;
    anzahlTowns = 0;
    anzahlVillages = 0;
    for(i = 0; i < numCities; i++) {
        if(city[i].typ == 5) {
            if(checkTown(city[i])) {
                anzahlTowns++;
            }
            else {
                anzahlVillages++;
            }
        }
    }
}
//Spezifische Stadtanalyse:
City specificSearch(char *townName, unsigned int ageGroup) {
    City Sp;
    unsigned int i;
    //Stadt/Dorf auswählen:
    for(i = 0; i < numCities; i++) {
        if(strcmp(townName, city[i].name) == 0) {
            Sp = city[i];
        }
    }
    if(checkTown(Sp)) {
        printf("Stadt: %s(%u)\n", Sp.name, Sp.typ);
    }
    else {
        printf("Dorf: %s(%u)\n", Sp.name, Sp.typ);
    }
    printf("Gesamt Bevoelkerung: %u(%.3lf%%)\n", Sp.aGroups[17].aggregateAmount, procentPeopleBw(Sp.aGroups[17].aggregateAmount, 0, 17));
    printf("Gesamt Maenner: %u(%.3lf%%)\n", Sp.aGroups[17].maleAmount, procentPeopleBw(Sp.aGroups[17].maleAmount, 1, 17));
    printf("Gesamt Frauen: %u(%.3lf%%)\n", Sp.aGroups[17].femaleAmount, procentPeopleBw(Sp.aGroups[17].femaleAmount, 2, 17));
    printf("%s Maennlich: %u(%.3lf%%)[%.3lf%%]{%.3lf%%}\n", Sp.aGroups[ageGroup].name, Sp.aGroups[ageGroup].maleAmount, procentPeopleBw(Sp.aGroups[ageGroup].maleAmount, 1, ageGroup), procentPeopleBw(Sp.aGroups[ageGroup].maleAmount, 1, 17), procentPeopleBw(Sp.aGroups[ageGroup].maleAmount, 0, 17));
    printf("%s Weiblich: %u(%.3lf%%)[%.3lf%%]{%.3lf%%}\n", Sp.aGroups[ageGroup].name, Sp.aGroups[ageGroup].femaleAmount, procentPeopleBw(Sp.aGroups[ageGroup].femaleAmount, 2, ageGroup), procentPeopleBw(Sp.aGroups[ageGroup].femaleAmount, 2, 17), procentPeopleBw(Sp.aGroups[ageGroup].femaleAmount, 0, 17));
    printf("%s gesamt: %u(%.3lf%%){%.3lf%%}\n", Sp.aGroups[ageGroup].name, Sp.aGroups[ageGroup].aggregateAmount, procentPeopleBw(Sp.aGroups[ageGroup].aggregateAmount, 0, ageGroup), procentPeopleBw(Sp.aGroups[ageGroup].aggregateAmount, 0, 17));
    printf("Prozent einer Person: %lf%%\n", getInfluenceOfOnePerson(Sp.aGroups[17].aggregateAmount));
    printf("(*) bezogen auf das gesamte Land in dieser Kategorie..\n");
    printf("[*] bezogen auf das ganze Land des selben Geschlechts..\n");
    printf("{*} bezogen auf die gesamte Bevoelkerung..\n");
    Sp.aGroups[17].procentAll = procentPeopleBw(Sp.aGroups[17].aggregateAmount, 0, 17);
    Sp.aGroups[17].mProcentBw = procentPeopleBw(Sp.aGroups[17].maleAmount, 1, 17);
    Sp.aGroups[17].fProcentBw = procentPeopleBw(Sp.aGroups[17].femaleAmount, 2, 17);
    return Sp;
}

//Gibt die Stadt mit den meisten Frauen aus:
City getMostWomenTown(unsigned int ageGroup) {
    City Women;
    bool first = true;
    unsigned int i, d;
    for(i = 0; i < numCities; i++) {
        if(first) {
            if(city[i].typ == 5) {
                Women = city[i];
                first = false;
            }
        }
        else {
            for(d = 0; d < numCities; d++) {
                if(city[d].typ == 5 && d != i && city[d].aGroups[ageGroup].femaleAmount > Women.aGroups[ageGroup].femaleAmount) {
                    Women = city[d];
                }
            }
        }
    }
    printf("The Most Women live in %s with %u\n", Women.name, Women.aGroups[ageGroup].femaleAmount);
    return Women;
}

City getMostMenTown(unsigned int ageGroup) {
    City Men;
    bool first = true;
    unsigned int i, d;
    for(i = 0; i < numCities; i++) {
        if(first) {
            if(city[i].typ == 5) {
                Men = city[i];
                first = false;
            }
        }
        else {
            for(d = 0; d < numCities; d++) {
                if(city[d].typ == 5 && d != i && city[d].aGroups[ageGroup].maleAmount > Men.aGroups[ageGroup].maleAmount) {
                    Men = city[d];
                }
            }
        }
    }
    printf("The Most Men live in %s with %u\n", Men.name, Men.aGroups[ageGroup].maleAmount);
    return Men;
}

//Getter/Setter
unsigned int getAnzahlPeople(void) {
    return city[0].aGroups[17].aggregateAmount;
}
unsigned int getAnzahlMen(void) {
    return city[0].aGroups[17].maleAmount;
}
unsigned int getAnzahlWomen(void) {
    return city[0].aGroups[17].femaleAmount;
}
unsigned int getAnzahlTowns(void) {
    return anzahlTowns;
}
unsigned int getAnzalVillages(void) {
    return anzahlVillages;
}

//Relize
void relize() {
    free(city);
}
