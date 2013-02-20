#include<keywordengine.h>
#include<porterstemmer.h>

/*
* Description   :   This function takes in size of the substring and returns the "sized" substring from the global variable input
* Input         :   Size of the substring.
* Returns       :   The substring of the global variable "input" of given size
*/
char * substring(int size)
{
    int index=strlen(input);
    char *temp=(char *)malloc(size+5);
    strncpy(temp,input+(index-size),size);
    temp[size]='\0';

    return temp;
}


/*  Description     :   This function checks if the character at given offset of input word is consonant or vowel
*   Input           :   int offset
*   Returns         :   'v' when its a vowel and 'c' otherwise
*/
char type_of_alphabet(int offset)
{

    switch(input[offset])
    {
    case 'a':
    case 'e':
    case 'i':
    case 'o':
    case 'u':
        return 'v';
    case 'y':
        if((offset-1)>=0)
            return type_of_alphabet(offset-1);
        else
            return 'v';
    default :
        return 'c';
    }
}



int short_vowel_test(int index)
{
    if ('c'==type_of_alphabet(index-3) &&'v'==type_of_alphabet(index-2)  && 'c'==type_of_alphabet(index-1) && (input[index-1]!='w' || input[index-1]!='x' || input[index-1]!='y' ))
        return 1;
    else
        return 0;
}


/*  Description     :   This function returns the length of m as per Porter Algorithm's requirement for inputs of type [C](VC)m[V]
*   Input           :   NA
*   Returns         :   value of m
*/
int m()
{
    int i=0;
    int j=0;
    int count=0;
    int toggle=0;

    while(i<strlen(input) && type_of_alphabet(i)!='v') i++;

    j=i+1;
    while(j<strlen(input))
    {
        if('c'== type_of_alphabet(j) && 0 == toggle)
        {
            count++;
            toggle=1;
        }
        else if('v'== type_of_alphabet(j) && 1==toggle)
            toggle=0;

        j++;
    }
    return count;
}


/*  Description     :   These functions apply the stemming rules per Porter's algorithm to the global variable "input".
*                       Refer to http://snowball.tartarus.org/algorithms/porter/stemmer.html for the rules
*   Input           :   NA
*   Returns         :   NA
*/
/* Start of Porter's Stemmer function block */
void step1a()
{
    int index=strlen(input);
    char *temp;

    if(4<index)
    {
        temp=substring(4);
        if(0==strcmp(temp,"sses"))
        {
            free(temp);
            strcpy(input+(index-4),"ss");
            return;
        }
    }

    if(3<index)
    {
        temp=substring(3);
        if(0==strcmp(temp,"ies"))
        {
            free(temp);
            strcpy(input+(index-3),"i");
            return;
        }
    }

    if(2<index)
    {
        temp=substring(2);
        if(0==strcmp(temp,"ss"))
        {
            free(temp);
            strcpy(input+(index-2),"ss");
            return;
        }
    }

    if(2<index)
    {
        temp=substring(1);
        if(0==strcmp(temp,"s"))
        {
            free(temp);
            strcpy(input+(index-1),"\0");
            return;
        }
    }

    return;
}


void step1b_subfunction()
{

    int index=strlen(input);
    char *temp;


    if(2<index)
    {
        temp=substring(2);
        if( 0==strcmp(temp,"at"))
        {
            free(temp);
            strcpy(input+(index),"e");
            return;
        }

        if( 0==strcmp(temp,"bl"))
        {
            free(temp);
            strcpy(input+(index),"e");
            return;
        }

        if( 0==strcmp(temp,"iz"))
        {
            free(temp);
            strcpy(input+(index),"e");
            return;
        }
    }

    if(1<index)
    {
        temp=substring(1);
        if(input[index-2]==input[index-1] && !(0==strcmp(temp,"l") || 0==strcmp(temp,"s") || 0==strcmp(temp,"z")))
        {
            free(temp);
            strcpy(input+(index-1),"\0");
            return;
        }

    }

    if(1==m())
    {
        if(1==short_vowel_test(index))
        {
            strcpy(input+(index),"e");
            return;
        }


        return;
    }
}


void step1b()
{
    int index=strlen(input);
    char *temp;
    int i=0;
    int vowel_flag=0;


    if(3<index)
    {
        temp=substring(3);

        if(1 < m() && 0 == strcmp(temp,"eed"))
        {
            free(temp);
            strcpy(input+(index-3),"ee");
            return;
        }

        i=0;
        vowel_flag=0;
        while(i < index-3 && 0 == vowel_flag)
        {
            if('v' == type_of_alphabet(i))
                vowel_flag=1;
            i++;
        }
        if(/*i<index-3*/ 1 == vowel_flag && 0 == strcmp(temp,"ing"))
        {
            free(temp);
            strcpy(input+(index-3),"\0");
            step1b_subfunction();
            return;
        }
        //free(temp);
    }

    if(2<index)
    {
        i=0;
        temp=substring(2);
        vowel_flag=0;
        while(i < index-2 && 0 == vowel_flag)
        {
            if('v' == type_of_alphabet(i))
                vowel_flag=1;
            i++;
        }
        if(1 == vowel_flag && 0==strcmp(temp,"ed"))
        {
            free(temp);
            strcpy(input+(index-2),"\0");
            step1b_subfunction();
            return;
        }
    }

    return;
}



void step1c()
{
    int index=strlen(input);
    char *temp;
    int i=0;


    if(1<index)
    {
        i=0;
        temp=substring(1);
        while(i > index-1 && 'v' != type_of_alphabet(i))
        {
            i++;
        }
        if(i<index-1 && 0==strcmp(temp,"y"))
        {
            free(temp);
            strcpy(input+(index-1),"i");
            return;
        }
    }

    return;
}


void step2()
{
    if(m()>0)
    {

        int index=strlen(input);
        char *temp;

        if(7<index)
        {
            temp=substring(7);
            if(0==strcmp(temp,"ational"))
            {
                free(temp);
                strcpy(input+(index-7),"ate");
                return;
            }
            else if(0==strcmp(temp,"ization"))
            {
                free(temp);
                strcpy(input+(index-7),"ize");
                return;
            }
            else if(0==strcmp(temp,"iveness"))
            {
                free(temp);
                strcpy(input+(index-7),"ive");
                return;
            }
            else if(0==strcmp(temp,"fulness"))
            {
                free(temp);
                strcpy(input+(index-7),"ful");
                return;
            }
            else if(0==strcmp(temp,"ousness"))
            {
                free(temp);
                strcpy(input+(index-7),"ous");
                return;
            }
        }

        if(6<index)
        {
            temp=substring(6);
            if(0==strcmp(temp,"tional"))
            {
                free(temp);
                strcpy(input+(index-6),"tion");
                return;
            }
            else if(0==strcmp(temp,"biliti"))
            {
                free(temp);
                strcpy(input+(index-6),"ble");
                return;
            }
        }

        if(5<index)
        {
            temp=substring(5);
            if(0==strcmp(temp,"entli"))
            {
                free(temp);
                strcpy(input+(index-5),"ent");
                return;
            }
            else if(0==strcmp(temp,"ousli"))
            {
                free(temp);
                strcpy(input+(index-5),"ous");
                return;
            }
            else if(0==strcmp(temp,"ation"))
            {
                free(temp);
                strcpy(input+(index-5),"ate");
                return;
            }
            else if(0==strcmp(temp,"alism"))
            {
                free(temp);
                strcpy(input+(index-5),"al");
                return;
            }
            else if(0==strcmp(temp,"aliti"))
            {
                free(temp);
                strcpy(input+(index-5),"al");
                return;
            }
            else if(0==strcmp(temp,"iviti"))
            {
                free(temp);
                strcpy(input+(index-5),"ive");
                return;
            }
        }

        if(4<index)
        {
            temp=substring(4);
            if(0==strcmp(temp,"enci"))
            {
                free(temp);
                strcpy(input+(index-4),"ence");
                return;
            }
            else if(0==strcmp(temp,"anci"))
            {
                free(temp);
                strcpy(input+(index-4),"ance");
                return;
            }
            else if(0==strcmp(temp,"izer"))
            {
                free(temp);
                strcpy(input+(index-4),"ize");
                return;
            }
            else if(0==strcmp(temp,"abli"))
            {
                free(temp);
                strcpy(input+(index-4),"able");
                return;
            }
            else if(0==strcmp(temp,"alli"))
            {
                free(temp);
                strcpy(input+(index-4),"al");
                return;
            }
            else if(0==strcmp(temp,"ator"))
            {
                free(temp);
                strcpy(input+(index-4),"ate");
                return;
            }
        }

        if(3<index)
        {
            temp=substring(3);
            if(0==strcmp(temp,"eli"))
            {
                free(temp);
                strcpy(input+(index-3),"e");
                return;
            }
        }
        return;
    }
    else return;
}

void step3()
{
    if(m()>0)
    {

        int index=strlen(input);
        char *temp;

        if(5<index)
        {
            temp=substring(5);
            if(0==strcmp(temp,"icate"))
            {
                free(temp);
                strcpy(input+(index-5),"ic");
                return;
            }
            else if(0==strcmp(temp,"ative"))
            {
                free(temp);
                strcpy(input+(index-5),"\0");
                return;
            }
            else if(0==strcmp(temp,"alize"))
            {
                free(temp);
                strcpy(input+(index-5),"al");
                return;
            }
            else if(0==strcmp(temp,"iciti"))
            {
                free(temp);
                strcpy(input+(index-5),"ic");
                return;
            }
        }

        if(4<index)
        {
            temp=substring(4);
            if(0==strcmp(temp,"ness"))
            {
                free(temp);
                strcpy(input+(index-4),"\0");
                return;
            }
            else if(0==strcmp(temp,"ical"))
            {
                free(temp);
                strcpy(input+(index-4),"ic");
                return;
            }
        }

        if(3<index)
        {
            temp=substring(3);
            if(0==strcmp(temp,"ful"))
            {
                free(temp);
                strcpy(input+(index-3),"\0");
                return;
            }
        }
        return;
    }
    else return;
}

void step4()
{
    if(m()>2)
    {

        int index=strlen(input);
        char *temp;

        if(5<index)
        {
            temp=substring(5);
            if(0==strcmp(temp,"ement"))
            {
                free(temp);
                strcpy(input+(index-5),"\0");
                return;
            }
        }

        if(4<index)
        {
            temp=substring(4);
            if(0==strcmp(temp,"ance"))
            {
                free(temp);
                strcpy(input+(index-4),"\0");
                return;
            }
            else if(0==strcmp(temp,"ence"))
            {
                free(temp);
                strcpy(input+(index-4),"\0");
                return;
            }
            else if(0==strcmp(temp,"able"))
            {
                free(temp);
                strcpy(input+(index-4),"\0");
                return;
            }
            else if(0==strcmp(temp,"ible"))
            {
                free(temp);
                strcpy(input+(index-4),"\0");
                return;
            }
            else if(0==strcmp(temp,"ment"))
            {
                free(temp);
                strcpy(input+(index-4),"\0");
                return;
            }
            else if(0==strcmp(temp,"sion"))
            {
                free(temp);
                strcpy(input+(index-3),"\0");
                return;
            }
            else if(0==strcmp(temp,"tion"))
            {
                free(temp);
                strcpy(input+(index-3),"\0");
                return;
            }
        }

        if(3<index)
        {
            temp=substring(3);
            if(0==strcmp(temp,"ant"))
            {
                free(temp);
                strcpy(input+(index-3),"\0");
                return;
            }

            else if(0==strcmp(temp,"ent"))
            {
                free(temp);
                strcpy(input+(index-3),"\0");
                return;
            }

            else if(0==strcmp(temp,"ism"))
            {
                free(temp);
                strcpy(input+(index-3),"\0");
                return;
            }

            else if(0==strcmp(temp,"ate"))
            {
                free(temp);
                strcpy(input+(index-3),"\0");
                return;
            }

            else if(0==strcmp(temp,"iti"))
            {
                free(temp);
                strcpy(input+(index-3),"\0");
                return;
            }

            else if(0==strcmp(temp,"ous"))
            {
                free(temp);
                strcpy(input+(index-3),"\0");
                return;
            }

            else if(0==strcmp(temp,"ive"))
            {
                free(temp);
                strcpy(input+(index-3),"\0");
                return;
            }

            else if(0==strcmp(temp,"ize"))
            {
                free(temp);
                strcpy(input+(index-3),"\0");
                return;
            }
        }
        if(2<index)
        {
            temp=substring(2);
            if(0==strcmp(temp,"al"))
            {
                free(temp);
                strcpy(input+(index-2),"\0");
                return;
            }

            else if(0==strcmp(temp,"er"))
            {
                free(temp);
                strcpy(input+(index-2),"\0");
                return;
            }

            else if(0==strcmp(temp,"ic"))
            {
                free(temp);
                strcpy(input+(index-2),"\0");
                return;
            }

            else if(0==strcmp(temp,"ou"))
            {
                free(temp);
                strcpy(input+(index-2),"\0");
                return;
            }

        }
        return;
    }
    else return;
}

void step5a()
{
    int index=strlen(input);
    if(m()>2)
    {

        if(input[index-1]=='e') input[index-1]='\0';

    }
    else if(1<m() && !short_vowel_test(index) && input[index-1]=='e')
    {
        input[index-1]='\0';
    }
    return;
}
void step5b()
{
    if(m()>1)
    {
        int index=strlen(input);

        if(index>=3 && input[index-1]=='l' && input[index-2]=='l')
        {
            input[index-1]='\0';
        }
    }
    return;
}
/* End of Porter's Stemmer function block */


/*  Description     :   This function takes filename with words to be stemmed and calls the rules in the required order
*   Input           :   Filename with words to be stemmed
*   Returns         :   NA
*/
void stemmer(char *filename)
{
    FILE *stemmingfile,*stemming_output;
    char *outputfilename=(char *)malloc(sizeof(char)*FILENAME);
    char *inputfilename=(char *)malloc(sizeof(char)*FILENAME);
    char unstemmed_word[WORDSIZE];

    strcpy(inputfilename,"../output/");
    strcat(inputfilename,filename);

    strcpy(outputfilename,"../output/stem_");
    strcat(outputfilename,filename);

    if((stemmingfile=fopen(inputfilename,"r"))==NULL)
    {
        printf("\nFatal Error! Could not open %s. Check output directory.\nErrorcode : %d\n",inputfilename,errno);
        exit(0);
    }

    if((stemming_output=fopen(outputfilename,"w"))==NULL)
    {
        printf("\nFatal Error! Could not create %s. Check output directory.\nErrorcode : %d\n",outputfilename,errno);
        exit(0);
    }

    while(!feof(stemmingfile))
    {
        fscanf(stemmingfile,"%s\n",unstemmed_word);
        input=(char *)malloc(sizeof(strlen(unstemmed_word))+5);
        strcpy(input,unstemmed_word);

        /* Stemming functions*/
        step1a();
        step1b();
        step1c();
        step2();
        step3();
        step4();
        step5a();
        step5b();
        /* write stemmed word to file */
        fprintf(stemming_output,"%s\n",input);

    }
    fclose(stemmingfile);
    fclose(stemming_output);
}

void stemming_consolidated(char *inputword)
{

           input=(char *)malloc(sizeof(strlen(inputword))+5);
        strcpy(input,inputword);

        /* Stemming functions*/
        step1a();
        step1b();
        step1c();
        step2();
        step3();
        step4();
        step5a();
        step5b();
        strcpy(inputword,input);

        /* write stemmed word to file */

}
