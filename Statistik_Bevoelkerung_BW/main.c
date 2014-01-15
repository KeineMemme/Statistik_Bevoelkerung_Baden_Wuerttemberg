#include <stdio.h>
#include <stdlib.h>
#include "analyser.h"




int main()
{
    City People;
    init("Bevoelkerung 2010.csv", 2010);
    readData();
    //printAllData();
    //People = getMostWomenTown(6);
    //People = getMostMenTown(6);
    printf("Es gibt %u Regierungsbezirke in Baden-Wuerttemberg\n", count(2));
    printf("Es gibt %u Stadtkreise in Baden-Wuerttemberg\n", count(3));
    printf("Es gibt %u Landkreise in Baden-Wuerttemberg\n", count(4));
    printf("Es gibt %u Staedte in Baden-Wuerttemberg\n", getAnzahlTowns());
    printf("Es gibt %u Doerfer in Baden-Wuerttemberg\n", getAnzalVillages());
    printf("Es leben %u Menschen in Baden-Wuerttemberg\n", getAnzahlPeople());
    printf("Es leben %u Maenner in Baden-Wuerttemberg\n", getAnzahlMen());
    printf("Es leben %u Frauen in Baden-Wuerttemberg\n\n", getAnzahlWomen());
    People = smallestCity();
    People = specificSearch("Grafenau", 6);
    relize();
    return 0;
}
