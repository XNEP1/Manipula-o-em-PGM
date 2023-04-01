/* 
 * Biblioteca para leitura de arquivos .pgm (P2 ou P5)..
 * de Bruno Krügel.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pgm.h"

#ifndef NULL
#define NULL 0
#endif

static void ignorar_comentario(FILE *file){
    if(fgetc(file) != '\n')
        fseek(file, -1, SEEK_CUR);

    char fist_letter = fgetc(file);
    if(fist_letter == '#'){
        while(fgetc(file) != '\n');
        ignorar_comentario(file);
    }
    else
        fseek(file, -1, SEEK_CUR);
}

type_pgm *criar_pgm(char type[2],int width, int height, int max_value){
    if(!(strcmp(type, "P2") || strcmp(type, "P5")) || width <= 0 || height <= 0 || max_value < 0)
        return NULL;

    int i, j;
    type_pgm *pgm = malloc(sizeof(type_pgm));
    if(!pgm)
        return NULL;

    strcpy(pgm->type, type);
    pgm->width = width;
    pgm->height = height;
    pgm->max_value = max_value;

    /* Aloca (altura x largura) pixeis na memoria. */
    pgm->pixel = malloc(pgm->height * sizeof(int *));
    for(i=0; i<pgm->height; i++){
        pgm->pixel[i] = malloc(pgm->width * sizeof(int));
        for(j=0; j<pgm->width; j++){
            pgm->pixel[i][j] = pgm->max_value;
        }
    }
    return pgm;
}

type_pgm *ler_pgm(FILE *pgm_file){
    int i, j;
    type_pgm *pgm;
    char type[2];
    int width, height, max_value;
    if(!pgm_file)
        return NULL;

    ignorar_comentario(pgm_file);
    fscanf(pgm_file, "%2s", type);
    ignorar_comentario(pgm_file);

    fscanf(pgm_file, "%d", &width);
    fscanf(pgm_file, "%d", &height);
    ignorar_comentario(pgm_file);

    fscanf(pgm_file, "%d", &max_value);
    ignorar_comentario(pgm_file);

    pgm = criar_pgm(type, width, height, max_value);
    if(!pgm)
        return NULL;

    if(!strcmp(pgm->type, "P2")){      /* magic_word == "P2" */
        for(i=0; i<pgm->height; i++)
            for(j=0; j<pgm->width; j++){
                fscanf(pgm_file, "%hhu", &pgm->pixel[i][j]);
            }
    }
    else{ /* magic_word == "P5" */
        for(i=0; i<pgm->height; i++)
            if(pgm->max_value < 256)
                fread(pgm->pixel[i], sizeof(char), pgm->width, pgm_file);
            else
                fread(pgm->pixel[i], 2*sizeof(char), pgm->width, pgm_file);
    }

    fclose(pgm_file);
    return pgm;
}

type_pgm *copiar_pgm(type_pgm *pgm){
    int i,j;
    type_pgm *copia;

    copia = criar_pgm(pgm->type, pgm->width, pgm->height, pgm->max_value);
    if(!copia)
        return NULL;

    for(i=0; i<pgm->height; i++){
        for(j=0; j<pgm->width; j++){
            copia->pixel[i][j] = pgm->pixel[i][j];
        }
    }

    return copia;
}

int escrever_pgm(type_pgm *pgm, FILE *out_pgm_file){
    int i, j;
    if(!pgm || !out_pgm_file)
        return 1;

    fprintf(out_pgm_file, "%s\n", pgm->type);
    fprintf(out_pgm_file, "%d %d\n", pgm->width, pgm->height);
    fprintf(out_pgm_file, "%d\n", pgm->max_value);

    if(!strcmp(pgm->type, "P2")){      /* magic_word == "P2" */
        for(i=0; i<pgm->height; i++){
            for(j=0; j<pgm->width; j++){
                fprintf(out_pgm_file, "%d ", pgm->pixel[i][j]);
            }
            fprintf(out_pgm_file, "\n");
        }
    }
    else{ /* magic_word == "P5" */
        for(i=0; i<pgm->height; i++)
            if(pgm->max_value < 256)
                fwrite(pgm->pixel[i], sizeof(char), pgm->width, out_pgm_file);
            else
                fwrite(pgm->pixel[i], 2*sizeof(char), pgm->width, out_pgm_file);
    }

    fclose(out_pgm_file);
    return 0;
}

void destruir_pgm(type_pgm *pgm){
    int i;

    for(i=0; i<pgm->height; i++)
        free(pgm->pixel[i]);
    free(pgm->pixel);
    free(pgm);
}