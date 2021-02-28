#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define NO_CARRETAS 5
#define NO_CAJAS 3

typedef struct Cliente
{
    int identificador;
    struct Carreta *carreta;
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
    int longitud;
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
    if (temp!=NULL){
    do
    {
        printf("%i - ", temp->identificador);
        temp = temp->siguiente;
    } while (temp != lista.primero);
    printf("\n");
    } else {
        printf(" no hay clientes comprando\n");
    }
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
Cliente *eliminarClienteDeCompras(int numero)
{
    Cliente *temp = lista.primero;
    Cliente *aux;
    if (temp->identificador == numero)
    {
        aux = temp;
        temp = temp->siguiente;
        lista.ultimo->siguiente = temp;
        lista.primero = temp;
        return aux;
    }
    else
    {
        do
        {
            if (temp->siguiente->identificador == numero)
            {
                aux = temp->siguiente;
                temp->siguiente = temp->siguiente->siguiente;
                return aux;
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
        cola_espera.longitud++;
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
        cola_espera.longitud++;
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
    if (temp != NULL)
    {
        do
        {
            printf("%i - ", temp->identificador);
            temp = temp->siguiente;
        } while (temp != NULL);
        printf("\n");
    }
    else
    {
        printf(" no hay clientes esperando\n");
    }
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
    }
    else
    {
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
            ingresarCarreta1(s + 1, num);
        }
        else
        {
            ingresarCarreta1(s + 1, num);
        }
    }
}
void imprimirCarreta(Pila pil)
{
    Carreta *temp = pil.primera;
    if (temp != NULL)
    {
        do
        {
            printf("%i - ", temp->no_carreta);
            temp = temp->siguiente;
        } while (temp != NULL);
        printf("\n");
    }
    else
    {
        printf(" no quedan carretas aca\n");
    }
}
void *agarrarCarreta()
{
    Carreta *aux;
    Carreta *aux2;
    if (carreta_1.longitud > 0)
    {
        aux = carreta_1.primera;
        if (carreta_1.primera->siguiente == NULL)
        {
            Cliente *temp = eliminarDeColaEspera();
            temp->carreta = aux;
            carreta_1.primera = NULL;
            carreta_1.ultima = NULL;
            carreta_1.longitud--;
            ingresarACicloCompras(temp);
        }
        else
        {
            while (aux->siguiente->siguiente != NULL)
            {
                aux = aux->siguiente;
            }
            Cliente *temp = eliminarDeColaEspera();
            temp->carreta = carreta_1.ultima;
            aux->siguiente = NULL;
            carreta_1.longitud--;
            carreta_1.ultima = aux;
            ingresarACicloCompras(temp);
        }
    }
    else
    {
        aux = carreta_2.primera;
        if (carreta_2.primera->siguiente == NULL)
        {
            Cliente *temp = eliminarDeColaEspera();
            temp->carreta = aux;
            carreta_2.primera = NULL;
            carreta_2.ultima = NULL;
            carreta_2.longitud--;
            ingresarACicloCompras(temp);
        }
        else
        {
            while (aux->siguiente->siguiente != NULL)
            {
                aux = aux->siguiente;
            }
            Cliente *temp = eliminarDeColaEspera();
            temp->carreta = carreta_2.ultima;
            aux->siguiente = NULL;
            carreta_2.longitud--;
            carreta_2.ultima = aux;
            ingresarACicloCompras(temp);
        }
    }
}

//Metodos de insercion y eliminación para cola de pagos

void ingresarAColaPagos(Cliente *ing)
{
    Cliente *nuevo = ing;
    nuevo->siguiente = NULL;
    Cliente *aux;
    if (cola_pago.primero == NULL)
    {
        cola_pago.conteo = 1;
        cola_pago.conteo++;
        cola_pago.primero = nuevo;
        cola_pago.longitud++;
    }
    else
    {
        aux = cola_pago.primero;
        while (aux->siguiente != NULL)
        {
            aux = aux->siguiente;
        }
        cola_pago.conteo++;
        aux->siguiente = nuevo;
        cola_pago.longitud++;
    }
}
int main()
{
    crearPilas();
    int cuantos;
    printf("Cuantos clientes nuevos ingresan?\n");
    scanf("%i", &cuantos);
    for (int a = 0; a < cuantos; a++)
    {
        ingresarAColaEspera();
    }
    printf("Pila 1 de carretas: ");
    imprimirCarreta(carreta_1);
    printf("Pila 2 de carretas: ");
    imprimirCarreta(carreta_2);
    printf("Clientes en espera: ");
    imprimirCola(cola_espera);
    for (int o = 0; o < cola_espera.longitud; o++)
    {
        if (carreta_1.longitud > 0 || carreta_2.longitud > 0)
        {
            agarrarCarreta();
        }
    }
    printf("Clientes que tienen carreta: ");
    imprimirClientes();
    printf("\n\n\n------------\n\n\n");
    printf("Pila 1 de carretas: ");
    imprimirCarreta(carreta_1);
    printf("Pila 2 de carretas: ");
    imprimirCarreta(carreta_2);
    printf("Clientes en espera todavia: ");
    imprimirCola(cola_espera);
    printf("Clientes comprando: ");
    imprimirClientes();
    //imprimirClientes();
    int sale = saleDeCompras();
    printf("El cliente que salio de comprar es: %i\n", sale);
    ingresarAColaPagos(eliminarClienteDeCompras(sale));
    printf("Clientes comprando: ");
    imprimirClientes();
    printf("Clientes en cola de pagos todavia: ");
    imprimirCola(cola_pago);
    //system("dot -Tpng grafoPrueba.txt -o grafoPrueba.png");
    return 0;
}