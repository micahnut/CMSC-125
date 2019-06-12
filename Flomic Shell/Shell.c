#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <math.h>
#include <windows.h>
#include <limits.h>
#include <stdlib.h>
#include <dos.h>
#include <conio.h>

#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif
#define _WIN32_WINNT 0x0501

void execute(DIR* , char []);
void receive_input(char []);
void cd_cmd(char *, char []);
void date_cmd(char *);
void time_cmd(char *);


void receive_input( char command[] ){
    gets(command);
    //scanf("%[^\n]", command);
}


void clearscreen(){
	
  // Assembly variables declaration
  HANDLE                     hStdOut;
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  DWORD                      count;
  DWORD                      cellCount;
  COORD                      homeCoords = { 0, 0 };

  hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );
  if (hStdOut == INVALID_HANDLE_VALUE) return;

  // Get number of cells in the current buffer
  if (!GetConsoleScreenBufferInfo( hStdOut, &csbi )) return;
  cellCount = csbi.dwSize.X *csbi.dwSize.Y;

  // Fill the entire buffer with spaces
  if (!FillConsoleOutputCharacter(
    hStdOut,
    (TCHAR) ' ',
    cellCount,
    homeCoords,
    &count
    )) return;

  // Fill the entire buffer with the current colors and attributes
  if (!FillConsoleOutputAttribute(
    hStdOut,
    csbi.wAttributes,
    cellCount,
    homeCoords,
    &count
    )) return;

  // Move the cursor to home coordinates
  SetConsoleCursorPosition( hStdOut, homeCoords );
}

char *remove_white_space(char  *str){
	char *end;
	
	//trim leading space
	while(isspace((unsigned char)*str))
		str++;
		
	if(*str == 0)  //All spaces?
		return str;	
	
	//Trim trailing space
	end = str + strlen(str) - 1;
	while(end > str && isspace((unsigned char)*end)) 
		end--;

  	// Write new null terminator character
  	end[1] = '\0';

  return str;
} /*source stackoverflow*/



/*
*	Funtion no_of_tokens
*	----------------------------------------------------
*  returns the number of tokens in a string
*/
int countArguments(char *command) { //if not yet tokenized
	char s[strlen(command)];
	char* token;
	int count = 0;

	strcpy(s, command);
	token = strtok(s, " ");
	while (token ) {
    	token = strtok(NULL, " ");
		count++;
	}
	return count;
}

void printTokens(char *token){
	char *tokens = token;
	while(tokens!=NULL){
		 printf ("%s\n",tokens);
    	 tokens = strtok (NULL, " ");
	}
}

void cd_cmd(char *command, char *tokens){
	//printTokens(tokens);
	int arguments = countArguments(command);
	arguments = arguments -1;
	//printf("number of args: %d \n", arguments);
	
	
	char *destination = command + strlen(tokens) + 1;  // Extract destination string token
  	//printf ("destination: %s\n",destination);
  	
  	if(arguments==0){
  		char cwd[1024];
   		getcwd(cwd, sizeof(cwd));                 // get current working directory
    	printf("%s\n", cwd);                      // print current working directory
	}else if(arguments>1){
		printf("cannot be");
	}else {
		if(chdir(remove_white_space(destination)) == -1) 
    	  	printf("The system cannot find the path specified.\n");   // System error on unlocatable path
  	}

}

void chdir_cmd(char *command, char *tokens) {
 	//printTokens(token);
	int arguments = countArguments(command);
	arguments = arguments -1;
	//printf("number of args: %d \n", arguments);
	
	
	char *destination = command + strlen(tokens) + 1;  // Extract destination string token
  	//printf ("destination: %s\n",destination);
  	
  	if(arguments==0){
  		char cwd[1024];
   		getcwd(cwd, sizeof(cwd));                 // get current working directory
    	printf("%s\n", cwd);                      // print current working directory
	}else if(arguments>1){
		printf("cannot be");
	}else {
		if(chdir(remove_white_space(destination)) == -1) 
    	  	printf("The system cannot find the path specified.\n");   // System error on unlocatable path
  	}
}



/*Essentially both of them do the same thing, 
but chdir is a POSIX system call while cd is 
a normal function used in a program which in 
this case is the shell. In practice, 
chdir is called by cd to make the change in directory 
since the program does not have kernel privileges to do it by itself.*/



void make_directory(char *command, char *tokens){ 
	//printTokens(command);
	char *folder = command + strlen(tokens) + 1;
    int arguments = countArguments(command);
    arguments = arguments -1;
	//printf("number of args: %d \n", arguments);
	
	if(arguments<1){
		printf("usage: mkdir [-pv] [-m mode] directory ...\n");
	}else{
		    if(mkdir(folder) == -1) {
				 printf("A subdirectory or file %s already exists.\n", folder);
			}
		}
	
}


void date_cmd(char *command){ 
	//printTokens(command);



		char *month_text[] = {"","January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
    	char *day_text[] = {"","Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
    	SYSTEMTIME time_;

        GetSystemTime(&time_);
        int year = time_.wYear;
        int month = time_.wMonth;
        int day = time_.wDay;
        int week;

        //calculate what day it is 
        week =  ( (day+=(month <3? year -- :(year -2))), (23* month/9+ day +4 + year /4- year /100+ year/400) ) % 7;
        printf("The current date is: %s, %s-%d-%d \n",day_text[week],month_text[time_.wMonth],time_.wDay,time_.wYear);
		
		 //set date
        char *new_date[10];
        printf("Enter the new date: (mm-dd-yyyy): ");
        scanf("%s", new_date);
        SetLocalTime(new_date);
        
        //default response from Windows Command Prompt
    	printf("A required privilege is not held by the client\n");
		
}


void time_cmd(char *command){
    char *day_text[] = {"","Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
    SYSTEMTIME time_;

    
        //show time
        GetLocalTime(&time_);
        printf("Current time is: %d : %d : %d\n",time_.wHour,time_.wMinute,time_.wSecond);
  
        //set time
        char *new_time[10];
        printf("Enter the new time: (hh:mm:ss): ");
        scanf("%s", new_time);
        SetLocalTime(new_time);
        
        
        //default response from Windows Command Prompt
        printf("A required privilege is not held by the client\n");

}



int _del(char **args){
	if (args[1] == NULL){
		printf("no argument(s) found");
		
	}else {
		
	}
}


void help_cmd(){
    printf("For more information on a specific command, type HELP command-name\n");
    printf("CD\tDisplays the name of or changes the current directory.\n");
    printf("CHDIR\tChanges the current directory.\n");
    printf("CLS\tClears the screen.\n");
    printf("CMD\tStarts a new instance of the command interpreter.\n");
    printf("COLOR\tSets the default console foreground and background colors.\n");
    printf("COPY\tCopies one or more files to another location.\n");
    printf("DATE\tDisplays or sets the date.\n");
    printf("DEL\tDeletes one or more files.\n");
    printf("DIR\tDisplays a list of files and subdirectories in a directory.\n");
    printf("HELP\tProvides Help information for Windows commands.\n");
    printf("MKDIR\tCreates a directory.\n");
    printf("MOVE\tMoves one or more files from one directory to another directory.\n");
    printf("RENAME\tRenames a file or files.\n");
    printf("RMDIR\tRemoves an empty directory.\n");
    printf("TIME\tDisplays or sets the system time.\n");
    printf("TYPE\tDisplays the contents of a text file.\n");

}

void color_cmd(char *tokens,char *command){
	printTokens(command);
	int arguments = countArguments(command);
    arguments = arguments -1;
	char *clr = command + strlen(tokens);
	//printf("%d", arguments); 
	char color[50]="";

    if(arguments<1){
    	printf("wrong usage \n");
        
        return ;
    }else{

	    strcat(color, command);
	    strcat(color, " ");
	    strcat(color, clr);
	    printf("%s", color);
	
	    system(color);
	}
    return ;
}
void type_cmd(char* command, char* token){
	printTokens(command);
	
	char *cmd_filename = command + strlen(token) + 1;
	FILE *file;
    char c;
    
	file = fopen(cmd_filename, "r");
	  if (file == NULL)
        {
            printf("Error: Unable to open %s \n",cmd_filename);
            
        }

        // Read contents from file
        c = fgetc(file);
        while (c != EOF)
        {
            printf ("%c", c);
            c = fgetc(file);
        }
        printf("\n");

        fclose(file);
	 
}

/* Delete files command or 'del'
 *
 * Params: char*
 */
void delete_cmd(char *cmd) {
  char *cmd_filename = strtok(NULL, " ");

  // Check if user specified a file to be deleted
  if(cmd_filename == NULL) {
    printf("The syntax of the command is incorrect.\n");
  } else {

    char *temp_fn = cmd_filename;
    int count = 1;
    int length = strlen(temp_fn);
    int running = 1;

    int delete_count = 0;

    // Count how many files are to be deleted
    while(running) {
      temp_fn = temp_fn + length + 1;
      length = length + strlen(temp_fn);

      if ((temp_fn = strtok(NULL, " ")) == NULL) {
        running = 0;
      } else {
        ++count;
      }
    }

    // Traverse through the files to be deleted
    while (count) {
      FILE *file;
      file = fopen(cmd_filename, "r");

      // Remove each file
      if(file) {
        fclose(file);
        int remove_success = remove(cmd_filename);

        if(remove_success == 0) {
          ++delete_count;
        } else {
          printf("Could not delete file %s\n", cmd_filename);
        }
      } else {
        printf("Could Not Find %s \n", cmd_filename);
      }

      // Go to the next file
      cmd_filename = cmd_filename + strlen(cmd_filename) + 1;
      --count;
    }

    // Inform user how many files deleted
    printf("%d file(s) deleted.\n", delete_count);
  }

}

void deleteone_cmd(char* command, char* token){ //not yet sure if correct na current directory
	int status;
	//printTokens(command);
	
	char *cmd_filename = command + strlen(token) + 1;
	FILE *file;
	
	status = remove(cmd_filename);
 
	  if (status == 0)
	    printf("\n%s file deleted successfully.\n", cmd_filename);
	  else
	  {
	    printf("Unable to delete the file\n");
	    perror("Following error occurred");
	  }
	
    
}
char *extractFileName(char *command){
  int filename_idx = 0;
  int i = strlen(command) - 1;
  char filename[128];

  // Extract index to the last slash
  for (; i >= 0; i--){
    if(command[i] == '/') {
      filename_idx = i;
      break;
    }
  }

  // Retrieve filename of the input file path
  // starting at the identified starting index
  i = filename_idx;
  int j = 0;
  for(; i < strlen(command); i++, j++) {
    filename[j] = command[i];
  }

  // Add null character terminator
  filename[j] = '\0';

  return filename;
}

/*function to check if file name exists in a path
 *
 * Params: char*
 */
int is_file_specific (char *command) {
  int i = 0;

  // Traverse through file path and determine if file name is included
  for (; i < strlen(command); i++) {
    if(command[i] == '.') {
      return 1;
    }
  }
  return 0;
}

/* //if text file to text file only
void copy_cmd(char* command, char* token){
   //printTokens(command);
   int arguments = countArguments(command);
   arguments = arguments -1;
   
   char ch;
   FILE *source, *target;
   int i=0;
   char *commandtemp[5];
   
   if(arguments <2){
   		printf("\nError! Syntax must be [copy]{space}<source_file>{space}<destination_file>\n");
   }
   else{
   
		   commandtemp[0]= strtok (command, " ");
		   
		   while(commandtemp[i]!=NULL){
		   		i++;
		   		commandtemp[i] = strtok (NULL, " ");
		   }
		   
		   char *source_file = commandtemp[1];
		   
		   printf("\nsource: %s \n", source_file);
		   source = fopen(source_file, "r");
		 
		   if (source == NULL)
		   {
		      printf("error while copying to file\n");
		   }
		 
		 

		   char *target_file = commandtemp[2];
		   printf("\ntarget: %s \n", target_file);
		        
		   target = fopen(target_file, "w");
		 
		   if (target == NULL)
		   {
		      fclose(source);
		       printf("error while copying to folder\n");
		   }
		 
		   while ((ch = fgetc(source)) != EOF)
		      fputc(ch, target);
		 
		   printf("File copied successfully.\n");
   }
 
   fclose(source);
   fclose(target);
}*/

void copy_cmd(char *command, char* token) {
  char *source = strtok(NULL, " ");
  char *target = source + strlen(source) + 1;
	
  printf("\nsource: %s \n", source);
  printf("\ntarget: %s \n", target);
  
  target = strtok(NULL, " ");

  // Check if args exist
  if(source == NULL) {
    printf("The syntax of the command is incorrect.\n");

  } else if (target == NULL){
    printf("The file cannot be copied onto itself.\n");

  } else {
  	
  	
  	
    char ch;
    FILE *source_file, *dest_file;

    // Open source file and copy the contents
    source_file = fopen(source, "r");

    if(source_file == NULL) {
      printf("The system cannot find the file specified.\n");
    } else {
      if(strcmp(source, target) == 0) {
        printf("The file cannot be copied onto itself.\n");
      } else {

        // Open source file or create source file
        if (!is_file_specific(target)) {
          char *new_file = extractFileName(source);

          if (target[strlen(target) - 1] == '/') {
            strcat(target, new_file);
          } else {
            strcat(target, "/");
            strcat(target, new_file);
          }
        }

        // Copy the contents of source file onto the destination file
        dest_file = fopen(target, "w");
        ch = fgetc(source_file);
        while (ch != EOF) {
          fputc(ch, dest_file);
          ch = fgetc(source_file);
        }
        printf("1 file(s) copied.\n");
        fclose(dest_file);
      }
      fclose(source_file);
    }
  }
}

int copy3_cmd(char *src, char* dest) {
  char *source = src;
  char *target = dest;
	
  printf("\nsource: %s \n", source);
  printf("\ntarget: %s \n", target);
   
    char ch;
    FILE *source_file, *dest_file;

    // Open source file and copy the contents
    source_file = fopen(source, "r");

    if(source_file == NULL) {
      printf("The system cannot find the file specified.\n");
    } else {
      if(strcmp(source, target) == 0) {
        printf("The file cannot be copied onto itself.\n");
      } else {

        // Open source file or create source file
        if (!is_file_specific(target)) {
          char *new_file =  extractFileName(source);

          if (target[strlen(target) - 1] == '/') {
            strcat(target, new_file);
          } else {
            strcat(target, "/");
            strcat(target, new_file);
          }
        }

        // Copy the contents of source file onto the destination file
        dest_file = fopen(target, "w");
       /* ch = fgetc(source_file);
        while (ch != EOF) {
          fputc(ch, dest_file);
          ch = fgetc(source_file);
          printf("%c\n", ch);
        }*/
         while ((ch = fgetc(source_file)) != EOF)
		      fputc(ch, dest_file);
        //printf("1 file(s) copied.\n");
        fclose(dest_file);
      }
      fclose(source_file);
    }
   return 1;  
}
/*
void copy2_cmd(char *command, char* token) {
  char *cmd_filename = strtok(NULL, " ");
  char *target =cmd_filename + strlen(cmd_filename) + 1;
	
  printf("\nsource: %s \n", cmd_filename);
  printf("\ntarget: %s \n", target);
  
  target = strtok(NULL, " ");

  // Check if args exist
  if(cmd_filename == NULL) {
    printf("The syntax of the command is incorrect.\n");

  } else if (target == NULL){
    printf("The file cannot be copied onto itself.\n");

  } else {
  	
  	char *temp_fn = strtok(cmd_filename, "+");
    char target_copy[500];
  	strcpy(target_copy, target);
	strcat(target_copy, " ");
	
  	int count =1;
  	int length = strlen(temp_fn);
  	int running = 1;
  	int copy_count =0;
  	
  	// Count how many files are to be deleted
    while(running) {
      temp_fn = temp_fn + length + 1;
      length = length + strlen(temp_fn);

      if ((temp_fn = strtok(NULL, " ")) == NULL) {
        running = 0;
      } else {
        ++count;
      }
    }
    char *temp_cmdfilename = strtok(cmd_filename, "+");
  	int length2 = strlen(temp_cmdfilename);
  	 // Traverse through the files to be deleted
    while (count) {
    	// temp_cmdfilename= temp_cmdfilename + length2 + 1;
         // length2 = length2 + strlen(temp_cmdfilename);
          
      FILE *file;
      file = fopen(temp_cmdfilename, "r");

      if(file) {
        //fclose(file);
        int copy_success = copy3_cmd(temp_cmdfilename,target);

        if(copy_success == 1) {
          ++copy_count;
        } else {
          printf("Could not delete file %s\n", temp_cmdfilename);
        }
      } else {
        printf("Could Not Find %s \n", temp_cmdfilename);
      }

      // Go to the next file
      temp_cmdfilename = temp_cmdfilename + strlen(temp_cmdfilename) + 1;
      target = target_copy;
      --count;
    }
    
    // Inform user how many files deleted
    printf("\n%d file(s) copied.\n", copy_count);
  	
  }
  
  
}*/


void rename_cmd(char* command, char* token){
   printTokens(command);
   int arguments = countArguments(command);
   arguments = arguments -1;
   int result;
   int i=0;
   char *commandtemp[5];
   commandtemp[0]= strtok (command, " ");
		   
		   while(commandtemp[i]!=NULL){
		   		i++;
		   		commandtemp[i] = strtok (NULL, " ");
		   }
   
	char *oldname = commandtemp[1];
	char *newname = commandtemp[2];
	
	result = rename(oldname,newname);
	
	if ( result == 0 )
   	 printf ( "File successfully renamed \n" );
 	else
     perror( "Error renaming file \n" );
}
void dir_cmd(DIR *dir){
	 struct dirent *de;  // Pointer for directory entry 
    // opendir() returns a pointer of DIR type.  
    dir = opendir("."); 
    struct stat attr;
	  char buff[20];
	  struct tm *time_info;
	  int file_count = 0;
      int dir_count = 0;
    if (dir == NULL)  // opendir returns NULL if couldn't open directory 
    { 
        printf("Could not open current directory" ); 
    } 
  	else{

    while ((de = readdir(dir)) != NULL){
	
         stat(de->d_name, &attr);
         time_info = localtime(&attr.st_mtime);
         strftime(buff, sizeof(buff), "%m/%d/%Y %I:%M %p", time_info);
         printf("%s", buff);
		
		// If directory element is a directory
      if(S_ISDIR(attr.st_mode)) {
        printf("\t\t  <DIR>\t");
      } else {
        int count = (int)log10(attr.st_size) + 1;
        count += (count-1)/3;

        // Routine to adjust spaces and tabs
        // and to keep directory view neat
        int i = 0;
        for (; i < 20-count; i++) {
            printf(" ");
        }

        // Call misc function to properly print integers
        formatInteger((unsigned)attr.st_size);
        printf("\t");
      }
		
		
		printf("\t%s\n", de->d_name); 
    	}
    	closedir(dir);

	}
/*	
	  struct dirent *dirnt;
  dir = opendir(".");

  struct stat attr;
  char buff[20];
  struct tm *time_info;

  int file_count = 0;
  int dir_count = 0;

  // If directory exists
  if (dir) {
    dirnt = readdir(dir);

    // Traverse directory entry
    while (dirnt != NULL) {
      stat(dirnt->d_name, &attr);
      time_info = localtime(&attr.st_mtime);
      strftime(buff, sizeof(buff), "%m/%d/%Y %I:%M %p", time_info);
      printf("%s", buff);

      // If directory element is a directory
      if(S_ISDIR(attr.st_mode)) {
        printf("\t\t  <DIR>\t");
      } else {
        int count = (int)log10(attr.st_size) + 1;
        count += (count-1)/3;

        // Routine to adjust spaces and tabs
        // and to keep directory view neat
        int i = 0;
        for (; i < 20-count; i++) {
            printf(" ");
        }

        // Call misc function to properly print integers
        formatInteger((unsigned)attr.st_size);
        printf("\t");
      }

      // Move to the next directory entry and print name
      printf("\t%s\n", dirnt->d_name);

      dirnt = readdir(dir);
    }
    closedir(dir);
  } else {
    perror("Error: ");
  }*/
}

/* function for formatting integer printing
 *
 * Params: unsigned int
 */
void formatInteger (unsigned n) {

  // Format negative numbers accordingly
  if (n < 0) {
    printf ("-");
    n = -n;
  }
  // Call child misc function
  formatIntegerFigures(n);

}

/* recursive function for formatting integer printing
 *
 * Base case: n < 1000
 * Params: unsigned int
 */
void formatIntegerFigures (unsigned n) {

  // Format >999 integers in recursive manner
  if (n < 1000) {
    printf("%d", n);
    return;
  }

  // Print appropriate commas
  formatIntegerFigures(n/1000);
  printf(",%03d", n%1000);
}

void remove_directory(char* command, char* token){
	char *destination = command + strlen(token) + 1;
  DIR *dir = opendir(destination);

  // check if directory to be deleted exists
  if (dir != NULL) {
    closedir(dir);
    int response = rmdir(destination);

    // if nonzero value was returned,
    // rmdir was not successfully executed
    if (response) {
      printf("The directory is not empty.\n\n");
    } else {
      printf("Successfully removed directory.\n");
    }
  } else {
    printf("The system cannot find the file specified.\n");
  }
}

/* //if move one file to next subfolder format only 
void move(char* command, char* token){
	
   int i=0;
   char *commandtemp[5];
   commandtemp[0]= strtok (command, " ");
		   
		   while(commandtemp[i]!=NULL){
		   		i++;
		   		commandtemp[i] = strtok (NULL, " ");
		   }
   
	char *source = commandtemp[1];
	char *destination = commandtemp[2];

  // If user provided source file and destination path
  if (source == NULL && destination == NULL) {
    printf("The syntax of the command is incorrect.\n");
  } else {
    char ch;
    FILE *source_file, *dest_file;

    source_file = fopen(source, "r");

    if(source_file == NULL) {
      printf("The system cannot find the file specified.\n");
    } else {

      // Copy the file to another directory
      char *new_file = source;
	
      // Concatenate file name to the destination path
      if (destination[strlen(destination) - 1] == '/') {
        strcat(destination, new_file);
      } else {
        strcat(destination, "/");
        strcat(destination, new_file);
      }

      // Create destination file
      dest_file = fopen(destination, "w");

      // Copy contents of source file to destination file
      ch = fgetc(source_file);
      while (ch != EOF) {
        fputc(ch, dest_file);
        ch = fgetc(source_file);
      }

      fclose(dest_file);
      fclose(source_file);

      // Delete the file in original directory
      int remove_success = remove(source);

      if(remove_success == 0) {
        printf("1 file(s) moved.\n");
      } else {
        printf("Could not delete file %s\n", source_file);
      }
    }
  }
}*/

void move_cmd(char *cmd) {
  char *cmd_src = strtok(NULL, " ");
  char *cmd_dest = cmd_src + strlen(cmd_src) + 1;

  cmd_dest = strtok(NULL, " ");

  // If user provided source file and destination path
  if (cmd_src == NULL && cmd_dest == NULL) {
    printf("The syntax of the command is incorrect.\n");
  } else {
    char ch;
    FILE *source_file, *dest_file;

    source_file = fopen(cmd_src, "r");

    if(source_file == NULL) {
      printf("The system cannot find the file specified.\n");
    } else {

      // Copy the file to another directory
      char *new_file = extractFileName(cmd_src);

      // Concatenate file name to the destination path
      if (cmd_dest[strlen(cmd_dest) - 1] == '/') {
        strcat(cmd_dest, new_file);
      } else {
        strcat(cmd_dest, "/");
        strcat(cmd_dest, new_file);
      }

      // Create destination file
      dest_file = fopen(cmd_dest, "w");

      // Copy contents of source file to destination file
      ch = fgetc(source_file);
      while (ch != EOF) {
        fputc(ch, dest_file);
        ch = fgetc(source_file);
      }

      fclose(dest_file);
      fclose(source_file);

      // Delete the file in original directory
      int remove_success = remove(cmd_src);

      if(remove_success == 0) {
        printf("1 file(s) moved.\n");
      } else {
        printf("Could not delete file %s\n", source_file);
      }
    }
  }
}




void execute(DIR *dir, char command[]){
	command = remove_white_space(command);
	char command_copy[500];
	strcpy(command_copy, command);
	strcat(command_copy, " ");
	
	char *token = strtok (command, " "); //tokenize
	
	//printTokens(token);
	//printf("after 1st print \n");
	
	if(strcmp(token, "exit")==0){ //finish
		exit(0);
	}
	else if(strcmp(token, "mkdir")==0){ //finish but without multiple folders
		make_directory(command_copy, token);
	}
	
	else if(strcmp(token, "rmdir") == 0){//chuim 
		remove_directory(command_copy,token);
	}
	else if(strcmp(token, "cd") == 0 ){ //finish
		cd_cmd(command_copy, token);
	}
	else if(strcmp(token, "cd..") == 0 ){ //finish
		chdir("..");
	}
	else if(strcmp(token, "chdir") == 0 ){ //finish not yet implemented
		cd_cmd(command_copy, token);
	}
	else if(strcmp(token, "chdir..") == 0 ){ //finish
		chdir("..");
	}
	else if(strcmp(token, "del") == 0 ){ //chuim or flo //finish
		delete_cmd(command_copy);
	}
	else if(strcmp(token, "rename") == 0 ){//chuim //finish
		rename_cmd(command_copy,token);
	}
	else if(strcmp(token, "move") == 0 ){//flo
		//move(command_copy, token);
		move_cmd(command);
	}
	else if(strcmp(token, "copy") == 0 ){//flo//finish
		 copy_cmd(command_copy, token);
		 //copy2_cmd(command_copy, token);
	}
	else if(strcmp(token, "cls") == 0 ){ //finish
		clearscreen();
	}
	else if(strcmp(token, "cmd") == 0 ){
		printf("Microsoft Windows [Version 10.0.15063]\n(c) 2018 Microsoft Corporation. All rights reserved.\n\n");
	}
	else if(strcmp(token, "date") == 0 ){//flo //finish
		date_cmd(command_copy);
	}
	else if(strcmp(token, "dir") == 0 ){ //chuim //not yet with time and all
		dir_cmd(dir);
	}
	else if(strcmp(token, "help") == 0 ){ //finish
		help_cmd();
	}
	else if(strcmp(token, "time") == 0 ){//flo//finish
		time_cmd(command_copy);
	}
	else if(strcmp(token, "type") == 0 ){//chuim //finish
		type_cmd(command_copy, token);
	}
	/*else if(strcmp(token, "color") == 0){ //chuim //to be added feature 
		//color_cmd(token, command_copy);
	}*/
	else{
		 printf("'%s' is not recognized as an internal or external command, operable program or batch file.\n", token);
	}
	
	
}



int main(){
	char command[1024];
	char cwd [1024];
	DIR *dir;
	
	printf("Microsoft Windows [Version 10.0.15063]\n(c) 2018 Microsoft Corporation. All rights reserved.\n\n");
	while(1){
		if(getcwd(cwd, sizeof(cwd))!= NULL){
		command[0] = '\0';
		printf("%s>", cwd);
        fflush(stdin);
		receive_input(command);
			if(command[0] != '\0'){
        		execute(dir,command);
      		}
  		}		
      else{
      	perror("Something went wrong. ");
            break;
	  }
	}
	
	return(0);
}



