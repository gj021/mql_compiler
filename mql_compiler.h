#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

#define MAX_RECORDS 100
#define MAX_RECORD_SIZE 100
char *file_name;




//gives index of a given field
int getindex(char *field,char *file_name)
{
    if(strcmp("EMP.txt",file_name)==0)
    {
      if(strcmp("eid",field)==0)
      {
        return 0;
      }
      else if(strcmp("ename",field)==0)
      {
        return 1;
      }
      else if(strcmp("eage",field)==0)
      {
        return 2;
      }
      else if(strcmp("eaddress",field)==0)
      {
        return 3;
      }
      else if(strcmp("salary",field)==0)
      {
        return 4;
      }
      else if(strcmp("#deptno",field)==0)
      {
        return 5;
      }
      else
      {
        return -1;
      }
    }
    else
    {
      if(strcmp("dnum",field)==0)
      {
        return 0;
      }
      else if(strcmp("dname",field)==0)
      {
        return 1;
      }
      else if(strcmp("dlocation",field)==0)
      {
        return 2;
      }
      else 
      {
        return -1;
      }
    }
    
}

int result(char *token,char *value,char *comparator,int index)
{
  if(index==0)
  {
    int value1=atoi(token);
    int value2=atoi(value);
    if(!strcmp(comparator,">="))
      return value1>=value2;
    else if(!strcmp(comparator,"!="))
      return value1!=value2;
    else if(!strcmp(comparator,"<="))
      return value1<=value2;
    else if(!strcmp(comparator,">"))
      return value1>value2;
    else if(!strcmp(comparator,"<"))
      return value1<value2;
    else
      return value1==value2;
  }
  else
  {
    int temp=strcmp(token,value);
    if(temp==0)
      return 1;
    return 0;
  }
}

int compare(int field_index,char *token,char *comparator,char *value)
{
  int result_value;
  
  if(field_index!=1 && field_index!=3)
  {
    result_value=result(token,value,comparator,0);
  }
  else
  {
    result_value=result(token,value,comparator,1);
  }
  return result_value;
}

void check(char *conditions,char *file_name,int *checkArray)
{
  char *line_buf = NULL;
  size_t line_buf_size = 0;
  int line_count = 0;
  ssize_t line_size;
  if(access(file_name,F_OK) == -1)
  {
    printf("FILE DOES NOT EXIST\n");
    return;
  }
  //start for conditions
  char *conditiontoken = strtok(conditions,",");
    
  int count=0;

  char *conditions_array[100];

  while(conditiontoken != NULL)
  {
    conditions_array[count]=conditiontoken;
    conditiontoken = strtok(NULL, ",");
    count++;
  }

  conditions_array[count]="*";
  
  int index=0;

  FILE *fp = fopen(file_name, "r");
  
  int lines=0;

  while(!feof(fp))
  {
  char ch = fgetc(fp);
    if(ch == '\n')
    {
      lines++;
    }
  }

  fclose(fp);

  for(int i=0;i<lines;i++)
    {
      checkArray[i]=0;
    }

  int next=0;

  while(index<=count/4)
  {
    int index_count=4*index;
    int field_index=getindex(conditions_array[index_count],file_name);
    // printf("%s %s %s %s\n",conditions_array[index_count],conditions_array[index_count+1],conditions_array[index_count+2],conditions_array[index_count+3]);
    index++;
    if(field_index==-1)
    {
      printf("Wrong Column Field Given");
      break;
    }

    fp= fopen(file_name, "r");

    /* Get the first line of the file. */
    line_size = getline(&line_buf, &line_buf_size, fp);
    line_count=0;
    /* Loop through until we are done with the file. */
    while (line_size >= 0)
    {
      int file_index_temp=0;
      if(line_count>0)
      {
        // printf("%s",line_buf);
        char *tmpAttributes = strdup(line_buf);
        char *attributeToken = strtok(tmpAttributes,",");

        //compares it with all the attributes of the file/table
        while(attributeToken != NULL && attributeToken!="\n")
        {
          if(file_index_temp==field_index)
          {
            if(index_count==0)
            {
              checkArray[line_count-1]=compare(field_index,attributeToken,conditions_array[index_count+1],conditions_array[index_count+2]);
            }
            else if(next==1 && checkArray[line_count-1]==1)
            {
              checkArray[line_count-1]=compare(field_index,attributeToken,conditions_array[index_count+1],conditions_array[index_count+2]);
            }
            else if(next==0 && checkArray[line_count-1]==0)
            {
              checkArray[line_count-1]=compare(field_index,attributeToken,conditions_array[index_count+1],conditions_array[index_count+2]);
            }
            break;
          }
          
          file_index_temp++;
          attributeToken = strtok(NULL,",");
        }
        
      }
      /* Get the next line */
      line_size = getline(&line_buf, &line_buf_size, fp);
      /* Increment our line count */
      line_count++;
    }
    if(!strcmp(conditions_array[index_count+3],"and") || !strcmp(conditions_array[index_count+3],"AND"))
          {
              next=1;
          }
          else if(!strcmp(conditions_array[index_count+3],"or") || !strcmp(conditions_array[index_count+3],"OR"))
          {
              next=0;
          }
          else
          {
              next=-1;
          }
    /* Free sthe allocated line buffer */
    free(line_buf);
    line_buf = NULL;

    /* Close the file now that we are done with it */
    fclose(fp);
    //we have conditions array here
  }
}


//get function
void get(char *fields, char *file, char *conditions)
{
  int flag=1;
  int fields_index[6];
  printf("%s\n", fields);
  for(int i=0;i<6;i++)
  {
    fields_index[i]=-1;
  }

  if(fields[0] == '*')
  {
    for(int i=0;i<6;i++)
    {
      fields_index[i]=1;
    }
    flag=0;
  }
  else
  {
    
    char *fieldToken = strtok(fields,",");

    //compares it with all the attributes of the file/table
    while(fieldToken != NULL)
    {
      if(getindex(fieldToken,file)==-1)
      {
        printf("Wrong Fields Given");
        flag=0;
        break;
      }
      fields_index[getindex(fieldToken,file)]=1;
      fieldToken = strtok(NULL,",");
    }
  }
  FILE *fp = fopen(file_name, "r");
  char *line_buf = NULL;
  file_name = file;
  size_t line_buf_size = 0;
  int line_count = 0;
  ssize_t line_size;
  
  fp= fopen(file_name, "r");
  

  int lines=0;

  while(!feof(fp))
  {
  char ch = fgetc(fp);
    if(ch == '\n')
    {
      lines++;
    }
  }

  fclose(fp);

  fp= fopen(file_name, "r");

  int checkArray[lines];
  
  if(strlen(conditions)!=0)
    check(conditions,file_name,checkArray);
  else
  {
    for(int i=0;i<lines;i++)
    {
      checkArray[i]=1;
    }  
  }
  
  

    /* Get the first line of the file. */
    line_size = getline(&line_buf, &line_buf_size, fp);
    line_count=0;
    /* Loop through until we are done with the file. */
    while (line_size >= 0)
    {
      int file_index_temp=0;
      if(line_count>0 && checkArray[line_count-1]==1)
      {
        // printf("%s",line_buf);
        char *tmpAttributes = strdup(line_buf);
        char *attributeToken = strtok(tmpAttributes,",");
        //compares it with all the attributes of the file/table
        int index=0;
        while(attributeToken != NULL)
        {
          if(fields_index[index]==1)
            printf("%s ",attributeToken);
          attributeToken = strtok(NULL,",");
          index++;
        }
        printf("\n");
      }
      /* Get the next line */
      line_size = getline(&line_buf, &line_buf_size, fp);
      /* Increment our line count */
      line_count++;
    }
  fclose(fp);

}

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
      printf("FOREIGN KEY CONFLICT\n");
      return 0;
    }
    return 1;
  }
  
}

//return 1 if no conflict of foreign key exists
int checkForeignKey(char *file_attributes, char *attributes)
{
  int numComma = 0;
  int flag = 0;
  //check for hash character
  //if hash is not there return 1
  for(int i = 0; i < strlen(file_attributes); i++)
  {
    if(file_attributes[i] == '#')
      {
        flag = 1;
        break;
      }
    if(file_attributes[i] == ',')
      numComma++; 
  }
  if(flag == 0)
    return 1;
  //else return 1 only if the to-be inserted string(attributes) has that value in dept.txt
  
  //getting deptno from to be inserted record
  char *tmpAttributes = strdup(attributes);
  char *deptno = strtok(tmpAttributes,",");
  int index = 0;
  while(deptno != NULL)
  {
    if(numComma <= index)
    {
      break;
    }
    index++;
    deptno = strtok(NULL,",");
  }
  printf("deptno %s\n", deptno);

  //open dept.txt files
  FILE *dfp;
  dfp = fopen("DEPT.txt", "r");
  char *line_buf;
  size_t line_buf_size = 0;
  ssize_t line_size;
  line_size = getline(&line_buf,&line_buf_size,dfp);
  //checking for dnum == deptno
  //while loop gets dnum one by one
  while(line_size > 0)
  {
    line_size = getline(&line_buf,&line_buf_size,dfp);
    char *dept_token = strtok(line_buf,",");
    
    printf("dept %s\n",dept_token);
    if(!strcmp(dept_token,deptno))
    {
      return 1;
    }

  }
  printf("FOREIGN KEY NOT PRESENT\n");
  return 0;
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
  if(!checkForeignKey(file_attributes,attributes))
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
  
  // fseek(fptr,0,SEEK_END);
  if(checkInsert(file_attributes,attributes))
  {
    fprintf(fp,"\n");
    fprintf(fp,attributes);
  }
  else
  {
    printf("INVALID SYNTAX\n");
  }
  fclose(fp);
  return;
}

//eg:gets *eid int and returns eid
char *getFieldfromToken(char *str, char *tmp)
{
  int index = 0;
  tmp = strdup("");
  // printf("field from token %s %s", result, str);
  int resultIndex = 0;
  if(str[index] == '*')
  {
    index++;
  }
  else
  {
    tmp[resultIndex++] = str[index++];
  }
  for(int i = index; i < strlen(str); i++)
  {
    if(str[i] != ' ')
    {
      tmp[resultIndex++] = str[i];
    }
    else
    {
      printf("plz %s\n", tmp);
      return tmp;
    } 
  }
}

//returns an array containing indexes of fields provided
int *getFieldIndex(char *fields)
{
  int *arr = (int *)malloc(sizeof(int)*commaCount(fields));
  FILE *fp;
  int arrIndex = 0;
  fp = fopen(file_name,"r");
  char *line_buf;
  size_t line_buf_size = 0;
  ssize_t line_size;
  line_size = getline(&line_buf,&line_buf_size,fp);

  char *token = strtok(fields,",");
  
  //takes one field at a time
  while(token != NULL)
  {
    printf("tken %s\n",token);
    int index = 0;
    char *tmpAttributes = strdup(line_buf);
    char *attributeToken = strtok(tmpAttributes,",");
    
    //compares it with all the attributes of the file/table
    while(attributeToken != NULL)
    {
      char *tmp;
      tmp = getFieldfromToken(attributeToken,tmp);
      printf("check tmp %s %s\n", tmp, token);
      if(!strcmp(tmp,token))
      {
        arr[arrIndex] = index;
        printf("h %d %d %s\n",arrIndex,index,tmp);
        break;
      }
      index++;
      attributeToken = strtok(NULL,",");
    }

    //if a field in not present in the file
    printf("%d %d %s\n", arr[arrIndex-1],arrIndex,line_buf);
    if(attributeToken == NULL)
    {
      
      arr[0] = -1;
    }
    token = strtok(NULL,",");
  }

  return arr;
  
}

//delete function
void delete(char *file, char *conditions)
{
  char table[100][100];
  char *condition_copy = strdup(conditions);
  FILE *fptr;
  file_name = file;
  int no_of_records = 0;
  fptr = fopen(file_name,"r");
  int i = 0;

  FILE *fp = fopen(file_name, "r");
  char *line_buf = NULL;
  file_name = file;
  size_t line_buf_size = 0;
  int line_count = 0;
  ssize_t line_size;
  
  fp= fopen(file_name, "r");
  

  int lines=0;

  while(!feof(fp))
  {
  char ch = fgetc(fp);
    if(ch == '\n')
    {
      lines++;
    }
  }

  fclose(fp);

  fp= fopen(file_name, "r");

  int checkArray[lines];

  if(strlen(conditions)!=0)
    check(conditions,file_name,checkArray);
  else
  {
    for(int i=0;i<lines;i++)
    {
      checkArray[i]=1;
    }  
  }
  

  

  
  while(fgets(table[i],100,fptr))
  {
    table[i][strlen(table[i])-1] = '\0';
    printf("%s\n",table[i]);
    i++;
  }
  no_of_records = i;
  fclose(fptr);
  fptr = fopen(file_name,"w");
  i = 0;
  fprintf(fptr,table[i]);
  fprintf(fptr,"\n");
  i++;
  //copy table in emp.txt
  while(i < no_of_records)
  {
    if(checkArray[i-1] == 0)
    {
      fprintf(fptr,table[i]);
      fprintf(fptr,"\n");

    }
    else
    {
      char *token = strtok(table[i],",");
      char condi[100] = "";
      strcat(condi,"#deptno");
      strcat(condi,",=,");
      strcat(condi,token);
      printf("table %s\n",condition_copy);
      if(!strcmp(file_name,"DEPT.txt"))
      {
        delete("EMP.txt",condi);
      }
    }
    
    i++;
  }
  fclose(fptr);
}

void update(char *file_name,char *fields,char *attributes,char *conditions)
{
  if(!strcmp(fields,"dnum"))
  {
    printf("UPDATE OF PRIMARY KEY NOT ALLOWED\n");
    return;
  }
  if(!strcmp(fields,"eid"))
  {
    printf("UPDATE OF PRIMARY KEY NOT ALLOWED\n");
    return;
  }
  printf("%s\n",conditions);
  int field_index=getindex(fields,file_name);
  printf("%d\n",field_index);
  if((field_index==0 || field_index==2 || field_index==4 || field_index==5)&& !strcmp(file_name,"EMP.txt"))
  {
    if(attributes[0]=='"')
    {
      printf("Type Error\n");
      return;
    }
  }
  else if(!strcmp(file_name,"EMP.txt") && (field_index==1 || field_index==3))
  {
    if(attributes[0]!='"')
    {
      printf("Type Error\n");
      return;
    }
  }
  else if(!strcmp(file_name,"DEPT.txt") && (field_index==1 || field_index==2))
  {
    if(attributes[0]!='"')
    {
      printf("Type Error\n");
      return;
    }
  }
  char *line_buf = NULL;
  size_t line_buf_size = 0;
  int line_count = 0;
  ssize_t line_size;
  FILE *filep;
  filep = fopen(file_name, "r");
  line_size = getline(&line_buf,&line_buf_size,filep);
  char *first_line = strdup(line_buf);
  fclose(filep);
  int counter = 0;
  int updateIndex = 0;
  if(access(file_name,F_OK) == -1)
  {
    printf("FILE DOES NOT EXIST\n");
    return;
  }
  char *conditiontoken = strtok(conditions,",");
    
  int count=0;

  char *conditions_array[100];

  while(conditiontoken != NULL)
  {
    conditions_array[count]=conditiontoken;
    conditiontoken = strtok(NULL, ",");
    count++;
  }

  conditions_array[count]="*";
  
  int index=0;

  FILE *fp = fopen(file_name, "r");
  
  int lines=0;

  while(!feof(fp))
  {
  char ch = fgetc(fp);
    if(ch == '\n')
    {
      lines++;
    }
  }

  fclose(fp);

  int checkArray[lines];

  for(int i=0;i<lines;i++)
    {
      checkArray[i]=0;
    }

  int next=0;

  while(index<=count/4)
  {
    int index_count=4*index;
    int field_index=getindex(conditions_array[index_count],file_name);
    // printf("%s %s %s %s\n",conditions_array[index_count],conditions_array[index_count+1],conditions_array[index_count+2],conditions_array[index_count+3]);
    index++;
    if(field_index==-1)
    {
      printf("Wrong Column Field Given");
      break;
    }

    fp= fopen(file_name, "r");

    /* Get the first line of the file. */
    line_size = getline(&line_buf, &line_buf_size, fp);
    line_count=0;
    /* Loop through until we are done with the file. */
    while (line_size >= 0)
    {
      int file_index_temp=0;
      if(line_count>0)
      {
        // printf("%s",line_buf);
        char *tmpAttributes = strdup(line_buf);
        char *attributeToken = strtok(tmpAttributes,",");

        //compares it with all the attributes of the file/table
        while(attributeToken != NULL && attributeToken!="\n")
        {
          if(file_index_temp==field_index)
          {
            if(index_count==0)
            {
              checkArray[line_count-1]=compare(field_index,attributeToken,conditions_array[index_count+1],conditions_array[index_count+2]);
            }
            else if(next==1 && checkArray[line_count-1]==1)
            {
              checkArray[line_count-1]=compare(field_index,attributeToken,conditions_array[index_count+1],conditions_array[index_count+2]);
            }
            else if(next==0 && checkArray[line_count-1]==0)
            {
              checkArray[line_count-1]=compare(field_index,attributeToken,conditions_array[index_count+1],conditions_array[index_count+2]);
            }
            break;
          }
          
          file_index_temp++;
          attributeToken = strtok(NULL,",");
        }
        
      }
      /* Get the next line */
      line_size = getline(&line_buf, &line_buf_size, fp);
      /* Increment our line count */
      line_count++;
    }
    if(!strcmp(conditions_array[index_count+3],"and"))
          {
              next=1;
          }
          else if(!strcmp(conditions_array[index_count+3],"or"))
          {
              next=0;
          }
          else
          {
              next=-1;
          }
    /* Free sthe allocated line buffer */
    free(line_buf);
    line_buf = NULL;

    /* Close the file now that we are done with it */
    fclose(fp);
  }
  for(int i=0;i<lines;i++)
  {
    printf("%d\n",checkArray[i]);
  }
  int ccount=0;
    if(!strcmp(file_name,"EMP.txt"))
    {
      ccount=5;
    }
    else
    {
      ccount=2;
    }
    
    char temp1[100][100];
    fp= fopen(file_name, "r");
    
    /* Get the first line of the file. */
    line_size = getline(&line_buf, &line_buf_size, fp);
    line_count=0;
    /* Loop through until we are done with the file. */
    while (line_size >= 0)
    {
      int file_index_temp=0;
      
      if(line_count>0)
      {
        // printf("d\n");
        
        char temp[]="";
        char *tmpAttributes = strdup(line_buf);
        char *attributeToken = strtok(tmpAttributes,",");

        //compares it with all the attributes of the file/table
        while(attributeToken != NULL)
        {
          // printf("del %s\n", attributeToken);
          if(file_index_temp==field_index && checkArray[line_count-1]==1)
          {
            strcat(temp,attributes);
            strcat(temp,",");
            // printf("%s",attributes); 
          }
          else if(file_index_temp<ccount)
          {
            strcat(temp,attributeToken);
            strcat(temp,",");
            // printf("%s",attributeToken);
          }
          else if(file_index_temp==ccount)
          {
            strcat(temp,attributeToken);
          }
          
          
          file_index_temp++;
          attributeToken = strtok(NULL,",");
        }
        // printf("%s\n",temp);
        strcpy(temp1[counter++],temp);
        updateIndex++;
      }
    else
    {
       char *tmpAttributes = strdup(line_buf);
      strcpy(temp1[counter++],tmpAttributes);
      updateIndex++;
    }
    printf("c:%d",counter);
      
      /* Get the next line */
    line_size = getline(&line_buf, &line_buf_size, fp);
      /* Increment our line count */
    line_count++;
    }
    /* Free sthe allocated line buffer */
    free(line_buf);
    line_buf = NULL;

    /* Close the file now that we are done with it */
    fclose(fp);
    
    // printf("%s\n",temp1[0]);
    FILE *fp1_temp = fopen(file_name, "w");
    // fprintf(fp1_temp,first_line);
    printf("counter: %s",first_line);
    for(int i=0;i<updateIndex;i++)
    {
      fprintf(fp1_temp,"%s",temp1[i]);
    }
    

    fclose(fp1_temp); 
}

