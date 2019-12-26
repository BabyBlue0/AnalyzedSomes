/* 2019/12/26
 * PEBから関数アドレスを取得するプログラム
*/

#include<windows.h>
#include<winternl.h>
#include<stdio.h>

/* マクロ関数 */
#define CALCVA(dllbase,rva) (DWORD)((PBYTE)dllbase+(UINT)(rva))
#define PCALCVA(dllbase,rva)    (PDWORD)CALCVA(dllbase,rva)

/* 関数ポインタ */
int (*func_create)() = NULL;
int (*func_write)() = NULL;
int (*func_close)() = NULL;


int main()
{
    PEB *ppeb;     //Process Enviroment Block
    _asm{
        xor edx, edx
        mov edx, fs:[edx+30h]
        mov ppeb, edx
    }
    printf("PEB_addr : %p\n", ppeb );    
    
    PEB_LDR_DATA *ppld = (PEB_LDR_DATA *)ppeb->Ldr;
    LIST_ENTRY *ple = ppld->InMemoryOrderModuleList.Flink;
    LDR_DATA_TABLE_ENTRY *pldte = (PBYTE)ple-8; //-8はoffset調節
    
    // ヘッダ分進める
    pldte = (PBYTE)pldte->InMemoryOrderLinks.Flink-8;
    // DLLのループ
    for( pldte; pldte->DllBase; pldte = (PBYTE)pldte->InMemoryOrderLinks.Flink-8 ){
        // dllbaseの取得
        IMAGE_DOS_HEADER *pdllbase = pldte->DllBase;

        // エクスポート関数テーブルの取得
        IMAGE_NT_HEADERS *pnthedr = CALCVA( pdllbase, pdllbase->e_lfanew );
        IMAGE_EXPORT_DIRECTORY *ied = CALCVA( pdllbase, pnthedr->OptionalHeader.DataDirectory[0].VirtualAddress );

        // DLL名取得
        PDWORD pfname = CALCVA( pdllbase, ied->AddressOfNames );
        printf("[%p] %s\n", pdllbase, CALCVA( pdllbase, ied->Name ) );

        // エクスポート関数テーブル内のすべての関数についてループ
        for( int i = 0; i < ied->NumberOfNames; i++ ){
            // AddressOfNameからのインデックスを取得
            WORD idx = *( PCALCVA( pdllbase, ((UINT)ied->AddressOfNameOrdinals+2*i) ) );
            // 関数アドレスの取得
            void (*func)() = (PVOID)( CALCVA(pdllbase, *( PCALCVA( pdllbase, ied->AddressOfFunctions )+idx )) );
            // 出力フォーマット: [関数のVA(アドレステーブルからのオフセット) 関数名]
            printf("   [%p(+%04x)] %s\n", func, idx, CALCVA( pdllbase, pfname[i] ));

            // 関数名が同じであれば変数にアドレスを格納する
            if( !strcmp("CreateFileA", CALCVA( pdllbase, pfname[i] )) ){
                func_create = func;
            }
            if( !strcmp("WriteFile", CALCVA( pdllbase, pfname[i] )) ){
                func_write = func;
            }
            if( !strcmp("CloseHandle", CALCVA( pdllbase, pfname[i] )) ){
                func_close = func;
            }
        }
    }

    // 取得したAPIを使ってファイルを生成(※動作しない)
    if( func_write != NULL && func_create != NULL && func_close != NULL ){
        LPSTR massage = "I write this massage with API FUNCTION!!\n";
        int handle = func_create( "hacked.txt", GENERIC_WRITE, 0, NULL, 
            CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        printf("handle : %d\n", handle );
        if( handle > 0 ){
            int w = WriteFile(handle , massage , strlen(massage) , 0 , NULL);
            printf("%d\n", w );
            func_close( handle );
        }
    }
    printf("func_create : %p\n", func_create );
    printf("func_write : %p\n", func_write );
    printf("func_close : %p\n", func_close );

    return 0;
}