#include "abb.h"

typedef struct nodo_abb {
    struct nodo_abb* izq;
    struct nodo_abb* der;
    const char* clave;
    void* dato;
} nodo_abb_t;

struct abb {
    nodo_abb_t* raiz;
    size_t cant;
    abb_destruir_dato_t destruir;
    abb_comparar_clave_t comparar;
};

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
    abb_t* abb = malloc(sizeof(abb_t));
    if(!abb) return NULL;
    abb->cant = 0;
    abb->comparar = cmp;
    abb->destruir = destruir_dato;
    abb->raiz = NULL;
    return abb;
}

nodo_abb_t* nodo_crear(const char* clave, void* dato){
    nodo_abb_t* nodo = malloc(sizeof(nodo_abb_t));
    if(!nodo) return NULL;
    nodo->clave = clave;
    nodo->dato = dato;
    nodo->der = NULL;
    nodo->izq = NULL;
    return nodo;
}

size_t abb_cantidad(abb_t *arbol){
    return arbol->cant;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato) {
    if (arbol == NULL) {
        arbol->raiz = nodo_crear(clave, dato);
        return true;
    } else {
        char* clave_aux = arbol->raiz->clave;
        if (arbol->comparar(clave, clave_aux) < 0) {
            abb_guardar(arbol->raiz->izq, clave, dato);
        } else {
            abb_guardar(arbol->raiz->der, clave, dato);
        }
    }
    return true;
}    

void abb_destruir(abb_t *arbol){
    if(arbol->cant == 1){
        arbol->destruir(arbol->raiz);
    }
    abb_destruir(arbol);
    abb_destruir(arbol);
}

void *abb_obtener(const abb_t *arbol, const char *clave){
    if (arbol->raiz == NULL) return NULL;
    void* res = NULL;
    if (arbol->raiz->clave == clave) {
        res = arbol->raiz->dato;
        return res;
    }
    if(clave < arbol->raiz->clave) {
        arbol->raiz->izq = abb_obtener(arbol->raiz->izq, clave);
        if (arbol->raiz->izq->clave == clave) {
            res = arbol->raiz->izq->dato;
        }

    } else {
        arbol->raiz->der = abb_obtener(arbol->raiz->der, clave);
        if (arbol->raiz->der->clave == clave) {
            res = arbol->raiz->der->dato;
    }
    return res;
    }
}

bool abb_pertenece(const abb_t *arbol, const char *clave) {
    if (arbol->raiz == NULL) return false;
    if (arbol->raiz->clave == clave) return true;

    if(clave < arbol->raiz->clave) {
        arbol->raiz->izq = abb_pertenece(arbol->raiz->izq, clave);
        if (arbol->raiz->izq->clave == clave) {
            return true;
        }

    }else{
        arbol->raiz->der = abb_pertenece(arbol->raiz->der, clave);
        if (arbol->raiz->der->clave == clave) {
            return true;
        }
    }
    return false;
}

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){
    if(arbol){
        abb_in_order(arbol->raiz->izq, visitar(arbol->raiz->izq, arbol->raiz->der, extra), extra);
        abb_in_order(arbol->raiz->der, visitar(arbol->raiz->izq, arbol->raiz->der, extra), extra);
    }
}

nodo_abb_t* nodo_minimo(nodo_abb_t* nodo) {
    nodo_abb_t* actual = nodo;
    while (actual && actual->izq != NULL) {
        actual = actual->izq;
    }
    return actual;
}

void *abb_borrar(abb_t *arbol, const char *clave){
    if (arbol->raiz == NULL) return NULL;

    if (clave < arbol->raiz->clave) {
        arbol->raiz->izq = abb_borrar(arbol->raiz->izq, clave);
    }
    else if (clave > arbol->raiz->clave) {
        arbol->raiz->der = abb_borrar(arbol->raiz->der, clave);
    }
    else {
        if (arbol->raiz->izq == NULL) {
            nodo_abb_t* aux = arbol->raiz->der;
            free(arbol->raiz->der);
            return aux->dato;
        }
        else if (arbol->raiz->der == NULL) {
            nodo_abb_t* aux = arbol->raiz->izq;
            free(arbol->raiz->izq);
            return aux->dato;
        }
        else{
            nodo_abb_t* aux = nodo_minimo(arbol->raiz->der);
            arbol->raiz->clave = aux->clave;

            arbol->raiz->der = abb_borrar(arbol->raiz->der, aux->clave);
            return arbol->raiz->der->dato;
        }
    }
}