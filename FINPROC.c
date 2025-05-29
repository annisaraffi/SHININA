#include <stdio.h>
#include <string.h>

#define MAKS 100

struct Pasien {
    char nama[50];
    int umur;
    char jenis_kelamin[10];
    char status_vaksin[10]; // "belum" / "lengkap"
    char riwayat_penyakit[100];
    char riwayat_alergi[100];
    int hamil;
    int alergi_berat;
    int sakit_berat;
    int imunokompromais;
    int gangguan_pembekuan_darah;
};

struct Pasien data[MAKS];
int jumlah_pasien = 0;

void cekSyaratVaksin(struct Pasien p) {
    printf("\n--- Cek Syarat Vaksin HPV untuk %s ---\n", p.nama);

    if (p.umur < 9) {
        printf("[X] Usia terlalu muda untuk vaksin HPV.\n");
    } else if (p.umur <= 26) {
        printf("[OK] Direkomendasikan untuk vaksin HPV.\n");
    } else if (p.umur <= 45) {
        printf("[!] Pertimbangkan lagi vaksin HPV (diskusi dengan dokter diperlukan).\n");
    } else {
        printf("[X] Usia melebihi batas rekomendasi vaksin HPV.\n");
    }

    if (p.hamil)
        printf("[X] Pasien sedang hamil. Tunda vaksinasi hingga setelah melahirkan.\n");

    if (p.alergi_berat)
        printf("[X] Pasien memiliki alergi berat. Tidak disarankan vaksinasi.\n");

    if (p.sakit_berat)
        printf("[X] Pasien sedang sakit berat. Tunda vaksinasi sampai sembuh.\n");

    if (strcmp(p.status_vaksin, "belum") == 0)
        printf("[!] Pasien belum vaksin lengkap. Perlu melanjutkan vaksinasi.\n");
    else
        printf("[OK] Vaksin HPV sudah lengkap.\n");
}

void tambahPasien() {
    struct Pasien p;

    printf("\n--- Input Data Pasien ---\n");
    printf("Nama: ");
    scanf(" %[^\n]", p.nama);
    printf("Umur: ");
    scanf("%d", &p.umur);
    printf("Jenis kelamin (pria/wanita): ");
    scanf(" %[^\n]", p.jenis_kelamin);

    // Jika pasien pria, langsung keluar
    if (strcmp(p.jenis_kelamin, "pria") == 0) {
        printf("Program ini hanya untuk pasien wanita. Vaksin HPV direkomendasikan khusus untuk wanita.\n");
        return;
    }

    printf("Status vaksin HPV (belum/lengkap): ");
    scanf(" %[^\n]", p.status_vaksin);
    printf("Riwayat penyakit: ");
    scanf(" %[^\n]", p.riwayat_penyakit);
    printf("Riwayat alergi: ");
    scanf(" %[^\n]", p.riwayat_alergi);

    printf("Apakah pasien sedang hamil? (1=Ya, 0=Tidak): ");
    scanf("%d", &p.hamil);
    printf("Apakah ada alergi berat? (1=Ya, 0=Tidak): ");
    scanf("%d", &p.alergi_berat);
    printf("Apakah sedang sakit berat? (1=Ya, 0=Tidak): ");
    scanf("%d", &p.sakit_berat);
    printf("Apakah pasien imunokompromais? (1=Ya, 0=Tidak): ");
    scanf("%d", &p.imunokompromais);
    printf("Apakah pasien memiliki gangguan pembekuan darah? (1=Ya, 0=Tidak): ");
    scanf("%d", &p.gangguan_pembekuan_darah);

    data[jumlah_pasien] = p;
    jumlah_pasien++;

    printf("\nData pasien berhasil ditambahkan.\n");
    cekSyaratVaksin(p);
}

void tampilkanSemuaPasien() {
    printf("\n=== Semua Data Pasien ===\n");

    for (int i = 0; i < jumlah_pasien; i++) {
        struct Pasien p = data[i];
        printf("\nPasien ke-%d:\n", i + 1);
        printf("Nama: %s\n", p.nama);
        printf("Umur: %d\n", p.umur);
        printf("Jenis kelamin: %s\n", p.jenis_kelamin);
        printf("Status vaksin HPV: %s\n", p.status_vaksin);
        printf("Riwayat penyakit: %s\n", p.riwayat_penyakit);
        printf("Riwayat alergi: %s\n", p.riwayat_alergi);

        cekSyaratVaksin(p);
    }
}

int main() {
    int pilihan;

    do {
        printf("\n====== MENU ======\n");
        printf("1. Tambah Data Pasien\n");
        printf("2. Tampilkan Semua Data Pasien\n");
        printf("3. Keluar\n");
        printf("Pilih menu: ");
        scanf("%d", &pilihan);

        switch (pilihan) {
            case 1:
                tambahPasien();
                break;
            case 2:
                tampilkanSemuaPasien();
                break;
            case 3:
                printf("Keluar dari program.\n");
                break;
            default:
                printf("Pilihan tidak valid.\n");
        }
    } while (pilihan != 3);

    return 0;
}
