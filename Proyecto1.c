#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define NO_CARRETAS 10
#define NO_CAJAS 3

typedef struct Cliente
{
    int identificador;
    struct Cliente *siguiente;
} Cliente;

typedef struct ListaClientes
{
    Cliente *primero;
    Cliente *ultimo;
    int longitud;
    int conteo;
} ListaClientes;

ListaClientes lista;

void ingresarCliente(int numero)
{
    Cliente *nuevo = (Cliente *)malloc(sizeof(Cliente));
    lista.conteo++;
    nuevo->identificador = lista.conteo;
    if (lista.primero == NULL)
    {
        lista.primero = nuevo;
        lista.ultimo = nuevo;
        nuevo->siguiente = nuevo;
        lista.longitud++;
    }
    else
    {
        Cliente *aux;
        aux = lista.ultimo;
        nuevo->siguiente = lista.primero;
        aux->siguiente = nuevo;
        lista.ultimo = nuevo;
        lista.longitud++;
    }
}

void imprimirClientes()
{
    Cliente *temp = lista.primero;
    do
    {
        printf("%i\n", temp->identificador);
        temp = temp->siguiente;
    } while (temp != lista.primero);
}

int saleDeCompras()
{
    Cliente *temp = lista.primero;
    int num, id = -1;
    while (id == -1)
    {
        srand(time(NULL));
        num = rand() % (100 + 1);
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

int main()
{
    int cuantos;
    printf("Ingresa cuantos clientes entran al minimarket\n");
    scanf("%i", &cuantos);
    for (int i = 0; i < cuantos; i++)
    {
        ingresarCliente(i + 1);
    }
    imprimirClientes();
    
    printf("Ingresa cuantos clientes entran al minimarket\n");
    scanf("%i", &cuantos);
    for (int i = 0; i < cuantos; i++)
    {
        ingresarCliente(i + 1);
    }
    imprimirClientes();
    //printf("El cliente que salio de comprar es: %i", saleDeCompras());
    system("dot -Tpng grafoPrueba.txt -o grafoPrueba.png");
    return 0;
}