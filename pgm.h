/* 
 * Biblioteca para leitura de arquivos .pgm (P2 ou P5).
 * de Bruno Krügel.
*/

#ifndef PGM_H
#define PGM_H

typedef struct pgm {
    char type[3];
    int width;
    int height;
    int max_value;
    unsigned char **pixel;
} type_pgm;

/* Cria e aloca um type_pgm de tamanho width por height com todos os pixeis brancos. */
/* Retorna NULL em caso de erro. */
type_pgm *criar_pgm(char type[2],int width, int height, int max_value);

/* Abre o arquivo, le o conteudo , cria e monta um tipo pgm com seus dados. */
/* Retorna NULL em caso de erro. */
type_pgm *ler_pgm(FILE *in_filen);

/* Faz uma copia da imagem pgm e retorna o ponteiro dessa copia. */
/* Retorna NULL em caso de erro. */
type_pgm *copiar_pgm(type_pgm *pgm);

/* Escreve o pgm no arquivo.*/
/* Em caso de sucesso, returna 0, se não, returna 1. */
int escrever_pgm(type_pgm *pgm, FILE *out_file);

/* Desaloca a memória criada para os pixeis da imagem pgm. */
void destruir_pgm(type_pgm *pgm);

#endif