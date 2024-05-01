#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef WIN32
#include <stdint.h>
#define CLS system("cls")
#else
#define CLS system("clear")
//#define CLS
#endif
#define PRINT_LINE puts("+-----+----------------------+-----------------+-------+---------+--------+---------+----------+");

/* start define bool type */
typedef uint8_t bool;
enum BOOL
{
    false, true
};
/* end define bool type */

enum
{
    MAX_STR_IN_FILE_LEN = 200,
    MAX_MODEL_NAME_LEN = 30,
    MAX_FILENAME_LEN = 20
};

typedef struct Brand
{
    char *name;
    struct Brand *next;
} Brand;

typedef struct Brands
{
//    int size;
    Brand *first_brand;
} Brands;

typedef struct Smartphone
{
    char *model;             /* Модель */
    Brand *brand;           /* Марка */
    int ram;                 /* Объем оперативной памяти, ГБ */
    int memory;              /* Объем постоянной памяти, ГБ */
    float screen_size;       /* Размер экрана, дюймы */
    float weight;            /* Вес, граммы */
    float price;             /* Цена, доллары */

    int index;               /* индекс */
    struct Smartphone *next; /* указатель на следующий элемент списка */
    struct Smartphone *prev; /* указатель на предыдущий элемент списка */
} Smartphone;

typedef struct Storage
{
    int size;
    bool saved;
    Smartphone *first_pos;
    Smartphone *last_pos;
} Storage;

typedef float(Getters)(Smartphone *smartphone);

typedef char *(StrGetters)(Smartphone *smartphone);

/*--------- Define function-----------*/

Storage *create_storage();

Brands *create_brands();

FILE *open_file();

void fill_storage(Storage *storage, FILE *source, Brands *brands);

Smartphone *create_position(int index);

void set_values(Smartphone *smartphone, char *str, Brands *brands);

void set_brand(Smartphone *smartphone, char *brand, Brands *brands);

void add_first(Storage *storage, Smartphone *new_position);

void add(Storage *storage, Smartphone *cur_position, Smartphone *new_position);

void menu(Storage *storage, Brands *brands, FILE *file);

// Function to display help information
void help();

void print_table(Storage *storage);

void print_header();

void print(Smartphone *smartphone);

void get_string(char *string, int max_len);

//void get_values_from_console(Smartphone *position, char *brand);

void insert_selected(Storage *storage, Smartphone *new, int index);

void delete_selected(Storage *storage, int index);

void change_position(Storage *storage, int index, Brands *brands);

int find(Storage *storage, Smartphone **arr, Getters get, float search_val);

int find_str(Storage *storage, Smartphone **arr, StrGetters str_get, char *search_str);

void sorting(Smartphone **arr, int b, int e, bool is_str, Getters get, StrGetters str_get);

/* getters */
char *get_model(Smartphone *smartphone);

char *get_brand(Smartphone *smartphone);

float get_ram(Smartphone *smartphone);

float get_memory(Smartphone *smartphone);

float get_screen_size(Smartphone *smartphone);

float get_weight(Smartphone *smartphone);

float get_price(Smartphone *smartphone);

/* ------ */

void print_opt();

void swap(Smartphone **a, Smartphone **b);

void save_storage(Storage *storage, FILE *file);

void delete_position(Smartphone *position);

void delete_brands(Brands *brand);

void delete_storage(Storage *storage);

/*--------- Define function-----------*/

int main()
{
    Storage *Market = NULL;
    Brands *brands = NULL;
    FILE *file;
    file = open_file();
    Market = create_storage();
    brands = create_brands();
    if (Market && brands)
    {
        fill_storage(Market, file, brands);
        if (Market->size >= 0)
            menu(Market, brands, file);

        /*----destroy structs----*/
        delete_storage(Market);
        delete_brands(brands);
    }
    return 0;
}

Storage *create_storage()
{
    Storage *storage;
    storage = malloc(sizeof(Storage));
    if (storage)
    {
        storage->size = 0;
        storage->saved = false;
        storage->first_pos = NULL;
        storage->last_pos = NULL;
    }
    return storage;
}

Brands *create_brands()
{
    Brands *brands;
    brands = malloc(sizeof(Brands));
    if (brands)
    {
//        brands->size = 0;
        brands->first_brand = NULL;
    }
    return brands;
}

FILE *open_file()
{
    char filename[MAX_FILENAME_LEN];
    char create;
    FILE *file;
    size_t len;
    printf("Input filename: ");
    fgets(filename, MAX_FILENAME_LEN, stdin);
    len = strlen(filename);
    filename[len - 1] = '\0';
    file = fopen(filename, "r+");
    if (!file)
    {
        printf("File not exist!\nDo you want create file?(y/n): ");
        create = (char) getchar();
        if (create == 'y' || create == 'Y') file = fopen(filename, "w");
    }
    return file;
}

void fill_storage(Storage *storage, FILE *source, Brands *brands)
{
    Smartphone *new_pos;
    char tmp_str[MAX_STR_IN_FILE_LEN];
    int i;
    if (storage != NULL)
    {
        if (source != NULL)
        {
            // проходимся по файлу
            for (i = 1; fgets(tmp_str, MAX_STR_IN_FILE_LEN, source); i++)
            {
                new_pos = create_position(i);
                if (new_pos)
                {
                    set_values(new_pos, tmp_str, brands);
                    add(storage, storage->last_pos, new_pos);
                }
            }
            storage->saved = true;
        } else
        {
            storage->size = -1;
            printf("Check the existence of the file\n");
        }
    }
}

Smartphone *create_position(int index)
{
    Smartphone *position = NULL;
    position = (Smartphone *) malloc(sizeof(Smartphone));
    if (position)
    {
        position->index = index;
        position->next = NULL;
    }
    return position;
}

void set_values(Smartphone *smartphone, char *str, Brands *brands)
{
    char *brand_name;
    smartphone->model = malloc(sizeof(char) * MAX_MODEL_NAME_LEN);
    smartphone->brand = NULL;
    brand_name = malloc(sizeof(char) * MAX_MODEL_NAME_LEN);
    if (str != NULL)
        sscanf(str, "%[^,],%[^,],%d,%d,%f,%f,%f", smartphone->model, brand_name,
               &(smartphone->ram), &(smartphone->memory), &(smartphone->screen_size),
               &(smartphone->weight), &(smartphone->price));
    else
    {
        printf("input model: ");
        get_string(smartphone->model, MAX_MODEL_NAME_LEN);
        printf("input brand: ");
        get_string(brand_name, MAX_MODEL_NAME_LEN);
        printf("input ram: ");
        scanf("%d", &(smartphone->ram));
        printf("input memory: ");
        scanf("%d", &(smartphone->memory));
        printf("input screen size: ");
        scanf("%f", &(smartphone->screen_size));
        printf("input weight: ");
        scanf("%f", &(smartphone->weight));
        printf("input price: ");
        scanf("%f", &(smartphone->price));

    }
    set_brand(smartphone, brand_name, brands);
}

void set_brand(Smartphone *smartphone, char *brand, Brands *brands)
{
    Brand *cur_brand, *prev_brand = NULL;
    cur_brand = brands->first_brand;

    while (cur_brand != NULL)
    {
        if (!strcmp(brand, cur_brand->name))
            smartphone->brand = cur_brand;
        prev_brand = cur_brand;
        cur_brand = cur_brand->next;
    }
    if (smartphone->brand == NULL)
    {
        cur_brand = malloc(sizeof(Brand));
        cur_brand->name = brand;
        cur_brand->next = NULL;
        if (prev_brand)
            prev_brand->next = cur_brand;
        else
            brands->first_brand = cur_brand;
        smartphone->brand = cur_brand;
    } else free(brand);
}

void add(Storage *storage, Smartphone *cur_position, Smartphone *new_position)
{
    //вставка в конец
    if (storage)
    {
        storage->size++;
        if (!cur_position)
        {
            add_first(storage, new_position);
        } else if (new_position)
        {
            if (cur_position->next == NULL)
            {
                cur_position->next = new_position;
                new_position->prev = cur_position;
                storage->last_pos = new_position;
            } else
            {
                new_position->next = cur_position->next;
                cur_position->next = new_position;
                new_position->prev = cur_position;
            }
        }
    }
}

void add_first(Storage *storage, Smartphone *new_position)
{
    if (storage && new_position)
    {
        storage->first_pos = new_position;
        storage->last_pos = new_position;
    }
}

void menu(Storage *storage, Brands *brands, FILE *file)
{
    int val;
    int i;
    char search_string[MAX_MODEL_NAME_LEN];
    float search_val;
    Smartphone *position;
    Smartphone **arr;
    Getters *getters[5];
    StrGetters *str_getters[2];

    str_getters[0] = get_model;
    str_getters[1] = get_brand;
    getters[0] = get_ram;
    getters[1] = get_memory;
    getters[2] = get_screen_size;
    getters[3] = get_weight;
    getters[4] = get_price;
    do
    {
        CLS;
        if (storage->saved) puts("\x1b[1;32mSaved\x1b[0m");
        else puts("\x1b[1;4;31mDon't saved\x1b[0m");
        puts("There are some options:");
        puts("1 - for HELP");
        puts("2 - for add position into Market");
        puts("3 - for insert position into Market");
        puts("4 - for remove position from Market");
        puts("5 - for change position into Market");
        puts("6 - for find position");
        puts("7 - for sort storage");
        puts("8 - for print storage");
        puts("9 - for save storage");
        puts("0 - for EXIT program");
        puts("\nEnter option: ");
        scanf("%d", &val);
        getchar();
        switch (val)
        {
            case 1:
            {
                CLS;
                help();
            }
                break;
            case 2:
            {
                position = create_position(storage->size + 1);
                if (position)
                {
                    set_values(position, NULL, brands);
                    add(storage, storage->last_pos, position);
                }
                storage->saved = false;
            }
                break;
            case 3:
            {
                puts("input index: ");
                scanf("%i", &val);
                getchar();
                position = create_position(val);
                if (position)
                {
                    set_values(position, NULL, brands);
                    insert_selected(storage, position, val);
                }
                storage->saved = false;
            }
                break;
            case 4:
            {
                CLS;
                if (storage->size == 0) printf("Market is empty\n");
                else
                {
                    print_table(storage);
                    puts("input index: ");
                    scanf("%i", &val);
                    getchar();
                    delete_selected(storage, val);
                    storage->saved = false;
                }
            }
                break;
            case 5:
            {
                puts("input index: ");
                scanf("%i", &val);
                getchar();
                change_position(storage, val, brands);
                storage->saved = false;
            }
                break;
            case 6:
            {
                puts("select the parameter to be searched for");
                print_opt();
                puts("input index: ");
                scanf("%i", &val);
                arr = malloc(storage->size * sizeof(Smartphone *));
                if (arr)
                {
                    for (i = 0, position = storage->first_pos; i < storage->size; i++, position = position->next)
                        arr[i] = position;
                    if (val > 0 && val < 3)
                    {
                        puts("input search string:");
                        getchar();
                        get_string(search_string, MAX_MODEL_NAME_LEN);
                        val = find_str(storage, arr, str_getters[val - 1], search_string);
                    } else if (val > 0)
                    {
                        puts("input search value:");
                        scanf("%f", &search_val);
                        val = find(storage, arr, getters[val - 3], search_val);
                    }
                    /*print table from arr*/
                    print_header();
                    for (i = 0; i < val; i++) print(arr[i]);
                    PRINT_LINE
                    getchar();
                }
                free(arr);
            }
                break;
            case 7:
            {
                puts("select the parameter by which the sorting will be performed");
                print_opt();
                puts("input index: ");
                scanf("%i", &val);
                arr = malloc(storage->size * sizeof(Smartphone *));
                if (arr)
                {
                    for (i = 0, position = storage->first_pos; i < storage->size; i++, position = position->next)
                        arr[i] = position;
                    if (val > 0 && val < 3)
                        sorting(arr, 0, storage->size - 1, true, NULL, str_getters[val - 1]);
                    else if (val > 0)
                        sorting(arr, 0, storage->size - 1, false, getters[val - 3], NULL);
                    /*print table from arr*/
                    print_header();
                    for (i = 0; i < storage->size; i++) print(arr[i]);
                    getchar();
                }
                free(arr);
            }
                break;
            case 8:
            {
                CLS;
                print_table(storage);
                printf("size: %d\n", storage->size);
            }
                break;
            case 9:
            {
                CLS;
                save_storage(storage, file);
            }
                break;
            case 0:
            {
                getchar();
            }
                break;
            default:
            {
                puts("Incorrect key!");
                getchar();
            }

        }
        puts("Press ENTER to continue");
        getchar();
    } while (val != 0);
}

void help()
{
    printf("help\n");
}

void print_table(Storage *storage)
{
    Smartphone *cur;
    cur = storage->first_pos;
    print_header();
    while (cur != NULL)
    {
        print(cur);
        cur = cur->next;
    }
    PRINT_LINE
}

void print_header()
{
    PRINT_LINE
    printf("| %-5s | %-20s | %-15s | %-5s | %-5s | %-6s | %-7s | %-8s |\n",
           "№", "Model", "Brand", "RAM", "Storage", "Screen", "Weight", "Price");
    PRINT_LINE
}

void print(Smartphone *smartphone)
{
    printf("| %3i | %-20s | %-15s | %-3dGB | %-5dGB | %-5.2f\" | %-6.2fg | $%-7.2f |\n",
           smartphone->index, smartphone->model, smartphone->brand->name, smartphone->ram, smartphone->memory,
           smartphone->screen_size, smartphone->weight, smartphone->price);
}

void insert_selected(Storage *storage, Smartphone *new, int index)
{
    Smartphone *cur;
    int found;

    found = 0;
    if (storage->size == 0)
    {
        add_first(storage, new);
        storage->size++;
    } else if (storage->last_pos->index < index) // storage->size < index
    {
        cur = storage->last_pos;
//        new->index = storage->size + 1;
        add(storage, cur, new);
    } else
    {
        cur = storage->first_pos;
        while (cur)
        {
            if ((cur->next) && (cur->index < index && cur->next->index > index))
            {
                add(storage, cur, new);
                cur = NULL;
            } else if ((cur->next) && (cur->next->index == index))
            {
                add(storage, cur, new);
                cur = cur->next->next;
                cur->index++;
                cur = cur->next;
                found = 1;
            } else
            {
                if (!found) cur = cur->next;
                else
                {
                    cur->index++;
                    if (!(cur->next) || (cur->next && cur->next->index - cur->index > 1)) cur = NULL;
                    else cur = cur->next;
                }
            }
        }
    }
}

void delete_selected(Storage *storage, int index)
{
    Smartphone *cur, *tmp;
    cur = storage->first_pos;

    // ищем что удалить
    if (storage->first_pos->index == index)
    {
        storage->first_pos = cur->next;
        if (cur->next) cur->next->prev = NULL;
        delete_position(cur);
        storage->size--;
    } else if (storage->size == 1 && cur->index == index)
    {
        tmp = cur;
        storage->first_pos = NULL;
        storage->last_pos = NULL;
        storage->size = 0;
        free(tmp);
        printf("Market is empty\n");
    } else if (storage->last_pos->index == index)
    {
        cur = storage->last_pos;
        storage->last_pos = cur->prev;
        storage->last_pos->next = NULL;
        delete_position(cur);
        storage->size--;
    } else if (storage->last_pos->index < index || index < 1)
    {
        printf("\x1b[1;31mindex is out of list\x1b[0m\n");
    } else
    {
        while (cur != NULL)
        {
            if (cur->next->index == index)
            {
                tmp = cur->next;
                cur->next = tmp->next;
                tmp->next->prev = cur;
                delete_position(tmp);
                cur = NULL;
                storage->size--;
            } else
                cur = cur->next;
        }
    }
}

void change_position(Storage *storage, int index, Brands *brands)
{
    Smartphone *item;
    char *brand_name;

    brand_name = malloc(sizeof(char) * MAX_MODEL_NAME_LEN);
    item = storage->first_pos;

    while (item->index != index) item = item->next;

    printf("\x1b[1;33mOld model name: %s\x1b[0m\n", item->model);
    printf("\x1b[1;36mNew model name: ");
    get_string(item->model, MAX_MODEL_NAME_LEN);

    printf("\x1b[1;33mOld brand: %s\x1b[0m\n", item->brand->name);
    printf("\x1b[1;36minput brand: ");
    get_string(brand_name, MAX_MODEL_NAME_LEN);
    set_brand(item, brand_name, brands);

    printf("\x1b[1;33mOld ram: %i\x1b[0m\n", item->ram);
    printf("\x1b[1;36minput ram: ");
    scanf("%d", &(item->ram));

    printf("\x1b[1;33mOld memory: %i\x1b[0m\n", item->memory);
    printf("\x1b[1;36minput memory: ");
    scanf("%d", &(item->memory));

    printf("\x1b[1;33mOld screen size: %.2f\x1b[0m\n", item->screen_size);
    printf("\x1b[1;36minput screen size: ");
    scanf("%f", &(item->screen_size));

    printf("\x1b[1;33mOld weight: %.2f\x1b[0m\n", item->weight);
    printf("\x1b[1;36minput weight: ");
    scanf("%f", &(item->weight));

    printf("\x1b[1;33mOld price: %.2f\x1b[0m\n", item->price);
    printf("\x1b[1;36minput price: ");
    scanf("%f", &(item->price));

}

void print_opt()
{
    puts("1 - model name\n"
         "2 - brand name\n"
         "3 - RAM size\n"
         "4 - memory size\n"
         "5 - screen size\n"
         "6 - weight\n"
         "7 - price");
}

int find(Storage *storage, Smartphone **arr, Getters get, float search_val)
{
    Smartphone *cur;
    int i;
    cur = storage->first_pos;
    i = 0;
    while (cur != NULL)
    {
        if (search_val == get(cur))
        {
            arr[i] = cur;
            i++;
        }
        cur = cur->next;
    }
    return i;
}

int find_str(Storage *storage, Smartphone **arr, StrGetters str_get, char *search_str)
{
    Smartphone *cur;
    int i;
    cur = storage->first_pos;
    i = 0;
    while (cur != NULL)
    {
        if ((strcmp(str_get(cur), search_str) == 0))
        {
            arr[i] = cur;
            i++;
        }
        cur = cur->next;
    }
    return i;
}

void sorting(Smartphone **arr, int b, int e, bool is_str, Getters get, StrGetters str_get)
{
    int left, right;
    float middle;
    char *str_middle;

    left = b;
    right = e;
    if (is_str)
        str_middle = str_get(arr[(left + right) / 2]);
    else
        middle = get(arr[(left + right) / 2]); /* It's a reference element */
    while (left <= right)
    {
        if (is_str)
        {
            while (strcmp(str_get(arr[left]), str_middle) < 0) left++;
            while (strcmp(str_get(arr[right]), str_middle) > 0) right--;
        } else
        {
            while (get(arr[left]) < middle) left++;
            while (get(arr[right]) > middle) right--;
        }
        if (left <= right) swap(&arr[left++], &arr[right--]);
    }
    if (b < right) sorting(arr, b, right, is_str, get, str_get);
    if (e > left) sorting(arr, left, e, is_str, get, str_get);
}

void swap(Smartphone **a, Smartphone **b)
{
    Smartphone *c;
    c = *a;
    *a = *b;
    *b = c;
}

/* getters */
char *get_model(Smartphone *smartphone)
{
    return smartphone->model;
}

char *get_brand(Smartphone *smartphone)
{
    return smartphone->brand->name;
}

float get_ram(Smartphone *smartphone)
{
    return (float) smartphone->ram;
}

float get_memory(Smartphone *smartphone)
{
    return (float) smartphone->memory;
}

float get_screen_size(Smartphone *smartphone)
{
    return (float) smartphone->screen_size;
}

float get_weight(Smartphone *smartphone)
{
    return (float) smartphone->weight;
}

float get_price(Smartphone *smartphone)
{
    return (float) smartphone->price;
}

/* ------ */

void get_string(char *string, int max_len)
{
    size_t len;
    if (string)
    {
        fgets(string, max_len, stdin);
        len = strlen(string);
        string[len - 1] = '\0';
    }
}

void save_storage(Storage *storage, FILE *file)
{
    if (!file) printf("☹️\n");
    else
    {
        rewind(file);
        Smartphone *cur = storage->first_pos;
        while (cur != NULL)
        {
            fprintf(file, "%s,%s,%d,%d,%.2f,%.2f,%.2f\n", cur->model, cur->brand->name,
                    cur->ram, cur->memory, cur->screen_size,
                    cur->weight, cur->price);
            cur = cur->next;
        }
        storage->saved = true;
        printf("Storage saved successfully!\n");
    }
}

void delete_position(Smartphone *position)
{
    free(position->model);
    position->next = NULL;
    position->prev = NULL;
    free(position);
}

void delete_brands(Brands *brands)
{
    Brand *cur_brand, *tmp_brand;

    cur_brand = brands->first_brand;
    while (cur_brand)
    {
        free(cur_brand->name);
        tmp_brand = cur_brand;
        cur_brand = cur_brand->next;
        free(tmp_brand);
    }
    free(brands);
}

void delete_storage(Storage *storage)
{
    Smartphone *cur, *next;
    cur = storage->first_pos;
    while (cur != NULL)
    {
        next = cur->next;
        delete_position(cur);
        cur = next;
    }
    free(storage);
}