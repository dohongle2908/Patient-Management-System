#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_PATIENTS 100
#define MAX_NAME 50

// dinh nghia muc do uu tien
typedef enum {
	CAP_CUU = 1,
	THONG_THUONG = 2,
	TAI_KHAM = 3,
} priority;

// ham chuyen muc do uu tien thanh chuoi
const char* priorityToString(int p) {
    switch(p) {
        case 1: return "CAP CUU";
        case 2: return "THONG THUONG";
        case 3: return "TAI KHAM";
        default: return "KHONG XAC DINH";
    }
}
// Ham chuyen trang thai kham thanh chuoi
const char* stateToString(char c) {
    if (c == 'X') return "DA KHAM";
    if (c == 'C') return "CHO KHAM";
    return "KHONG XAC DINH";
}

// cau truc luu tru thong tin benh nhan 
typedef struct {
	int id;                     // ma benh nhan 
	char name[MAX_NAME];        // ten benh nhan
	char birthday[11];          // ngay sinh 
	char gender[5];            // gioi tinh: Nam , Nu, Khac
	char time[10];              // thoi gian den
	int number;                 // stt den 
	int priority;               // muc do uu tien
	char state;                 // trang thai kham: X la da kham. C la cho kham
} patient;

// mang luu tru benh nhan v√† bien luu chi so so benh nhan da kham v√† tong so benh nhan 
patient allPatient[MAX_PATIENTS];
int totalCount = 0;
int examinedCount = 0;

 // ham kiem tra ma benh nhan trung
 int check_id(patient allPatient[], int *totalCount,int id){
 	for(int i = 0; i < *totalCount; i++){
 		if(allPatient[i].id == id)
 			return 1; // trung ma
	 }
	 return 0;
 }
 
 // ham xu li khi nhap loi
 int retry(const char* message){
 	char choice[3];
    while (1) {
        printf("%s\n", message);
        printf("1. Thu lai\n");
        printf("2. Thoat\n");
        printf("Lua chon: ");
        fgets(choice, sizeof(choice), stdin);
        if (choice[0] == '1' || choice[0] == '2') return choice[0] - '0';
        printf("Lua chon khong hop le. Vui long nhap lai.\n");
 }
}

// ham kiem tra chuoi chi cÛ khoang trang 
int is_blank(const char *str) {
    while (*str) {
        if (!isspace((unsigned char)*str)) return 0;
        str++;
    }
    return 1;
}

 // ham them benh nhan 
 void addPatient(patient allPatient[], int *totalCount){
 	if (*totalCount >= MAX_PATIENTS){
 		printf("Danh sach benh nhan da day - qua tai benh nhan.\n");
 		return;
	 }
	 patient p;
	 char buffer[100];
	 // nhap ma 
	 while (1){
	 	printf("Nhap ma benh nhan: ");
	 	fgets(buffer, sizeof(buffer), stdin);
	 	p.id = atoi(buffer);
	 	if (p.id == 0){
	 		if (retry("ma benh nhan khong duoc de trong.") == 2)
	 			return;
	 		continue;
		 }
		 if (check_id(allPatient, totalCount,p.id)){
		 	if (retry("ma benh nhan da ton tai.") == 2) 
			 	return;
		 	continue;
		 }
		 break;
	 }
	 
	 // nhap ho ten
	 while (1) {
        printf("Nhap ho ten benh nhan: ");
        fgets(p.name, sizeof(p.name), stdin);
        p.name[strcspn(p.name, "\n")] = 0;
        if (is_blank(p.name)){
            if (retry("Ho ten khong duoc de trong.") == 2)
                return;
            continue;
        }
        break;
    }
    
    // nhap ngay sinh 
    while (1) {
        printf("Nhap ngay sinh (dd/mm/yyyy): ");
        fgets(p.birthday, sizeof(p.birthday), stdin);
        p.birthday[strcspn(p.birthday, "\n")] = '\0';
        if (strlen(p.birthday) != 10) {
            if (retry("Ngay sinh khong dung dinh dang (dd/mm/yyyy).") == 2)
				 return;
            continue;
        }
        break;
    }
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    
    // nhap gioi tinh
    while (1) {
        printf("Nhap gioi tinh (Nam/Nu/Khac): ");
        fgets(p.gender, sizeof(p.gender), stdin);
        p.gender[strcspn(p.gender, "\n")] = '\0';
        if (is_blank(p.gender)) {
            if (retry("Gioi tinh khong duoc de trong.") == 2) 
				return;
            continue;
        }
        break;
    }
    
    // Nhap muc do uu tien
    while (1) {
        printf("Nhap muc do uu tien (1: Cap cuu, 2: Thong thuong, 3: Tai kham): ");
        fgets(buffer, sizeof(buffer), stdin);
        p.priority = atoi(buffer);
        if (p.priority < 1 || p.priority > 3) {
            if (retry("Muc do uu tien khong hop le.") == 2) 
                return;
            continue;
        }
        break;
    }
        // thoi gian
		time_t rawtime;
		struct tm * timeinfo;

		time(&rawtime);
		timeinfo = localtime(&rawtime);
		strftime(p.time, sizeof(p.time), "%H:%M", timeinfo);
		 //stt
		p.number = *totalCount + 1;
		p.state = 'C';

	// them vao list
	allPatient[*totalCount] = p;
	(*totalCount)++;
	
	printf("Them benh nhan thanh cong!\n");

 	}

 
 /*ham so sanh benh nhan : tra ve 1 neu p1 truoc p2 l√† dung
 							tra ve 2 neu p1 truoc p2 la sai */
int comparePatient(patient p1, patient p2){
	if (p1.priority < p2.priority)
		return 1; 
	if (p1.priority == p2.priority){
		return p1.number < p2.number;
	}
	return 0;
}
 
 // ham sap xep benh nhan moi them 
void insertLastPatientByPriority(patient allPatient[], int totalCount) {
    int i = totalCount;
    patient key = allPatient[i-1];
    int j = i - 2;

    // sap xep benh nhan moi them vao dung cho 
    while (j >= examinedCount && allPatient[j].state == 'C' &&
           comparePatient(allPatient[j], key) == 0){
        allPatient[j + 1] = allPatient[j];
        j--;
    }

    allPatient[j + 1] = key;
}

// Ham hien thi thong tin benh nhan
void printPatient(patient p) {
    printf("%-5d %-20s %-12s %-8s %-8s %-6d %-12s %-6s\n",
                   p.id,
                   p.name,
                   p.birthday,
                   p.gender,
                   p.time,
                   p.number,
                   priorityToString(p.priority),
                   stateToString(p.state));
}

// ham goi benh nhan 
void callPatient(patient allPatient[], int *examinedCount, int totalCount){ 
	if (*examinedCount == totalCount){
		printf("Khong con benh nhan nao cho kham.\n");
		return;
	} 
	printf("Moi benh nhan vao kham:\n");
	printPatient(allPatient[*examinedCount]);
	
	allPatient[*examinedCount].state = 'X';
	(*examinedCount)++;
	
	printf("Cap nhat trang thai benh nhan thanh cong!");
}

// ham in danh sach benh nhan ra man hinh theo trang thai 
void printPatientsByState(patient allPatient[], int totalCount) {
    char choice;
    char stateFilter;
    int found;

    do {
        printf("\n===== MENU IN DANH SACH BENH NHAN =====\n");
        printf("1. Danh sach benh nhan CHO KHAM\n");
        printf("2. Danh sach benh nhan DA KHAM\n");
        printf("3. Danh sach TAT CA benh nhan\n");
        printf("0. Quay lai\n");
        printf("Chon chuc nang (0-3): ");
        fflush(stdin);
        choice = getchar();

        switch (choice) {
            case '1':
                stateFilter = 'C';
                printf("\nDANH SACH BENH NHAN CHO KHAM:\n");
                break;
            case '2':
                stateFilter = 'X';
                printf("\nDANH SACH BENH NHAN DA KHAM:\n");
                break;
            case '3':
                stateFilter = 'A';
                printf("\nDANH SACH TAT CA BENH NHAN:\n");
                break;
            case '0':
                printf("Quay lai menu chinh.\n");
                return;
            default:
                printf("Lua chon khong hop le. Vui long chon lai.\n");
                continue;
        }

        printf("%-5s %-20s %-12s %-8s %-8s %-6s %-12s %-6s\n",
               "ID", "Name", "Birthday", "Gender", "Time", "No", "Priority", "State");
        printf("-------------------------------------------------------------------------------\n");

        found = 0;
        for (int i = 0; i < totalCount; i++) {
            if (stateFilter == 'A' || allPatient[i].state == stateFilter) {
                printPatient(allPatient[i]);
                found = 1;
            }
        }

        if (!found) {
            if (stateFilter == 'A')
                printf("Khong co benh nhan nao trong danh sach.\n");
            else
                printf("Khong co benh nhan nao co trang thai '%s'.\n", stateToString(stateFilter));
        }

    } while (choice != '0');
}


// ham tim kiem theo ma benh nhan
int searchByID(patient allPatient[], int totalCount, int id){
	for (int  i = 0; i < totalCount; i++){
		if (allPatient[i].id == id){
			printPatient(allPatient[i]);
			return i;
		}
	}
	return -1;
}

// ham chuyen thanh chu thuong
void toLower(char* str) {
    for (int i = 0; str[i]; i++) {
        if ('A' <= str[i] && str[i] <= 'Z') str[i] += 32;
    }
}

// Ham kiem tra chuoi con khong phan biet hoa thuong
int containsIgnoreCase(const char *text, const char *pattern) {
    char textLower[100], patternLower[100];
    strcpy(textLower, text);
    strcpy(patternLower, pattern);
    toLower(textLower);
    toLower(patternLower);
    return strstr(textLower, patternLower) != NULL;
}

// ham tim kiem theo ten 
int searchByName(patient allPatient[], int totalCount, const char* keyword){
    int found = 0;
    printf("Ket qua tim kiem:\n");
    for (int i = 0; i < totalCount; i++) {
        if (containsIgnoreCase(allPatient[i].name, keyword)) {
            printPatient(allPatient[i]);
            found = 1;
        }
    }
    if (!found) {
        printf("Khong tim thay benh nhan.\n");
        return -1;
    }
    return 1; 
}


// Ham tim kiem benh nhan 
void searchPatient(patient allPatient[], int totalCount){
	int choice; 
	char input[50];
	
	while (1){
		printf("Tim kiem benh nhan:\n");
		printf("1. Theo ma benh nhan\n");
		printf("2. Theo ho ten\n");
		printf("Nhap lua chon: ");
		fgets(input, sizeof(input), stdin);
		choice = atoi(input);
		
		if (choice == 1 || choice == 2) break;
		if (retry("Lua chon khong hop le. Vui long nhap 1 hoac 2.") == 2)
			return;
	}
	
	if (choice == 1){
		while (1){
			printf("Nhap ma benh nhan: ");
			fgets(input, sizeof(input), stdin);
			int id = atoi(input);
			if (id == 0){
				if (retry("Ma benh nhan khong hop le.") == 2) return ;
				continue;
			}
			if (searchByID(allPatient, totalCount, id) == -1){
				if (retry("Khong tim thay benh nhan.") == 2) return;
				continue;
			}
			break;
		}
	} else {
		while (1) {
			printf("Nhap ho ten: ");
			fgets(input, sizeof(input), stdin);
			input[strcspn(input, "\n")] = 0; // xoa ki tu newline
			
		if (strlen(input) == 0) {
                if (retry("Ten khong duoc de trong.") == 2) return;
                continue; 
            }
            if (searchByName(allPatient, totalCount, input) == -1) {
                if (retry("Khong tim thay benh nhan.") == 2) return;
                continue;
            }
            break;	
		}
	}
}

// Ham cap nhat trang thai benh nhan 
void updatePatientState(patient allPatient[], int totalCount, int* examinedCount) {
    int id;
    printf("Nhap ID benh nhan can cap nhat trang thai: ");
    scanf("%d", &id);

	printf("\n--- Thong tin hien tai ---\n");
	    
    int index = searchByID(allPatient, totalCount, id);

    if (index == -1) {
        printf("Khong tim thay benh nhan co ID %d.\n", id);
        return;
    }

    patient temp = allPatient[index];
    char currentState = temp.state;

    int newState;
    printf("\nNhap trang thai moi (0 - Cho kham, 1 - Da kham): ");
    scanf("%d", &newState);

    if (newState != 0 && newState != 1) {
        printf("Trang thai khong hop le. Cap nhat that bai.\n");
        return;
    }

    char newCharState = (newState == 1) ? 'X' : 'C';

    if (newCharState == currentState) {
        printf("Trang thai khong thay doi.\n");
        return;
    }

    if (newCharState == 'X') {
        // Chuyen tu cho kham sang da kham
        for (int i = index; i < totalCount - 1; i++) {
            allPatient[i] = allPatient[i + 1];
        }

        temp.state = 'X';

        for (int i = totalCount-1; i > *examinedCount; i--) {
            allPatient[i] = allPatient[i - 1];
        }
        allPatient[*examinedCount] = temp;
        (*examinedCount)++;
    } else {
        // Chuyen tu da kham sang cho kham
        for (int i = index; i < totalCount -1; i++) {
            allPatient[i] = allPatient[i + 1];
        }
        (*examinedCount)--;

        temp.state = 'C';
        allPatient[totalCount - 1] = temp;
        insertLastPatientByPriority(allPatient, totalCount);
    }

    printf("Cap nhat trang thai va vi tri thanh cong.\n");
}

// Ham them tung benh nhan tu file vao hang cho
int addPatientManual(patient allPatient[], int *totalCount, patient p) {
    if (*totalCount >= MAX_PATIENTS) return 0;
    if (check_id(allPatient, totalCount, p.id)) return -1;

    p.number = *totalCount + 1;
    p.state = 'C';
    allPatient[*totalCount] = p;
    (*totalCount)++;
    return 1;
}

// ham doc du lieu tu file
int readPatientsFromFile(const char *filename, patient allPatient[], int *totalCount) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Khong the mo file: %s\n", filename);
        return 0;
    }

    char line[256];
    int lineCount = 0;

    while (fgets(line, sizeof(line), file)) {
        lineCount++;

        // Bo dong trang
        if (strlen(line) <= 1) continue;

        patient p;
        char *token;
        
        // Tach va kiem tra tung truong
        token = strtok(line, "|");
        if (!token || (p.id = atoi(token)) <= 0) {
            printf("Du lieu dong %d khong dung dinh dang.\n", lineCount);
            continue;
        }

        token = strtok(NULL, "|");
        if (!token || strlen(token) == 0) {
            printf("Du lieu dong %d khong dung dinh dang.\n", lineCount);
            continue;
        }
        strncpy(p.name, token, sizeof(p.name));

        token = strtok(NULL, "|");
        if (!token || strlen(token) != 10) {
            printf("Du lieu dong %d khong dung dinh dang.\n", lineCount);
            continue;
        }
        strncpy(p.birthday, token, sizeof(p.birthday));

        token = strtok(NULL, "|");
        if (!token || strlen(token) == 0) {
            printf("Du lieu dong %d khong dung dinh dang.\n", lineCount);
            continue;
        }
        strncpy(p.gender, token, sizeof(p.gender));

        token = strtok(NULL, "|");
        if (!token || strlen(token) == 0) {
            printf("Du lieu dong %d khong dung dinh dang.\n", lineCount);
            continue;
        }
        strncpy(p.time, token, sizeof(p.time));

        token = strtok(NULL, "|");
        if (!token || (p.priority = atoi(token)) < 1 || p.priority > 3) {
            printf("Du lieu dong %d khong dung dinh dang.\n", lineCount);
            continue;
        }

        // Them benh nhan v‡o hang cho
        int result = addPatientManual(allPatient, totalCount, p);
        if (result == -1) {
            printf("Dong %d: Trung ma benh nhan (%d).\n", lineCount, p.id);
        } else if (result == 0) {
            printf("Danh sach da day. Ngung them du lieu.\n");
            break;
        }
    }

    fclose(file);
    printf("Da doc xong du lieu. Tong so benh nhan : %d\n", *totalCount);
    return 1;
}


// ham sap xep benh nhan cho kham (thuat toan SelectionSort)
void sortPatientsByPriority(patient allPatient[], int examinedCount, int totalCount) {
    if (examinedCount >= totalCount) {
        printf("Khong co benh nhan chua duoc kham de sap xep.\n");
        return;
    }

    for (int i = examinedCount; i < totalCount - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < totalCount; j++) {
            if (
                allPatient[j].priority < allPatient[minIndex].priority ||
                (allPatient[j].priority == allPatient[minIndex].priority &&
                 allPatient[j].number < allPatient[minIndex].number)
            ) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            patient temp = allPatient[i];
            allPatient[i] = allPatient[minIndex];
            allPatient[minIndex] = temp;
        }
    }
    printf("Da sap xep cac benh nhan chua duoc kham theo muc do uu tien va thu tu den.\n");
}


// ham in danh sach benh nhan ra file 
int writePatientsToTextFile(const char *filename, patient allPatient[], int totalCount) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Khong the ghi file: %s\n", filename);
        return 0;
    }

    // Ghi tieu de cot
    fprintf(file, "%-6s %-25s %-12s %-8s %-6s %-6s %-12s %-8s\n",
            "ID", "Name", "Birthday", "Gender", "Time", "No", "Priority", "State");
    fprintf(file, "-------------------------------------------------------------------------------\n");

    // Ghi tung dong du lieu
    for (int i = 0; i < totalCount; i++) {
        fprintf(file, "%-6d %-25s %-12s %-8s %-6s %-6d %-12s %-8s\n",
                allPatient[i].id,
                allPatient[i].name,
                allPatient[i].birthday,
                allPatient[i].gender,
                allPatient[i].time,
                allPatient[i].number,
                priorityToString(allPatient[i].priority),
                stateToString(allPatient[i].state));
    }

    fclose(file);
    return 1;
}

// ham menu
int menu() {
    int choice;
    printf("\n================= MENU QUAN LY BENH NHAN =================\n");
    printf("1. Them benh nhan\n");
    printf("2. Doc danh sach tu file\n");
    printf("3. In danh sach benh nhan ra man hinh\n");
    printf("4. Cap nhat trang thai cua benh nhan.\n");
    printf("5. Tim kiem benh nhan theo ID hoac ten\n");
    printf("6. Sap xep benh nhan theo muc do uu tien\n");
    printf("7. Ghi danh sach ra file (TXT hoac CSV)\n");
    printf("8. Goi benh nhan theo thu tu uu tien\n");
    printf("0. Thoat\n");
    printf("===========================================================\n");
    printf("Nhap lua chon cua ban: ");
    scanf("%d", &choice);
    return choice;
}

// ham main 
int main() {
    patient allPatient[MAX_PATIENTS];
    int totalCount = 0;
    int examinedCount = 0;
    int running = 1;

    while (running) {
        int choice = menu();
        getchar();
        
        switch (choice) {
            case 1:
                addPatient(allPatient, &totalCount);
                insertLastPatientByPriority(allPatient, totalCount);
                break;
            case 2:
                readPatientsFromFile("test_data.txt", allPatient, &totalCount);
                break;
            case 3:
                printPatientsByState(allPatient, totalCount);
                break;
            case 4:
            	updatePatientState(allPatient, totalCount, &examinedCount);
                break;
            case 5:
                searchPatient(allPatient, totalCount);
                break;
            case 6:
                sortPatientsByPriority(allPatient, examinedCount, totalCount);
                break;
            case 7:
                writePatientsToTextFile("output_data.txt", allPatient, totalCount);
                break;
            case 8:
                callPatient(allPatient, &examinedCount, totalCount);
                break;
            case 0:
                running = 0;
                printf("Ket thuc chuong trinh.\n");
                break;
            default:
                printf("Lua chon khong hop le. Vui long thu lai!\n");
        }
    }

    return 0;
}
