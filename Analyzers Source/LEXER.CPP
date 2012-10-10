#include<stdio.h>
#include<string.h>
#include<ctype.h>




//GLOBAL DECLARATIONS
 FILE *myFile;
 char inputFile[100];
 char word[100];
 int pos = 0;
 int iRec = 0;
 int iUnrec = 0;
 int iLn = 1;

void returnItAll()
{
 for(int i = pos-1; i >= 0; i--)
    {
    ungetc(word[i],myFile);
    word[i] = '\0';
    }
 pos = 0;
}

void remove()
{
 pos = 0;
 for(int i = 0; word[i] != '\0'; i++)
 {
  word[i] = '\0';
 }

}

void recognized(char *word, char token[20])
{
 FILE*outFile;

 outFile = fopen(inputFile, "a");
 if(outFile == NULL)
 {
  printf("Cannot create %s\n", inputFile);
 }

 fprintf(outFile, "%d      %s      %s\n", iLn, token, word);
 fclose(outFile);
 pos = 0;

 for(int i = 0; word[i] != '\0'; i++)
 {
  word[i] = '\0';
 }

}

int isTerminated(char c)
{
 if(c == '\n')
  iLn++;

 if(c == '~' || c == ')'||c == '('||c == '<' || c == '>' || c == '=' || c == '!' || c == '+' || c == '-' || c == '/' || c == '*' || c == '^' || c == '%' || c == ';' || c == ':' || c == '{' || c == '}' || c == '|' || c == '&' || c == '?' || c == '\'' || c == '\n' || c == ' ' || c == '\t'||c == ',')
   return 1;
 else
   return 0;
}


void addletter(char c)
{
 word[pos] = c;
 pos++;
}

int isComment(char red)
{
 int stateNum = 0;
 do
 {
   if(red == '\n')
   {
    iLn++;
    if(stateNum == 2)
     return 1;
    else
     return 0;
   }
  addletter(red);
  switch(stateNum)
  {
   case 0:
   {
    if(red == '/')
     stateNum = 1;
    else
     return 0;
    break;
   }
   case 1:
   {
    if(red == '/')
     stateNum = 2;
    else
     return 0;
    break;
   }
  }
 }while((red = fgetc(myFile)) != EOF);
 return 1;
}

int isIdentifier(char red)
{
 int stateNum = 0;
 while((red = fgetc(myFile)) != EOF)
 {
  if(isTerminated(red))
  {
   if(red == '\n')
    iLn--;
   ungetc(red,myFile);
   if(red == '\'' || red == '=')
    return 0;
   return 1;
  }
  addletter(red);
  switch(stateNum)
  {
   case 0:
   {
    if(isalpha(red) || red == '_')
     stateNum = 1;
    else
     return 0;
    break;
   }
   case 1:
   {
    if(isalnum(red) || red == '_')
     stateNum = 1;
    else
     return 0;
    break;
   }
  }

 }//while((red=fgetc(myFile)) != EOF);
 return 1;
}

char *isNumber(char red)
{
 int stateNum = 0;
 while((red=fgetc(myFile)) != EOF)
 {
  /*/if(red == '-' || red == '+')
  {
   if(stateNum == 0)
   ;
   else
   {
   ungetc(red, myFile);
   break;
   }
  }
  else //*/ if(isTerminated(red))
  {
   ungetc(red,myFile);
   if(red == '\n')
   iLn--;
   if(red == '\'' || red == '=')
    return "ERR";
   break;
  }
  addletter(red);
  switch(stateNum)
  {
   case -2:
   {
    if(isdigit(red))
     stateNum = 2;
    else
     return "ERR";
    break;
   }
   case -1:
   {
    if(isdigit(red))
     stateNum = 1;
    else if(red == '.')
     stateNum = 2;
    else
     return "ERR";
    break;
   }
   case 0:
   {
    if(isdigit(red))
     stateNum = 1;
    else if(red == '.')
     stateNum = -2;
    else if(red == '-' || red == '+')
     stateNum = -1;
    else
     return "ERR";

    break;
   }
   case 1:
   {
    if(isdigit(red))
     stateNum = 1;
    else if(red == '.')
     stateNum = 2;
    else
     return "ERR";
    break;
   }
   case 2:
   {
    if(isdigit(red))
     stateNum = 2;
    else
     return "ERR";
   }
  }
 }//while((red=fgetc(myFile)) != EOF);

  if(stateNum == 1)
   return "INTEGER";
  if(stateNum == 2)
   return "FLOAT";
  else
   return "ERR";
}

int isString(char red)
{
 int stateNum = 0;
 while((red = fgetc(myFile)) != EOF)
 {
  if(red == '\n')
  {
   if(stateNum == 1)
    return 2;
   else
    return 0;
  }
  addletter(red);
  switch(stateNum)
  {
   case 0:
   {
    if(red == '\'')
     stateNum = 1;
    else
     return 0;
    break;
   }
   case 1:
   {
    if(red == '\'')
     return 1;
    break;
   }
  }
 }
 return 0;
}

char *isKeyword(char red)
{
int stateNum = 0;
while((red = fgetc(myFile)) != EOF)
{
 if(isTerminated(red))
 {
  if(red == '\n')
  iLn--;
  ungetc(red,myFile);
  break;
 }

 addletter(red);

 switch(stateNum)
 {
  case 0:
  {
   if(red == 'a')
    stateNum = 1;
   else if(red == 'b')
    stateNum = 7;
   else if(red == 'c')
    stateNum = 11;
   else if(red == 'd')
    stateNum = 15;
   else if(red == 'e')
    stateNum = 18;
   else if(red == 'f')
    stateNum = 21;
   else if(red == 'i')
    stateNum = 26;
   else if(red == 'l')
    stateNum = 31;
   else if(red == 'n')
    stateNum = 34;
   else if(red == 'p')
    stateNum = 38;
   else if(red == 'r')
    stateNum = 47;
   else if(red == 's')
    stateNum = 53;
   else if(red == 't')
    stateNum = 56;
   else
    return "ERR";
   break;
  }

  case 1:
  {
  if(red == 'd')
   stateNum = 2;
  else if(red == 's')
   stateNum = 4;
  else if(red == 'r')
   stateNum = 201;
  else
   return "ERR";
  break;
  }

  case 2:
  {
   if(red == 'd')
    stateNum = 3;
   else
    return "ERR";
   break;
  }

  case 3:
  {
    return "ERR";
  }

  case 4:
  {
   return "ERR";
  }

  case 201:
  {
   if(red == 'r')
    stateNum = 5;
   else
    return "ERR";
   break;
  }

  case 5:
  {
   if(red == 'y')
    stateNum = 6;
   else
    return "ERR";
   break;
  }

  case 6:
   return "ERR";

  case 7:
  {
   if(red == 'o')
    stateNum = 8;
   else
    return "ERR";
   break;
  }

  case 8:
  {
   if(red == 'o')
    stateNum = 9;
   else
    return "ERR";
   break;
  }

  case 9:
  {
   if(red == 'l')
    stateNum = 10;
   else
    return "ERR";
   break;
  }

  case 10:
   return "ERR";

  case 11:
  {
   if(red == 'n')
    stateNum = 12;
   else
    return "ERR";
   break;
  }

  case 12:
  {
   if(red == 's')
    stateNum = 13;
   else
    return "ERR";
   break;
  }

  case 13:
  {
   if(red == 't')
    stateNum = 14;
   else
    return "ERR";
   break;
  }

case 14:
  return "ERR";

case 15:
  {
   if(red == 'q')
    stateNum = 16;
   else
    return "ERR";
   break;
  }

  case 16:
  {
   if(red == 'e')
    stateNum = 17;
   else
    return "ERR";
   break;
  }

  case 17:
   return "ERR";

  case 18:
  {
   if(red == 'q')
    stateNum = 19;
   else
    return "ERR";
   break;
  }

  case 19:
  {
   if(red == 'e')
    stateNum = 20;
   else
    return "ERR";
   break;
  }

  case 20:
   return "ERR";

  case 21:
  {
   if(red == 'l')
    stateNum = 22;
   else
    return "ERR";
   break;
  }

  case 22:
  {
   if(red == 't')
    stateNum = 23;
   else if(red == 's')
    stateNum = 24;
   else
    return "ERR";
   break;
  }
  case 23:
   return "ERR";
  case 24:
  {
   if(red == 'e')
    stateNum = 25;
   else
    return "ERR";
   break;
  }
  case 25:
    return "ERR";
  case 26:
  {
   if(red == 's')
    stateNum = 27;
   else if(red == 'n')
    stateNum = 28;
   else
    return "ERR";
   break;
  }
  case 27:
   return "ERR";
  case 28:
  {
   if(red == 'p')
    stateNum = 29;
   else
    return "ERR";
   break;
  }
  case 29:
  {
   if(red == 't')
    stateNum = 30;
   else
    return "ERR";
   break;
  }
  case 30:
   return "ERR";
  case 31:
  {
   if(red == 's')
    stateNum = 32;
   else
    return "ERR";
   break;
  }
  case 32:
  {
   if(red == 't')
    stateNum = 33;
   else
    return "ERR";
   break;
  }
  case 33:
   return "ERR";
  case 34:
  {
   if(red == 'u')
    stateNum = 36;
   else if(red == 'o')
    stateNum = 35;
   else
    return "ERR";
   break;
  }
  case 35:
   return "ERR";
  case 36:
  {
   if(red == 'm')
    stateNum = 37;
   else
    return "ERR";
   break;
  }
  case 37:
   return "ERR";
  case 38:
  {
   if(red == 'o')
    stateNum = 39;
   else if(red == 'u')
    stateNum = 41;
   else if(red == 'r')
    stateNum = 44;
   else
    return "ERR";
   break;
  }
  case 39:
  {
   if(red == 'p')
    stateNum = 40;
   else
    return "ERR";
   break;
  }
  case 40:
   return "ERR";
  case 41:
  {
   if(red == 's')
    stateNum = 42;
   else
    return "ERR";
   break;
  }
  case 42:
  {
   if(red == 'h')
    stateNum = 43;
   else
    return "ERR";
   break;
  }
  case 43:
   return "ERR";
  case 44:
  {
   if(red == 'n')
    stateNum = 45;
   else
    return "ERR";
   break;
  }
  case 45:
  {
   if(red == 't')
    stateNum = 46;
   else
    return "ERR";
   break;
  }
  case 46:
   return "ERR";
  case 47:
  {
   if(red == 'p')
    stateNum = 48;
   else if(red == 't')
    stateNum = 50;
   else
    return "ERR";
   break;
  }
  case 48:
  {
   if(red == 't')
    stateNum = 49;
   else
    return "ERR";
   break;
  }
  case 49:
   return "ERR";
  case 50:
  {
   if(red == 'r')
    stateNum = 51;
   else
    return "ERR";
   break;
  }
  case 51:
  {
   if(red == 'n')
    stateNum = 52;
   else
    return "ERR";
   break;
  }
  case 52:
   return "ERR";
  case 53:
  {
   if(red == 't')
    stateNum = 54;
   else
    return "ERR";
   break;
  }
  case 54:
  {
   if(red == 'r')
    stateNum = 55;
   else
    return "ERR";
   break;
  }
  case 55:
   return "ERR";
  case 56:
  {
   if(red == 'h')
    stateNum = 57;
   else if(red == 'r')
    stateNum = 60;
   else
    return "ERR";
   break;
  }
  case 57:
  {
   if(red == 'i')
    stateNum = 58;
   else
    return "ERR";
   break;
  }
  case 58:
  {
   if(red == 's')
    stateNum = 59;
   else
    return "ERR";
   break;
  }
  case 59:
   return "ERR";
  case 60:
  {
   if(red == 'u')
    stateNum = 61;
   else
    return "ERR";
   break;
  }
  case 61:
  {
   if(red == 'e')
    stateNum = 62;
   else
    return "ERR";
   break;
  }
  case 62:
   return "ERR";
 }//switch
}//while((red = fgetc(myFile)) != EOF);

   switch(stateNum)
  {
   case 3: return "ADD_TO_LIST";
   case 4: return "ITR_AS";
   case 6: return "ARRY_TYPE";
   case 10: return "BOOL_TYP";
   case 14: return "CONSTANT";
   case 17: return "DEQ_LIST";
   case 20: return "ENQ_LIST";
   case 23: return "FLT_TYP";
   case 25: return "FALSE";
   case 27: return "IF_";
   case 30: return "INPT_DATA";
   case 33: return "LIST_TYPE";
   case 35: return "ELSE_";
   case 37: return "INT_TYP";
   case 40: return "POP_LIST";
   case 43: return "PUSH_LIST";
   case 46: return "PRNT_DATA";
   case 49: return "LOOP_THIS";
   case 52: return "RETURN_VAL";
   case 55: return "STR_TYP";
   case 59: return "THIS";
   case 62: return "TRUE";
    default: {
    //ungetc(red,myFile);
    return "ERR";
    }
  }
}
//isKeyword

void startLex()
{
 char *result;
 char red;
 char *number;
 int iString;

 while((red = fgetc(myFile)) != EOF)
 {
  //red = NULL;
  //red = EOF;
  if(red == '\n')
   iLn++;
  if(red == ' ' || red == '\n' || red == '\t')
   continue;

  if(isComment(red) == 1)
  {
   remove();
   continue;
  }
  else
   returnItAll();

  iString = isString(red);
  if(iString)
  {
   if(iString == 2)
   {
    iUnrec++;
    printf("\nUnrecognized lexeme: %s",word);
    recognized(word,"UNIDENTIFIED");
    continue;
   }
   iRec++;
   recognized(word, "STRING_LITERAL");
   continue;
  }
  else
   returnItAll();


  if(red == '\'');

  else if(isTerminated(red) == 1)
  {
   int found = 1;
   red = fgetc(myFile);
   addletter(red);
   char sec = fgetc(myFile);
   if(sec == '=')
    {
     addletter(sec);
     if(red == '<')
     {
      iRec++;
      recognized(word,"LESSEQL");
      continue;
     }
     else if(red == '>')
     {
      iRec++;
      recognized(word,"GRTREQL");
      continue;
     }
     else if(red == '!')
     {
      iRec++;
      recognized(word,"NOTEQL");
      continue;
     }
     else
     {
      iUnrec++;
      recognized(word,"UNIDENTIFIED");
     }
    }

   //if(isdigit(sec));
   //else
   // found = 0;

   else if(red == '<')
    recognized(word,"GRTRTH");
   else if(red == '>')
    recognized(word,"LESSTH");
   else if(red == '!')
    recognized(word,"NOT");
   else if(red == '+')
    recognized(word,"ADD");
   else if(red == '-')
    recognized(word,"SUB");
   else if(red == '*')
    recognized(word,"MUL");
   else if(red == '^')
    recognized(word,"RAISE");
   else if(red == '/')
    recognized(word,"DIV");
   else if(red == '%')
    recognized(word,"MOD");
   else if(red == '~')
    recognized(word,"TRUNC_DIV");
   else if(red == '|')
    recognized(word,"OR");
   else if(red == '&')
    recognized(word,"AND");
   else if(red == '=')
    recognized(word,"EQLTO");
   else if(red == '?')
    recognized(word,"COND_END");
   else if(red == ':')
    recognized(word,"ASSIGN");
   else if(red == '{')
    recognized(word,"B_START");
   else if(red == '}')
    recognized(word,"B_END");
   else if(red == ';')
    recognized(word,"STMNT_END");
   else if(red == '(')
    recognized(word,"P_START");
   else if(red == ')')
    recognized(word,"P_END");
   else if(red == ',')
    recognized(word,"COM");

   if(found)
   {
   iRec++;
   ungetc(sec,myFile);
   continue;
   }
   else
   returnItAll();
  }

  if(!(strcmp((number = isNumber(red)),"ERR") == 0))
  {
   iRec++;
   recognized(word, number);
   continue;
  }
  else
   returnItAll();


  if(!(strcmp((result = isKeyword(red)),"ERR") == 0))
  {
   iRec++;
   recognized(word, result);
   continue;
  }
  else
   returnItAll();
  if(isIdentifier(red))
  {
   iRec++;
   recognized(word,"ID");
   continue;
  }
  else
   returnItAll();

  {
   iUnrec++;
   while((red = fgetc(myFile)) != EOF)
   {
    if(red == EOF)
     break;
    if(isTerminated(red))
    {
     if(red == '\n')
     iLn--;
     if(red == '\'' || red == '=');
     else
     {
	 ungetc(red,myFile);
	 break;
	 }
    }
    addletter(red);
   }
   printf("\nLine %d: Unrecognized lexeme %s",iLn,word);
   recognized(word, "UNIDENTIFIED");
  }
 }//while((red = fgetc(myFile)) != EOF);
}

void main(int argc, char *argv[])
{

 if(argc > 1) {
  strcpy(inputFile, argv[1]);
  puts(inputFile);
 } else {
 printf("Enter .lex file: ");
 scanf("%s", inputFile);
}
 //***************************FILE NOT FOUND
 if((myFile = fopen(inputFile,"r")) == NULL)
 {
 printf("File Not Found.\nPROGRAM EXIT.");
 }
 else
 {
  size_t size = strlen(inputFile);
  if(!(inputFile[size - 1] == 'e' && inputFile[size - 2] == 'o' && inputFile[size - 3] == 'm'))
	printf("File is not in specified format.\nPROGRAM EXIT.");

  else
  {
  printf("FILE FOUND. CREATING SYMBOL TABLE");
  strcpy(inputFile, "output.mlx");
  remove(inputFile);
  startLex();
  printf("\nSuccessful.");
  printf("\nRecognized Units: %d", iRec);
  printf("\nUnrecognized Units: %d", iUnrec);

  printf("\nFile Created: %s", inputFile);
  fclose(myFile);
  }
 }

}