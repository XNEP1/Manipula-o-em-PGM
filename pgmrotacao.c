#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "pgm.h"

#ifndef NULL
#define NULL 0
#endif

#define ERROR_ARG 1
#define ERROR_PGM 2

#ifndef M_PI
#define M_PI 3.14
#endif

double toRad(int degree){
    return ((degree*M_PI)/180);
}

/* Retorna o maior valor entre a,b,c e d. */
double max(double a, double b, double c, double d){
    double array[] = {a,b,c,d};
    int i;
    double max = array[0];
    for(i=0; i<4; i++)
        if(array[i] > max)
            max = array[i];
    
    return max;
}

/* Retorna o menor valor entre a,b,c e d. */
double min(double a, double b, double c, double d){
    double array[] = {a,b,c,d};
    int i;
    double min = array[0];
    for(i=0; i<4; i++)
        if(array[i] < min)
            min = array[i];
    
    return min;
}

/* Matriz de rotação para X */
double rotateX(int x, int y, double a){
    return (x*cos(a) - y*sin(a));
}

/* Matriz de rotação para Y */
double rotateY(int x, int y, double a){
    return (x*sin(a) + y*cos(a));
}

type_pgm *aplicar_filtro(type_pgm *source, int angulo){
    int i, j;
    double x, y;
    double peso_x, peso_y;
    int x_1, y_1, x_2, y_2;
    int valor_x1y1, valor_x1y2, valor_x2y1, valor_x2y2;
    double a = toRad(angulo);

    /* Compara as coordenadas das 4 bordas para descobrir o quão maior a imagem irá ficar depois de rotacionar. */
    int max_width = (int)max(rotateX(0, 0, a), rotateX(0, source->height-1, a), rotateX(source->width-1, 0, a), rotateX(source->width-1, source->height-1, a));
    int max_height = (int)max(rotateY(0, 0, a), rotateY(0, source->height-1, a), rotateY(source->width-1, 0, a), rotateY(source->width-1, source->height-1, a));
    int min_width = (int)min(rotateX(0, 0, a), rotateX(0, source->height-1, a), rotateX(source->width-1, 0, a), rotateX(source->width-1, source->height-1, a));
    int min_height = (int)min(rotateY(0, 0, a), rotateY(0, source->height-1, a), rotateY(source->width-1, 0, a), rotateY(source->width-1, source->height-1, a));

    type_pgm *editado = criar_pgm(source->type, (max_width - min_width), (max_height - min_height), source->max_value);
    if(!editado)
        return NULL;

    for(i=min_height; i<max_height; i++){
        for(j=min_width; j<max_width; j++){
            x = rotateX(j, i, -1*a);
            y = rotateY(j, i, -1*a);
            
            x_1 = (int)floor(x);
            y_1 = (int)floor(y);
            x_2 = (int)ceil(x);
            y_2 = (int)ceil(y);

            valor_x1y1 = editado->max_value;
            valor_x1y2 = editado->max_value;
            valor_x2y1 = editado->max_value;
            valor_x2y2 = editado->max_value;

            peso_x = (x - x_1);
            peso_y = (y - y_1);

            if(x_1 >= 0 && x_1 < source->width && y_1 >= 0 && y_1 < source->height)
                valor_x1y1 = source->pixel[y_1][x_1]; 

            if(x_1 >= 0 && x_1 < source->width && y_2 >= 0 && y_2 < source->height)
                valor_x1y2 = source->pixel[y_2][x_1];

            if(x_2 >= 0 && x_2 < source->width && y_1 >= 0 && y_1 < source->height)
                valor_x2y1 = source->pixel[y_1][x_2];

            if(x_2 >= 0 && x_2 < source->width && y_2 >= 0 && y_2 < source->height)
                valor_x2y2 = source->pixel[y_2][x_2];

            editado->pixel[i - min_height][j - min_width] = (int)((valor_x1y1*(1-peso_x) + valor_x2y1*peso_x)*(1-peso_y) + (valor_x1y2*(1-peso_x) + valor_x2y2*peso_x)*(peso_y));
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
    int angulo = 90;
    type_pgm *pgm, *editado;
    for(i=1; i<argc; i+=2){
        if(!strcmp(argv[i], "-i")){

            in_filename = argv[i+1];
        }
        else if(!strcmp(argv[i], "-o")){
            out_filename = argv[i+1];
        }
        else if(!strcmp(argv[i], "-a")){
            angulo = atoi(argv[i+1]);
        }
        else{
		    perror("Os argumentos estão errados! Use: **** -a [angulo] -i [input] -o [output]\n");
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
        printf("%p\n", pgm);
        exit(ERROR_PGM);
    }

    editado = aplicar_filtro(pgm, angulo);
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
