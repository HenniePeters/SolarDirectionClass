#include <iostream>

using namespace std;

#include <math.h>

// Functie: bereken daglengte in minuten op dag 'dag' (1 t/m 365 of 366)
// Werkt goed voor gemiddelde locatie in Nederland (bijv. 52,5° N)

const float PI = 3.14159265;

float berekenDaglengte(int dag) {
    const float PI = 3.14159265;
    float A = 2.0 * PI * (dag - 81) / 365.2425;

    // Hoofdterm + correctie voor asymmetrie
    float daglengte = 720.0
                    + 258.0 * sin(A)        // hoofdamplitude
                    + 18.0 * sin(2*A)       // correctie voor vorm
                    + 5.0  * sin(3*A);      // kleine verbetering

    return daglengte;
}

// Tijdsvereffening (in minuten)
float tijdsvereffening(int dag) {
    float B = 2.0 * PI * (dag - 81) / 365.0;
    return 9.87 * sin(2*B) - 7.53 * cos(B) - 1.5 * sin(B);
}

void berekenZonTijden(int dag, float lengtegraad) {
    float E = tijdsvereffening(dag);
    float daglengte = berekenDaglengte(dag);
    float halve_dag = daglengte / 2.0;

    // Ware middag in zonnetijd (minuten van de dag)
    float middag = 720.0 + E;

    int opgang = (int)(middag - halve_dag + 0.5);
    int ondergang = (int)(middag + halve_dag + 0.5);

    opgang = (opgang + 1440) % 1440;
    ondergang = (ondergang + 1440) % 1440;

    printf("Zonsopgang %02d:%02d ondergang %02d:%02d\n",
           opgang / 60, opgang % 60,
           ondergang / 60, ondergang % 60);
}

int main() {
    int jaardag = 182;  // 1 juli
    berekenZonTijden( 182, 6.07 );
}
