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

    int after_stemming=0;
    int before_stemming=0;
    double ratio=0.0;

    double array[20];

    for(i=0; i<20; i++)
    {
        array[i]=0;
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
        /* Add to postings list */
        initialize();
        before_stemming=add_document_to_postingslist(stemming_file);

        /* Apply Porter's Stemmer */
        stemmer(stemming_file);
        /* Add to postings list */
        initialize();
        after_stemming=add_document_to_postingslist(posting_filename);

        ratio=(double)after_stemming/before_stemming;

        //printf("\nbefore=%d and after=%d Ratio= %lf\n",before_stemming,after_stemming,ratio);

        if(0 <= ratio && 0.05 > ratio )
            array[0]++;
        else if(0.75 <= ratio && 0.765 > ratio )
            array[1]++;
        else if(0.765 <= ratio && 0.780 > ratio )
            array[2]++;
        else if(0.780 <= ratio && 0.795 > ratio )
            array[3]++;
        else if(0.795 <= ratio && 0.810 > ratio )
            array[4]++;
        else if(0.810 <= ratio && 0.825 > ratio )
            array[5]++;
        else if(0.825 <= ratio && 0.840 > ratio )
            array[6]++;
        else if(0.840 <= ratio && 0.855 > ratio )
            array[7]++;
        else if(0.855 <= ratio && 0.870 > ratio )
            array[8]++;
        else if(0.870 <= ratio && 0.885 > ratio )
            array[9]++;
        else if(0.885 <= ratio && 0.9 > ratio )
            array[10]++;
        else if(0.9 <= ratio && 0.915 > ratio )
            array[11]++;
        else if(0.915 <= ratio && 0.930 > ratio )
            array[12]++;
        else if(0.930 <= ratio && 0.945 > ratio )
            array[13]++;
        else if(0.945 <= ratio && 0.960 > ratio )
            array[14]++;
        else if(0.960 <= ratio && 0.975 > ratio )
            array[15]++;
        else if(0.975 <= ratio && 0.990 > ratio )
            array[16]++;
        else if(0.990 <= ratio && 1.05 > ratio )
            array[17]++;
        else if(1.05 <= ratio && 1.20 > ratio )
            array[18]++;
        else if(1.20 <= ratio && 1.35 >= ratio )
            array[19]++;

        i++;
        free(fileinput);
        fileinput=NULL;
    }
    fclose(posting_file);
    fclose(inputfiles);


    gnuplot_ctrl *h1;
    h1 = gnuplot_init() ;
    gnuplot_setstyle(h1, "lines");
    gnuplot_set_xlabel(h1, "Compression");
    gnuplot_set_ylabel(h1, "Frequency");

    gnuplot_plot_x(h1, array ,20, "Ratio Graph") ;
    getchar();
    /*Closing the files*/
    gnuplot_close(h1);

    return 0;
}
