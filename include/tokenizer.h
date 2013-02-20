

/* Function Prototypes */

/*
* Description   :   This function checks if the input word is a stop word or not by comparing with the stop words present in ../input/stopwords.txt
* Input         :   The word to be checked, if it is a stop word or not.
* Returns       :   Returns 1 if the word is a stopword, else returns 0.
*/
int check_for_stop_word(char *subject);


/*
* Description   :   This function gets the document name as the input and removes all the punctuation marks, spaces, stop words and gives all the relevant words.
* Input         :   Document name
* Returns       :   returns count of keywords
*/
int getwords(char *docname);


/*
* Description   :   This function gets the document name as the input and tokenises the file
* Input         :   Document name
* Returns       :   returns tokenised count of file
*/
int tokenise(char *docname);
