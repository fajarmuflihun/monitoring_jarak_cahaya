# Sistem Monitoring Jarak dan Cahaya Berbasis ESP32

Proyek **Responsi 1 Internet of Things (IoT)** menggunakan **ESP32 DevKit V1**, sensor **Ultrasonic HC-SR04**, dan **LDR digital 3 pin**. Sistem menggabungkan pengukuran jarak objek dan kondisi cahaya untuk menampilkan status peringatan melalui Serial Monitor. ESP32 terhubung ke jaringan Wi-Fi 2,4 GHz.

## Dokumentasi foto

**Foto rangkaian pada tahap perakitan** (belum merupakan bukti verifikasi akhir sambungan):

![Foto rangkaian ESP32, Ultrasonic, dan LDR](dokumentasi/rangkaian-awal.jpg)

**Hasil pengujian Serial Monitor:**

![Hasil pembacaan Ultrasonic dan LDR](dokumentasi/hasil-serial-monitor.jpg)

> Foto rangkaian diambil pada tahap perakitan. Pastikan sambungan final sesuai tabel, khususnya perlindungan jalur ECHO, sebelum mengoperasikan rangkaian.

## Komponen dan fungsi

| Komponen | Fungsi |
|---|---|
| ESP32 DevKit V1 | Membaca sensor, memproses logika, menghubungkan Wi-Fi |
| Ultrasonic HC-SR04 | Mengukur jarak objek menggunakan gelombang ultrasonik |
| Modul LDR digital 3 pin | Membedakan kondisi terang/gelap berdasarkan ambang modul |
| Breadboard dan kabel jumper | Media penghubung rangkaian |
| Resistor 1 kΩ dan 2 kΩ | Pembagi tegangan sinyal ECHO dari sekitar 5V ke sekitar 3,3V |

## Koneksi pin

| Komponen | Pin sensor | Pin ESP32 |
|---|---|---|
| LDR | VCC | 3V3 |
| LDR | GND | GND |
| LDR | OUT | GPIO 34 |
| HC-SR04 | VCC | VIN / 5V |
| HC-SR04 | GND | GND |
| HC-SR04 | TRIG | GPIO 5 |
| HC-SR04 | ECHO | GPIO 18 **melalui pembagi tegangan** |

**Keselamatan:** ECHO HC-SR04 dapat mengeluarkan sinyal 5V dan **tidak boleh langsung** dihubungkan ke GPIO ESP32. Hubungkan ECHO → resistor 1 kΩ → titik GPIO 18; dari titik tersebut hubungkan resistor 2 kΩ → GND. Gunakan ground bersama. Foto yang tersedia tidak membuktikan pemasangan pembagi tegangan; verifikasi fisiknya tetap diperlukan.

## Alur kerja sistem

1. ESP32 melakukan inisialisasi sensor dan menghubungkan Wi-Fi.
2. Ultrasonic mengirim pulsa pemicu lalu mengukur durasi pantulan untuk menghitung jarak (cm).
3. LDR digital membaca kondisi cahaya. Berdasarkan pengujian modul yang digunakan: **HIGH = gelap**, **LOW = terang**.
4. ESP32 menggabungkan kondisi jarak dengan kondisi cahaya.
5. Hasilnya ditampilkan di Serial Monitor setiap 1 detik. Jika Wi-Fi terputus, program mencoba menyambungkan ulang secara berkala.

## Logika sistem

Ambang objek dekat adalah **50 cm**.

| Jarak | Cahaya | Status |
|---|---|---|
| ≤ 50 cm | Gelap | PERINGATAN! Objek dekat saat gelap |
| ≤ 50 cm | Terang | Objek terdeteksi dalam jarak dekat |
| > 50 cm | Gelap | Ruangan gelap, tidak ada objek dekat |
| > 50 cm | Terang | Kondisi normal |
| Tidak terbaca | Apa pun | Pengukuran jarak gagal |

## Pengujian yang telah diamati

Dari tangkapan layar Serial Monitor:

| Pengamatan | Hasil |
|---|---|
| Koneksi Wi-Fi | Terhubung |
| LDR saat pengujian | Terang dan Gelap pernah tampil |
| Ultrasonic saat objek didekatkan | 5,4 cm dan 13,9 cm |
| Gabungan jarak dekat + gelap | `PERINGATAN! Objek dekat saat gelap` |
| Pembacaan Ultrasonic | Pernah muncul `Tidak terbaca` / `Pengukuran jarak gagal` |

Pembacaan jarak yang gagal sesekali perlu ditinjau kembali pada pengujian stabilitas. Pengujian jarak lebih dari 50 cm dan kondisi terang saat objek dekat belum didokumentasikan pada screenshot yang tersedia.

## Cara menjalankan

1. Buka `esp32-monitoring-jarak-cahaya.ino` menggunakan Arduino IDE.
2. Instal board ESP32 melalui Boards Manager bila belum tersedia.
3. Pilih board **ESP32 Dev Module**, port COM yang sesuai, dan baud Serial Monitor **115200**.
4. Ubah `ssid` dan `password` pada file `.ino` sesuai jaringan Wi-Fi milikmu (gunakan Wi-Fi 2,4 GHz).
5. Periksa seluruh kabel dan pembagi tegangan ECHO, lalu klik **Verify** dan **Upload**.
6. Buka Serial Monitor dan uji perubahan jarak serta cahaya.

## Catatan keamanan

Jangan unggah password Wi-Fi asli ke repository publik. Kode dalam repository menggunakan placeholder `NAMA_WIFI` dan `PASSWORD_WIFI`; ubah hanya pada salinan lokal sebelum mengunggah program ke ESP32.

## Dokumentasi GitHub

Repository ini memuat kode program, uraian komponen, alur kerja, hasil pengujian yang telah diamati, dan foto dokumentasi untuk keperluan Responsi 1 IoT.
