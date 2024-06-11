/*
    Integrantes del grupo. En caso de ser un grupo de dos integrantes, no completar el último campo.
    Si alguno de los integrantes del grupo dejara la materia, completar de todos modos sus datos, aclarando que no entrega.
    -----------------
    Apellido: Santillan
    Nombre: Lautaro Ezequiel
    DNI: 45175053
    Entrega: Si
    -----------------
    Apellido: Ibarra
    Nombre: Lucas Nicolas
    DNI: 41332340
    Entrega: Si
    -----------------
*/
#include "funciones_estudiante.h"
#include "estructuras.h"
#include "constantes.h"
//bmpmanipuleitor.exe   --funciones   nombreArchivo.bmp -> cmd

int solucion(int argc, char *argv[]) {
    if (argc < 3)
        return ERR_ARG;

    const char *nombreArchivo = argv[argc - 1];

    for (int i = 1; i < argc - 1; i++)
    {
        if (strcmp(argv[i], "--escala-de-grises") == 0)
        {
            if (convertirEscalaGrises(nombreArchivo) != TODO_OK)
            {
                puts("Error al convertir la imagen a escala de grises.");
                return NO_SE_PUEDE_CREAR_ARCHIVO;
            }
        }
        else if (strcmp(argv[i], "--tonalidad-azul") == 0)
        {
            if (convertirTonalidadAzul(nombreArchivo) != TODO_OK)
            {
                puts("Error al convertir la imagen a tonalidad azul.");
                return NO_SE_PUEDE_CREAR_ARCHIVO;
            }
        }
        else if (strcmp(argv[i], "--tonalidad-negativa") == 0)
        {
            if (convertirTonalidadNegativo(nombreArchivo) != TODO_OK)
            {
                puts("Error al convertir la imagen a tonalidad negativa.");
                return NO_SE_PUEDE_CREAR_ARCHIVO;
            }
        }
        else if (strcmp(argv[i], "--rotar-derecha") == 0)
        {
            if (rotarDerecha(nombreArchivo) != TODO_OK)
            {
                puts("Error al rotar la imagen a la derecha.");
                return NO_SE_PUEDE_CREAR_ARCHIVO;
            }
        }
        else if (strcmp(argv[i], "--tonalidad-roja") == 0)
        {
            if (convertirTonalidadRojo(nombreArchivo) != TODO_OK)
            {
                puts("Error al convertir la imagen a tonalidad rojo.");
                return NO_SE_PUEDE_CREAR_ARCHIVO;
            }
        }
        else if (strcmp(argv[i], "--tonalidad-verde") == 0)
        {
            if (convertirTonalidadVerde(nombreArchivo) != TODO_OK)
            {
                puts("Error al convertir la imagen a tonalidad verde.");
                return NO_SE_PUEDE_CREAR_ARCHIVO;
            }
        }
        else if (strcmp(argv[i], "--rotar-izquierda") == 0)
        {
            if (rotarIzquierda(nombreArchivo) != TODO_OK)
            {
                puts("Error al rotar la imagen a la izquierda.");
                return NO_SE_PUEDE_CREAR_ARCHIVO;
            }
        }
        else if (strcmp(argv[i], "--recortar") == 0)
        {
            if (recortar(nombreArchivo) != TODO_OK)
            {
                puts("Error al recortar la imagen.");
                return NO_SE_PUEDE_CREAR_ARCHIVO;
            }
        }
        else if (strcmp(argv[i], "--aumentar-contraste") == 0)
        {
            if (aumentarContraste(nombreArchivo) != TODO_OK)
            {
                puts("Error al aumentar el contraste.");
                return NO_SE_PUEDE_CREAR_ARCHIVO;
            }
        }
        else if (strcmp(argv[i], "--reducir-contraste") == 0)
        {
            if (reducirContraste(nombreArchivo) != TODO_OK)
            {
                puts("Error al reducir el contraste.");
                return NO_SE_PUEDE_CREAR_ARCHIVO;
            }
        }
        else
        {
            printf("Opcion no reconocida: %s", argv[i]);
            return ARCHIVO_NO_ENCONTRADO;
        }
    }

    puts("Modificaciones realizadas con exito!");
    return TODO_OK;
}

int leerBMP(const char *nombreArchivo, t_metadata *metadata, t_pixel **imagen)
{
    FILE *archivo = fopen(nombreArchivo, "rb");
    if (!archivo)
    {
        puts("No se puede abrir el archivo");
        return ARCHIVO_NO_ENCONTRADO;
    }

    unsigned int inicioIMG;
    fseek(archivo, sizeof(int) / 2, SEEK_SET);
    fread(&metadata->tamArchivo, sizeof(unsigned int), 1, archivo);
    fseek(archivo, sizeof(int), SEEK_CUR);
    fread(&inicioIMG, sizeof(unsigned int), 1, archivo);
    fread(&metadata->tamEncabezado, sizeof(unsigned int), 1, archivo);
    fread(&metadata->ancho, sizeof(unsigned int), 1, archivo);
    fread(&metadata->alto, sizeof(unsigned int), 1, archivo);
    fseek(archivo, sizeof(int) / 2, SEEK_CUR);
    fread(&metadata->profundidad, sizeof(unsigned short), 1, archivo);

    *imagen = (t_pixel*)malloc(metadata->ancho * metadata->alto * sizeof(t_pixel));
    if (!(*imagen))
    {
        puts("No se pudo reservar memoria para la imagen");
        fclose(archivo);
        return ARCHIVO_NO_ENCONTRADO;
    }

    fseek(archivo, inicioIMG, SEEK_SET);
    int padding = (4 - (metadata->ancho * 3) % 4) % 4;

    for (int i = 0; i < metadata->alto; i++)
    {
        for (int j = 0; j < metadata->ancho; j++)
        {
            fread((*imagen)[(metadata->alto - 1 - i) * metadata->ancho + j].pixel, sizeof(unsigned char), 3, archivo);
            (*imagen)[(metadata->alto - 1 - i) * metadata->ancho + j].profundidad = metadata->profundidad;
        }
        fseek(archivo, padding, SEEK_CUR);
    }

    fclose(archivo);
    return TODO_OK;
}

int guardarBMP(const char *nombreArchivo, const t_metadata *metadata, const t_pixel *imagen)
{
    FILE *archivo = fopen(nombreArchivo, "wb");
    if (!archivo)
    {
        puts("No se puede abrir el archivo");
        return ARCHIVO_NO_ENCONTRADO;
    }

    unsigned int inicioIMG = 54; // Offset de los datos de imagen

    // Escribir el encabezado BMP
    fseek(archivo, 0, SEEK_SET);
    putc('B', archivo);
    putc('M', archivo);

    // Escribir tamaño del archivo
    fwrite(&metadata->tamArchivo, sizeof(unsigned int), 1, archivo);

    // Reservado (4 bytes)
    unsigned int reservado = 0;
    fwrite(&reservado, sizeof(unsigned int), 1, archivo);

    // Offset de los datos de la imagen
    fwrite(&inicioIMG, sizeof(unsigned int), 1, archivo);

    // Escribir el encabezado
    fseek(archivo, 14, SEEK_SET);
    unsigned int tamEncabezado = 40;
    fwrite(&tamEncabezado, sizeof(unsigned int), 1, archivo);
    fwrite(&metadata->ancho, sizeof(unsigned int), 1, archivo);
    fwrite(&metadata->alto, sizeof(unsigned int), 1, archivo);

    unsigned short planos = 1;
    fwrite(&planos, sizeof(unsigned short), 1, archivo);
    fwrite(&metadata->profundidad, sizeof(unsigned short), 1, archivo);

    unsigned int compresion = 0;
    fwrite(&compresion, sizeof(unsigned int), 1, archivo);

    unsigned int tamImagen = metadata->ancho * metadata->alto * 3;
    fwrite(&tamImagen, sizeof(unsigned int), 1, archivo);

    unsigned int resX = 0, resY = 0;
    fwrite(&resX, sizeof(unsigned int), 1, archivo);
    fwrite(&resY, sizeof(unsigned int), 1, archivo);

    unsigned int coloresUsados = 0, coloresImportantes = 0;
    fwrite(&coloresUsados, sizeof(unsigned int), 1, archivo);
    fwrite(&coloresImportantes, sizeof(unsigned int), 1, archivo);

    // Calcular padding
    int padding = (4 - (metadata->ancho * 3) % 4) % 4;

    // Escribir los datos de la imagen
    fseek(archivo, inicioIMG, SEEK_SET);
    for (int i = 0; i < metadata->alto; i++)
    {
        for (int j = 0; j < metadata->ancho; j++)
            fwrite(imagen[(metadata->alto - 1 - i) * metadata->ancho + j].pixel, sizeof(unsigned char), 3, archivo);
        for (int k = 0; k < padding; k++)
            putc(0, archivo);
    }

    fclose(archivo);
    return TODO_OK;
}

int convertirEscalaGrises(const char *nombreArchivo)
{
    t_metadata metadata;
    t_pixel *imagen;

    if (leerBMP(nombreArchivo, &metadata, &imagen) != TODO_OK)
        return ARCHIVO_NO_ENCONTRADO;

    for (int i = 0; i < metadata.ancho * metadata.alto; i++)
    {
        unsigned char gris = (unsigned char)((imagen[i].pixel[2] + imagen[i].pixel[1] + imagen[i].pixel[0]) / 3);
        imagen[i].pixel[0] = gris;
        imagen[i].pixel[1] = gris;
        imagen[i].pixel[2] = gris;
    }

    char nombreArchivoGris[255] = "reyes_escala-de-grises.bmp";

    if (guardarBMP(nombreArchivoGris, &metadata, imagen) != TODO_OK)
    {
        free(imagen);
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

    free(imagen);
    return TODO_OK;
}

int convertirTonalidadRojo(const char *nombreArchivo)
{
    t_metadata metadata;
    t_pixel *imagen;

    if (leerBMP(nombreArchivo, &metadata, &imagen) != TODO_OK)
        return ARCHIVO_NO_ENCONTRADO;

    for (int i = 0; i < metadata.ancho * metadata.alto; i++)
    {
        if(imagen[i].pixel[2] * 1.5 > 255)
            imagen[i].pixel[2] = 255;
        else
            imagen[i].pixel[2] *= 1.5;
    }

    char nombreArchivoRojo[255] = "reyes_tonalidad-roja.bmp";

    if (guardarBMP(nombreArchivoRojo, &metadata, imagen) != TODO_OK)
    {
        free(imagen);
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

    free(imagen);
    return TODO_OK;
}

int convertirTonalidadAzul(const char *nombreArchivo)
{
    t_metadata metadata;
    t_pixel *imagen;

    if (leerBMP(nombreArchivo, &metadata, &imagen) != TODO_OK)
        return ARCHIVO_NO_ENCONTRADO;

    for (int i = 0; i < metadata.ancho * metadata.alto; i++)
    {
        if(imagen[i].pixel[0] * 1.5 > 255)
            imagen[i].pixel[0] = 255;
        else
            imagen[i].pixel[0] *= 1.5;
    }

    char nombreArchivoAzul[255] = "reyes_tonalidad-azul.bmp";

    if (guardarBMP(nombreArchivoAzul, &metadata, imagen) != TODO_OK)
    {
        free(imagen);
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

    free(imagen);
    return TODO_OK;
}

int convertirTonalidadVerde(const char *nombreArchivo)
{
    t_metadata metadata;
    t_pixel *imagen;

    if (leerBMP(nombreArchivo, &metadata, &imagen) != TODO_OK)
        return ARCHIVO_NO_ENCONTRADO;

    for (int i = 0; i < metadata.ancho * metadata.alto; i++)
    {
        if(imagen[i].pixel[1] * 1.5 > 255)
            imagen[i].pixel[1] = 255;
        else
            imagen[i].pixel[1] *= 1.5;
    }

    char nombreArchivoVerde[255] = "reyes_tonalidad-verde.bmp";

    if (guardarBMP(nombreArchivoVerde, &metadata, imagen) != TODO_OK)
    {
        free(imagen);
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

    free(imagen);
    return TODO_OK;
}

int convertirTonalidadNegativo(const char *nombreArchivo)
{
    t_metadata metadata;
    t_pixel *imagen;

    if (leerBMP(nombreArchivo, &metadata, &imagen) != TODO_OK)
        return ARCHIVO_NO_ENCONTRADO;

    for (int i = 0; i < metadata.ancho * metadata.alto; i++)
    {
        imagen[i].pixel[0] = 255 - imagen[i].pixel[0]; // azul
        imagen[i].pixel[1] = 255 - imagen[i].pixel[1]; // verde
        imagen[i].pixel[2] = 255 - imagen[i].pixel[2]; // roja
    }

    char nombreArchivoNegativo[255] = "reyes_tonalidad-negativa.bmp";

    if (guardarBMP(nombreArchivoNegativo, &metadata, imagen) != TODO_OK)
    {
        free(imagen);
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

    free(imagen);
    return TODO_OK;
}

int ajustarContraste(const char *nombreArchivo, float factor, const char *palabra)
{
    t_metadata metadata;
    t_pixel *imagen;

    if (leerBMP(nombreArchivo, &metadata, &imagen) != TODO_OK)
        return ARCHIVO_NO_ENCONTRADO;

    const unsigned char media = 128;
    for (int i = 0; i < metadata.ancho * metadata.alto; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            int nuevoValor = media + (imagen[i].pixel[j] - media) * factor;
            imagen[i].pixel[j] = nuevoValor > 255 ? 255 : (nuevoValor < 0 ? 0 : nuevoValor);
        }
    }

    char nombreArchivoContraste[255] = "reyes_";
    strcat(nombreArchivoContraste, palabra);
    strcat(nombreArchivoContraste, "-contraste.bmp");

    if (guardarBMP(nombreArchivoContraste, &metadata, imagen) != TODO_OK)
    {
        free(imagen);
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

    free(imagen);
    return TODO_OK;
}

int aumentarContraste(const char *nombreArchivo)
{
    const char *palabra = "aumentar";
    return ajustarContraste(nombreArchivo, 1.25, palabra);
}

int reducirContraste(const char *nombreArchivo)
{
    const char *palabra = "reducir";
    return ajustarContraste(nombreArchivo, 0.75, palabra);
}

int recortar(const char *nombreArchivo)
{
    t_metadata metadata;
    t_pixel *imagen;

    if (leerBMP(nombreArchivo, &metadata, &imagen) != TODO_OK)
        return ARCHIVO_NO_ENCONTRADO;

    // Nuevas dimensiones
    unsigned int nuevoAncho = metadata.ancho / 2;
    unsigned int nuevoAlto = metadata.alto / 2;

    t_pixel *nuevaImagen = (t_pixel *)malloc(nuevoAncho * nuevoAlto * sizeof(t_pixel));
    if (!nuevaImagen)
    {
        free(imagen);
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

    // Copiar los píxeles
    for (unsigned int i = 0; i < nuevoAlto; i++) // y
    {
        for (unsigned int j = 0; j < nuevoAncho; j++) // x
            nuevaImagen[i * nuevoAncho + j] = imagen[i * metadata.ancho + j];
    }

    // Actualizar metadatos
    metadata.ancho = nuevoAncho;
    metadata.alto = nuevoAlto;
    metadata.tamArchivo = 54 + nuevoAncho * nuevoAlto * sizeof(t_pixel);

    char nombreArchivoRecortada[255] = "reyes_recortada.bmp";

    if (guardarBMP(nombreArchivoRecortada, &metadata, nuevaImagen) != TODO_OK)
    {
        free(imagen);
        free(nuevaImagen);
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

    free(imagen);
    free(nuevaImagen);
    return TODO_OK;
}

int rotarDerecha(const char *nombreArchivo)
{
    t_metadata metadata;
    t_pixel *imagen;

    if (leerBMP(nombreArchivo, &metadata, &imagen) != TODO_OK)
        return ARCHIVO_NO_ENCONTRADO;

    unsigned int nuevoAncho = metadata.alto;
    unsigned int nuevoAlto = metadata.ancho;

    t_pixel *nuevaImagen = (t_pixel *)malloc(nuevoAncho * nuevoAlto * sizeof(t_pixel));
    if (!nuevaImagen)
    {
        free(imagen);
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

    // Rotar 90 grados a la derecha
    for (unsigned int i = 0; i < metadata.alto; i++) // y
    {
        for (unsigned int j = 0; j < metadata.ancho; j++)
            nuevaImagen[j * nuevoAncho + (nuevoAncho - i - 1)] = imagen[i * metadata.ancho + j];
    }

    // Actualizar metadatos
    metadata.ancho = nuevoAncho;
    metadata.alto = nuevoAlto;
    metadata.tamArchivo = 54 + nuevoAncho * nuevoAlto * sizeof(t_pixel);

    char nombreArchivoRotada[255] = "reyes_rotar-derecha.bmp";

    if (guardarBMP(nombreArchivoRotada, &metadata, nuevaImagen) != TODO_OK)
    {
        free(imagen);
        free(nuevaImagen);
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

    free(imagen);
    free(nuevaImagen);
    return TODO_OK;
}

int rotarIzquierda(const char *nombreArchivo)
{
    t_metadata metadata;
    t_pixel *imagen;

    if (leerBMP(nombreArchivo, &metadata, &imagen) != TODO_OK)
        return ARCHIVO_NO_ENCONTRADO;

    unsigned int nuevoAncho = metadata.alto;
    unsigned int nuevoAlto = metadata.ancho;

    t_pixel *nuevaImagen = (t_pixel *)malloc(nuevoAncho * nuevoAlto * sizeof(t_pixel));
    if (!nuevaImagen)
    {
        free(imagen);
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

    // Rotar 90 grados a la izquierda
    for (unsigned int i = 0; i < metadata.alto; i++) // y
    {
        for (unsigned int j = 0; j < metadata.ancho; j++) // j
            nuevaImagen[(nuevoAlto - j - 1) * nuevoAncho + i] = imagen[i * metadata.ancho + j];

    }

    // Actualizar metadatos
    metadata.ancho = nuevoAncho;
    metadata.alto = nuevoAlto;
    metadata.tamArchivo = 54 + nuevoAncho * nuevoAlto * sizeof(t_pixel);

    char nombreArchivoRotada[255] = "reyes_rotar-izquierda.bmp";

    if (guardarBMP(nombreArchivoRotada, &metadata, nuevaImagen) != TODO_OK)
    {
        free(imagen);
        free(nuevaImagen);
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

    free(imagen);
    free(nuevaImagen);
    return TODO_OK;
}

int concatenar(const char* nombreArchivo, const char* nombreArchivo2)
{
t_metadata metadata, metadata2;
    t_pixel *img = NULL;
    t_pixel *img2 = NULL;
    t_pixel *imgCon = NULL;
    unsigned char color[3] = {173, 216, 230};

    if (leerBMP(nombreArchivo, &metadata, &img) != TODO_OK)
        return ARCHIVO_NO_ENCONTRADO;

    if (leerBMP(nombreArchivo2, &metadata2, &img2) != TODO_OK) {
        free(img);
        return ARCHIVO_NO_ENCONTRADO;
    }

    int nuevoAncho = (metadata.ancho < metadata2.ancho) ? metadata2.ancho : metadata.ancho;
    int nuevoAlto = metadata.alto + metadata2.alto;

    imgCon = malloc(nuevoAlto * nuevoAncho * sizeof(t_pixel));
    if (!imgCon) {
        free(img);
        free(img2);
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

    // Rellenar con nuevo color
    for (int i = 0; i < nuevoAlto; i++) {
        for (int j = 0; j < nuevoAncho; j++) {
            imgCon[i * nuevoAncho + j].pixel[0] = color[0];
            imgCon[i * nuevoAncho + j].pixel[1] = color[1];
            imgCon[i * nuevoAncho + j].pixel[2] = color[2];
            imgCon[i * nuevoAncho + j].profundidad = img[0].profundidad; // Suponiendo que todas las imÃ¡genes tienen la misma profundidad
        }
    }

    // Copiar primera imagen
    for (int i = 0; i < metadata.alto; i++) {
        for (int j = 0; j < metadata.ancho; j++) {
            imgCon[i * nuevoAncho + j] = img[i * metadata.ancho + j];
        }
    }

    // Copiar segunda imagen
    for (int i = 0; i < metadata2.alto; i++) {
        for (int j = 0; j < metadata2.ancho; j++) {
            imgCon[(i + metadata.alto) * nuevoAncho + j] = img2[i * metadata2.ancho + j];
        }
    }

    char nombreArchivoConcatenar[255] = "reyes_concatenar.bmp";

    if (guardarBMP(nombreArchivoConcatenar, &metadata, imgCon) != TODO_OK) {
        free(img);
        free(img2);
        free(imgCon);
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

    free(img);
    free(img2);
    free(imgCon);

    return TODO_OK;
}
