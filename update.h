
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

char *file_name;



void update(char *file_name,char *fields,char *attributes,char *conditions)
{
  printf("%s\n",conditions);
  int field_index=getindex(fields,file_name);
  printf("%d\n",field_index);
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
        printf("d\n");
        
        char temp[]="";
        char *tmpAttributes = strdup(line_buf);
        char *attributeToken = strtok(tmpAttributes,",");

        //compares it with all the attributes of the file/table
        while(attributeToken != NULL)
        {
          printf("del %s\n", attributeToken);
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
    printf("counter: %d",counter);
    for(int i=0;i<updateIndex;i++)
    {
      fprintf(fp1_temp,"%s",temp1[i]);
    }

    fclose(fp1_temp); 
}