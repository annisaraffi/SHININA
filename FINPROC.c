#include <stdio.h>
#include <string.h>

#define MAX 100

struct Pasien {
    char nama[50];
    int umur;
    float berat, tinggi, bmi;
    char vaksin[10]; // "Belum" atau "Lengkap"
    char riwayat_penyakit[100];
    char alergi[100];
    int hamil;
    int alergi_berat;
    int sakit_berat;
    int imunokompromais;
    int gangguan_pembekuan;
};

struct Pasien data[MAX];
int jumlah = 0;

// Fungsi menghitung BMI
float hitungBMI(float berat, float tinggi) {
    return berat / (tinggi * tinggi);
}

// Fungsi cek kelayakan vaksin HPV
void cekSyaratVaksin(struct Pasien p) {
    printf("\n--- Cek Syarat Vaksin HPV untuk %s ---\n", p.nama);

    int layak = 1;

    if (p.hamil == 1) {
        printf("[X] Pasien sedang hamil. Tunda vaksinasi.\n");
        layak = 0;
    }
    if (p.alergi_berat == 1) {
        printf("[X] Pasien memiliki alergi berat. Tidak direkomendasikan untuk vaksin HPV.\n");
        layak = 0;
    }
    if (p.sakit_berat == 1) {
        printf("[X] Pasien sedang sakit berat. Tunda vaksinasi.\n");
        layak = 0;
    }
    if (p.gangguan_pembekuan == 1) {
        printf("[!] Pasien memiliki gangguan pembekuan darah. Konsultasikan dulu ke dokter.\n");
        layak = 0;
    }

    if (layak && p.umur >= 9 && p.umur <= 45) {
        printf("[OK] Direkomendasikan untuk vaksin HPV.\n");

        if (strcmp(p.vaksin, "Belum") == 0) {
            printf("[!] Pasien belum vaksin lengkap. Perlu melanjutkan vaksinasi.\n");
        } else {
            printf("[OK] Vaksin HPV sudah lengkap.\n");
        }
    } else if (layak) {
        printf("[!] Usia di luar rentang rekomendasi. Konsultasikan ke dokter.\n");
    }
}

// Menampilkan semua data pasien
void tampilkanData() {
    printf("\n=== Semua Data Pasien ===\n");
    for (int i = 0; i < jumlah; i++) {
        printf("\nPasien ke-%d:\n", i + 1);
        printf("Nama: %s\n", data[i].nama);
        printf("Umur: %d\n", data[i].umur);
        printf("Berat badan: %.2f kg\n", data[i].berat);
        printf("Tinggi badan: %.2f m\n", data[i].tinggi);
        printf("BMI: %.2f\n", data[i].bmi);
        printf("Status vaksin HPV: %s\n", data[i].vaksin);
        printf("Riwayat penyakit: %s\n", data[i].riwayat_penyakit);
        printf("Riwayat alergi: %s\n", data[i].alergi);
        cekSyaratVaksin(data[i]);
    }
}

int main() {
    int pilihan;

    do {
        printf("\n======= MENU =======\n");
        printf("1. Tambah Data Pasien\n");
        printf("2. Tampilkan Semua Data Pasien\n");
        printf("3. Keluar\n");
        printf("Pilih menu: ");
        scanf("%d", &pilihan);
        getchar(); // menangkap newline

        switch (pilihan) {
            case 1:
                printf("\n--- Input Data Pasien ---\n");
                printf("Nama: ");
                fgets(data[jumlah].nama, sizeof(data[jumlah].nama), stdin);
                strtok(data[jumlah].nama, "\n");

                printf("Umur: ");
                scanf("%d", &data[jumlah].umur);

                printf("Berat badan (kg): ");
                scanf("%f", &data[jumlah].berat);

                printf("Tinggi badan (meter): ");
                scanf("%f", &data[jumlah].tinggi);

                data[jumlah].bmi = hitungBMI(data[jumlah].berat, data[jumlah].tinggi);

                getchar();
                printf("Status vaksin HPV (Belum/Lengkap): ");
                fgets(data[jumlah].vaksin, sizeof(data[jumlah].vaksin), stdin);
                strtok(data[jumlah].vaksin, "\n");

                printf("Riwayat penyakit: ");
                fgets(data[jumlah].riwayat_penyakit, sizeof(data[jumlah].riwayat_penyakit), stdin);
                strtok(data[jumlah].riwayat_penyakit, "\n");

                printf("Riwayat alergi: ");
                fgets(data[jumlah].alergi, sizeof(data[jumlah].alergi), stdin);
                strtok(data[jumlah].alergi, "\n");

                printf("Apakah pasien sedang hamil? (1=Ya, 0=Tidak): ");
                scanf("%d", &data[jumlah].hamil);

                printf("Apakah ada alergi berat (misalnya ragi)? (1=Ya, 0=Tidak): ");
                scanf("%d", &data[jumlah].alergi_berat);

                printf("Apakah sedang sakit berat? (1=Ya, 0=Tidak): ");
                scanf("%d", &data[jumlah].sakit_berat);

                printf("Apakah pasien imunokompromais (misal HIV)? (1=Ya, 0=Tidak): ");
                scanf("%d", &data[jumlah].imunokompromais);

                printf("Apakah pasien memiliki gangguan pembekuan darah? (1=Ya, 0=Tidak): ");
                scanf("%d", &data[jumlah].gangguan_pembekuan);

                jumlah++;
                break;

            case 2:
                if (jumlah == 0) {
                    printf("Belum ada data pasien.\n");
                } else {
                    tampilkanData();
                }
                break;

            case 3:
                printf("Terima kasih!\n");
                break;

            default:
                printf("Pilihan tidak valid. Coba lagi.\n");
        }

    } while (pilihan != 3);

    return 0;
}


