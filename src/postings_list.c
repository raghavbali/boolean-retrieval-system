#include<keywordengine.h>
#include<postingslist.h>


void initialize()
{
    int i;
    no_of_terms=0;
    if(termhead != NULL)
    {
        free(termhead);
        termhead=NULL;
    }
    listofdocs=(struct docinfo *)malloc(maxdocuments*sizeof(struct docinfo));
    for(i=0; i<maxdocuments; i++)
    {
        strcpy(listofdocs[i].docname,"");
    }
    currentdoc=-1;
}


struct document* create_document_entry()
{
    struct document *newdoc;
    newdoc=(struct document *)malloc(sizeof(struct document));
    newdoc->docid=currentdoc;
    newdoc->termcount=1;
    newdoc->nextdoc=NULL;
    return newdoc;
}
void update_document(struct term *input)
{
    struct document *temp;
    temp=input->firstdoc;
    while(temp->nextdoc!=NULL) temp=temp->nextdoc;
    if(temp->docid==currentdoc) temp->termcount++;
    else
    {
        input->length++;
        temp->nextdoc=create_document_entry();
    }
}

void check_for_term(char *word)
{
    struct term *temp;
    temp=termhead;

    while(temp->nextterm!=NULL && strcmp(temp->termname,word)!=0)
    {
        temp=temp->nextterm;
    }
    if(strcmp(temp->termname,word)==0)
    {
        temp->totalcount=temp->totalcount+1;
        update_document(temp);

    }
    else
    {
        temp->nextterm=(struct term *)malloc(sizeof(struct term));
        strcpy(temp->nextterm->termname,word);
        temp->nextterm->totalcount=1;
        temp->nextterm->nextterm=NULL;
        temp->nextterm->length=1;
        temp->nextterm->firstdoc=create_document_entry();
        no_of_terms++;
    }

}


void insert_term(char *word)
{
    if(termhead==NULL)
    {
        termhead=(struct term *)malloc(sizeof(struct term));
        strcpy(termhead->termname,word);
        termhead->totalcount=1;
        termhead->nextterm=NULL;
        termhead->length=1;
        termhead->firstdoc=create_document_entry();
        no_of_terms=1;

    }
    else check_for_term(word);


}





int add_document_to_list(char *filename)
{
    currentdoc++;
    if(currentdoc<maxdocuments)
    {
        strcpy(listofdocs[currentdoc].docname,filename);
        return 1;
    }
    return 0;

}

int add_documents_or_files()
{
    initialize();
    FILE *fp,*doc_process;
    char *inputfilename=(char *)malloc(sizeof(char)*FILENAME);

    if((fp=fopen("../output/posting_list_file_input.txt","r"))==NULL)
    {
        printf("\nCould not read file list for creating postings list :%d\n",errno);
        exit(0);
    }
    char fileinput[docnamelength],word_process[wordlength];
    while(!feof(fp))
    {
        fscanf(fp,"%s\n",fileinput);
        add_document_to_list(fileinput);
        strcpy(inputfilename,"../output/");
        strcat(inputfilename,fileinput);
        if((doc_process=fopen(inputfilename,"r"))==NULL)
        {
            printf("\nCould not read stemmed file %s for postings list :%d\n",fileinput,errno);
            exit(0);
        }
        while(!feof(doc_process) && currentdoc<maxdocuments)
        {
            fscanf(doc_process,"%s\n",word_process);
            insert_term(word_process);
        }
        if(currentdoc>=maxdocuments) printf("Warning: Maximum 100 documents can be handled.Document count exceeded 100\n");
        fclose(doc_process);
    }
    fclose(fp);

    return no_of_terms;
}


int add_document_to_postingslist(char *fileinput)
{
    //initialize();
    FILE *doc_process;
    char *inputfilename=(char *)malloc(sizeof(char)*FILENAME);
    char word_process[wordlength];

    strcpy(inputfilename,"../output/");
    strcat(inputfilename,fileinput);
    if((doc_process=fopen(inputfilename,"r"))==NULL)
    {
        printf("\nCould not read input file %s for postings list :%d\n",fileinput,errno);
        exit(0);
    }
    add_document_to_list(fileinput);
    while(!feof(doc_process) && currentdoc<maxdocuments)
    {
        fscanf(doc_process,"%s\n",word_process);
        insert_term(word_process);
    }
    if(currentdoc>=maxdocuments) printf("Warning: Maximum 100 documents can be handled.Document count exceeded 100\n");

    fclose(doc_process);


    return no_of_terms;
}


struct term* find_term(char *mission)
{
    struct term *temp;
    temp=termhead;
    if(temp==NULL) return NULL;
    while(temp->nextterm!=NULL && strcmp(temp->termname,mission)!=0)
    {
        temp=temp->nextterm;
    }
    if(strcmp(temp->termname,mission)==0) return temp;
    else return NULL;
}

void display_documents(struct term *input)
{
    struct document *temp;
    temp=input->firstdoc;
    while(temp!=NULL)
    {
        printf(" ---> doc_id=%d, doc count=%d", temp->docid,temp->termcount);
        temp=temp->nextdoc;
    }
}
void display_terms()
{
    struct term *temp;
    temp=termhead;
    while(temp!=NULL)
    {
        printf("\n\nword=%s, total corpus count=%d and frequency=%d",temp->termname,temp->totalcount,temp->length);
        display_documents(temp);
        temp=temp->nextterm;
    }

}


void frequency_array(double *array)
{
    struct term *temp;
    temp=termhead;
    //int array[no_of_terms];
    int i=0;

    while(temp!=NULL)
    {
        array[i]=temp->length;
        temp=temp->nextterm;
        i++;
    }
}



void conjunction(char *first, char *second)
{
    struct term *firsttermptr=NULL, *secondtermptr=NULL;
    int conj_count=0;
    struct document *firstdocptr=NULL, *seconddocptr=NULL;
    firsttermptr=find_term(first);
    secondtermptr=find_term(second);
    if(firsttermptr!=NULL && secondtermptr!=NULL)
    {
        firstdocptr=firsttermptr->firstdoc;
        seconddocptr=secondtermptr->firstdoc;
        while(firstdocptr!=NULL && seconddocptr!=NULL)
        {
            if(firstdocptr->docid==seconddocptr->docid)
            {
                printf("\n **%.*s** ",(int)strlen(listofdocs[firstdocptr->docid].docname)-12,&listofdocs[firstdocptr->docid].docname[12]);
                conj_count++;
                firstdocptr=firstdocptr->nextdoc;
                seconddocptr=seconddocptr->nextdoc;
            }
            else if(firstdocptr->docid>seconddocptr->docid) seconddocptr=seconddocptr->nextdoc;
            else if(firstdocptr->docid<seconddocptr->docid) firstdocptr=firstdocptr->nextdoc;
        }
    }
    printf("\n No. of documents satisfying this query are %d\n",conj_count);
}

void search(char *first)
{
    struct term *firsttermptr=NULL;
    int conj_count=0;
    struct document *firstdocptr=NULL;
    firsttermptr=find_term(first);
    if(firsttermptr!=NULL)
    {
        firstdocptr=firsttermptr->firstdoc;
        while(firstdocptr!=NULL)
        {

            printf("\n **%.*s** ",(int)strlen(listofdocs[firstdocptr->docid].docname)-12,&listofdocs[firstdocptr->docid].docname[12]);
            conj_count++;
            firstdocptr=firstdocptr->nextdoc;



        }
    }
    printf("\n No. of documents satisfying this query are %d\n",conj_count);
}


void disjunction(char *first, char *second)
{
    struct term *firsttermptr=NULL, *secondtermptr=NULL;
    int disj_count=0;
    struct document *firstdocptr=NULL, *seconddocptr=NULL;
    firsttermptr=find_term(first);
    secondtermptr=find_term(second);
    if(firsttermptr!=NULL) firstdocptr=firsttermptr->firstdoc;
    if(secondtermptr!=NULL) seconddocptr=secondtermptr->firstdoc;
    while(firstdocptr!=NULL && seconddocptr!=NULL)
    {
        if(firstdocptr->docid==seconddocptr->docid)
        {
            printf("\n **%.*s** ",(int)strlen(listofdocs[firstdocptr->docid].docname)-12,&listofdocs[firstdocptr->docid].docname[12]);
            disj_count++;
            firstdocptr=firstdocptr->nextdoc;
            seconddocptr=seconddocptr->nextdoc;
        }
        else if(firstdocptr->docid>seconddocptr->docid)
        {

            printf("\n **%.*s** ",(int)strlen(listofdocs[seconddocptr->docid].docname)-12,&listofdocs[seconddocptr->docid].docname[12]);
            disj_count++;
            seconddocptr=seconddocptr->nextdoc;
        }
        else if(firstdocptr->docid<seconddocptr->docid)
        {
            printf("\n **%.*s** ",(int)strlen(listofdocs[firstdocptr->docid].docname)-12,&listofdocs[firstdocptr->docid].docname[12]);
            disj_count++;
            firstdocptr=firstdocptr->nextdoc;
        }
    }
    if(firstdocptr!=NULL)
    {
        while(firstdocptr!=NULL)
        {
            printf("\n **%.*s** ",(int)strlen(listofdocs[firstdocptr->docid].docname)-12,&listofdocs[firstdocptr->docid].docname[12]);
            disj_count++;
            firstdocptr=firstdocptr->nextdoc;
        }
    }
    if(seconddocptr!=NULL)
    {
        while(seconddocptr!=NULL)
        {
            printf("\n **%.*s** ",(int)strlen(listofdocs[seconddocptr->docid].docname)-12,&listofdocs[seconddocptr->docid].docname[12]);
            disj_count++;
            seconddocptr=seconddocptr->nextdoc;
        }
    }
    printf("\n No. of documents satisfying this query are %d\n",disj_count);
}

void first_conj_not_of_other(char *first, char *second)
{
    struct term *firsttermptr=NULL, *secondtermptr=NULL;
    int exclu_count=0;
    struct document *firstdocptr=NULL, *seconddocptr=NULL;
    firsttermptr=find_term(first);
    secondtermptr=find_term(second);
    if(firsttermptr!=NULL) firstdocptr=firsttermptr->firstdoc;
    if(secondtermptr!=NULL) seconddocptr=secondtermptr->firstdoc;
    while(firstdocptr!=NULL && seconddocptr!=NULL)
    {
        if(firstdocptr->docid==seconddocptr->docid)
        {
            firstdocptr=firstdocptr->nextdoc;
            seconddocptr=seconddocptr->nextdoc;
        }
        else if(firstdocptr->docid<seconddocptr->docid)
        {
            printf("\n **%.*s** ",(int)strlen(listofdocs[firstdocptr->docid].docname)-12,&listofdocs[firstdocptr->docid].docname[12]);
            exclu_count++;
            firstdocptr=firstdocptr->nextdoc;
        }
        else if(firstdocptr->docid>seconddocptr->docid) seconddocptr=seconddocptr->nextdoc;
    }
    if(firstdocptr!=NULL)
    {
        while(firstdocptr!=NULL)
        {
            printf("\n **%.*s** ",(int)strlen(listofdocs[firstdocptr->docid].docname)-12,&listofdocs[firstdocptr->docid].docname[12]);
            exclu_count++;
            firstdocptr=firstdocptr->nextdoc;
        }
    }
    printf("\n No. of documents satisfying this query are %d\n",exclu_count);

}

void not_of(char *first)
{
    int universe=0;
    struct term *firsttermptr=NULL;
    int not_count=0;
    struct document *firstdocptr=NULL;
    firsttermptr=find_term(first);
    if(firsttermptr!=NULL) firstdocptr=firsttermptr->firstdoc;
    while(universe<=currentdoc && firstdocptr!=NULL)
    {
        if(universe==firstdocptr->docid)
        {
            universe++;
            firstdocptr=firstdocptr->nextdoc;
        }
        else if(universe<firstdocptr->docid)
        {
            printf("\n **%.*s** ",(int)strlen(listofdocs[universe].docname)-12,&listofdocs[universe].docname[12]);
            not_count++;
            universe++;
        }
        else if(universe>firstdocptr->docid) firstdocptr=firstdocptr->nextdoc;
    }
    if(universe<=currentdoc)
    {
        while(universe<=currentdoc)
        {


            printf("\n **%.*s** ",(int)strlen(listofdocs[universe].docname)-12,&listofdocs[universe].docname[12]);
            not_count++;
            universe++;
        }
    }
    printf("\n No. of documents satisfying this query are %d\n",not_count);
}


int persistent_postingslist(char *fileinput,char *originalfile)
{
    FILE *doc_process;
    char *inputfilename=(char *)malloc(sizeof(char)*FILENAME);
    char word_process[wordlength];

    strcpy(inputfilename,"../output/");
    strcat(inputfilename,fileinput);
    if((doc_process=fopen(inputfilename,"r"))==NULL)
    {
        printf("\nCould not read input file %s for postings list :%d\n",fileinput,errno);
        exit(0);
    }
    add_document_to_list(originalfile);
    while(!feof(doc_process) && currentdoc<maxdocuments)
    {
        fscanf(doc_process,"%s\n",word_process);
        insert_term(word_process);
    }
    if(currentdoc>=maxdocuments) printf("Warning: Maximum 100 documents can be handled.Document count exceeded 100\n");

    fclose(doc_process);


    return no_of_terms;
}





void serialize_postings_list()
{
    struct term *temp;
    temp=termhead;
    struct document *temp_term;
    char convert[5];
    int flag =0;


    FILE *doc_process;
    char *inputfilename=(char *)malloc(sizeof(char)*FILENAME);
    char *word_process;

    strcpy(inputfilename,"../cluster/python-fp-growth-master/");
    strcat(inputfilename,"postingslist.txt");
    if((doc_process=fopen(inputfilename,"w"))==NULL)
    {
        printf("\nCould not read input file %s for postings list :%d\n",inputfilename,errno);
        exit(0);
    }

    while(temp!=NULL)
    {
        temp_term=temp->firstdoc;

        while(temp_term!=NULL)
        {
            //to prevent last line blank
            if(!flag)
                {
                    fprintf(doc_process,"%s %s %d",temp->termname,listofdocs[temp_term->docid].docname,temp_term->termcount);
                    flag = 1;
                }
            else
                fprintf(doc_process,"\n%s %s %d",temp->termname,listofdocs[temp_term->docid].docname,temp_term->termcount);

            temp_term=temp_term->nextdoc;
        }

        temp=temp->nextterm;

    }

    fclose(doc_process);

}
