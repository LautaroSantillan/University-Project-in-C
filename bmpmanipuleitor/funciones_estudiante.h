#ifndef FUNCIONES_ESTUDIANTE_H_INCLUDED
#define FUNCIONES_ESTUDIANTE_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estructuras.h"

int solucion(int argc, char *argv[]);
int leerBMP(const char *nombreArchivo, t_metadata *metadata, t_pixel **imagen);
int guardarBMP(const char *nombreArchivo, const t_metadata *metadata, const t_pixel *imagen);
int convertirEscalaGrises(const char *nombreArchivo);
int convertirTonalidadRojo(const char *nombreArchivo);
int convertirTonalidadAzul(const char *nombreArchivo);
int convertirTonalidadVerde(const char *nombreArchivo);
int convertirTonalidadNegativo(const char *nombreArchivo);
int ajustarContraste(const char *nombreArchivo, float factor, const char *palabra);
int aumentarContraste(const char *nombreArchivo);
int reducirContraste(const char *nombreArchivo);
int recortar(const char *nombreArchivo);
int rotarDerecha(const char *nombreArchivo);
int rotarIzquierda(const char *nombreArchivo);
int concatenar(const char* nombreArchivo, const char* nombreArchivo2);

#endif // FUNCIONES_ESTUDIANTE_H_INCLUDED
