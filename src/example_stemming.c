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

        /* Tokenise and remove stopwords */
        getwords(fileinput);

        /* Apply Porter's Stemmer */
        stemmer(stemming_file);

        free(fileinput);
        fileinput=NULL;
    }
    fclose(inputfiles);


    return 0;
}
