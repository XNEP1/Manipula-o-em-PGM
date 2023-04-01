#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "pgm.h"

#ifndef NULL
#define NULL 0
#endif

#define ERROR_ARG 1
#define ERROR_PGM 2

type_pgm *aplicar_filtro(type_pgm *source, const int *mascara){
    type_pgm *editado = copiar_pgm(source);
    if(!editado)
        return NULL;

    editado->max_value = 255;
    int i, j, k, l, sum, valor_limiar;
    for(i=0; i<editado->height; i++){
        for(j=0; j<editado->width; j++){
            sum = 0;
            valor_limiar = source->pixel[i][j];
            for(k=-1; k<=1; k++){
                for(l=-1; l<=1; l++){
                    if((j+l) >= 0 && (j+l) < editado->width && (i+k) >= 0 && (i+k) < editado->height){
                        if(source->pixel[i+k][j+l] >= valor_limiar)
                            if(mascara[((k+1)*3) + l+1] != -1)
                                sum += mascara[((k+1)*3) + l+1];
                    /* */
                    }
                }
            }
            editado->pixel[i][j] = sum;
        }
    }

    return editado;
}

int main(int argc, char **argv){
    int i;
    int escrever_status;
    const int mascara[9] = {1, 2, 4, 8, -1, 16, 32, 64, 128};
    char *in_filename = NULL; 
    char *out_filename = NULL;
    FILE *in_file, *out_file;
    type_pgm *pgm, *editado;
    for(i=1; i<argc; i+=2){
        if(!strcmp(argv[i], "-i")){
            in_filename = argv[i+1];
        }
        else if(!strcmp(argv[i], "-o")){
            out_filename = argv[i+1];
        }
        else{
		    perror("Os argumentos estão errados! Use: **** -i [input] -o [output]\n");
		    exit(ERROR_ARG);
        }
    }
    if(!in_filename){
        in_file = stdin; /* A existencia ou não de uma imagem PGM só será feita na função ler_pgm. */
    }
    else{
        in_file = fopen(in_filename, "r");
    }

    if(!out_filename){
        out_file = stdout;
    }
    else{
        out_file = fopen(out_filename, "w");
    }

    pgm = ler_pgm(in_file);
    if(!pgm){
        perror("Erro na leitura do arquivo PGM.\n");
        exit(ERROR_PGM);
    }

    editado = aplicar_filtro(pgm, mascara);
    if(!editado){
        perror("Erro ao aplicar filtro.\n");
        exit(ERROR_PGM);
    }

    escrever_status = escrever_pgm(editado, out_file);
    if(escrever_status){
        perror("Erro para escrever o arquivo PGM editado.\n");
        exit(ERROR_PGM);
    }

    destruir_pgm(pgm);
    destruir_pgm(editado);
    exit(0);
}