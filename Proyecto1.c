#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define NO_CARRETAS 10
#define NO_CAJAS 3

typedef struct Cliente
{
    int identificador;
    int carreta;
    struct Cliente *siguiente;
} Cliente;

typedef struct Caja
{
    int no_caja;
    int id_cliente;
    int tiempo_servicio;
    char estado[10];
    int carreta_cliente;
    struct Caja *siguiente;
} Caja;

typedef struct Carreta
{
    int no_carreta;
    struct Carreta *siguiente;
} Carreta;

typedef struct ListaClientes
{
    Cliente *primero;
    Cliente *ultimo;
    int conteo;
} ListaClientes;

typedef struct Cola
{
    Cliente *primero;
    int conteo;
} Cola;

typedef struct Pila
{
    Carreta *primera;
    Carreta *ultima;
    int longitud;
} Pila;

ListaClientes lista;
Cola cola_espera;
Cola cola_pago;
Pila carreta_1;
Pila carreta_2;

//Métodos de ingreso, obtención y eliminación de clientes que entran al bucle de compras
void ingresarACicloCompras(Cliente *aIngresar)
{
    Cliente *nuevo = aIngresar;
    lista.conteo++;
    nuevo->identificador = lista.conteo;
    if (lista.primero == NULL)
    {
        lista.primero = nuevo;
        lista.ultimo = nuevo;
        nuevo->siguiente = nuevo;
    }
    else
    {
        Cliente *aux;
        aux = lista.ultimo;
        nuevo->siguiente = lista.primero;
        aux->siguiente = nuevo;
        lista.ultimo = nuevo;
    }
}
void imprimirClientes()
{
    Cliente *temp = lista.primero;
    do
    {
        printf("%i - ", temp->identificador);
        temp = temp->siguiente;
    } while (temp != lista.primero);
    printf("\n");
}
int saleDeCompras()
{
    Cliente *temp = lista.primero;
    int num, id = -1;
    while (id == -1)
    {
        srand(time(NULL));
        num = rand() % (10 - 1) + 1;
        do
        {
            if (num == temp->identificador)
            {
                id = num;
                break;
            }
            else
            {
                temp = temp->siguiente;
            }
        } while (temp != lista.primero);
    }
    return id;
}
void eliminarClienteDeCompras(int numero)
{
    Cliente *temp = lista.primero;
    Cliente *aux;
    if (temp->identificador == numero)
    {
        aux = temp;
        temp = temp->siguiente;
        lista.ultimo->siguiente = temp;
        lista.primero = temp;
        free(aux);
    }
    else
    {
        do
        {
            if (temp->siguiente->identificador == numero)
            {
                aux = temp->siguiente;
                temp->siguiente = temp->siguiente->siguiente;
                free(aux);
                break;
            }
            else
            {
                temp = temp->siguiente;
            }
        } while (temp != lista.primero);
    }
}

//Métodos de ingreso, obtención y eliminación de colas
void ingresarAColaEspera()
{
    Cliente *nuevo = (Cliente *)malloc(sizeof(Cliente));
    nuevo->siguiente = NULL;
    Cliente *aux;
    if (cola_espera.primero == NULL)
    {
        cola_espera.conteo = 1;
        nuevo->identificador = 1;
        cola_espera.conteo++;
        cola_espera.primero = nuevo;
    }
    else
    {
        aux = cola_espera.primero;
        while (aux->siguiente != NULL)
        {
            aux = aux->siguiente;
        }
        nuevo->identificador = cola_espera.conteo;
        cola_espera.conteo++;
        aux->siguiente = nuevo;
    }
}
Cliente *eliminarDeColaEspera()
{
    Cliente *aux;
    if (cola_espera.primero != NULL)
    {
        if (cola_espera.primero->siguiente == NULL)
        {
            aux = cola_espera.primero;
            cola_espera.primero = NULL;
            return aux;
        }
        else
        {
            aux = cola_espera.primero;
            cola_espera.primero = cola_espera.primero->siguiente;
            return aux;
        }
    }
    else
    {
        return NULL;
    }
}
void imprimirCola(Cola imp)
{
    Cliente *temp = imp.primero;
    do
    {
        printf("%i - ", temp->identificador);
        temp = temp->siguiente;
    } while (temp != NULL);
    printf("\n");
}

//Métodos de ingreso y obtención de las carretas
void ingresarCarreta1(int noCarreta, int pila_no)
{
    Carreta *nueva = (Carreta *)malloc(sizeof(Carreta));
    Carreta *aux;
    nueva->no_carreta = noCarreta;
    nueva->siguiente = NULL;
    if (pila_no == 0)
    {
        if (carreta_1.primera == NULL)
        {
            carreta_1.primera = nueva;
            carreta_1.ultima = nueva;
            carreta_1.longitud++;
        }
        else
        {
            aux = carreta_1.primera;
            while (aux->siguiente != NULL)
            {
                aux = aux->siguiente;
            }
            aux->siguiente = nueva;
            carreta_1.ultima = nueva;
            carreta_1.longitud++;
        }
    } else {
        if (carreta_2.primera == NULL)
        {
            carreta_2.primera = nueva;
            carreta_2.ultima = nueva;
            carreta_2.longitud++;
        }
        else
        {
            aux = carreta_2.primera;
            while (aux->siguiente != NULL)
            {
                aux = aux->siguiente;
            }
            aux->siguiente = nueva;
            carreta_2.ultima = nueva;
            carreta_2.longitud++;
        }
    }
}
void crearPilas()
{
    srand(time(NULL));
    for (int s = 0; s < NO_CARRETAS; s++)
    {
        int num = rand() % 2;
        if (num == 0)
        {
            ingresarCarreta1(s + 1,num);
        }
        else
        {
            ingresarCarreta1(s + 1,num);
        }
    }
}
void imprimirCarreta(Pila pil)
{
    Carreta *temp = pil.primera;
    do
    {
        printf("%i - ", temp->no_carreta);
        temp = temp->siguiente;
    } while (temp != NULL);
    printf("\n");
}
int main()
{
    crearPilas();
    int cuantos;
    printf("Cuantos clientes nuevos ingresan?\n");
    scanf("%i",&cuantos);
    for (int a = 0; a < cuantos; a++){
        ingresarAColaEspera();
    }
    printf("Pila 1 de carretas: ");
    imprimirCarreta(carreta_1);
    printf("Pila 2 de carretas: ");
    imprimirCarreta(carreta_2);
    printf("Clientes en espera: ");
    imprimirCola(cola_espera);
    //imprimirClientes();
    //int sale = saleDeCompras();
    //printf("El cliente que salio de comprar es: %i\n", sale);
    //eliminarClienteDeCompras(sale);
    //imprimirClientes();
    //system("dot -Tpng grafoPrueba.txt -o grafoPrueba.png");
    return 0;
}