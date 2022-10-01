#include<stdio.h>
#include<conio.h>
#include<string.h>
#include<stdlib.h>


struct contact 
{
	double ph;
	char name[20],add[20];
}list;

char name[20];
FILE *fp, *ft;
int i,n,ch,l,found;

int main()
{
int i,n,ch,l,found;

do
{
	system("cls");    //pulisco tutto e ristampo dopo
	printf("\n\t\t=========================================================\n\t\t\xB3\t\t  RUBRICA \t TELEFONICA  \t\t\xB3\n\t\t=========================================================");
	printf("\n\n\n\t\t\t\tMenu principale\n\t\t\t\t======================");
	printf("\n\t\t\t\t[1] Nuovo contatto\n\t\t\t\t[2] Lista contatti\n\t\t\t\t[3] Modifica contatto\n\t\t\t\t[4] Cancella contatto\n\t\t\t\t[0] Esci");
	printf("\n\t\t\t\t======================\n\t\t");
	//
	printf("\n\t\t\t\tSeleziona una scelta:");
	scanf("%d",&ch);
	

	switch(ch) // partono le casistiche
	{
		case 0: //ultimo 
		system("cls");
		printf("\n\n\t\tBYE BYE");
		break;
		
		
		/* *********************Nuovo contatto************  */
		case 1:
		system("cls");
		printf("\n\t\t=========================================================\n\t\t\xB3\t\t  NUOVO \t CONTATTO  \t\t\xB3\n\t\t=========================================================");
		fp=fopen("contact.dll","a");
		for (;;)
		{ 
			fflush(stdin);
			printf("\n\t\t[Per uscire premere spazio e invio dopo il nome]");
			printf("\n\n\t\tNome:");
			scanf("%[^\n]",&list.name);
			if(stricmp(list.name," ")==0) //cosi ritorno al menu 
			break;
			fflush(stdin);
			printf("\n\t\tNumero:");
			scanf("%lu",&list.ph);
			fflush(stdin);
			printf("\n\t\tIndirizzo:");
			scanf("%[^\n]",&list.add);
			fflush(stdin);
			fwrite(&list,sizeof(list),1,fp); //annoto quante ce ne sono
		}
		fclose(fp);	
		break;
		
		
		/* *********************lista dei contatti*************************  */
		case 2:
		system("cls");
		printf("\n\t\t=========================================================\n\t\t\xB3\t\t  LISTA \t CONTATTI  \t\t\xB3\n\t\t=========================================================");
		printf("\n\t\t\t\tNome\tNumero\tIndirizzo\n\t\t=========================================================\n");
		
		for(i=97;i<=122;i=i+1)
		{
			fp=fopen("contact.dll","r");
			fflush(stdin);
			found=0;
			while(fread(&list,sizeof(list),1,fp)==1) //legge in binari
			{
				if(list.name[0]==i || list.name[0]==i-32)
				{
					printf("\n\t\tNome: %s\n\t\tNumero: %lu\n\t\tIndirizzo: %s \n\t\t=========================================================",list.name,list.ph,list.add); 				//si sovrappongono???
					found++;
				}
			}
			if(found!=0)
			{
			    printf("\n\t\t [%c]-(%d)\n\n",i-32,found);
			    getch();
			}
			fclose(fp);
		}
		break;
		
		
		/* *********************modifica contatti************************/
		case 3:
		system("cls");
		fp=fopen("contact.dll","r");
		ft=fopen("temp.dat","w");
		fflush(stdin);
			printf("\n\t\t=========================================================\n\t\t\xB3\t\t  MODIFICA \t CONTATTO  \t\t\xB3\n\t\t=========================================================");
		printf("\n\n\t\t Contatto modificare:");
		scanf("%[^\n]",name);
		while(fread(&list,sizeof(list),1,fp)==1)
		{
			if(stricmp(name,list.name)!=0)  //verifico 
			{
				fwrite(&list,sizeof(list),1,ft);	
			}
		}
		fflush(stdin);
		system("cls");
		printf("\n\n\t\t\t\tModifica di '%s'\n",name);
		printf("\n\t\t\t\tNome: ");
		scanf("%[^\n]",&list.name);
		fflush(stdin);
		printf("\n\t\t\t\tNumero:"); //boh? non va
		scanf("%lu",&list.ph);
		fflush(stdin);
		printf("\n\t\t\t\tIndirizzo:");
		scanf("%[^\n]",&list.add);
		fflush(stdin);
		printf("\n");
		fwrite(&list,sizeof(list),1,ft);
		fclose(fp);
		fclose(ft);
		remove("contact.dll");
		rename("temp.dat","contact.dll");
		break;
		
		
		/* ********************cancello**********************/
		case 4:
		system("cls");
		fflush(stdin);
		printf("\n\t\t=========================================================\n\t\t\xB3\t\t  ELIMINA \t CONTATTO  \t\t\xB3\n\t\t=========================================================");
		printf("\n\n\t\t\t\tEliminazione dell'account ");
		scanf("%[^\n]",&name);
		fp=fopen("contact.dll","r");
		ft=fopen("temp.dat","w");
		while(fread(&list,sizeof(list),1,fp)!=0)
		if (stricmp(name,list.name)!=0)
		{
			fwrite(&list,sizeof(list),1,ft);
			fclose(fp);
			fclose(ft);
			remove("contact.dll");
			rename("temp.dat","contact.dll");	
		}
		break;
	}
}
while (ch != 0);
return 0;
}

