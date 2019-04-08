#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

char *file_name;

//returns the number of commas in given string
int commaCount(char *str)
{
  int count = 0;
  int l = strlen(str);
  for(int i = 0; i < l; i++)
  {
    if(str[i] == ',')
      count++;
  }
  return count;
}

//checks if all datatypes are same at that of the table for insert fn
int checkDatatype(char *file_attributes, char *attributes,int numComma)
{
  int filePtr = 0;
  int attPtr = 0;
  int lenAtt = strlen(attributes);
  while(numComma > -1)
  {
    while(file_attributes[filePtr] != ' ')
    {
      filePtr++;
    }
    filePtr++;
    if(file_attributes[filePtr] == 'i' && attributes[attPtr] == '"')
    {
      printf("why1\n");
      return 0;
    }
    else if(file_attributes[filePtr] == 's' && attributes[attPtr] != '"')
    {
      printf("why2 %c\n", attributes[attPtr]);
      return 0;
    }
    while(attributes[attPtr] != ',' && attPtr < lenAtt)
    {
      attPtr++;
    }
    attPtr++;
    numComma--;
  }
  
  return 1;
}

//used for insert
//checks if a entry is there in given string
//returns 0 if entry exists
int checkEntry(char *attributes, int commaCount)
{
  FILE *fp;
  fp = fopen(file_name,"r");

}

//checks for primary key for insert fn
int checkPrimarykey(char *file_attributes, char *attributes)
{
  int fileAttlen = strlen(file_attributes);
  int flag = 0;
  int commaCount = 0;
  for(int i = 0; i < fileAttlen; i++)
  {
    if(file_attributes[fileAttlen] == '*')
    {
      flag = 1;
    }
    if(flag == 0 && file_attributes[fileAttlen] == ',')
    {
      commaCount++;
    }
  } 
  if(flag == 0)
    return 1;
  else
  {
    if(!checkEntry(attributes,commaCount))
    {
      return 0;
    }
    return 1;
  }
  
}

//checks if insert statement has correct semantics
int checkInsert(char *file_attributes, char *attributes)
{
  int numComma;
  //checking number of attributes
  if(commaCount(file_attributes) != commaCount(attributes))
    return 0;
  else
    numComma = commaCount(file_attributes);
  if(!checkDatatype(file_attributes,attributes,numComma))
    return 0;
  if(!checkPrimarykey(file_attributes,attributes))
    return 0;
}

void insert(char *attributes, char *file)
{
  FILE *fptr;
  FILE *fp;
  file_name = file;
  if(access(file_name,F_OK) == -1)
  {
    printf("FILE DOES NOT EXISTS\n");
    return;
  }
  fptr = fopen(file_name,"r");
  char file_attributes[100];
  fgets(file_attributes,100,fptr);
  fclose(fptr);
  fp = fopen(file_name,"a");
  printf("%s",file_attributes);
  fseek(fptr,0,SEEK_END);
  if(checkInsert(file_attributes,attributes))
  {
    fprintf(fptr,"\n");
    fprintf(fptr,attributes);
  }
  else
  {
    printf("INVALID SYNTAX\n");
  }
}