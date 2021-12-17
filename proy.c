#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
    int id;
    float p_x;
    float p_y;
    int score;
    int dominio[];
} Nodo;

typedef struct {   
    int score_act;
    float px_act; 
    float py_act;
    float t_act;
    int ruta_act[];
} Arbol;


Arbol* Tree(int N) {  // Crear un arbol vacio
    Arbol* t;
    t = (Arbol*)malloc(sizeof(Arbol)+4*N);
    t->score_act = 0;
    t->px_act = 0.0;
    t->py_act = 0.0;
    t->t_act = 0.0;
    for(int i=0; i<N; i++){
        t->ruta_act[i] = 0;
    }
    return t;
}

Nodo* NuevoNodo(int N, int id, float px, float py, int score, int* dominio){
    Nodo* n;
    n = (Nodo*)malloc(sizeof(Nodo)+4*N);
    n->id = id;
    n->p_x = px;
    n->p_y = py;
    n->score = score;
    n->dominio[0] = 0;
    for(int i=0; i<N; i++){
        n->dominio[i] = dominio[i];
    }
    return n;
}

Nodo* ReiniciarDominios(Nodo* n, int N){
    for(int i=0 ; i<N ; i++){
            if(n->id==i || i==0){
                n->dominio[i] = 0;
                continue;
            }else{
                n->dominio[i] = i;
            }
        }
    return n;
}

Nodo* CompararRuta(Nodo* n, Arbol* t, int N){
    for(int l = 0; l<N; l++){
        for(int i = 0; i<N; i++){
            if (n->dominio[l] == t->ruta_act[i]){
                n->dominio[l] = 0;
            }
        }       
    }
    return n;
}

Nodo* IrAtras(Nodo* n, int N, int id){
    printf("i: %d, id: %d\n",n->id, id);
    for(int i=0; i<N; i++){
        if(n->dominio[i] == id){
            n->dominio[i] = 0;
        }
         printf("dom: %d\n",n->dominio[i]);
    }
    return n;
}

int QuedaAlgo(Nodo* n, int N){
    int flag = 0;
    if(n->id == 0){
        for(int i = 0; i<N-1; i++){
        if(n->dominio[i]==0){
            continue;
        } else{
            flag = 1;
        }
    }
    }else{
        for(int i = 0; i<N; i++){
            if(n->dominio[i]==0){
                continue;
            } else{
                flag = 1;
            }
        }
    }
    
    return flag;

}

int VerSolFact(Arbol* t, Nodo* n, int tmax, int N){
    int flag = 0;
    float dist = sqrt(pow(n->p_x - t->px_act, 2)+pow(n->p_y - t->py_act, 2));
    if((t->t_act + dist) > tmax){
        flag = 1;
    }
    if(flag == 0){ //factible
    // ver si esta en la ruta
        int f = 0;
        for(int i = 0; i< N; i++){
            printf("ruta,id : %d, %d\n",t->ruta_act[i], n->id);
            if(t->ruta_act[i] == n->id){
                printf("AAAAAAAAAAAA\n");
                f = 1;
            }
        }
        if(f==1){
            flag = 1;
            return flag;
        }

        t->px_act = n->p_x;
        t->py_act = n->p_y;
        t->t_act = t->t_act+dist;
        t->score_act += n->score;
        
    }

    return flag;
}

int main() { 
    FILE *archivo  = fopen("prueba1.txt", "r");
    int n, m;
    float tmax;
    char buffer[100];
    
    fscanf(archivo,"%s %d", buffer, &n);
    fscanf(archivo,"%s %d", buffer, &m);
    fscanf(archivo,"%s %f", buffer, &tmax);
    
	float p_x, p_y;
    int score;
    
	Nodo* nodos[n];
    int dominio[n-1];
    int i = 0;
    while(!feof(archivo)){
    	fscanf(archivo,"%f %f %d",&p_x,&p_y,&score);
        for(int j=0 ; j<n ; j++){
            if(j==i || j==0){
                dominio[j] = 0;
                continue;
            }else{
                dominio[j] = j;
            }
        }
        Nodo *nodo;
        nodo = NuevoNodo(n,i,p_x,p_y,score,dominio);
        nodos[i] = nodo;
    	i++;
    }

    fclose(archivo);

    //Primero, encontrar la ruta óptima
    FILE *rutas  = fopen("rutas.txt", "w+");
    int k = 0;
    Arbol* t = Tree(n);

    //version dos:
    int cont = 0;
    for(int i = 0; i<n; i++){ //recorre los nodos
        //printf("i: %d\n",i);
        if(i==0){
            int a = QuedaAlgo(nodos[i], n);
            if(a == 0){ //no queda nada mas por ver, ir atras
                break;
            }
            t->px_act = nodos[i]->p_x;
            t->py_act = nodos[i]->p_y;
            t->ruta_act[0] = nodos[i]->id;
        }else if(i == n-1){
            //printf("Termino la ruta\n");
            //imprimir rutas en el archivo y volver atras,
            fprintf(rutas,"score: %d, tiempo: %f\n",t->score_act, t->t_act);
            int ultimo = 0, u_pos = 0;
            for(int j = 0; j<n; j++){
                if(t->ruta_act[j] == 0){
                    continue;
                }else{
                    fprintf(rutas,"%d\n",t->ruta_act[j]);
                    ultimo = t->ruta_act[j];
                    u_pos = j;
                }
            }
            fprintf(rutas,"%d\n",-1);
            //volver atrás
            //ultimo -> nodos[ultimo]
            //Nodo* anterior = nodos[t->ruta[ultimo-1]]
            //nodos[i] = IrAtras(nodos[i-1],n, nodos[i]->id);
            nodos[i] = ReiniciarDominios(nodos[ultimo], n);
            nodos[t->ruta_act[u_pos-1]] = IrAtras(nodos[t->ruta_act[u_pos-1]],n, nodos[ultimo]->id);
            
            t->px_act = nodos[t->ruta_act[u_pos-1]]->p_x;
            t->py_act = nodos[t->ruta_act[u_pos-1]]->p_y;
            t->score_act -= nodos[ultimo]->score;
            t->t_act = t->t_act - (pow(nodos[ultimo]->p_x - nodos[t->ruta_act[u_pos-1]]->p_x, 2)+pow(nodos[ultimo]->p_y - nodos[t->ruta_act[u_pos-1]]->p_y, 2));
            t->ruta_act[k-1] = 0;
            k-=1;
            i = i - 2;
        }
        for(int j = 0; j<n; j++){
            nodos[i] = CompararRuta(nodos[i], t, n);
            //printf("dominio: %d\n", nodos[i]->dominio[j]);
            // ver si queda algo 
            int a = QuedaAlgo(nodos[i], n);
            if(a == 0){ //no queda nada mas por ver, ir atras
                //printf("no queda nada por ver\n");
                nodos[i] = ReiniciarDominios(nodos[i], n);
                nodos[i-1] = IrAtras(nodos[i-1],n, nodos[i]->id);
                
                //actualizar los tiempos, score y posiciones
                int a = t->ruta_act[k-2];
                t->px_act = nodos[a]->p_x;
                t->py_act = nodos[a]->p_y;
                t->score_act -= nodos[i]->score;
                //printf("tiempo 1: %f\n",t->t_act);
                //printf("ndos: %d\n",nodos[i]->id);
                t->t_act = t->t_act - sqrt(pow(nodos[i]->p_x - nodos[a]->p_x, 2)+pow(nodos[i]->p_y - nodos[a]->p_y, 2));
                //printf("tiempo 2: %f\n",t->t_act);
                t->ruta_act[k-1] = 0;
                k-=1;
                i = i - 2;
            }
            else if(nodos[i]->dominio[j] == 0){ //dominio ya revisado
                continue;
            } else{
                int f = VerSolFact(t, nodos[nodos[i]->dominio[j]], tmax, n);
                //printf("fact: %d\n",f);
                if(f == 0){ //factible -> ya se manejo, seguir dando vuelta
                    t->ruta_act[k] = nodos[nodos[i]->dominio[j]]->id;
                    k+=1;
                    //printf("ruta: %d\n",t->ruta_act[k]);
                    i = nodos[nodos[i]->dominio[j]]->id - 1;
                    break;
                } else{ //no factible
                    nodos[i]->dominio[j] = 0;
                    int a = QuedaAlgo(nodos[i], n);
                    if(a == 0){ //no queda nada mas por ver, ir atras
                        //printf("no queda nada por ver\n");
                        nodos[i] = ReiniciarDominios(nodos[i], n);
                        nodos[i-1] = IrAtras(nodos[i-1],n, nodos[i]->id);
                        
                        //actualizar los tiempos, score y posiciones
                        int a = t->ruta_act[k-2];
                        t->px_act = nodos[a]->p_x;
                        t->py_act = nodos[a]->p_y;
                        t->score_act -= nodos[i]->score;
                        t->t_act = t->t_act - sqrt(pow(nodos[i]->p_x - nodos[i-1]->p_x, 2)+pow(nodos[i]->p_y - nodos[i-1]->p_y, 2));
                        t->ruta_act[k-1] = 0;
                        k-=1;
                        i = i - 2;
                    }
                }
            }

        }
        if(cont == 100){
            break;
        } else{
            cont+=1;
        }

    }
    remove("rutas.txt");
    printf("El arbol terminó en: %f, %f\n",t->px_act, t->py_act);
    printf("tiempo: %f\n", t->t_act);
    printf("score: %d\n", t->score_act);
    for(int i = 0; i < n; i++){
        printf("ruta: %d\n", t->ruta_act[i]);
    }
	return 0;
}