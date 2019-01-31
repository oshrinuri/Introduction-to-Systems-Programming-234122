#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

typedef void* Element;
typedef bool (*cmpFunction)(Element, Element);
typedef Element (*copyFunction)(Element);
typedef void (*destroyFunction)(Element);

bool intCmp(Element a, Element b) {
    assert (a && b);
    return (*(int *) a) < (*(int *) b);
}

bool intCmp2(Element a, Element b) {
    return (int)a < (int)b;
}

bool doubleCmp(Element a, Element b) {
    assert (a && b);
    return (*(double *) a) < (*(double *) b);
}

bool stringCmp(Element a, Element b) {
    assert (a && b);
    return (strcmp((char*)a,(char*)b) < 0)?true:false;
}

Element stringCopy(Element str) {
    char* copy = malloc(1+strlen((char*)str));
    strcpy(copy,(char*)str);
    return copy;
}

void stringDestroy(Element str) {
    free(str);
}

static void printArrINT(int **arr, int n) {
    for (int i = 0; i < n; ++i) {
        printf("%d ", *arr[i]);
    }
}
static void printArrINT2(int* arr, int n) {
    for (int i = 0; i < n; ++i) {
        printf("%d ", arr[i]);
    }
}
static void printArrDOUBLE(double **arr, int n) {
    for (int i = 0; i < n; ++i) {
        printf("%lf ", *arr[i]);
    }
}

static void printArrSTRING(char **arr, int n) {
    for (int i = 0; i < n; ++i) {
        printf("%s ", arr[i]);
    }
}

static Element copyInt(Element element) {
    int* temp = malloc(sizeof(int*));
    *temp = *(int*)element;
    return temp;
}

static void destInt(Element element) {
    free(element);
}

static void merge(Element* arr, Element* leftArr, int leftCount, Element* rightArr,
                  int rightCount, cmpFunction compare, copyFunction copyFunc) {
    int i, j, k;
    i = 0; j = 0; k = 0;

    while (i < leftCount && j < rightCount) {
        if (compare(leftArr[i], rightArr[j])) {
            arr[k++] = copyFunc(leftArr[i++]);
        } else {
            arr[k++] = copyFunc(rightArr[j++]);
        }
    }
    while (i < leftCount) {
        arr[k++] = copyFunc(leftArr[i++]);
    }
    while (j < rightCount) {
        arr[k++] = copyFunc(rightArr[j++]);
    }
}

void mergeSort(void **arr, int n, cmpFunction compare, copyFunction copyFunc, destroyFunction destroyFunc) {
    if (n < 2) return;
    int mid = n / 2;
    Element* leftArr = malloc(mid * sizeof(Element));
    Element* rightArr = malloc((n - mid)*sizeof(Element));
    for (int i = 0; i < mid; ++i) {
        leftArr[i] = copyFunc(arr[i]);
    }
    for (int i = mid; i < n; ++i) {
        rightArr[i - mid] = copyFunc(arr[i]);
    }
    mergeSort(leftArr, mid, compare, copyFunc, destroyFunc);
    mergeSort(rightArr, n - mid, compare, copyFunc, destroyFunc);
    merge(arr, leftArr, mid, rightArr, n - mid, compare, copyFunc);
    destroyFunc(leftArr);
    destroyFunc(rightArr);
}

int main() {
//     int arr[10] = { 29, 64, 73, 34, 20, -1, 0, 100, -1250, 12 };
//     printArrINT2(arr,10);
//    // array of int*
//    int *arr[10];
//    for (int i = 0; i < 10; ++i) {
//        arr[i] = malloc(sizeof(int));
//        *arr[i] = 10 - i;
//    }
//    printf("Array of int pointers:\n");
//    printArrINT(arr, 10);
//    printf("\n");
//    mergeSort((Element*) arr, 10, intCmp,copyInt, destInt);
//    printArrINT(arr, 10);

//    printf("\n");
//    // array of double*
//    double *arr2[10];
//    for (int i = 0; i < 10; ++i) {
//        arr2[i] = malloc(sizeof(double));
//        *arr2[i] = 100.0 / (i + 1);
//    }
//    printf("Array of double pointers:\n");
//    printArrDOUBLE(arr2, 10);
//    printf("\n");
//    mergeSort((Element*) arr2, 10, doubleCmp);
//    printArrDOUBLE(arr2, 10);
//
//    printf("\n");
    // array of strings
//     char *arr3[5];
//     arr3[0] = "Zaftra";
//     arr3[1] = "Abba";
//     arr3[2] = "KazaM";
//     arr3[3] = "KAzaM";
//     arr3[4] = "Hello";
//     printf("Array of strings:\n");
//     printArrSTRING(arr3, 5);
//     printf("\n");
// //
//     mergeSort((Element*) arr3, 5, stringCmp, stringCopy, stringDestroy);
//     printArrSTRING(arr3, 5);
}