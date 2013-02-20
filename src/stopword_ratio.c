#include<keywordengine.h>


int main(int argc, char *argv[])
{
    FILE *inputfiles;
    int i=0;
    char *fileinput;

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

    int all_words=0;
    int only_keywords=0;
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


        /* Tokenise and remove stopwords */
        only_keywords=getwords(fileinput);

        /* all words */
        all_words=tokenise(fileinput);

        ratio=(double)only_keywords/all_words;

        //printf("\nbefore=%d and after=%d Ratio= %lf\n",only_keywords,all_words,ratio);

        if(0.45 <= ratio && 0.4625 > ratio )
            array[0]++;
        else if(0.4625 <= ratio && 0.4750 > ratio )
            array[1]++;
        else if(0.4750 <= ratio && 0.4875 > ratio )
            array[2]++;
        else if(0.4875 <= ratio && 0.5 > ratio )
            array[3]++;
        else if(0.5 <= ratio && 0.5125 > ratio )
            array[4]++;
        else if(0.5125 <= ratio && 0.5250 > ratio )
            array[5]++;
        else if(0.5250 <= ratio && 0.5375 > ratio )
            array[6]++;
        else if(0.5375 <= ratio && 0.5500 > ratio )
            array[7]++;
        else if(0.5500 <= ratio && 0.5625 > ratio )
            array[8]++;
        else if(0.5625 <= ratio && 0.5750 > ratio )
            array[9]++;
        else if(0.5750 <= ratio && 0.5875 > ratio )
            array[10]++;
        else if(0.5875 <= ratio && 0.6 > ratio )
            array[11]++;
        else if(0.6 <= ratio && 0.6125 > ratio )
            array[12]++;
        else if(0.6125 <= ratio && 0.6250 > ratio )
            array[13]++;
        else if(0.6250 <= ratio && 0.6375 > ratio )
            array[14]++;
        else if(0.6375 <= ratio && 0.6500 > ratio )
            array[15]++;
        else if(0.6500 <= ratio && 0.6625 > ratio )
            array[16]++;
        else if(0.6625 <= ratio && 0.6750 > ratio )
            array[17]++;
        else if(0.6750 <= ratio && 0.6875 > ratio )
            array[18]++;
        else if(0.6875 <= ratio && 0.7 >= ratio )
            array[19]++;

        i++;
        free(fileinput);
        fileinput=NULL;
    }
    fclose(inputfiles);


    gnuplot_ctrl *h1;
    h1 = gnuplot_init() ;
    gnuplot_setstyle(h1, "lines");
    gnuplot_set_xlabel(h1, "Ratio of Count of Words(post stopword removal) / Count of all words");
    gnuplot_set_ylabel(h1, "Frequency");

    gnuplot_plot_x(h1, array ,20, "Ratio Graph") ;
    getchar();
    /*Closing the files*/
    gnuplot_close(h1);


    return 0;
}
