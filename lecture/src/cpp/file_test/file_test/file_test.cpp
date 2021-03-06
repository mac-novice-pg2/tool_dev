// file_test.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include "pch.h"

#include <cstdio>

void file_write( void )
{
    char hello[] = "#hello world";

    // printf()でhelloを表示する
    printf( "%s\n", hello );

    // fprintf()は出力先を指定出来る以外、printf()と同じ
    fprintf( stdout, "%s\n", hello );

    FILE *fp = fopen( "hello.txt", "wt" );//wt =write text

    // fprintf()の出力先をファイルにしてみる
    fprintf( fp, "%s\n", hello );

    fclose( fp );
} // file_write()

void file_read( void )
{
    char buf[ 64 ];

    printf( "文字列を入力してEnterキーを押して下さい\n" );
    // fgets()で標準入力(キーボード入力=stdin)を読み込む
    fgets( buf, sizeof( buf ), stdin );
    printf( "入力文字列 : %s\n", buf );

    // fgets()は出力先を指定出来る以外、printf()と同じ
    FILE *fp = fopen( "hello.txt", "rt" );//rt=read txt
    fgets( buf, sizeof( buf ), fp );
    printf( "入力文字列 : %s\n", buf );

    fclose( fp );
} // file_read()

int main()
{
    file_write();
    file_read();
}
