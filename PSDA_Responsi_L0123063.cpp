#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <list>
#include <queue>
#include <set>
#include <ctime>
#include <vector>
#include <stack>

using namespace std;

// Struktur data untuk menyimpan informasi akun
struct Akun
{
    string username;
    string password;
    time_t timestamp;

    // Konstruktor untuk Akun
    Akun() {}
    Akun(string uname, string pass, time_t time, string prog) : username(uname), password(pass), timestamp(time) {}
};

// Struktur data untuk node dalam reta
struct Node
{
    string location;
    Node *parent;
    vector<Node *> children;

    Node(string loc, Node *par) : location(loc), parent(par) {}
};

// Fungsi untuk membersihkan layar terminal
void clearScreen()
{
#ifdef _WIN32
    system("cls"); // For Windows
#else
    system("clear"); // For Unix-based systems (Linux, macOS)
#endif
}

// Fungsi untuk menunggu hingga pengguna menekan tombol apapun sebelum melanjutkan
void pressAnyKeyToContinue()
{
    cout << "\n";
    cout << "Tekan tombol apapun untuk melanjutkan...";
    cin.ignore(); // Membersihkan input buffer
    cin.get();    // Menunggu pengguna menekan tombol apapun
}

// Fungsi untuk membuat akun baru
void buatAkun(map<string, Akun> &daftarAkun, list<Akun> &listAkun, set<string> &daftarUsername)
{
    clearScreen(); // membersihkan layar sebelum membuat akun

    Akun akunBaru;
    cout << "Masukkan username: ";
    cin >> akunBaru.username;

    // Memeriksa apakah username sudah digunakan
    if (daftarUsername.find(akunBaru.username) != daftarUsername.end())
    {
        cout << "Username sudah digunakan\n";
        return;
    }

    daftarUsername.insert(akunBaru.username);

    cout << "Masukkan password: ";
    cin >> akunBaru.password;
    akunBaru.timestamp = time(0); // Mengambil timestamp saat ini
    daftarAkun[akunBaru.username] = akunBaru;
    listAkun.push_back(akunBaru);

    // Simpan akun ke dalam file
    ofstream file("akun.txt", ios::app);
    if (file.is_open())
    {
        file << akunBaru.username << " " << akunBaru.password << " " << akunBaru.timestamp << endl;
        file.close();
        cout << "Akun berhasil dibuat\n";
    }
    else
    {
        cout << "Gagal membuat akun\n";
    }
}

// Fungsi untuk memeriksa keberadaan akun berdasarkan username
bool cekAkun(map<string, Akun> &daftarAkun, string username)
{
    bool ditemukan = daftarAkun.find(username) != daftarAkun.end();
    if (!ditemukan)
        cout << "Akun tidak ditemukan\n";
    return ditemukan;
}

// Fungsi untuk menghapus akun
void hapusAkun(map<string, Akun> &daftarAkun, list<Akun> &listAkun, set<string> &daftarUsername, string username)
{
    auto it = daftarUsername.find(username);
    if (it != daftarUsername.end())
    {
        daftarUsername.erase(it);
        daftarAkun.erase(username);
        for (auto itList = listAkun.begin(); itList != listAkun.end(); ++itList)
        {
            if (itList->username == username)
            {
                listAkun.erase(itList);
                break;
            }
        }

        cout << "Akun berhasil dihapus\n";

        // Menyimpan kembali akun-akun ke dalam file
        ofstream file("akun.txt");
        if (file.is_open())
        {
            for (auto &entry : daftarAkun)
            {
                file << entry.second.username << " " << entry.second.password << " " << entry.second.timestamp << endl;
            }
            file.close();
        }
        else
        {
            cout << "Gagal menyimpan data akun\n";
        }
    }
    else
    {
        cout << "Akun tidak ditemukan\n";
    }
}

// Fungsi untuk menampilkan daftar akun
void lihatAkun(list<Akun> &listAkun)
{
    clearScreen(); // Membersihkan layar sebelum memeriksa akun

    cout << "Daftar Akun:\n";
    cout << "-----------------------------------------\n";
    cout << "| No. | Username    | Password    | Waktu |\n";
    cout << "-----------------------------------------\n";
    int nomor = 1;
    for (auto &akun : listAkun)
    {
        // Konversi timestamp ke string
        char timestampStr[20];
        strftime(timestampStr, 20, "%Y-%m-%d %H:%M:%S", localtime(&akun.timestamp));
        printf("| %-4d| %-12s| %-12s| %-20s|\n", nomor++, akun.username.c_str(), akun.password.c_str(), timestampStr);
    }
    cout << "-----------------------------------------\n";
}

// Fungsi untuk menampilkan menu utama
void tampilkanMenu()
{
    cout << "Selamat datang di game\n";
    cout << "Sensei : Beyond Journey's End\n";
    cout << "Silahkan pilih menu di bawah\n";
    cout << "1. Buat Akun\n";
    cout << "2. Hapus Akun\n";
    cout << "3. Lihat Akun\n";
    cout << "4. Lihat Peta\n";
    cout << "5. Mulai Story\n";
    cout << "6. Keluar\n";
}

// Fungsi untuk menampilkan peta
void tampilkanPeta(Node *locationTree)
{
    clearScreen(); // Membersihkan layar sebelum menampilkan peta

    cout <<
        R"(                       Kota
                    /       \
                Taman       Pasar
                /          /     \
            Danau  Padang Pasir Stasiun  
            /   \               /     \
     Selokan     |        Jembatan  Pusat Permainan
                 |  
            Sekolah
            /     \
    Taman Bermain   Rumah Sakit
)" << endl;
}

// Fungsi untuk membuat travel history
void tampilkanTravelHistory(stack<string> historyStack, map<string, string> endings)
{
    if (historyStack.empty())
    {
        cout << "Travel history kosong\n";
        return;
    }

    stack<string> tempStack;
    while (!historyStack.empty())
    {
        tempStack.push(historyStack.top());
        historyStack.pop();
    }

    int i = 1;
    while (!tempStack.empty())
    {
        cout << i << ". " << tempStack.top();
        if (tempStack.size() > 1)
        {
            cout << " < ";
        }
        else
        {
            auto it = endings.find(tempStack.top());
            if (it != endings.end())
            {
                cout << " < " << it->second;
            }
        }
        tempStack.pop();
        i++;
    }
}

// Fungsi untuk menampilkan teks cerita di lokasi tertentu
string teks(string lokasi)
{
    if (lokasi == "Kota")
    {
        return "Plana\t: Sensei...\n"
               "Plana\t: Sekarang kita akan bagaimana?\n"
               "Plana\t: Semua yang ada disini telah hancur.\n"
               "\t<Kamu pun termenung dengan kondisi saat ini>\n"
               "\t<Semua yang kamu lihat seperti neraka dunia>\n"
               "\t<Semuanya hancur tidak tersisa>\n"
               "Plana\t: Sensei...?\n"
               "Kamu\t: Ayo Plana, kita menuju lokasi berikutnya.\n"
               "Kamu\t: Kita lihat apakah ada sisa-sisa kehidupan disana.\n"
               "Plana\t: Baik Sensei.\n"
               "\t<Kamu dan Plana\tmelanjutkan perjalanan ke lokasi selanjutnya...>\n";
    }
    else if (lokasi == "Taman")
    {
        return "\n\t<Setelah menghabiskan 1 jam perjalanan, Kamu dan Plana akhirnya tiba di taman.>\n"
               "\t<Kerusakan yang kamu lihat bahkan lebih parah dibandingkan di kota>\n"
               "\t<Tempat yang sering kamu kunjungi dulu, kini tak menyisakan apapun>\n"
               "Kamu\t: Aku tidak menyangka, kalau aku gagal menyelamatkan mereka.\n"
               "Kamu\t: Ternyata efek chroma lebih parah dari yang aku duga.\n"
               "Kamu\t: Kekuatan untuk mengambil alih kesadaran terlalu mengerikan.\n"
               "Plana\t: ....\n"
               "\t<Plana hanya termenung menatapmu>\n"
               "\t....\n"
               "\t<Setelah beberapa saat kamu dan Plana menlanjutkan perjalanan >\n"
               "Kamu\t: Plana.\n"
               "Kamu\t: Siap Sensei?\n"
               "Kamu\t: Apakah menurutmu masih ada yang bertahan dari ini semua?\n";
    }
    else if (lokasi == "Danau")
    {
        return "\n\t<Kamu melanjutkan perjalanan menuju Danau>.\n"
               "\t<Sisa-sisa serangan chroma tidak terlalu parah disini>.\n"
               "\t<Hanya saja, tidak ada sinyal disini>.\n"
               "Plana\t: Sensei, mengapa Kitovos dilanda bencana seperti ini?.\n"
               "Kamu\t: Entahlah, aku pun kurang tahu apa tujuan mereka.\n"
               "Kamu\t: Menurut informasi yang aku dapatkan sebelum penyerangan dimulai, mereka berasal dari dunia lain dari Kitovos.\n"
               "Plana\t: Apakah dunia lain itu seperti dunia cermin?\n"
               "Kamu\t: Mungkin konsepnya seperti itu, namun yang ada disana hanyalah pembantaian dan kekejaman\n"
               "Kamu\t: Aku rasa itu semua terjadi karena tidak adanya....\n"
               "\t<BOOM!!!>\n"
               "Kamu\t: APA ITU!?.\n"
               "\t<Terdengar suara ledakan dari 2 arah yang berbeda>\n"
               "\t<Kamu dan Plana pun bergegas menuju suara ledakan itu>\n";
    }
    else if (lokasi == "Selokan")
    {
        return "\n\t<Kamu dan Plana memilih pergi ke selokan>.\n"
               "\t<Sebuah tempat bawah tanah yang dijadikan jalan bawah tanah untuk bertransaksi gelap>.\n"
               "\t<Disana terdengar suara tembakan yang mengerikan>.\n"
               "\t<Kamu pun mencoba memberanikan diri untuk melihat apa yang terjadi>.\n"
               "Hina\t: Bertahanlah semuanya!!.\n"
               "Hina\t: Sembunyi di balik mobil.\n"
               "\t<Kamu menyadari kalau itu Hina dan Anggota Komite Disiplin lainnya>.\n"
               "Kamu\t: Hina, apakah kamu bisa mendengarkanku?.\n"
               "Hina\t: Sensei?\n"
               "Kamu\t: Iya, Kalian semua dengarkan aku, Aku memiliki ide untuk mengatasi robot-robot itu.\n"
               "\t<Setelah memberikan masukkna untuk anggota KomDis, robot-robot itu berhasil dikalahkan>\n"
               "Kamu\t: Kalian, apakah semuanya baik-baik saja?"
               "Iori\t: Iya Sensei, tetapi peluru kita sisa sedikit akibat pertempuran tadi."
               "Chinatsu\t: Omong-omong Sensei, kenapa sensei bisa sampai sini?\n"
               "\t<Kamu pun menjelaskan apa yang terjadi>\n"
               "Hina\t: Begitu ya? Sekarang apa yang akan kita lakukan?\n"
               "Kamu\t: Bagaimana kalau sebaiknya kita keluar terlebih dahulu?\n"
               "Hina\t: Apakah keadaan di luar tidak berbahaya?\n"
               "Kamu\t: Entahlah, oleh karena itu, sebaiknya kita cek kondisi di luar terlebih dahulu.\n"

               "\n\tSelamat Kamu memperoleh Ending 1 : BAD ENDING\n"
               "\t<Kamu pun berjalan menyusuri selokan dan akhirnya kamu menemukan setitik cahaya di ujung lorong yang gelap ini>\n"
               "\t<Namun saat hendak menuju titik keluar kamu mendengarkan suara robot yang jumlahnya sangat banyak berdatangan dari ujung lorong dan dari belakangmu>\n"
               "\t<Kamu tidak memiliki pilihan selain melawan mereka>\n"
               "\t<Namun, para murid telah diambang batasnya dan tidak terduga kamu pun terkena serangan peluru yang datang dari 2 arah tepat mengenai perut dan kakimu>\n"
               "\t<Halo para murid yang disampingmu pun mulai melemah dan tidak dapat menahan serangan peluru yang ditembakkan musuh>\n"
               "\t<Kamu pun menyesali perbuatanmu karena mengambil keputusan tanpa mempertimbangkan yang akan terjadi. Mungkin aku adalah Sensei yang buruk>\n\n";
    }
    else if (lokasi == "Sekolah")
    {
        return "\n\t<Kamu dan Plana memilih untuk pergi ke selokan>.\n"
               "\t<Disana, kalian melihat murid Trinity sedang melawan pasukan chroma>.\n"
               "Plana\t: Sensei, Mereka murid Trinity.\n"
               "Plana\t: Iya, mereka sedang melawan Chesed yang berada dalam kendali chroma.\n"
               "Plana\t: Plana, tolong hubungkan tabletku dengan mereka\n"
               "Plana\t: Baik, Sensei.\n"
               "Plana\t: ....\n"
               "Plana\t: Sensei, tablet berhasi disambungkan dengan Mika dan lainnya\n"
               "Kamu\t: Mika, kalian semua, apakah suaraku terdengar?\n"
               "Mika\t: Ini? Sensei?\n"
               "Kamu\t: Iya, Semuanya tolong ikuti instruksiku, Chesed tidak akan menyerang jika kalian menghabisi anak buahnya terlebih dahulu.\n"
               "\t<Kamu memberi instruksi kepada mereka dan tidak lama kemudian chesed berhasil dikalahkan>\n"
               "Kamu\t: Semuanya! Apakah kalian baik-baik saja?\n"
               "Mika\t: Kami baik-baik saja Sensei\n"
               "Mika\t: Syukurlah, apakah ada yang masih di dalam sekolah?\n"
               "Mika\t: Tidak ada Sensei, mereka semua telah meniggalkan sekolah ini sejak kemunculan chroma\n"
               "Mika\t: Kalau begitu ayo kita kabur dari sini, Pasukan Chroma telah mengepung distrik 4\n"
               "\t<Kalian pun melanjutkan perjalanan dengan murid Trinity>\n";
    }
    else if (lokasi == "TamanBermain")
    {
        return "\n\t<Kalian melanjutkan perjalanan menuju Taman Bermain>.\n"
               "\t<Tempat dimana festival akan dirayakan setiap 1 tahun sekali>\n"
               "\t<Kini berubah menjadi saksi bisu dari serangan chroma>\n"
               "Plana\t: Tak kusangka tempat ini terkena dampak chroma.\n"
               "Mika\t: Sepertinya chroma menyebar di seluruh penjuru Kitovos.\n"
               "Kamu\t: Ternyata efek dari chroma separah ini.\n"
               "Kamu\t: Kalian, tunggu sebentar, aku akan memeriksa tablet untu-...\n"
               "\t<Terdengar suara tembakan dari arah belakang>\n"
               "\t<Rasa sakit yang menusuk jantungmu itu membuat pandanganmu semakin jelas>\n"
               "\t<Kamu melihat Mika dan teman-temannya mencoba mengalahkan Kaiten yang daritadi mengikutimu>\n"
               "\t<Kini pandanganmu mulai pudar>\n"
               "Kamu\t: Apakah aku telah menjalankan tugasku sebagai seorang Sensei?\n\n"

               "\n\tSelamat Kamu memperoleh Ending 2 : SAD ENDING\n\n"
               "\t<Ditengah hilangnya kesadaranmu, Plana memanggilmu>\n"
               "\t<Namun, hanya bisu yang terdengar>\n"
               "\t<Tangannya bergetar memegangmu, air mata membanjiri matanya, mengungkapkan kepedihan yang telalu dalam untuk di ucapkan>\n"
               "\t<Kata perpisahanpun tak dapat terucap>\n"
               "\t<Hanya senyuman yang dapat kau berikan untuk terakhir kalinya>\n";
    }
    else if (lokasi == "RumahSakit")
    {
        return "\n\t<Kalian melanjutkan perjalanan menuju Rumah Sakit>.\n"
               "\t<Lokasi ini jauh dengan perkotaan dan hanya memiliki sedikit penduduk>\n"
               "\t<Diperjalanan, kalian bertemu Sena>.\n"
               "Kamu\t: Sepertinya itu Sena.\n"
               "Plana\t: Hmm, Sepertinya dia sedang mengurus beberapa murid yang terluka.\n"
               "Kamu\t: Baiklah, mari kita pergi untuk mengecek kondisi di sana.\n"
               "\t<Kalian semua pergi menuju rumah sakit untuk mengecek kondisi disana>\n"
               "\t....\n"
               "\t<Setibanya dirumah sakit kalian bertemu Sena>\n"
               "Kamu\t: Sena!.\n"
               "Sena\t: Sensei? Kalian?.\n"
               "Kamu\t: Bagaimana keadaan di sekitar lokasi ini?.\n"
               "Sena\t: Serangan chroma belum mengenai titik ini, namun efek dari chroma akan segera meluas.\n"
               "Sena\t: Kita tidak tahu sampai kapan lokasi ini akan aman dari serangan chroma.\n\n"

               "\n\tSelamat Kamu memperoleh Ending 3 : MISSING ENDING\n\n"
               "\t<Diambang kebingungan antara melanjutkan perjalanan atau menetap di tempat ini>\n"
               "\t<Terlalu banyak teka-teki untuk mengatasi ini semua>\n"
               "\t<Sudah menjadi tugas orang dewasa untuk bertanggung jawab atas semua ini>\n"
               "\t<Tetapi, apakah ini jalan yang akan ku tempuh dengan cara lari dari semua masalah ini?>\n";
    }
    else if (lokasi == "Pasar")
    {
        return "\nPlana\t: Sensei, keadaan di sini ternyata lebih parah daripada yang ada di kota.\n"
               "\nKamu\t: Kamu benar Plana, kerusakan sebesar ini pasti di sebabkan oleh boss Raid.\n"
               "\nPlana\t: Sepertinya ini ulah dari Binah.\n"
               "\nPlana\t: Sensei, lihat, bukankah itu Shiroko?.\n"
               "\nKamu\t: Sepertinya kamu benar, tetapi apa yang dilakukan Shiroko di depan lubang besar itu?.\n"
               "\nPlana\t: Sebaiknya kita lihat terlebih dahulu.\n"
               "\t<Tidak lama kemudian Shiroko berlari ke arahmu>\n"
               "Shiroko\t: Huh? Sensei?\n"
               "Shiroko\t: SENSEI LARILAH!\n"
               "Kamu\t: Huh?\n"
               "\t<Terdengar suara getaran dari bawah tanah>\n"
               "\t<Ternyata dugaanmu benar, Raid Binah yang telah terkontaminasi oleh chroma menuju ke arahmu>\n"
               "Kamu\t: Ayo Plana, kita pergi dari sini\n"
               "Kamu\t: Shiroko kau juga, cepatlah\n"
               "Shiroko\t: Baik!\n";
    }
    else if (lokasi == "PadangPasir")
    {
        return "\n\t<Kalian berlari menuju Padang Pasir>\n"
               "\t<Tiba-tiba kalian memperoleh sinyal pesan dari arah Padang Pasir>\n"
               "Nonomi\t: Sensei apakah kau bisa mendengarku?\n"
               "Kamu\t: Nonomi? apakah itu kamu?\n"
               "Nonomi\t: Hmm, kami mempunyai rencana Sensei.\n"
               "Nonomi\t: Kami telah menyiapkan perangkap untuk melawan Binah.\n"
               "Kamu\t: Kami telah menyiapkan perangkap untuk melawan Binah.\n"
               "Nonomi\t: Di beberapa bagian bank central telah kami pasang Bom untuk menghentikan pergerakan Binah.\n"
               "Kamu\t: Baiklah kami akan menuju ke sana.\n"
               "\t<Kalian bertiga berlari ke arah bank\n"
               "\t<Nonomi dan murid Abydos lain telah bersiaga di sekitar Bank Central>\n"
               "\t<Dengan perhitungan yang matang, Kalian berhasil mengalahkan Binah>\n"
               "Serika\t: Sensei, Shiroko, kalian baik-baik saja?\n"
               "Shiroko\t: Kita baik-baik saja\n"
               "Kamu\t: Apakah dampak chroma di daerah ini sudah meluas?\n"
               "Nonomi\t: Beberapa sudah ada yang terkena, namun belum separah itu.\n"
               "Nonomi\t: Syukurlah kalau begitu.\n\n"

               "\n\tSelamat Kamu memperoleh Ending 4 : GOOD ENDING\n\n"
               "\t<Setelah mengetahui dampak yang di akibatkan dari chroma belum terlalu luas>\n"
               "\t<Kamu dan murid-murid kivotos mulai melakukan perjalanan ke arah Barat>\n"
               "\t<Tempat itu merupakan markas utama dari kesatuan Kitovos yang menjadi pusat suatu negara>\n"
               "\t<Mungkin dengan ini, masalah dapat di selesaikan>\n";
    }
    else if (lokasi == "Stasiun")
    {
        return "\n\t<Raid Binah masih mengikutimu hingga Stasiun>\n"
               "\t<Disana kamu menemukan murid-murid yang lain bersama dengan anggota Schale>\n"
               "\t<Berkat bantuan mereka, Binah berhasil di kalahkan dengan cepat>\n"
               "\t<Tiba-tiba muncul gate yang besar di Stasiun>\n"
               "\t<Dan orang yang keluar dari Gate tersebut adalah Shiroko Alter>\n"
               "\t<Shiroko alter mencoba untuk memanggilmu dan para murid lain>\n"
               "\t<Tetapi, kalian berlari menjauh dari Shiroko alter>\n";
    }
    else if (lokasi == "Jembatan")
    {
        return "\n\t<Sesampainya di Jembatan kalian menuju pesawat yang berada di markas seberang jembatan>\n"
               "Shiroko\t: Sensei, itu adalah...\n"
               "Shiroko\t: Iya, itu dirimu dari dunia lain\n"
               "Yuuka\t: Haruskah kita melawannya\n"
               "Kamu\t: Tidak perlu, kita akan menggunakan rencana terakhir\n"
               "Kamu\t: Kita akan melakukan teleportasi dan pergi dari Kitovos\n"
               "\t<Shiroko alter mengeluarkan Gate dan berada di hadapan kalian>\n"
               "\t<Pertempuran yang sengit pun terjadi>\n"
               "\t<....>\n"
               "\t<Setelah memakan banyak waktu dan tenaga kalian akhirnya bisa mengalahkan Shiroko Alter>\n"
               "Kamu\t: Jadi, apa rencanamu berikutnya? dan apa tujuanmu datang ke dunia ini?\n"
               "\t<Shiroko Alter menjelaskan semunya dimana dunia yang ditempatinya telah kehilangan sosok Sensei>\n"
               "\t<Hal itu menyebabkan peperangan tanpa akhir terjadi disana>\n"
               "\t<Shiroko ingin menculik sensei untuk dapat memimpin keadaan disana>\n"
               "\t<Kamu pun memahami maksud dan perasaan Shiroko>\n"
               "\t<Namun, itu bukanlah pilihan yang tepat>\n\n"

               "\n\tSelamat Kamu memperoleh Ending 5 : TRUE ENDING\n\n"
               "\t<Setelah berdiskusi dengan Shiroko, dia pun setuju dengan rencanamu>\n"
               "\t<Kalian semua menaiki pesawat dan mencari Kitovos di dunia yang lain>\n"
               "\t<Terkadang, tidak semua masalah dapat diselesaikan dengan kekerasan>\n"
               "\t<Mungkin, Memahami sesama lebih penting daripada beradu senjata>\n";
    }
    else if (lokasi == "PusatPermainan")
    {
        return "\n\t<Sesampainya di Pusat Permainan, kalian menuju maskas Schale melewati gang>\n"
               "Shiroko\t: Sensei, itu tadi adalah...\n"
               "Kamu\t: Iya, itu dirimu dari dunia lain\n"
               "Yuuka\t: Bukankah sebaiknya kita melawannya?\n"
               "Kamu\t: Tidak Yuuka, kita akan menghubungi setiap sekolah yang ada di Kitovos\n"
               "Kamu\t: Kita perlu memeriksa keadaan murid lain\n"
               "\t<Shiroko alter mengeluarkan Gate dan berada di hadapan kalian>\n"
               "\t<Setelah itu terjadilah baku tembak>\n"
               "\t<Pertempuran yang sengit pun terjadi>\n"
               "\t<....>\n"
               "\t<Sangat di sayangkan, Semua murid yang bertarung dikalahkan Shiroko Alter>\n"
               "\t<Shiroko Alter menjelaskan semunya dimana dunia yang ditempatinya telah kehilangan sosok Sensei>\n"
               "\t<Hal itu menyebabkan peperangan tanpa akhir terjadi disana>\n"
               "\t<Shiroko ingin menculik sensei untuk dapat memimpin keadaan disana>\n"
               "Shiroko Alter\t: Jadi Sensei, aku memberimu 2 pilihan\n"
               "Shiroko Alter\t: Pergi bersamaku ke dunia lain atau serangan chroma ini tidak akan berakhir.\n"
               "\t<Pilihan ini begitu sulit, namun sebagai orang dewasa, kau memiliki rasa tanggung jawab yang besar>\n\n"

               "\n\tSelamat Kamu memperoleh Ending 6 : THE UNSEEN PATH\n\n"
               "\t<Kamu pun menyetujui rencana Shiroko>\n"
               "\t<Meskipun bukanlah pilihan terbaik, tetapi hanya inilah pilihan yang bisa ku lakukan>\n"
               "\t<Semenjak hilangnya sosok Sensei dari Kitovos, Chroma juga telah menghilang>\n"
               "\t<Namun, tidak ada yang tahu apa yang terjadi di dunia paralel Kitovos>\n";
    }
    else
    {
        return "Teks belum didefinisikan untuk lokasi ini.\n";
    }
}

// Fungsi untuk memulai cerita
void mulaiStory(map<string, Akun> &daftarAkun, Node *locationTree)
{
    clearScreen(); // Membersihkan layar sebelum memulai cerita

    string username;
    cout << "Masukkan username: ";
    cin >> username;

    if (!cekAkun(daftarAkun, username))
    {
        cout << "Akun tidak ditemukan\n";
        return;
    }

    queue<Node *> travelHistory;
    stack<string> tempTravelHistory;

    Node *currentLocation = locationTree;
    bool selesai = false;

    while (!selesai)
    {
        clearScreen(); // Membersihkan layar sebelum menampilkan lokasi saat ini
        tampilkanPeta(locationTree);

        cout << "Current Location: " << currentLocation->location << endl;
        cout << "Travel History: ";
        tampilkanTravelHistory(tempTravelHistory, {{"Selokan", "Ending 1: Kota < Taman < Danau < Selokan"}});
        cout << endl;

        cout << "Anda berada di " << currentLocation->location << ".\n";
        cout << teks(currentLocation->location);

        cout << "Pilihan Lokasi:\n";
        cout << "0. Menu\n";
        for (int i = 0; i < currentLocation->children.size(); ++i)
        {
            cout << i + 1 << ". " << currentLocation->children[i]->location << endl;
        }

        int pilihan;
        cout << "Pilihan Anda: ";
        cin >> pilihan;

        if (pilihan == 0)
        {
            break;
        }
        else if (pilihan >= 1 && pilihan <= currentLocation->children.size())
        {
            tempTravelHistory.push(currentLocation->location);
            travelHistory.push(currentLocation);

            currentLocation = currentLocation->children[pilihan - 1];
        }
        else
        {
            cout << "Pilihan tidak valid\n";
        }
    }
}

int main()
{
    map<string, Akun> daftarAkun;
    list<Akun> listAkun;
    set<string> daftarUsername;

    // Membaca akun dari file akun.txt
    ifstream file("akun.txt");
    if (file.is_open())
    {
        string username, password;
        time_t timestamp;
        while (file >> username >> password >> timestamp)
        {
            daftarUsername.insert(username);
            Akun akun(username, password, timestamp, "");
            daftarAkun[username] = akun;
            listAkun.push_back(akun);
        }
        file.close();
    }

    // Node awal adalah "Kota"
    Node *locationTree;
    locationTree = new Node("Kota", nullptr);

    Node *taman = new Node("Taman", locationTree);
    Node *danau = new Node("Danau", taman);
    Node *selokan = new Node("Selokan", danau); // Memperbaiki letak node "Selokan"
    Node *sekolah = new Node("Sekolah", danau);
    Node *tamanBermain = new Node("TamanBermain", sekolah);
    Node *rumahSakit = new Node("RumahSakit", sekolah);

    taman->children.push_back(danau);

    danau->children.push_back(sekolah);
    danau->children.push_back(selokan);

    sekolah->children.push_back(tamanBermain);
    sekolah->children.push_back(rumahSakit);

    Node *pasar = new Node("Pasar", locationTree);
    Node *padangPasir = new Node("PadangPasir", pasar);
    Node *stasiun = new Node("Stasiun", pasar);
    Node *jembatan = new Node("Jembatan", stasiun);
    Node *pusatPermainan = new Node("PusatPermainan", stasiun);

    pasar->children.push_back(padangPasir);
    pasar->children.push_back(stasiun);

    stasiun->children.push_back(jembatan);
    stasiun->children.push_back(pusatPermainan);

    locationTree->children.push_back(taman);
    locationTree->children.push_back(pasar);

    int pilihan;
    while (true)
    {
        clearScreen(); // Membersihkan layar sebelum menampilkan menu

        tampilkanMenu();

        cout << "\nPilihan: ";
        cin >> pilihan;

        switch (pilihan)
        {
        case 1:
            buatAkun(daftarAkun, listAkun, daftarUsername);
            pressAnyKeyToContinue();
            break;
        case 2:
            clearScreen();
            cout << "Masukkan username yang akan dihapus: ";
            {
                string username;
                cin >> username;
                hapusAkun(daftarAkun, listAkun, daftarUsername, username);
            }
            pressAnyKeyToContinue();
            break;
        case 3:
            lihatAkun(listAkun);
            pressAnyKeyToContinue();
            break;
        case 4:
            tampilkanPeta(locationTree);
            pressAnyKeyToContinue();
            break;
        case 5:
            mulaiStory(daftarAkun, locationTree);
            break;
        case 6:
            clearScreen();
            cout << "Terima kasih telah bermain!\n";
            return 0;
        default:
            cout << "Pilihan tidak valid\n";
            break;
        }
    }

    return 0;
}
