#include "veb.h"

int high(int x, int u){

    return (int)(x/(int)sqrt(u));
}

int low(int x, int u){

    return (x % (int)sqrt(u));
}

vEB* vEB_tree_member(vEB *V, int x){

    if(!V)
        return NULL;
    else if(V->min == x || V->max == x)
        return V;
    else{
        if(V->cluster)
            return vEB_tree_member(V->cluster[high(x, V->u)], low(x, V->u));
        else
            return NULL;
    }
}

int vEB_tree_Minimum(vEB *V){

    return V->min;
}

int vEB_tree_Maximum(vEB *V){

    return V->max;
}

vEB* vEB_tree_insert(vEB *V, int x, int u){

    if(!V){
        V = (vEB*)malloc(sizeof(vEB));
        V->min = V->max = x;
        V->u = u;
        if(u > 2){
            V->summary = NULL;
            V->cluster = (vEB**)calloc(sqrt(u),sizeof(vEB*));
        }
        else{
            V->summary = NULL;
            V->cluster = NULL;
        }
        return V;
    }
    else{
        if(x < V->min){
            int aux = V->min;
            V->min = x;
            x = aux;
        }
        if(V->u > 2){
            if(V->cluster[high(x, V->u)] == NULL){
                V->summary = vEB_tree_insert(V->summary, high(x, V->u), sqrt(V->u));
                V->cluster[high(x, V->u)] =
                vEB_tree_insert(V->cluster[high(x, V->u)], low(x, V->u), sqrt(V->u));
            }
            else{
                V->cluster[high(x, V->u)] =
                vEB_tree_insert(V->cluster[high(x, V->u)], low(x, V->u), sqrt(V->u));
            }
        }
        if(x > V->max)
            V->max = x;
        return V;
    }
}

vEB* vEB_tree_delete(vEB *V, int x, int u){
    if(V->min == V->max){
        free(V);
        return NULL;
    }
    else if(V->u == 2){
        if(x == 0)
            V->min = 1;
        else
            V->min = 0;
        V->max = V->min;
        return V;
    }
    else{
        if(x == V->min){
            int first_cluster = vEB_tree_Minimum(V->summary);
            x = first_cluster * (int)sqrt(V->u) + vEB_tree_Minimum(V->cluster[first_cluster]);
            V->min = x;
            V->cluster[high(x, V->u)] = vEB_tree_delete(V->cluster[high(x, V->u)], low(x, V->u), sqrt(V->u));
            if(V->cluster[high(x, V->u)] == NULL){
                V->summary = vEB_tree_delete(V->summary, high(x, V->u), sqrt(V->u));
                if(x = V->max){
                    if(!V->summary){
                        V->max = V->min;
                    }
                    else{
                        int summary_max = vEB_tree_Maximum(V->summary);
                        V->max = summary_max * (int)sqrt(V->u) + vEB_tree_Maximum(V->cluster[summary_max]);
                    }
                }
            }
            else if(x == V->max){
                V->max = high(x, V->u) * (int)sqrt(V->u) + vEB_tree_Maximum(V->cluster[high(x, V->u)]);
            }
        }
        return V;
    }
}

void vEB_tree_print(vEB *V){
    if(V){
        printf("min: %d\tmax: %d\tu: %d\tsummary: %d\n", V->min, V->max, V->u, V->summary);
        if(V->cluster){
            int i=0;
            printf("cluster: ");
            for(i = 0; i < sqrt(V->u); i++){
                printf("%d:%d\t", i, V->cluster[i]);
            }
            printf("\n");
        }
        else{
            printf("cluster: 0\n");
        }
        vEB_tree_print(V->summary);
        if(V->cluster){
            int i, tam = sqrt(V->u);
            for(i = 0; i < tam; i++){
                vEB_tree_print(V->cluster[i]);
            }
            printf("\n\n");
        }
    }
}

void vEB_tree_print_number(vEB *V){
    if(V){
        printf("min: %d\n", V->min);
        if(V->u == 2){
            if(V->min != V->max)
                printf("max: %d\n", V->max);
        }
        if(V->cluster){
            int i, tam = sqrt(V->u);
            for(i = 0; i < tam; i++){
                vEB_tree_print_number(V->cluster[i]);
            }
        }
    }
}
