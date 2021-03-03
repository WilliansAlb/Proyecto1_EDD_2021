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
int graficas = 0;

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
    srand(time(NULL));
    if (temp == NULL)
    {
        printf("\nNo hay personas comprando");
    }
    else
    {
        num = rand() % (100 - 1) + 1;
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
                if (temp->siguiente != lista.ultimo)
                {
                    aux = temp->siguiente;
                    temp->siguiente = temp->siguiente->siguiente;
                    return aux;
                    break;
                }
                else
                {
                    aux = temp->siguiente;
                    temp->siguiente = lista.primero;
                    lista.ultimo = temp;
                    return aux;
                    break;
                }
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
        printf("\nEl cliente %i se sumo a la cola de espera", nuevo->identificador);
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
        printf("\nEl cliente %i se sumo a la cola de espera", nuevo->identificador);
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
void ingresarCarreta2(Carreta *nueva, int pila_no)
{
    Carreta *aux;
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
            printf("\nEl cliente %i agarro la carreta %i y entro a comprar", temp->identificador, temp->carreta->no_carreta);
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
            printf("\nEl cliente %i agarro la carreta %i y entro a comprar", temp->identificador, temp->carreta->no_carreta);
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
            printf("\nEl cliente %i agarro la carreta %i y entro a comprar", temp->identificador, temp->carreta->no_carreta);
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
            printf("\nEl cliente %i agarro la carreta %i y entro a comprar", temp->identificador, temp->carreta->no_carreta);
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
    int noCaja = cajaLibre(ing);
    if (noCaja != -1 && cola_pago.longitud == 0)
    {
        printf("longitud cola %i", cola_pago.longitud);
        printf("\nEl cliente %i paso directamente a la caja %i por no haber cola de pago", ing->identificador, noCaja);
    }
    else
    {
        if (cola_pago.primero == NULL)
        {
            cola_pago.conteo = 1;
            cola_pago.conteo++;
            printf("\nEl cliente %i se agrego a la cola de pago", nuevo->identificador);
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
            printf("\nEl cliente %i se agrego a la cola de pago", nuevo->identificador);
            aux->siguiente = nuevo;
            cola_pago.longitud++;
        }
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
            cola_pago.longitud--;
            return aux;
        }
        else
        {
            aux = cola_pago.primero;
            cola_pago.primero = cola_pago.primero->siguiente;
            cola_pago.longitud--;
            return aux;
        }
    }
    else
    {
        return NULL;
    }
}
//Metodos de insercion y eliminación de clientes de las cajas
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
                sprintf(str, "\"Caja %d\\n%s\";", temp->no_caja, temp->estado);
                fputs(str, archivo);
            }
        }
        temp = temp->siguiente;
    }
    fclose(archivo);
}
int cajaLibre(Cliente *aIngresar)
{
    int cajaAbierta = -1;
    Caja *temp = cajas;
    while (temp != NULL)
    {
        if (temp->estado == "libre")
        {
            temp->actual = aIngresar;
            temp->estado = "ocupado";
            temp->tiempo_servicio++;
            cajaAbierta = temp->no_caja;
            break;
        }
        else
        {
            temp = temp->siguiente;
        }
    }
    return cajaAbierta;
}
int cajaLibre2()
{
    int cajaAbierta = -1;
    Caja *temp = cajas;
    while (temp != NULL)
    {
        if (temp->estado == "libre")
        {
            return temp->no_caja;
            break;
        }
        else
        {
            temp = temp->siguiente;
        }
    }
    return cajaAbierta;
}
void liberarCajas()
{
    Caja *temp = cajas;
    srand(time(NULL));
    while (temp != NULL)
    {
        if (temp->estado == "ocupado")
        {
            int num = rand() % 2;
            if (num == 1)
            {
                int num2 = rand() % 10;
                Cliente *aEliminar = temp->actual;
                if (num2 < 5)
                {
                    printf("\nEl cliente %i termina de pagar, libera la caja %i y la carreta %i",
                           aEliminar->identificador, temp->no_caja, aEliminar->carreta->no_carreta);
                    ingresarCarreta2(aEliminar->carreta, 0);
                    temp->estado = "libre";
                    temp->tiempo_servicio++;
                }
                else
                {
                    printf("\nEl cliente %i termina de pagar, libera la caja %i y la carreta %i",
                           aEliminar->identificador, temp->no_caja, aEliminar->carreta->no_carreta);
                    ingresarCarreta2(aEliminar->carreta, 1);
                    temp->estado = "libre";
                    temp->tiempo_servicio++;
                }
                free(aEliminar);
            }
            temp = temp->siguiente;
        }
        else
        {
            temp = temp->siguiente;
        }
    }
}

void agrupar()
{
    //Método para agrupar los items de la lista de cajas en un mismo recuadro en graphviz
    Caja *temp = cajas;
    FILE *archivo = fopen(instruccion, "a");
    if (temp != NULL)
    {
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
    }

    //Método para agrupar los items de la lista de clientes en el ciclo de compras
    Cliente *temp2 = lista.primero;
    if (temp2 != NULL)
    {
        fputs("\nsubgraph cluster_3 {\nnode [style=filled];\n", archivo);
        do
        {
            if (temp2->siguiente != NULL)
            {
                char str[100];
                sprintf(str, " \"Cliente %i\\nCarreta %i\" ",
                        temp2->identificador, temp2->carreta->no_carreta);
                fputs(str, archivo);
            }
            temp2 = temp2->siguiente;
        } while (temp2 != lista.primero);
        fputs(";\ncolor=red;\nlabel=\"Lista de compradores\";}\n", archivo);
    }
    else
    {
        fputs("\nsubgraph cluster_3 {node [style=filled];rankdir=TB;node4;color=blue;label=\"Lista de compradores\";}\n", archivo);
    }

    Cliente *temp3 = cola_pago.primero;
    if (temp3 != NULL)
    {
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
    }
    else
    {
        fputs("\nsubgraph cluster_0 {node [style=filled];rankdir=TB;node3;color=blue;label=\"Cola de pago\";}\n", archivo);
    }

    Cliente *temp4 = cola_espera.primero;
    if (temp4 != NULL)
    {
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
    }
    else
    {
        fputs("\nsubgraph cluster_1 {node [style=filled];rankdir=TB;node2;color=blue;label=\"Cola de espera\";}\n", archivo);
    }
    Carreta *temp5 = carreta_1.primera;
    if (temp5 != NULL)
    {
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
    }
    else
    {
        fputs("\n node0[label=\"VACIA\"];\n", archivo);
        fputs("\nsubgraph cluster_4 {node [style=filled];rankdir=TB;node0;color=blue;label=\"Pila de carretas 1\";}\n", archivo);
    }
    Carreta *temp6 = carreta_2.primera;
    if (temp6 != NULL)
    {
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
    }
    else
    {
        fputs("\n node1[label=\"VACIA\"];\n", archivo);
        fputs("\nsubgraph cluster_5 {node [style=filled];rankdir=TB;node1;color=blue;label=\"Pila de carretas 2\";}\n", archivo);
    }
    fclose(archivo);
    FILE *archivo3 = fopen(instruccion, "a");
    fputs("}", archivo3);
    fclose(archivo3);
}
void graficar()
{
    sprintf(instruccion, "nuevo.txt", graficas);
    sprintf(instruccion2, "paso%i.png", graficas);
    FILE *archivo2 = fopen(instruccion, "w");
    fputs("digraph G{\n rankdir=LR; graph [fontsize=10 fontname=\"Verdana\"];\nnode [shape=record fontsize=10 fontname=\"Verdana\"];\n", archivo2);
    fclose(archivo2);
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
                sprintf(str, "\"Caja %d\\n%s\";", temp->no_caja, temp->estado);
                fputs(str, archivo);
            }
        }
        temp = temp->siguiente;
    }
    Cliente *temp2 = cola_espera.primero;
    if (temp2 != NULL)
    {
        do
        {
            if (temp2->siguiente != NULL)
            {
                char str[100];
                sprintf(str, "\"Cliente %i\"->\"Cliente %i\";\n",
                        temp2->identificador,
                        temp2->siguiente->identificador);
                fputs(str, archivo);
            }
            temp2 = temp2->siguiente;
        } while (temp2 != NULL);
        fputs("\n", archivo);
    }
    else
    {
        fputs("\n node2[label=\"VACIA\"];\n", archivo);
    }
    Cliente *temp3 = cola_pago.primero;
    if (temp3 != NULL)
    {
        do
        {
            if (temp3->siguiente != NULL)
            {
                char str[100];
                sprintf(str, "\"Cliente %i\\nCarreta %i\"->\"Cliente %i\\nCarreta %i\";\n",
                        temp3->identificador, temp3->carreta->no_carreta,
                        temp3->siguiente->identificador, temp3->siguiente->carreta->no_carreta);
                fputs(str, archivo);
            }
            temp3 = temp3->siguiente;
        } while (temp3 != NULL);
        fputs("\n", archivo);
    }
    else
    {
        fputs("\n node3[label=\"VACIA\"];\n", archivo);
    }
    Cliente *temp4 = lista.primero;
    if (temp4 != NULL)
    {
        do
        {
            if (temp4->siguiente != NULL)
            {
                char str[400];
                sprintf(str, "\"Cliente %i\\nCarreta %i\"->\"Cliente %i\\nCarreta %i\";\n",
                        temp4->identificador, temp4->carreta->no_carreta,
                        temp4->siguiente->identificador, temp4->siguiente->carreta->no_carreta);
                fputs(str, archivo);
            }
            temp4 = temp4->siguiente;
        } while (temp4 != lista.primero);
        printf("\n");
    }
    else
    {
        fputs("\n node4[label=\"VACIA\"];\n", archivo);
    }
    fclose(archivo);
    agrupar();
    char aIntroducir[100];
    sprintf(aIntroducir, "dot -Tpng %s -o %s", instruccion, instruccion2);
    system(aIntroducir);
}
Cliente *crearNodoCliente()
{
    return (Cliente *)malloc(sizeof(Cliente));
}
void iniciarListas()
{
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
    printf("*************************PASO 0**************************");
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
    char aIntroducir[100];
    char pasos[100];
    int cuantos = 0;
    int salida = 0;
    int opciones = 0;
    iniciarListas();
    graficar();
    printf("¿Cuántos clientes ingresan?");
    scanf("%i", &cuantos);
    while (salida == 0)
    {
        graficas++;
        sprintf(pasos, "*************************PASO %i**************************", graficas);
        printf("\n\n");
        printf(pasos);
        printf("\n\n");
        for (int i = 0; i < cuantos; i++)
        {
            ingresarAColaEspera();
        }
        for (int o = 0; o < cola_espera.conteo - 1; o++)
        {
            if (carreta_1.longitud > 0 || carreta_2.longitud > 0)
            {
                agarrarCarreta();
            }
        }
        int pasado = saleDeCompras();
        if (pasado != -1)
        {
            ingresarAColaPagos(eliminarClienteDeCompras(pasado));
        }
        else
        {
            printf("\nNingun cliente sale de comprar en este paso");
        }
        if (cola_pago.longitud > 0)
        {
            int cantidad_inicial = cola_pago.longitud;
            for (int i = 0; i < cantidad_inicial; i++)
            {
                int donde = cajaLibre2();
                if (donde != -1)
                {
                    Cliente *tep = popColaPago();
                    int donde2 = cajaLibre(tep);
                    printf("\nEl cliente %i pasa a la caja %i", tep->identificador, donde2);
                }
            }
        }
        else
        {
            printf("\nNo hay personas haciendo cola para pagar");
        }
        liberarCajas();
        printf("\n\nQue deseas realizar? -- Graficar y continuar con la simulacion (1) -- Graficar y terminar simulacion (2) -- Terminar la simulacion (3)");
        printf("\n\n");
        scanf("%i", &opciones);
        switch (opciones)
        {
        case 1:
            graficar();
            printf("Cuantos clientes ingresan?  ");
            scanf("%i", &cuantos);
            break;
        case 2:
            graficar();
            salida = 10;
            break;
        case 3:
            salida = 10;
            break;
        default:
            salida = 10;
            break;
        }
    }

    return 0;
}