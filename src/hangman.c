#include "../include/hangman.h"

char tolower(int c){
	if(c>='A'&&c<='Z'){
		return c-('A'-'a');
	}
	return c;
}

char toupper(int c){
	if(c>='a'&&c<='z'){
		return c+('A'-'a');
	}
	return c;
}

char isalpha(int c){
	if(c>='A'&&c<='z'){
		return 1;
	}
	return 0;
}

char fileLoc[500]; // The backup file location
void showLogo() {
      printf("HANGMAN\n");
}
void prn_galg(int i) {
 printf("Amount of wrong letters: %d\n\n", i);
 /*switch (i) {
     case 0 :
      printf("\n");
      printf("\n");
      printf("\n");
      printf("\n");
      printf("\n");
      printf("\n");
      printf("____________\n\n");
     break;
     case 1 :
      printf("\n");
      printf("  |\n");
      printf("  |\n");
      printf("  |\n");
      printf("  |\n");
      printf("  |\n");
      printf("__|_________\n\n");
     break;
     case 2 :
      printf("  _______\n");
      printf("  |\n");
      printf("  |\n");
      printf("  |\n");
      printf("  |\n");
      printf("  |\n");
      printf("__|_________\n\n");
     break;
     case 3 :
      printf("  _______\n");
      printf("  |/\n");
      printf("  |\n");
      printf("  |\n");
      printf("  |\n");
      printf("  |\n");
      printf("__|_________\n\n");
     break;
     case 4 :
      printf("  _______\n");
      printf("  |/   | \n");
      printf("  |    O \n");
      printf("  |\n");
      printf("  |\n");
      printf("  |\n");
      printf("__|_________\n\n");
     break;
     case 5 :
      printf("  _______\n");
      printf("  |/   | \n");
      printf("  |    O \n");
      printf("  |    |\n");
      printf("  |    |\n");
      printf("  |\n");
      printf("__|_________\n\n");
     break;
     case 6 :
      printf("  _______\n");
      printf("  |/   | \n");
      printf("  |    O \n");
      printf("  |   \\|\n");
      printf("  |    | \n");
      printf("  |\n");
      printf("__|_________\n\n");
     break;
     case 7 :
      printf("  _______\n");
      printf("  |/   | \n");
      printf("  |    O \n");
      printf("  |   \\|/\n");
      printf("  |    | \n");
      printf("  |\n");
      printf("__|_________\n\n");
     break;
     case 8 :
      printf("  _______\n");
      printf("  |/   | \n");
      printf("  |    O \n");
      printf("  |   \\|/\n");
      printf("  |    | \n");
      printf("  |   /\n");
      printf("__|_________\n\n");
     break;
     case 9 :
      printf("  _______\n");
      printf("  |/   | \n");
      printf("  |    O \n");
      printf("  |   \\|/\n");
      printf("  |    | \n");
      printf("  |   / \\\n");
      printf("__|_________\n\n");
     break;
     case 10 :
      printf("  _______\n");
      printf("  |/   | \n");
      printf("  |    X \n");
      printf("  |   \\|/\n");
      printf("  |    | \n");
      printf("  |   / \\\n");
      printf("__|_________\n\n");
     break;
 }*/
}
char randomNumber(int max_number) {
srand(getmin());
int g = (rand() % (max_number + 1));
return g;
}
char *getWord() {
  char c[512];  /* declare a char array */
  msg_t msg;
  msg.command=21;
  msg.argc=3;
  msg.argv[0]="hangman/words.conf";
  msg.argv[1]=(void*)0;
  msg.argv[2]=c;
  msgWrite(&msg);
  /* Opening words file */
  
  /* Incase the file cant be openend */
  if(c[0]==NULL) {
    printf("An error has occured: can't open words file.\nPlease create this file at /hangman/words.conf\n");
    return 0;
  }
    /* Separating the contents, divided by | and declaring variables */
    char words[20][25];
    char *final=malloc(25);
    int f = 0, i=0, z=0;
    while(c[i])
    {
		if(c[i]=='\n'){
			words[f][z]=0;
			//printf("word: %s\n",words[f]);
			f++;
			z=0;
		}
		else{
			words[f][z++]=c[i];
		}
		i++;
    }
    /* Retrieving a random number */
    int wordN = randomNumber(f);
    strcpy(final, words[wordN]);
    return final;
}
int hangMain(void) {
//char udi[10];
//int flag=1;
//strcpy(udi, "EMPTY");
//while (flag) {
// getchar();
// strcpy(udi, "EMPTY");
 /* Declaring variables */
 /* Random deciding which word is chosen to be guessed:
 guessWord is the word that needs to be guessed
 currentWord is the word that is filled with dots */
 /* Retrieving the word that matches with the wordNumber */
 /* Check which number was chosen: printf("%d", wordNumber); */
 char *tempWord = getWord();
 if(tempWord==NULL){
	 return 0;
 }
 /* Declaring the guessWord with the length of dkljafoue */
 char guessWord[strlen(tempWord)+1];
 /* Copying the string of dkljafoue into guessWord */
 strcpy(guessWord, tempWord);
 /* Freeing the pointer */
 free(tempWord);
 /* Calculate the length of the guessWord */
 int wordlength = strlen(guessWord);
 /* Creating the dotword (name: currentWord) */
 char* currentWord = malloc( wordlength );
 int t;
 for (t = 0; t <= wordlength; t++) {
  if (t == wordlength) {
      currentWord[t] = '\0';
  } else {
      currentWord[t] =  '.';
  }
 }
 /* Currentword check: printf("Currentword: \"%s\"", currentWord); */
 /* Declar: %ing variables */
 int errors = 0; /* Error amount, if its higher than 10 the loop stops */
 int guessedLetter = 0; /* Boolean Integer used to check wether the player entered a correct letter 0 = false, 1 = true */
 int i,n = 1;
 char c;
 /* Printing logo */
 showLogo();
 /* Printing introduction message */
 printf("%s\n\n%s\n%s\n%s\n%s\n\n%s%s\n\n",
 "Welcome to the game Hangman!",
 "The objective in this game is to guess the word.",
 "You can enter both uppercase and lowercase letters.",
 "If you think you know the word, you can type it in.",
 "You will loose if you have guessed 10 letters wrong.",
 "This is the word you need to guess: ",
 currentWord);
 printf("%d.     %s", n, "Enter the letter(s) you want to guess: ");
 /* As long as the word hasn't been guessed or the errors are lower than 10: */
 while( (strcmp(currentWord, guessWord) != 0) && (errors < 10) ){
 scanf("%c", &c); /* Retrieving the user entry */
 c = tolower(c); /* Removing caps */
 if (c != '\n') {
 if (isalpha(c)) { /* Making sure that the letter is alphanumeric */
 /* Checking wether the letter that has been entered (c) occurs in the guessWord */
 for (i = 0; i < wordlength; i++) {
  if (guessWord[i] == c) {
   currentWord[i] = c;
   guessedLetter = 1;
  }
 }
 /* Actions taken if the letter c doesn't occur in the guessWord and when it does */
 if (guessedLetter == 0) {
  errors++;
  printf("\nThat letter was incorrect.\n\n");
 } else {
 guessedLetter = 0;
  printf("\nThat letter was correct.\n\n");
 }
 /* Showing the galg and the amount of errors */
 printf("%s%s\n\n", "The word including the letters you guessed: ", currentWord);
 prn_galg(errors);
 n++; /* Increasing attempt amount */
 /* Showing header if the word has not been guessed and the errors are lower than 10 */
 if ( (strcmp(currentWord, guessWord) != 0) && (errors < 10) ) {
  printf("%d.     %s", n, "Enter the letter(s) you want to guess: ");
 }
 /* If the letter isn't alphanumeric (isalpha()) */
 } else {
  printf("Only alphanumeric symbols are allowed (a-z, A-Z), try again:\n");
 }
 }
 }
 /* Showing the results, wether the player won or not  */
 printf("---------------\n");
 printf("--- Results ---\n");
 printf("---------------\n\n");
 if (errors < 10) {
  if (strcmp(currentWord, guessWord) == 0) {
      printf("Congratulations you have guessed the right word!\n\n");
  } else {
      printf("You have guessed the wrong word, better luck next time!\n\n");
  }
 } else {
     printf("You have guessed the wrong word, better luck next time!\n\n");
 }
 printf("\nLetters guessed wrong: %d\nThe word that needed to be guessed: %s\nThe word you guessed: %s\n", errors, guessWord, currentWord);
 /*printf("\nEnter 'end' to end the game or enter 'again' to guess another word:\n");
 // Making sure that the user doesn't enter strange things
 while ((strcmp(udi, "END") != 0) && (strcmp(udi, "AGAIN") != 0)) {
  if (strcmp(udi, "EMPTY") != 0) {
      printf("\n\nIt is not allowed to enter anything else than 'again' or 'end', try again:\n");
  }
  // Retrieving the udi (udi = user determined input)
  scanf("%s", udi);
  // Converting the udi to uppercase
  int x;
  for (x = 0; x < 5; x++) {
      udi[x] = toupper(udi[x]);
  }
  printf("upper: %s\n", udi);
 }
 printf("\n\n--------------------------------------------\n\n");
 if(strcmp(udi, "END") == 0){
	 flag=0;
 }
 printf("not end\n");
 int again=(strcmp(udi, "AGAIN") == 0);
 printf("again: %d\n", again);
 getchar();
 int empty=(strcmp(udi, "EMPTY") == 0);
 printf("empty: %d\n", empty);
 getchar();
 if(!again && !empty ){
	 flag=0;
 }
}*/
return 0;
}
