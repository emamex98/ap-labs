#include <stdio.h>
#include <fcntl.h>  
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define REPORT_FILE "packages_report.txt"
#define TABLE_SZ 1000

void analizeLog(char *logFile, char *report);

int main(int argc, char **argv) {

    if (argc < 2) {
	    printf("Usage:./pacman-analizer.o pacman.log\n");
	    return 1;
    }

    analizeLog(argv[1], REPORT_FILE);

    return 0;
}

/***    ENTRY TYPE DEF    ***/ 
typedef struct entry_t {
    char *key;
    char *install_d;
    char *update_d;
    char *removal_d;
    int update_n;
    struct entry_t *next;
} entry_t;

/***    TABLE TYPE DEF    ***/ 
typedef struct {
    entry_t **entries;
} hash_t;

/***    TABLE CONSTRUCTOR    ***/
hash_t *newHashTable(void){
    int i;

    // Alocates memory for table
    hash_t *hashTable = malloc(sizeof(hash_t) * 10);

    // Alocates memory for each entry
    hashTable->entries = malloc(sizeof(entry_t*) * TABLE_SZ);

    // Set all indexes to null
    for(i = 0; i < TABLE_SZ; i++) {
        hashTable->entries[i] = NULL;
    }

    return hashTable;
}

entry_t *addInstallPair(const char *key, const char *date) {

    // Allocate memory
    entry_t *entry = malloc(sizeof(entry) * 10);
    entry->key = malloc(strlen(key) + 1);
    entry->install_d = malloc(strlen(date) + 1);

    // Copy values
    strcpy(entry->key, key);
    strcpy(entry->install_d, date);

    entry->update_d = NULL;
    entry->removal_d = NULL;
    entry->update_n = 0;
    entry->next = NULL;

    return entry;
}

/***    HASH FUNCTION    ***/
unsigned int hash(const char *key) {
    unsigned long int val = 0;
    unsigned int keyLen = strlen(key);
    unsigned int i;

    for(i = 0; i < keyLen; i++) {
        val = val * 37 + key[i];
    }

    val = val % TABLE_SZ;
    return val;
}

/***    ADD ENTRY - Call when package is installed   ***/
int newPackage(hash_t *hashTable, const char *key, const char *date) {

    // Get hash value
    unsigned int index = hash(key);

    // Get current value in slot
    entry_t *entry = hashTable->entries[index];

    // Push entry if slot is empty
    if(entry == NULL) {
        hashTable->entries[index] = addInstallPair(key, date);
        return 1;
    }

    entry_t *prev;

    while (entry != NULL) {

        // Replace value if same key is equal
        if(strcmp(entry->key, key) == 0) {

            free(entry->install_d);

            entry->install_d = malloc(strlen(date) + 1);
            strcpy(entry->install_d, date);

            free(entry->update_d);
            free(entry->removal_d);

            entry->update_d = NULL;
            entry->removal_d = NULL;
            entry->update_n = 0;

            return 0;
        }

        // Walk to next item in linked list
        prev = entry;
        entry = prev->next;
    }

    prev->next = addInstallPair(key, date);
    return 1;
}

/***    UPDATE ENTRY - Call when package is upgraded   ***/
int updatePackage(hash_t *hashTable, const char *key, const char *date) {

    // Get hash value
    unsigned int index = hash(key);

    // Get current value in slot
    entry_t *entry = hashTable->entries[index];

    if(entry == NULL) {
        return 0;
    }

    entry_t *prev;
    char *uDate;

    while (entry != NULL) {

        // Replace value if same key is equal
        if(strcmp(entry->key, key) == 0) {

            if(entry->update_d != NULL) {
                free(entry->update_d);
            }

            uDate = entry->update_d = malloc(strlen(date) + 1);
            strcpy(uDate, date);
            entry->update_n++;

            if(entry->update_n > 1)
                return 0;
            else
                return 1;
        }

        // Walk to next item in linked list
        prev = entry;
        entry = prev->next;
    }

    return 0;
}

/***    REMOVE ENTRY - Call when package is removed   ***/
int removePackage(hash_t *hashTable, const char *key, const char *date) {

    // Get hash value
    unsigned int index = hash(key);

    // Get current value in slot
    entry_t *entry = hashTable->entries[index];

    // Push entry if slot is empty
    if(entry == NULL) {
        return 0;
    }

    entry_t *prev;
    char *rDate;

    while (entry != NULL) {

        // Replace value if same key is equal
        if(strcmp(entry->key, key) == 0) {

            if(entry->removal_d != NULL) {
                return 0;
            }

            rDate = entry->removal_d = malloc(strlen(date) + 1);
            strcpy(rDate, date);
            return 1;
        }

        // Walk to next item in linked list
        prev = entry;
        entry = prev->next;
    }

    return 0;
}

/***    ITOA - Converts int to string   ***/
char* itoa(int val, int base) {
	static char buf[32] = {0};
	int i = 30;
	
	for(; val && i ; --i, val /= base)
		buf[i] = "0123456789abcdef"[val % base];
	
	return &buf[i+1];
}

/***    Write line to file - Converts int to string   ***/
void writeLine(int fd, char *str){
    write(fd, str, strlen(str)); 
}

void analizeLog(char *logFile, char *report) {

    int i = 0, j = 0, k = 0, m = 0;
    char c;
    char line[300], name[50], date[20];
    char *p, *q, *r;

    int installed = 0, removed = 0, upgraded = 0;
    int ret;

    hash_t *hashTable = newHashTable();

    // Creare File Descriptor
    int fdR = open(logFile, O_RDONLY);
    int fdW = open(report, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    // Return error if file is not found
    if (fdR < 0) { 
        printf("Error creating file read descripter. Check path and try again.\n");
        exit(1); 
    }

    if (fdW < 0) { 
        printf("Error creating file write descripter. Check path and try again.\n");
        exit(1); 
    }

    // Read file 
    while(read(fdR, &c, 1) == 1){

        // Detect end of line
        if(strncmp(&c, "\n", 1) == 0){

            // Detect commands
            p = strstr(line, "] installed"); 
            q = strstr(line, "] upgraded");
            r = strstr(line, "] removed");  

            // Handle installs
            if (p) {

                j = 12, k = 0;
                memset(&name[0], 0, sizeof(name));

                while(p[j] != ' '){
                    name[k] = p[j];
                    j++;
                    k++;
                }

                j = 2, k = 0;
                memset(&date[0], 0, sizeof(date));

                while(line[j] != ']'){
                    date[k] = line[j];
                    j++;
                    k++;
                }

                ret = newPackage(hashTable, name, date);
                installed = installed + ret;
            }

            // Handle upgrades
            else if (q){

                j = 11, k = 0;
                memset(&name[0], 0, sizeof(name));

                while(q[j] != ' '){
                    name[k] = q[j];
                    j++;
                    k++;
                }

                j = 2, k = 0;
                memset(&date[0], 0, sizeof(date));

                while(line[j] != ']'){
                    date[k] = line[j];
                    j++;
                    k++;
                }

                ret = updatePackage(hashTable, name, date);
                upgraded = upgraded + ret;
            }

            // Handle removals
            else if (r){

                j = 10, k = 0;
                memset(&name[0], 0, sizeof(name));

                while(r[j] != ' '){
                    name[k] = r[j];
                    j++;
                    k++;
                }

                j = 2, k = 0;
                memset(&date[0], 0, sizeof(date));

                while(line[j] != ']'){
                    date[k] = line[j];
                    j++;
                    k++;
                }

                ret = removePackage(hashTable, name, date);
                removed = removed + ret;
            }

            memset(&line[0], 0, sizeof(line));
            i = 0;
        }

        // Detect end of file
        else if(strncmp(&c, "\0", 1) == 0){
            break;
        }

        line[i] = c;
        i++;
    } 

    writeLine(fdW, "Pacman Packages Report\n----------------------");

    writeLine(fdW, "\n- Installed packages  : ");
    writeLine(fdW, itoa(installed, 10));

    writeLine(fdW, "\n- Removed packages    : ");
    writeLine(fdW, itoa(removed, 10));

    writeLine(fdW, "\n- Upgraded packages   : ");
    writeLine(fdW, itoa(upgraded, 10));

    writeLine(fdW, "\n- Currently installed : ");
    writeLine(fdW, itoa(installed-removed, 10));

    writeLine(fdW, "\n\nList of packages\n----------------");


    for (m = 0; m < TABLE_SZ; m++) {
        entry_t *entry = hashTable->entries[m];

        if (entry == NULL) {
            continue;
        }

        for(;;) {

            writeLine(fdW, "\n- Package Name        : ");
            writeLine(fdW, entry->key);

            writeLine(fdW, "\n  - Install date      : ");
            writeLine(fdW, entry->install_d);

            writeLine(fdW, "\n  - Last update date  : ");
            if(entry->update_d != NULL)
                writeLine(fdW, entry->update_d);
            else
                writeLine(fdW, "-");

            writeLine(fdW, "\n  - How many updates  : ");
            if(entry->update_n != 0)
                writeLine(fdW, itoa(entry->update_n, 10));
            else
                writeLine(fdW, "0");

            writeLine(fdW, "\n  - Removal date      : ");
            if(entry->removal_d != NULL)
                writeLine(fdW, entry->removal_d);
            else
                writeLine(fdW, "-");

            if (entry->next == NULL) {
                break;
            }

            entry = entry->next;
        }

    }

    // Close files once finished
    if (close(fdR) < 0 || close(fdW) < 0)  
    { 
        printf("Error closing files."); 
        exit(1); 
    }

    printf("Generating Report from: [%s] log file\n", logFile);
    printf("Report is generated at: [%s]\n", report);
}

