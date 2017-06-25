//
// SPI シリアルフラッシュメモリ W25Q64 操作検証プログラム
// W25Q64のメモリ領域構造
//   総バイト数 8388608
//   メモリ空間 24ビットアドレス指定 0x00000 - 0x7FFFFF 
//   ブロック数 128 (64KB/ブロック)
//   セクタ数 2048  ( 4KB/セクタ)
//   総セクタ数 2048

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "spi_lib.c"
#include "W25Q64.c"

#define SPI_CHANNEL "/dev/spidev0.0"
//#define SPI_CHANNEL "/dev/spidev1.0"

//naren//
//naren// 書込みデータのダンプリスト
//naren// dt(in) : データ格納先頭アドレス
//naren// n(in)  : 表示データ数
//naren//
//narenvoid dump(uint8_t *dt, uint32_t n) {
//naren  uint32_t sz;
//naren  char buf[64];
//naren  uint16_t clm = 0;
//naren  uint8_t data;
//naren  uint8_t sum;
//naren  uint8_t vsum[16];
//naren  uint8_t total =0;
//naren  uint32_t saddr =0;
//naren  uint32_t eaddr =n-1;
//naren  sz = eaddr -saddr;
//naren  
//naren  printf("----------------------------------------------------------\n");
//naren  uint16_t i;
//naren  for (i=0;i<16;i++) vsum[i]=0;  
//naren  uint32_t addr;
//naren  for (addr = saddr; addr <= eaddr; addr++) {
//naren    data = dt[addr];
//naren    if (clm == 0) {
//naren      sum =0;
//naren      printf("%05x: ",addr);
//naren    }
//naren
//naren    sum+=data;
//naren    vsum[addr % 16]+=data;
//naren    
//naren    printf("%02x ",data);
//naren    clm++;
//naren    if (clm == 16) {
//naren      printf("|%02x \n",sum);
//naren      clm = 0;
//naren    }
//naren  }
//naren  printf("----------------------------------------------------------\n");
//naren  printf("       ");
//naren  for (i=0; i<16;i++) {
//naren    total+=vsum[i];
//naren    printf("%02x ",vsum[i]);
//naren  }
//naren  printf("|%02x \n\n",total);
//naren}

void main() {
    uint8_t buf[256];     // 取得データ
    uint8_t wdata[16];    // 書込みデータ
    uint8_t i;
    
    uint16_t n;           // 取得データ数

    // SPI channel 0 を 2MHz で開始。
    // Start SPI channel 0 with 2MHz
    spi_config_t spiConf = { 0, 8, 1000000, 0};
    int fd = spi_open(SPI_CHANNEL, spiConf);
    if (fd < 0) {
      printf("SPISetup failed:\n");
      return;
    }
    
    // フラッシュメモリ利用開始
    // Start Flush Memory
    W25Q64_begin(fd);
    
    // JEDEC IDの取得テスト
    // JEDEC ID Get
    W25Q64_readManufacturer(buf);
    printf("JEDEC ID : ");
    for (i=0; i< 3; i++) {
      printf("%x ",buf[i]);
    }
    printf("\n");
    
    // Unique IDの取得テスト
    // Unique ID Get
    W25Q64_readUniqieID(buf);
    printf("Unique ID : ");
    for (i=0; i< 7; i++) {
      printf("%x ",buf[i]);
    }
    printf("\n");
    
//naren    // データの読み込み(アドレス0から256バイト取得)
//naren    // Read 256 byte data from Address=0
//naren    memset(buf,0,256);
//naren    n =  W25Q64_read(0, buf, 256);
//naren    printf("Read Data: n=%d\n",n);
//naren    dump(buf,256);
//naren
//naren    // 高速データの読み込み(アドレス0から256バイト取得)
//naren    // First read 256 byte data from Address=0
//naren    memset(buf,0,256);
//naren    n =  W25Q64_fastread(0, buf, 256);
//naren    printf("Fast Read Data: n=%d\n",n);
//naren    dump(buf,256);
//naren
//naren    // セクタ単位の削除
//naren    // Erase data by Sector
//naren    n = W25Q64_eraseSector(0,true);
//naren    printf("Erase Sector(0): n=%d\n",n);
//naren    memset(buf,0,256);
//naren    n =  W25Q64_read (0, buf, 256);
//naren    dump(buf,256);
//naren 
//naren    // データ書き込みテスト
//naren    // Write data to Sector=0 Address=10
//naren    for (i=0; i < 26;i++) {
//naren      wdata[i]='A'+i; // A-Z     
//naren    }  
//naren    n =  W25Q64_pageWrite(0, 10, wdata, 26);
//naren    printf("page_write(0,10,d,26): n=%d\n",n);
//naren
//naren    // データの読み込み(アドレス0から256バイト取得)
//naren    // Read 256 byte data from Address=0
//naren    memset(buf,0,256);
//naren    n =  W25Q64_read(0, buf, 256);
//naren    printf("Read Data: n=%d\n",n);
//naren    dump(buf,256);
//naren
//naren    // データ書き込みテスト
//naren    // Write data to Sector=0 Address=0
//naren    for (i=0; i < 10;i++) {
//naren      wdata[i]='0'+i; // 0-9     
//naren    }  
//naren    n =  W25Q64_pageWrite(0, 0, wdata, 10);
//naren    printf("page_write(0,0,d,10): n=%d\n",n);
//naren
//naren    // 高速データの読み込み(アドレス0から256バイト取得)
//naren    // First read 256 byte data from Address=0
//naren    memset(buf,0,256);
//naren    n =  W25Q64_fastread(0,buf, 256);
//naren    printf("Fast Read Data: n=%d\n",n);
//naren    dump(buf,256);

    // ステータスレジスタ1の取得
    // Get fron Status Register1
    buf[0] = W25Q64_readStatusReg1();
    printf("Status Register-1: %x\n",buf[0]);

    // ステータスレジスタ2の取得
    // Get fron Status Register2
    buf[0] = W25Q64_readStatusReg2();
    printf("Status Register-2: %x\n",buf[0]);

    spi_close(fd);
}

