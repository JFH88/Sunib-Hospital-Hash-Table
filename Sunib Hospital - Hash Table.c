#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct medicine{
	char medID[25];
	char medName[55];
	char manufacturName[30];
	char regNum[12];
	int batchNum;
	struct medicine *next;
};

struct medicine *hashTable[1007];
const int tableSize = 1007;
int totalMedicine = 1;

void viewMedicine()
{
	int i, notNull = 0;
	
	for(i=0;i<tableSize;i++)
	{
		if(hashTable[i] != NULL)
		{
			int j = 0;
			if(notNull == 0)
			{
				notNull = 1;
				for(j=0;j<123;j++) printf("-");
				printf("\n");
				printf("| %-20s | %-30s | %-25s | %-12s | %-20s |\n", "Medicine ID", "Medicine Name", "Manufacturer Name", "Reg. No", "Batch/Lot Number");
				for(j=0;j<123;j++) printf("-");
				printf("\n");
			}
			
			struct medicine *ptr = hashTable[i];
			
			while(ptr != NULL)
			{
				printf("| %-20s | %-30s | %-25s | %-12s | %-20d |\n", ptr->medID, ptr->medName, ptr->manufacturName, ptr->regNum, ptr->batchNum);
				for(j=0;j<123;j++) printf("-");
				printf("\n");
				ptr = ptr->next;
			}
		}
	}
	
	if(notNull == 1)
	{
		printf("\n\n");
	}
	else
	{
		printf("There is no medicine(s) !\n");
	}
	printf("Press any key to continue . . .");
	getch();
}

char toUpperCase(char character)
{
	if(character >= 97 && character <= 122)
	{
		character -= 32;
	}
	return character;
}

int toAscii(char *str)
{
	int i=0, asciiSum=0;
	while(str[i] != '\0')
	{
		asciiSum += (int)str[i];
		i++;
	}
	
	return asciiSum;
}

void processInsert(struct medicine *newMed)
{
	char totalMedicineStr[10];
	char medIdTemp[25] = "";
	itoa(totalMedicine, totalMedicineStr, 10);
	int totalMedicineStrLen = strlen(totalMedicineStr);
	int i;
	
	strcat(medIdTemp, "M");
	
	for(i=1;i<=5-totalMedicineStrLen;i++)
	{
		strcat(medIdTemp, "0");
	}
		
	strcat(medIdTemp, totalMedicineStr);
	strcat(medIdTemp, "-");
	strcat(medIdTemp, newMed->regNum);
	strcat(medIdTemp, "-");
	medIdTemp[18] = toUpperCase(newMed->manufacturName[0]);
	medIdTemp[19] = toUpperCase(newMed->medName[0]);
	medIdTemp[20] = '\0';
	
	strcpy(newMed->medID, medIdTemp);

	int asciiSum = toAscii(medIdTemp);
	int key = asciiSum % tableSize;
	
	if(hashTable[key] == NULL)
	{
		newMed->next = NULL;
		hashTable[key] = newMed;
	}
	else
	{
		struct medicine *ptr = hashTable[key];
		while(ptr->next != NULL)
		{
			ptr = ptr->next;
		}
		newMed->next = NULL;
		ptr->next = newMed;
	}
	
	printf("index : %d\n\n", key);
	totalMedicine++;
}

int endWithLtd(char *str)
{
	int strlength = strlen(str);
	int i = strlength - 5;
	char ltdStr[5] = " Ltd.";
	int j = 0;
	
	for(i;i<strlength;i++,j++)
	{
		if(str[i] != ltdStr[j])
		{
			return 0;
		}
	}
	
	return 1;
}

int isUnique(char *str)
{
	int i;
	struct medicine *ptr = NULL;
	
	for(i=0;i<tableSize;i++)
	{
		if(hashTable[i] != NULL)
		{
			ptr = hashTable[i];
			while(ptr != NULL)
			{
				if(strcmp(ptr->medName, str) == 0)
				{
					return 0;
				}
				else
				{
					ptr = ptr->next;
				}
			}
		}
	}
	
	return 1;
}

int isNumeric(char *str)
{
	int i = 0;
	while(str[i] != '\0')
	{
		if(str[i] < 48 || str[i] > 57)
		{
			return 0;
		}
		i++;
	}
	return 1;
}

void insertMedicine()
{
	struct medicine *newMed = (struct medicine*)calloc(1, sizeof(struct medicine));
	int ltdOK = 0, uniqueName = 0, numericReg = 0;
	
	do{
		printf("Input Medicine Name[5-50][unique]: ");
		fflush(stdin);
		scanf("%[^\n]", newMed->medName);
		uniqueName = isUnique(newMed->medName);
	}while( (strlen(newMed->medName)<5 || strlen(newMed->medName)>50) || uniqueName == 0 );
	
	do{
		printf("Input Manufacturer Name[3-25][end with Ltd.]: ");
		fflush(stdin);
		scanf("%[^\n]", newMed->manufacturName);
		ltdOK = endWithLtd(newMed->manufacturName);
	}while( (strlen(newMed->manufacturName)<3 || strlen(newMed->manufacturName)>25) || ltdOK == 0 );
	
	do{
		printf("Input Registration No.[10 digits][numeric]: ");
		fflush(stdin);
		scanf("%s", newMed->regNum);
		numericReg = isNumeric(newMed->regNum);
	}while(strlen(newMed->regNum) != 10 || numericReg != 1);
	
	do{
		printf("Input Batch/Lot[>= 16]: ");
		fflush(stdin);
		scanf("%d", &newMed->batchNum);
	}while(newMed->batchNum < 16);
	
	processInsert(newMed);
	
	printf("Insert Success !\n");
	printf("Press any key to continue . . .");
	getch();
}

char confirmDeletion(struct medicine *ptr)
{
	char confirmDel;
	printf("\n\n");
	printf("Medicine ID : %s\n", ptr->medID);
	printf("Medicine Name : %s\n", ptr->medName);
	printf("Manufacturer Name : %s\n", ptr->manufacturName);
	printf("Registration No. : %s\n", ptr->regNum);
	printf("Batch/Lot : %d\n", ptr->batchNum);
	
	do{
		printf("Are you sure [y/n]? ");
		fflush(stdin);
		scanf("%c", &confirmDel);
	}while(confirmDel != 'y' && confirmDel != 'n');
	
	return confirmDel;
}

void removeMedicine()
{
	int dataFound = 0;
	char inputID[20];
	printf("Input medicine ID to delete: ");
	fflush(stdin);
	scanf("%s", &inputID);
	
	int asciiSum = toAscii(inputID);
	int keyToBeSearched = asciiSum % tableSize;
	struct medicine *ptr = hashTable[keyToBeSearched];
	struct medicine *ptrBef = NULL;
	
	if(ptr == NULL)
	{
		dataFound = 0;
	}
	else if(strcmp(ptr->medID, inputID)==0)
	{
		char confirmDel = confirmDeletion(ptr);
		dataFound = 1;
		if(confirmDel == 'y')
		{
			hashTable[keyToBeSearched] = hashTable[keyToBeSearched]->next;
		}
	}
	else
	{
		ptr = ptr->next;
		ptrBef->next = ptr;
		while(ptr != NULL)
		{
			if( strcmp(ptr->medID, inputID)==0 )
			{
				char confirmDel = confirmDeletion(ptr);
				dataFound = 1;
				
				if(confirmDel == 'y')
				{
						ptr = ptr->next;
						ptrBef->next = ptr;
				}
				break;
			}
			else
			{
				ptrBef = ptr;
				ptr = ptr->next;
			}
		}
	}
		
	if(dataFound == 0) printf("\nMedicine not found !\n");
	
	printf("Press any key to continue . . . ");
	getch();
}

void showMenu()
{
	printf("Sunib Hospital\n");
	printf("==============\n");
	printf("1. View Medicine\n");
	printf("2. Insert Medicine\n");
	printf("3. Remove Medicine\n");
	printf("4. Exit\n");
	printf(">> ");
}

int main()
{
	int i;
	for(i=0;i<1007;i++)
	{
		hashTable[i] = NULL;
	}
	
	int inputMenu;
	
	do{
		inputMenu = 0;
		do{
			system("cls");
			showMenu();
			fflush(stdin);
			scanf("%d", &inputMenu);
		}while(inputMenu < 1 || inputMenu > 4);
		
		switch(inputMenu)
		{
			case 1:
				viewMedicine();
				break;
			case 2:
				insertMedicine();
				break;
			case 3:
				removeMedicine();
		}
	}while(inputMenu != 4);
	
	return 0;
}
