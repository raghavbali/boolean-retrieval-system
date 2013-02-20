#include<keywordengine.h>


int main(int argc, char *argv[])
{
    FILE *inputfiles,*posting_file;
    int i=0;
    char *fileinput,*stemming_file,*posting_filename;

    if(argc < 2)
    {
        printf("\nIncorrect Usage. ./keywordengine <filelist.txt>\n");
        return 0;
    }

    if((inputfiles=fopen(argv[1],"r+"))==NULL)
    {
        printf("\nCould not open %s. Exiting\n",argv[1]);
        exit(0);
    }

    if((posting_file=fopen("../output/posting_list_file_input.txt","w"))==NULL)
    {
        printf("\nFatal Error! Could not open/create posting_list_file_input.txt. Check output directory.\nErrorcode : %d\n",errno);
        exit(0);
    }
    while(!feof(inputfiles))
    {
        fileinput=(char *)malloc(sizeof(char)*FILENAME);
        fscanf(inputfiles,"%s\n",fileinput);
        stemming_file=(char *)malloc(sizeof(char)*(strlen(fileinput)+8));

        strcpy(stemming_file,"output_");
        strcat(stemming_file,fileinput);


        posting_filename=(char *)malloc(sizeof(char)*(strlen(stemming_file)+6));
        strcpy(posting_filename,"stem_");
        strcat(posting_filename,stemming_file);
        fprintf(posting_file,"%s\n",posting_filename);


        /* Tokenise and remove stopwords */
        getwords(fileinput);

        /* Apply Porter's Stemmer */
        stemmer(stemming_file);

        i++;
        free(fileinput);
        fileinput=NULL;
    }
    fclose(posting_file);
    fclose(inputfiles);


    /* Add to postings list */
    int size=add_documents_or_files();
    int j=0;
    //display_terms();
   // disjunction("amar","human");
    double array[size];
    double array_graph[size];

    frequency_array(array);
    /*for(i=0;i<size;i++) printf(" arr[%d]=%lf",i,array[i]);*/
    quicksort(0,size,array);

    for(j=0,i=size-1;j<size && i>=0;i--,j++)
    {
        array_graph[j]=array[i];
        if(array_graph[j]<0)
            printf("j=%d a=%lf",j,array_graph[j]);
    }

    gnuplot_ctrl *h1;
    h1 = gnuplot_init() ;
    gnuplot_setstyle(h1, "lines");
    gnuplot_set_xlabel(h1, "Words");
    gnuplot_set_ylabel(h1, "Frequency");

    gnuplot_plot_x(h1, array_graph ,size, "Frequency Graph") ;
    getchar();
    /*for(i=0;i<size;i++) printf(" arr[%d] %lf",i,array[i]);*/
    /*Closing the files*/
    gnuplot_close(h1);
    while(1)
    {
        int input;
        char a[wordsize],b[wordsize];


        printf("\n\n**************************************\nWELCOME TO BOOLEAN QUERY DEMO\n**************************************\n\n");
        printf("Considering A and B as two words, give input to the corresponding operations:\n");
        printf("1.\t\"A OR B\"\n");
                printf("2.\t\"A AND B\"\n");

        printf("3.\t\"NOT(A)\"\n");
                printf("4.\t\"A AND NOT(B)\"\n");
                printf("5.\tsearch A\n");
                printf("0.\tExit\n");
                printf("Choice: ");
                scanf("%d",&input);
                if(input==0) break;
                else if(input==1 || input==2 || input==4)
                {
                    printf("Enter the first word:\n");
                    scanf("%s",a);
                    printf("Enter the second word:\n");
                    scanf("%s",b);
                    stemming_consolidated(a);
                    stemming_consolidated(b);
                    printf("\nPorter's stemming algorithm resulted into \"%s\" and \"%s\"\n",a,b);
                    if(input==1) disjunction(a,b);
                    else if(input==2) conjunction(a,b);
                    else if(input==4) first_conj_not_of_other(a,b);
                }
                else if(input==3 || input==5)
                {
                    printf("Enter the word:\n");
                    scanf("%s",a);
                    //stem the word
                    stemming_consolidated(a);
                                        printf("\nPorter's stemming algorithm resulted into \"%s\"\n",a);

                   if(input==3) not_of(a);
                   else if(input==5) search(a);

                }
                else printf("ERROR: wrong input..\n\n\n\n");


    }

    return 0;
}
