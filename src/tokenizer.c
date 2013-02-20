#include<keywordengine.h>
#include<tokenizer.h>


/*
* Description   :   This function checks if the input word is a stop word or not by comparing with the stop words present in ../input/stopwords.txt
* Input         :   The word to be checked, if it is a stop word or not.
* Returns       :   Returns 1 if the word is a stopword, else returns 0.
*/
int check_for_stop_word(char *subject)
{
    FILE *fp;
    if((fp=fopen("../input/stopwords.txt","r"))==NULL)
    {
        printf("\nFatal Error! Could not open stopwords.txt. Check input directory.\nErrorcode : %d\n",errno);
        exit(0);
    }
    int check=0;
    char contender[wordsize];
    while(check==0 && !feof(fp))
    {
        fscanf(fp,"%s -> ",contender);
        if(strcmp(contender,subject)==0) check=1;
    }
    fclose(fp);
    return check;
}


/*
* Description   :   This function gets the document name as the input and removes all the punctuation marks, spaces, stop words and gives all the relevant words.
* Input         :   Document name
* Returns       :   returns count of keywords
*/
int getwords(char *docname)
{
    FILE *fp,*output;
    char *inputfilename=(char *)malloc(sizeof(char)*FILENAME);
    strcpy(inputfilename,"../input/");
    strcat(inputfilename,docname);
    int count=0;

    if((fp=fopen(inputfilename,"r"))==NULL)
    {
        printf("\nCould not open %s. Exiting\n",inputfilename);
        exit(0);
    }
    char *output_filename=(char*)malloc(sizeof(char)*FILENAME);
    strcpy(output_filename,"../output/output_");
    strcat(output_filename,docname);
    if((output=fopen(output_filename,"w"))==NULL)
    {
        exit(0);
    }
    char temp;
    char *word;
    char increment[wordsize];
    word=(char *)calloc(1,wordsize*sizeof(char));

    int stopword;
    while(!feof(fp))
    {


        fscanf(fp,"%c",&temp);
        if(isalpha(temp))
        {

            temp=tolower(temp);
        }
        if(isalpha(temp))
        {
            word[strlen(word)]=temp;
            word[strlen(word)]='\0';

        }
        else
        {


            stopword=check_for_stop_word(word);
            if(stopword==0 && strlen(word) != 0)
            {
                fprintf(output,"%s\n",word);
                free(word);
                word=NULL;
                count++;
                word=(char *)calloc(1,wordsize*sizeof(char));

            }
            else
            {
                free(word);
                word=NULL;
                word=(char *)calloc(1,wordsize*sizeof(char));
            }

        }



    }
    fclose(output);
    fclose(fp);
    free(word);
    word=NULL;
    fp=NULL;
    output=NULL;

    return count;
}


/*
* Description   :   This function gets the document name as the input and tokenises the file
* Input         :   Document name
* Returns       :   returns tokenised count of file
*/
int tokenise(char *docname)
{
    FILE *fp,*output;
    int count=0;
    char *inputfilename=(char *)malloc(sizeof(char)*FILENAME);
    strcpy(inputfilename,"../input/");
    strcat(inputfilename,docname);

    if((fp=fopen(inputfilename,"r"))==NULL)
    {
        printf("\nCould not open %s. Exiting\n",inputfilename);
        exit(0);
    }
    char *output_filename=(char*)malloc(sizeof(char)*FILENAME);
    strcpy(output_filename,"../output/output_");
    strcat(output_filename,docname);
    if((output=fopen(output_filename,"w"))==NULL)
    {
        exit(0);
    }
    char temp;
    char *word;
    char increment[wordsize];
    word=(char *)calloc(1,wordsize*sizeof(char));

    int stopword;
    while(!feof(fp))
    {


        fscanf(fp,"%c",&temp);
        if(isalpha(temp))
        {

            temp=tolower(temp);
        }
        if(isalpha(temp))
        {
            word[strlen(word)]=temp;
            word[strlen(word)]='\0';

        }
        else
        {

            if(strlen(word) != 0)
            {
                fprintf(output,"%s\n",word);
                free(word);
                word=NULL;
                count++;
                word=(char *)calloc(1,wordsize*sizeof(char));

            }

        }



    }
    fclose(output);
    fclose(fp);
    free(word);
    word=NULL;
    fp=NULL;
    output=NULL;

    return count;
}
