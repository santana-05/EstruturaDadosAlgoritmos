// Maurício Santana e Daniel Santana

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 500
#define MAX_POINTS 300

typedef struct {
    int x;
    int y;
} Point;


float distanceFromOrigin(Point p);
float distanceBetweenPoints(Point a, Point b);
int parsePoints(const char *input, Point points[]);
int comparePoints(const void *a, const void *b);
void writeResults(FILE *output, Point sortedPoints[], int count, float pathDistance, float shortcut);
void processInputFile(FILE *input, FILE *output);

int main() {
    FILE *input = fopen("L0Q1.in", "r");
    FILE *output = fopen("L0Q1.out", "w");

    if (!input || !output) {
        perror("Erro ao abrir arquivos");
        return EXIT_FAILURE;
    }

    processInputFile(input, output);

    fclose(input);
    fclose(output);
    return EXIT_SUCCESS;
}


float distanceFromOrigin(Point p) {
    return sqrtf(p.x * p.x + p.y * p.y);
}


float distanceBetweenPoints(Point a, Point b) {
    int dx = a.x - b.x;
    int dy = a.y - b.y;
    return sqrtf(dx * dx + dy * dy);
}


int parsePoints(const char *input, Point points[]) {
    int count = 0;
    const char *ptr = input;
    
    while (*ptr) {
        
        while (isspace(*ptr)) ptr++;
        
        if (*ptr == '(') {
            Point p;
            int charsRead;
            
            
            if (sscanf(ptr, "(%d,%d)%n", &p.x, &p.y, &charsRead) == 2 ||
                sscanf(ptr, " ( %d , %d ) %n", &p.x, &p.y, &charsRead) == 2) {
                
                points[count++] = p;
                ptr += charsRead;
                continue;
            }
        }
        ptr++;
    }
    
    return count;
}


int comparePoints(const void *a, const void *b) {
    const Point *p1 = (const Point *)a;
    const Point *p2 = (const Point *)b;
    
    float d1 = distanceFromOrigin(*p1);
    float d2 = distanceFromOrigin(*p2);
    
    if (d1 < d2) return -1;
    if (d1 > d2) return 1;
    return 0;
}


void writeResults(FILE *output, Point sortedPoints[], int count, float pathDistance, float shortcut) {
    fprintf(output, "points");
    for (int i = 0; i < count; i++) {
        fprintf(output, " (%d,%d)", sortedPoints[i].x, sortedPoints[i].y);
    }
    fprintf(output, " distance %.2f shortcut %.2f\n", pathDistance, shortcut);
}


void processInputFile(FILE *input, FILE *output) {
    char line[MAX_LINE_LENGTH];
    
    while (fgets(line, sizeof(line), input)) {
        
        line[strcspn(line, "\r\n")] = '\0';
        
        
        int isEmpty = 1;
        for (char *ptr = line; *ptr; ptr++) {
            if (!isspace(*ptr)) {
                isEmpty = 0;
                break;
            }
        }
        
        if (isEmpty) continue;
        
        Point points[MAX_POINTS];
        Point sortedPoints[MAX_POINTS];
        
        int pointCount = parsePoints(line, points);
        
        if (pointCount == 0) {
            fprintf(output, "points distance 0.00 shortcut 0.00\n");
            continue;
        }
        
        
        memcpy(sortedPoints, points, pointCount * sizeof(Point));
        qsort(sortedPoints, pointCount, sizeof(Point), comparePoints);
        
        
        float totalDistance = 0;
        for (int i = 0; i < pointCount - 1; i++) {
            totalDistance += distanceBetweenPoints(points[i], points[i + 1]);
        }
        
        
        float shortcut = distanceBetweenPoints(points[0], points[pointCount - 1]);
        
        writeResults(output, sortedPoints, pointCount, totalDistance, shortcut);
    }
}