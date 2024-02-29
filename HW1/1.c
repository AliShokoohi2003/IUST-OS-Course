#include <stdio.h>
#include <string.h>

typedef struct {
    char name[20];
    int age;
    int points;
} Student;

typedef struct{
    char name1[20];
    int age1;
    int points1;
    char name2[20];
    int age2;
    int points2;
    int points;   
    }Student1;
    Student1 students1[50];
void swap(Student *a, Student *b) {
    Student temp = *a;
    *a = *b;
    *b = temp;
}
void swap1(Student1 *a, Student1 *b) {
    Student1 temp = *a;
    *a = *b;
    *b = temp;
}

void sortStudents(Student students[], int n) {
    Student min = students[n-1];
    int index = n - 1;
        for (int j = 0; j < n - 1; j++) {
            if (students[j].points < students[j + 1].points) {
                if (students[j].points < min.points)
                {
                    min = students[j];
                    index = j;
                }
                //swap(&students[j], &students[j + 1]);
            }
        }
        while ((index + 1) < n)
        {
            swap(&students[index], &students[index+1]);
            index ++;
        }
        
    }
void sortStudents1(Student1 students1[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (students1[j].points < students1[j + 1].points) {
                swap1(&students1[j], &students1[j + 1]);
            }
        }
    }
}

int main() {
    int n;
    scanf("%d", &n);

    Student students[100];
    for (int i = 0; i < n; i++) {
        scanf("%s %d %d", students[i].name, &students[i].age, &students[i].points);
    }
    
    

    int start = 0;
    int end;
    if (n % 2 == 0) {
        end = n - 1;
    } else {
        sortStudents(students, n);
        end = n - 2;
        n -= 1;
    }

    int i = 0;
    while (start < end) {
        strcpy(students1[i].name1, students[start].name);
        strcpy(students1[i].name2, students[end].name);
        students1[i].age1 = students[start].age;
        students1[i].age2 = students[end].age;
        students1[i].points1 = students[start].points;
        students1[i].points2 = students[end].points;
        students1[i].points = students1[i].points1 + students1[i].points2;
        //printf("%s %d %d, %s %d %d\n", students[start].name, students[start].age, students[start].points, students[end].name, students[end].age, students[end].points);
        start++;
        end--;
        i++;
    }
    
    n = n / 2;
    sortStudents1(students1,n);
    n *= 2;
    n = n - (n % 2);
    for (int i = 0; i < n - 2 ;i++)
    {
        if (students1[i].points1)
        {
        printf("%s %d %d, %s %d %d\n", students1[i].name1, students1[i].age1, students1[i].points1, students1[i].name2, students1[i].age2, students1[i].points2);
    }}



    return 0;
}