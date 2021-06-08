
#include "modelos.h"
#include "gerador.h"

void criaXML(char** ficheiros) {
    FILE* fd;

    char *diretoria = strdup("../files/primitivas.xml");

    fd = fopen(diretoria,"w");

    if(fd != NULL){
        char* ss = (char*)malloc(sizeof(char)*1024);
        ss = strcat(ss, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<scene>\n");

        for (int i = 0; ficheiros[i]!=NULL; i++) {
            ss = strcat(ss,"<model file=\"../files/");
            ss = strcat(ss,ficheiros[i]);
            ss = strcat(ss,"\" />\n");
        }

        ss = strcat(ss,"\n</scene>\n" );

        fprintf(fd,"%s",ss);

    } else{
        cout << "Erro ao criar o ficheiro!" << endl;
    }
    fclose(fd);
}

int main(int argc,char *argv[]) {

    if (argc <= 1 || argc >= 8) {
        printf("Erro!");

    } else {

        char *diretoria = strdup("../files/");

        if (!strcmp(argv[1],"plano")) {
            if (argc != 4) {
                printf("Use: ./generator plano <tamanho> <ficheiro>\n");
            } else {
                char *ficheiro = strcat(diretoria,argv[3]);
                float tam = atof(argv[2]);

                desenhaPlano(tam, ficheiro);
            }
        } else if (!strcmp(argv[1],"cubo")) {
            if (argc != 7) {
                printf("Use: ./generator cubo <x> <y> <z> <divisoes> <ficheiro>\n");
            } else {

                char *ficheiro = strcat(diretoria,argv[6]);

                float x = atof(argv[2]);
                float y = atof(argv[3]);
                float z = atof(argv[4]);
                int d = atoi(argv[5]);

                desenhaCaixa(x, y, z, d, ficheiro);
            }
        } else if (!strcmp(argv[1],"esfera")) {
            if (argc != 6) {
                printf("Use: ./generator esfera <raio> <faces> <pilhas> <ficheiro>\n");
            } else {

                char *ficheiro = strcat(diretoria,argv[5]);

                float raio = atof(argv[2]);
                int faces = atoi(argv[3]);
                int pilhas = atoi(argv[4]);

                desenhaEsfera(raio, faces, pilhas, ficheiro);
            }
        } else if (!strcmp(argv[1],"cone")) {
            if (argc != 7) {
                printf("Use: ./generator cone <raio> <altura> <faces> <pilhas> <ficheiro>\n");
            } else {

                char *ficheiro = strcat(diretoria, argv[6]);

                float raio = atof(argv[2]);
                float altura = atof(argv[3]);
                int faces = atoi(argv[4]);
                int pilhas = atoi(argv[5]);

                desenhaCone(raio, altura, faces, pilhas, ficheiro);
            }
        } else if (!strcmp(argv[1],"torus")) {
            if (argc != 7) {
                printf("Use: ./generator anel <raioInt> <raioExt> <faces> <aneis> <ficheiro>\n");
            } else {

                char *ficheiro = strcat(diretoria, argv[6]);

                float raioInt = atof(argv[2]);
                float raioExt = atof(argv[3]);
                int faces = atoi(argv[4]);
                int aneis = atoi(argv[5]);

                desenhaTorus(raioInt, raioExt, faces, aneis, ficheiro);
            }

        } else if (!strcmp(argv[1],"xml")) {
            if (argc <= 2) {
                printf("Use: ./gerador xml\n");
            } else {
                char **nomes = (char **)malloc(1024*sizeof(char **));
                int cont = 0;
                for (int i = 2; i<argc; i++) {
                    nomes[cont++] = argv[i];
                }
                criaXML(nomes);
            }
        }
    }
    return 0;
}

