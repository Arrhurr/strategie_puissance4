// Ceci est l'IA de Arthur Donou et Guillaume Puill en 1B2.
// Nous avons pris l'IA de Guillaume Puill et rajoute quelque elements afin d'ameliorer 
// le debut de jeu de l'ancienne IA qui etait faible.
// Il y a encore des ameliorations possibles mais j'ai du mal a savoir d'ou viennent les 
// failles du system de decision.
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define LIGNE 6
#define COLONNE 7

typedef int t_grille[LIGNE][COLONNE];
typedef int t_coupPossible[COLONNE];
 

int chercherLigne(t_grille laGrille, int col);
int maStrategie(t_grille laGrille);
int main(int argc, char** argv);
void chargerGrille(char** argv, t_grille grille);
void balayage(t_grille laGrille, t_coupPossible coupPossible);
int compteur(t_grille tab);
bool compteurPrems(t_grille tab);
bool alignementHorizontalDeUnPion(t_grille grille, int ligne, int colonne, int pionsAlignes);
bool alignementVerticalDeUnPion(t_grille grille, int ligne, int colonne, int pionsAlignes);
bool alignementDiagonaleNordEstVersSudOuest(t_grille grille, int ligne, int colonne, int pionsAlignes);
bool alignementDiagonaleNordOuestVersSudEst(t_grille grille, int ligne, int colonne, int pionsAlignes);
bool alignementDesPions(t_grille grille, int ligne, int colonne, int pionsAlignes);
int comparateur(t_grille grille, t_coupPossible coupPossible, int pionsAlignes,int joueur);
int comparateurVertical(t_grille grille, t_coupPossible coupPossible, int pionsAlignes,int joueur);
int comparateurHorizontal(t_grille grille, t_coupPossible coupPossible, int pionsAlignes,int joueur);
int choiMin(t_grille laGrille);
const int FAUX =-1;
const int VIDE = 0;
const int JOUEUR = 1;
const int ADVERSAIRE = 2;

// Faire et utiliser une fonction qui check si on joue en premier.
// adapter dans la prise de décision avec l'information de la fonction 
// Si on est premier jouer de manière aggressive 
// Sinon jouer de manière defensives


int chercherLigne(t_grille laGrille, int colonne){
    //Recherche la ligne plus basse dans une colonne.
    int ligne = -1;
    while (ligne<LIGNE-1 && laGrille[ligne+1][colonne]==VIDE){
        ligne++;
    }
    return ligne;
}

//Utilise notamment balayge et comparateur
int maStrategie(t_grille laGrille){
    // faire une variable global de la grille pour savoir quel est le dernier pion jouer.
    //comparer la grille que l'on a et celle que l'on recoit
    int resultat,tampon;//
    t_coupPossible coupPossible;
    balayage(laGrille,coupPossible);
    tampon=comparateur(laGrille, coupPossible,4,JOUEUR);//Si on peut faire un Puissance 4.
    if (tampon==FAUX)//Si un puissance 4 est possible.
    {
        tampon=comparateur(laGrille, coupPossible,4,ADVERSAIRE);//Si l'adversaire peut faire un puissance 4.
        if (tampon==FAUX)
        {
            tampon=comparateurVertical(laGrille, coupPossible,3,ADVERSAIRE);//Si l'adversaire place deux pions l'un au dessus de l'autre
            if (tampon==FAUX)
            {
                if (compteur(laGrille)<=(LIGNE*COLONNE)/2)
                {
                    if (!compteurPrems(laGrille))
                    {
                        tampon=comparateurHorizontal(laGrille, coupPossible,3,ADVERSAIRE);
                    }

                }
                if (compteur(laGrille)>(LIGNE*COLONNE)/2){
                    tampon=comparateur(laGrille,coupPossible,3,ADVERSAIRE);
                }
                if (tampon==FAUX)
                {
                    tampon=comparateur(laGrille, coupPossible,3,JOUEUR);
                }
                
                if (tampon==FAUX)
                {
                    if (laGrille[0][3]==VIDE)//Si la colonne 4 n'est pas remplie, place le pion dans la colonne 4
                    {
                        tampon = 3;
                    }
                    else if (laGrille[0][4]==VIDE)//Si la colonne 5 n'est pas remplie, place le pion dans la colonne 5
                    {
                        tampon = 4;
                    }
                    else if (laGrille[0][2]==VIDE)//Si la colonne 3 n'est pas remplie, place le pion dans la colonne 2
                    {
                        tampon = 2;
                    }
                    else//Jouez dans la colonne avec le moins de pions.
                    {
                        tampon = choiMin(laGrille);
                    }
                }
            }
        }
    }
    resultat=tampon;
    return resultat;
}
//Fonction qui vient repertorier les emplacement des pions jouables, l'index du tableau t_coupPossible est egale 
//a l'index de la colonne et la valeur dans le tableau est egal a la valeur de la ligne.
//ce tableau va permettre de simuler nos coups ainsi que ceux de notre adversaire.
void balayage(t_grille laGrille, t_coupPossible coupPossible){
    
    for (int i = 0; i < COLONNE; i++)
    {
        coupPossible[i]=chercherLigne(laGrille,i);
    }
}
// Compte le nombre de pion present dans la grille
int compteur(t_grille tab){
    int i,j,cpt;
    cpt=0;
    for(i=0;i<LIGNE;i++){
        for(j=0;j<COLONNE;j++){
            if(tab[i][j]!=VIDE){
                cpt++;
            }
        }
    }
    return cpt;
}

//Verification de qui joue en premier, renvoie true si on est premier false sinon
bool compteurPrems(t_grille tab){
    int i,j,cpt;
    bool res;
    cpt=0;
    for(i=0;i<LIGNE;i++){
        for(j=0;j<COLONNE;j++){
            if(tab[i][j]!=VIDE){
                cpt++;
            }
        }
    }
    if (cpt%2==0){
        res=true;
    }
    else{
        res=false;
    }
    return res;
}

//verfie un alignement d'un certain nombre de pion de maniere horizontale. Est utilisé dans alignementDesPions.
bool alignementHorizontalDeUnPion(t_grille grille, int ligne, int colonne, int pionsAlignes){
    bool resultat;
    int i,compteurHorizon;
    int jetonType;
    jetonType=grille[ligne][colonne];
    resultat = false;
    i=colonne-(pionsAlignes);//ligne du pion plus le nombre de pions qui suivent
    if (i<0)
    {
        i=0;
    }
    while ((i<COLONNE)&&(resultat==false))
    {
        if (grille[ligne][i]!=jetonType)
        {
            compteurHorizon=0;
        }else
        {
            compteurHorizon++;
        }
        if (compteurHorizon==(pionsAlignes-1))
        {
            resultat=true;
        }
        i++;
    }
    return resultat;
}
//verfie un alignement d'un certain nombre de pion de maniere Verticale. Est utilisé dans alignementDesPions.
bool alignementVerticalDeUnPion(t_grille grille, int ligne, int colonne, int pionsAlignes){
    bool resultat;
    int i,jetonType;
    jetonType=grille[ligne][colonne];
    resultat = true;
    i=0;
    while ((i<(pionsAlignes))&&(resultat==true))
    {
        if (grille[ligne+i][colonne]==jetonType)
        {
            i++;
        }
        else
        {
            resultat=false;
        }
    }
    return resultat;
}
//verfie un alignement d'un certain nombre de pion en Diagonale du Nord-Est vers le Sud-Ouest. Est utilisé dans alignementDesPions.
bool alignementDiagonaleNordEstVersSudOuest(t_grille grille, int ligne, int colonne, int pionsAlignes){
    bool resultat;
    int j, k,compteurDiag;//j=vertical,k=horizontal
    int jetonType;
    resultat=false;
    jetonType=grille[ligne][colonne];
    j=ligne;
    k=colonne;
    compteurDiag=0;
    while ((j>0)&&(k<COLONNE))// on amene les valeurs en haut a droite du tableau j tent vers 0 et k tent vers 6
    {
        j--;
        k++;
    }
    while ((j<LIGNE)&&(k>=0))
    {
        if (grille[j][k]==jetonType)
        {
            compteurDiag++;
        }else
        {
            compteurDiag=0;
        }
        if (compteurDiag==pionsAlignes-1)
        {
            resultat=true;
        }
        j++;
        k--;
    }
    return resultat;  
}
//verfie un alignement d'un certain nombre de pion en Diagonale du Nord-Ouest vers le Sud-Est. Est utilisé dans alignementDesPions.
bool alignementDiagonaleNordOuestVersSudEst(t_grille grille, int ligne, int colonne, int pionsAlignes){
    bool resultat;
    int j, k,compteurDiag,jetonType;
    j=ligne;
    k=colonne;
    resultat=false;
    jetonType=grille[ligne][colonne];
    compteurDiag=0;
    while ((j>0)&&(k>0))// on amene les valeurs en haut a droite du tableau j tent vers 0 et k tent vers 6
    {
        j--;
        k--;
    }
    while ((j<LIGNE)&&(k<COLONNE))
    {
        if (grille[j][k]==jetonType)
        {
            compteurDiag++;
        }else
        {
            compteurDiag=0;
        }
        if (compteurDiag==pionsAlignes-1)
        {
            resultat=true;
        }
        j++;
        k++;
    }
    return resultat;
}

//Cette fonction va effectuer une recherche complete d'un alignement de pions donné si jamais une des vérifications est vrai alors on n'a pas a faire les suivantes.
bool alignementDesPions(t_grille grille, int ligne, int colonne, int pionsAlignes){
    bool resultat;
    resultat = false;
    if (!resultat)
    {
        resultat=alignementHorizontalDeUnPion(grille,ligne,colonne,pionsAlignes);
        if (!resultat)
        {
            resultat=alignementVerticalDeUnPion(grille,ligne,colonne,pionsAlignes);
            if (!resultat)
            {
                resultat=alignementDiagonaleNordEstVersSudOuest(grille,ligne,colonne,pionsAlignes);
                if (!resultat)
                {
                    resultat=alignementDiagonaleNordOuestVersSudEst(grille,ligne,colonne,pionsAlignes);
                }
            }
        }
    }
    return resultat;
}


//Compare la liste des coups possibles obtenu avec balayage, avec les recherches de pions alignes.
int comparateur(t_grille grille, t_coupPossible coupPossible, int pionsAlignes,int joueur){
    int resultat,i;
    t_grille grilleModifie;
    i=0;
    resultat=FAUX;
    while ((i<COLONNE)&&(resultat==FAUX))
    {
        //Creation d'une fausse grille.
        for(int x = 0; x < LIGNE; x++){
            for(int y = 0; y < COLONNE; y++){
                grilleModifie[x][y] = grille[x][y];
            }
        }
        //verification si la grille remplie les condition remplie dans la fonction
        while (coupPossible[i]==FAUX)
        {
            i++;
        }
        grilleModifie[coupPossible[i]][i]=joueur;
        if (alignementDesPions(grilleModifie,coupPossible[i],i,pionsAlignes))
        {
            resultat=i;
        }
        
        i++;
    }
    return resultat;
}

//Compare la liste des coups possibles obtenu avec balayage, avec la recherche de pions verticale.
int comparateurVertical(t_grille grille, t_coupPossible coupPossible, int pionsAlignes,int joueur){
    int resultat,i;
    t_grille grilleModifie;
    i=0;
    resultat=FAUX;
    while ((i<COLONNE)&&(resultat==FAUX))
    {
        //Creation d'une fausse grille.
        for(int x = 0; x < LIGNE; x++){
            for(int y = 0; y < COLONNE; y++){
                grilleModifie[x][y] = grille[x][y];
            }
        }
        //verification si la grille remplie les condition remplie dans la fonction
        if (coupPossible[i]!=FAUX)
        {
            grilleModifie[coupPossible[i]][i]=joueur;
            if (alignementVerticalDeUnPion(grilleModifie,coupPossible[i],i,pionsAlignes))
            {
                resultat=i;
            }
        }
        i++;
    }
    return resultat;
}

//Compare la liste des coups possibles obtenu avec balayage, avec la recherche de pions horizontal.
int comparateurHorizontal(t_grille grille, t_coupPossible coupPossible, int pionsAlignes,int joueur){
    int resultat,i;
    t_grille grilleModifie;
    i=0;
    resultat=FAUX;
    while ((i<COLONNE)&&(resultat==FAUX))
    {
        //Creation d'une fausse grille.
        for(int x = 0; x < LIGNE; x++){
            for(int y = 0; y < COLONNE; y++){
                grilleModifie[x][y] = grille[x][y];
            }
        }
        //verification si la grille remplie les condition remplie dans la fonction
        if (coupPossible[i]!=FAUX)
        {
            grilleModifie[coupPossible[i]][i]=joueur;
            if (alignementHorizontalDeUnPion(grilleModifie,coupPossible[i],i,pionsAlignes))
            {
                resultat=i;
            }
        }
        i++;
    }
    return resultat;
}

// Renvoie la valeur de la colonne dans laquelle il y a le moins de pion.
int choiMin(t_grille laGrille){
    int i, id_col, val_col;
    id_col=0;
    for ( i = 0; i < COLONNE; i++)
    {
        val_col=chercherLigne(laGrille,i);
        if (val_col>(chercherLigne(laGrille,id_col)))
        {
            id_col=i;
        }
        
    }
    return id_col;
}

//Pas touche
int main(int argc, char** argv) 
{
    t_grille grille;

    chargerGrille(argv, grille);

    return maStrategie(grille);
}
// Charge la grille du tour actuel
// Vous n'avez pas à modifier cette fonction
void chargerGrille(char** argv, t_grille grille) 
{
    for(int i = 0; i < LIGNE; i++)
        for(int j = 0; j < COLONNE; j++)
            grille[i][j] = atoi(argv[i * COLONNE + j + 1]);
}




