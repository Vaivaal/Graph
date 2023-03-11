/* Vaiva Ališauskaitė, 5 grupė
Duotas lėktuvų skrydžių sąrašas: miestas, miestas, kaina.
Rasti pigiausią skrydžio iš vieno duoto miesto į kitą duotą miestą maršrutą ir jo kainą.
Numatyti atvejį, kad toks maršrutas neegzistuoja.
(grafo realizacija paremta kaimynystės matrica)
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define DATA_FILE "flightData.txt"

void readData(int matrix[][20], char cities[][30], int *cityCount);
int isInArray(char str[], char array[][strlen(str)],  int cityCount);
int findLowestPrice(int dep, int dest, int matrix[][20], int cityCount, int path[]);
void shiftLoop(int n, int array[]);

int main()
{
    int matrix[100][20];
    char cities[100][30];
    int cityCount;

    readData(matrix, cities, &cityCount);
    int path[cityCount];

    int dep = 8, dest = 0;
    int price = findLowestPrice(dep, dest, matrix, cityCount, path);

    printf("dep: %s\n", cities[dep]);
    printf("dest: %s\n", cities[dest]);

    if (price != -1){
        printf("lowest price: %d\n", price);
        printf("path: ");
        for (int i = 0; i < cityCount; ++i){
            if (path[i] != -1)
                printf("%s ", cities[path[i]]);
        }
    }else{
        printf("there is no route to selected destination\n");
    }

    printf("\n");
    for (int i = 0; i < cityCount; ++i){
        for (int j = 0; j < cityCount; ++j){
            printf("%3d ", matrix[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < cityCount; ++i){
        printf("%s, %d\n", cities[i], i);
    }

    return 0;
}
int findLowestPrice(int dep, int dest, int matrix[][20], int cityCount, int path[] ){

    int price[cityCount];
    int visited[cityCount];
    int paths[cityCount][cityCount];

    for (int i = 0; i < cityCount; ++i){
        price[i] = INT_MAX;
        visited[i] = 0;
        path[i] = -1;
    }

    for (int i = 0; i < cityCount; ++ i){
        for (int j = 0; j < cityCount; ++j){
            paths[i][j] = -1;
        }
    }

    price[dep] = 0;

    for (int count = 0; count < cityCount - 1; ++count){

        int lowestPrice = INT_MAX;
        int minIndex = 0;
        for (int i = 0; i < cityCount; ++i){
            if (price[i] <= lowestPrice && visited[i] == 0){
                lowestPrice = price[i];
                minIndex = i;
            }
        }

        visited[minIndex] = 1;
        int ind = 0;
        for (int j = 0; j < cityCount; ++j){

            if (!visited[j] && matrix[minIndex][j] != -1 && price[minIndex] != INT_MAX
                && price[minIndex] + matrix[minIndex][j] < price[j]){

                    price[j] = price[minIndex] + matrix[minIndex][j];

                    for (int i = 0; i < cityCount; ++i){
                        if (paths[minIndex][i] != -1){
                             paths[j][ind] = paths[minIndex][i];
                             ++ind;
                        }
                    }
                    paths[j][cityCount - 1] = minIndex;
            }
        }
    }

    for (int i = 0; i < cityCount; ++i){
        path[i] = paths[dest][i];
    }

    shiftLoop(cityCount, path);
    path[cityCount-1] = dest;
    shiftLoop(cityCount, path);

    if (price[dest] != INT_MAX){
        return price[dest];
    }else{
        return -1;
    }

}

void readData(int matrix[][20], char cities[][30], int *cityCount){
    FILE *flightData;
    flightData = fopen(DATA_FILE, "r");

    for (int i = 0; i < 20; ++i){
        for (int j = 0; j < 20; ++j){
            matrix[i][j] = -1;
        }
    }

    *cityCount = 0;

    if (flightData){

        for (int i = 0, j = 1; !feof(flightData); i+=2, j+=2){
            char str[30];

            fscanf(flightData, "%s", str);

            int indexi = isInArray(str, cities, *cityCount);
            if (indexi == -1){
                strcpy(cities[i], str);
                ++*cityCount;
                indexi = i;
            }else{
                --j;
                --i;
            }

            fscanf(flightData, "%s", str);
            int price = 0;
            fscanf(flightData, "%d", &price);

            int indexj = isInArray(str, cities, *cityCount);
            if (indexj == -1){
                strcpy(cities[j], str);
                ++*cityCount;
                indexj = j;
            }else{
                --i;
                --j;
            }

            matrix[indexi][indexj] = price;
            matrix[indexj][indexi] = price;
        }
    }

    fclose(flightData);
}

int isInArray(char str[], char array[][30],  int cityCount){

    for (int i = 0; i < cityCount; ++i){
        if (strcmp(array[i], str) == 0){
             return i;
        }
    }
    return -1;
}

void shiftLoop(int n, int array[]){
    for (int i = 0; i < n - 1; ++i){
        if (array[i] == -1){
            array[i] = array[i+1];
            array[i+1] = -1;
        }
    }
}
