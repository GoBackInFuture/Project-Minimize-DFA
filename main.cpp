#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

const int dimensiuneMaxMatrice = 50;
const int dimensiuneMaxCuvant = 40;

void initializareMatrice(int n,int m,int a[][dimensiuneMaxMatrice])
{
    int i,j;
    for( i = 0 ; i < n; i++)
        for(j = 0; j < m ; j++)
            a[i][j] = -1;
}

void initializareMatrice2(int n,int a[][dimensiuneMaxMatrice])
{
     int i,j;
    for( i = 0 ; i < n; i++)
        for(j = 0; j < n ; j++)
            a[i][j] = 0;
}

int pozitie(char x,char alfabet[40],int NumarLitereAlfabet)
{
    int i;
    for(i = 0 ; i < NumarLitereAlfabet; i++)
        if(x == alfabet[i])
            return i;
    return -1;
}
int stareFin(int i,int nrStariFinale,int StariFinale[20])
{
    int j;
    for(j = 0 ; j < nrStariFinale; j++)
        if(StariFinale[j] == i)
            return 0;
    return -1;
}
void afisare_matrice(int n,int matrice[dimensiuneMaxMatrice][dimensiuneMaxMatrice])
{
    int i,j;
    for(i = 0 ; i < n ; i++)
    {
        for(j = 0 ; j < i; j++)
            cout << matrice[i][j] << " ";
        cout << endl;
    }
}
void citire_date_automat(int &NumarStari,int &NumarTranzitii,
                         int &NumarLitereAlfabet,int DFA[][50],
                         char alfabet[40],int &NumarStariFinale,
                         int StariFinale[20])
{
    int i, q1, q2, poz;
    char x;
    ifstream f("input.in");

    f >> NumarStari;
    f >> NumarTranzitii;
    f >> NumarLitereAlfabet;
    f >> NumarStariFinale;

    for(i = 0; i < NumarLitereAlfabet; i++)
    {
        f >> alfabet[i];
    }

    for(i = 0; i < NumarTranzitii; i++)
    {
        f >> q1 >> x >> q2;
        poz = pozitie(x,alfabet,NumarLitereAlfabet);
        DFA[poz][q1] = q2;
    }

    for(i = 0 ; i < NumarStariFinale; i++)
    {
        f >> StariFinale[i];
    }
    f.close();
}
void construiesteMatriceEchivalenta(int NumarStari,int NumarLitereAlfabet,
                                    int MatriceEchivalenta[][dimensiuneMaxMatrice],int DFA[][dimensiuneMaxMatrice],
                                    char alfabet[40])
{
     int check = 1;

    // Constructie matrice echivalenta
    int i,j,k;
    while(check == 1)
    {
    check = 0;
    for(i = 0 ; i < NumarStari; i++)
        for(j = 0 ; j < i; j++)
        {
            if(MatriceEchivalenta[i][j] == 0)
            {
                for(k = 0 ; k < NumarLitereAlfabet; k++)
                {
                    int l1 = DFA[pozitie(alfabet[k],alfabet,NumarLitereAlfabet)][i];
                    int l2 = DFA[pozitie(alfabet[k],alfabet,NumarLitereAlfabet)][j];
                    if(l1!=-1 && l2!= -1 && MatriceEchivalenta[max(l1,l2)][min(l1,l2)] == 1)
                        {
                            MatriceEchivalenta[max(i,j)][min(i,j)] = 1;
                            check = 1;
                        }
                }
            }
        }
    }
}
int main()
{
    int DFA[dimensiuneMaxMatrice][dimensiuneMaxMatrice];
    int NumarStari, NumarTranzitii, StariFinale[20], NumarStariFinale;
    int NumarLitereAlfabet, poz, StareUrmatoare, linie, ok , q1, q2, i,j;

    NumarTranzitii = StareUrmatoare  = linie = 0;
    ok = 1;

    char x;
    char alfabet[40];

    initializareMatrice(30,30,DFA);
    citire_date_automat(NumarStari,NumarTranzitii,NumarLitereAlfabet,
                        DFA,alfabet,NumarStariFinale,StariFinale);

    int MatriceEchivalenta[dimensiuneMaxMatrice][dimensiuneMaxMatrice];
    initializareMatrice2(30,MatriceEchivalenta);

    for(i = 0 ; i < NumarStari-1; i++)
        for(j = i+1; j <NumarStari; j++)
            if(stareFin(i,NumarStariFinale,StariFinale)!=stareFin(j,NumarStariFinale,StariFinale))
                MatriceEchivalenta[max(i,j)][min(i,j)] = 1;

    //afisare_matrice(NumarStari,MatriceEchivalenta);
    construiesteMatriceEchivalenta(NumarStari,NumarLitereAlfabet,MatriceEchivalenta,DFA,alfabet);
    afisare_matrice(NumarStari,MatriceEchivalenta);
    cout << '\n';

    // Constructie matrice temporara pentru DFA-ul minimal rezultat

    int MatriceTemporara[20][20];
    for(i = 0 ; i <20; i++)
        for(j = 0 ; j < 20; j++)
            MatriceTemporara[i][j] = -1;

    int v[10];
    for(i = 0 ; i < 10; i++)
        v[i] = 0;

    for(i = 0 ; i < NumarStari; i++)
        for(j = 0 ; j < i; j++)
            if(MatriceEchivalenta[i][j] == 0)
                {
                    MatriceTemporara[min(i,j)][v[min(i,j)]] = max(i,j);
                    v[min(i,j)]++;
                }
    // Afisare matrice temporara
    //for(i = 0 ; i < 5; i++)
      //  {
        //    for(j = 0 ; j < 5; j++)
          //      cout << MatriceTemporara[i][j]<< " ";
           // cout << endl;
        //}

    cout << '\n';
    int NumarStariAutomatMinimal = 0;

    int vectorStari[20];

    for(i = 0 ; i <20; i++)
        vectorStari[i] = 0;

    int k;
    for(i = 0 ; i < NumarStari; i++)
    {
        if(MatriceTemporara[i][0] == -1 && v[i]!=-1)
        {
            vectorStari[NumarStariAutomatMinimal] *=10;
            vectorStari[NumarStariAutomatMinimal] += i;
            if(i == 0)
                vectorStari[NumarStariAutomatMinimal] *=10;
            NumarStariAutomatMinimal++;
        }
        else if (v[i]!=-1)
        {
            vectorStari[NumarStariAutomatMinimal] *=10;
            vectorStari[NumarStariAutomatMinimal] += i;
            for(j = 0 ; j < v[i]; j++)
            {
                vectorStari[NumarStariAutomatMinimal] *=10;
                vectorStari[NumarStariAutomatMinimal]+= MatriceTemporara[i][j];
                v[MatriceTemporara[i][j]] = -1;
            }
            if(i == 0)
                vectorStari[NumarStariAutomatMinimal] *=10;
            NumarStariAutomatMinimal++;
        }
    }

    int DFAmin[dimensiuneMaxMatrice][dimensiuneMaxMatrice];

    for(i = 0 ; i < 20; i++)
        for(j = 0 ; j < 20; j++)
            DFAmin[i][j] = -1;

    cout << "Numarul de stari al automatului minimal este de: " << NumarStariAutomatMinimal << '\n';

    for(i = 0 ; i < NumarStariAutomatMinimal; i++)
        cout << vectorStari[i] << " ";
    cout << '\n';
    for(j = 0 ; j < NumarStariAutomatMinimal; j++)
    {
        int cop = vectorStari[j];
        if(cop == 0)
        {
            for(k = 0 ; k < NumarStariAutomatMinimal; k++)
            {
                int copk = vectorStari[k];
                while(copk)
                {
                    for(i = 0 ; i < NumarLitereAlfabet; i++)
                    {
                        if( DFA[pozitie(alfabet[i],alfabet,NumarLitereAlfabet)][cop%10] == copk%10)
                        {
                            DFAmin[pozitie(alfabet[i],alfabet,NumarLitereAlfabet)][j] = k;
                        }
                        else
                            if(DFA[pozitie(alfabet[i],alfabet,NumarLitereAlfabet)][copk%10] == cop%10)
                            {

                                DFAmin[pozitie(alfabet[i],alfabet,NumarLitereAlfabet)][min(j,k)] = j;
                            }
                    }
                copk/=10;
                }
            }
        }
        else
        while(cop)
        {

            for(k = 0 ; k < NumarStariAutomatMinimal; k++)
            {
                int copk = vectorStari[k];
                while(copk)
                {
                    for(i = 0 ; i < NumarLitereAlfabet; i++)
                    {
                        if( DFA[pozitie(alfabet[i],alfabet,NumarLitereAlfabet)][cop%10] == copk%10  && DFAmin[pozitie(alfabet[i],alfabet,NumarLitereAlfabet)][j] == -1 )
                        {
                            DFAmin[pozitie(alfabet[i],alfabet,NumarLitereAlfabet)][j] = k;
                        }
                        else
                            if(DFA[pozitie(alfabet[i],alfabet,NumarLitereAlfabet)][copk%10] == cop%10  && DFAmin[pozitie(alfabet[i],alfabet,NumarLitereAlfabet)][j] == -1)
                            {

                                DFAmin[pozitie(alfabet[i],alfabet,NumarLitereAlfabet)][min(j,k)] = j;
                            }
                    }
                copk/=10;
                }
                }
        cop = cop/10;
        }

    }
    cout << '\n';
    int StariFinaleAutomatMinimal[20];
    for(i = 0 ; i < 10; i ++)
        StariFinaleAutomatMinimal[i] = -1;

    int NumarStariFinaleAutMinim = 0;

    for(i = 0 ; i < NumarStariFinale; i++)
    {
        for(j = 0 ; j < NumarStariAutomatMinimal; j++ )
        {
            int k = vectorStari[j];
            while(k)
            {
                if(k%10 == StariFinale[i])
                {
                    int ok = 0;
                    int l;
                    for(l = 0 ; l < NumarStariFinaleAutMinim && ok == 0; l++)
                        if(StariFinaleAutomatMinimal[l] == j)
                            ok = 1;
                    if (ok==0)
                    {
                        StariFinaleAutomatMinimal[NumarStariFinaleAutMinim] = j;
                        NumarStariFinaleAutMinim++;
                    }

                }
                k/=10;
            }
        }
    }
    cout << "Matricea acestui automat este: \n";
    for(i = 0 ; i < NumarLitereAlfabet; i++)
    {
       for(j = 0 ; j < NumarStariAutomatMinimal; j++)
            cout << DFAmin[i][j] << " ";
        cout << '\n';
    }
    cout << "Starile finale ale acestui automat sunt: \n";
    for(i = 0 ; i < NumarStariFinaleAutMinim;i++)
        cout << StariFinaleAutomatMinimal[i] << " ";
    return 0;
}
