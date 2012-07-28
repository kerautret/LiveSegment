


#include "algorithm.h"









AntiPodalH determineAPH(Node* cvhull, AntiPodalH ah, Point2D M)
{
  

  
  //Initialisation
  //Mbot
  Node* MbotCV=getNext(cvhull);
  Point2D Mbot=getContent(*MbotCV);
  Point2DinCV pMbot = Point2DinCV(Mbot);
  pMbot.setPos(MbotCV);

  
    //Mtop
  Node* MtopCV=getLast(cvhull);
  Point2D Mtop=getContent(*MtopCV);
  Point2DinCV pMtop = Point2DinCV(Mtop);
  pMtop.setPos(MtopCV);
  
	
  //M
  Node* MCV=cvhull;
  Point2DinCV ptrM=Point2DinCV(M);
  ptrM.setPos(MCV);



  //Determination les paramettres
  long remain=ah.getEdge().remain(M);
  long remainVertex=ah.getEdge().remain(ah.getVertex());
  long remainEgde= ah.getEdge().remain(ah.getEdge().getBegin());
  long a=ah.getEdge().getA();
//  long b=ah.getEdge().getB();
  long yM=M.getY();
  long yVertex=ah.getVertex().getY();
  long yBegin=ah.getEdge().getBegin().getY();
  long yEnd=ah.getEdge().getEnd().getY();
  long yMbot=Mbot.getY();
  long yMtop=Mtop.getY();
  Point2DinCV pBegin=ah.getEdge().getBegin();
  Point2DinCV pEnd=ah.getEdge().getEnd();
  Point2DinCV pVertex;
  Point2DinCV pEdge;
  
  if(ah.getEdge().remain(pMbot)==remainVertex) 
    pVertex=pMbot;
  else if (ah.getEdge().remain(pMtop)==remainVertex) 
    pVertex=pMtop;
  else 
    pVertex=ah.getVertex();
  
  if(ah.getEdge().remain(pMbot)==remainEgde) pEdge=pMbot;
  else if(ah.getEdge().remain(pMtop)==remainEgde) 
    pEdge=pMtop; 
  else 
    pEdge=pBegin;
  

#ifdef DEBUG
  cout<<"Les parametres de APH"<<endl;
  cout<<"a "<<a<<" b"<<b<<endl;
  cout<<"remain "<<remain<<" remainVertex "<<remainVertex<<" remainEgde"<<remainEgde<<endl;
#endif


	
  //Si M se trouve entre 2 droites englobantes
  if((remain-remainVertex)*(remain-remainEgde)<0)  
    return ah;	
  
  
  //Si M se trouve sur 2 droites englobantes
  if(remain==remainEgde)
    {
      Line line=ah.getEdge();
      if((yM-yBegin)*(yM-yEnd)<0){
	//cout<<"New added point is between 2 leaning points on leaning line"<<endl;
	return ah;}
      line=Line(ptrM, pEdge);
      ah.setEdge(line);
      return ah;
    }
  if(remain==remainVertex)
    {
      Point2DinCV nVertex;
      Line line;
      
      //Si M n'est pas le point de depart dans l'enveloppe convexe.
      Point2D M1=getContent(*ptrM.getPos());
      if((M1.getX()!=M.getX())||(M1.getY()!=M.getY())) return ah;
      
      if((yM-yBegin)*(yM-yEnd)<=0) ah.setVertex(ptrM);
      else
	{
	  line=Line(ptrM,pVertex);
	  if((yBegin-yM)*(yBegin-yEnd)<=0) nVertex=ah.getEdge().getBegin();
	  if((yEnd-yM)*(yEnd-yBegin)<=0) nVertex=ah.getEdge().getEnd();
	  ah=AntiPodalH(nVertex,line);
	}
      return ah;
    }
  
	
  //=========================================================================================
  // Cas particulier
  //  yM=yZ
  //=========================================================================================
	
	if(yM==yVertex)
	  {

	  // Si le point ajoute est le nouveau sommet du paire antipodal
		if((remainVertex-remain)*(remainVertex-remainEgde)<0)
		{				
			ah.setVertex(ptrM);
			return ah;
		}
		// Si non, on cherche le nouveau paire antipodal
		else
		{
		  
		  if(yMtop!=yVertex)	
		    {
			Node* iterVPre= getLast(ah.getVertex().getPos());
			Point2DinCV pVPre=Point2DinCV(getContent(*iterVPre));
			pVPre.setPos(iterVPre);

			Line lVVPre=Line(ah.getVertex(),pVPre);		
			Line lMtopM=Line(pMtop,ptrM);
			if(angle2Line(lVVPre,lMtopM)>0) 
				return AntiPodalH(ah.getVertex(),lMtopM);
			else  return AntiPodalH(ptrM,lVVPre);
			}
			else
			{
			Node* iterVPre= getNext(ah.getVertex().getPos());
			Point2DinCV pVPre=Point2DinCV(getContent(*iterVPre));
			pVPre.setPos(iterVPre);
			Line lVVPre=Line(ah.getVertex(),pVPre);
			
			Line lMbotM=Line(pMbot,ptrM);
			if(angle2Line(lVVPre,lMbotM)<0) 
			       return AntiPodalH(ah.getVertex(),lMbotM);	
			else   return AntiPodalH(ptrM,lVVPre);
			}
			return ah;
		}
	}
	

	//==============================================================================================
	// Cas general
	//==============================================================================================
	Node* iterC;
	Node* iterCsuivant;
	Node* iterCprecedent;
	Node* iterCC;		
	AntiPodalH apH;	
	Direction dr;
	long yC,yCC;
	//Determination la direction de travail
	if(a!=0)
	{
	  if((remain>remainEgde)&&(remain>remainVertex)&&(yM>yVertex)) dr=LEFT;
	  if((remain>remainEgde)&&(remain>remainVertex)&&(yM<yVertex)) dr=RIGHT;
	  if((remain<remainEgde)&&(remain<remainVertex)&&(yM>yVertex)) dr=RIGHT;
	  if((remain<remainEgde)&&(remain<remainVertex)&&(yM<yVertex)) dr=LEFT;
	}
	
	if(a==0)
	  {
	    cout<<"Erreur: la droite horizontale est parallele avec Ox"<<endl;
	    exit(1);
	  }
	
	
	if(dr==RIGHT)
	{
		if((remainEgde-remain)*(remainEgde-remainVertex)<0) iterC=pVertex.getPos();
		if((remainVertex-remainEgde)*(remainVertex-remain)<0) 
		{
			if(pBegin.isNext(pEnd))iterC=pBegin.getPos();
			else iterC=pEnd.getPos();
		}
		yC=getContent(*iterC).getY();			
		iterCC=getLast(iterC);	
				
		Line lMbotM=Line(Mbot,M);	
		iterCsuivant=getNext(iterC);			
		Line lCCsuivant=Line(getContent(*iterC),getContent(*iterCsuivant));
		double angle=angle2Line(lCCsuivant,lMbotM);
      		#ifdef DEBUG
		cout<<"lMbotM"<<lMbotM;
		cout<<"lCC'"<<lCCsuivant;
		cout<<"angle "<<angle<<endl;
        	#endif
		
		
	while(angle>0)
		  {
		    iterC=iterCsuivant;
		    iterCC=getLast(iterC);	
		    iterCsuivant=getNext(iterC);
			
		    //Test iterC
		    yC=getContent(*iterC).getY();
		    yCC=getContent(*iterCC).getY();
		    if((yM-yC)*(yM-yCC)<=0)break;
		    
		    //Calcul pour ape suivant			
		    lCCsuivant=Line(Point2DinCV(iterC),Point2DinCV(iterCsuivant));
		    angle=angle2Line(lCCsuivant,lMbotM);
		  }
                #ifdef DEBUG
		cout<<"APH iterC after loop in right direction";
		printNode(iterC);
		printNode(iterCC);
		cout<<"yC "<<yC<<" yM "<<yM<<" yMbot "<<yMbot<<endl;
    		#endif		
		if(yC==yM)
		{
			Line lC=Line(Point2DinCV(iterC),Point2DinCV(iterCsuivant));
			Line lM=Line(Point2DinCV(MbotCV),ptrM);
			angle=angle2Line(lC,lM);
			if(angle<=0) apH=AntiPodalH(Point2DinCV(iterC),lM);
			else apH=AntiPodalH(ptrM,lC);
			return apH;
		}
		else
		{
			if((yC-yM)*(yC-yMbot)<=0){apH=AntiPodalH(Point2DinCV(iterC),Line(Point2DinCV(MbotCV),ptrM));return apH;}
//			if((yC-yM)*(yC-yMbot)>=0)
			else
			  {apH=AntiPodalH(Point2DinCV(ptrM),Line(Point2DinCV(iterCC),Point2DinCV(iterC)));return apH;}
		}

	}
	else
	{
	  if((remainEgde-remain)*(remainEgde-remainVertex)<0) iterC=pVertex.getPos();
	  if((remainVertex-remainEgde)*(remainVertex-remain)<0) 
	    {
	      if(pBegin.isLast(pEnd))iterC=pBegin.getPos();
	      else iterC=pEnd.getPos();
	    }
		yC=getContent(*iterC).getY();
		iterCC=getNext(iterC);
		Line lMtopM=Line(Mtop,M);
		iterCprecedent=getLast(iterC);
		Line lCCprecedent=Line(getContent(*iterC),getContent(*iterCprecedent));
		double angle=angle2Line(lCCprecedent,lMtopM);
     		#ifdef DEBUG
		cout<<"APH iter C in left direction"<<endl;
		printNode(iterC);
		cout<<"lMtopM"<<lMtopM;
		cout<<"lCC'"<<lCCprecedent;
		cout<<"angle "<<angle<<endl;
	       	#endif
		while(angle<0)
		{		
			iterC=iterCprecedent;	
			iterCC=getNext(iterC);
			iterCprecedent=getLast(iterC);
			
			//Test iterC
			yC=getContent(*iterC).getY();			
			yCC=getContent(*iterCC).getY();			
			if((yM-yC)*(yM-yCC)<=0)break;
			
			//Calcul pour l'etape suivante			
			lCCprecedent=Line(getContent(*iterC),getContent(*iterCprecedent));
			angle=angle2Line(lCCprecedent,lMtopM);
		}
#ifdef DEBUG
		cout<<"APH iterC after left loop";
		printNode(iterC);		
		cout<<"yC "<<yC<<"yCC "<<yCC<<" yM "<<yM<<" yMtop "<<yMtop<<endl;
   		#endif 
		if(yC==yM)
		{
			Line lC=Line(Point2DinCV(iterC),Point2DinCV(iterCprecedent));
			Line lM=Line(Point2DinCV(MtopCV),ptrM);
			angle=angle2Line(lC,lM);
			if(angle>=0) apH=AntiPodalH(Point2DinCV(iterC),lM); 
			else apH=AntiPodalH(ptrM,lC);
			return apH;
		}
		else
		{
			if((yC-yMtop)*(yC-yM)<=0) {apH=AntiPodalH(Point2DinCV(iterC),Line(Point2DinCV(MtopCV),ptrM));return apH;}
//			if((yC-yMtop)*(yC-yM)>=0)
			else
			  {apH=AntiPodalH(ptrM,Line(Point2DinCV(iterCC),Point2DinCV(iterC)));return apH;}
		}
		
		
	}	
}













AntiPodalV determineAPV(Node* cvhull, AntiPodalV av, Point2D M)
{


  
  
  //Point2D v2=av.getVertex();
  // Point2D p3 = av.getEdge().getBegin();
  //Point2D p4 = av.getEdge().getEnd();
	
  //Vérification que le point ajouté est différent d'un des sommets de l'antipodal:
  //     if(	   (M.getX()==v2.getX() && M.getY()==v2.getY())||
  //	   (M.getX()==p3.getX() && M.getY()==p3.getY())||
  //	   (M.getX()==p4.getX() && M.getY()==p4.getY())){
  
  //    return av;
  //   }
	

	//Initialisation les points plus droit, et plus gauche du point M	
	//Mbot
	Node* MbotCV=getNext(cvhull);
	Point2D Mbot=getContent(*MbotCV);
	Point2DinCV pMbot = Point2DinCV(Mbot);
	pMbot.setPos(MbotCV);
	
	//Mtop
	Node* MtopCV=getLast(cvhull);
	Point2D Mtop=getContent(*MtopCV);
	Point2DinCV pMtop = Point2DinCV(Mtop);
	pMtop.setPos(MtopCV);
	
	//M
	Node* MCV=cvhull;
	Point2DinCV ptrM=Point2DinCV(M);
	ptrM.setPos(MCV);
	
		
	//Determination les remains du point M, des sommets du paire antipodal
	long remain=av.getEdge().remain(M);
	long remainVertex= av.getEdge().remain(av.getVertex());
	long remainEgde= av.getEdge().remain(av.getEdge().getBegin());
	long xM=M.getX();
	long xMbot=Mbot.getX();
	long xMtop=Mtop.getX();
	long xVertex=av.getVertex().getX();
	long xBegin=av.getEdge().getBegin().getX();
	long xEnd=av.getEdge().getEnd().getX();
	long a=av.getEdge().getA();
	long b=av.getEdge().getB();
	Point2DinCV pBegin=av.getEdge().getBegin();
 	Point2DinCV pEnd=av.getEdge().getEnd();
	Point2DinCV pVertex;
	Point2DinCV pEdge;
	
	if(av.getEdge().remain(pMbot)==remainVertex) pVertex=pMbot;
	else if (av.getEdge().remain(pMtop)==remainVertex) pVertex=pMtop; else pVertex=av.getVertex();
	
	if(av.getEdge().remain(pMbot)==remainEgde) pEdge=pMbot;
	else if(av.getEdge().remain(pMtop)==remainEgde) pEdge=pMtop; else pEdge=pBegin;
	
	
   	#ifdef DEBUG 
	cout<<"Les parametres de APV "<<endl;
	cout<<"remain"<<remain<< "remainVertex " <<remainVertex<< " remainEdge "<<remainEgde<<endl;
	cout<<"a= "<<a<<" b="<<b<<endl;
	printCirculator(ptrM.getPos());
   	#endif	

	
	

	
	//Si M se trouve entre 2 droites englobantes
	if((remain-remainVertex)*(remain-remainEgde)<0)  return av;
			
	//Si M se trouve sur 2 droites englobantes
	if(remain==remainEgde)
	{	
		Line line=av.getEdge();
		if((xM-xBegin)*(xM-xEnd)<=0){
		  //cout<<"New added point is between 2 leaning points on leaning line"<<endl; 
		  return av;}
		line=Line(ptrM, pEdge);
		av.setEdge(line);
		return av;
	}
	if(remain==remainVertex)
	{
		Line line;
		Point2DinCV nVertex;
		
		//Si M n'est pas le point de depart dans l'enveloppe convexe.
		Point2D M1=getContent(*ptrM.getPos());
		if((M1.getX()!=M.getX())||(M1.getY()!=M.getY())) return av;
		
		if((xM-xBegin)*(xM-xEnd)<=0) av.setVertex(ptrM);
		else
		{
			line=Line(ptrM,pVertex);
			if((xBegin-xM)*(xBegin-xEnd)<=0) nVertex=av.getEdge().getBegin();
			if((xEnd-xM)*(xEnd-xBegin)<=0)nVertex=av.getEdge().getEnd();
			av=AntiPodalV(nVertex,line);
		}
		return av;
	}
	
	//=========================================================================================
	// Cas particulier
	//  xM=xZ
	//=========================================================================================
	if(xM==xVertex)
	{
		// Si le point ajoute est le nouveau sommet du paire antipodal
		if((remainVertex-remain)*(remainVertex-remainEgde)<0)
		{				
			av.setVertex(ptrM);
			return av;
		}
		// Si non, on cherche le nouveau paire antipodal
		else
		{
			if(xM!=xMtop)
			{
			Node* iterVPre= getLast(av.getVertex().getPos());
 			Point2DinCV pVPre=Point2DinCV(getContent(*iterVPre));
			pVPre.setPos(iterVPre);
			Line lVVPre=Line(av.getVertex(),pVPre);	
			Line lMtopM=Line(pMtop,ptrM);
			if(angle2Line(lVVPre,lMtopM)>0)
				 return AntiPodalV(av.getVertex(),lMtopM);
			else return AntiPodalV(ptrM,lVVPre);
			}
			else
			{
			Node* iterVPre= getNext(av.getVertex().getPos());
 			Point2DinCV pVPre=Point2DinCV(getContent(*iterVPre));
			pVPre.setPos(iterVPre);
			Line lVVPre=Line(av.getVertex(),pVPre);
			Line lMbotM=Line(pMbot,ptrM);
			if(angle2Line(lVVPre,lMbotM)<0)
				 return AntiPodalV(av.getVertex(),lMbotM);
			else   return AntiPodalV(ptrM,lVVPre);
			}
			return av;
		}
	}

	
	//==============================================================================
	//Cas general
	//==============================================================================
	
	//Determination de la direction
	
	Node* iterC;
	Node* iterCsuivant;
	Node* iterCprecedent;
	Node* iterCC;
	AntiPodalV apV;	
	Direction dr;	
	
	long xC,xCC;
	
	if((b>=0)||(a==0))
	{
		if(((remain>remainEgde)&&(remain>remainVertex))&&(xM<xVertex)) dr=RIGHT;		
		if(((remain>remainEgde)&&(remain>remainVertex))&&(xM>xVertex)) dr=LEFT;		
		if(((remain<remainEgde)&&(remain<remainVertex))&&(xM<xVertex)) dr=LEFT;			
		if(((remain<remainEgde)&&(remain<remainVertex))&&(xM>xVertex)) dr=RIGHT;		
	}
	if((b<0)&&(a>0))
	{
		if(((remain>remainEgde)&&(remain>remainVertex))&&(xM<xVertex)) dr= LEFT;		
		if(((remain>remainEgde)&&(remain>remainVertex))&&(xM>xVertex)) dr= RIGHT;		
		if(((remain<remainEgde)&&(remain<remainVertex))&&(xM<xVertex)) dr= RIGHT;		
		if(((remain<remainEgde)&&(remain<remainVertex))&&(xM>xVertex)) dr= LEFT;			
	}
	
	if(b==0)
	{
		cout<<"Erreur: la droite verticale est paralelle avec Oy"<<endl;
		exit(1);
	}	
	
	
	
	//Traitement sur le direction positive
	bool isBreak=false;
	if(dr==RIGHT)	
	{
		if((remainEgde-remain)*(remainEgde-remainVertex)<0) iterC=pVertex.getPos();
		if((remainVertex-remain)*(remainVertex-remainEgde)<0) 
		{
			if(pBegin.isNext(pEnd)) iterC=pBegin.getPos();
			else iterC=pEnd.getPos();			
		}
		xC=getContent(*iterC).getX();
		iterCC=getLast(iterC);
  		#ifdef DEBUG
		cout<<"APV iter C start in right direction"<<endl;
		printNode(iterC);	
  		#endif
		Line lMbotM=Line(Mbot,M);
		iterCsuivant=getNext(iterC);
		Line lCCsuivant=Line(Point2DinCV(iterC),Point2DinCV(iterCsuivant));
		double angle=angle2Line(lCCsuivant,lMbotM);
		while(angle>0)
		{
			iterC=iterCsuivant;
			iterCC=getLast(iterC);
			iterCsuivant=getNext(iterC);
			
			//Test iterC
			xC=getContent(*iterC).getX();
			xCC=getContent(*iterCC).getX();
			if((xM-xC)*(xM-xCC)<=0){ isBreak=true; break;}
			
			//Calcul pour l'etape suivante			
			lCCsuivant=Line(Point2DinCV(iterC),Point2DinCV(iterCsuivant));
			angle=angle2Line(lCCsuivant,lMbotM);
		}
    	    	#ifdef DEBUG
		cout<<"APV iter C after right loop"<<endl;
		printNode(iterC);	
		cout<<"lMbotM"<<lMbotM;
		cout<<"xC "<<xC<<endl;
    	    	#endif
					
		if (xC==xM)
		{
		       	Line lC=Line(Point2DinCV(iterC),Point2DinCV(iterCsuivant));
			Line lM=Line(Point2DinCV(MbotCV),ptrM);
			angle=angle2Line(lC,lM);
			if(angle<0)apV=AntiPodalV(Point2DinCV(iterC),lM);
			else apV=AntiPodalV(ptrM,lC);
			return apV;
		}	
		else
		{
			//MgM, C - arrete sommet de nouvelle antipodal
			if((xC-xM)*(xC-xMbot)<=0) {apV=AntiPodalV(Point2DinCV(iterC), Line(Point2DinCV(MbotCV),ptrM)); return apV;}			
			//iterCC iterC, M arret sommet de nouvelle antipodal
//			if((xC-xM)*(xC-xMbot)>=0)
                        else
			{apV=AntiPodalV(ptrM,Line(Point2DinCV(iterCC),Point2DinCV(iterC)));return apV;}
		}
	
	}
	else
	{
		if((remainEgde-remain)*(remainEgde-remainVertex)<0) iterC=pVertex.getPos();
		if((remainVertex-remain)*(remainVertex-remainEgde)<0) 
		{
			if(pBegin.isLast(pEnd)) iterC=pBegin.getPos();
			else iterC=pEnd.getPos();
		}
		xC=getContent(*iterC).getX();
		iterCC=getNext(iterC);
  		#ifdef DEBUG
		cout<<"APV iter C start in left direction"<<endl;
		printNode(iterC);			
  		#endif
		Line lMtopM=Line(Mtop,M);	
		iterCprecedent=getLast(iterC);
		Line lCCprecedent=Line(getContent(*iterC),getContent(*iterCprecedent));
  		double angle=angle2Line(lCCprecedent,lMtopM);
		while(angle<0)
		{
			iterC=iterCprecedent;
			iterCC=getNext(iterC);
			iterCprecedent=getLast(iterC);
			
			//Test iterC
			xC=getContent(*iterC).getX();
			xCC=getContent(*iterCC).getX();
 			if((xM-xC)*(xM-xCC)<=0) {isBreak=true; break;}
			
			//Calcul pour l'etape suivante		
			Line lCCprecedent=Line(getContent(*iterC),getContent(*iterCprecedent));
			angle=angle2Line(lCCprecedent,lMtopM);
		}
  		#ifdef DEBUG
		cout<<"APV iter C after left loop"<<endl;
		printNode(iterC);	
		cout<<"lMtopM: "<<lMtopM;
		cout<<"xC "<<xC<<"xMtop "<<xMtop<<"xM "<<xM<<endl;
  		#endif			
		if (xC==xM)
		{
	        	Line lC=Line(Point2DinCV(iterC),Point2DinCV(iterCprecedent));
			Line lM=Line(Point2DinCV(MtopCV),ptrM);			
			angle=angle2Line(lC,lM);
			if (angle>=0) apV=AntiPodalV(Point2DinCV(iterC),lM); else apV=AntiPodalV(ptrM,lC);
			return apV;
		}
		else
		{

			if((xC-xMtop)*(xC-xM)<=0)
			{apV=AntiPodalV(Point2DinCV(iterC), Line(Point2DinCV(MtopCV),ptrM)); return apV;}
//			if((xC-xMtop)*(xC-xM)>=0)
			else
			  {apV=AntiPodalV(ptrM,Line(Point2DinCV(iterCC),Point2DinCV(iterC))); return apV;}
		}
		
	}
	
}
