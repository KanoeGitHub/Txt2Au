/* テキストファイルをAUファイルに変換するプログラム
 * PCMデータがテキスト形式で書かれたものをAUファイルに変換して再生できる様にする
 * 
 *	エンコードの種別、チャンネル数などの設定は
 *	https://ja.wikipedia.org/wiki/Sunオーディオファイル
 *	が参考になります。
 *
 * コンパイル：gcc -o Text2Au Text2Au.c
 * 実行： Text2Au <FILENAME.txt> <outputFileName.au>
 * FILENAME.txt はPCMデータが書かれたテキストファイルの名前
 * outputFileName.au 出力されるAUファイルの名前
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 
typedef struct au_filehdr {
	uint32_t au_magic;       /* マジックナンバー(.snd) */
	uint32_t au_offset;      /* オーディオデータが記録されているバイトオフセット */
	uint32_t au_data_size;   /* データ長（バイト） */
	uint32_t au_encoding;    /* エンコードの種別 */
	uint32_t au_sample_rate; /* 一秒あたりのサンプル数 */
	uint32_t au_channels;    /* チャンネル数 */
} AuHd;
 
int main(int argc, char *argv[]) {
	
	if(argc < 3 || argc > 4){
		printf("Usage$ %s <FILENAME.txt> <outputFileName.au> \n", argv[0]);
		return 1;
	}
	
	FILE *TextFile;
	FILE *AUFile;
	//AuHd ah = {0x2e736e64,24,0xffffffff,"エンコードの種類","サンプリングレート","チャンネル数"};
	AuHd ah = {0x2e736e64,24,0xffffffff,3,11025,1};
	//データ長は不明(0xffffffff)でOK
 
	if ((AUFile = fopen(argv[2], "wb")) == NULL) {
		printf("ファイルを書き込みできません");
		exit(1);
	}//作成するSUNオーディオファイル

	if((TextFile=fopen(argv[1],"rb"))==NULL){
		return(-1);			
	}//PCMデータのテキストファイル読み取り
		
		
	for(int i=0;i<44100;i++) //サンプリングレート数に対する書き出すサンプルの量を入力
		{
			unsigned short a;
			double d;
			if(fscanf(TextFile, "%lf", &d )==EOF){
				break;
				}
			a = d;
			fwrite(&a,sizeof(signed short),1,AUFile);
		}
		
	fseek(AUFile, 0, SEEK_SET);
	fwrite(&ah, sizeof(ah), 1, AUFile);
	
	fclose(AUFile);
	fclose(TextFile);
	printf("出力完了\n");
    return 0;
 }
