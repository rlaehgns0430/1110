#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

long long comp_count = 0;
int max_stack_depth = 0;
int current_stack_depth = 0;

void merge(int* arr, int l, int m, int r, int* temp) {
    int i = l;
    int j = m + 1;
    int k = l;
    while (i <= m && j <= r) {
        comp_count++;
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        }
        else {
            temp[k++] = arr[j++];
        }
    }
    while (i <= m) {
        temp[k++] = arr[i++];
    }
    while (j <= r) {
        temp[k++] = arr[j++];
    }
    for (i = l; i <= r; i++) {
        arr[i] = temp[i];
    }
}

void merge_sort_rec(int* arr, int l, int r, int* temp) {
    current_stack_depth++;
    if (current_stack_depth > max_stack_depth) {
        max_stack_depth = current_stack_depth;
    }
    if (l < r) {
        int m = l + (r - l) / 2;
        merge_sort_rec(arr, l, m, temp);
        merge_sort_rec(arr, m + 1, r, temp);
        merge(arr, l, m, r, temp);
    }
    current_stack_depth--;
}

void swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

int partition_first(int* arr, int low, int high) {
    int pivot = arr[low];
    int i = low + 1;
    for (int j = low + 1; j <= high; j++) {
        comp_count++;
        if (arr[j] < pivot) {
            swap(&arr[i], &arr[j]);
            i++;
        }
    }
    swap(&arr[low], &arr[i - 1]);
    return i - 1;
}

void quick_sort_first(int* arr, int low, int high) {
    current_stack_depth++;
    if (current_stack_depth > max_stack_depth) {
        max_stack_depth = current_stack_depth;
    }
    if (low < high) {
        int p = partition_first(arr, low, high);
        quick_sort_first(arr, low, p - 1);
        quick_sort_first(arr, p + 1, high);
    }
    current_stack_depth--;
}

int partition_random(int* arr, int low, int high) {
    int p_idx = low + rand() % (high - low + 1);
    swap(&arr[low], &arr[p_idx]);
    int pivot = arr[low];
    int i = low + 1;
    for (int j = low + 1; j <= high; j++) {
        comp_count++;
        if (arr[j] < pivot) {
            swap(&arr[i], &arr[j]);
            i++;
        }
    }
    swap(&arr[low], &arr[i - 1]);
    return i - 1;
}

void quick_sort_random(int* arr, int low, int high) {
    current_stack_depth++;
    if (current_stack_depth > max_stack_depth) {
        max_stack_depth = current_stack_depth;
    }
    if (low < high) {
        int p = partition_random(arr, low, high);
        quick_sort_random(arr, low, p - 1);
        quick_sort_random(arr, p + 1, high);
    }
    current_stack_depth--;
}

int partition_median(int* arr, int low, int high) {
    int mid = low + (high - low) / 2;
    comp_count++;
    if (arr[low] > arr[mid]) swap(&arr[low], &arr[mid]);
    comp_count++;
    if (arr[low] > arr[high]) swap(&arr[low], &arr[high]);
    comp_count++;
    if (arr[mid] > arr[high]) swap(&arr[mid], &arr[high]);
    swap(&arr[low], &arr[mid]);
    int pivot = arr[low];
    int i = low + 1;
    for (int j = low + 1; j <= high; j++) {
        comp_count++;
        if (arr[j] < pivot) {
            swap(&arr[i], &arr[j]);
            i++;
        }
    }
    swap(&arr[low], &arr[i - 1]);
    return i - 1;
}

void quick_sort_median(int* arr, int low, int high) {
    current_stack_depth++;
    if (current_stack_depth > max_stack_depth) {
        max_stack_depth = current_stack_depth;
    }
    if (low < high) {
        int p = partition_median(arr, low, high);
        quick_sort_median(arr, low, p - 1);
        quick_sort_median(arr, p + 1, high);
    }
    current_stack_depth--;
}

int is_sorted(int* arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) return 0;
    }
    return 1;
}

int main() {
    srand((unsigned int)time(NULL));
    int sizes[] = { 1000, 2000, 4000, 8000, 16000, 20000 };
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    for (int s = 0; s < num_sizes; s++) {
        int n = sizes[s];
        int* orig = (int*)malloc(sizeof(int) * n);
        int* arr = (int*)malloc(sizeof(int) * n);
        int* temp = (int*)malloc(sizeof(int) * n);
        for (int i = 0; i < n; i++) {
            orig[i] = rand() % 100000;
        }
        printf("Data Size : %d\n\n", n);
        printf("Algorithm       Comparisons    Time(ms)    Memory(Bytes)\n");
        printf("--------------------------------------------------------\n");

        memcpy(arr, orig, sizeof(int) * n);
        comp_count = 0;
        max_stack_depth = 0;
        current_stack_depth = 0;
        clock_t start = clock();
        merge_sort_rec(arr, 0, n - 1, temp);
        clock_t end = clock();
        double t_merge = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;
        long long mem_merge = (sizeof(int) * n) + (max_stack_depth * sizeof(void*) * 4);
        if (is_sorted(arr, n)) {
            printf("Merge Sort      %-14lld %-11.2f %-lld\n", comp_count, t_merge, mem_merge);
        }

        memcpy(arr, orig, sizeof(int) * n);
        comp_count = 0;
        max_stack_depth = 0;
        current_stack_depth = 0;
        start = clock();
        quick_sort_first(arr, 0, n - 1);
        end = clock();
        double t_qf = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;
        long long mem_qf = max_stack_depth * sizeof(void*) * 3;
        if (is_sorted(arr, n)) {
            printf("Quick (First)   %-14lld %-11.2f %-lld\n", comp_count, t_qf, mem_qf);
        }

        memcpy(arr, orig, sizeof(int) * n);
        comp_count = 0;
        max_stack_depth = 0;
        current_stack_depth = 0;
        start = clock();
        quick_sort_random(arr, 0, n - 1);
        end = clock();
        double t_qr = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;
        long long mem_qr = max_stack_depth * sizeof(void*) * 3;
        if (is_sorted(arr, n)) {
            printf("Quick (Random)  %-14lld %-11.2f %-lld\n", comp_count, t_qr, mem_qr);
        }

        memcpy(arr, orig, sizeof(int) * n);
        comp_count = 0;
        max_stack_depth = 0;
        current_stack_depth = 0;
        start = clock();
        quick_sort_median(arr, 0, n - 1);
        end = clock();
        double t_qm = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;
        long long mem_qm = max_stack_depth * sizeof(void*) * 3;
        if (is_sorted(arr, n)) {
            printf("Quick (Median3) %-14lld %-11.2f %-lld\n", comp_count, t_qm, mem_qm);
        }
        printf("\n");
        free(orig);
        free(arr);
        free(temp);
    }
    return 0;
}