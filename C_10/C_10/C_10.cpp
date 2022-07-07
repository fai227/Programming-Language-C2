#include "Setting.h"
#include "People.h"
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <vector>

#pragma comment(lib, "winmm.lib")
#define NUMTHREAD 2

#define USEMUTEX

HANDLE threadMutex;
std::vector<int> threadQueue;

//八百屋、客を生成
Yaoya yaoya;
Customer customer[2];

//スレッドの実行内容
void process(int customerNum) {
    //リンゴが無くなるまで続ける
    while (true) {
        //家から向かう
        Sleep(customer[customerNum].TripTime);

        //店員が空くのを待つ
#ifdef USEMUTEX
        WaitForSingleObject(threadMutex, INFINITE);
#endif // USEMUTEX
        
        //購入する
        Sleep(customer[customerNum].TradingTime);
        if (yaoya.NumberOfApple == 0) {
            //リンゴが売り切れなら何もしない
            break;
        }
        else if (customer[customerNum].AppleAtOneTime > yaoya.NumberOfApple) {
            //品切れにする
            customer[customerNum].NumberOfApple += yaoya.NumberOfApple;
            yaoya.NumberOfApple = 0;
        }
        else {
            //普通に買う
            customer[customerNum].NumberOfApple += customer[customerNum].AppleAtOneTime;
            yaoya.NumberOfApple -= customer[customerNum].AppleAtOneTime;
        }

        //店員を開ける
#ifdef USEMUTEX
        ReleaseMutex(threadMutex);
#endif // USEMUTEX


        //帰る
        Sleep(customer[customerNum].TripTime);

        //荷下ろしをする
        Sleep(customer[customerNum].DroppingTime);
    }
}

void thread(LPVOID data) {
    process((int)data);
}

int main()
{
    //カレントディレクトリの取得
    char currentDirectory[CHARBUFF];
    getCurrentDirectory(currentDirectory);

    //設定ファイルの読込
    char settingFile[CHARBUFF];
    sprintf_s(settingFile, "%s\\Setting.ini", currentDirectory);

    //読込用の文字列
    char section[CHARBUFF];
    char keyWord[CHARBUFF];

    //八百屋の設定を読み込む
    sprintf_s(section, "Akito");
    yaoya.NumberOfApple = readInt("Akito", "NumberOfApple", -1, settingFile);

    //客の設定を読み込む
    for (int i = 0; i < 2; i++) {
        if (i == 0) {   //Sakuraさん読込
            sprintf_s(section, "Sakura");
        }
        else {  //Tomoyoさん読込
            sprintf_s(section, "Tomoyo");
        }
        customer[i].TripTime = readInt(section, "TripTime", -1, settingFile);
        customer[i].DroppingTime = readInt(section, "DroppingTime", -1, settingFile);
        customer[i].TradingTime = readInt(section, "TradingTime", -1, settingFile);
        customer[i].AppleAtOneTime = readInt(section, "AppleAtOneTime", -1, settingFile);
        customer[i].NumberOfApple = 0;
    }

    fprintf(stdout, "売買を始めます。\n");

    //２つのスレッドを生成
    HANDLE threadHandler[2];
    DWORD threadID[2];

    threadMutex = CreateMutex(NULL, FALSE, NULL);
    for (int i = 0; i < 2; i++) {
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread, (LPVOID)i, 0, &threadID[i]);
    }

    int count = 0;
    //八百屋のリンゴがなくなるまで待つ
    while (yaoya.NumberOfApple > 0) {
        //1秒待ってリンゴの数表示させる
        Sleep(1000);
        count++;
        fprintf_s(stdout, "%d秒経過：残り%d個\n", count, yaoya.NumberOfApple);
    }

    //結果を表示する
    fprintf_s(stdout, "---------結果表示---------\n");
    fprintf_s(stdout, "Sakuraさんのリンゴ購入数：%d\n", customer[0].NumberOfApple);
    fprintf_s(stdout, "Tomoyoさんのリンゴ購入数：%d\n", customer[1].NumberOfApple);

    writeInt("Answer", "Sakura", customer[0].NumberOfApple, settingFile);
    writeInt("Answer", "Tomoyo", customer[1].NumberOfApple, settingFile);

    Sleep(5000);
    return 0;
}