/* Declaring contants */
#define WORDSIZE 100
#define FILENAME 300

/* Global variables for stemming */
char *input;


/* Function Prototypes */

/*
* Description   :   This function takes in size of the substring and returns the "sized" substring from the global variable input
* Input         :   Size of the substring.
* Returns       :   The substring of the global variable "input" of given size
*/
char * substring(int size);


/*  Description     :   This function checks if the character at given offset of input word is consonant or vowel
*   Input           :   int offset
*   Returns         :   'v' when its a vowel and 'c' otherwise
*/
char type_of_alphabet(int offset);



/*  Description     :   This function returns the length of m as per Porter Algorithm's requirement for inputs of type [C](VC)m[V]
*   Input           :   NA
*   Returns         :   value of m
*/
int m();

/*  Description     :   These functions apply the stemming rules per Porter's algorithm to the global variable "input".
*                       Refer to http://snowball.tartarus.org/algorithms/porter/stemmer.html for the rules
*   Input           :   NA
*   Returns         :   NA
*/
/* Start of Porter's Stemmer function block */
void step1a();
void step1b();
void step1b_subfunction();
void step1c();
int short_vowel_test(int index);
/* End of Porter's Stemmer function block */

/*  Description     :   This function takes filename with words to be stemmed and calls the rules in the required order
*   Input           :   Filename with words to be stemmed
*   Returns         :   NA
*/
void stemmer(char *filename);

void stemming_consolidated(char *inputword);
