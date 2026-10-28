#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

//Declaring data structure
struct nodo_autostrada{     //declaration for the list of stations on the highway
    int distance;
    int max;
    struct nodo_cars* cars;
    struct nodo_autostrada *next;
};
typedef struct nodo_autostrada* list_stations;

struct nodo_cars{           //declaration for a list of cars
    int autonomy;
    struct nodo_cars *next;
};
typedef struct nodo_cars *list_cars;


//function declaration
int add_station(list_stations*, int);
void add_car(list_cars*, int);
void aggiungi_auto(list_stations*, int, int);
void completa_stazione(list_stations*, int);
void delete_car(list_stations*, int, int);
void demolisci_stazione(list_stations*, int);
void shortest_path(list_stations*, int, int);
int find_position(list_stations*, int);
int min_distance(int*, bool*, int, int);
void free_all(list_stations*, int);

//void print_car_station(list_stations*);
//void printMax(list_stations*);

//main code
int main() {
    int distance, destination, car;
    char *command, *input;
    list_stations stations = NULL;
    command = (char*) malloc(20*sizeof(char));
    input = (char*) malloc(16*sizeof(char));

    while(scanf("%s", command) != EOF){           //read the main command on every new line

        if(strncmp("aggiungi-stazione", command, 17) == 0){     //branch for command aggiungi-stazione
            if(scanf(" %s", input) == 1){                //read the distance and convert it to integer
                distance = strtol(input, NULL, 10);
                if(add_station(&stations, distance)==1){                           //if we add the station succesfully
                    completa_stazione(&stations, distance);
                    printf("aggiunta\n");           //we print "aggiunta"
                }
                else{
                    while(scanf("%s", input)==1){
                        if(getchar()=='\n'){
                            break;
                        }
                    }
                    printf("non aggiunta\n");       //else we print non aggiunta
                }
            }
        }

        else if(strncmp("demolisci-stazione", command, 18) == 0){       //branch for command demolisci-stazione
            if(scanf(" %s", input) == 1){
                distance = strtol(input, NULL, 10);
                demolisci_stazione(&stations, distance);
                getchar();
            }
        }

        else if(strncmp("aggiungi-auto", command, 13) == 0){            //branch for command aggiungi-auto
            if(scanf(" %s", input) == 1){
                distance = strtol(input, NULL, 10);
                if(scanf(" %s", input) == 1){
                    car = strtol(input, NULL, 10);
                    aggiungi_auto(&stations, distance, car);
                    getchar();
                }
            }
        }

        else if(strncmp("rottama-auto", command, 12) == 0){             //branch for command rottama-auto
            if(scanf(" %s", input) == 1){
                distance = strtol(input, NULL, 10);
                if(scanf(" %s", input)==1){
                    car = strtol(input, NULL, 10);
                    delete_car(&stations, distance, car);
                }
            }
            getchar();
        }

        else if(strncmp("pianifica-percorso", command, 18) == 0){       //branch for command pianifica-percorso
            if(scanf(" %s", input) == 1){
                distance = strtol(input, NULL, 10);
                if(scanf(" %s", input) == 1){
                    destination = strtol(input, NULL, 10);
                    shortest_path(&stations, distance, destination);
                }
            }
            getchar();
        }
        //print_car_station(&stations);
        //printMax(&stations);
    }
    //free the space allocated for command and the input
    while(stations != NULL){
        list_stations next_station = stations->next;
        free_all(&stations, stations->distance);
        stations = next_station;
    }
    free(input);
    free(command);
    return 0;
}


int add_station(list_stations *head, int distance){
    list_stations newNode = (list_stations) malloc(sizeof(struct nodo_autostrada));
    newNode->distance = distance;
    newNode->max = 0;
    newNode->cars = NULL;           //create the space for the node and generate the datas
    if(*head != NULL && distance == (*head)->distance){
        free(newNode);
        return 0;
    }
    //case to add the node in the head and return
    if(*head == NULL || distance < (*head)->distance){
        newNode->next = *head;
        *head = newNode;
        return 1;
    }       //else find the right position

    list_stations curr = *head;
    while(curr->next != NULL && curr->next->distance < newNode->distance){
        curr = curr->next;
    }
    if(curr->next != NULL && curr->next->distance == newNode->distance){
        free(newNode);
        return 0;
    }
    newNode->next = curr->next;
    curr->next = newNode;
    return 1;
}


void add_car(list_cars *head, int autonomy ){
    list_cars new_node = (list_cars)malloc(sizeof(struct nodo_cars));
    new_node->autonomy = autonomy;
    new_node->next = (*head);
    (*head) = new_node;
}


void aggiungi_auto(list_stations *head, int target_distance, int newcar){
    if(*head != NULL){
        if((*head)->distance == target_distance){
            add_car(&(*head)->cars, newcar);
            if(newcar > (*head)->max){
                (*head)->max = newcar;
            }
            printf("aggiunta\n");
            return;
        }
        list_stations curr = *head;
        while(curr->next != NULL && curr->next->distance < target_distance){
            curr = curr->next;
        }
        if(curr->next != NULL && curr->next->distance == target_distance){
            add_car(&curr->next->cars, newcar);
            if(newcar > curr->next->max){
                curr->next->max = newcar;
            }
            printf("aggiunta\n");
            return;
        }
    }
    printf("non aggiunta\n");
}


void completa_stazione(list_stations *head, int target_distance){
    char *reader;
    int value;
    reader = (char*)malloc(16*sizeof(char));
    if(*head != NULL){
        if((*head)->distance == target_distance){
            while(scanf(" %s", reader) == 1){
                value = strtol(reader, NULL, 10);
                if(value > (*head)->max){
                    (*head)->max = value;
                }
                add_car(&(*head)->cars, value);
                if(getchar() == '\n'){
                    break;
                }
            }
            free(reader);
            return;
        }

        list_stations curr = *head;
        while(curr->next->distance < target_distance){
            curr = curr->next;
        }
        if(curr->next->distance == target_distance){
            while(scanf(" %s", reader) == 1){
                value = strtol(reader, NULL, 10);
                if(value > curr->next->max){
                    curr->next->max = value;
                }
                add_car(&curr->next->cars, value);
                if(getchar() == '\n'){
                    break;
                }
            }
        }
    }
    free(reader);
}


void delete_car(list_stations *head, int target_distance, int target_car){
    if(*head != NULL){
        if((*head)->distance == target_distance){       //if the head->distance already is the distance we are looking for
            list_cars curr = (*head)->cars;
            list_cars prev = NULL;
            while(curr != NULL && curr->autonomy != target_car){
                prev = curr;
                curr = curr->next;                      //we scroll until we find the car
            }
            if(curr != NULL){                       //if we actually find a car
                if(prev == NULL){
                    (*head)->cars = curr->next;             //if it's the first car on the list we just update the head of the list of cars
                }
                else{
                    prev->next = curr->next;        //else we will have prev and curr somewhere and we isolate curr
                }
                free(curr);             //free curr after we isolated it
                if((*head)->max == target_car){              //check if we just canceled our most autonomous car
                    curr = (*head)->cars;
                    int max = 0;
                    while(curr != NULL){                    //we check every car to take the new most autonomous
                        if(curr->autonomy > max){
                            max = curr->autonomy;
                        }
                        curr = curr->next;
                    }
                    (*head)->max = max;
                }
                printf("rottamata\n");
                return;
            }
        }

        list_stations curr = *head;
        while(curr->next != NULL && curr->next->distance < target_distance){        //we look for a node
            curr = curr->next;
        }
        if(curr->next != NULL && curr->next->distance == target_distance){      //if we find it we do the same thing we did before
            list_cars curr_car = curr->next->cars;
            list_cars prev_car = NULL;
            while(curr_car != NULL && curr_car->autonomy != target_car){
                prev_car = curr_car;
                curr_car = curr_car->next;
            }
            if(curr_car != NULL){
                if(prev_car == NULL){
                    curr->next->cars = curr_car->next;
                }
                else{
                    prev_car->next = curr_car->next;
                }
                free(curr_car);
                if(curr->next->max == target_car){
                    curr_car = curr->next->cars;
                    int max = 0;
                    while(curr_car != NULL){
                        if(curr_car->autonomy > max){
                            max = curr_car->autonomy;
                        }
                        curr_car = curr_car->next;
                    }
                    curr->next->max = max;
                }
                printf("rottamata\n");
                return;
            }
        }
    }
    printf("non rottamata\n");
}


void demolisci_stazione(list_stations *head, int distance){
    if(*head != NULL){
        if((*head)->distance == distance){          //if the head is the station we want to demolish
            list_cars curr = (*head)->cars;
            list_cars prev = NULL;
            while(curr != NULL){
                prev = curr;
                curr = curr->next;
                free(prev);                 //we firstly free all the cars stored in it
            }
            list_stations next_head = (*head)->next;        //then we generate our next head
            free(*head);            //we free our old head
            *head = next_head;                  //and assign our new head
            printf("demolita\n");
            return;
        }
        list_stations curr = *head;             //we repeat the passages seen above after we found the right node
        while(curr->next != NULL && curr->next->distance < distance){
            curr = curr->next;
        }
        if(curr->next != NULL && curr->next->distance == distance){
            list_cars curr_car = curr->next->cars;
            list_cars prev_car = NULL;
            while(curr_car != NULL){
                prev_car = curr_car;
                curr_car = curr_car->next;
                free(prev_car);
            }
            list_stations next = curr->next->next;
            free(curr->next);
            curr->next = next;
            printf("demolita\n");
            return;
        }
    }
    printf("non demolita\n");
}


// I use the position of a node in the list as a flag to run a dijkstra algorithm
void shortest_path(list_stations *head, int source, int destination){
    int n=0;
    list_stations curr = *head, analysed;
    while(curr != NULL){            //we count the number of nodes in our list and store it as int n
        n++;
        curr = curr->next;
    }
    struct nodo_autostrada **node_ptrs;
    node_ptrs = (struct nodo_autostrada **) malloc(n*sizeof(struct nodo_autostrada*));
    curr = *head;
    for(int iter = 0; iter<n; iter++){
        node_ptrs[iter] = curr;             //assegno un array di puntatori ai nodi della lista per accedere più velocemente ai diversi nodi
        curr = curr->next;
    }

    int *dist = (int*)malloc(n*sizeof(int));        //array that support dijkstra implementation
    bool *visited = (bool*)malloc(n*sizeof(bool));     //array that support dijkstra implementation (priority queue)
    int *parent = (int*)malloc(n*sizeof(int));      //array that support dijkstra implementation

    for(int i=0; i<n; i++){
        dist[i] = INT_MAX;      //when a value of dist[i] == INT_MAX it means it is not in reach
        visited[i] = false;         //this let us understand whenever during the algorithm a node has already been visited
        parent[i] = -1;         //will keep trace of the parent for the node in each position
    }
    int position_s = find_position(&(*head), source);       //define the source position in the list
    int position_d = find_position(&(*head), destination);  //define the position of the destination in the list
    int u = position_s;
    if(position_s != INT_MAX && position_d != INT_MAX){
        dist[position_s] = 0;
        for(int count = 0; count<n-1; count++){
            u = min_distance(dist, visited, n, u);     //look for the most promising node
            if(u == -1){
                break;
            }
            visited[u] = true;              //we update the distance values for that node
            analysed = node_ptrs[u];        //look for the node we are analysing next
            int lower = analysed->distance - analysed->max;
            int upper = analysed->distance + analysed->max;         //update the values of reach
            //want to iterate on each node vedere to see if we are in the range of distances and check if dist[u] + 1 < dist[v] if so update dist and parent
            for(int v=0; v<n; v++){
                curr = node_ptrs[v];
                if(curr->distance >= lower && curr->distance <= upper && v!=u){
                    if(dist[u]+1 < dist[v]){
                        dist[v] = dist[u]+1;
                        parent[v] = u;
                    }
                }
                if(curr->distance >= upper){
                    break;
                }
            }
            if(dist[position_d] != INT_MAX){
                break;
            }
        }
        if(dist[position_d] == INT_MAX || parent[position_d] == -1){        //se la distanza è infinita o non ha genitori
            printf("nessun percorso\n");
            free(dist);
            free(visited);
            free(parent);
            return;
        }
        //array of parents has been generated as well as dist, I want to program the path printer
        int path[n];            //worst case possible I had to travel trough every node
        int path_length = 0;            //count node put in the array
        int current_node = position_d;
        path[path_length] = current_node;
        while(current_node != -1){
            path_length++;
            current_node = parent[current_node];
            path[path_length] = current_node;
        }
        for(int i = path_length-1; i>=0; i--){
            curr = node_ptrs[path[i]];
            if(i!=0){
                printf("%d ", curr->distance);
            }
            else{
                printf("%d\n", curr->distance);
            }
        }
        free(dist);
        free(visited);
        free(parent);
        return;
    }
    free(dist);
    free(visited);
    free(parent);
    printf("nessun percorso\n");
}


int find_position(list_stations *head, int search){          //find the node in a certain position
    int n=0;
    list_stations curr = *head;
    while(curr != NULL && curr->distance < search){
        n++;
        curr = curr->next;
    }
    if(curr->distance == search){
        return n;
    }
    return INT_MAX;             //if the node is not existing I return INT_MAX
}


int min_distance(int dist[], bool visited[], int n, int check){
    int min = INT_MAX, min_index=-1;
    for(int v = 0; v<n; v++){
        if(v>check && dist[v] == INT_MAX){
            break;
        }
        if(visited[v] == false && dist[v]<min){
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}


void free_all(list_stations *head, int distance){
    if(*head != NULL){
        if((*head)->distance == distance){          //if the head is the station we want to demolish
            list_cars curr = (*head)->cars;
            list_cars prev = NULL;
            while(curr != NULL){
                prev = curr;
                curr = curr->next;
                free(prev);                 //we firstly free all the cars stored in it
            }
            list_stations next_head = (*head)->next;        //then we generate our next head
            free(*head);            //we free our old head
            *head = next_head;                  //and assign our new head
            return;
        }
        list_stations curr = *head;             //we repeat the passages seen above after we found the right node
        while(curr->next != NULL && curr->next->distance < distance){
            curr = curr->next;
        }
        if(curr->next != NULL && curr->next->distance == distance){
            list_cars curr_car = curr->next->cars;
            list_cars prev_car = NULL;
            while(curr_car != NULL){
                prev_car = curr_car;
                curr_car = curr_car->next;
                free(prev_car);
            }
            list_stations next = curr->next->next;
            free(curr->next);
            curr->next = next;
            return;
        }
    }
}



