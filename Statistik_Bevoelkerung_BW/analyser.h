#ifndef ANALYSER_H
#define ANALYSER_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

//Struktur für die einzelnen Altersgruppen
struct ageGroups {
    unsigned int id;                //Ordnet der Gruppe eine bestimmte ID zu, für bessere Ordnung
    char name[255];                 //Name der Altersgruppe
    unsigned int aggregateAmount;   //Anzahl der Leute in der Altersgruppe
    double procentAll;              //Prozentual Stadt auf Land
    unsigned int maleAmount;        //Anzahl Männer in der Altergruppe
    unsigned int femaleAmount;      //Anzahl Frauen in der Altergruppe
    unsigned int mProcentCity;      //Prozentuale Anzahl der Männer auf die Stadt/Dorf
    unsigned int mProcentBw;        //Prozentuale Anzahl der Männer auf ganz Baden-Württemberg
    unsigned int fProcentCity;      //Prozentuale Anzahl der Frauen auf die Stadt/Dorf
    unsigned int fProcentBw;        //Prozentuale Anzahl der Frauen auf ganz Baden-Württemberg

};

typedef struct ageGroups ageGroups;
//Struktur für die jeweilligen Städte
struct city {
    unsigned int typ;                //Ordnet Stadttyp zu(1land, 2Regierungsbezirk, 3stadtkreis, 4landkreis, 5gemeinde, 6stadt, 7dorf)
    char name[255];                 //Name der Stadt
    ageGroups aGroups[18];          //Da die Daten in 17 Teile unterteilt wurden
};

typedef struct city City;

//Variablen Dekleration:
static char dataname[255];  //Filename der zu analysierenden Datei
static City *city;          //Enthält später alle Städte in einem Array
static unsigned int analyseJahr;
static unsigned int rows;   //Die Reihen, die auch zählen..
static unsigned int numCities = 0;
static unsigned int anzahlTowns = 0;
static unsigned int anzahlVillages = 0;


//Macht erstmal die Grundeinstellungen:
//fname = Datei, die analysiert wird, jahr = in welchem Jahr..
void init(char *fname, unsigned int jahr);
//Zählt die zu verarbeiteten Zeilen(unnötige Zeilen schon abgezogen):
unsigned int countRows(char *fname);
//Liest Inhalt der Datei in die Struktur ein:
bool readData(void);
//Ausgabe aller Daten:
bool printAllData(void);
//Kleinste Gemeinde:
City smallestCity(void);
//ProzentBevoelkerungBerechnung:
double procentPeopleBw(unsigned int anzahl, int gender, int ageGroup);


//Einfluss einer Person in dieser Gemeinde
double getInfluenceOfOnePerson(unsigned int numPeople);
//Zaehlt Typen(Bezirke, Kreise usw. usw.)
unsigned int count(int typ);
//Checkt nach Stadt oder Dorf:
bool checkTown(City frage);
//Zähle Dörfer, Städte:
void countVTs(void);
//Spezifische Stadtanalyse:
City specificSearch(char *townName, unsigned int ageGroup);
//Gibt die Stadt mit den meisten Frauen aus:
City getMostWomenTown(unsigned int ageGroup);
//Gibt die Stadt mit den meisten Männern aus:
City getMostMenTown(unsigned int ageGroup);


//Getter/Setter
unsigned int getAnzahlPeople(void);
unsigned int getAnzahlMen(void);
unsigned int getAnzahlWomen(void);
unsigned int getAnzahlTowns(void);
unsigned int getAnzalVillages(void);


//Macht Speicherplatz wieder frei
void relize(void);


#endif
