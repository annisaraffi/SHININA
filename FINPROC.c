#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h> // Untuk tolower()

// Struktur Data Pasien
#define PANJANG_NAMA 50
#define PANJANG_INFO 50
#define PANJANG_BUFFER 100 // Buffer untuk input string
#define NAMA_FILE_DATA "pasien_data.bin"

typedef struct {
    char nama[PANJANG_NAMA];
    int umur;
    char jenis_kelamin[10]; // pria/wanita
    char riwayat_penyakit[PANJANG_INFO];
    char riwayat_alergi[PANJANG_INFO];
    char status_vaksin[10]; // belum/lengkap
    int dosis_vaksin;
    int hamil; // 0 = Tidak, 1 = Ya
    int alergi_berat_vaksin; // 0 = Tidak, 1 = Ya (alergi ragi)
    int sakit_berat_saat_ini; // 0 = Tidak, 1 = Ya
    int imunokompromais; // 0 = Tidak, 1 = Ya (HIV)
    int gangguan_darah; // 0 = Tidak, 1 = Ya
} Pasien;

// Penyimpanan Data Dinamis
Pasien *daftar_pasien = NULL;
int jumlah_pasien = 0;

// --- Fungsi Helper --- 
void bersihkan_buffer_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Membaca string, konversi ke lowercase
int bacaStringLower(const char *prompt, char *buffer, int size) {
    char temp_buffer[PANJANG_BUFFER];
    printf("%s", prompt);
    if (fgets(temp_buffer, sizeof(temp_buffer), stdin) == NULL) {
        buffer[0] = '\0';
        return 0;
    }
    temp_buffer[strcspn(temp_buffer, "\n")] = 0;
    if (strlen(temp_buffer) >= size) {
        printf("ERROR: Input terlalu panjang (maks %d karakter).\n", size - 1);
        if (strchr(temp_buffer, '\n') == NULL) {
            bersihkan_buffer_stdin();
        }
        buffer[0] = '\0';
        return 0;
    }
    for (int i = 0; temp_buffer[i]; i++) {
        buffer[i] = tolower(temp_buffer[i]);
    }
    buffer[strlen(temp_buffer)] = '\0';
    return 1;
}

// Membaca integer dalam rentang tertentu
int bacaInteger(const char *prompt, int min_val, int max_val) {
    int value;
    printf("%s", prompt);
    while (scanf("%d", &value) != 1 || value < min_val || value > max_val) {
        printf("Input tidak valid. Masukkan angka antara %d dan %d: ", min_val, max_val);
        bersihkan_buffer_stdin();
    }
    bersihkan_buffer_stdin();
    return value;
}

// Membaca input Ya/Tidak (case-insensitive, termasuk singkatan y/t)
int bacaYaTidak(const char *prompt) {
    char buffer[PANJANG_BUFFER];
    while (1) {
        // Gunakan prompt lengkap termasuk pilihan
        printf("%s (ya/tidak/y/t): ", prompt);
        if (!bacaStringLower("", buffer, sizeof(buffer))) continue; // Coba lagi jika baca gagal

        if (strcmp(buffer, "ya") == 0 || strcmp(buffer, "y") == 0) {
            return 1; // Ya
        } else if (strcmp(buffer, "tidak") == 0 || strcmp(buffer, "t") == 0) {
            return 0; // Tidak
        } else {
            printf("Input tidak valid. Ketik 'ya'/'y' atau 'tidak'/'t'.\n");
        }
    }
}

// --- Fungsi File Handling --- 
void simpanDataKeFile() {
    FILE *file = fopen(NAMA_FILE_DATA, "wb");
    if (file == NULL) {
        perror("ERROR: Gagal membuka file untuk menyimpan data");
        return;
    }
    if (fwrite(&jumlah_pasien, sizeof(int), 1, file) != 1) {
        printf("ERROR: Gagal menulis jumlah pasien ke file.\n");
        fclose(file);
        return;
    }
    if (jumlah_pasien > 0) {
        if (fwrite(daftar_pasien, sizeof(Pasien), jumlah_pasien, file) != jumlah_pasien) {
            printf("ERROR: Gagal menulis data pasien ke file.\n");
        } else {
            printf("Data berhasil disimpan ke %s\n", NAMA_FILE_DATA);
        }
    } else {
        printf("Tidak ada data pasien untuk disimpan (file %s diperbarui/dikosongkan).\n", NAMA_FILE_DATA);
    }
    fclose(file);
}

void muatDataDariFile() {
    FILE *file = fopen(NAMA_FILE_DATA, "rb");
    if (file == NULL) {
        printf("Info: File data %s tidak ditemukan. Memulai dengan data kosong.\n", NAMA_FILE_DATA);
        jumlah_pasien = 0;
        daftar_pasien = NULL;
        return;
    }
    if (fread(&jumlah_pasien, sizeof(int), 1, file) != 1) {
        if (feof(file)) {
            printf("Info: File data %s kosong. Memulai dengan data kosong.\n", NAMA_FILE_DATA);
            jumlah_pasien = 0;
            daftar_pasien = NULL;
        } else {
            printf("ERROR: Gagal membaca jumlah pasien dari file.\n");
            jumlah_pasien = 0;
            daftar_pasien = NULL;
        }
        fclose(file);
        return;
    }
    if (jumlah_pasien > 0) {
        daftar_pasien = (Pasien *)malloc(jumlah_pasien * sizeof(Pasien));
        if (daftar_pasien == NULL) {
            printf("ERROR: Gagal mengalokasikan memori untuk memuat data pasien!\n");
            jumlah_pasien = 0;
            fclose(file);
            return;
        }
        if (fread(daftar_pasien, sizeof(Pasien), jumlah_pasien, file) != jumlah_pasien) {
            printf("ERROR: Gagal membaca data pasien dari file (data mungkin korup).\n");
            free(daftar_pasien);
            daftar_pasien = NULL;
            jumlah_pasien = 0;
        } else {
            printf("Info: Berhasil memuat %d data pasien dari %s\n", jumlah_pasien, NAMA_FILE_DATA);
        }
    } else {
        daftar_pasien = NULL;
        printf("Info: Tidak ada data pasien yang dimuat dari file (jumlah pasien 0).\n");
    }
    fclose(file);
}

// --- Fungsi Utama Aplikasi --- 

// Revisi: Jika sudah lengkap, hanya tampilkan itu.
void cekSyaratVaksin(Pasien p) {
    printf("--- Cek Syarat Vaksin HPV untuk %s ---\n", p.nama);

    // Cek paling awal: Jika sudah lengkap, selesai.
    if (strcmp(p.status_vaksin, "lengkap") == 0) {
        printf("[OK] Vaksin HPV sudah lengkap (berdasarkan data).\n");
        printf("Kesimpulan: Vaksinasi HPV sudah lengkap.\n");
        printf("--------------------------------------------\n");
        return;
    }

    // Jika belum lengkap, lanjutkan pengecekan syarat
    int layak = 1;
    int rekomendasi_usia_ditemukan = 0;

    // Cek kontraindikasi utama
    if (p.alergi_berat_vaksin == 1) {
        printf("[X] Tidak disarankan: Riwayat alergi berat thd komponen vaksin (ragi).\n");
        layak = 0;
    }
    if (p.sakit_berat_saat_ini == 1) {
        printf("[X] Tunda: Sedang sakit berat. Vaksinasi setelah sembuh.\n");
        layak = 0;
    }
    if (strcmp(p.jenis_kelamin, "wanita") == 0 && p.hamil == 1) {
        printf("[X] Tunda: Sedang hamil. Vaksinasi setelah melahirkan.\n");
        layak = 0;
    }

    // Jika ada kontraindikasi, langsung ke kesimpulan
    if (!layak) {
        printf("Kesimpulan: Vaksinasi HPV tidak disarankan atau perlu ditunda saat ini.\n");
        printf("--------------------------------------------\n");
        return;
    }

    // Lanjut cek usia hanya jika tidak ada kontraindikasi
    int usia_min_rekomendasi = 9;
    int usia_max_rekomendasi = 26;
    int usia_max_catchup = 45;

    if (p.umur >= usia_min_rekomendasi && p.umur <= usia_max_rekomendasi) {
        printf("[OK] Usia (%d thn) masuk rentang rekomendasi utama (9-26 thn).\n", p.umur);
        rekomendasi_usia_ditemukan = 1;
    } else if (p.umur > usia_max_rekomendasi && p.umur <= usia_max_catchup) {
        printf("[!] Usia (%d thn) di luar rekomendasi utama, masuk rentang catch-up (27-45 thn).\n", p.umur);
        printf("    -> Pertimbangkan vaksinasi setelah diskusi dg dokter.\n");
        rekomendasi_usia_ditemukan = 1;
    } else if (p.umur > usia_max_catchup) {
        printf("[X] Usia (%d thn) melebihi batas usia catch-up (45 thn).\n", p.umur);
        layak = 0;
    } else { // p.umur < usia_min_rekomendasi
        printf("[X] Usia (%d thn) di bawah batas usia minimal (9 thn).\n", p.umur);
        layak = 0;
    }

    // Jika usia tidak memenuhi syarat, langsung ke kesimpulan
    if (!layak) {
        printf("Kesimpulan: Vaksinasi HPV tidak disarankan atau perlu ditunda saat ini.\n");
        printf("--------------------------------------------\n");
        return;
    }

    // Cek Dosis hanya jika usia memungkinkan dan tidak ada kontraindikasi
    if (p.dosis_vaksin < 3) {
        printf("[!] Perlu melanjutkan/memulai vaksinasi HPV (Dosis ke-%d).\n", p.dosis_vaksin + 1);
    } else {
        printf("[INFO] Telah menerima 3 dosis, status seharusnya 'lengkap'. Harap perbarui data.\n");
    }

    // Info Tambahan
    if (p.imunokompromais == 1) {
        printf("[INFO] Pasien imunokompromais (HIV): mungkin perlu jadwal/dosis khusus.\n");
    }
    if (p.gangguan_darah == 1) {
        printf("[INFO] Pasien dg gangguan pembekuan darah: perlu perhatian khusus saat injeksi.\n");
    }

    // Kesimpulan akhir jika semua syarat terpenuhi
    printf("Kesimpulan: Vaksinasi HPV dapat dilanjutkan/dimulai.\n");
    printf("--------------------------------------------\n");
}

// Revisi: Gunakan bacaYaTidak untuk input boolean
void tambahPasien() {
    Pasien *temp = realloc(daftar_pasien, (jumlah_pasien + 1) * sizeof(Pasien));
    if (temp == NULL) {
        printf("ERROR: Gagal mengalokasikan memori! Tidak bisa menambah pasien.\n");
        return;
    }
    daftar_pasien = temp;
    Pasien *p_baru = &daftar_pasien[jumlah_pasien];
    char buffer[PANJANG_BUFFER];

    // Perubahan: Tampilkan nomor pasien sebagai jumlah_pasien + 1 (bukan jumlah_pasien + 1)
    printf("--- Input Data Pasien ke-%d ---\n", jumlah_pasien + 1);

    printf("Nama Lengkap: ");
    fgets(p_baru->nama, PANJANG_NAMA, stdin);
    p_baru->nama[strcspn(p_baru->nama, "\n")] = 0;
    if (strlen(p_baru->nama) == 0) { 
        fgets(p_baru->nama, PANJANG_NAMA, stdin);
        p_baru->nama[strcspn(p_baru->nama, "\n")] = 0;
    }

    p_baru->umur = bacaInteger("Umur (tahun): ", 0, 150);

    while (1) {
        if (!bacaStringLower("Jenis Kelamin (pria/wanita/p/w): ", buffer, sizeof(buffer))) continue;
        if (strcmp(buffer, "pria") == 0 || strcmp(buffer, "p") == 0) {
            strcpy(p_baru->jenis_kelamin, "pria");
            break;
        } else if (strcmp(buffer, "wanita") == 0 || strcmp(buffer, "w") == 0) {
            strcpy(p_baru->jenis_kelamin, "wanita");
            break;
        } else {
            printf("Input tidak valid. Ketik 'pria'/'p' atau 'wanita'/'w'.\n");
        }
    }

    printf("Riwayat Penyakit (jika tidak ada, ketik '-'): ");
    fgets(p_baru->riwayat_penyakit, PANJANG_INFO, stdin);
    p_baru->riwayat_penyakit[strcspn(p_baru->riwayat_penyakit, "\n")] = 0;

    printf("Riwayat Alergi (jika tidak ada, ketik '-'): ");
    fgets(p_baru->riwayat_alergi, PANJANG_INFO, stdin);
    p_baru->riwayat_alergi[strcspn(p_baru->riwayat_alergi, "\n")] = 0;

    while (1) {
        if (!bacaStringLower("Status Vaksin HPV (belum/lengkap/b/l): ", buffer, sizeof(buffer))) continue;
        if (strcmp(buffer, "belum") == 0 || strcmp(buffer, "b") == 0) {
            strcpy(p_baru->status_vaksin, "belum");
            break;
        } else if (strcmp(buffer, "lengkap") == 0 || strcmp(buffer, "l") == 0) {
            strcpy(p_baru->status_vaksin, "lengkap");
            break;
        } else {
            printf("Input tidak valid. Ketik 'belum'/'b' atau 'lengkap'/'l'.\n");
        }
    }

    p_baru->dosis_vaksin = 0;
    if (strcmp(p_baru->status_vaksin, "belum") == 0) {
        p_baru->dosis_vaksin = bacaInteger("Jumlah dosis vaksin HPV yg sudah diterima (0-3): ", 0, 3);
    } else {
        p_baru->dosis_vaksin = 3;
    }

    // Gunakan bacaYaTidak
    p_baru->hamil = 0;
    if (strcmp(p_baru->jenis_kelamin, "wanita") == 0) {
        p_baru->hamil = bacaYaTidak("Apakah pasien sedang hamil?");
    }
    p_baru->alergi_berat_vaksin = bacaYaTidak("Apakah ada riwayat alergi berat thd komponen vaksin (ragi)?");
    p_baru->sakit_berat_saat_ini = bacaYaTidak("Apakah sedang sakit BERAT saat ini?");
    p_baru->imunokompromais = bacaYaTidak("Apakah pasien imunokompromais (misal, HIV)?");
    p_baru->gangguan_darah = bacaYaTidak("Apakah pasien memiliki gangguan pembekuan darah?");

    jumlah_pasien++;
    printf("\nData pasien '%s' berhasil ditambahkan.\n", p_baru->nama);
    cekSyaratVaksin(*p_baru);
}

// Revisi: Gunakan bacaYaTidak untuk opsi cek detail
void tampilkanSemuaPasien() {
    printf("\n--- Daftar Semua Pasien (%d orang) ---\n", jumlah_pasien);
    if (jumlah_pasien == 0) {
        printf("Belum ada data pasien.\n");
        return;
    }
    printf("--------------------------------------------------------------------------------------------------------\n");
    printf("No | %-*s | Umur | Sex | Status HPV | Dosis | Hamil | Alergi Ragi | Sakit Berat | Imuno | Ggn Darah | Riwayat Penyakit / Alergi\n", PANJANG_NAMA, "Nama");
    printf("--------------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < jumlah_pasien; i++) {
        printf("%2d | %-*s | %3d  | %-3s | %-10s | %-5d | %-5s | %-11s | %-11s | %-5s | %-9s | %s / %s\n",
            i + 1,
            PANJANG_NAMA, daftar_pasien[i].nama,
            daftar_pasien[i].umur,
            (strcmp(daftar_pasien[i].jenis_kelamin, "pria") == 0) ? "P" : "W",
            daftar_pasien[i].status_vaksin,
            daftar_pasien[i].dosis_vaksin,
            (strcmp(daftar_pasien[i].jenis_kelamin, "wanita") == 0) ? (daftar_pasien[i].hamil ? "Ya" : "Tidak") : "-",
            daftar_pasien[i].alergi_berat_vaksin ? "Ya" : "Tidak",
            daftar_pasien[i].sakit_berat_saat_ini ? "Ya" : "Tidak",
            daftar_pasien[i].imunokompromais ? "Ya" : "Tidak",
            daftar_pasien[i].gangguan_darah ? "Ya" : "Tidak",
            daftar_pasien[i].riwayat_penyakit, daftar_pasien[i].riwayat_alergi);
    }
    printf("--------------------------------------------------------------------------------------------------------\n");
    printf("Keterangan: Sex(P/W), Status HPV(belum/lengkap), Hamil(Ya/Tidak/-), Alergi Ragi(Ya/Tidak), Sakit Berat(Ya/Tidak), Imuno(Ya/Tidak), Ggn Darah(Ya/Tidak)\n");

    // Gunakan bacaYaTidak
    int pilihan_cek = bacaYaTidak("\nIngin melihat detail cek syarat vaksin untuk pasien tertentu?");

    if (pilihan_cek == 1) {
        if (jumlah_pasien > 0) {
            int no_pasien = bacaInteger("Masukkan nomor pasien (1-N): ", 1, jumlah_pasien);
            cekSyaratVaksin(daftar_pasien[no_pasien - 1]);
        } else {
            printf("Tidak ada pasien untuk dicek.\n");
        }
    }
}

// Revisi: Gunakan bacaYaTidak untuk edit boolean
void editPasien() {
    if (jumlah_pasien == 0) {
        printf("Belum ada data pasien untuk diedit.\n");
        return;
    }

    printf("\n--- Edit Data Pasien ---\n");
    printf("Daftar Pasien:\n");
    for (int i = 0; i < jumlah_pasien; i++) {
        printf("%d. %s\n", i + 1, daftar_pasien[i].nama);
    }

    int no_pasien = bacaInteger("Pilih nomor pasien yang akan diedit (1-N): ", 1, jumlah_pasien);
    int index_pasien = no_pasien - 1;
    Pasien *p = &daftar_pasien[index_pasien];

    printf("\nData Pasien Terpilih: %s\n", p->nama);
    int pilihan_edit;
    char buffer[PANJANG_BUFFER];

    do {
        printf("\n--- Menu Edit untuk %s ---\n", p->nama);
        if (strcmp(p->jenis_kelamin, "wanita") == 0) {
            printf("1. Status Hamil: %s\n", p->hamil ? "Ya" : "Tidak");
        } else {
            printf("1. Status Hamil: (Tidak Berlaku untuk Pria)\n");
        }
        printf("2. Status Sakit Berat Saat Ini: %s\n", p->sakit_berat_saat_ini ? "Ya" : "Tidak");
        printf("3. Status Vaksin HPV: %s\n", p->status_vaksin);
        printf("4. Jumlah Dosis Vaksin: %d (Hanya relevan jika status 'belum')\n", p->dosis_vaksin);
        printf("5. Kembali ke Menu Utama\n");

        pilihan_edit = bacaInteger("Pilih data yang ingin diubah (1-5): ", 1, 5);

        switch (pilihan_edit) {
            case 1: // Edit Status Hamil
                if (strcmp(p->jenis_kelamin, "wanita") == 0) {
                    // Gunakan bacaYaTidak
                    p->hamil = bacaYaTidak("Masukkan status hamil baru");
                    printf("Status hamil berhasil diubah.\n");
                } else {
                    printf("Tidak dapat mengubah status hamil untuk pasien pria.\n");
                }
                break;
            case 2: // Edit Status Sakit Berat
                // Gunakan bacaYaTidak
                p->sakit_berat_saat_ini = bacaYaTidak("Masukkan status sakit berat baru");
                printf("Status sakit berat berhasil diubah.\n");
                break;
            case 3: // Edit Status Vaksin HPV
                while (1) {
                    if (!bacaStringLower("Masukkan status vaksin HPV baru (belum/lengkap/b/l): ", buffer, sizeof(buffer))) continue;
                    if (strcmp(buffer, "belum") == 0 || strcmp(buffer, "b") == 0) {
                        strcpy(p->status_vaksin, "belum");
                        p->dosis_vaksin = bacaInteger("Masukkan jumlah dosis yang sudah diterima (0-3): ", 0, 3);
                        printf("Status vaksin dan dosis berhasil diubah.\n");
                        break;
                    } else if (strcmp(buffer, "lengkap") == 0 || strcmp(buffer, "l") == 0) {
                        strcpy(p->status_vaksin, "lengkap");
                        p->dosis_vaksin = 3;
                        printf("Status vaksin berhasil diubah menjadi 'lengkap' (dosis otomatis 3).\n");
                        break;
                    } else {
                        printf("Input tidak valid. Ketik 'belum'/'b' atau 'lengkap'/'l'.\n");
                    }
                }
                break;
            case 4: // Edit Jumlah Dosis Vaksin
                if (strcmp(p->status_vaksin, "belum") == 0) {
                    p->dosis_vaksin = bacaInteger("Masukkan jumlah dosis baru (0-3): ", 0, 3);
                    printf("Jumlah dosis berhasil diubah.\n");
                } else {
                    printf("Tidak dapat mengubah dosis jika status vaksin adalah 'lengkap'. Ubah status ke 'belum' terlebih dahulu jika perlu.\n");
                }
                break;
            case 5: // Kembali
                printf("Selesai mengedit data %s. Kembali ke menu utama.\n", p->nama);
                break;
            default:
                printf("Pilihan edit tidak valid.\n");
                break;
        }
        if (pilihan_edit >= 1 && pilihan_edit <= 4) {
            printf("\nMemeriksa ulang syarat vaksin untuk %s setelah perubahan:\n", p->nama);
            cekSyaratVaksin(*p);
        }
    } while (pilihan_edit != 5);
}

// --- Fungsi Utama (main) --- 
int main() {
    int pilihan;
    muatDataDariFile();

    do {
        printf("\n========== MENU MANAJEMEN VAKSIN HPV ==========\n");
        printf("1. Tambah Data Pasien & Cek Syarat Vaksin\n");
        printf("2. Tampilkan Semua Data Pasien\n");
        printf("3. Edit Data Pasien\n"); 
        printf("4. Keluar\n"); 
        printf("===============================================\n");
        printf("Jumlah Pasien Tersimpan: %d\n", jumlah_pasien);
        printf("Pilih menu (1-4): ");

        if (scanf("%d", &pilihan) != 1) {
            printf("Input tidak valid. Masukkan angka menu.\n");
            bersihkan_buffer_stdin();
            pilihan = 0; 
            continue;
        }
        bersihkan_buffer_stdin(); 

        switch (pilihan) {
            case 1:
                tambahPasien();
                break;
            case 2:
                tampilkanSemuaPasien();
                break;
            case 3:
                editPasien(); 
                break;
            case 4:
                printf("Menyimpan data sebelum keluar...\n");
                simpanDataKeFile();
                printf("Keluar dari program.\n");
                break;
            default:
                printf("Pilihan tidak valid. Masukkan angka 1-4.\n");
        }
    } while (pilihan != 4);

    free(daftar_pasien);
    printf("Terima kasih!\n");
    return 0;
}
