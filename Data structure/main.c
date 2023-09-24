#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define cls system("clear")

/*
    ...................
    1. Databse AVL Tree
    ...................
*/
// A. buat struct database information
struct database {
  char judul[30];
  char penyanyi[30];
  char tahun[5];
};

// B. buat strcut untuk avl tree
struct avl_music {
  struct database dataMusic;
  struct avl_music *left;
  struct avl_music *right;
  int height;
};

// declare root awal-awal
struct avl_music *root = NULL;

// struct untuk linked list
struct node { // buat data linked list
  struct database dataMusic;
  struct node *next;
  struct node *prev;
};

struct playlist { // struct kumpulan linked list
  char namaPlaylist[30];
  struct node *head;
  struct node *tail;
};

struct playlist *playlists[1000];

// di bawah ini adalah function avl yg dibutuhkan
// C. buat function struct create new music
struct avl_music *createNewMusic(char *judul, char *penyanyi, char *tahun) {
  struct avl_music *newMusic =
      (struct avl_music *)malloc(sizeof(struct avl_music));

  strcpy(newMusic->dataMusic.judul, judul);
  strcpy(newMusic->dataMusic.penyanyi, penyanyi);
  strcpy(newMusic->dataMusic.tahun, tahun);

  newMusic->left = NULL;
  newMusic->right = NULL;
  newMusic->height = 0;

  return newMusic;
}

// D. buat function insert BST dilanjut balancing AVL
// D.2. buat function max
int max(int a, int b) {
  if (a > b) {
    return a;
  } else {
    return b;
  }
}

// D.3. buat function height
int height(struct avl_music *root) {
  if (root == NULL) {
    return -1;
  } else {
    return root->height;
  }
}
// D.4. buat function rotate untuk AVL
// D.4.1. rotate right
struct avl_music *rotateRight(struct avl_music *root) {
  // awal-awal y nge-pointing ke root left
  struct avl_music *y = root->left;
  // awal-awal suby nge-pointing ke y right
  struct avl_music *suby = y->right;

  // update y pointer right nge-pointing ke root
  y->right = root;
  // update root pointer left nge-pointing ke suby
  root->left = suby;

  // update height
  root->height = 1 + max(height(root->left), height(root->right));
  y->height = 1 + max(height(y->left), height(y->right));

  return y;
}
// D.4.2 rotate left
struct avl_music *rotateLeft(struct avl_music *root) {
  // awal-awal y nge-pointing ke root right
  struct avl_music *y = root->right;
  // awal-awal suby nge-pointing ke y left
  struct avl_music *suby = y->left;

  // update y pointer left nge-pointing ke root
  y->left = root;
  // update root pointer right nge-pointing ke suby
  root->right = suby;

  // update height
  root->height = 1 + max(height(root->left), height(root->right));
  y->height = 1 + max(height(y->left), height(y->right));

  return y;
}
// D.1 buat function insert dulu sebelum max, height, rotate
struct avl_music *insert(char *judul, char *penyanyi, char *tahun,
                         struct avl_music *root) {

  // binary search tree insert
  if (root == NULL) {
    return createNewMusic(judul, penyanyi, tahun);
  } else if (strcmp(judul, root->dataMusic.judul) > 0) {
    root->right = insert(judul, penyanyi, tahun, root->right);
  } else if (strcmp(judul, root->dataMusic.judul) < 0) {
    root->left = insert(judul, penyanyi, tahun, root->left);
  }

  // mulai masuk ke avl
  // 1. update height dulu
  root->height = 1 + max(height(root->left), height(root->right));

  // 2. balancing
  // 2.1. hitung balance value
  int balance = height(root->left) - height(root->right);
  printf("[debug] balance: %d\n\n");

  // 2.2. check berat kiri dan kanan
  // 2.2.1 balance > 1 artinya berat ke kiri
  if (balance > 1) {
    // kasus a: lurus ke kiri semua
    printf("[debug] berat ke kiri\n");
    if (strcmp(judul, root->left->dataMusic.judul) < 0) {
      printf("[debug] rotate kanan aja\n");
      return rotateRight(root);
    }
    // kasus b: belok
    else if (strcmp(judul, root->left->dataMusic.judul) > 0) {
      // rotate kiri
      printf("[debug] rotate kiri, kanan\n");
      root->left = rotateLeft(root->left);
      // rotate kanan
      return rotateRight(root);
    }
  }
  // 2.2.2 balance < -1 artinya berat ke kanan
  else if (balance < -1) {
    // kasus a: lurus ke kanan semua
    printf("[debug] berat ke kanan\n");
    if (strcmp(judul, root->right->dataMusic.judul) > 0) {
      printf("[debug] rotate kiri aja\n");
      return rotateLeft(root);
    }
    // kasus b: belok
    else if (strcmp(judul, root->right->dataMusic.judul) < 0) {
      // rotate kanan
      printf("[debug] rotate kanan, kiri\n");
      root->right = rotateRight(root->right);
      // rotate kiri
      return rotateLeft(root);
    }
  }

  return root;
}

// E. buat function print in order
int avl_count_music = 0;
struct avl_data_table {
  char judul[30];
  char penyanyi[30];
  char tahun[5];
};
struct avl_data_table
    table[1000]; // untuk bantuan pilih delete music berdasarkan angka

void printInOrder(struct avl_music *root) {
  if (root != NULL) {
    printInOrder(root->left);
    printf("| %-3d | %-30s | %-30s | %-15s |\n", avl_count_music + 1,
           root->dataMusic.judul, root->dataMusic.penyanyi,
           root->dataMusic.tahun);

    strcpy(table[avl_count_music].judul, root->dataMusic.judul);
    strcpy(table[avl_count_music].penyanyi, root->dataMusic.penyanyi);
    strcpy(table[avl_count_music].tahun, root->dataMusic.tahun);

    avl_count_music++;
    printInOrder(root->right);
  }
}

/*
    ...................
       2. Admin page
    ...................
*/

void adminPage(); // prosedur untuk admin aja
// A. adminAddNewMusic
void adminAddNewMusic() {

  // minta input dari admin
  char tempJudul[30], tempPenyanyi[30], tempTahun[5];
  printf("Judul musik: ");
  scanf("%[^\n]", tempJudul);
  getchar();

  printf("Nama penyanyi: ");
  scanf("%[^\n]", tempPenyanyi);
  getchar();

  printf("Tahun rilis: ");
  scanf("%[^\n]", tempTahun);
  getchar();

  printf("\nProses insert [%s] ke AVL Tree dan file database\n", tempJudul);
  // insert musik baru ke avl
  root = insert(tempJudul, tempPenyanyi, tempTahun, root);

  // open stream file
  FILE *fp = fopen("datasong.txt", "a");
  // melakukan writing/printing nilai variable ke file "datasong.txt"
  fprintf(fp, "%s#%s#%s\n", tempJudul, tempPenyanyi, tempTahun);
  fclose(fp);

  printf("\n[%s - %s - %s]\n", tempJudul, tempPenyanyi, tempTahun);
  printf("Data berhasil disimpan!\n");
}
// B. adminDeleteMusic
// function pendukung buat predecessor pada function removeNodeTree
struct avl_music *findMax(struct avl_music *root) {
  while (root->right != NULL) {
    root = root->right;
  }
  return root;
}
// function pendukung buat balancing removeNodeTree
int getBalance(struct avl_music *root) {
  if (root == NULL) {
    return 0;
  } else {
    return height(root->left) - height(root->right);
  }
}

// removeNodeTree pakai cara precedessor (kiri sekali, kanan sampai habis)
struct avl_music *removeNodeTree(char *judul, struct avl_music *root) {

  // traverse sampai nemu node yang mau dihapus
  if (root == NULL) {
    return NULL;
  } else if (strcmp(judul, root->dataMusic.judul) > 0) {
    root->right = removeNodeTree(judul, root->right);
  } else if (strcmp(judul, root->dataMusic.judul) < 0) {
    root->left = removeNodeTree(judul, root->left);
  }
  // nanti node yg mau dihapus masuk ke else ini
  else {
    // ketika node tersebut 0 child
    if (root->left == NULL && root->right == NULL) {
      struct avl_music *temp = root;
      root = NULL;
      free(temp);
      return root;
    }
    // ketika node tersebut dikatakan punya 1 child di kiri apabila child
    // right-nya = NULL
    else if (root->right == NULL) {
      struct avl_music *temp = root;
      root = root->left;
      free(temp);
    }
    // ketika node tersebut dikatakan punya 1 child di kanan apabila child
    // left-nya = NULL
    else if (root->left == NULL) {
      struct avl_music *temp = root;
      root = root->right;
      free(temp);
    }
    // ketika node tersebut dikatakan punya 2 child
    else {
      // pakai metode predecessor (kiri sekali, kanan sampai habis)
      struct avl_music *maxValue = findMax(root->left);
      strcpy(root->dataMusic.judul, maxValue->dataMusic.judul);
      root->left = removeNodeTree(maxValue->dataMusic.judul, root->left);
    }
  }

  // mulai masuk ke avl
  // 1. update height dulu
  root->height = 1 + max(height(root->left), height(root->right));

  // 2. balancing
  // 2.1. hitung balance value
  int balance = getBalance(root);
  printf("[debug] balance: %d\n\n");

  // 2.2. check berat kiri dan kanan
  // 2.2.1 balance > 1 artinya berat ke kiri
  if (balance > 1) {
    // kasus a: lurus ke kiri semua
    printf("[debug] berat ke kiri\n");
    if (getBalance(root->left) >= 0) {
      printf("[debug] rotate kanan aja\n");
      return rotateRight(root);
    }
    // kasus b: belok
    else if (getBalance(root->left) < 0) {
      // rotate kiri
      printf("[debug] rotate kiri, kanan\n");
      root->left = rotateLeft(root->left);
      // rotate kanan
      return rotateRight(root);
    }
  }
  // 2.2.2 balance < -1 artinya berat ke kanan
  else if (balance < -1) {
    // kasus a: lurus ke kanan semua
    printf("[debug] berat ke kanan\n");
    if (getBalance(root->right) <= 0) {
      printf("[debug] rotate kiri aja\n");
      return rotateLeft(root);
    }
    // kasus b: belok
    else if (getBalance(root->right) > 0) {
      // rotate kanan
      printf("[debug] rotate kanan, kiri\n");
      root->right = rotateRight(root->right);
      // rotate kiri
      return rotateLeft(root);
    }
  }

  return root;
}

void savePlaylisttoFile();

// delete music
int count_playlist =
    0; // dipake untuk menghitung berapa banyak playlist yang udah dibuat
char copy_str_judul_musik[30]; // untuk menyimpan judul musik mana yang akan
                               // terhapus
// deleteNode memerlukan nilai dari input pilih musik nomor berapa, dan juga max
// musik yang ada pada playlist tersebut
void adminDeleteMusic() {

  avl_count_music = 0; // reset count untuk avl

  printf("Daftar seluruh musik yang ada pada database dan AVL Tree\n");
  printf("%s", "---------------------------------------------------------------"
               "----------------------------\n");
  printf("| %-3s | %-30s | %-30s | %-15s |\n", "No", "Judul Lagu", "Penyanyi",
         "Tahun Rilis");
  printf("%s", "---------------------------------------------------------------"
               "----------------------------\n");
  printInOrder(root);
  printf("%s", "---------------------------------------------------------------"
               "----------------------------\n\n");

  // minta input angka dari admin
  int input;
  do {
    printf("Pilih Musik [1-%d]: ", avl_count_music);
    scanf("%d", &input);
    getchar();
  } while (input <= 0 ||
           input >
               avl_count_music); // akan keluar looping kalau nilai yg dipilih
                                 // di antara 1 sd. maks musik yg ada

  printf("\nProses hapus [%s] ke AVL Tree dan file database\n",
         table[input - 1].judul);
  // removeNodeTree dari AVL Tree
  root = removeNodeTree(table[input - 1].judul, root);

  // open stream file
  FILE *fp = fopen("datasong.txt", "w");

  int count = 0;
  for (int i = 0; i < avl_count_music; i++) {
    if (i != input - 1) { // 5 != 4
      fprintf(fp, "%s#%s#%s\n", table[i].judul, table[i].penyanyi,
              table[i].tahun);
    }
  }
  fclose(fp);
  printf("\n- %s berhasil dihapus dari AVL Tree dan file database\n",
         table[input - 1].judul);

  // musik harus dihapus pada setiap playlist yang ada di user
  for (int i = 0; i < count_playlist;
       i++) { // akan dilakukan perintah ini kalau count_playlist minimal = 1
    if (playlists[i]->head != NULL) {
      struct node *curr = playlists[i]->head;
      struct node *curr_t = playlists[i]->tail;

      if (strcmp(curr->dataMusic.judul, table[input - 1].judul) == 0) {
        if (playlists[i]->head ==
            playlists[i]
                ->tail) { // kalo ternyata head itu sama dengan tail, artinya
                          // cuma ada 1 musik, maka head dan tail nya jadii NULL
          playlists[i]->head = playlists[i]->tail = NULL;
        } else { // kalo ternyata musiknya ada banyak, berarti cuma delete head
                 // aja
          playlists[i]->head = curr->next;
          playlists[i]->head->prev = playlists[i]->tail;
          playlists[i]->tail->next = playlists[i]->head;
        }
        strcpy(copy_str_judul_musik,
               curr->dataMusic.judul); // copy dulu judul musik yang mau dihapus
        free(curr);                    // baru hapus node judul musik tersebut
        printf("\n- %s berhasil dihapus pada playlist [%s]\n",
               table[input - 1].judul, playlists[i]->namaPlaylist);
      } else if (strcmp(curr_t->dataMusic.judul, table[input - 1].judul) == 0) {

        playlists[i]->tail = curr_t->prev;
        playlists[i]->tail->next = playlists[i]->head;
        playlists[i]->head->prev = playlists[i]->tail;
        strcpy(copy_str_judul_musik,
               curr_t->dataMusic.judul); // copy dulu judul yg mau dihapus
        free(curr_t);                    // baru hapus node judul musik tersebut
        printf("\n- %s berhasil dihapus pada playlist [%s]\n",
               table[input - 1].judul, playlists[i]->namaPlaylist);
      } else {
        while (curr != playlists[i]->tail) {
          if (strcmp(curr->dataMusic.judul, table[input - 1].judul) == 0) {
            // kalo user delete musik yang berada di tengah, maka harus traverse
            // dulu dari head sampai nomor input yang dipilih
            curr->prev->next = curr->next;
            curr->next->prev = curr->prev;
            strcpy(copy_str_judul_musik,
                   curr->dataMusic.judul); // copy dulu judul yg mau dihapus
            free(curr); // baru hapus node judul musik tersebut
            printf("\n- %s berhasil dihapus pada playlist [%s]\n",
                   table[input - 1].judul, playlists[i]->namaPlaylist);
          }
          curr = curr->next;
        }
      }
    }
  }
  avl_count_music = 0; // reset count untuk avl

  savePlaylisttoFile();
}

/*
    ...................
    3. Linked List User
    ...................
*/

void savePlaylisttoFile() {

  FILE *fps = fopen("dataplaylist.txt", "w");

  for (int i = 0; i < count_playlist; i++) {
    struct node *curr = playlists[i]->head;
    fprintf(fps, "%s\n", playlists[i]->namaPlaylist);

    if (curr != NULL) {
      while (curr != playlists[i]->tail) {
        fprintf(fps, "%s-%s-%s#.\n", curr->dataMusic.judul,
                curr->dataMusic.penyanyi, curr->dataMusic.tahun);
        curr = curr->next;
      }

      fprintf(fps, "%s-%s-%s#*\n\n", curr->dataMusic.judul,
              curr->dataMusic.penyanyi, curr->dataMusic.tahun);
    }
  }

  fclose(fps);
}

void userPage(); // prosedur untuk user aja

// alokasiin memory untuk new playlist
void createNewPlaylist(char *namaPlaylist) {
  struct playlist *newPlaylist =
      (struct playlist *)malloc(sizeof(struct playlist));

  strcpy(newPlaylist->namaPlaylist, namaPlaylist);
  newPlaylist->head = NULL;
  newPlaylist->tail = NULL;
  playlists[count_playlist] = newPlaylist;

  count_playlist++;
  printf("\nPlaylist [%s] Successfully Added!\n", namaPlaylist);

  savePlaylisttoFile();
}
// buat gerbong manual dari menu user
void userCreateNewPlaylist() {

  int exit = 0;
  char namaPlaylist[30];
  do {
    printf("Nama Playlist: ");
    scanf("%[^\n]", namaPlaylist);
    getchar();
    if (strcmp(namaPlaylist, "\n") != 0) {
      exit = 1;
    }
  } while (!exit);

  createNewPlaylist(namaPlaylist);
}
// alokasiin memory untuk node baru
struct node *createNewNode(char tempjudul[], char temppenyanyi[],
                           char temptahun[]) {
  struct node *newNode = (struct node *)malloc(sizeof(struct node));

  strcpy(newNode->dataMusic.judul, tempjudul);
  strcpy(newNode->dataMusic.penyanyi, temppenyanyi);
  strcpy(newNode->dataMusic.tahun, temptahun);
  newNode->next = NULL;
  newNode->prev = NULL;

  return newNode;
}
// untuk print avl secara inorder
void avlShowMusic() {
  printf("%s", "---------------------------------------------------------------"
               "----------------------------\n");
  printf("| %-3s | %-30s | %-30s | %-15s |\n", "No", "Judul Lagu", "Penyanyi",
         "Tahun Rilis");
  printf("%s", "---------------------------------------------------------------"
               "----------------------------\n");
  printInOrder(root);
  printf("%s", "---------------------------------------------------------------"
               "----------------------------\n\n");
}

int playlistNum = 0; // variable memilih opsi playlist, default 0
int userChoosePlaylist() {
  playlistNum = 0; // variable akan selalu di-setting awal-awal harus 0
  printf("Daftar seluruh playlist yang ada pada database dan Linked List\n");
  printf("%-40s", "----------------------------------------\n");
  printf("| %-3s | %-30s |\n", "No", "Nama Playlist");
  printf("%-40s", "----------------------------------------\n");

  // kalo playlist nya kosong, maka langsung print penutup divider
  if (count_playlist == 0) {
    printf("%-40s", "----------------------------------------\n\n");
  } else {
    // print seluruh playlist yg ada
    for (int i = 0; i < count_playlist; i++) {
      printf("| %-3d | %-30s |\n", i + 1, playlists[i]->namaPlaylist);
    }
    printf("%-40s", "----------------------------------------\n\n");

    // minta input angka dari user
    do {
      printf("Pilih Playlist [1-%d]: ", count_playlist);
      scanf("%d", &playlistNum);
      getchar();
    } while (playlistNum <= 0 ||
             playlistNum >
                 count_playlist); // akan keluar looping kalau nilai yg dipilih
                                  // di antara 1 sd. maks musik yg ada
  }
  // return -1 karena nanti dipakai untuk index array
  return playlistNum - 1;
}

// add music to playlist
int user_flag_err_delete_music =
    0; // variable untuk menandai harus print message yg mana ke layar
void userAddtoPlaylist() {

  int playlistNum = userChoosePlaylist();

  cls;
  if (playlistNum == -1) { // kalo 0-1 = -1, artinya ga ada playlist
    printf("Tidak ada playlist\n");
    printf("Silahkan membuat playlist terlebih dahulu pada [menu 1]\n\n");
  } else {
    // print musik yang ada di avl
    avl_count_music = 0; // variable awal untuk menyimpan informasi berapa
                         // banyak musik yang ada di avl
    avlShowMusic();      // avl_count_music akan diiterasi pada function ini

    // minta input angka dari admin
    int input;
    do {
      printf("Pilih Musik [1-%d]: ", avl_count_music);
      scanf("%d", &input);
      getchar();
    } while (input <= 0 ||
             input >
                 avl_count_music); // akan keluar looping kalau nilai yg dipilih
                                   // di antara 1 sd. maks musik yg ada

    // ini biar pakai "table[input-1]"" karena acuan display musiknya dari avl
    // ketika avlShowMusic, di situ lah akan traverse secara in order, dan di
    // situ juga dilakukan string copy ke array table
    struct node *newNode =
        createNewNode(table[input - 1].judul, table[input - 1].penyanyi,
                      table[input - 1].tahun);

    // kalo awal-awal playlist masih kosong, maka head dan tail di-update
    // nge-pointing ke new node
    if (playlists[playlistNum]->head == NULL) {
      playlists[playlistNum]->head = newNode;
      playlists[playlistNum]->tail = newNode;
    } else {
      // pointing node tail pointer next nya ke new node
      playlists[playlistNum]->tail->next = newNode;
      // pointing new node pointer prev nya ke node tail
      newNode->prev = playlists[playlistNum]->tail;
      // pointing new node pointer next nya ke node head
      newNode->next = playlists[playlistNum]->head;
      // pointing node head pointer prev nya ke new node
      playlists[playlistNum]->head->prev = newNode;
      // update tail di new node
      playlists[playlistNum]->tail = newNode;
    }

    printf("\n\nMusik [%s] berhasil ditambahkan ke playlist [%s]\n",
           table[input - 1].judul, playlists[playlistNum]->namaPlaylist);
  }
  // reset lagi avl_count_music jadi 0
  avl_count_music = 0;

  savePlaylisttoFile();
}
// menampilkan music pada suatu playlist tertentu untuk menu user
int userDisplayMusicPlaylist() {

  user_flag_err_delete_music = 0; // flag penanda print message tertentu jadiin
                                  // 0

  int playlistNum = userChoosePlaylist();
  cls;

  int linkedList_count_music = 0; // variable untuk menghitung seberapa banyak
                                  // musik yang ada pada 1 playlist tertentu
  // kalo playlist num 0-1 = -1, berarti ga ada playlist
  if (playlistNum == -1) {
    printf("Tidak ada playlist\n");
    printf("Silahkan membuat playlist terlebih dahulu pada [menu 1]\n\n");
    user_flag_err_delete_music = 1; // flag message aktifin jadi 1
  } else {
    printf("Daftar seluruh musik yang ada pada playlist [%s]\n",
           playlists[playlistNum]->namaPlaylist);
    printf("%s", "-------------------------------------------------------------"
                 "------------------------------\n");
    printf("| %-3s | %-30s | %-30s | %-15s |\n", "No", "Judul Lagu", "Penyanyi",
           "Tahun Rilis");
    printf("%s", "-------------------------------------------------------------"
                 "------------------------------\n");

    struct node *curr =
        playlists[playlistNum]->head; // awal-awal curr nge-pointing ke head
                                      // pada playlist yang dipilih

    if (playlists[playlistNum]->head ==
        NULL) { // kalo head nya NULL, berarti ga ada musik sama sekali
      printf("| %-3d | %-30s | %-30s | %-15s |\n", 0, "NULL", "NULL", "NULL");
      user_flag_err_delete_music = 1; // aktifin flag message jadi 1
    } else {
      // kalo ga NULL, traverse dari head sampe tail
      while (curr != playlists[playlistNum]->tail) {
        linkedList_count_music++; // variable diiterasikan untuk
                                  // merepresentasikan banyaknya musik yang ada
        printf("| %-3d | %-30s | %-30s | %-15s |\n", linkedList_count_music,
               curr->dataMusic.judul, curr->dataMusic.penyanyi,
               curr->dataMusic.tahun);
        curr = curr->next; // traverse si curr
      }
      linkedList_count_music++; // +1 karena di bawah akan dihitung dan di-print
                                // untuk node tail nya
      printf("| %-3d | %-30s | %-30s | %-15s |\n", linkedList_count_music,
             curr->dataMusic.judul, curr->dataMusic.penyanyi,
             curr->dataMusic.tahun);
    }
    // print divider penutup
    printf("%s", "-------------------------------------------------------------"
                 "------------------------------\n");
    if (user_flag_err_delete_music ==
        1) { // kalo flag-nya aktif alias = 1, maka print message berikut
      printf("Tambahkan musik ke playlist terlebih dahulu di [menu 2]\n\n");
    }
  }
  // me-return nilai dari berapa banyaknya musik yang ada pada suatu playlist
  // yang dipilih
  return linkedList_count_music;
}

void deleteNode(int input, int linkedList_count_music) {

  playlistNum =
      playlistNum -
      1; // walau nilai dari playlistNum itu udah di-return -1, tapi sayangnya
         // itu ga mengubah nilai playlistNum asli yang mana di-passing ke
         // function deleteNode. Sehingga perlu dilakukan -1

  struct node *curr =
      playlists[playlistNum]->head; // tempatin curr pertama kali di head

  if (input ==
      1) { // kalo user pilih 1, berarti kita delete node pertama alias si head
    if (playlists[playlistNum]->head ==
        playlists[playlistNum]
            ->tail) { // kalo ternyata head itu sama dengan tail, artinya cuma
                      // ada 1 musik, maka head dan tail nya jadii NULL
      playlists[playlistNum]->head = playlists[playlistNum]->tail = NULL;
    } else { // kalo ternyata musiknya ada banyak, berarti cuma delete head aja
      playlists[playlistNum]->head = curr->next;
      playlists[playlistNum]->head->prev = playlists[playlistNum]->tail;
      playlists[playlistNum]->tail->next = playlists[playlistNum]->head;
    }
    strcpy(copy_str_judul_musik,
           curr->dataMusic.judul); // copy dulu judul musik yang mau dihapus
    free(curr);                    // baru hapus node judul musik tersebut
  } else if (input == linkedList_count_music) { // kalo input itu sama dengan
                                                // maks. musik yang ada, artinya
                                                // user ingin delete tail
    struct node *curr = playlists[playlistNum]->tail; // curr langsung di tail

    playlists[playlistNum]->tail = curr->prev;
    playlists[playlistNum]->tail->next = playlists[playlistNum]->head;
    playlists[playlistNum]->head->prev = playlists[playlistNum]->tail;
    strcpy(copy_str_judul_musik,
           curr->dataMusic.judul); // copy dulu judul yg mau dihapus
    free(curr);                    // baru hapus node judul musik tersebut
  } else {
    // kalo user delete musik yang berada di tengah, maka harus traverse dulu
    // dari head sampai nomor input yang dipilih
    for (int i = 1; i < input; i++) {
      curr = curr->next;
    }
    curr->prev->next = curr->next;
    curr->next->prev = curr->prev;
    strcpy(copy_str_judul_musik,
           curr->dataMusic.judul); // copy dulu judul yg mau dihapus
    free(curr);                    // baru hapus node judul musik tersebut
  }
}
// delete musik pada playlist
void userDeleteMusicPlaylist() {

  int linkedList_count_music = userDisplayMusicPlaylist();

  if (linkedList_count_music > 0) {
    // minta input angka dari admin
    int input;
    do {
      printf("Pilih Musik [1-%d]: ", linkedList_count_music);
      scanf("%d", &input);
      getchar();
    } while (input <= 0 ||
             input > linkedList_count_music); // akan keluar looping kalau nilai
                                              // yg dipilih di antara 1 sd. maks
                                              // musik yg ada

    deleteNode(input, linkedList_count_music);

    printf("\nMusik [%s] berhasil dihapus pada playlist [%s]\n",
           copy_str_judul_musik, playlists[playlistNum]->namaPlaylist);
  } else {
    if (user_flag_err_delete_music != 1) {
      printf("Tambahkan musik ke playlist terlebih dahulu di [menu 2]\n\n");
    }
  }

  savePlaylisttoFile();
}

void userDeletePlaylist() {

  int playlistNum = userChoosePlaylist();
  cls;
  if (playlistNum == -1) {
    printf("Tidak ada playlist\n");
    printf("Silahkan membuat playlist terlebih dahulu pada [menu 1]\n\n");
  } else {
    struct node *curr = playlists[playlistNum]->head;

    // copy judul yg mau di-delete
    char copy_str_name_playlist[30];
    strcpy(copy_str_name_playlist, playlists[playlistNum]->namaPlaylist);

    if (curr != NULL) {
      playlists[playlistNum]->tail->next = NULL;
      // curr sebelum tail
      printf("Proses penghapusan playlist [%s] sedang berjalan...\n\n",
             copy_str_name_playlist);
      while (curr != NULL) {
        struct node *delete = curr;
        printf("[debug] Musik [%s] berhasil dihapus\n", curr->dataMusic.judul);
        curr = curr->next;
        free(delete);
      }
    }

    for (int i = playlistNum; i < count_playlist; i++) {
      playlists[i] = playlists[i + 1];
    }

    free(playlists[count_playlist - 1]);
    count_playlist--;
    printf("\nPlaylist [%s] berhasil dihapus\n", copy_str_name_playlist);
  }

  savePlaylisttoFile();
}

void fileAddPlaylist(char tempJudul[], char tempPenyanyi[], char tempTahun[],
                     int count_playlist) {

  struct node *newNode = createNewNode(tempJudul, tempPenyanyi, tempTahun);

  if (playlists[count_playlist]->head == NULL) {
    playlists[count_playlist]->head = newNode;
    playlists[count_playlist]->tail = newNode;
  } else {
    // pointing node tail pointer next nya ke new node
    playlists[count_playlist]->tail->next = newNode;
    // pointing new node pointer prev nya ke node tail
    newNode->prev = playlists[count_playlist]->tail;

    // pointing new node pointer next nya ke node head
    newNode->next = playlists[count_playlist]->head;
    // pointing node head pointer prev nya ke new node
    playlists[count_playlist]->head->prev = newNode;

    // update tail di new node

    playlists[count_playlist]->tail = newNode;
  }
}

/*
    ...................
    4. File Processing
    ...................
*/
// A. read file aja
void readFile() {

  // read file seluruh musik yang ada
  FILE *fp = fopen("datasong.txt", "r");

  char tempNamaPlaylist[30], tempJudul[30], tempPenyanyi[30], tempTahun[5],
      tempStatus;
  while (!feof(fp)) {
    fscanf(fp, " %[^#]#%[^#]#%s\n", tempJudul, tempPenyanyi, tempTahun);
    printf("[debug add] Judul: %s\n", tempJudul);
    root = insert(tempJudul, tempPenyanyi, tempTahun, root);
  }
  fclose(fp);

  // read file playlist yg ada
  FILE *fpr = fopen("dataplaylist.txt", "r");

  if (!feof(fpr)) {

    fscanf(fpr, " %[^\n]", tempNamaPlaylist);
    printf("[debug] nama playlist: %s", tempNamaPlaylist);
    struct playlist *newPlaylist =
        (struct playlist *)malloc(sizeof(struct playlist));
    strcpy(newPlaylist->namaPlaylist, tempNamaPlaylist);
    newPlaylist->head = NULL;
    newPlaylist->tail = NULL;
    playlists[count_playlist] = newPlaylist;

    while (!feof(fpr)) {
      fscanf(fpr, " %[^-]-%[^-]-%[^#]#%c\n", tempJudul, tempPenyanyi, tempTahun,
             &tempStatus);

      fileAddPlaylist(tempJudul, tempPenyanyi, tempTahun, count_playlist);

      // ditandai tempStatus = "*" artinya sudah sampai tail di satu playlist,
      // untuk playlist selanjutnya akan lanjut dibaca sampai bertemu"*" lagi
      if (tempStatus == '*') {
        count_playlist++;
        fscanf(fp, "\n");

        if (!feof(fpr)) {
          fscanf(fpr, " %[^\n]", tempNamaPlaylist);

          struct playlist *newPlaylist =
              (struct playlist *)malloc(sizeof(struct playlist));
          strcpy(newPlaylist->namaPlaylist, tempNamaPlaylist);
          newPlaylist->head = NULL;
          newPlaylist->tail = NULL;
          playlists[count_playlist] = newPlaylist;
        }
      }
    }
  }

  fclose(fpr);
}

/*
    ...................
    5. Others
    ...................
*/
void menu(char *tipe);  // berbagai tampilan menu
void enterToContinue(); // untuk nahan sebelum di-clear screen

void main() {

  /* Open file stream saat pertama kali menjalankan program, jika tidak ada
  file, maka akan dibuat dan jika masih error, mengeluarkan file tidak
  ditemukan.*/
  FILE *fp = fopen("datasong.txt", "a");
  if (fp == NULL) {
    printf("File tidak ditemukan!\n");
  }
  fclose(fp);

  readFile();

  printf("\n\nProses sinkronasi dengan database selesai...\n");
  enterToContinue();

  int exit = 0;
  do {
    cls;          // clear screen
    menu("awal"); // call menu awal

    int opsi;
    printf("Enter option: ");
    scanf("%d", &opsi);
    getchar();

    switch (opsi) {
    case 1: // 1. Admin page
      cls;
      adminPage();
      break;
    case 2: // 2. User dashboard
      cls;
      userPage();
      break;
    case 0: // 0. Exit
      exit = 1;
      puts("Program keluar...");
      break;
    default:
      puts("Input invalid");
      enterToContinue();
      break;
    }
  } while (!exit);
}

void adminPage() {

  int exit = 0;

  do {
    cls;           // clear screen
    menu("admin"); // call menu admin

    int opsi;
    printf("Enter option: ");
    scanf("%d", &opsi);
    getchar();

    switch (opsi) {
    case 1: // 1. Add new music
      cls;
      adminAddNewMusic();
      enterToContinue();
      break;
    case 2: // 2. Delete music
      cls;
      adminDeleteMusic();
      enterToContinue();
      break;
    case 3: // 3. Show all music [A-Z]
      cls;
      avlShowMusic();
      printf("Total musik: %d\n", avl_count_music);
      avl_count_music = 0; // reset count untuk avl
      enterToContinue();
      break;
    case 99: // 99. Back
      exit = 1;
      break;
    default:
      puts("Input invalid");
      enterToContinue();
      break;
    }
  } while (!exit);
}

void userPage() {

  int exit = 0;

  do {
    cls;          // clear screen
    menu("user"); // call menu user

    int opsi;
    printf("Enter option: ");
    scanf("%d", &opsi);
    getchar();

    switch (opsi) {
    case 1: // 1. Create new playlist
      cls;
      userCreateNewPlaylist();
      enterToContinue();
      break;
    case 2: // 2. Add music to playlist
      cls;
      userAddtoPlaylist();
      enterToContinue();
      break;
    case 3: // 3. Delete music from playlist
      cls;
      userDeleteMusicPlaylist();
      enterToContinue();
      break;
    case 4: // 4. Delete playlist
      cls;
      userDeletePlaylist();
      enterToContinue();
      break;
    case 5: // 5. Show all music in a specific playlist
      cls;
      int linkedList_count_music = userDisplayMusicPlaylist();
      printf("\nTotal musik: %d\n", linkedList_count_music);
      enterToContinue();
      break;
    case 99: // 99. Back
      exit = 1;
      break;
    default:
      puts("Input invalid");
      enterToContinue();
      break;
    }
  } while (!exit);
}

// tampilan menu aja
void menu(char *tipe) {
  if (strcmp(tipe, "awal") == 0) {
    puts("Welcome to MusicPlay");
    puts("---------------------------");
    puts("1. Admin page");
    puts("2. User page");
    puts("0. Exit");
    puts("---------------------------");
  } else if (strcmp(tipe, "admin") == 0) {
    puts("Menu Admin");
    puts("---------------------------");
    puts("1. Add new music");
    puts("2. Delete music");
    puts("3. Show all music [A-Z]");
    puts("99. Back");
    puts("---------------------------");
  } else if (strcmp(tipe, "user") == 0) {
    puts("Menu User");
    puts("---------------------------");
    puts("1. Create new playlist");
    puts("2. Add music to playlist");
    puts("3. Delete music from playlist");
    puts("4. Delete playlist");
    puts("5. Show all music in a specific playlist");
    puts("99. Back");
    puts("---------------------------");
  }
}

// getchar buat nahan sebelum di-clear screen
void enterToContinue() {
  puts("enter to continue...");
  getchar();
}