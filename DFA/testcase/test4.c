/*
 * Programma per la semplificazione di polinomi di grado superiore al primo con metodo Ruffini
 * Versione = 0.1
 * Autore = Saverio Custodi
 * Linguaggio usato = C
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

//includere funzione per trovare potenziali 0 dei polinomi non interi in un array e funzione per stampa fancy delle espressione (NO +1x-3)

void clearscreen()
{
    system("@cls||clear");
}

int ruffiniCon2Coefficienti(float coefficiente1, float coefficiente2, int termineNoto)
{
    int i, a;
    int divisoreTermineNoto, divisoreTermineNotoOpposto, risultatoRuffini1, sottraendo1, risultatoRuffini2;
    
    if(termineNoto < 0 )
    {
        a = -termineNoto; //Per il ciclo for ho bisogno di un numero positivo, se il termine noto è negativo si bugga
    }
    else
    {
        a = termineNoto;
    }
    
    for(i = 1; i <= a; i++)
    {
        if(termineNoto % i == 0)
        {
            divisoreTermineNoto = i;
            divisoreTermineNotoOpposto = -i;
            
            if( (pow(divisoreTermineNoto, 2) * coefficiente1 + divisoreTermineNoto * coefficiente2 + termineNoto) == 0)
            {
                risultatoRuffini1 = coefficiente1;
                sottraendo1 = coefficiente1 * divisoreTermineNoto;
                
                risultatoRuffini2 = coefficiente2 - sottraendo1;
                
                
                printf("(x%+d)(%+dx%+d)\n", divisoreTermineNotoOpposto, risultatoRuffini1, risultatoRuffini2);
                return 0;
                break;
            }
            
            if((pow(divisoreTermineNotoOpposto, 2) * coefficiente1 + divisoreTermineNotoOpposto * coefficiente2 + termineNoto) == 0)
            {
                risultatoRuffini1 = coefficiente1;
                sottraendo1 = coefficiente1 * divisoreTermineNotoOpposto;
                
                risultatoRuffini2 = coefficiente2 - sottraendo1;
                
                
                printf("(x%+d)(%+dx%+d)\n", divisoreTermineNoto, risultatoRuffini1, risultatoRuffini2);
                return 0;
                break;
            }
            if(i >= a)
            {
                printf("errore, lo zero del polinomio non è intero, prova un'altra scomposizione\n");
                return 1;
                break;
            }
            
        }
        
        if(i >= a)
        {
            printf("errore, lo zero del polinomio non è intero, prova un'altra scomposizione\n");
            break;
        }
    }
}

int ruffiniCon3Coefficienti(float coefficiente1, float coefficiente2, float coefficiente3, int termineNoto)
{
    int i, a, controllo = 0; //dichiarazione del contatore per il ciclo for
    int divisoreTermineNoto, divisoreTermineNotoOpposto, risultatoRuffini1, sottraendo1, risultatoRuffini2, sottraendo2, risultatoRuffini3; //dichiarazione delle variabili per il calcolo
    
    if(termineNoto < 0 )
    {
        a = -termineNoto; //Per il ciclo for ho bisogno di un numero positivo, se il termine noto è negativo si bugga
    }
    else
    {
        a = termineNoto;
    }
    
    for(i = 1; i <= a; i++)
    {
        
        if(termineNoto % i == 0)
        {
            divisoreTermineNoto = i;
            divisoreTermineNotoOpposto = -i;
            
            if( (pow(divisoreTermineNoto, 3) *coefficiente1) + (pow(divisoreTermineNoto, 2) * coefficiente2) + (divisoreTermineNoto * coefficiente3) + termineNoto == 0)
            {
                risultatoRuffini1 = coefficiente1;
                sottraendo1 = coefficiente1 * divisoreTermineNoto;
                
                risultatoRuffini2 = coefficiente2 - sottraendo1;
                sottraendo2 = risultatoRuffini2 * divisoreTermineNoto;
                
                risultatoRuffini3 = coefficiente3 - sottraendo2;
                
                printf("(x%+d)(%+dx²%+dx%+d)\n", divisoreTermineNotoOpposto, risultatoRuffini1, risultatoRuffini2, risultatoRuffini3);
                controllo = divisoreTermineNoto;
                return 0;
                break;
            }
            
            if((pow(divisoreTermineNotoOpposto, 3) *coefficiente1) + (pow(divisoreTermineNotoOpposto, 2) * coefficiente2) + (divisoreTermineNotoOpposto * coefficiente3) + termineNoto == 0)
            {
                risultatoRuffini1 = coefficiente1;
                sottraendo1 = coefficiente1 * divisoreTermineNotoOpposto;
                
                risultatoRuffini2 = coefficiente2 - sottraendo1;
                sottraendo2 = risultatoRuffini2 * divisoreTermineNotoOpposto;
                
                risultatoRuffini3 = coefficiente3 - sottraendo2;
                
                printf("(x%+d)(%+dx²%+dx%+d)\n", divisoreTermineNoto, risultatoRuffini1, risultatoRuffini2, risultatoRuffini3);
                controllo = divisoreTermineNotoOpposto;
                return 0;
                break;
            }
            
            if(i >= a)
            {
                printf("errore, lo zero del polinomio non è intero, prova un'altra scomposizione\n");
                return 1;
                break;
            }
        }
        
        if(i >= a)
        {
            printf("errore, lo zero del polinomio non è intero, prova un'altra scomposizione\n");
            return 1;
            break;
        }
        
    }
    
    if((ruffiniCon2Coefficienti(risultatoRuffini1, risultatoRuffini2, risultatoRuffini3) == 0) && (controllo > 0))
    {
        printf("(x%+d)", divisoreTermineNotoOpposto);
        ruffiniCon2Coefficienti;
    }
    
    if((ruffiniCon2Coefficienti(risultatoRuffini1, risultatoRuffini2, risultatoRuffini3) == 0) && (controllo < 0))
    {
        printf("(x%+d)", divisoreTermineNoto);
        ruffiniCon2Coefficienti;
    }
    else
    {
        printf("La scomposizione termina qui\n");
        return 0;
    }
    
}

ruffiniCon4Coefficienti(float coefficiente1, float coefficiente2, float coefficiente3, float coefficiente4, int termineNoto)
{
    int i, a, controllo = 0; //dichiarazione del contatore per il ciclo for
    int divisoreTermineNoto, divisoreTermineNotoOpposto, risultatoRuffini1, sottraendo1, risultatoRuffini2, sottraendo2, risultatoRuffini3, sottraendo3, risultatoRuffini4; //dichiarazione delle variabili per il calcolo
    
    if(termineNoto < 0 )
    {
        a = -termineNoto; //Per il ciclo for ho bisogno di un numero positivo, se il termine noto è negativo si bugga
    }
    else
    {
        a = termineNoto;
    }
    
    for( i = 1; i <= a; i++)
    {
        if(termineNoto % i == 0)
        {
            divisoreTermineNoto = i;
            divisoreTermineNotoOpposto = -i;
            
            if( ( (pow(divisoreTermineNoto, 4) * coefficiente1) + ( pow(divisoreTermineNoto, 3) * coefficiente2 ) + (pow(divisoreTermineNoto, 2) * coefficiente3) + (divisoreTermineNoto * coefficiente4) + termineNoto) == 0)
            {
                
                risultatoRuffini1 = coefficiente1;
                sottraendo1 = coefficiente1 * divisoreTermineNoto;
                
                risultatoRuffini2 = coefficiente2 - sottraendo1;
                sottraendo2 = risultatoRuffini2 * divisoreTermineNoto;
                
                risultatoRuffini3 = coefficiente3 - sottraendo2;
                sottraendo3 = risultatoRuffini3 * divisoreTermineNoto;
                
                risultatoRuffini4 = coefficiente4 - sottraendo3;
                
                printf("(x%+d)(%+dx³%+dx²%+dx+%+d)\n", divisoreTermineNotoOpposto, risultatoRuffini1, risultatoRuffini2, risultatoRuffini3, risultatoRuffini4);
                
                controllo = divisoreTermineNoto;
                
                return 0;
                break;
                
            }
            
            if( ( (pow(divisoreTermineNotoOpposto, 4) * coefficiente1) + (pow(divisoreTermineNotoOpposto, 3) * coefficiente2) + (pow(divisoreTermineNotoOpposto, 2) * coefficiente3) + (divisoreTermineNotoOpposto * coefficiente4) + termineNoto) == 0)
            {
                risultatoRuffini1 = coefficiente1;
                sottraendo1 = coefficiente1 * divisoreTermineNotoOpposto;
                
                risultatoRuffini2 = coefficiente2 - sottraendo1;
                sottraendo2 = risultatoRuffini2 * divisoreTermineNotoOpposto;
                
                risultatoRuffini3 = coefficiente3 - sottraendo2;
                sottraendo3 = risultatoRuffini3 * divisoreTermineNotoOpposto;
                
                risultatoRuffini4 = coefficiente4 - sottraendo3;
                
                printf("(x%+d)(%+dx³%+dx²%+dx+%+d)\n", divisoreTermineNoto, risultatoRuffini1, risultatoRuffini2, risultatoRuffini3, risultatoRuffini4);
                
                controllo = divisoreTermineNotoOpposto;
                
                return 0;
                break;
            }
            
            if(i >= a)
            {
                printf("errore, lo zero del polinomio non è intero, prova un'altra scomposizione\n");
                return 1;
                break;
            }
        }
        if(i >= a)
        {
            printf("errore, lo zero del polinomio non è intero, prova un'altra scomposizione\n");
            return 1;
            break;
        }
    }
    if((ruffiniCon3Coefficienti(risultatoRuffini1, risultatoRuffini2, risultatoRuffini3, risultatoRuffini4) == 0) && (controllo > 0))
    {
        printf("(x%+d)", divisoreTermineNotoOpposto);
        ruffiniCon3Coefficienti;
    }
    if((ruffiniCon3Coefficienti(risultatoRuffini1, risultatoRuffini2, risultatoRuffini3, risultatoRuffini4) == 0) && (controllo < 0))
    {
        printf("(x%+d)", divisoreTermineNoto);
        ruffiniCon3Coefficienti;
    }
}

int main()
{
    int *ptr = (int *) malloc(10 * sizeof (int));
    if (ptr == NULL) {
        /* Memory could not be allocated, the program should
         handle the error here as appropriate. */
    } else {
        /* Allocation succeeded.  Do something.  */
        free(ptr);  /* We are done with the int objects, and
                     free the associated pointer. */
        ptr = NULL; /* The pointed-to-data  must not be used again,
                     unless re-assigned by using malloc
                     again. */
    }
    signed int coefficienteTermineAllaQuarta, coefficienteTermineCubico, coefficienteTermineQuadratico, coefficienteTermineLineare, terNoto, scelta;
    char continuare;
    printf("Benvenuto nel programma per le semplificazioni con Ruffini!\n\t IMPORTANTE : Inserisci i segni dei coefficienti\nRicorda di usare Ruffini solo se non puoi scomporre in altri modi!\n\n\n");
    do
    {
        printf("[1] Ruffini per polinomi di secondo grado\n[2] Ruffini per polinomi di terzo grado\n[3] Ruffini per polinomi di quarto grado\n[0] Esci\n");
        scanf("%d", &scelta);
        switch(scelta)
        {
            case 1:
                
                printf("Inserisci il coefficiente del termine al quadrato:\n");
                scanf(" %d", &coefficienteTermineQuadratico);
                
                printf("Inserisci il coefficiente del termine lineare (elevato alla 1):\n");
                scanf(" %d", &coefficienteTermineLineare);
                
                printf("Inserisci il termine noto:\n");
                scanf(" %d", &terNoto);
                
                ruffiniCon2Coefficienti(coefficienteTermineQuadratico, coefficienteTermineLineare, terNoto);
                
                printf("Vuoi continuare a fare calcoli? (s/n)\n");
                scanf(" %c", &continuare);
                
                if(continuare == 's' || continuare == 'S')
                {
                    clearscreen();
                }
                break;
                
            case 2:
                
                printf("Inserisci il coefficiente del termine al cubo:\n");
                scanf(" %d", &coefficienteTermineCubico);
                
                printf("Inserisci il coefficiente del termine al quadrato:\n");
                scanf(" %d", &coefficienteTermineQuadratico);
                
                printf("Inserisci il coefficiente del termine lineare (elevato alla 1):\n");
                scanf(" %d", &coefficienteTermineLineare);
                
                printf("Inserisci il termine noto:\n");
                scanf(" %d", &terNoto);
                
                ruffiniCon3Coefficienti(coefficienteTermineCubico, coefficienteTermineQuadratico, coefficienteTermineLineare, terNoto);
                
                printf("Vuoi continuare a fare calcoli? (s/n)\n");
                scanf(" %c", &continuare);
                
                if(continuare == 's' || continuare == 'S')
                {
                    clearscreen();
                }
                break;
                
            case 3:
                
                printf("Inserisci il coefficiente del termine alla quarta:\n");
                scanf(" %d", &coefficienteTermineAllaQuarta);
                
                printf("Inserisci il coefficiente del termine al cubo:\n");
                scanf(" %d", &coefficienteTermineCubico);
                
                printf("Inserisci il coefficiente del termine al quadrato:\n");
                scanf(" %d", &coefficienteTermineQuadratico);
                
                printf("Inserisci il coefficiente del termine lineare (elevato alla 1):\n");
                scanf(" %d", &coefficienteTermineLineare);
                
                printf("Inserisci il termine noto:\n");
                scanf(" %d", &terNoto);
                
                ruffiniCon4Coefficienti(coefficienteTermineAllaQuarta, coefficienteTermineCubico, coefficienteTermineQuadratico, coefficienteTermineLineare, terNoto);
                
                printf("Vuoi continuare a fare calcoli? (s/n)\n");
                scanf(" %c", &continuare);
                
                if(continuare == 's' || continuare == 'S')
                {
                    clearscreen();
                }
                break;
                
            case 0:
                continuare = 'n';
                break;
                
            default:
                
                continuare = 's';
                clearscreen();
                printf("\nHai sbagliato!\n\n");
                continue;
                
        }
        
    }while(continuare == 's' || continuare == 'S');
    
    return 0;
}
