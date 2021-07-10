#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

//構造体の定義
typedef struct houwadat {
	double temptr;
	double houwa;
	struct houwadat *p;
} mktd;
//構造体のデータを追加する
mktd *addlist(double temptr,  double houwa, mktd *head);
void showlist(mktd *p); //登録されたデータリストの表示
void freelist(mktd *p); //プログラム終了時に使用したメモリを開放
void reglist(mktd *p); //自己参照構造体をファイルに書き出す
void regdatetime(void); //日付・時刻データ生成
char sdate[16], stime[16];

double sete(double t);

int main(void) {
	mktd *head; //先頭ポインタ
	int k;
	double a, tempt, e;
	k=0; tempt = 0; e = 0; a = 0;
	head = NULL;
	puts(" *** 自己参照構造体の利用：飽和水蒸気量の計算→ファイル ***\n");
	puts(" ----- 計算中... -----\n");
	for (k=0; k<121; k++){
		tempt = k-20;
		e = sete(tempt);
		a = 217*e/(tempt+273.15);
		head = addlist(tempt, a, head);
	}
	puts(" ----- 入力されたデータの表示 -----");
	showlist(head);
	puts(" ----- 入力されたデータをファイルに書き出す -----");
	reglist(head);
	puts("   ---Complete!--- ");
	puts(" ----- メモリの開放 -----");
	freelist(head);
	puts("   ---Complete!--- ");
	puts(" ----- 終了 -----");
	return EXIT_SUCCESS;
}
/*データをリストに追加*/
mktd *addlist(double tempt,  double a, mktd *head) {
	mktd *p;
	//記憶領域の確保
	if ((p = (mktd *) malloc(sizeof(mktd))) == NULL) {
		printf("メモリ確保できません\n");
		exit(EXIT_FAILURE);
	}
	p->temptr = tempt;	p->houwa = a;
	//自己参照ポインタの付け替え
	p->p=head;
	head = p;
	return head;
};
/* 記録されたデータの表示 */
void showlist(mktd *p) {
	while (p != NULL) {
		printf(" %f %f\n", p->temptr, p->houwa);
		p=p->p;
	}
	return;
};
/*使用したメモリの開放*/
void freelist(mktd *p) {
	mktd *p2;
	while(p != NULL) {
		p2 = p -> p;
		free(p);
		p= p2;
	}
	return;
}
/*自己参照構造体をファイルに書き出す*/
void reglist(mktd *p) {
	char fn[128];	FILE *fp;
	//ファイル名を決定
	puts("   --- 外部ファイルにデータを記録します ---");
	printf("   ファイル名を入力-->");		fflush(0);
NEWFILE:
	scanf( "%s", fn);
	if ((fp=(fopen(fn, "r")))==NULL){
		fclose(fp);
		fp=fopen(fn, "w");
		while (p!=NULL){
			//日付・時刻データの確定
			regdatetime();
			//ファイルへ書き出し
			fprintf(fp, "%f %f %s %s\n", p->temptr, p->houwa, sdate, stime);
			p = p->p;
		}
		fclose(fp);
	} else {
		printf(" 既にファイルが存在しています。 \n 別のファイル名を入力--> ");	fflush(0);
		fclose(fp);
		goto NEWFILE;
	}
	return;
}
/*日付・時刻データ生成*/
void regdatetime(void) {
	//日付時刻データの取り込み
	time_t current;
	struct tm *local;
	time(&current);
	local=localtime(&current);
	//データの作成
	sprintf(sdate, "%d/%d/%d", local->tm_year+1900, local->tm_mon, local->tm_mday);
	sprintf(stime, "%d:%d:%d", local->tm_hour, local->tm_min, local->tm_sec);
	return;
}
double sete(double t) {
	double e;
	e = 6.1078 * pow(10, 7.5*t/(t+237.3));
	return e;
}