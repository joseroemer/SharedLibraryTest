#include "stdio.h"
#include "stdlib.h"
#include "dlfcn.h"
#include "gnu/lib-names.h"
#include <string.h>

#define BUF_MAX_LINE 1024
const int expectedTokens  = 2;
FILE *logFile;

/* 
 * Function: 
 *		- ltrim
 * Description:
 * 		remove spaces from left side
 * Parameters:
 * 		- char * bufLine
 * Output:
 *		- char *
 * creation date:
 * 		- 09/12/23
 * 
 */
char *ltrim(char *bufLine)
{
	char *pBufLine = bufLine;   
    while((*pBufLine ==' ') || (*pBufLine=='\t'))
    {
        pBufLine++;
    }

    return pBufLine;
}

/* 
 * Function: 
 *		- rtrim
 * Description:
 * 		remove spaces from rigth side
 * Parameters:
 * 		- char[] bufLine
 * Output:
 *		- char *
 * creation date:
 * 		- 09/12/23
 * 
 */ 
char *rtrim(char bufLine[])
{
	int lIndex = strlen(bufLine)-1;
	while((bufLine[lIndex] == ' ') 
	      || (bufLine[lIndex] == '\t') 
	      || (bufLine[lIndex] == '\n'))
	{
		bufLine[lIndex--] = '\0';
	}
	return bufLine;
}

/* 
 * Function: 
 *		- processComments
 * Description:
 * 		it receive a line and will discart the part commented with '#'
 *		will return only the left part of the '#'	
 * Parameters:
 * 		- char * pLine
 * Output:
 *		-  char *
 * creation date:
 * 		- 09/14/23
 * 
 */
char * processComments(char *pLine)
{
	char *pLineAux = pLine;
	int len=strlen(pLine);
	int i=0;
	while(i<len)
	{
		if (pLineAux[i]=='#' || pLineAux[i]==';')
		{
			pLineAux[i]='\0';
		}	
		i++;
	}	
	return pLine;
}

/* 
 * Function: 
 *		- getParamValue
 * Description:
 * 		recieve a line like:
 *			--param=value | -param=value
 *		and return the part of value if it is possible
 * Parameters:
 * 		- char * param
 * Output:
 *		-  char *
 * creation date:
 * 		- 09/14/23
 * 
 */
char *getParamValue(char *param)
{
	char *pValue = param;
	int len = strlen(param);
	int i=0;
	int vFound = 0;
	while (i<len)
	{
		if (pValue[i] == '=')
		{
			vFound = 1;
			break;
		}
		i++;
	}	
	if(!vFound)
	{
		return NULL; 
	}	
	return &pValue[++i];
}

/* 
 * Function: 
 *		- addLog
 * Description:
 * 		add a line in log file
 * Parameters:
 * 		- char * msg
 * Output:
 *		- void
 * creation date:
 * 		- 09/12/23
 * 
 */ 
void addLog(char *msg)
{
	if (logFile!=NULL)
	{	
		fprintf(logFile,"%s",msg);
	}	
}

/* 
 * Function: 
 *		- startLog
 * Description:
 * 		open log file 
 * Parameters:
 * 		- char * nameLogFile
 * Output:
 *		- void
 * creation date:
 * 		- 09/12/23
 * 
 */
void startLog(char *nameLogFile)
{
	char nameLogAux[BUF_MAX_LINE];
	sprintf(nameLogAux, "%s.log",nameLogFile);
	logFile = fopen(nameLogAux,"a+");
	addLog("-------------------------------\n");
	addLog("opening log file\n");	
}

/* 
 * Function: 
 *		- closeLog
 * Description:
 * 		close log file 
 * Parameters:
 * 		- void
 * Output:
 *		- void
 * creation date:
 * 		- 09/12/23
 * 
 */
void closeLog()
{
	if (logFile!=NULL)
	{
		addLog("closing log file\n");
		addLog("-------------------------------\n");
		fclose(logFile);
	}	
}

/* 
 * Function: 
 *		- callFunction
 * Description:
 * 		Try to call a function from a shared library return 1 
 * 		if it was successful and 0 in other case
 * Parameters:
 * 		- void * handleLib
 * 		- char * nameFunction
 * Output:
 *		- int
 * creation date:
 * 		- 09/12/23
 * 
 */
int callFunction(void *handleLib, char *nameFunction)
{
	char msg[BUF_MAX_LINE];
	int bReturn;
	void (*func)(void);
	func = dlsym(handleLib, nameFunction);

	if (func!=NULL)
	{
		sprintf(msg, "\tcalling function %s\n", nameFunction);
		addLog(msg);
		printf("    %s", msg);
		
		func();
		bReturn = 1;
	}
	else
	{
		sprintf(msg, "\tfunction %s is not found\n", nameFunction);
		addLog(msg);
		printf("    the function is not found\n");
		
		bReturn = 0;
	}	
	
	return bReturn;
}

/* 
 * Function: 
 *		- loadLibrary
 * Description:
 * 		Try to load a library in run time 
 *		return a void pointer with the handle if it was possible
 * 		and NULL in other case
 * Parameters:
 * 		- char * nameLibrary
 * Output:
 *		- void *
 * creation date:
 * 		- 09/12/23
 * 
 */
void *loadLibrary(char *nameLibrary)
{
	char msg[BUF_MAX_LINE];
	sprintf(msg, "\tloading library %s\n", nameLibrary);
	addLog(msg);
	
	printf("    loading library %s ...\n", nameLibrary);
	void *handleLib = dlopen(nameLibrary, RTLD_LAZY);

	if (handleLib!=NULL)
	{
		//perror("ldopen");
		sprintf(msg, "\tloaded successfully\n");
		addLog(msg);
		printf("    successfully\n");
	}

	return handleLib;
}

/* 
 * Function: 
 *		- closeLibrary
 * Description:
 * 		Try to close a library 
 * Parameters:
 * 		- void * handleLib
 * Output:
 *		- void 
 * creation date:
 * 		- 09/12/23
 * 
 */
void closeLibrary(void *handleLib)
{
	//printf("    releasing\n");
	char msg[BUF_MAX_LINE];
	sprintf(msg, "   release library\n");
	addLog(msg);
	dlclose(handleLib);
}

/* 
 * Function: 
 *		- getInstructions
 * Description:
 * 		receive a line and process that line to get
 *		command (use|call|other) and 
 *		parameter (function's name or library's name)
 *		return 1 if it was possible and 0 in other case
 * Parameters:
 * 		- char * pLine
 * 		- int lineNumber
 * 		- char commands[][]
 * Output:
 *		- void 
 * creation date:
 * 		- 09/14/23
 * 
 */
int getInstructions(char *pLine, int lineNumber, 
                     char commands[2][BUF_MAX_LINE])
{
	char line[BUF_MAX_LINE];

	strcpy(line, pLine);
	
	int bReturn = 0;
	int numTokens = 0;
	char *sep = " ";

	memset(commands[0], '\0', BUF_MAX_LINE);
	memset(commands[1], '\0', BUF_MAX_LINE);
	char *pStr = strtok(line, sep);
	if (pStr!=NULL)
	{	int excceded = 0;
		int i=0;
		while(pStr != NULL)
		{
			if (numTokens++>=expectedTokens)
			{
				/*sprintf(msg, "\tline: %d, error: more parameters than \
			expected %d\n",
				        lineNumber, numTokens);
				addLog(msg);*/
				excceded = 1;
				break;
			}
			strcpy(commands[i++], pStr);
			
			pStr = strtok(NULL, sep);
		}
		if(excceded==0)
		{	
			bReturn=1;
		}	
	}
	
	return bReturn;
}	

/* 
 * Function: 
 *		- executeCommand
 * Description:
 * 		receive a pair of command and parameter like:
 * 			use  lib1
 * 			call func1
 * 		and process the line 
 *			either open a library or call a function
 *		return a library handle or NULL 
 * Parameters:
 * 		- char commands[][]
 *		- void * handleLib
 * 		- int lineNumber
 * Output:
 *		-  void *
 * creation date:
 * 		- 09/14/23
 * 
 */
void *executeCommand(char commands[2][BUF_MAX_LINE], void *handleLib, 
                  int lineNumber)
{
	char msg[BUF_MAX_LINE];
	void *handleLibrary = handleLib;
	if (strcmp(commands[0],"use")==0)
	{
		printf("use library %s\n", commands[1]);
		sprintf(msg, "   use library %s\n", commands[1]);
		addLog(msg);
		if (handleLibrary)
		{
			sprintf(msg, "\tline: %d, changing library \n", lineNumber);
			addLog(msg);
			closeLibrary(handleLibrary);
		}	
		
		if((handleLibrary = loadLibrary(commands[1]))==NULL)
		{
			sprintf(msg, "\tline: %d, couldn't load library %s\n", 
			        lineNumber, commands[1]);
			addLog(msg);
			printf("    file not found\n");
		}	
	}
	else if (strcmp(commands[0],"call")==0)
	{
		printf("call function %s\n", commands[1]);
		sprintf(msg, "   call function %s\n", commands[1]);
		addLog(msg);
		if(handleLibrary)
		{			
			if (!callFunction(handleLibrary, commands[1]))
			{
				sprintf(msg, "\tline: %d, couldn't call function %s\n", 
				        lineNumber, commands[1]);
				addLog(msg);
				//printf("    line: %d, call %s couldn't be executed\n",
				//       lineNumber, commands[1]);
			}	
		}
		else
		{
			sprintf(msg, "\tline: %d, library %s couldn't be loaded\n", 
				        lineNumber, commands[1]);
				addLog(msg);
			printf("    the library is not loaded\n");	
		}	
	}
	else
	{
		sprintf(msg, "\tline: %d, error: unknow command %s\n",
				        lineNumber, commands[0]);
		addLog(msg);
		printf("unknow command %s\n", commands[0]);
	}
	return handleLibrary;
}

/* 
 * Function: 
 *		- processLine
 * Description:
 * 		receive a line and try to get cammand and parameter
 *		if it is possible execute the command obtenined
 * 		return a library handle or NULL
 * Parameters:
 * 		- char *line
 * 		- int lineNumber
 * 		- void *handleLibrary 
 * Output:
 *		- void *
 * creation date:
 * 		- 09/15/23
 * 
 */

void *processLine(char *line, int lineNumber, void *handleLibrary)
{
	char commands[2][BUF_MAX_LINE];
	char *pLine = line;
	if(getInstructions(pLine, lineNumber,
	                   commands))
	{	
		handleLibrary = executeCommand(commands, handleLibrary, 
                        lineNumber);
	}	
	else
	{
		char msg[BUF_MAX_LINE];
		sprintf(msg, "   line %d couldn't be proccessed\n", lineNumber);
		addLog(msg);
	}	
	return handleLibrary;
}

/* 
 * Function: 
 *		- openScriptFile
 * Description:
 * 		open a script file
 *		return a handle file or NULL 
 * Parameters:
 * 		- char nameScriptFile
 * Output:
 *		-  FILE *
 * creation date:
 * 		- 09/14/23
 * 
 */
FILE *openScriptFile(char *nameScriptFile)
{
	FILE *scriptFile;
	char msg[BUF_MAX_LINE];
	sprintf(msg, "opening script file '%s'\n", nameScriptFile);
	addLog(msg);
	if ((scriptFile = fopen(nameScriptFile,"r"))==NULL)
    {
		sprintf(msg, "file '%s' doesn't exist\n", nameScriptFile);
		addLog(msg);
    }

	return scriptFile; 
}	

/* 
 * Function: 
 *		- closeScriptFile
 * Description:
 * 		close a script file 
 * Parameters:
 * 		- FILE * scriptFile
 * Output:
 *		-  void
 * creation date:
 * 		- 09/14/23
 * 
 */
void closeScriptFile(FILE *scriptFile)
{
	char msg[BUF_MAX_LINE];
	sprintf(msg, "closing script file\n");
	addLog(msg);
	fclose(scriptFile);
}


/* 
 * Function: 
 *		- processScriptFile
 * Description:
 * 		this function is called when 
 * 		the program receive a script file
 * 		it return 0 if script can't open
 * 		and 1 in other case
 * Parameters:
 * 		- char * nameScript
 * Output:
 *		-  int
 * creation date:
 * 		- 09/14/23
 * last update:
 * 		- 09/15/23
 * 
 */
int processScriptFile(char *nameScript)
{
	FILE *scriptFile;
	scriptFile = openScriptFile(nameScript);
	if (scriptFile==NULL)
	{
		return 0;
	}	
	else	
	{
		char line[BUF_MAX_LINE];
		char breakLine[BUF_MAX_LINE];
		char msg[BUF_MAX_LINE];
		int lineNumber = 0;
		int lineNumberBreakLine = 0;
		void * handleLibrary = NULL;
		int changeCommand = 0;
		int bLineBreak=0;
		int bCurrentLineBreak = 0; // prueba 01
		while(fgets(line, BUF_MAX_LINE, scriptFile)!=NULL)
		{	
			lineNumber++;
			char *pLine = line; 
			if (!bLineBreak)
			{
				strcpy(line, ltrim(rtrim(pLine)));
			}	
			
			strcpy(line, rtrim(pLine));
			
			switch(*pLine)
			{	
				case '#':
				case ';':
					
					sprintf(msg, "line [%d]: '%s'\n   skip comment line\n", 
					        lineNumber, pLine);
					addLog(msg);
					break;
				case '\0':
				case '\n':
					sprintf(msg, "line [%d]: '%s'\n   skip empty line or return\n", 
					        lineNumber, pLine);
					addLog(msg);
					break;
				default:

					char commands[2][BUF_MAX_LINE];
					strcpy(line, processComments(pLine));
					int lenLine = strlen(line); 
					if (line[lenLine-1] == '\\')
					{
						line[lenLine-1] = '\0';
						if (bLineBreak==0)
						{	
							// do this if there is not a previous breakline
							strcpy(breakLine,line);
							lineNumberBreakLine = lineNumber;
							bLineBreak = 1;
							//bCurrentLineBreak = 1; // prueba 01
							continue;
						}	
						else
						{
							bCurrentLineBreak = 1; // prueba 01
						}	
					}	
					
					int bValidCommand = getInstructions(ltrim(pLine),
					                                  lineNumber,commands);
					if(bLineBreak)
					{
						if ((bValidCommand)&&
						    ((strcmp(commands[0],"use")==0) 
						     || (strcmp(commands[0],"call")==0)))
						{
							// break line has finished
							// process break line
							char *pLine1 = breakLine;
							sprintf(msg, "line [%d]: '%s'\n   processing\n", 
							        lineNumberBreakLine, pLine1);
							addLog(msg);
							handleLibrary = processLine(pLine1, 
							                            lineNumberBreakLine, 
						                            	handleLibrary);
							/* prueba 01 */
							if (bCurrentLineBreak)
							{
								strcpy(breakLine,line);
								lineNumberBreakLine = lineNumber;
								bLineBreak = 1;
								bCurrentLineBreak = 0;
								continue;
							}	
							/*prueba 01*/
							bLineBreak = 0;
						}
						else
						{
							if((strlen(line)+strlen(breakLine))<BUF_MAX_LINE)
							{	
								strcat(breakLine,line);
							}
							else
							{
								printf("line exceed the limit %d", 
								       BUF_MAX_LINE);
								return 0;	
							}	
						}	
					}

					if (!bLineBreak)	
					{
						sprintf(msg, "line [%d]: '%s'\n   processing\n",
						        lineNumber, pLine);
						addLog(msg);
						if(bValidCommand)
						{	
							handleLibrary = executeCommand(commands, 
							                               handleLibrary, 
						                        		   lineNumber);
						}
					}	
					break;
			}
		}
		if(bLineBreak)
		{
			char *pLine1 = breakLine;
			handleLibrary = processLine(pLine1, 
			                            lineNumberBreakLine, 
		                            	handleLibrary);
			bLineBreak = 0;
		}	
		// if there is loaded library we need to release first 
		// and then load the next library
		if (handleLibrary)
		{
			closeLibrary(handleLibrary);
		}

		closeScriptFile(scriptFile);
	}
	return 1;
	
}

/* 
 * Function: 
 *		- processCommandLine
 * Description:
 * 		this function is called when 
 * 		the program receive parameters:
 *	 		--use=[LIBRARY] 
 *			--call=[FUNCTION]
 * 		and proecess that information
 * Parameters:
 * 		- char * library
 * 		- char * function
 * Output:
 *		-  void
 * creation date:
 * 		- 09/14/23
 * 
 */
void processCommandLine(char *library, char *function)
{
	int lineNumber = 0;
	char msg[BUF_MAX_LINE];
	void * handleLibrary = NULL;
	if (library!=NULL)
	{
		printf("use: \n");
		
		//pLine = inputLibrary;
		sprintf(msg, "   use library %s\n", library);
		addLog(msg);
		if((handleLibrary = loadLibrary(library))==NULL)
		{
			//pLine = inputLibrary;
			sprintf(msg, "\tline: %d, couldn't load library %s\n", 
				    lineNumber, library);
			addLog(msg);
			printf("    file not found\n");
		}	
	}
	if (function!=NULL)
	{
		printf("call: \n");
		
		if(handleLibrary)
		{			
			//pLine = inputFunction;
			sprintf(msg, "   call function %s\n", function);
			addLog(msg);
			if (!callFunction(handleLibrary, function))
			{
				//pLine = inputFunction;
				sprintf(msg, "\tline: %d, couldn't call function %s\n", 
						lineNumber, function);
				addLog(msg);
			}	
		}
		else
		{
			sprintf(msg, "\tline: %d, librarycouldn't be loaded\n", 
						lineNumber);
			addLog(msg);
			printf("    the library is not loaded\n");	
		}	
	}
	
}

/* 
 * Function: 
 *		- processInteractiveMode
 * Description:
 * 		this function is called when 
 * 		the program receive parameters:
 *	 		--interactive-mode | -i 
 * 		and run the program on interactive mode
 * 		when the program is run this way will ask for a library and if it 
 *		is loaded good then will ask for a function to call
 * Parameters:
 * 		- 
 * Output:
 *		-  void
 * creation date:
 * 		- 09/14/23
 * 
 */
void processInteractiveMode()
{
	char inputLibrary[BUF_MAX_LINE];
	char inputFunction[BUF_MAX_LINE];
	char msg[BUF_MAX_LINE];
	char *pLine;  
	void * handleLibrary = NULL;
	int lineNumber = 0;
	int bQuit = 0;
	do
	{
		printf("quit(q) if you whant to exit\n"); 
		printf("back(b) if you want return to the previous menu\n"); 

		printf("use: \n");
		memset(inputLibrary,'\0', BUF_MAX_LINE);
		scanf("%s", inputLibrary);

		if ((strcmp(inputLibrary, "quit")==0) ||
		    (strcmp(inputLibrary, "q")==0))
		{
			addLog("program leaving\n");
			bQuit=1;
			break;
		}

		pLine = inputLibrary;
		sprintf(msg, "   use library %s\n", pLine);
		addLog(msg);
		if((handleLibrary = loadLibrary(inputLibrary))==NULL)
		{
			//pLine = inputLibrary;
			sprintf(msg, "\tline: %d, couldn't load library %s\n", 
				    lineNumber, pLine);
			addLog(msg);
			printf("    file not found\n");
		}	
		
		int bBack = 0;
		while((handleLibrary!=NULL) && (bBack==0) &&
		      (bQuit != 1))
		{
			printf("call: \n");
			memset(inputFunction,'\0', BUF_MAX_LINE);
			scanf("%s", inputFunction);

			if ((strcmp(inputFunction, "back")==0) ||
			    (strcmp(inputFunction, "b")==0))
			{
				addLog("returning back\n");
				bBack=1;
				break;
			}	

			if ((strcmp(inputFunction, "quit")==0) ||
			    (strcmp(inputFunction, "q")==0))
			{
				addLog("program leaving\n");
				bQuit=1;
				break;
			}
			
			if(handleLibrary)
			{			
				pLine = inputFunction;
				sprintf(msg, "   call function %s\n", pLine);
				addLog(msg);
				if (!callFunction(handleLibrary, inputFunction))
				{
					//pLine = inputFunction;
					sprintf(msg, "\tline: %d, couldn't call function %s\n", 
							lineNumber, pLine);
					addLog(msg);
				}	
			}
			else
			{
				sprintf(msg, "\tline: %d, librarycouldn't be loaded\n", 
							lineNumber);
				addLog(msg);
				printf("    the library is not loaded\n");	
			}	
		}
			
	}while(bQuit!=1);

	if (handleLibrary)
	{
		closeLibrary(handleLibrary);
	}	
}


/* 
 * main program: 
 *		- libraryTest.c
 * Description:
 * 		this program can load a library and call a function in run time   
 * 		can be executed from 3 ways
 *	 	normal | interactive mode | command line
 * 		normal: 
 * 			./shareLibraryTest file.sc
 * 				where file.sc is a script with instructions to load libraries
 *				and call function once a library is loaded
 * 		interactive mode
 *			./shareLibraryTest -i | --interactive-mode
 *				the program will execute asking for a library to load
 * 				if the library can be loaded then the program will ask you 
 * 				for a function to call
 * 			* if you want to return for a menu enter b | back
 * 			* if you want to return for a menu enter q | quit
 * 		command line
 * 			./shareLibraryTest use=library1.so --call=function1
 * 				using this way the program will try to load
 * 				library1.so and will try to call function1
 * 				the results will be displayed in the screen
 * 		in every moment you can review the log file with mode information 
 *		about every run. the log file has the same name like the program but 
 *		with .log extension like:  shareLibraryTest.log
 * 			
 * creation date:
 * 		- 09/12/23
 * last update:
 * 		- 09/14/23
 * 		- 09/15/23
 */
int main(int argc, char *argv[])
{
	if (argc<2)
	{
		printf("see --help option\n");
		return EXIT_FAILURE;
	}	

	if (strcmp(argv[1], "--help")==0)
	{
		printf("testShareLibrary program\n");
		printf("\n\t-i\t--interactive-mode\tto run as interactive mode");
		printf("\n\t-u\t--use=[LIBRARY]\t\tto load a library in command line");
		printf("\n\t-c\t--call=[FUNCTION]\tto call a function in command line");
		printf("\n\t[FILE]\tto execute a script file to load libraries and call functions");
		printf("\n\n");
		return EXIT_SUCCESS;
	}    
	    
	startLog(argv[0]);
	int i=0;
	if ((strcmp(argv[1], "--interactive-mode")==0) ||
	    (strcmp(argv[1], "-i")==0))
	{
		addLog("starting interactive mode\n");
		processInteractiveMode();
	} 
	else
	{   
		char inputLibrary[BUF_MAX_LINE];
		char inputFunction[BUF_MAX_LINE];
		int bCommandLine=0;
		char *pLineLib = NULL;
		char *pLineFun = NULL;
		for (int i=1; i<argc; i++)
		{
			if (strncmp(argv[i],"--use",5)==0)
			{
				strcpy(inputLibrary,argv[i]); 
				bCommandLine = 1;	
			}
			if(strncmp(argv[i],"--call",5)==0)
			{
				strcpy(inputFunction,argv[i]);
				bCommandLine = 1;
			}	
			if(strncmp(argv[i],"-u",2)==0)
			{
				if ((i+1)>=argc)
				{
					return EXIT_FAILURE;
				}
				pLineLib = argv[++i];
				bCommandLine = 1;	
			}	
			if(strncmp(argv[i],"-c",2)==0)
			{
				if ((i+1)>=argc)
				{
					return EXIT_FAILURE;
				}
				pLineFun = argv[++i];
				bCommandLine = 1;	
			}	
		}	
		if (bCommandLine)
		{
			//char *pLineLib;
			//char *pLineFun;
			printf("starting command line \n");
			if (pLineLib==NULL)
			{	
				pLineLib = getParamValue(inputLibrary);
			}	
			if (pLineFun==NULL)
			{	
				pLineFun = getParamValue(inputFunction);
			}	

			printf("library: %s, function: %s\n", pLineLib, pLineFun);
			processCommandLine(pLineLib, pLineFun);
		}
		else
		{	
			addLog("starting normal mode\n");
			if(!processScriptFile(argv[1]))
			{
				closeLog();
				return EXIT_FAILURE;
			}
		}	
	}
	closeLog();

	return EXIT_SUCCESS;
}
