#include <stdio.h>
#include <string.h>

#define MAX_PASIEN 100

// Struct untuk data pasien
struct Pasien {
    char nama[50];
    int umur;
    float berat;
    float tinggi;
    float bmi;
    char vaksin[20];
    char alergi[100];
    char riwayatPenyakit[100];
};

// Function untuk hitung BMI
float hitungBMI(float berat, float tinggi) {
    return berat / (tinggi * tinggi);
}

// Function untuk cek syarat vaksin
void cekSyaratVaksin(struct Pasien p) {
    printf("\n-- Cek Syarat Vaksin untuk %s --\n", p.nama);
    if (p.umur < 12) {
        printf("Pasien belum cukup umur untuk vaksin (minimal 12 tahun).\n");
    } else if (strcmp(p.vaksin, "Lengkap") == 0) {
        printf("Pasien sudah vaksin lengkap.\n");
    } else {
        printf("Pasien boleh mendapatkan vaksin.\n");
    }
}

// Menu utama
int main() {
    struct Pasien daftarPasien[MAX_PASIEN];
    int jumlahPasien = 0;
    int pilihan;

    do {
        printf("\n===== MENU =====\n");
        printf("1. Tambah Data Pasien\n");
        printf("2. Tampilkan Data Semua Pasien\n");
        printf("3. Keluar\n");
        printf("Pilih: ");
        scanf("%d", &pilihan);
        getchar(); // membersihkan newline di buffer

        switch (pilihan) {
            case 1: {
                struct Pasien p;
                printf("\n--- Input Data Pasien ---\n");
                printf("Nama: ");
                fgets(p.nama, sizeof(p.nama), stdin);
                p.nama[strcspn(p.nama, "\n")] = 0; // hapus newline

                printf("Umur: ");
                scanf("%d", &p.umur);

                printf("Berat badan (kg): ");
                scanf("%f", &p.berat);

                printf("Tinggi badan (m): ");
                scanf("%f", &p.tinggi);
                getchar(); // buffer

                p.bmi = hitungBMI(p.berat, p.tinggi);
                printf("BMI: %.2f\n", p.bmi);

                printf("Status vaksin (Belum / Lengkap): ");
                fgets(p.vaksin, sizeof(p.vaksin), stdin);
                p.vaksin[strcspn(p.vaksin, "\n")] = 0;

                printf("Riwayat penyakit: ");
                fgets(p.riwayatPenyakit, sizeof(p.riwayatPenyakit), stdin);
                p.riwayatPenyakit[strcspn(p.riwayatPenyakit, "\n")] = 0;

                printf("Alergi: ");
                fgets(p.alergi, sizeof(p.alergi), stdin);
                p.alergi[strcspn(p.alergi, "\n")] = 0;

                // Simpan ke array
                daftarPasien[jumlahPasien++] = p;

                // Cek vaksin
                cekSyaratVaksin(p);
                break;
            }
            case 2: {
                printf("\n--- Data Semua Pasien ---\n");
                for (int i = 0; i < jumlahPasien; i++) {
                    printf("\nPasien #%d:\n", i + 1);
                    printf("Nama: %s\n", daftarPasien[i].nama);
                    printf("Umur: %d\n", daftarPasien[i].umur);
                    printf("Berat: %.2f kg\n", daftarPasien[i].berat);
                    printf("Tinggi: %.2f m\n", daftarPasien[i].tinggi);
                    printf("BMI: %.2f\n", daftarPasien[i].bmi);
                    printf("Vaksin: %s\n", daftarPasien[i].vaksin);
                    printf("Riwayat Penyakit: %s\n", daftarPasien[i].riwayatPenyakit);
                    printf("Alergi: %s\n", daftarPasien[i].alergi);
                }
                break;
            }
            case 3:
                printf("Terima kasih!\n");
                break;
            default:
                printf("Pilihan tidak valid.\n");
        }
    } while (pilihan != 3);

    return 0;
}

