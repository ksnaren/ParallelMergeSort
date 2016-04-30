/**
 *  \file parallel-mergesort.cc
 *
 *  \brief Implement your parallel mergesort in this file.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <omp.h>
#include <iostream>
#include "sort.hh"

using namespace std;

int SIZE;
int insertPos(keytype *temp, keytype target, int low, int high) {

	high = max(low, high + 1);
	while (low < high) {
		int mid = (low + high) / 2;
		if (target <= temp[mid])
			high = mid;
		else
			low = mid + 1;
	}
	return high;
}

void pMerge(keytype *T, int p1, int r1, int p2, int r2, keytype *A, int p3, int depth) {

	int n1 = (r1 - p1 + 1);
	int n2 = (r2 - p2 + 1);
	if (n1 < n2) {
		swap(n1, n2);
		swap(p1, p2);
		swap(r1, r2);
	}

	if (depth <= 1) {

		while (p1 <= r1 && p2 <= r2) {
			if (T[p1] < T[p2]) {
				A[p3] = T[p1];
				p1++;
				p3++;
			} else {
				A[p3] = T[p2];
				p2++;
				p3++;
			}
		}

		while (p1 <= r1) {
			A[p3] = T[p1];
			p1++;
			p3++;
		}
		while (p2 <= r2) {
			A[p3] = T[p2];
			p2++;
			p3++;
		}
		return;
	}

	if (n1 == 0)
		return;
	else {
		int q1 = (p1 + r1) / 2;
		int q2 = insertPos(T, T[q1], p2, r2);
		int q3 = p3 + (q1 - p1) + (q2 - p2);
		A[q3] = T[q1];
		//cout << "Position: " << q3 << " filled with: " << T[q1] << endl;
#pragma omp task
		{
			pMerge(T, p1, q1 - 1, p2, q2 - 1, A, p3, depth/2);
		}
		pMerge(T, q1 + 1, r1, q2, r2, A, q3 + 1, depth/2);
#pragma omp taskwait
	}
}

static int compare(const void* a, const void* b) {
	keytype ka = *(const keytype *) a;
	keytype kb = *(const keytype *) b;
	if (ka < kb)
		return -1;
	else if (ka == kb)
		return 0;
	else
		return 1;
}

void pMergeSort(keytype *A, int p, int r, keytype *B, int s, int depth) {
	//printf( "Tid: %d\n", omp_get_thread_num());
	int n = (r - p + 1);
	if (n == 1)
		B[s] = A[p];
	else if (depth <= 1) {
		qsort(A + p, n, sizeof(keytype), compare);
		memcpy(B + s, A + p, n * sizeof(keytype));
	} else {
		keytype *T = newKeys(n);
		int q = (p + r) / 2;
		int qD = (q - p + 1);
#pragma omp task 
		{
			pMergeSort(A, p, q, T, 1, depth/2);
		}
		pMergeSort(A, q + 1, r, T, qD + 1, depth/2);
#pragma omp taskwait
		{
			//cout<<"Depth: "<<depth<<endl;
			pMerge(T, 1, qD, qD + 1, n, B, s, depth);
		}
		free(T);
	}
}

void parallelSort(int N, keytype *A) {

	keytype *B = newKeys(N);
	SIZE = N;
	int depth = omp_get_max_threads();
	omp_set_num_threads(depth);
#pragma omp parallel
	{
#pragma omp single nowait
		{
			pMergeSort(A, 0, N - 1, B, 0, depth);
			memcpy(A, B, N * sizeof(keytype));
		}
	}

	free(B);

}

/* eof */
