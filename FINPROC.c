#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struktur data pasien
typedef struct {
    char nama[100];
    int umur;
    float berat;
    float tinggi;
    float bmi;
    char riwayatPenyakit[200];
    char alergi[200];
    char statusVaksin[100];
} Pasien;

