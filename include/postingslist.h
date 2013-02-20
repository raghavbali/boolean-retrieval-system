/* Declaring contants */
#define docnamelength 200
#define maxdocuments 100
#define wordlength 50

/* Global variables and data structures */
int currentdoc;
int no_of_terms=0;

struct document
{
    int docid;
    int termcount;
    struct document *nextdoc;
};

struct docinfo
{
    char docname[docnamelength];
};



struct term
{
    int totalcount;
    int length;
    char termname[wordlength];
    struct term *nextterm;
    struct document *firstdoc;
}*termhead;

struct docinfo *listofdocs;



/* Function Prototypes */

/*
* Description   :   This function initializes the list of document names to NULL
* Input         :   NA
* Returns       :   NA
*/
void initialize();


/*  Description     :   This function creates a new document node in the postings list for a new term encountered
*   Input           :   NA
*   Returns         :   Returns a document type pointer
*/
struct document* create_document_entry();



/*  Description     :   This function appends a doc_id to the end of the list of a word or increments thee counter for an existing doc_id
*   Input           :   Input term found.
*   Returns         :   NA
*/
void update_document(struct term *input);

/*  Description     :   This function checks if a term exists in the posting list, if it exists then calls update_document else creates a new entry
*   Input           :   The term to be searched
*   Returns         :   NA
*/
void check_for_term(char *word);


/*  Description     :   This function inserts a new term in the postings list
*   Input           :   The term to be added
*   Returns         :   NA
*/
void insert_term(char *word);


int add_document_to_list(char *filename);
int add_documents_or_files();
struct term* find_term(char *mission);
void display_documents(struct term *input);
void display_terms();
void conjunction(char *first, char *second);
void disjunction(char *first, char *second);
void first_conj_not_of_other(char *first, char *second);
void not_of(char *first);
void frequency_array(double *);
void search(char *first);
int add_document_to_postingslist(char *fileinput);
