#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct node {
	
	char name[15];
	int count; 
	int state;
	struct node *adjacent[10];
	struct node *next;
	int closeness[10]; //contains closeness with all people
	int close;
	int betweenness[9][4];  //contains path indexes of going to other nodes
//	int c1; //availability rate in intermedia nodes
//	int c2; //availability rate in path
	
  };  typedef struct node node;
  
  	FILE *fptr;
	int count=0; // total people count
    node *people = (node*)malloc(sizeof(node));
    node *start= NULL;
    node *last= NULL;
	node *temp;
	node *iter =NULL;
	node *knot;
	char all[10][8];
	int controlbtw;
	int c1[10];		
	int c2[10];		
	char row[20]; 
	char row2[20]; 
	char row3[20]; 
	char row4[20]; 
			
	void insertPeople(char word[]);
  	int findSize_ins();
  	void fillMatrix(int matrix[][10]);
  	node * nameToNode(char word[15]);
	void komsuyazdir(node * temp);
	void insertAdjacent(node * source, char dest[15]);
	void printMatrix(int matrix [][10]);
	void degreeCentrality();
	node * fillBtwArray(node * temp, node * iter, node *knot);
	node * calculateCloseness_Btw(node * top);
	int controladj(node * temp, node * iter);
	int find(char * word ); // return index of all[][] from word
	void closenessCentrality();
	void betweennessCentrality();
	void printBetween(node *temp);
	void printbtw_STD();
	void printSecondMin(node * temp, node* target);

int main(){
	

	people -> next = NULL;
	strcpy(people -> name, "*");  //first inserting

	count = findSize_ins();
	
	int matrix[10][10] ;
	
	fillMatrix(matrix);
	
	printMatrix(matrix);
	
	degreeCentrality();
	closenessCentrality();
	betweennessCentrality();
	printbtw_STD();

	printf("\n\n\t***  OVER   ***");
	return 1;	
	
}	

int findSize_ins(){  //finding size and generate people node array

	char word[15] ;
 	fptr = fopen("./input.txt", "r");  

	while (!(feof(fptr))){  //for people
	
	fscanf(fptr, "%s ", &word);
 		
		 if(word[strlen(word)-1] == ';') {    //purification
	
			word[strlen(word)-1] =NULL;
			
			insertPeople(word);
		 	strcpy(all[count], word);			 
			 count++; 
		 	iter = nameToNode(word);
		 }
	} 
		    fclose(fptr);

 		fptr = fopen("./input.txt", "r");  

	while (!(feof(fptr))){  //for adjacent
	
	fscanf(fptr, "%s ", &word);
 		
		 if(word[strlen(word)-1] == ';') {    //purification

			word[strlen(word)-1] =NULL;
		 	iter = nameToNode(word);
		 }
		 
		 else{

		 	if(word[strlen(word)-1] == ',')   word[strlen(word)-1] =NULL;  //purification
		 	
		 	insertAdjacent(iter, word);
		 }
	}	
	        fclose(fptr);
	        
			return count;	
}

void fillMatrix(int matrix[][10]){

	for(int i=0; i<10; i++){  	  //All elements of the matrix will initially be zero
		for(int j=0; j<10; j++){
			matrix[i][j]=0;
		}
	}
		
	node * temp= start;
	node *iter;

	for(int i=0; i<count ;i++){ //for main person
			iter = start;
			
	      for(int j=0; j< count; j++){ //for adjacement
						
				for(int k=0; k< temp -> count ; k++){   //temp's adj array in struct 

						if(strcmp(temp -> adjacent[k] -> name, iter -> name ) ==0)
							matrix[i][j] = 1;
				}
							if(iter-> next != NULL)	
								 iter = iter -> next;
   	     }
					if(temp-> next != NULL)	 
			  		   temp = temp-> next;
	}	
}

void insertPeople(char word[]){
	
    node *current = (node*)malloc(sizeof(node));
	strcpy(current-> name , word);
	current -> next= NULL;
	
	if(start== NULL){
		start = current;
		last = current;
		current -> count=0;
	}
	 else{
	 	last ->  next =current;
	 	last = current;
	 			current -> count=0;
	 }
	
}

void insertAdjacent(node * source, char dest[15]){

	node * destination = nameToNode(dest);
	source -> adjacent[source -> count]= destination;
	source -> count++;

}

node * nameToNode(char word[15]){

	temp= start;
	while(strcmp(temp-> name, word)!=0){
	
		if(strcmp(temp-> name, word) == 0){

			return temp;
		}

			temp= temp -> next;
	}

	return temp;
}

void komsuyazdir(node * temp){
	
	for(int i=0; i<temp-> count; i++){
		printf("%s ", temp-> adjacent[i] -> name);
	}
}

void printMatrix(int matrix [][10]){
	
	node * temp= start;
	printf("\t");
	for(int i=0; i<10; i++){   //ustteki basliklar 
		printf("%s\t", temp -> name);
			temp= temp -> next;
	}
		temp= start;

		printf("\n");
		for(int i=0; i<10; i++){

				printf("%s\t", temp -> name); //soldaki isimler
				temp= temp -> next;

				for(int j=0; j<10; j++){   //0lar ve 1ler

					printf("%d \t", matrix[i][j]);
				}
				printf("\n");
		}
}

void degreeCentrality(){

	node * temp= start;
		printf("\n Node\tScore\tStandardized\n     \t     \t   Score\n ---------------------------");
		
		for(int i=0; i<count; i++){
			printf("\n %s\t  %d\t%7.2f", temp-> name, temp -> count, ((temp-> count)*1.0/(count-1)));
			temp =temp -> next;
		}	
}

void closenessCentrality(){
	
	for(int i=0; i<10; i++) nameToNode(all[i]) -> close =0;
	node * temp= start;
	
	for(int i=0; i<count; i++){  //added values of closeness[] of all nodes will be assign to close variable of current node
			calculateCloseness_Btw(	nameToNode(all[i]));
	
		for(int k=0; k<count ;k++ ){
		nameToNode(all[i])-> close += nameToNode(all[i])-> closeness[k];
		}
	}
	
		printf("\n\n\n Node\t  Score   Standardized\n     \t     \t     Score\n ---------------------------");
		
		for(int i=0; i<count; i++){
			printf("\n %s\t%7.3f\t  %7.2f", temp-> name, (1.0/(temp->close)) , (1.0/(temp->close)) * (count-1) );
			temp =temp -> next;
		}
}

void betweennessCentrality(){

	printf("\n\n\n Source\t    Target       Intermedia Nodes\t Path  \n ----------------------------------------------------------");
	controlbtw = 1;   //control implementations in calculateCloseness_Btw();
	node *temp = start;

for(int k=0; k<count; k++){ 
	calculateCloseness_Btw(temp);
	temp = temp -> next;
}
		return;
}

node * calculateCloseness_Btw(node * top){

	//FOR BETWEENEESS CALCULATION
	for(int k=0; k<count; k++){ //hepsini -1 yaptim
	temp = nameToNode(all[k]); 

		for(int i=0; i<count; i++){  //tek node icin

			for(int j=0 ; j<4; j++)  //tek index icin
				temp -> betweenness[i][j]= -1;
		}
	}
	
	//FOR CLOSENESS CALCULATION
	for(int i=0; i<10; i++) nameToNode(all[i]) -> state =0;

int adjs[6], adjs1[6]; //2. orderdan sonrakiler icin gerekli nodelarin all[][]'daki indexini tutacak bir array.

	for(int i=0; i< 6;i++) adjs[i]= -1;

top -> state=1;

for(int i=0; i<count; i++) top -> closeness[i]=0; 	//top node'umun closeness arrayini 0'la
			
	//all[]="cem, ayse, belma, edi, dundar, gamze, ferit, halit, ilke, jale";
	//        0    1      2     3     4       5      6      7      8    9
		
int a=0, i=0; //it represents
int adj=0; //adjs arrayi icin index tutar.
node * target= start, *current =start, *iter=start, *temp1, * temp2;

	while(a<9){  //top node umun closeness arrayini dolduracagim
		
		//LEVEL 1		
		for(int i=0; i< count; i++){  //adj ler icin 1 yaptim
			 if(controladj(top, nameToNode(all[i]) ) ){
			 	nameToNode(all[i]) -> state=1;
			 	top-> closeness[i]=1;
			 	a++;
			 }
		}

		//LEVEL 2
		for(int k=0; k< top-> count; k++){		//adjlerin adjleri icin 2
			current= top -> adjacent[k];	  //current= ayse;
		
			   for(int l=0; l< current -> count; l++){
						
					if(current-> adjacent[l]->state ==0 && a<9){
						iter = current-> adjacent[l]; 	   //iter=belma;
				    	top-> closeness[find(iter-> name)] =2;
				    	iter -> state=1;
				    	a++;
						    adjs[adj] = find(iter-> name);	
							adj++;	
					if(controlbtw ==1)	fillBtwArray(top, iter, current); //i called betweeneessCentrality func
					}
			  }
		}

		//LEVEL 3
		adj=0;
		for(int w=0; w<6; w++){
			
			if(adjs[w]==-1) break;
			  
			     iter= nameToNode(all[adjs[w]]); 		//iter=halit for cem
				
				for(int y=0; y< iter -> count; y++){
					if(iter-> adjacent[y]->state ==0 && a<9){
				
						current = iter-> adjacent[y]; 	   //current=Ilke;
				    	top-> closeness[find(current-> name)] =3;
				    	current -> state=1;
				    	a++;
						    adjs1[adj]= find(current-> name);	
							adj++;	
							adjs1[adj]=-1;
					
						if(controlbtw ==1){	 //i called betweeneessCentrality func
							for(int p=0; p<4; p++)
							 top-> betweenness[find(current-> name)][p] = top-> betweenness[find(iter-> name)][p];
							
						fillBtwArray(top, current, iter);
						}
					
					}
				}
		}

		//LEVEL 4
		adj=0;
		for(int w=0; w<6; w++){
			if(adjs1[w]==-1) break;
			  
			     current= nameToNode(all[adjs1[w]]); 		//current=Ilke for cem

				for(int y=0; y< current -> count; y++){
					if(current-> adjacent[y]->state ==0 && a<9){
				
						iter = current-> adjacent[y]; 	   //iter=Jale;
				    	top-> closeness[find(iter-> name)] =4;
				    	iter -> state=1;
				    	
						    adjs[adj]= find(iter-> name);	
							adj++;	
							adjs[adj]=-1;
		
			if(a!=9){  //betweeneess calculation
				
					if(controlbtw ==1)	 //i called betweeneessCentrality func
					{
						for(int p=0; p<4; p++)
						 top-> betweenness[find(iter-> name)][p] = top-> betweenness[find(current-> name)][p];
						fillBtwArray(top, iter, current);
					}
			}			
					a++;	
					}
				}
		}
	
		//LEVEL 5
		adj=0;
		for(int w=0; w<6; w++){
			
			if(adjs[w]==-1) break;
			  
			     iter= nameToNode(all[adjs[w]]); 		

				for(int y=0; y< iter -> count; y++){
					if(iter-> adjacent[y]->state ==0 && a<9){
				
						current = iter-> adjacent[y]; 	   
				    	top-> closeness[find(current-> name)] =5;
				    	iter -> state=1;
				    	
						    adjs1[adj]= find(current-> name);	
							adj++;	
							adjs1[adj]=-1;
						if(a!=9){  //betweeneess calculation
						
							if(controlbtw ==1)	 //i called betweeneessCentrality func
						{
							for(int p=0; p<4; p++)
							 top-> betweenness[find(current-> name)][p] = top-> betweenness[find(iter-> name)][p];
							fillBtwArray(top, current, iter);
						} //if controlbtw
							
						} //if a!=9
						a++;
					}
				}
		}

}  //while'in { }

		if(controlbtw ==1)	printBetween(top);  //just if i called betweeneessCentrality func, it will implement.
	return top;
}

int controladj (node * temp, node * iter){
	
		for(int i=0; i< temp-> count; i++) {  //top un adjacentlarina bakacak
					
			if(strcmp(iter -> name, temp -> adjacent[i] -> name ) == 0)  {
				return 1;
			}
		}
		return 0;
}

int find(char * word ){  // return index of all[][] from word

	for(int i=0; i<count; i++){
		if(strcmp(word, all[i] ) == 0)
			return i;
	}
	return 0;
}

node * fillBtwArray(node * temp, node * iter, node *knot){ //filling betweenness array's index of target with knot of temp 
	        //		  source       target    intermediate
	int index= find(iter -> name);

	for(int i=0; i<4; i++){

		if(temp -> betweenness[index][i] == -1){
			temp -> betweenness[index][i] = find(knot-> name);
			return temp;
		}
	}
}

void printBetween(node *temp){
	
char mediatePath[30]; //for path string
strcpy(mediatePath, "");
strcpy(row, "");

node * target, *iter;
int path=0, path2=0;
int cnt = count -find(temp -> name);
	
for(int i=0; i<cnt ; i++){ //target degisiyor
			
	target= nameToNode(all[find(temp -> name) +i]);

	if(strcmp(target -> name, temp -> name) ==0) continue; //kendisine olan pathi yazilmayacak
		
		if(strcmp(temp-> name, "Dundar") ==0)  //outputun duzenli durmasi icin
			printf("\n  %s     %s\t ", temp -> name, target -> name );

		else  printf("\n  %s\t     %s\t ", temp -> name, target -> name );
		
	if(controladj(temp, target)) {    //temp'in adj'lerinin intermediate node'u yok!
		printf("\t-   ");	
		printf("\t\t%s,%s\t ", temp -> name, target -> name );
		  continue;
	}
		
	else{
		for(int k=0; k< 4; k++){
			path = temp -> betweenness[find(target -> name)][k];
				strcat(mediatePath, all[path] ); 
			
			if(path!=-1) {
			strcat(mediatePath, ",");
			
			c1[path]++;//availability rate in intermedia nodes
		//	c2[path]++;//availability rate in intermedia nodes
			}					
			
			if( path != -1)  printf("%s", all[path]);	
				
			if(k!=3 && (temp -> betweenness[find(target -> name)][k+1]) != -1) printf(",");
		} //for each index control of betweenness[0][k]
		
		//for output regulation 
		if(strlen(mediatePath) <10) printf("\t\t\t%s,%s%s\t ", temp -> name, mediatePath, target -> name );
		else if(strlen(mediatePath) <15) printf("\t\t%s,%s%s\t ", temp -> name, mediatePath, target -> name );
		else  printf("\t%s,%s%s\t ", temp -> name, mediatePath, target -> name );
		
		int t=0;
			
		printSecondMin(temp,target);
		
	}  //else
	strcpy(mediatePath, "");
}  //target changing
	
}

void printbtw_STD(){

	node *temp= start; 
	int x=0;
	
	//!!!
		for(int i=0; i<10; i++) c2[i]=c2[i]+ c1[i]+9; //the total number of sources and targets of each node is equal to (n-1);

		printf("\n\n");
		for(int i=0; i<10; i++){
		
			if(c1[i]==0)	printf("\nCbetweenness (%s) = %d", temp -> name, c1[i]);
			else	printf("\nCbetweenness (%s) = %d/%d", temp -> name, c1[i], c2[i]);

		temp = temp -> next;	
		} 
			printf("\n\nAfter making standardization (n-1)(n-2)/2= %d\n", (count-1)*(count-2)/2 );

		temp =start;
		for(int i=0; i<10;i++){
			x=(c2[i])* ((count-1)*(count-2)/2);
			
			if(c1[i]==0)	printf("\nCbetweenness (%s) = %d", temp -> name, c1[i]);
			else	printf("\nCbetweenness (%s) = %d/%d=%7.4f", temp -> name, c1[i],x , c1[i]*1.0/ x );
		
			temp = temp -> next;
		} 	
		return;
}

void printSecondMin(node * temp, node* target){
	int control=0;
	node * knot, *knot2;
	int path= temp-> betweenness[find(target -> name)][0];
	for(int s=0; s<temp -> count; s++){	//for second min way
			
				knot =nameToNode(temp -> adjacent[s] -> name); //tek tek tempimin adjlerine bakacak
				
				if(find(knot -> name) == path) continue;
				else{
					
						if(controladj( knot, target))  { //target adjnin adjsi
							
						printf("\n  %s   \t%s\t\t%s " ,temp -> name, target -> name, knot-> name);
							
						printf("\t\t%s,%s,%s\t ", temp -> name, knot->name, target -> name );
		
						c2[find(temp-> name)]++;//availability rate in path
						c2[find(target-> name)]++;//availability rate in path
						c1[find(knot-> name)]++;//availability rate in int nodes

					if(find(temp-> name) ==4 && find(target-> name) ==7) {
						strcpy(row, knot->name );
						c1[find(knot-> name)]+=2;//availability rate in int nodes
				
								strcat(row,",");	strcat(row, target->name );	//for ilke and jale
							c1[find(target-> name)]+=2;//availability rate in int nodes

							}
						}
							
						else{  //target adjnin adjsi degil
						
							
							if( find(temp-> name) ==4 && (find(target-> name) ==8 || find(target-> name) ==9 )){  //for ilke and jale
								
							  if(control==0){
								printf("\n  %s \t%s\t%s" ,temp -> name, target -> name, row);
								printf("\t\t%s,%s,%s\t ", temp -> name, row, target -> name );
						
								c2[find(temp-> name)]++;//availability rate in path
								c2[find(target-> name)]++;//availability rate in path
							  //c1[find(knot-> name)]++;//availability rate in int nodes
							  }		
									
								if( find(target-> name) ==8 && control==0 ) {//for jale
									strcat(row,","); strcat(row, target->name );
											c1[find(target-> name)]++;//availability rate in int nodes
	
								}
									control++;
								
						continue;	}
							
							else if(find(temp->name)==2 && (find(target ->name) ==7 || find(target ->name) ==8|| find(target ->name) ==9) ){  //for belma second ways of halit, Ilke, Jale
								
								if(find(target ->name) == 7){ //for halit (belma is a temp)
									
									for(int h=0; h< knot->count; h++){
								knot2 = nameToNode(knot -> adjacent[h] -> name); //tek tek tempimin adjlerine bakacak	

								if(controladj(knot2, target)){
							
									if(find(knot->name) == 4 && find(knot2->name) == 5)	{
										strcpy(row2, knot-> name);strcat(row2, ", ");strcat(row2, knot2-> name);
											printf("\n  %s \t%s\t%s" ,temp -> name, target -> name, row2);
											printf("\t\t%s,%s,%s\t ", temp -> name, row2, target -> name );
											strcat(row2, ", ");strcat(row2, target-> name);
											c1[find(target-> name)]+=2;//availability rate in path
									}	
											
									else if(find(knot->name) == 4 && find(knot2->name) == 6){
										strcpy(row3, knot-> name);strcat(row3, ", ");strcat(row3, knot2-> name);
											printf("\n  %s \t%s\t%s" ,temp -> name, target -> name, row3);
											printf("\t\t%s,%s,%s\t ", temp -> name, row3, target -> name );
											strcat(row3, ", ");strcat(row3, target-> name);
											c1[find(target-> name)]+=2;//availability rate in path

									}	
									else {
										strcpy(row4, knot-> name);strcat(row4, ", ");strcat(row4, knot2-> name);
											printf("\n  %s \t%s\t%s" ,temp -> name, target -> name, row4);
											printf("\t\t%s,%s,%s\t ", temp -> name, row4, target -> name );
											strcat(row4, ", ");strcat(row4, target-> name);
											c1[find(target-> name)]+=2;//availability rate in path
	
									}		
							  				c2[find(temp-> name)]++;//availability rate in path
										c2[find(target-> name)]++;//availability rate in path
							  			c1[find(knot-> name)]+=3;//availability rate in int nodes
							  			c1[find(knot2-> name)]+=3;//availability rate in int nodes
									continue;
								}
							}
									
								}
								else {  //for ilke and jale (belma is a temp)
							
											if(find(knot->name) == 4){	//for dundar

												printf("\n  %s \t%s\t%s" ,temp -> name, knot2 -> name, row3);
												printf("\t\t%s,%s,%s\t ", temp -> name, row3, knot2 -> name );
						if( find(knot2->name) == 8)	{
							strcat(row3, ", "); strcat(row3, knot2-> name); 
							c1[find(knot2-> name)]+=2;//availability rate in int nodes
							c2[find(knot2-> name)]+=2;//availability rate in int nodes

						}	
							c2[find(temp-> name)]++;//availability rate in path
												
												printf("\n  %s \t%s\t%s" ,temp -> name, knot2 -> name, row2);
												printf("\t\t%s,%s,%s\t ", temp -> name, row2, knot2 -> name );
						if( find(knot2->name) == 8)	{
								strcat(row2, ", "); strcat(row2, knot2-> name);
									c1[find(knot2-> name)]++;//availability rate in int nodes
									c2[find(knot2-> name)]++;//availability rate in int nodes

						}	
							c2[find(temp-> name)]++;//availability rate in path
												
											}
											
											else{  //for edip
												printf("\n  %s \t%s\t%s" ,temp -> name, knot2 -> name, row4);
												printf("\t\t%s,%s,%s\t ", temp -> name, row4, knot2 -> name );
												strcat(row4, ", "); strcat(row4, knot2-> name);
													c2[find(temp-> name)]++;//availability rate in path
											}
								}
							}
							
							else	continue;	
							
						}
				} 
		}
}


