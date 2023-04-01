#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "pgm.h"

#ifndef NULL
#define NULL 0
#endif

#define ERROR_ARG 1
#define ERROR_PGM 2

int intcmp(const void *a, const void *b){
    return ( *(int *)a - *(int *)b );
}

int calcular_mascara(type_pgm *source, const int tamanho_da_mascara, int i, int j){
    int k, l;
    int array[tamanho_da_mascara*tamanho_da_mascara];
    int t = (tamanho_da_mascara-1)/2;

    for(k=(t*-1); k<=t; k++){
        for(l=(t*-1); l<=t; l++){
            if((j+l) >= 0 && (j+l) < source->width && (i+k) >= 0 && (i+k) < source->height){
                array[(((k + t)*tamanho_da_mascara) + l)] = source->pixel[i+k][j+l];
            }
            else
                return source->pixel[i][j];
        }
    }

    qsort(array, tamanho_da_mascara, sizeof(int), intcmp);

    return array[(tamanho_da_mascara - 1)/2]; /* Como o tamanho sempre é impar, isso sempre será a mediana do array. */
}

type_pgm *aplicar_filtro(type_pgm *source, const int tamanho_da_mascara){
    type_pgm *editado = copiar_pgm(source);
    if(!editado)
        return NULL;

    int i, j;
    for(i=0; i<editado->height; i++){
        for(j=0; j<editado->width; j++){
            editado->pixel[i][j] = calcular_mascara(source, tamanho_da_mascara, i, j);
        }
    }

    return editado;
}

int main(int argc, char **argv){
    int i;
    int escrever_status;
    char *in_filename = NULL; 
    char *out_filename = NULL;
    FILE *in_file, *out_file;
    int tamanho_da_mascara = 3;
    type_pgm *pgm, *editado;
    for(i=1; i<argc; i+=2){
        if(!strcmp(argv[i], "-i")){

            in_filename = argv[i+1];
        }
        else if(!strcmp(argv[i], "-o")){
            out_filename = argv[i+1];
        }
        else if(!strcmp(argv[i], "-m")){
            tamanho_da_mascara = atoi(argv[i+1]);
        }
        else{
		    perror("Os argumentos estão errados! Use: **** -m [N] -i [input] -o [output]\n");
		    exit(ERROR_ARG);
        }
    }
    if(!(tamanho_da_mascara % 2)){
        perror("O tamanho da mascara deve ser um número impar.\n");
        exit(ERROR_ARG);
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

    editado = aplicar_filtro(pgm, tamanho_da_mascara);
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