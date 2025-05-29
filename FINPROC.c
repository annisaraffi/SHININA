#include <stdio.h>  
#include <string.h>  
#include <stdlib.h> 

// Struktur Data Pasien 
#define PANJANG_NAMA 50
#define PANJANG_INFO 50
typedef struct {
    char nama[PANJANG_NAMA];
    int umur;
    char jenis_kelamin[10];
    char riwayat_penyakit[PANJANG_INFO];
    char riwayat_alergi[PANJANG_INFO];
    char status_vaksin[10];
    int dosis_vaksin;
    int hamil;
    int alergi_berat_vaksin;
    int sakit_berat_saat_ini;
    int imunokompromais;
    int gangguan_darah;
} Pasien;

// Penyimpanan Data Dinamis 
Pasien *daftar_pasien = NULL;
int jumlah_pasien = 0;

// Fungsi Cek Syarat Vaksin 
void cekSyaratVaksin(Pasien p) {
    printf("--- Cek Syarat Vaksin HPV untuk %s ---\n", p.nama);
    if (p.alergi_berat_vaksin == 1) {
        printf("[X] Tidak disarankan vaksinasi karena riwayat alergi berat terhadap komponen vaksin (ragi).\n");
        return;
    }
    if (p.sakit_berat_saat_ini == 1) {
        printf("[X] Tunda vaksinasi karena sedang sakit berat. Vaksinasi setelah sembuh.\n");
        return;
    }
    if (strcmp(p.jenis_kelamin, "wanita") == 0 && p.hamil == 1) {
        printf("[X] Tunda vaksinasi karena sedang hamil. Vaksinasi setelah melahirkan.\n");
        return;
    }
    if (strcmp(p.status_vaksin, "lengkap") == 0) {
        printf("[OK] Vaksin HPV sudah lengkap.\n");
        return;
    }
    int usia_min_rekomendasi = 9;
    int usia_max_rekomendasi = 26;
    int usia_max_catchup = 45;
    int rekomendasi_ditemukan = 0;
    if (p.umur >= usia_min_rekomendasi && p.umur <= usia_max_rekomendasi) {
        printf("[OK] Memenuhi syarat usia untuk vaksinasi HPV.\n");
        printf("[INFO] Usia %d tahun termasuk dalam rentang usia rekomendasi utama (9-26 tahun).\n", p.umur);
        rekomendasi_ditemukan = 1;
    } else if (p.umur > usia_max_rekomendasi && p.umur <= usia_max_catchup) {
        printf("[!] Usia %d tahun di luar rentang rekomendasi utama, namun masih dalam rentang catch-up (hingga 45 tahun).\n", p.umur);
        printf("[INFO] Pertimbangkan vaksinasi HPV setelah diskusi dengan dokter.\n");
        rekomendasi_ditemukan = 1;
    } else if (p.umur > usia_max_catchup) {
        printf("[X] Usia %d tahun melebihi batas usia rekomendasi catch-up vaksin HPV (45 tahun).\n", p.umur);
    } else {
        printf("[X] Usia %d tahun di bawah batas usia minimal rekomendasi vaksin HPV (9 tahun).\n", p.umur);
        return;
    }
    if (strcmp(p.status_vaksin, "belum") == 0 && rekomendasi_ditemukan) {
        if (p.dosis_vaksin < 3) {
            printf("[!] Perlu melanjutkan vaksinasi HPV (Dosis %d).\n", p.dosis_vaksin + 1);
        } else {
            printf("[INFO] Telah menerima 3 dosis, status seharusnya 'lengkap'. Harap perbarui data.\n");
        }
    }
    if (p.imunokompromais == 1) {
        printf("[INFO] Pasien imunokompromais (HIV) mungkin memerlukan jadwal/dosis khusus, diskusikan dengan dokter.\n");
    }
    if (p.gangguan_darah == 1) {
        printf("[INFO] Pasien dengan gangguan pembekuan darah perlu perhatian khusus saat injeksi, diskusikan dengan dokter.\n");
    }
}

// Fungsi Tambah Data Pasien 
void tambahPasien() {
    Pasien *temp = realloc(daftar_pasien, (jumlah_pasien + 1) * sizeof(Pasien));
    if (temp == NULL) {
        printf("ERROR: Gagal mengalokasikan memori! Tidak bisa menambah pasien.\n");
        return;
    }
    daftar_pasien = temp;
    Pasien p_baru;
    printf("--- Input Data Pasien ke-%d ---\n", jumlah_pasien + 1);
    printf("Nama: ");
    scanf(" %[^\n]", p_baru.nama);
    printf("Umur: ");
    scanf("%d", &p_baru.umur);
    printf("Jenis kelamin (pria/wanita): ");
    scanf("%9s", p_baru.jenis_kelamin);
    while (strcmp(p_baru.jenis_kelamin, "pria") != 0 && strcmp(p_baru.jenis_kelamin, "wanita") != 0) {
        printf("Input tidak valid. Ketik 'pria' atau 'wanita': ");
        scanf("%9s", p_baru.jenis_kelamin);
    }
    printf("Riwayat penyakit: ");
    scanf(" %[^\n]", p_baru.riwayat_penyakit);
    printf("Riwayat alergi: ");
    scanf(" %[^\n]", p_baru.riwayat_alergi);
    printf("Status vaksin HPV (belum/lengkap): ");
    scanf("%9s", p_baru.status_vaksin);
    while (strcmp(p_baru.status_vaksin, "belum") != 0 && strcmp(p_baru.status_vaksin, "lengkap") != 0) {
        printf("Input tidak valid. Ketik 'belum' atau 'lengkap': ");
        scanf("%9s", p_baru.status_vaksin);
    }
    p_baru.dosis_vaksin = (strcmp(p_baru.status_vaksin, "belum") == 0) ? 0 : 3;
    if (strcmp(p_baru.status_vaksin, "belum") == 0) {
        printf("Jumlah dosis vaksin HPV yang sudah diterima (0-3): ");
        scanf("%d", &p_baru.dosis_vaksin);
        while (p_baru.dosis_vaksin < 0 || p_baru.dosis_vaksin > 3) {
            printf("Input tidak valid. Masukkan angka 0-3: ");
            scanf("%d", &p_baru.dosis_vaksin);
        }
    }
    p_baru.hamil = 0;
    if (strcmp(p_baru.jenis_kelamin, "wanita") == 0) {
        printf("Apakah pasien sedang hamil? (1=Ya, 0=Tidak): ");
        scanf("%d", &p_baru.hamil);
    }
    printf("Apakah ada alergi terhadap ragi? (1=Ya, 0=Tidak): ");
    scanf("%d", &p_baru.alergi_berat_vaksin);
    printf("Apakah sedang sakit BERAT saat ini? (1=Ya, 0=Tidak): ");
    scanf("%d", &p_baru.sakit_berat_saat_ini);
    printf("Apakah pasien imunokompromais (HIV)? (1=Ya, 0=Tidak): ");
    scanf("%d", &p_baru.imunokompromais);
    printf("Apakah pasien memiliki gangguan pembekuan darah? (1=Ya, 0=Tidak): ");
    scanf("%d", &p_baru.gangguan_darah);
    daftar_pasien[jumlah_pasien] = p_baru;
    jumlah_pasien++;
    printf("Data pasien berhasil ditambahkan.\n");
    cekSyaratVaksin(p_baru);
}

// Fungsi tampil semua pasien
void tampilkanSemuaPasien() {
    printf("\n--- Daftar Semua Pasien ---\n");
    if (jumlah_pasien == 0) {
        printf("Belum ada data pasien.\n");
        return;
    }
    for (int i = 0; i < jumlah_pasien; i++) {
        printf("\nPasien ke-%d:\n", i + 1);
        printf("Nama: %s\n", daftar_pasien[i].nama);
        printf("Umur: %d\n", daftar_pasien[i].umur);
        printf("Jenis Kelamin: %s\n", daftar_pasien[i].jenis_kelamin);
        printf("Riwayat Penyakit: %s\n", daftar_pasien[i].riwayat_penyakit);
        printf("Riwayat Alergi: %s\n", daftar_pasien[i].riwayat_alergi);
        printf("Status Vaksin HPV: %s\n", daftar_pasien[i].status_vaksin);
        printf("Dosis Vaksin: %d\n", daftar_pasien[i].dosis_vaksin);
        cekSyaratVaksin(daftar_pasien[i]);
    }
}

// Fungsi utama
int main() {
    int pilihan;
    do {
        printf("\n========== MENU MANAJEMEN VAKSIN HPV ==========\n");
        printf("1. Tambah Data Pasien & Cek Syarat Vaksin\n");
        printf("2. Tampilkan Semua Data Pasien & Hasil Cek Syarat\n");
        printf("3. Keluar\n");
        printf("===============================================\n");
        printf("Jumlah Pasien Tersimpan: %d\n", jumlah_pasien);
        printf("Pilih menu: ");
        if (scanf("%d", &pilihan) != 1) {
            printf("Input tidak valid. Masukkan angka 1-3.\n");
            while (getchar() != '\n'); // bersihkan input buffer
            continue;
        }
        switch (pilihan) {
            case 1:
                tambahPasien();
                break;
            case 2:
                tampilkanSemuaPasien();
                break;
            case 3:
                printf("Keluar dari program. Terima kasih!\n");
                break;
            default:
                printf("Pilihan tidak valid. Masukkan angka 1-3.\n");
        }
    } while (pilihan != 3);

    free(daftar_pasien);
    return 0;
}