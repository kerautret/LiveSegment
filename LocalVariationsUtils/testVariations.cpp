#include <cmath>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>


#include "Arguments.h"
#include "localvariations.h"
#include <QtGui>
#include <stdlib.h>
#include <iostream>


using namespace std;

using namespace ImaGene;



int  retTaille(string nomFichier);
double * extraitChamp(string nomFichier, int indice);





static Arguments args;


int
remain( int argc, char** argv ) 
{
  args.addOption("-import", "-import <fichier.dat> epsilon isClosed:1:0 ", "0", "O.0001", "0");
  

  


  if ( ( argc <= 1 ) 
       || ! args.readArguments( argc, argv ) ||
       (!args.check("-import"))){
    cerr << args.usage( "testVariations", 
			" test detection zones min/max locales",
			"-import" ) << endl;
  }
  
  string nomFichier = " ";
  
  if(args.check("-import")){
    nomFichier = args.getOption( "-import" )->getValue(0);
    double epsilon = args.getOption( "-import" )->getFloatValue(1);
    bool isclosed = ((args.getOption( "-import" )->getIntValue(2))==1)? true: false;
    char file[nomFichier.size()];
    strcpy( file, nomFichier.c_str() );
    int tailleTab = retTaille(nomFichier);
    double * tabValues = extraitChamp(nomFichier, 2);
    
    vector<double> vectVal ;
    for(int i=0; i< tailleTab; i++){
      vectVal.push_back(tabValues[i]);
      
    }
    
    fstream streamValMin;
    streamValMin.open("min.txt", ios::out);
    vector<int> vectMin = LocalVariations::getLocalMinimaIndices(vectVal, epsilon, isclosed);

    vector<int>::iterator itMin = vectMin.begin();    
    while(itMin!=vectMin.end()){
      int p = *itMin;
      streamValMin << p << " " << tabValues[p] << endl;
      ++itMin;
    }
    fstream streamValMax;
    streamValMax.open("max.txt", ios::out);
    vector<int> vectMax = LocalVariations::getLocalMaximaIndices(vectVal, epsilon, isclosed);
    
    vector<int>::iterator itMax = vectMax.begin();    
    while(itMax!=vectMax.end()){
      int p = *itMax;
      streamValMax << p << " " << tabValues[p] << endl;
      ++itMax;
    }
    streamValMin <<endl;
    streamValMax <<endl;


  }


  return 0;

}










double *
extraitChamp(string nomFichier, int indice){
  int taille = retTaille(nomFichier);
  double* tabResult = new double[taille];
  char file[nomFichier.size()];
  strcpy(file, nomFichier.c_str() );
  fstream flux;
  flux.open (file, ios::in);

  
  string str;
  getline(flux, str );
  int position=0;
  while ( flux.good() )
    {
      if ( ( str != "" ) 
	   && ( str[ 0 ] != '#' ) )
	{
	  istringstream in_str( str );
	  uint idx = 1;
	  double val;
	  while ( in_str.good() )
	    {
	      in_str >> val;
	      if ( indice == idx ){
		tabResult[position]=val;
		position++;
		break;
		
	      }
	      ++idx;
	    }
	}
      getline( flux, str );
      
    }
  return tabResult;
}





int 
retTaille(string nomFichier){
  char file[nomFichier.size()];
  strcpy(file, nomFichier.c_str() );
  fstream fluxx;
  fluxx.open (file, ios::in);
  int nb=0;
  string s;
  while(true){
    getline(fluxx,s);
    if(!fluxx.good())
      break;
    if(s[0]=='#')
      continue;
    else if(s=="")
      continue;
    nb++;
  }
  return nb;
}
