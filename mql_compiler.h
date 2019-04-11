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
int *checkEntry(char *attributes, int commaCount)
{
  char *line_buf = NULL;
  size_t line_buf_size = 0;
  int line_count = 0;
  ssize_t line_size;
  char *tmpAttributes = strdup(attributes);
  char *attributetoken = strtok(tmpAttributes,",");
  int index = 0;
  while(attributetoken != NULL)
  {
    if(commaCount <= index)
      break;
    index++;
    attributetoken = strtok(NULL, ",");
  }
  printf("attribute token: %s\n", attributetoken);
  FILE *fp = fopen(file_name, "r");
  int *arr = (int *)malloc(sizeof(int)*100);
  arr[0] = 0;
  /* Get the first line of the file. */
  line_size = getline(&line_buf, &line_buf_size, fp);

  /* Loop through until we are done with the file. */
  while (line_size >= 0)
  {
    /* Show the line details */
    char *primaryKey;
    index = 0;
    if(line_count>0)
    {
      char* token = strtok(line_buf, ","); 
      while (token != NULL) 
      { 
        if(commaCount <= index)
          break;
        index++;
        token = strtok(NULL, ","); 
      }
      if(!strcmp(token,attributetoken))
      {
        printf("%s %s\n",token,attributetoken);
        arr[0]++;
        arr[arr[0]] = line_count;
      }

    }
    /* Get the next line */
    line_size = getline(&line_buf, &line_buf_size, fp);
    /* Increment our line count */
    line_count++;
  }

  /* Free the allocated line buffer */
  free(line_buf);
  line_buf = NULL;

  /* Close the file now that we are done with it */
  fclose(fp);

  return arr;
}

//checks for primary key for insert fn
//return 1 if primary key conflict is not there
int checkPrimarykey(char *file_attributes, char *attributes)
{
  int fileAttlen = strlen(file_attributes);
  int flag = 0;
  int commaCount = 0;
  for(int i = 0; i < fileAttlen; i++)
  {
    if(file_attributes[i] == '*')
    {
      flag = 1;
      break;
    }
    if(flag == 0 && file_attributes[i] == ',')
    {
      commaCount++;
    }
  } 
  if(flag == 0)
    return 1;
  else
  {
    int *arr = checkEntry(attributes,commaCount);
    int arrSize = arr[0];
    if(arrSize != 0)
    {
      return 0;
    }
    return 1;
  }
  
}

//checks if insert statement has correct semantics
//returns 1 is insert is valid
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
  return 1;
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