#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NB_LIGNES 6
#define NB_COLONNES 7
const int VIDE = 0;
const int JOUEUR = 1;
const int ADVERSAIRE = 2;

typedef int t_grille[NB_LIGNES][NB_COLONNES];
typedef int connaitre[NB_COLONNES];

bool grilleVide(t_grille laGrille);
int strategie_Arthur(t_grille tab, int J);
int seraVainqueur(t_grille laGrille, int lig, int col);
void chargerGrille(char** argv, t_grille grille);
int chercherLigne(t_grille laGrille, int col);
int colLig(t_grille tab,char J);






int main(int argc, char** argv) 
{
    t_grille grille;

    chargerGrille(argv, grille);

    return strategie_Arthur(grille ,1);
}

int chercherLigne(t_grille laGrille, int col){
    int ligne = -1;

    while (ligne<NB_LIGNES-1 && laGrille[ligne+1][col]==VIDE){
        ligne++;
    }
    return ligne;
}

void chargerGrille(char** argv, t_grille grille) 
{
    for(int i = 0; i < NB_LIGNES; i++)
        for(int j = 0; j < NB_COLONNES; j++)
            grille[i][j] = atoi(argv[i * NB_COLONNES + j + 1]);
}


int colLig(t_grille tab,char J){
    int k,ligne,colTrouver;
    k=0;
    colTrouver=-1;
    while(k<NB_COLONNES && colTrouver==-1){
        ligne=chercherLigne(tab,k)+1;
        if(ligne<NB_LIGNES){
            if(tab[ligne][k]!=J && tab[ligne][k]!=VIDE){   
                colTrouver=seraVainqueur(tab,ligne,k);
            }
        }
        k++;
    }
    return colTrouver;
}
int strategie_Arthur(t_grille tab, int J){
    bool vide;
    bool vaGagner=false;
    int colgagnante=-1;
    vide = grilleVide(tab);
    bool rushRT=false;
    int lig,col;
    col=-1;
    if (vide){  //si on est le premier à jouer
        col=3;
        lig=chercherLigne(tab,col);
    }
    if(tab[NB_LIGNES-1][3]== J || tab[NB_LIGNES-1][3]==VIDE){ //si on a joué en premier (rush de la victoire)
        if(colLig(tab,J)!=-1){
            col=colLig(tab,J);
            lig=chercherLigne(tab,col);
        }
        else if(tab[chercherLigne(tab,2)][3]==J){ //On suit la stratégie en vérifiant que les cases autour du centre qu'on possède sont vides
            col=2;
            lig=chercherLigne(tab,col);
        }
        else if((tab[chercherLigne(tab,4)][3]) == J){
            col=4;
            lig=chercherLigne(tab,col);
        }
        else if(tab[chercherLigne(tab,1)][3] == J){
            col=1;
            lig=chercherLigne(tab,col);
        }
        else if(tab[chercherLigne(tab,5)][3]== J){
            col=5;
            lig=chercherLigne(tab,col);
        }
        else{
            rushRT=true;
        }
    }
    if((rushRT || tab[NB_LIGNES-1][3]!=J) && !vide){
        if(colLig(tab,J)!=-1){
            col=colLig(tab,J);
            lig=chercherLigne(tab,col);
        }
        else{
            if((tab[chercherLigne(tab,4)][3])==J){ //On suit la stratégie en vérifiant que la case à côté du centre est libre
                col=2;
                lig=chercherLigne(tab,col);
            }
            else if((tab[chercherLigne(tab,4)][3]) == J){
                col=4;
                lig=chercherLigne(tab,col);
            }
            else if(tab[chercherLigne(tab,1)][3] == J){
                col=1;
                lig=chercherLigne(tab,col);
            }
            else if(tab[chercherLigne(tab,5)][3]== J){
                col=5;
                lig=chercherLigne(tab,col);
            }
            else{
                col=3;
                lig=chercherLigne(tab,col);
            }
        }
    }
    
    
    return col;
}


bool grilleVide(t_grille laGrille){
    bool vide = true;
    int c = 0;
    while (vide && c<NB_COLONNES){
        if (laGrille[NB_LIGNES-1][c] != VIDE){
            vide = false;
        }
        c++;
    }
    return vide;
}

int seraVainqueur(t_grille laGrille, int lig, int col){ //renvoie la colonne à bloquer
    char lePion = laGrille[lig][col];
    int cpt,i,j;
    int colTrouver=-1;

    // regarder la verticale, en comptant le nombre de pions au Sud (inutile de regarder au Nord du pion qui vient de tomber)
    i = lig;
    cpt = 0;
    while (i<NB_LIGNES && i<lig+4){
        if(laGrille[i][col]==lePion){
            cpt++;
        }
        i++;
    }
    if (cpt>=3){
        return col;
    }
    // regarder l'horizontale, en comptant le nombre de pions Ã  l'Est et Ã  l'Ouest
    j = col;
    cpt = 0;
    // on regarde Ã  l'est
    while (j>=0 && j>col-4){
        if(laGrille[lig][j]==lePion){
            cpt++;
        }
        else if(colTrouver==-1 && laGrille[lig][j]==VIDE){
            colTrouver=j;
        }
        j--;
    }
    j = col+1;
    // on regarde Ã  l'ouest
    while (j<NB_COLONNES && j<col+4){
        if(laGrille[lig][j]==lePion){
            cpt++;
        }
        else if(colTrouver==-1 && laGrille[lig][j]==VIDE){
            colTrouver=j;
        }
        j++;
    }
    if (cpt>=3 ){
        return colTrouver;
    }
    // regarder la diagonale descendante, en comptant le nombre de pions au Nord Ouest et au Sud Est
    i = lig;
    j = col;
    cpt = 0;
    // on regarde au Nord Ouest
    while (j>=0 && i>=0 && i>lig-4 && j>col-4){
        if(laGrille[i][j]==lePion){
            cpt++;
        }
        else if(colTrouver==-1 && laGrille[lig][j]==VIDE){
            colTrouver=j;
        }
        i--;
        j--;
    }
    i = lig+1;
    j = col+1;
    // on regarde au Sud Est
    while (i<NB_LIGNES && j<NB_COLONNES && i<lig+4 && j<col+4){
        if(laGrille[i][j]==lePion){
            cpt++;
        }
        else if(colTrouver==-1 && laGrille[lig][j]==VIDE){
            colTrouver=j;
        }
        i++;
        j++;
    }
    if (cpt>=3 ){
        return colTrouver;
    }
    // regarder la diagonale descendante, en comptant le nombre de pions au Nord Est et au Sud Ouest
    i = lig;
    j = col;
    cpt = 0;
    // on regarde au Nord Est
    while (j<NB_COLONNES && i>=0 && i>lig-4 && j<col+4){
        if(laGrille[i][j]==lePion){
            cpt++;
        }
        else if(colTrouver==-1 && laGrille[lig][j]==VIDE){
            colTrouver=j;
        }
        i--;
        j++;
    }
    i = lig+1;
    j = col-1;
    // on regarde au Sud Ouest
    while (i<NB_LIGNES && j>=0 && i<lig+4 && j>col-4){
        if(laGrille[i][j]==lePion){
            cpt++;
        }
        else if(colTrouver==-1 && laGrille[lig][j]==VIDE){
            colTrouver=j;
        }
        i++;
        j--;
    }
    if (cpt>=3 ){
        return colTrouver;
    }
    return -1;
}
