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

// --- Fungsi File Handling --- 
void simpanDataKeFile() {
    FILE *file = fopen(NAMA_FILE_DATA, "wb");
    if (file == NULL) {
        perror("ERROR: Gagal membuka file untuk menyimpan data");
        return;
    }

    // Tulis jumlah pasien
    if (fwrite(&jumlah_pasien, sizeof(int), 1, file) != 1) {
        printf("ERROR: Gagal menulis jumlah pasien ke file.\n");
        fclose(file);
        return;
    }

    // Tulis data pasien jika ada
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
        // Jika file tidak ada, ini bukan error, anggap saja belum ada data.
        // perror("Info: Gagal membuka file data (mungkin belum ada)");
        printf("Info: File data %s tidak ditemukan. Memulai dengan data kosong.\n", NAMA_FILE_DATA);
        jumlah_pasien = 0;
        daftar_pasien = NULL;
        return;
    }

    // Baca jumlah pasien
    if (fread(&jumlah_pasien, sizeof(int), 1, file) != 1) {
        if (feof(file)) { // File kosong, ini normal jika baru dibuat
             printf("Info: File data %s kosong. Memulai dengan data kosong.\n", NAMA_FILE_DATA);
             jumlah_pasien = 0;
             daftar_pasien = NULL;
        } else {
            printf("ERROR: Gagal membaca jumlah pasien dari file.\n");
            jumlah_pasien = 0; // Reset jika gagal baca
            daftar_pasien = NULL;
        }
        fclose(file);
        return;
    }

    // Jika jumlah pasien > 0, alokasikan memori dan baca data
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
            free(daftar_pasien); // Bebaskan memori yang dialokasikan
            daftar_pasien = NULL;
            jumlah_pasien = 0;
        } else {
             printf("Info: Berhasil memuat %d data pasien dari %s\n", jumlah_pasien, NAMA_FILE_DATA);
        }
    } else {
        // Jumlah pasien 0, tidak perlu alokasi
        daftar_pasien = NULL;
        printf("Info: Tidak ada data pasien yang dimuat dari file (jumlah pasien 0).\n");
    }

    fclose(file);
}


// --- Fungsi Utama Aplikasi --- 

void cekSyaratVaksin(Pasien p) {
    printf("--- Cek Syarat Vaksin HPV untuk %s ---\n", p.nama);
    int layak = 1;

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
    if (strcmp(p.status_vaksin, "lengkap") == 0) {
        printf("[OK] Vaksin HPV sudah lengkap (berdasarkan data).\n");
    }

    int usia_min_rekomendasi = 9;
    int usia_max_rekomendasi = 26;
    int usia_max_catchup = 45;
    int rekomendasi_usia_ditemukan = 0;

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
    } else { 
        printf("[X] Usia (%d thn) di bawah batas usia minimal (9 thn).\n", p.umur);
        layak = 0;
    }

    if (rekomendasi_usia_ditemukan && strcmp(p.status_vaksin, "belum") == 0) {
        if (p.dosis_vaksin < 3) {
            printf("[!] Perlu melanjutkan/memulai vaksinasi HPV (Dosis ke-%d).\n", p.dosis_vaksin + 1);
        } else {
            printf("[INFO] Telah menerima 3 dosis, status seharusnya 'lengkap'. Harap perbarui data.\n");
        }
    }

    if (p.imunokompromais == 1) {
        printf("[INFO] Pasien imunokompromais (HIV): mungkin perlu jadwal/dosis khusus.\n");
    }
    if (p.gangguan_darah == 1) {
        printf("[INFO] Pasien dg gangguan pembekuan darah: perlu perhatian khusus saat injeksi.\n");
    }

    if (layak && rekomendasi_usia_ditemukan && strcmp(p.status_vaksin, "belum") == 0) {
         printf("Kesimpulan: Vaksinasi HPV dapat dilanjutkan/dimulai.\n");
    } else if (!layak) {
         printf("Kesimpulan: Vaksinasi HPV tidak disarankan atau perlu ditunda saat ini.\n");
    } else if (strcmp(p.status_vaksin, "lengkap") == 0) {
         printf("Kesimpulan: Vaksinasi HPV sudah lengkap.\n");
    }
     printf("--------------------------------------------\n");
}

void tambahPasien() {
    // Gunakan realloc untuk menambah ukuran array
    Pasien *temp = realloc(daftar_pasien, (jumlah_pasien + 1) * sizeof(Pasien));
    if (temp == NULL) {
        printf("ERROR: Gagal mengalokasikan memori! Tidak bisa menambah pasien.\n");
        // Jika realloc gagal, daftar_pasien lama masih valid
        return;
    }
    daftar_pasien = temp;
    Pasien *p_baru = &daftar_pasien[jumlah_pasien]; // Pointer ke elemen baru
    char buffer[PANJANG_BUFFER];
    int input_int;

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

    p_baru->hamil = 0;
    if (strcmp(p_baru->jenis_kelamin, "wanita") == 0) {
        p_baru->hamil = bacaInteger("Apakah pasien sedang hamil? (1=Ya, 0=Tidak): ", 0, 1);
    }

    p_baru->alergi_berat_vaksin = bacaInteger("Apakah ada riwayat alergi berat thd komponen vaksin (ragi)? (1=Ya, 0=Tidak): ", 0, 1);
    p_baru->sakit_berat_saat_ini = bacaInteger("Apakah sedang sakit BERAT saat ini? (1=Ya, 0=Tidak): ", 0, 1);
    p_baru->imunokompromais = bacaInteger("Apakah pasien imunokompromais (misal, HIV)? (1=Ya, 0=Tidak): ", 0, 1);
    p_baru->gangguan_darah = bacaInteger("Apakah pasien memiliki gangguan pembekuan darah? (1=Ya, 0=Tidak): ", 0, 1);

    jumlah_pasien++; // Increment jumlah pasien SETELAH data baru siap
    printf("\nData pasien '%s' berhasil ditambahkan.\n", p_baru->nama);
    cekSyaratVaksin(*p_baru);
}

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

    printf("\nIngin melihat detail cek syarat vaksin untuk pasien tertentu? (1=Ya, 0=Tidak): ");
    int pilihan_cek = bacaInteger("", 0, 1);

    if (pilihan_cek == 1) {
        if (jumlah_pasien > 0) {
             int no_pasien = bacaInteger("Masukkan nomor pasien (1-N): ", 1, jumlah_pasien);
             cekSyaratVaksin(daftar_pasien[no_pasien - 1]);
        } else {
             printf("Tidak ada pasien untuk dicek.\n");
        }
    }
}

// --- Fungsi Edit Data --- 
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
    Pasien *p = &daftar_pasien[index_pasien]; // Pointer ke pasien yang dipilih

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
                    p->hamil = bacaInteger("Masukkan status hamil baru (1=Ya, 0=Tidak): ", 0, 1);
                    printf("Status hamil berhasil diubah.\n");
                } else {
                    printf("Tidak dapat mengubah status hamil untuk pasien pria.\n");
                }
                break;
            case 2: // Edit Status Sakit Berat
                p->sakit_berat_saat_ini = bacaInteger("Masukkan status sakit berat baru (1=Ya, 0=Tidak): ", 0, 1);
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
                        p->dosis_vaksin = 3; // Otomatis set 3 jika lengkap
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
                break; // Keluar dari switch
            default:
                 // Seharusnya tidak terjadi karena ada validasi bacaInteger
                 printf("Pilihan edit tidak valid.\n");
                 break;
        }
        // Setelah edit (kecuali kembali), cek ulang syarat vaksin
        if (pilihan_edit >= 1 && pilihan_edit <= 4) {
             printf("\nMemeriksa ulang syarat vaksin untuk %s setelah perubahan:\n", p->nama);
             cekSyaratVaksin(*p);
        }

    } while (pilihan_edit != 5); // Loop sampai pengguna memilih kembali
}

// --- Fungsi Utama (main) --- 
int main() {
    int pilihan;
    muatDataDariFile(); // Muat data saat program dimulai

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
                simpanDataKeFile(); // Simpan data sebelum keluar
                printf("Keluar dari program.\n");
                break;
            default:
                printf("Pilihan tidak valid. Masukkan angka 1-4.\n");
        }
    } while (pilihan != 4);

    free(daftar_pasien); // Bebaskan memori sebelum keluar
    printf("Terima kasih!\n");
    return 0;
}


