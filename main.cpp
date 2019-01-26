#include <iostream>
#include <cstring>
#include <fstream>
#include <stdlib.h>
#include <algorithm>
#include <sstream>
#include<list>
using namespace std;

string int_to_str(int N){
    string dato_convertido;
    stringstream convertidor;
    convertidor<<N;
    dato_convertido= convertidor.str();
    return dato_convertido;
}

int str_to_int(string S){
    int dato_convertido;
    stringstream convertidor;
    convertidor<<S;
    convertidor>>dato_convertido;
    return dato_convertido;
}

//variable usada para recuperar el nrr cuando ya existe archivo de registros -> contribuyentes.txt
int global_nrr;


class Usuario
{
public:
    string nombre,descripcion,precio,codigo,cant_entienda,provincia,cat_producto;
    int nrr=0,puntero,i;

    void EscribirArshivo();
    void LeerArshivo();

};

class Nodo
{
public:

    Usuario u;
    Nodo *siguiente;
    Nodo *anterior;

    friend class Lista;
};

class Lista
{
public:
    Nodo *raiz;
    Nodo *temp;
    Nodo *aux;
    list<Nodo> lista_nodos;

    void llenarlista(Nodo *ptr);
    void Inicializar();
    void Mostrar(Nodo *ptr);
    void Llenar();
    void Insertar(Usuario elemento);
    void PasarA();
    void buscar();

    friend class Nodo;
};

// mis funciones
string generarllave(Usuario user){
    string llave="";
    llave += user.codigo[0];
    llave += user.codigo[1];
    //llave += user.nombre[2];
    llave += int_to_str(user.nrr);
    return llave;
}

bool operator>(Usuario a, Usuario b){
    string llave_a = generarllave(a);
    string llave_b = generarllave(b);
    string st_a,st_b;

    for(int i=0;i<2;i++){
        if(llave_a[i]==llave_b[i]){
            continue;
        }
        if(llave_a[i]>llave_b[i]){
            return true;
        }else{
            return false;
        }
    }
    for(int i=3;llave_a[i];i++){
        st_a+=llave_a[i];
    }
    for(int i=3;llave_b[i];i++){
        st_b+=llave_b[i];
    }
    //cout<<"comparacion "<<st_a<<" ** "<<st_b<<endl;
    if(str_to_int(st_a)>str_to_int(st_b)){
        return true;
    }
    return false;
}

void Lista::buscar(){
    string llave;
    cout<<"cual es la llave a buscar: ";
    cin>>llave;
    transform(llave.begin(),llave.end(),llave.begin(), ::toupper);

    aux = raiz;
    bool buscar = true;
    do{
        if(llave == generarllave(aux->u)){
            cout<<"NRR: "<<aux->u.nrr<<endl;
            cout<<"Codigo: "<<aux->u.codigo<<endl;
            cout<<"Nombre: "<<aux->u.nombre<<endl;
            cout<<"Descripcion: "<<aux->u.descripcion<<endl;
            cout<<"Precio: "<<aux->u.precio<<endl;
            cout<<"cantidad que dispone la tienda: "<<aux->u.cant_entienda<<endl;
            cout<<"Provincia: "<<aux->u.provincia<<endl;
            cout<<"Categoria del Producto: "<<aux->u.cat_producto<<endl;
            buscar = false;
        }
        if(llave > generarllave(aux->u)){
            if(aux->siguiente!=NULL){
                aux=aux->siguiente;
            }else{
                cout<<"no existe"<<endl;
                buscar= false;
            }
        }else{
            if(aux->anterior!=NULL){
                aux=aux->anterior;
            }else{
                cout<<"no existe"<<endl;
                buscar= false;
            }
        }
    }while(buscar);
}
//fin de mis funciones

void Lista :: Inicializar()
{
    raiz=NULL;
}


void Lista::llenarlista(Nodo *ptr ){
    if(ptr->anterior!=NULL){
        llenarlista(ptr->anterior);
    }
    lista_nodos.push_back(*ptr);
    if(ptr->siguiente){
        llenarlista(ptr->siguiente);
    }
}

bool menor(Nodo a, Nodo b){
    if(a.u.nrr < b.u.nrr){
        return true;
    }
    return false;
}

void Lista :: PasarA()
{
    ofstream temporal("Temporal.txt",ios::app);
    lista_nodos.clear();
    llenarlista(raiz);
    lista_nodos.sort(menor);
    list<Nodo>::iterator it=lista_nodos.begin();
    for(int i=0;i<lista_nodos.size();i++){
            temporal<<"NRR    LLAVE    H.IZQ    H.DER\n"<<(*it).u.nrr<<"\t"<<generarllave((*it).u)<<"\t";
            if((*it).anterior!=NULL){
                temporal<<(*it).anterior->u.nrr<<"\t";
            }
            else{
                temporal<<"-1"<<"\t";
            }
            if((*it).siguiente!=NULL){
                temporal<<(*it).siguiente->u.nrr<<endl;
            }
            else{
                temporal<<"-1"<<endl;
            }
            it++;
    }
    temporal.close();
}


void Lista :: Mostrar(Nodo *ptr)
{
    if(ptr==NULL ){
        cout<<"Lista Vacia"<<endl;
        return;
    }

    else{

        if(ptr->anterior!=NULL)
        {
            Mostrar(ptr->anterior);
        }

        cout<<"______________________________"<<endl;
        cout<<"NRR    LLAVE    H.IZQ    H.DER\n"<<ptr->u.nrr<<"\t"<<generarllave(ptr->u)<<"\t";

        if(ptr->anterior==NULL && ptr->siguiente==NULL)
        {
            cout<<"-1"<<"\t";
            cout<<"-1"<<"\n";
        }
        else
        {
            if(ptr->anterior!=NULL)
            {
               cout<<ptr->anterior->u.nrr<<"\t";
            }

            else
            {
                cout<<"-1"<<"\t";
            }

            if(ptr->siguiente!=NULL)
            {
                cout<<" "<<ptr->siguiente->u.nrr<<"\n";
            }

            else
            {
                cout<<"-1"<<"\n";
            }
        }

        if(ptr->siguiente!=NULL)
        {
            Mostrar(ptr->siguiente);

        }

        //cout<<"______________________________"<<endl;

    }

}



void Lista :: Llenar()
{
    Usuario usuario;

    ifstream leer("Contribuyentes.txt", ios::in);
    if(!leer.good())
    {
        cout<<"Error, el archivo no existe\n";
    }
    else
    {
        while(!leer.eof())
        {

            int i=0;
            char auxi0[30];
            do
            {
                leer.read((char*)&auxi0[i], 1);
                if(leer.eof())break;
                i++;
            }
            while(auxi0[i-1]!='|');
            if(leer.eof())break;

            auxi0[i-1]='\0';
            usuario.nrr = atoi(auxi0);//nrr = int(auxi0);
            global_nrr = usuario.nrr;

            char auxi[30];
            i=0;;
            do
            {
                leer.read((char*)&auxi[i], 1);
                i++;
            }
            while(auxi[i-1]!='|');
            auxi[i-1]='\0';
            usuario.codigo = auxi;//RFC = string(auxi);


            char auxi2[30];
            i=0;
            do
            {
                leer.read((char*)&auxi2[i], 1);
                i++;
            }
            while(auxi2[i-1]!='|');
            auxi2[i-1]='\0';
            usuario.nombre=auxi2;
            //nombre = string(auxi2);


            char auxi3[30];
            i=0;
            do
            {
                leer.read((char*)&auxi3[i], 1);
                i++;
            }
            while(auxi3[i-1]!='|');
            auxi3[i-1]='\0';
            usuario.descripcion = auxi3;

            char auxi10[30];
            i=0;
            do
            {
                leer.read((char*)&auxi10[i], 1);
                i++;
            }
            while(auxi10[i-1]!='|');
            auxi10[i-1]='\0';
            usuario.precio = auxi0;

            char auxi4[30];
            i=0;
            do
            {
                leer.read((char*)&auxi4[i], 1);
                i++;
            }
            while(auxi4[i-1]!='|');
            auxi4[i-1]='\0';
            usuario.cant_entienda = auxi4;


            char auxi5[30];
            i=0;
            do
            {
                leer.read((char*)&auxi5[i], 1);
                i++;
            }
            while(auxi5[i-1]!='|');
            auxi5[i-1]='\0';
            usuario.provincia = auxi5;

            char auxi6[30];
            i=0;
            do
            {
                leer.read((char*)&auxi6[i], 1);
                i++;
            }
            while(auxi6[i-1]!='|');
            auxi6[i-1]='\0';
            usuario.cat_producto = auxi6;

            Insertar(usuario);  // cout<<"\nNRR: "<<nrr<<"\nRFC: "<<RFC<<"\nNombre: "<<nombre<<"\nApellido paterno: "<<apellido<<"\nApellido materno: "<<apellido1<<"\nTelefono: "<<telefono<<"\nDireccion: "<<direccion<<"\nCiudad: "<<cuidad<<"\nFecha de nacimiento: "<<fecha<<"\nEstado civil: "<<Estado_civil<<"\nNumero de dependientes: "<<no_depen<<"\n\n";
        }

        leer.close();
    }


}

void Lista :: Insertar(Usuario elemento)
{
    temp=new Nodo();
    temp->siguiente=NULL;
    temp->anterior=NULL;
    temp->u=elemento;
    if(raiz==NULL)
    {
        raiz=temp;
    }
    else
    {
        aux=raiz;
        while(aux!=NULL)
        {
            if(temp->u > aux->u)
            {
                if(aux->siguiente != NULL)
                {
                    aux=aux->siguiente;
                }

                else
                {
                    aux->siguiente=temp;
                    break;
                }

            }
            else
            {

                if(aux->anterior != NULL)
                {
                    aux=aux->anterior;
                }
                else
                {
                    aux->anterior=temp;
                    break;
                }
            }

        }
    }
}



void Usuario::LeerArshivo()
{

    ifstream leer("Contribuyentes.txt", ios::in);
    if(!leer.good())
    {
        cout<<"Error, el archivo no existe\n";
    }
    else
    {
        while(!leer.eof())
        {

            int i=0;
            char auxi0[30];
            //if(leer.eof())break;
            do
            {
                leer.read((char*)&auxi0[i], 1);
                if(leer.eof())break;
                i++;
            }
            while(auxi0[i-1]!='|');
            if(leer.eof())break;

            auxi0[i-1]='\0';
            cout<<"NRR: "<<auxi0<<endl;//nrr = int(auxi0);

            char auxi[30];
            i=0;
            do
            {
                leer.read((char*)&auxi[i], 1);
                i++;
            }
            while(auxi[i-1]!='|');
            auxi[i-1]='\0';
            cout<<"Codigo: "<<auxi<<endl;//RFC = string(auxi);


            char auxi2[30];
            i=0;
            do
            {
                leer.read((char*)&auxi2[i], 1);
                i++;
            }
            while(auxi2[i-1]!='|');
            auxi2[i-1]='\0';
            cout<<"Nombre: "<<auxi2<<endl;


            char auxi3[30];
            i=0;
            do
            {
                leer.read((char*)&auxi3[i], 1);
                i++;
            }
            while(auxi3[i-1]!='|');
            auxi3[i-1]='\0';
            cout<<"Descripcion: "<<auxi3<<endl;

            ///////////////////////
            char auxi10[30];
            i=0;
            do
            {
                leer.read((char*)&auxi10[i], 1);
                i++;
            }
            while(auxi10[i-1]!='|');
            auxi10[i-1]='\0';
            cout<<"Precio: "<<auxi10<<endl;

            ////////

            char auxi4[30];
            i=0;
            do
            {
                leer.read((char*)&auxi4[i], 1);
                i++;
            }
            while(auxi4[i-1]!='|');
            auxi4[i-1]='\0';
            cout<<"Cantidad que dispone la tienda: "<<auxi4<<endl;



            char auxi5[30];
            i=0;
            do
            {
                leer.read((char*)&auxi5[i], 1);
                i++;
            }
            while(auxi5[i-1]!='|');
            auxi5[i-1]='\0';
            cout<<"Provincia: "<<auxi5<<endl;
            //if(leer.eof())break;


            char auxi6[30];
            i=0;
            do
            {
                leer.read((char*)&auxi6[i], 1);
                i++;
            }
            while(auxi6[i-1]!='|');
            auxi6[i-1]='\0';
            cout<<"Categoria del producto: "<<auxi6<<endl<<endl;

        }
        leer.close();
    }

}





void Usuario::EscribirArshivo()
{

    ofstream escribe("Contribuyentes.txt", ios::app);
    //ofstream escribeIn("Index.txt", ios::app);
    system("cls");

    cout<<"Codigo: ";
    cin>>codigo;
    cin.ignore();
    transform(codigo.begin(), codigo.end(),codigo.begin(), ::toupper);
    cout<<"\nNombre: ";
    cin>>nombre;
    cin.ignore();
    transform(nombre.begin(), nombre.end(),nombre.begin(), ::toupper);
    cout<<"\nDescripcion: ";
    //cin>>descripcion;
    getline(cin,descripcion);
    transform(descripcion.begin(), descripcion.end(),descripcion.begin(), ::toupper);
    cout<<"\nPrecio: ";
    cin>>precio;
    cin.ignore();
    transform(precio.begin(), precio.end(),precio.begin(), ::toupper);
    cout<<"\nCantidad que dispone en tienda: ";
    cin>>cant_entienda;
    cin.ignore();
    transform(cant_entienda.begin(), cant_entienda.end(),cant_entienda.begin(), ::toupper);
    cout<<"\nProvincia: ";
    getline(cin,provincia);
    transform(provincia.begin(), provincia.end(),provincia.begin(), ::toupper);
    cout<<"\nCategoria del producto: ";
    cin>>cat_producto;
    cin.ignore();
    transform(cat_producto.begin(),cat_producto.end(),cat_producto.begin(), ::toupper);
    cout<<"\nLlenado completo\n\n";

    escribe.seekp(0,escribe.end);
    puntero=escribe.tellp();
    escribe<<nrr<<'|'<<codigo<<'|'<<nombre<<'|'<<descripcion<<'|'<<precio<<'|'<<cant_entienda<<'|'<<provincia<<'|'<<cat_producto<<'|';
    //escribeIn<<nrr<<'|'<<codigo<<'|'<<puntero<<'|';
    nrr++;
    escribe.close();
    //escribeIn.close();


}

int main()
{
    Usuario auxi;
    Lista lista;

    bool salir;
    int menu;

    lista.Inicializar();
    //se debe recuperar el nrr si existe archivo
    //cbx
    ifstream l("Contribuyentes.txt", ios::in);
    if(l.is_open())
    {
        lista.Llenar();
        auxi.nrr=global_nrr;
        auxi.nrr++;
        //cout<<auxi.nrr;
    }
    //fin cbx
    do
    {
        salir = true;
        cout<<"1.- Capturar "<<endl;
        cout<<"2.- Mostrar "<<endl;
        cout<<"3.- Buscar "<<endl;
        cout<<"4.- Salir "<<endl<<endl;
        cout<<"Elige una opcion:  ";
        cin>>menu;
        system("cls");


        switch(menu)
        {

        case 1:

            auxi.EscribirArshivo();
            lista.Insertar(auxi);
            system("PAUSE");
            system("cls");
            break;

        case 2:
            do
            {

                cout<<"1.- Mostrar archivo"<<endl;
                cout<<"2.- Mostrar archivo de indices"<<endl;
                cout<<"3.- Salir\n";
                cout<<"\nElige una opcion: ";
                cin>>menu;
                cin.ignore();
                system("cls");

                switch(menu)
                {
                case 1:
                    auxi.LeerArshivo();
                    system("PAUSE");
                    system("cls");
                    break;


                case 2:

                    lista.Inicializar();
                    lista.Llenar();
                    lista.Mostrar(lista.raiz);
                    //lista.PasarA(lista.raiz);
                    lista.PasarA();
                    system("PAUSE");
                    system("cls");

                    break;

                case 3:
                    salir = false;
                    break;


                default:
                    cout<<"Error"<<endl;
                    break;
                }


            }
            while(salir);
            salir = true;

            break;
        case 3:
            //auxi.Buscar();
            lista.Llenar();
            lista.buscar();
            system("PAUSE");
            system("cls");
            break;

        case 4:
            salir = false;
            break;


        default:
            cout<<"Opcion Invalida"<<endl;
            break;
        }


    }
    while(salir);

    return 0;
}

