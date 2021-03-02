#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
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
    Cliente *actual;
    int tiempo_servicio;
    char *estado;
    struct Caja *siguiente;
    struct Caja *anterior;
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
Caja *cajas;
char instruccion[100];
char instruccion2[100];

//Métodos de ingreso, obtención y eliminación de clientes que entran al bucle de compras
void ingresarACicloCompras(Cliente *aIngresar)
{
    Cliente *nuevo = aIngresar;
    lista.conteo++;
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
    if (temp != NULL)
    {
        FILE *archivo = fopen(instruccion, "a");
        do
        {
            if (temp->siguiente != NULL)
            {
                char str[400];
                sprintf(str, "\"Cliente %i\\nCarreta %i\"->\"Cliente %i\\nCarreta %i\";\n",
                        temp->identificador, temp->carreta->no_carreta,
                        temp->siguiente->identificador, temp->siguiente->carreta->no_carreta);
                fputs(str, archivo);
            }
            temp = temp->siguiente;
        } while (temp != lista.primero);
        fclose(archivo);
        printf("\n");
    }
    else
    {
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
        nuevo->identificador = cola_espera.longitud;
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
        nuevo->identificador = cola_espera.longitud;
        cola_espera.conteo++;
        aux->siguiente = nuevo;
        cola_espera.longitud++;
    }
}
Cliente *popColaEspera()
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
void imprimirColaEspera(Cola imp)
{
    Cliente *temp = imp.primero;
    if (temp != NULL)
    {
        FILE *archivo = fopen(instruccion, "a");
        do
        {
            if (temp->siguiente != NULL)
            {
                char str[100];
                sprintf(str, "\"Cliente %i\"->\"Cliente %i\";\n",
                        temp->identificador,
                        temp->siguiente->identificador);
                fputs(str, archivo);
            }
            temp = temp->siguiente;
        } while (temp != NULL);
        fputs("\n", archivo);
        fclose(archivo);
    }
    else
    {
        printf(" no hay clientes esperando\n");
    }
}
void imprimirColaPago(Cola imp)
{
    Cliente *temp = imp.primero;
    if (temp != NULL)
    {
        FILE *archivo = fopen(instruccion, "a");
        do
        {
            if (temp->siguiente != NULL)
            {
                char str[100];
                sprintf(str, "\"Cliente %i\\nCarreta %i\"->\"Cliente %i\\nCarreta %i\";\n",
                        temp->identificador, temp->carreta->no_carreta,
                        temp->siguiente->identificador, temp->siguiente->carreta->no_carreta);
                fputs(str, archivo);
            }
            temp = temp->siguiente;
        } while (temp != NULL);
        fputs("\n", archivo);
        fclose(archivo);
    }
    else
    {
        printf(" no hay clientes esperando\n");
    }
}

//Métodos de ingreso y obtención de las carretas
Carreta *crearNodoCarreta(int carre)
{
    Carreta *nueva = (Carreta *)malloc(sizeof(Carreta));
    nueva->no_carreta = carre;
    return nueva;
}
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
void agarrarCarreta()
{
    Carreta *aux;
    Carreta *aux2;
    if (carreta_1.longitud > 0)
    {
        aux = carreta_1.primera;
        if (carreta_1.primera->siguiente == NULL)
        {
            Cliente *temp = popColaEspera();
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
            Cliente *temp = popColaEspera();
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
            Cliente *temp = popColaEspera();
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
            Cliente *temp = popColaEspera();
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
Cliente *popColaPago()
{
    Cliente *aux;
    if (cola_pago.primero != NULL)
    {
        if (cola_pago.primero->siguiente == NULL)
        {
            aux = cola_pago.primero;
            cola_pago.primero = NULL;
            return aux;
        }
        else
        {
            aux = cola_pago.primero;
            cola_pago.primero = cola_pago.primero->siguiente;
            return aux;
        }
    }
    else
    {
        return NULL;
    }
}
//Metodos de insercion y eliminación de clientes de las cajas
void ingresarCajas() 
{
    Caja *nueva = (Caja *)malloc(sizeof(Caja));
    nueva->no_caja = tamanioListaCajas() + 1;
    nueva->estado = "libre";
    nueva->tiempo_servicio = 0;
    Caja *aux;
    if (cajas == NULL)
    {
        nueva->siguiente = NULL;
        nueva->anterior = NULL;
        cajas = nueva;
    }
    else
    {
        aux = cajas;
        while (aux->siguiente != NULL)
        {
            aux = aux->siguiente;
        }
        nueva->anterior = aux;
        nueva->siguiente = NULL;
        aux->siguiente = nueva;
    }
}
int tamanioListaCajas()
{
    Caja *temp = cajas;
    int conteo = 0;
    if (cajas != NULL)
    {
        while (temp != NULL)
        {
            conteo++;
            temp = temp->siguiente;
        }
        return conteo;
    }
    else
    {
        return 0;
    }
}
void imprimirListaCajas()
{
    Caja *temp = cajas;
    FILE *archivo = fopen(instruccion, "a");
    while (temp != NULL)
    {
        if (temp->anterior != NULL)
        {
            if (temp->siguiente != NULL)
            {
                char str[50];
                sprintf(str, "\"Caja %d\\n%s\"->\"Caja %d\\n%s\";\n",
                        temp->no_caja, temp->estado,
                        temp->siguiente->no_caja, temp->siguiente->estado);
                char str1[50];
                sprintf(str1, "\"Caja %d\\n%s\"->\"Caja %d\\n%s\";\n",
                        temp->no_caja, temp->estado,
                        temp->anterior->no_caja, temp->anterior->estado);
                fputs(str, archivo);
                fputs(str1, archivo);
            }
            else
            {
                char str[50];
                sprintf(str, "\"Caja %d\\n%s\"->\"Caja %d\\n%s\";\n",
                        temp->no_caja, temp->estado,
                        temp->anterior->no_caja, temp->anterior->estado);
                fputs(str, archivo);
            }
        }
        else
        {
            if (temp->siguiente != NULL)
            {
                char str[50];
                sprintf(str, "\"Caja %d\\n%s\"->\"Caja %d\\n%s\";\n",
                        temp->no_caja, temp->estado,
                        temp->siguiente->no_caja, temp->siguiente->estado);
                fputs(str, archivo);
            }
            else
            {
                char str[50];
                sprintf(str, "\"Caja %d\\n%s\";", temp->no_caja,temp->estado);
                fputs(str, archivo);
            }
        }
        temp = temp->siguiente;
    }
    fclose(archivo);
}
void agrupar()
{
    //Método para agrupar los items de la lista de cajas en un mismo recuadro en graphviz
    Caja *temp = cajas;
    FILE *archivo = fopen(instruccion, "a");
    fputs("\nsubgraph cluster_2 {\nnode [style=filled];\n", archivo);
    while (temp != NULL)
    {
        char str[100];
        sprintf(str, " \"Caja %d\\n%s\" ",
                temp->no_caja, temp->estado);
        fputs(str, archivo);
        temp = temp->siguiente;
    }
    fputs(";\ncolor=blue;\nlabel=\"Lista de cajas\";}\n", archivo);
    fclose(archivo);

    //Método para agrupar los items de la lista de clientes en el ciclo de compras
    Cliente *temp2 = lista.primero;
    FILE *archivo2 = fopen(instruccion, "a");
    fputs("\nsubgraph cluster_3 {\nnode [style=filled];\n", archivo2);
    if (temp2 != NULL)
    {
        do
        {
            if (temp2->siguiente != NULL)
            {
                char str[100];
                sprintf(str, " \"Cliente %i\\nCarreta %i\" ",
                        temp2->identificador, temp2->carreta->no_carreta);
                fputs(str, archivo2);
            }
            temp2 = temp2->siguiente;
        } while (temp2 != lista.primero);
        fputs(";\ncolor=red;\nlabel=\"Lista de compradores\";}\n", archivo2);
        fclose(archivo2);
    }
    else
    {
        printf(" no hay clientes comprando\n");
    }

    Cliente *temp3 = cola_pago.primero;
    if (temp3 != NULL)
    {
        FILE *archivo = fopen(instruccion, "a");
        fputs("subgraph cluster_0 {node [style=filled];", archivo);
        do
        {
            char str2[100];
            sprintf(str2, "\"Cliente %i\\nCarreta %i\" \n",
                    temp3->identificador, temp3->carreta->no_carreta);
            fputs(str2, archivo);
            temp3 = temp3->siguiente;
        } while (temp3 != NULL);
        fputs(";color=blue;label=\"Cola de pago\";}", archivo);
        fclose(archivo);
    }
    else
    {
        printf(" no hay clientes esperando\n");
    }

    Cliente *temp4 = cola_espera.primero;
    if (temp4 != NULL)
    {
        FILE *archivo = fopen(instruccion, "a");
        fputs("subgraph cluster_1 {node [style=filled];", archivo);
        do
        {
            char str2[100];
            sprintf(str2, "\"Cliente %i\" \n",
                    temp4->identificador);
            fputs(str2, archivo);
            temp4 = temp4->siguiente;
        } while (temp4 != NULL);
        fputs(";color=blue;label=\"Cola de espera\";}", archivo);
        fclose(archivo);
    }
    else
    {
        printf(" no hay clientes esperando\n");
    }
    Carreta *temp5 = carreta_1.primera;
    if (temp5 != NULL)
    {
        FILE *archivo = fopen(instruccion, "a");
        fputs("\n node0[label=\"", archivo);
        do
        {
            if (temp5->siguiente != NULL)
            {
                char str2[50];
                sprintf(str2, " %i |",
                        temp5->no_carreta);
                fputs(str2, archivo);
            }
            else
            {
                char str2[50];
                sprintf(str2, "%i",
                        temp5->no_carreta);
                fputs(str2, archivo);
            }
            temp5 = temp5->siguiente;
        } while (temp5 != NULL);
        fputs("\"];\n", archivo);
        fputs("\nsubgraph cluster_4 {node [style=filled];rankdir=TB;node0;color=blue;label=\"Pila de carretas 1\";}\n", archivo);
        fclose(archivo);
    }
    else
    {
        FILE *archivo = fopen(instruccion, "a");
        fputs("\n node0[label=\"Pila vacia\"];\n", archivo);
        fputs("\nsubgraph cluster_4 {node [style=filled];rankdir=TB;node0;color=blue;label=\"Pila de carretas 1\";}\n", archivo);
        fclose(archivo);
    }
    Carreta *temp6 = carreta_2.primera;
    if (temp6 != NULL)
    {
        FILE *archivo = fopen(instruccion, "a");
        fputs("\n node1[label=\"", archivo);
        do
        {
            if (temp6->siguiente != NULL)
            {
                char str2[50];
                sprintf(str2, " %i |",
                        temp6->no_carreta);
                fputs(str2, archivo);
            }
            else
            {
                char str2[50];
                sprintf(str2, "%i",
                        temp6->no_carreta);
                fputs(str2, archivo);
            }
            temp6 = temp6->siguiente;
        } while (temp6 != NULL);
        fputs("\"];\n", archivo);
        fputs("\nsubgraph cluster_5 {node [style=filled];rankdir=TB;node1;color=blue;label=\"Pila de carretas 2\";}\n", archivo);
        fclose(archivo);
    }
    else
    {
        FILE *archivo = fopen(instruccion, "a");
        fputs("\n node1[label=\"Pila vacia\"];\n", archivo);
        fputs("\nsubgraph cluster_5 {node [style=filled];rankdir=TB;node1;color=blue;label=\"Pila de carretas 2\";}\n", archivo);
        fclose(archivo);
    }
}

Cliente *crearNodoCliente()
{
    return (Cliente *)malloc(sizeof(Cliente));
}
void iniciarListas()
{
    FILE *archivo = fopen(instruccion, "w");
    fputs("digraph G{\n rankdir=LR; graph [fontsize=10 fontname=\"Verdana\"];\nnode [shape=record fontsize=10 fontname=\"Verdana\"];\n", archivo);
    fclose(archivo);
    int noCarretas1, noCarretas2, noClientesEspera, noClientesCompra, noClientesPago, noCajas;
    printf("Ingresa el numero de carretas de la pila 1: ");
    scanf("%d", &noCarretas1);
    printf("Ingresa el numero de carretas de la pila 2: ");
    scanf("%d", &noCarretas2);
    printf("Ingresa el numero de clientes en la cola de espera: ");
    scanf("%d", &noClientesEspera);
    printf("Ingresa el numero de clientes que se encuentran comprando: ");
    scanf("%d", &noClientesCompra);
    printf("Ingresa el numero de clientes en la cola de pagos: ");
    scanf("%d", &noClientesPago);
    printf("Ingresa el numero de cajas: ");
    scanf("%d", &noCajas);
    for (int i = 0; i < noCarretas1; i++)
    {
        ingresarCarreta1(i + 1, 0);
    }
    for (int i = noCarretas1; i < noCarretas2 + noCarretas1; i++)
    {
        ingresarCarreta1(i + 1, 1);
    }
    for (int i = 0; i < noClientesPago; i++)
    {
        Cliente *temp = crearNodoCliente();
        Carreta *carrito = crearNodoCarreta(noCarretas1 + noCarretas2 + 1 + i);
        temp->identificador = i + 1;
        temp->carreta = carrito;
        ingresarAColaPagos(temp);
    }
    for (int i = noClientesPago; i < noClientesPago + noClientesCompra; i++)
    {
        Cliente *temp = crearNodoCliente();
        Carreta *carrito = crearNodoCarreta(noCarretas1 + noCarretas2 + noClientesPago + 1 + i);
        temp->identificador = i + 1;
        temp->carreta = carrito;
        ingresarACicloCompras(temp);
    }
    cola_espera.longitud = noClientesCompra + noClientesPago + 1;
    for (int i = 0; i < noClientesEspera; i++)
    {
        ingresarAColaEspera();
    }
    for (int i = 0; i < noCajas; i++)
    {
        ingresarCajas();
    }
}
int main()
{
    int graficas = 0;
    char aIntroducir[100];
    sprintf(instruccion,"nuevo%i.txt",graficas);
    sprintf(instruccion2,"nuevo%i.png",graficas);
    iniciarListas();
    imprimirListaCajas();
    imprimirColaEspera(cola_espera);
    imprimirClientes();
    imprimirColaPago(cola_pago);
    //system("dot -Tpng graficas.txt -o graficas.png");
    agrupar();
    FILE *archivo = fopen(instruccion, "a");
    fputs("}", archivo);
    fclose(archivo);
    sprintf(aIntroducir,"dot -Tpng %s -o %s",instruccion,instruccion2);
    system(aIntroducir);
    graficas++;
    sprintf(instruccion,"nuevo%i.txt",graficas);
    sprintf(instruccion2,"nuevo%i.png",graficas);
    FILE *archivo2 = fopen(instruccion, "w");
    fputs("digraph G{\n rankdir=LR; graph [fontsize=10 fontname=\"Verdana\"];\nnode [shape=record fontsize=10 fontname=\"Verdana\"];\n", archivo2);
    fclose(archivo2);
    for (int o = 0; o < cola_espera.conteo-1; o++)
    {
        if (carreta_1.longitud > 0 || carreta_2.longitud > 0)
        {
            agarrarCarreta();
        }
    }
    imprimirListaCajas();
    imprimirColaEspera(cola_espera);
    imprimirClientes();
    imprimirColaPago(cola_pago);
    agrupar();
    FILE *archivo3 = fopen(instruccion, "a");
    fputs("}", archivo3);
    fclose(archivo3);
    sprintf(aIntroducir,"dot -Tpng %s -o %s",instruccion,instruccion2);
    system(aIntroducir);
    return 0;
}