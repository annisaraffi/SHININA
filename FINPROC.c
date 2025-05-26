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

// Fungsi untuk menghitung BMI
float hitungBMI(float berat, float tinggi) {
    return berat / (tinggi * tinggi);
}

// Fungsi untuk menampilkan menu
void tampilkanMenu() {
    printf("\n=== Menu ===\n");
    printf("1. Input data pasien\n");
    printf("2. Tampilkan semua data pasien\n");
    printf("3. Cek syarat vaksin pasien\n");
    printf("0. Keluar\n");
    printf("Pilihan Anda: ");
}

// Fungsi input data pasien
void inputDataPasien(Pasien *p) {
    printf("Nama: ");
    scanf(" %[^\n]", p->nama);

    printf("Umur: ");
    scanf("%d", &p->umur);

    printf("Berat badan (kg): ");
    scanf("%f", &p->berat);

    printf("Tinggi badan (m): ");
    scanf("%f", &p->tinggi);

    p->bmi = hitungBMI(p->berat, p->tinggi);

    printf("Riwayat penyakit: ");
    scanf(" %[^\n]", p->riwayatPenyakit);

    printf("Alergi: ");
    scanf(" %[^\n]", p->alergi);

    printf("Status vaksin (Belum/Lengkap): ");
    scanf(" %[^\n]", p->statusVaksin);

    printf("\nData berhasil disimpan!\n");
}

// Tampilkan seluruh data pasien
void tampilkanSemuaPasien(Pasien *list, int jumlah) {
    if (jumlah == 0) {
        printf("Belum ada data pasien.\n");
        return;
    }
    for (int i = 0; i < jumlah; i++) {
        printf("\n--- Data Pasien #%d ---\n", i + 1);
        printf("Nama             : %s\n", list[i].nama);
        printf("Umur             : %d\n", list[i].umur);
        printf("Berat            : %.2f kg\n", list[i].berat);
        printf("Tinggi           : %.2f m\n", list[i].tinggi);
        printf("BMI              : %.2f\n", list[i].bmi);
        printf("Riwayat Penyakit : %s\n", list[i].riwayatPenyakit);
        printf("Alergi           : %s\n", list[i].alergi);
        printf("Status Vaksin    : %s\n", list[i].statusVaksin);
    }
}

// Cek syarat vaksin untuk pasien tertentu
void cekSyaratVaksin(Pasien *list, int jumlah) {
    if (jumlah == 0) {
        printf("Belum ada data pasien.\n");
        return;
    }

    int idx;
    printf("Masukkan nomor pasien (1-%d): ", jumlah);
    scanf("%d", &idx);
    idx--;

    if (idx < 0 || idx >= jumlah) {
        printf("Nomor pasien tidak valid.\n");
        return;
    }

    Pasien p = list[idx];
    printf("\nCek syarat vaksin untuk %s:\n", p.nama);
    if (p.umur < 12) {
        printf("- Umur belum memenuhi syarat vaksin.\n");
    } else if (strcmp(p.statusVaksin, "Lengkap") == 0) {
        printf("- Vaksin sudah lengkap.\n");
    } else {
        printf("- Pasien memenuhi syarat untuk vaksin.\n");
    }
}

// Program utama
int main() {
    int kapasitas = 5;
    Pasien *pasienList = (Pasien *)malloc(kapasitas * sizeof(Pasien));
    int jumlahPasien = 0;
    int pilihan;

    do {
        tampilkanMenu();
        scanf("%d", &pilihan);

        switch (pilihan) {
            case 1:
                if (jumlahPasien == kapasitas) {
                    kapasitas *= 2;
                    pasienList = (Pasien *)realloc(pasienList, kapasitas * sizeof(Pasien));
                }
                inputDataPasien(&pasienList[jumlahPasien++]);
                break;
            case 2:
                tampilkanSemuaPasien(pasienList, jumlahPasien);
                break;
            case 3:
                cekSyaratVaksin(pasienList, jumlahPasien);
                break;
            case 0:
                printf("Terima kasih telah menggunakan ShiNiNa!\n");
                break;
            default:
                printf("Pilihan tidak valid, coba lagi.\n");
        }
    } while (pilihan != 0);

    free(pasienList);
    return 0;
}
