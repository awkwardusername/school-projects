#include<stdio.h>
#include<string.h>
#include<alloc.h>
#include<stdlib.h>

void program();
void SL(int*);
void SLP();
void S(int*);

void D();
void V(char*);
void dt(char*, int);
void dtp(char*);
void dterm(char*);

char* A(char*);
char* AP(char*);
char* ex(char*);
char* exp(char*,char*);
char* md(char*);
char* mdp(char*,char*);
char* pow(char*);
char* powp(char*, char*);
char* neg(char*);
char* term(char*);

void cnd();
void cndp();

void cnt();

void io();
void out();
void write();
void consume();
void vommit();
void consumeStatement();

void error(char*);
void warning(char*);

char token[30];
char lexeme[100];
int iptr = 1;
FILE* mlxFile;
char mlxName[100];
char mtbName[30];
int atEnd;
int success = 1;
int expSuc = 1;

int iLn = 1;
int iLnLast = 1;

int isConst = 0;
int isAssignment = 0;
char* validType(char[], char[], int, char*);
int writeToTable(char*);
int inTable();

char* idType(char*, int, int );
int inTable();
void checkUnused();

typedef struct varNode
{
 int isConst;
 char type[30];
 char name[100];
 int isDef;
 int isUsed;
 int iLn;
 varNode* nextVar;
}VARLIST;

typedef struct scoNode
{
 VARLIST* scope;
 scoNode* lastScope;
 scoNode* nextScope;
}SCOPELIST;

VARLIST* varListHead = NULL;
VARLIST* varListCurr = NULL;
SCOPELIST* scopeListHead = NULL;
SCOPELIST* scopeListCurr = NULL;

void main(int argc, char** argv)
{
if(argc > 1) {
  strcpy(mlxName, argv[1]);
} else {
printf("Enter symbol table: ");
gets(mlxName);
}

if((mlxFile = fopen(mlxName,"r")) == NULL) {
  printf("FILE NOT FOUND.\nPROGRAM EXIT.");
  return;
}

size_t size = strlen(mlxName);
if(!(mlxName[size-4] == '.' && mlxName[size-3] == 'm'  && mlxName[size-2] == 'l' && mlxName[size-1] == 'x'))
        {
		printf("FILE NOT IN SPECIFIED FORMAT.\nPROGRAM EXIT.");
        return;
		}
strcpy(mtbName, mlxName);
mtbName[size - 2] = 't';
mtbName[size - 1] = 'b';
remove(mtbName);
printf("STARTING SYNTAX ANALYSIS\n");
fclose(mlxFile);
consume();
program();
//while(!atEnd)
//{
//consume();
//printf("\n%d      %s      %s",iLn,token,lexeme);
//}
printf("\nProgram parsing : ");
if(success)
printf("Sucessful");
else
printf("FAILED");
}

int writeToTable(char* type)
{
 VARLIST* var = (VARLIST*) malloc(sizeof(VARLIST));
 VARLIST* lastVar;
 strcpy(var -> name, "NULL");

 if(varListHead == NULL)
 {
  scopeListCurr -> scope = var;
  varListHead = var;
  varListCurr = var;
 }
 else
 {
  varListCurr -> nextVar = var;
  lastVar = varListCurr;
  varListCurr = var;
 }
 var -> nextVar = NULL;
 if(inTable())
  {
   varListCurr = lastVar;
   varListCurr -> nextVar = NULL;
   free(var);
   return 0;
  }
 varListCurr -> isConst = isConst;
 strcpy(varListCurr -> type, type);
 strcpy(varListCurr -> name, lexeme);
 varListCurr -> isDef = 0;
 varListCurr -> isUsed = 0;
 varListCurr -> iLn = iLn;
 varListCurr -> nextVar = NULL;
 return 1;
}

int inTable()
{
 SCOPELIST* tempScope = scopeListHead;

 while(tempScope != NULL)
 {
  VARLIST* tempVar = tempScope -> scope;
  while(strcmp(tempVar -> name, "NULL") && !tempVar == NULL )
  {
   if(!strcmp(tempVar -> name, lexeme))
	return 1;
   tempVar = tempVar -> nextVar;
  }
  tempScope = tempScope -> nextScope;
 }
 return 0;
 }

void checkUnused()
{
 VARLIST* tempVar = scopeListCurr -> scope;

 while(tempVar != NULL)
 {
  if(tempVar -> isUsed == 0)
  {
   char err[120];
   strcpy(err, "\'");
   strcat(err,tempVar -> name);
   strcat(err,"\' was never used.");
   iLnLast = iLn;
   warning(err);
  }
  tempVar = tempVar -> nextVar;
 }
}

char* idType(char* idName, int def, int used)
{
 VARLIST* is;
 int found = 0;

 SCOPELIST* tempScope = scopeListHead;

 while(tempScope != NULL)
 {
  VARLIST* tempVar = tempScope -> scope;
  while(!tempVar == NULL)
  {
   if(!strcmp(tempVar -> name, idName))
   {
	is = tempVar;
	found = 1;
	break;
   }
   tempVar = tempVar -> nextVar;
  }
  tempScope = tempScope -> nextScope;
 }

 if(found)
 {
  if(def == 2);

  else if(def)
   is -> isDef = 1;
  if(used == 2);

  else if(used)
  {
   if(!is -> isDef && isAssignment)
   {
   char err[120];
   strcpy(err, "Variable \'");
   strcat(err, lexeme);
   strcat(err, "\' was used but may not be initialized.");
   warning(err);
   }
   is -> isUsed = 1;
  }

  isConst = is->isConst;

  return is -> type;
 }
 return "ERR";
}

void clrWord(char *arry)
{
 int max = strlen(arry);
 int i;
 for(i = 0; i < max; i++)
 {
 arry[i] = '\0';
 }
}

void error(char*mess)
{
 success = 0;
 printf("\nLine %d ERROR MESSAGE: %s",iLnLast ,mess);
}
void warning(char*mess)
{
 printf("\nLine %d WARNING MESSAGE: %s",iLnLast ,mess);
}

void vommit()
{
 iptr-=2;
 atEnd = 0;
 consume();
}

void consume()
{
 if(atEnd)
 {
 clrWord(token);
 clrWord(lexeme);
 return;
 }

 char red;
 char temp[30];

 FILE* readFile = fopen(mlxName,"r");

 if(readFile == NULL)
 {
 printf("\nERROR READING .mlx FILE.");
 return;
 }

 int tmp = 0;
 iLnLast = iLn;
 while(tmp<iptr)
 {
 fscanf(readFile,"%d      %s      ",&iLn,token);
 if(!strcmp(token,"STRING_LITERAL"))
 {
  char c;
  clrWord(lexeme);
  int strIn = 0;
  c = fgetc(readFile);
  lexeme[strIn] = c;
  strIn++;
  while(1)
  {
   c = fgetc(readFile);
   lexeme[strIn] = c;
   strIn++;
   if(c == '\'')
	break;
  }
  fgetc(readFile);
  char d = fgetc(readFile);
  if(!feof(readFile))
  ungetc(d,readFile);
 }
 else
 fscanf(readFile,"%s ",lexeme);

 tmp++;
 }
 //printf("\n%s\t%s",lexeme,token);
 if(feof(readFile))
  atEnd = 1;
 iptr++;

 fclose(readFile);

 }

void consumeStatement()
{
 while(strcmp("STMNT_END",token))
 {
  consume();
  if(atEnd)
   break;
 }
}

char* validType(char from[30], char to[30], int talk, char opt[5])
{

 if(!strcmp(opt, "~"))
 {
  return to;
 }

 if(!(strcmp(opt, "!=") && strcmp(opt, "=") ))
 {
  return "BOOL_TYP";
 }
 if(!strcmp(opt, ""))
 {
  if(!(strcmp(from, "FLOAT") && strcmp(from, "FLT_TYP")))
   if(!(strcmp(to,"INTEGER") && strcmp(to,"INT_TYP")))
	{
	 error("Incompatible type from FLOAT to INTEGER type.");
	 return "ERR";
	}

  if(!(strcmp(from, "BOOL_TYP") && strcmp(from, "TRUE") && strcmp(from, "FALSE")))
	 if(strcmp(to, "BOOL_TYP") && strcmp(to, "TRUE") && strcmp(to, "FALSE"))
	 {
	  if(talk && expSuc)
	  {
	   char err[100];
	   strcpy(err, "Incompatible type from BOOLEAN to ");
	   strcat(err, to);
	   strcat(err, ".");
	   error(err);
	  }
	  expSuc = 0;
	  return "ERR";
	 }
   return "SUCCESS!";
 }

 if(!(strcmp(opt, "%")))
 {
  if(!(strcmp(from, "INTEGER") && strcmp(from, "INT_TYP")))
  {
   if(!(strcmp(to, "INTEGER") && strcmp(to, "INT_TYP")))
	return "INTEGER";
  }
  if(talk && expSuc)
   {
	char err[100];
	strcpy(err, "Operator '");
	strcat(err, opt);
	strcat(err, "' can only be applied to INTEGER type.");
	error(err);
   }
   expSuc = 0;
   return "ERR";
 }

 if(!(strcmp(opt, "+") && strcmp(opt, "-")))
 {
  if(!(strcmp(from, "STR_TYP") && strcmp(from, "STRING_LITERAL")))
  {
   if(!(strcmp(to, "STR_TYP") && strcmp(to,"STRING_LITERAL")))
	return "STRING_LITERAL";
   if(talk && expSuc)
   {
	char err[100];
	strcpy(err, "Operator '");
	strcat(err, opt);
	strcat(err, "' can only be applied to INTEGER type.");
	error(err);
   }
   expSuc = 0;
   return "ERR";
  }
 }

 if(!(strcmp(opt, "+") && strcmp(opt, "-") && strcmp(opt, "*") && strcmp(opt, "/") && strcmp(opt, "~") && strcmp(opt, "^")))
 {
  if(!(strcmp(from, "BOOL_TYP") && strcmp(from, "TRUE") && strcmp(from, "FALSE") && strcmp(to, "BOOL_TYP") && strcmp(to, "TRUE") && strcmp(to, "FALSE")))
  {
   if(talk && expSuc)
   {
	char err[100];
	strcpy(err, "Operator '");
	strcat(err, opt);
	strcat(err, "' cannot be applied to boolean type.");
	error(err);
   }
   expSuc = 0;
   return "ERR";
  }

  if(!strcmp(from, "INT_TYP"))
  {
   if(!strcmp(to, "INTEGER"))
   {
	return "INTEGER";
   }
  }

 if(!strcmp(from, "INTEGER"))
  {
   if(!strcmp(to, "INT_TYP"))
   {
	return "INTEGER";
   }
  }



  if(!strcmp(from, to))
   {
	if(!strcmp(from, "INT_TYP"))
	 return "INTEGER";
	if(!strcmp(from, "FLT_TYP"))
	 return "FLOAT";
	return from;
   }
  return "FLOAT";
 }

if(!(strcmp(opt,"<") && strcmp(opt,"<=") && strcmp(opt,">") && strcmp(opt,">=")))
{
 if(!(strcmp(from, "BOOL_TYP") && strcmp(from, "TRUE") && strcmp(from, "FALSE") && strcmp(to, "BOOL_TYP") && strcmp(to, "TRUE") && strcmp(to, "FALSE")))
 {
  if(talk && expSuc)
  {
   char err[100];
   strcpy(err, "Operator '");
   strcat(err, opt);
   strcat(err, "' cannot be applied to boolean type.");
   error(err);
  }
  expSuc = 0;
  return "ERR";
 }
 return "BOOL_TYP";
}



if(!(strcmp(opt,"|") && strcmp(opt,"&")))
{
 if(!(strcmp(from, "BOOL_TYP") && strcmp(from, "TRUE") && strcmp(from, "FALSE") && strcmp(to, "BOOL_TYP") && strcmp(to, "TRUE") && strcmp(to, "FALSE")))
 {
  return "BOOL_TYP";
 }
 if(talk && expSuc)
  {
   char err[100];
   strcpy(err, "Operator '");
   strcat(err, opt);
   strcat(err, "' cannot be applied to boolean type.");
   error(err);
  }
  expSuc = 0;
 return "ERR";
}

if(talk && expSuc)
{
 char err[120];
 strcpy(err, "Incompatible type: ");
 strcat(err, from);
 strcat(err, " to: ");
 strcat(err, to);
 error(err);
}
 expSuc = 0;
 return "ERR";
}

void program()
{
 if(!strcmp("run",lexeme))
 {
  consume();
  if(!strcmp("P_START",token))
  {
   consume();
   if(!strcmp("P_END",token))
   {
	consume();
	if(!strcmp("B_START",token))
	{
	 int sentence = 0;
	 SCOPELIST* scope = (SCOPELIST*) malloc(sizeof(SCOPELIST));
	 if(scopeListCurr == NULL)
	 {
	  if(!scope)
	   printf("Cannot create struct SCOPELIST!");
	  else
	  {
	   scopeListHead = scope;
	   scopeListCurr = scope;
	   scopeListCurr -> lastScope = NULL;
	  }
     }
	 scopeListCurr -> nextScope = NULL;
     varListHead = NULL;
     scopeListCurr -> scope -> nextVar = NULL;
     scopeListCurr -> scope = NULL;
     consume();
	 SL(&sentence);
     if(!strcmp("B_END",token))
	  {
	   consume();
           checkUnused();
      }
     else
      error("Expecting } symbol");
    }
    else
    error("Expecting { symbol");
   }
   else
   error("Expecting ) symbol");
  }
  else
  error("Expecting ( symbol");
 }
 else
 error("Expecting run method");

 if(strcmp("",token))
 {
  success = 0;
  printf("\nERROR MESSAGE: EOF not reached. Statements out of program's scope exist. ");
 }
}

void SL(int* sentence)
{
 S(sentence);
 if(sentence == 0)
  error("Statement missing.");
 SLP();
}

void SLP()
{
 if(atEnd);

 else if(strcmp("B_END",token))
 {
 int tmp = 0;
 S(&tmp);
 SLP();
 }
}



void S(int* sentence)
{
 isConst = 0;
 isAssignment = 0;
 if((!strcmp("ID",token) && strcmp(A(""), "NOTASSIGN")))
 (*sentence)++;
 else if(!(strcmp("CONSTANT",token) && strcmp("FLT_TYP",token) && strcmp("BOOL_TYP",token) && strcmp("INT_TYP",token) && strcmp("STR_TYP",token)))
  {
  (*sentence)++;
  D();
  }
 else if(!strcmp("IF_",token))
  {
  (*sentence)++;
  cnd();
  }
 else if(!strcmp("LOOP_THIS",token))
  {
  (*sentence)++;
  cnt();
  }
 else if(!(strcmp("INPT_DATA",token) && strcmp("PRNT_DATA",token)))
  {
  (*sentence)++;
  io();
  }
 else if(!strcmp("B_START",token))
 {
 //(*sentence)++;
  SCOPELIST* scope =(SCOPELIST*) malloc(sizeof(SCOPELIST));
  if(!scope)
   printf("Cannot create struct SCOPELIST!");
  else
	 {
	  scopeListCurr -> nextScope = scope;
	  scope -> lastScope = scopeListCurr;
	  scopeListCurr = scope;
	  varListHead = NULL;
	  scopeListCurr -> scope -> nextVar = NULL;
	  scopeListCurr -> nextScope = NULL;
	  scopeListCurr -> scope = varListHead;
	 }
  consume();
  SL(sentence);
  if(!strcmp("B_END",token))
   {
    checkUnused();
    scopeListCurr = scopeListCurr -> lastScope;
    free(scopeListCurr -> nextScope);
    scopeListCurr -> nextScope = NULL;
	varListHead = scopeListCurr -> scope;
    varListCurr = varListHead;
	while(varListCurr -> nextVar!= NULL)
	 varListCurr = varListCurr -> nextVar;
	consume();
   }
  else
   error("Expecting } symbol");
 }
 else if(!strcmp("B_END", token));
 else
 {
 int once = 1;
 iLnLast = iLn;
 error("Unknown statement");
 while(strcmp("STMNT_END",token))
 {
  if(once)
  {
   printf("\n\t");
   once = 0;
  }
  printf(" %s",lexeme);
  consume();
  if(atEnd)
   break;
 }
 consume();
 }
}

void D()
{
 if(!strcmp("CONSTANT",token))
 {
  isConst = 1;
  consume();
 }
 if(!(strcmp("FLT_TYP",token) && strcmp("INT_TYP",token) && strcmp("STR_TYP",token) && strcmp("BOOL_TYP",token)))
 {
  char type[30];
  strcpy(type, token);
  consume();
  V(type);
  if(!strcmp("STMNT_END",token))
   consume();
  else
   error("Declaration syntax error, terminator missing.");
 }
 else
 {
  error("Declaration syntax error, constant datatype missing.");
  consumeStatement();
  consume();
 }
}

void V(char* type)
{
 dt(type,0);
 if(!strcmp("COM",token))
 {
 consume();
 V(type);
 }
}


void dt(char* type, int mode)
{
 if(!strcmp("ID",token))
 {
  if(!mode) //mode 0
  {
   if(!writeToTable(type))
   {
	char err[100] = "Multple declaration for \'";
	strcat(err, lexeme);
    strcat(err, "\'");
    error(err);
   }
  }
  else //mode 1
  {
   if(!inTable())
   {
    char err[100] = "Undeclared variable \'";
    strcat(err, lexeme);
	strcat(err, "\'");
    error(err);
   }
   isAssignment = 1;
   idType(lexeme, 2, 1);
   isAssignment = 0;
  }
  consume();
  dtp(type);
 }
 else
 {
  if(mode)
   error("Declaration syntax error. Identifier missing.");
  else
   error("Declaration syntax error. Value missing.");
  consumeStatement();
 }
}

void dtp(char* type)
{
  if(!strcmp("ASSIGN",token))
  {
   vommit();
   idType(lexeme, 1, 2);
   consume();
   consume();
   dterm(type);
  }
  else
   if(isConst)
    error("Contant values must be initialized upon declaration.");
}

void dterm(char* type)
{
 char actType[30];
 if(!(strcmp("SUB", token) && strcmp("ADD", token)))
  {
   consume();
   if(!(strcmp("INTEGER",token) && strcmp("FLOAT",token)));
   else if(!strcmp("ID",token))
    {
     strcpy(actType, idType(lexeme, 2, 1));
	 if(strcmp("INT_TYP", actType) && strcmp("FLT_TYP", actType))
      error("Right side of a -/+ sign must be integer or float type");
    }
  }

 else if(!strcmp("NOT", token))
  {
   consume();
   if(!(strcmp("TRUE",token) && strcmp("FALSE", token)));
   else if(!strcmp("ID",token))
   {
    strcpy(actType, idType(lexeme, 2, 1));
    if(strcmp("BOOL_TYP", actType))
	 error("Right token of a ! operator must be boolean type.");
   }
  }

  if(!(strcmp("INTEGER",token) && strcmp("FLOAT",token) && strcmp("STRING_LITERAL",token) && strcmp("TRUE",token) && strcmp("FALSE",token)))
  {
  validType(token,type,1,"");
  consume();
  }

 else
 {
  if(!strcmp("ERR", idType(lexeme, 2, 1)));

  else
   validType(idType(lexeme, 2, 1), type, 1,"");
  dt(type, 1);
 }
}

char* A(char leftToken[30])
{
 isAssignment = 1;
 expSuc = 1;
 char type[30];
 char expType[30];
 char err[120];
 consume();
 if(!strcmp("ASSIGN",token))
  {
   vommit();
   if(!inTable())
   {
    iLnLast = iLn;
    strcpy(err, "Undefined symbol \'");
	strcat(err, lexeme);
	strcat(err, "\'");
    error(err);
    expSuc = 0;
   }
   else
   {
	strcpy(expType,idType(lexeme, 2, 2));
	if(isConst)
    {
     error("Constant variable cannot be reinitialized.");
	 consumeStatement();
	 consume();
     return "";
    }
   }

   consume(); //consume ID
   consume(); //consume assignment opt

   strcpy(type, AP(leftToken));
   if(expSuc)
	strcpy(type, validType(type, expType, 1, ""));

   return type;
  }
 else
  {
   vommit();
   return "NOTASSIGN";
  }
}

char* AP(char leftToken[30])
{
 char id[100];
 char type[30];
 if(!strcmp("ID",token))
 {
  strcpy(id, lexeme);
  consume();
  if(!strcmp("ASSIGN",token))
  {
   vommit();
   strcpy(type, A(leftToken));
  }
   else
  {
   vommit();
   strcpy(type, ex(leftToken));
   if(!strcmp("STMNT_END",token))
	consume();
   else
   {
    error("Assignment syntax error, missing ; terminator.");
    consumeStatement();
    consume();
   }//else
  }//else
 idType(id, 1, 2);
 }
 else
 {
  strcpy(type, ex(leftToken));
  if(!strcmp("STMNT_END",token))
   consume();
  else
  {
   error("Assignment syntax error, missing ; terminator.");
   consumeStatement();
   consume();
  }//else
 }//else
 return type;
}

char* ex(char leftToken[30])
{
 char type[30];
 char type2[30];
 char resultType[30];
 char lex[5];

 strcpy(type, md(leftToken));
 if(!strcmp(type, "ERR"))
  return "ERR";
 strcpy(type2, exp(type, lex));
 if(!strcmp(type2, "NULL"))
  return type;
 else if(!strcmp(type2, "ERR"))
  return "ERR";
 strcpy(resultType, validType(type2, type, 1,lex));
 return resultType;
}

char* exp(char leftToken[30], char* lex)
{
 char type[30];
 char type2[30];
 char expType[30] = "NULL";
 char resultType[30];
 int talk = 0;

 if(!(strcmp("LESSTH",token) && strcmp("GRTRTH",token) && strcmp("EQLTO",token) && strcmp("LESSEQL",token) && strcmp("GRTREQL",token) && strcmp("NOTEQL",token) && strcmp("AND",token) && strcmp("OR",token) && strcmp("ADD",token) && strcmp("SUB",token)))
 {
 if(!(strcmp(token, "EQLTO") && strcmp(token, "NOTEQL")))
  talk = 1;
 if(!(strcmp(token, "LESSTH") && strcmp(token, "LESSEQL") && strcmp(token, "GRTRTH") && strcmp(token, "GRTREQL") && strcmp(token, "ADD") && strcmp(token, "SUB")))
  strcpy(expType, "FLT_TYP");
 if(!(strcmp(token, "AND") && strcmp(token, "OR")))
  strcpy(expType, "BOOL_TYP" );

 strcpy(lex, lexeme);
 consume();
 strcpy(type, md(leftToken));

 if(!strcmp(type, "NOT"))
  strcpy(resultType, "NOT");
 else if(!strcmp(type, "ERR"))
  return "ERR";
 else if(strcmp(expType, "NULL"))
 {
 if(talk)
 {
  ;
 }
 else if(!(strcmp("ERR", validType(type, expType, 0, lex)) && strcmp("ERR", validType(leftToken, expType, 0,lex))) && strcmp(leftToken, ""))
 {
  char err[120];
  strcpy(err, "Operator ");
  strcat(err, lex);
  strcat(err, " must be applied to INTEGER or FLOAT type.");
  error(err);
  return "ERR";
 }
 }
 strcpy(type2, exp(type, lex));
 if(!strcmp(type2, "NULL"))
  return type;
 else
  strcpy(resultType, (validType(type2, type, 1, lex)));

 if(!strcmp("ERR", resultType))
  return "NULL";
 else
  return resultType;
 }
 else
  strcpy(type, "NULL");
 return type;
}

char* md(char leftToken[30])
{
 char type[30];
 char type2[30];
 char resultType[30];
 char lex[5];
 strcpy(type, pow(leftToken));
 if(!strcmp(type, "ERR"))
  return "ERR";
 strcpy(type2, mdp(type,lex));
 if(!strcmp(type2, "NULL"))
  return type;
 else if(!strcmp(type2, "ERR"))
  return "ERR";
 strcpy(resultType, validType(type2, type, 1, ""));
 return resultType;
}


char* mdp(char leftToken[30], char* lex)
{
 char type[30];
 char type2[30];
 char resultType[30];
 char expType[30] = "FLT_TYP";

 if(!(strcmp("MUL",token) && strcmp("DIV",token) && strcmp("TRUNC_DIV",token) && strcmp("MOD",token)))
 {
 strcpy(lex, lexeme);
 if(!strcmp(lex, "%"))
  strcpy(expType, "INTEGER");
 consume();
 strcpy(type, pow(leftToken));

 if(!strcmp(type, "NOT"))
  strcpy(resultType, "NOT");
 else if (!strcmp(type, "ERR"))
  return "ERR";
 else if(!strcmp("ERR", validType(type, expType, 0, lex)) && strcmp(leftToken, ""))
 {
  char err[120];
  strcpy(err, "Operator ");
  strcat(err, lex);
  strcat(err, " cannot be applied to ");
  strcat(err, type);
  error(err);
  return "ERR";
 }
  else if(!strcmp("ERR", validType(leftToken, expType, 0,lex)) && strcmp(leftToken, ""))
 {
  char err[120];
  strcpy(err, "Operator ");
  strcat(err, lex);
  strcat(err, " cannot be applied to ");
  strcat(err, leftToken);
  error(err);
  return "ERR";
 }

 strcpy(type2, mdp(type,lex));
 if(strcmp(type2, "NULL") && strcmp(type, "NOT"))
  strcpy(resultType, (validType(type2, type, 1, lex)));
 else
  return "NULL";

 if(!strcmp("ERR", resultType))
  return "NULL";
 else
  return resultType;
 }
 else
  strcpy(type, "NULL");
 return type;
}

char* pow(char leftToken[30])
{
 char type[30];
 char type2[30];
 char resultType[30];
 char lex[5];

 strcpy(type, neg(leftToken));
 if(!strcmp(type, "ERR"))
  return "ERR";
 strcpy(type2, powp(type, lex));
 if(!strcmp(type2, "NULL"))
  return type;
 else if(!strcmp("ERR", type2))
  return "ERR";
 strcpy(resultType, validType(type2, type, 1, lex));
 return resultType;
}

char* powp(char leftToken[30], char*lex)
{
char type[30];
char type2[30];
char resultType[30];

 if(!strcmp("RAISE",token))
 {
 strcpy(lex, lexeme);
 consume();
 strcpy(type, neg(leftToken));

 if(!strcmp(type, "NOT"))
  strcpy(resultType, "NOT");
 else if(!(strcmp("ERR", validType(type, "FLT_TYP", 0,"^")) && strcmp("ERR", validType(leftToken, "FLT_TYP", 0,"^"))) && strcmp(leftToken, ""))
  {
  error("Operator ^ must be applied to INTEGER or FLOAT type.");
  consumeStatement();
  return "ERR";
  }
 strcpy(type2, powp(type,lex));
 if(strcmp(type2, "NULL") && strcmp(type, "NOT"))
  strcpy(resultType, validType(type2, type, 1,"^"));
 else
  return type;//return "NULL";

 if(!strcmp("ERR", resultType))
   return "NULL";
 else
   return resultType;
 }
 else
  strcpy(resultType, "NULL");
 return resultType;
}

char* neg(char leftToken[30])
{
 return term(leftToken);
}

char* term(char leftToken[30])
{
 char type[30] = "UNK";
 char err[120] = "";
 char expType[30] = "NULL";
 char resultType[30];

 if(!strcmp("NOT",token))
 {
  consume();
  strcpy(expType, "BOOL_TYP");
 }

 else if(!(strcmp("ADD",token) && strcmp("SUB",token)))
 {
  consume();
  strcpy(expType, "FLT_TYP");
 }

 if(!(strcmp("INTEGER",token) && strcmp("STRING_LITERAL",token) && strcmp("FLOAT",token) && strcmp("TRUE",token) && strcmp("FALSE",token) && strcmp("ID",token)))
 {
  if(!strcmp("ID", token))
   strcpy(type, idType(lexeme, 2, 1));
  else
   strcpy(type, token);

  if(!strcmp("ERR",type))
  {
   iLnLast = iLn;
   strcpy(err, "Undefined symbol \'");
   strcat(err, lexeme);
   strcat(err, "\'");
   error(err);
   consume();
   return "NOT";
  }
  consume();
 }

 else if(!strcmp("P_START",token))
 {
 consume();
 strcpy(type, ex(leftToken));
 if(!strcmp("P_END",token))
  consume();
 else
  {
   error("Expecting ) symbol.");
   consumeStatement();
  }
 }//else if

 else
 {
  error("Expression syntax error. ");
  consumeStatement();
  strcpy(resultType, "");
 }

  if(strcmp(expType, "NULL"))
   strcpy(err, validType(type, expType, 0,""));

  if(!strcmp(err, "ERR"))
  {
	if(!strcmp(expType, "FLT_TYP"))
	 error("Right side of +/- sign must be FLOAT or INTEGER type.");
	else if(!strcmp(expType, "BOOL_TYP"))
	 error("Right side of ! operator must be boolean type.");
	return type;
  }
  else
   return type;
}

void cnd()
{
  consume();
  ex("");
  if(!strcmp("COND_END",token))
  {
   int tmp = 0;
   consume();
   S(&tmp);
   cndp();
  }
  else
  {
   error("Conditional syntax error. Expecting ? symbol.");
   consumeStatement();
  }
}

void cndp()
{
 if(!strcmp("ELSE_",token))
 {
  int sentence = 0;
  consume();
  S(&sentence);
  if(sentence == 0)
   error("No statement missing.");
 }
}

void cnt()
{
 consume();
 cnd();
}

void io()
{
 if(!strcmp("INPT_DATA",token))
 {
  consume();
  if(!strcmp("ID",token))
  {
   consume();
  }                                      
  else
  {                                      
  error("Input statement syntax error, missing an Identifier.");
  consumeStatement();                    
  }                                      
 }
 else
 {                                       
  consume();                             
  ex("");
 }                                       
 if(!strcmp("STMNT_END",token))          
  consume();                             
 else                                    
  error("IO syntax error, missing ; symbol.");
}                                        
                                         
void out()
{
 write();                                
 if(!strcmp("ADD",token))                
 {                                       
  consume();                             
  out();                                 
 }                                       
}
                                         
void write()                             
{                                        
 if(!strcmp("STRING_LITERAL",token))
  consume();
 else
  ex("");
}